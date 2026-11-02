#!/usr/bin/env python3
"""Submission gate: reject matches that come from tricks rather than source.

Usage:
  python3 tools/gate.py [--base REF]     # check the branch's changes against REF (default: matching)
  python3 tools/gate.py --all-units      # only the report check, over every unit

Checks (see docs/PLAN.md, "Submission gate"):
  * changed C/C++ sources: no inline asm, no #pragma or goto that wasn't
    already in that file (or in the donor file of the same name), no m2c
    leftovers, no field access through casts and offsets, no .incbin;
  * protected files untouched: measuring tools, objdiff/report settings, tool
    versions, orig/;
  * symbols.txt: real (non-placeholder) symbols keep their address, size,
    type and name; placeholder symbols (fn_, lbl_, *_80xxxxxx) may be
    renamed, resized or split into the real symbols they cover;
  * no unit marked Matching contains a fakematch (FakeMatch headers,
    force_order dummies, "fakematch" comments), donor-imported or not;
  * every unit marked Matching is 100 % in code and data in the report
    (build/G4SE01/report.json must be current).

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
    "config/G4SE01/approved_symbol_changes.txt",
    "tools/gate.py", "tools/fdiff.py", "tools/progress.py", "tools/project.py",
    "tools/data_splits.py", "orig/",
]
PROTECTED_CONFIGURE = re.compile(r"functionRelocDiffs|progress_report_args|_tag\s*=|report|objdiff")
SOURCE = re.compile(r"\.(c|cpp|h|hpp|inc)$")
DONORS = Path("../donors")

BANNED = [
    (re.compile(r"\basm\b\s*[({]|__asm\b|\basm\s+\w+[^;]*\("), "inline asm"),
    (re.compile(r"\bregister\b[^;]*\basm\b"), "register bound to a machine register"),
    (re.compile(r"\bM2C_|\bsaved_reg_|\bunksp|\bvar_[rf]\d+\b|\bsp[0-9A-F]{2,}\b|\bMULT_HI\b|\?\s*/\*\s*unknown"), "m2c leftover"),
    (re.compile(r"\*\s*\(\s*[\w\s]+\*+\s*\)\s*\(\s*\(\s*(u8|s8|char|unsigned char|u32|uintptr_t)\s*\*?\s*\)"), "field access through a cast and an offset"),
    (re.compile(r"\.incbin|INCBIN"), "embedded binary"),
]
RESTRICTED = [
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


def donor_texts(path):
    name = Path(path).name
    if not DONORS.is_dir():
        return []
    texts = []
    for hit in DONORS.glob(f"*/**/{name}"):
        try:
            texts.append(hit.read_text(errors="replace"))
        except OSError:
            pass
    return texts


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
            if line[:1] in "+-" and not line.startswith(("+++", "---")) and PROTECTED_CONFIGURE.search(line):
                problems.append(f"configure.py: report or tool setting changed: {line}")


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
        if (size, kind) != (new_size, new_kind):
            problems.append(f"symbols.txt: {name} size/type changed ({size},{kind} -> {new_size},{new_kind})")
        if new_name != name and not PLACEHOLDER.search(name) and (name, name, new_name) not in approved:
            problems.append(f"symbols.txt: real name {name} renamed to {new_name}")


FAKEMATCH = re.compile(r"FakeMatch|force_order|FAKEMATCH|fakematch|NO_INLINE\(\)", re.I)


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


def check_report(problems):
    report = Path("build/G4SE01/report.json")
    if not report.exists():
        problems.append("build/G4SE01/report.json missing: run ninja build/G4SE01/report.json")
        return
    units = {u["name"]: u for u in json.loads(report.read_text())["units"]}
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        if not meta.get("complete"):
            continue
        m = (units.get(unit["name"]) or {}).get("measures", {})
        code = (int(m.get("matched_code", 0)), int(m.get("total_code", 0)))
        data = (int(m.get("matched_data", 0)), int(m.get("total_data", 0)))
        if code[0] != code[1] or data[0] != data[1]:
            problems.append(
                f"{meta.get('source_path', unit['name'])}: marked Matching but code {code[0]}/{code[1]}, data {data[0]}/{data[1]}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--base", default="matching")
    ap.add_argument("--all-units", action="store_true")
    args = ap.parse_args()
    problems = []
    if not args.all_units:
        # Compare against where the branch started, not the base's current tip.
        args.base = git("merge-base", args.base, "HEAD").strip()
        changed = set(git("diff", "--name-only", f"{args.base}...HEAD").split()) | set(git("diff", "--name-only", "HEAD").split())
        check_sources(args.base, sorted(changed), problems)
        check_protected(args.base, sorted(changed), problems)
        check_symbols(args.base, changed, problems)
    check_report(problems)
    check_fakematch(problems)
    for p in problems:
        print("FAIL", p)
    print("gate: ok" if not problems else f"gate: {len(problems)} problem(s)")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
