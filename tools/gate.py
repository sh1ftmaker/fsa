#!/usr/bin/env python3
"""Submission gate: reject matches that come from tricks rather than source.

Usage:
  python3 tools/gate.py [--base REF]     # check the branch's changes against REF (default: matching)
  python3 tools/gate.py --all-units      # only the report and link checks, over every unit
  python3 tools/gate.py --no-link        # skip the link (quick iteration only; merges link)

Checks (see docs/PLAN.md, "Submission gate"):
  * changed C/C++ sources: no inline asm, #pragma or goto that wasn't
    already in that file (or in the donor file of the same name), no m2c
    leftovers, no field access through casts and offsets, no .incbin;
  * protected files untouched: measuring tools, objdiff/report settings, tool
    versions, orig/;
  * symbols.txt: real (non-placeholder) symbols keep their address, size,
    type and name; placeholder symbols (fn_, lbl_, *_80xxxxxx) may be
    renamed, resized or split into the real symbols they cover;
  * no unit marked Matching contains a fakematch (FakeMatch headers,
    force_order dummies, "fakematch" comments), donor-imported or not;
  * every unit marked Matching is 100 % in code and data in the honest
    report (build/G4SE01/report_honest.json, every unit diffed; report.json
    counts Matching units as 100 % unseen);
  * main.dol links and its SHA-1 equals build.sha1 (ninja build/G4SE01/ok);
    if not, tools/link_check.py names the Matching units at fault.

The gate runs ninja itself (report_honest.json, then the link), so never run
it while another ninja runs in the same worktree.

Exit status 0 when everything passes.
"""

import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

APPROVALS = Path("config/G4SE01/approved_symbol_changes.txt")
PROTECTED = [
    "config/G4SE01/approved_symbol_changes.txt", "config/G4SE01/provenance_reviewed.txt",
    "tools/gate.py", "tools/fdiff.py", "tools/progress.py", "tools/project.py",
    "tools/honest_report.py", "tools/link_check.py",
    # The SHA-1 the link is checked against, and config.yml's hash: of the original DOL.
    "config/G4SE01/build.sha1", "config/G4SE01/config.yml",
    "tools/data_splits.py", "orig/",
]
PROTECTED_CONFIGURE = re.compile(r"functionRelocDiffs|progress_report_args|_tag\s*=|report|objdiff")
# Link and SHA-1 settings in configure.py (unit Object(...) lines, which carry
# FIXME(link) comments, are exempt).
LINK_CONFIGURE = re.compile(r"check_sha|sha1|ldflags|linker_version|link_order|ldscript|custom_build"
                            r"|non_matching|^[+-]\s*(Matching|NonMatching|Equivalent)\s*=")
SOURCE = re.compile(r"\.(c|cpp|h|hpp|inc)$")
DONORS = Path(__file__).resolve().parent.parent.parent / "donors"  # fsadecomp/donors, from any worktree

BANNED = [
    (re.compile(r"\bregister\b[^;]*\basm\b"), "register bound to a machine register"),
    (re.compile(r"\bM2C_|\bsaved_reg_|\bunksp|\bvar_[rf]\d+\b|\bsp[0-9A-F]{2,}\b|\bMULT_HI\b|\?\s*/\*\s*unknown"), "m2c leftover"),
    (re.compile(r"\.incbin|INCBIN"), "embedded binary"),
]
RESTRICTED = [
    # Donor library code (MSL alloc.c) does this in its matched source; new code may not.
    (re.compile(r"\*\s*\(\s*[\w\s]+\*+\s*\)\s*\(\s*\(\s*(u8|s8|char|unsigned char|u32|uintptr_t)\s*\*?\s*\)"), "field access through a cast and an offset"),
    # Original asm functions from a donor's matched source are source; new asm is not.
    (re.compile(r"\basm\b\s*[({]|__asm\b|\basm\s+\w+[^;]*\("), "inline asm"),
    (re.compile(r"^\s*#\s*pragma\b.*"), "#pragma"),
    (re.compile(r"\bgoto\s+\w+\s*;"), "goto"),
]
PLACEHOLDER = re.compile(r"^(fn_|lbl_|auto_|func_|jumptable_|@)|_80[0-9A-Fa-f]{6}$")
SYMBOL_LINE = re.compile(r"^(\S+) = (\S+):(0x[0-9A-Fa-f]+); // (.*)$")


def git(*args):
    return subprocess.run(["git", *args], capture_output=True, text=True, check=True).stdout


def added_lines(base, path):
    diff = git("diff", "-U0", f"{base}...HEAD", "--", path) + git("diff", "-U0", "HEAD", "--", path)
    return [line[1:] for line in diff.splitlines() if line.startswith("+") and not line.startswith("+++")]


def file_at(base, path):
    try:
        return git("show", f"{base}:{path}")
    except subprocess.CalledProcessError:
        return ""


# Only donors whose code may be copied count as a precedent: a LICENSE file at
# the top, and not MetroidPrimePort (its NOTICE excludes extern/sdk and the game).
UNUSABLE_DONORS = {"Odrannnn__MetroidPrimePort"}


def licensed_donors():
    for repo in sorted(DONORS.iterdir()):
        if repo.name in UNUSABLE_DONORS or not repo.is_dir():
            continue
        if any(p.name.upper().startswith(("LICENSE", "COPYING", "UNLICENSE")) for p in repo.iterdir()):
            yield repo


def donor_texts(path, suffix=False):
    """Same-named files in licensed donors (with suffix=True also e.g. dsp_GBAKey.c for GBAKey.c)."""
    name = Path(path).name
    if not DONORS.is_dir():
        return []
    texts = []
    pattern = f"**/*{name}" if suffix else f"**/{name}"
    for hit in (h for repo in licensed_donors() for h in repo.glob(pattern)):
        try:
            texts.append(hit.read_text(errors="replace"))
        except OSError:
            pass
    return texts


IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]{3,}")


COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


def strip_comments(text):
    return COMMENT.sub(" ", text)


INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
INCLUDE_ROOTS = ("include", "libs/sdk/include", "libs/sdk/msl_include")


def included_identifiers(path, text, skip=()):
    """Identifiers declared in the headers this file includes directly (the API it
    uses, which any independent writer shares). A header the same change added
    doesn't count: it could carry copied names."""
    ids = set()
    for inc in INCLUDE.findall(text):
        for root in (Path(path).parent, *map(Path, INCLUDE_ROOTS)):
            h = root / inc
            if h.is_file() and str(h) not in skip:
                ids |= set(IDENT.findall(strip_comments(h.read_text(errors="replace"))))
                break
    return ids


def unlicensed_texts(path):
    """Same-named files in donors whose code may not be copied."""
    name = Path(path).name
    if not DONORS.is_dir():
        return []
    ok = {r.name for r in licensed_donors()}
    texts = []
    for repo in sorted(DONORS.iterdir()):
        if repo.is_dir() and repo.name not in ok:
            for hit in repo.glob(f"**/{name}"):
                try:
                    texts.append((repo.name, hit.read_text(errors="replace")))
                except OSError:
                    pass
    return texts


def check_provenance(base, changed, problems):
    """A changed source whose distinctive identifiers (those no licensed donor's
    same-named file has) mostly come from an unlicensed donor's file was copied."""
    new_headers = {p for p in changed if p.endswith((".h", ".hpp")) and base and not file_at(base, p)}
    reviewed = Path("config/G4SE01/provenance_reviewed.txt")
    cleared = {l.split()[0] for l in reviewed.read_text().splitlines() if l.strip() and not l.startswith("#")} if reviewed.exists() else set()
    for path in changed:
        if not SOURCE.search(path) or not Path(path).exists() or path in cleared:
            continue
        text = Path(path).read_text(errors="replace")
        ours = set(IDENT.findall(strip_comments(text)))
        licensed = included_identifiers(path, text, skip=new_headers)
        for t in donor_texts(path, suffix=True):
            licensed |= set(IDENT.findall(t))
        distinctive = ours - licensed
        if len(distinctive) < 8:
            continue
        for repo, text in unlicensed_texts(path):
            shared = distinctive & set(IDENT.findall(strip_comments(text)))
            if len(shared) >= 8 and len(shared) >= 0.5 * len(distinctive):
                problems.append(f"{path}: {len(shared)}/{len(distinctive)} distinctive identifiers match "
                                f"unlicensed {repo}'s {Path(path).name}; write it from the asm and licensed donors")


def check_sources(base, changed, problems):
    for path in changed:
        if not SOURCE.search(path) or not Path(path).exists():
            continue
        old = file_at(base, path)
        donors = None
        for line in added_lines(base, path):
            code = line.split("//")[0]
            for pattern, what in BANNED:
                if pattern.search(code):
                    problems.append(f"{path}: {what}: {line.strip()}")
            for pattern, what in RESTRICTED:
                m = pattern.search(code)
                if not m:
                    continue
                text = m.group(0).strip()
                if text in old:
                    continue
                if donors is None:
                    donors = donor_texts(path)
                if any(text in d for d in donors):
                    continue
                problems.append(f"{path}: new {what} not in the file before or its donor: {line.strip()}")


def check_protected(base, changed, problems):
    for path in changed:
        if any(path == p or path.startswith(p) for p in PROTECTED if p.endswith("/")) or path in PROTECTED:
            problems.append(f"{path}: protected file changed")
    if "configure.py" in changed:
        diff = git("diff", "-U0", f"{base}...HEAD", "--", "configure.py") + git("diff", "-U0", "HEAD", "--", "configure.py")
        for line in diff.splitlines():
            if line[:1] not in "+-" or line.startswith(("+++", "---")):
                continue
            if PROTECTED_CONFIGURE.search(line):
                problems.append(f"configure.py: report or tool setting changed: {line}")
            elif "Object(" not in line and LINK_CONFIGURE.search(line):
                problems.append(f"configure.py: link or SHA-1 setting changed: {line}")


def parse_symbols(text):
    """Key by (section, address, n): several symbols (labels, functions) can share an address."""
    out, seen = {}, {}
    for line in text.splitlines():
        m = SYMBOL_LINE.match(line)
        if m:
            attrs = dict(a.split(":", 1) for a in m.group(4).split() if ":" in a)
            place = (m.group(2), int(m.group(3), 16))
            n = seen.get(place, 0)
            seen[place] = n + 1
            out[(*place, n)] = (m.group(1), attrs.get("size"), attrs.get("type"))
    return out


def check_symbols(base, changed, problems):
    path = "config/G4SE01/symbols.txt"
    if path not in changed:
        return
    approved = set()
    if APPROVALS.exists():
        for line in APPROVALS.read_text().splitlines():
            parts = line.split()
            if len(parts) >= 3 and not line.startswith("#"):
                approved.add((parts[0], parts[1], parts[2]))
    old = parse_symbols(file_at(base, path))
    new = parse_symbols(Path(path).read_text())
    for key, (name, size, kind) in old.items():
        if PLACEHOLDER.search(name):
            # dtk's guessed symbols may be split into the real ones or resized.
            continue
        if key not in new:
            problems.append(f"symbols.txt: symbol at {key[0]}:0x{key[1]:08X} ({name}) removed")
            continue
        new_name, new_size, new_kind = new[key]
        if (name, size, new_size) in approved and kind == new_kind:
            continue
        if (name, f"{size}/{kind}", f"{new_size}/{new_kind}") in approved:  # size and type, e.g. a label that is really a function
            continue
        if (size, kind) != (new_size, new_kind):
            problems.append(f"symbols.txt: {name} size/type changed ({size},{kind} -> {new_size},{new_kind})")
        if new_name != name and not PLACEHOLDER.search(name) and (name, name, new_name) not in approved:
            problems.append(f"symbols.txt: real name {name} renamed to {new_name}")


FAKEMATCH = re.compile(r"FakeMatch|force_order|FAKEMATCH|fakematch|NO_INLINE\(\)|CW_FORCE_(STRINGS|BSS)|fake_function", re.I)


def check_fakematch(problems):
    """A Matching unit may not rely on (or admit to) a fakematch, even one imported from a donor."""
    roots = ("libs/sdk/", "libs/sdk_gx/", "libs/jsystem/", "src/")
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        src = meta.get("source_path") or ""
        if meta.get("complete") and src.startswith(roots) and Path(src).exists():
            for n, line in enumerate(Path(src).read_text(errors="replace").splitlines(), 1):
                if FAKEMATCH.search(line):
                    problems.append(f"{src}:{n}: Matching unit with a fakematch: {line.strip()}")
                    break


def ninja(*targets):
    return subprocess.run(["timeout", "1800", "ninja", *targets], capture_output=True, text=True)


def check_report(problems):
    report = Path("build/G4SE01/report_honest.json")
    out = ninja(str(report))
    if out.returncode != 0:
        problems.append("ninja build/G4SE01/report_honest.json failed:\n" + (out.stdout + out.stderr)[-3000:])
        return
    units = {u["name"]: u for u in json.loads(report.read_text())["units"]}
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        if not meta.get("complete"):
            continue
        name = meta.get("source_path", unit["name"])
        if unit["name"] not in units:
            problems.append(f"{name}: marked Matching but missing from the honest report")
            continue
        m = units[unit["name"]].get("measures", {})
        code = (int(m.get("matched_code", 0)), int(m.get("total_code", 0)))
        data = (int(m.get("matched_data", 0)), int(m.get("total_data", 0)))
        if not code[1] and not data[1]:
            problems.append(f"{name}: marked Matching but has no code or data in the honest report")
        elif code[0] != code[1] or data[0] != data[1]:
            problems.append(
                f"{meta.get('source_path', unit['name'])}: marked Matching but code {code[0]}/{code[1]}, data {data[0]}/{data[1]}")


def check_link(problems):
    """Every Matching unit must link: main.dol's SHA-1 must equal build.sha1."""
    out = ninja("build/G4SE01/ok")
    if out.returncode == 0:
        return
    tail = (out.stdout + out.stderr)[-1500:]
    result = Path("build/link_check/result.json")
    result.unlink(missing_ok=True)
    subprocess.run(["timeout", "3600", sys.executable, "tools/link_check.py", "--json", str(result)],
                   capture_output=True, text=True)
    failing = [u for u in json.loads(result.read_text())["units"] if not u["identical"]] if result.exists() else []
    for u in failing:
        problems.append(f"{u['unit']}: marked Matching but doesn't link byte-identical ({u['reason']})")
    if not failing:
        problems.append("main.dol doesn't link or its SHA-1 differs, and tools/link_check.py "
                        "couldn't attribute it to a Matching unit:\n" + tail)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="matching")
    ap.add_argument("--all-units", action="store_true")
    ap.add_argument("--no-link", action="store_true", help="skip the link check (not for merges)")
    args = ap.parse_args()
    problems = []
    if not args.all_units:
        # Compare against where the branch started, not the base's current tip.
        args.base = git("merge-base", args.base, "HEAD").strip()
        changed = set(git("diff", "--name-only", f"{args.base}...HEAD").split()) | set(git("diff", "--name-only", "HEAD").split())
        check_sources(args.base, sorted(changed), problems)
        check_provenance(args.base, sorted(changed), problems)
        check_protected(args.base, sorted(changed), problems)
        check_symbols(args.base, changed, problems)
    check_report(problems)
    if not args.no_link:
        check_link(problems)
    check_fakematch(problems)
    for p in problems:
        print("FAIL", p)
    print("gate: ok" if not problems else f"gate: {len(problems)} problem(s)")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
