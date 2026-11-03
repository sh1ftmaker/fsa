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
  * every unit marked Matching has source and a compiled object (configure.py
    would otherwise link dtk's split of the original in its place) and is
    100 % in code and data in the honest report
    (build/G4SE01/report_honest.json, every unit diffed, no symbol_mappings);
  * objdiff.json has no symbol_mappings (objdiff's report honors them);
  * build/G4SE01/weakdedup_manifest.json: every extab entry the honest report
    left out belongs to a WEAK FUNC, and its map evidence (UNREFERENCED
    DUPLICATE in this object, extab and extabindex labels UNUSED) is in the
    map of a current link that includes this exact object and whose DOL
    matches build.sha1; every stripped object differs from the compiled one
    only by those entries, and no other unit is diffed against a copy;
    every dead-stripped data object it left out is listed UNUSED (with its
    size) for this object in that map, every allocated location referencing
    it lies in a function or object the map lists UNUSED for this object,
    only zero padding goes with it, and the map places every kept symbol of
    its section where the stripped copy has it;
  * main.dol links and its SHA-1 equals build.sha1 (ninja build/G4SE01/ok);
    if not, tools/link_check.py names the Matching units at fault.

The gate runs ninja itself (report_honest.json, then the link), so never run
it while another ninja runs in the same worktree.

Exit status 0 when everything passes.
"""

import argparse
import hashlib
import json
import struct
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
    # Build artifacts (maps, weak-dedup evidence, objects) are never committed.
    "build/",
]
PROTECTED_CONFIGURE = re.compile(r"functionRelocDiffs|progress_report_args|_tag\s*=|report|objdiff")
# Link and SHA-1 settings in configure.py (unit Object(...) lines, which carry
# FIXME(link) comments, are exempt).
LINK_CONFIGURE = re.compile(r"check_sha|sha1|ldflags|linker_version|link_order|ldscript|custom_build"
                            r"|non_matching|^[+-]\s*(Matching|NonMatching|Equivalent)\s*=")
SOURCE = re.compile(r"\.(c|cpp|h|hpp|inc)$")
def _donors_dir():
    """fsadecomp/donors, found through the main checkout (git's common dir), so
    the gate finds it from the main checkout and from every worktree's copy."""
    common = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"],
                            capture_output=True, text=True).stdout.strip()
    return Path(common).parent.parent / "donors" if common else Path(__file__).resolve().parent.parent.parent / "donors"


DONORS = _donors_dir()

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
# Placeholders: dtk's names, address-suffixed provisional names (also inside mangled names,
# e.g. init__18ActorBase_804A5770Fv), cls_<addr> classes and provisional game-unit __sinit names.
PLACEHOLDER = re.compile(r"^(fn_|lbl_|auto_|func_|jumptable_|@)|_80[0-9A-Fa-f]{6}(?![0-9a-z])|cls_[0-9A-Fa-f]{8}|__sinit_tu_[0-9A-Fa-f]{8}")
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
        sys.exit(f"gate: donors directory {DONORS} not found; the provenance check can't run")
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


def check_configure(problems):
    """tools/project.py names Matching objects it can't compile (no source) or
    that no split covers; configure.py then links the original in their place."""
    out = subprocess.run([sys.executable, "configure.py"], capture_output=True, text=True)
    if out.returncode != 0:
        problems.append("configure.py failed:\n" + (out.stdout + out.stderr)[-1500:])
    for line in out.stdout.splitlines():
        if line.startswith(("Matching object without source", "Matching object not in the split")):
            problems.append(f"{line.split(': ', 1)[-1]}: {line.split(':', 1)[0]}")


def check_report(problems):
    report = Path("build/G4SE01/report_honest.json")
    out = ninja(str(report))
    if out.returncode != 0:
        problems.append("ninja build/G4SE01/report_honest.json failed:\n" + (out.stdout + out.stderr)[-3000:])
        return
    units = {u["name"]: u for u in json.loads(report.read_text())["units"]}
    for unit in json.loads(Path("objdiff.json").read_text())["units"]:
        meta = unit.get("metadata") or {}
        if unit.get("symbol_mappings"):
            problems.append(f"{meta.get('source_path', unit['name'])}: objdiff.json has symbol_mappings "
                            f"(the report honors them); remove them")
        if not meta.get("complete"):
            continue
        name = meta.get("source_path", unit["name"])
        base = unit.get("base_path")
        if not base or not Path(base).is_file():
            problems.append(f"{name}: marked Matching but has no compiled object ({base or 'no base_path'})")
            continue
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
    check_weakdedup(problems)


class Elf:
    """Sections, symbols and relocations of a big-endian ELF32 object."""

    def __init__(self, path):
        d = self.d = Path(path).read_bytes()
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 2:
            raise ValueError(f"{path}: not a big-endian ELF32 object")
        self.ehdr = d[:0x34]
        shoff, = struct.unpack_from(">I", d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", d, 0x2E)
        self.sh = [struct.unpack_from(">IIIIIIIIII", d, shoff + i * shentsize) for i in range(shnum)]
        self.names = [self.cstr(self.sh[shstrndx][4] + h[0]) for h in self.sh]
        self.symtab = next(i for i, h in enumerate(self.sh) if h[1] == 2)
        strtab = self.sh[self.sh[self.symtab][6]][4]
        self.syms = []
        for k in range(self.sh[self.symtab][5] // 16):
            nm, val, size, info, other, shndx = struct.unpack_from(">IIIBBH", d, self.sh[self.symtab][4] + k * 16)
            self.syms.append((self.cstr(strtab + nm), val, size, info >> 4, info & 15, shndx, nm, other))

    def cstr(self, o):
        return self.d[o:self.d.index(0, o)].decode("latin-1")

    def data(self, i):
        h = self.sh[i]
        return self.d[h[4]:h[4] + h[5]] if h[1] != 8 else b""  # NOBITS: no bytes

    def rela(self, i):
        c = self.data(i)
        return [struct.unpack_from(">IIi", c, k * 12) for k in range(len(c) // 12)]

    def relocs(self, sec):
        out = []
        for i, h in enumerate(self.sh):
            if h[1] == 4 and h[7] == sec:
                out += [(off, info >> 8, add) for off, info, add in self.rela(i)]
        return out


def _sha1(path):
    return hashlib.sha1(Path(path).read_bytes()).hexdigest()


def _map_lines(path, cache={}):
    if path not in cache:
        cache[path] = Path(path).read_text(encoding="latin-1").splitlines()
    return cache[path]


def _map_section(lines, n):
    """The `<section> section layout` a (1-based) map line belongs to."""
    for i in range(n - 1, -1, -1):
        m = re.match(r"^(\S+) section layout\s*$", lines[i])
        if m:
            return m[1]
    return None


def _main_link_inputs():
    text = Path("build.ninja").read_text().replace("$\n", "")
    m = re.search(r"^build build/G4SE01/fsa_game_code\.elf(?: \|[^:]*)?:\s+link\s+(.*?)(?:\||$)", text, re.M)
    return m.group(1).split() if m else []


def _tracked(path):
    return bool(git("ls-files", "--", str(path)).strip())


def check_link_record(key, rec, expected):
    """(problem or None, inputs, objects) for one link the manifest relies on.
    A trial link is linked again (tools/link_check.py --verify-unit)."""
    if not Path(rec.get("map", "")).is_file() or _sha1(rec["map"]) != rec.get("map_sha1"):
        return f"map {rec.get('map')} missing or changed since the report", [], None
    if key == "main":
        mp, elf, dol = (Path("build/G4SE01/fsa_game_code" + x) for x in (".elf.MAP", ".elf", ".dol"))
        inputs = _main_link_inputs()
        if rec["map"] != str(mp) or rec.get("dol") != str(dol):
            return "the main link record doesn't name ninja's map and DOL", inputs, None
        if not elf.is_file() or not dol.is_file():
            return f"{elf} or {dol} missing", inputs, None
        if not (_sha1(dol) == rec.get("dol_sha1") == expected):
            return f"{dol} changed or doesn't match build.sha1", inputs, None
        if elf.stat().st_mtime > mp.stat().st_mtime + 1 or dol.stat().st_mtime + 1 < mp.stat().st_mtime:
            return f"{mp} is not from the link that wrote {dol}", inputs, None
    else:
        ev = Path(key)
        if ev.parent != Path("build/G4SE01/weakdedup_evidence") or not ev.is_file():
            return f"trial evidence {key} missing or outside build/G4SE01/weakdedup_evidence", [], None
        if _tracked(ev) or _tracked(rec["map"]):
            return f"trial evidence {key} is tracked by git", [], None
        try:
            r = json.loads(ev.read_text())
            if not (isinstance(r, dict) and isinstance(r.get("unit"), str)
                    and isinstance(r.get("objects"), dict) and isinstance(r.get("inputs"), list)):
                raise ValueError("not an evidence record (unit, objects, inputs)")
        except (OSError, ValueError) as err:
            return f"trial evidence {key} is malformed: {err}", [], None
        slug = r.get("unit", "").split("/", 1)[-1].replace("/", "__")
        inputs, objects = r.get("inputs", []), r.get("objects", {})
        if ev.name != f"{slug}.json" or not (r.get("map") == rec["map"] == str(ev.with_suffix(".MAP"))):
            return f"trial evidence {key} doesn't name its own unit's map", inputs, objects
        if r.get("map_sha1") != rec["map_sha1"] or r.get("dol_sha1") != expected or len(objects) != 1:
            return f"trial evidence {key} doesn't match the manifest or build.sha1", inputs, objects
        out = subprocess.run([sys.executable, "tools/link_check.py", "--verify-unit", r["unit"]],
                             capture_output=True, text=True)
        try:
            v = json.loads(out.stdout.strip().splitlines()[-1])
        except (ValueError, IndexError):
            return f"trial link of {r['unit']} couldn't be re-run: {(out.stdout + out.stderr)[-300:]}", inputs, objects
        if v["dol_sha1"] != expected:
            return f"re-running the trial link of {r['unit']} gives DOL SHA-1 {v['dol_sha1']}, not build.sha1", inputs, objects
        if v["map_sha1"] != rec["map_sha1"]:
            return f"re-running the trial link of {r['unit']} gives another map", inputs, objects
        if v["object_sha1"] != next(iter(objects.values())):
            return f"{r['unit']}'s object changed since the trial link", inputs, objects
        if v["inputs_sha1"] != hashlib.sha1("\n".join(inputs).encode()).hexdigest():
            return f"the trial link of {r['unit']} now has other inputs", inputs, objects
    if hashlib.sha1("\n".join(inputs).encode()).hexdigest() != rec.get("inputs_sha1"):
        return "link inputs changed since the report", inputs, None
    return None, inputs, None


def expected_stripped(a, removed):
    """Compare an independently rebuilt expectation of the stripped object with b:
    `removed` = {section index: [(offset, size)]} cut from the compiled object a.
    Returns a function b -> list of differences."""
    def shift(sec, x):
        return sum(sz for o, sz in removed.get(sec, []) if o + sz <= x)

    def cut(sec, x):
        return any(o <= x < o + sz for o, sz in removed.get(sec, []))

    def differences(b):
        diffs = []
        if a.ehdr != b.ehdr or a.names != b.names or len(a.sh) != len(b.sh):
            return ["ELF header or section list differs"]
        rela_of = {i: h[7] for i, h in enumerate(a.sh) if h[1] == 4}
        for i, n in enumerate(a.names):
            ha, hb = a.sh[i], b.sh[i]
            if i == a.symtab:
                want = []
                for name, val, size, bind, typ, shndx, nm, other in a.syms:
                    if shndx in removed and size and cut(shndx, val):
                        want.append((name, 0, 0, bind, typ, 0xFFF1, nm, other))
                    elif shndx in removed:
                        want.append((name, val - shift(shndx, val), size, bind, typ, shndx, nm, other))
                    else:
                        want.append((name, val, size, bind, typ, shndx, nm, other))
                got = b.syms
            elif i in removed:
                c = a.data(i)
                want = bytes(x for j, x in enumerate(c) if not cut(i, j))
                got = b.data(i)
            elif rela_of.get(i) in removed:
                sec = rela_of[i]
                want = [(off - shift(sec, off), info, add) for off, info, add in a.rela(i) if not cut(sec, off)]
                got = b.rela(i)
            else:
                want, got = (ha, a.data(i)), (hb, b.data(i))
            if want != got:
                diffs.append(f"section {n or i} isn't the compiled one less exactly the listed entries")
            if i in removed or rela_of.get(i) in removed or i == a.symtab:
                if ha[:5] + ha[6:] != hb[:5] + hb[6:]:
                    diffs.append(f"section header {n or i} differs beyond its size")
        return diffs
    return differences

SHT_PROGBITS, SHF_ALLOC, SHF_EXECINSTR, STT_OBJECT, STT_FUNC, STT_SECTION = 1, 2, 4, 1, 2, 3
_UNUSED = re.compile(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$")
_PLACED = re.compile(r"^\s+([0-9a-f]{8}) ([0-9a-f]{6}) ([0-9a-f]{8})\s+(\d+) (\S+)\s+(\S+)\s*$")


def _map_objects(path, cache={}):
    """{(section, object): {"placed": [(offset, size, label)], "unused": {label: [line numbers]}}}
    from a map's section layouts, read independently of tools/honest_report.py."""
    if path not in cache:
        out, section = {}, None
        for n, line in enumerate(_map_lines(path), 1):
            m = re.match(r"^(\S+) section layout\s*$", line)
            if m:
                section = m[1]
                continue
            if section is None:
                continue
            m = _UNUSED.match(line)
            if m:
                out.setdefault((section, m[3]), {"placed": [], "unused": {}})["unused"].setdefault(m[2], []).append(n)
                continue
            m = _PLACED.match(line)
            if m:
                out.setdefault((section, m[6]), {"placed": [], "unused": {}})["placed"].append(
                    (int(m[1], 16), int(m[2], 16), m[5]))
                if m[5] == section and m[4] == "1":
                    out.setdefault(("starts", section), []).append((int(m[1], 16), m[6]))
        cache[path] = out
    return cache[path]


def _data_section(elf, i):
    if not 0 <= i < len(elf.sh):
        return False
    h = elf.sh[i]
    return (h[1] == SHT_PROGBITS and h[2] & SHF_ALLOC and not h[2] & SHF_EXECINSTR
            and elf.names[i] not in ("extab", "extabindex"))


def _dead_in_map(elf, mp, name, sec, sym, evidence):
    """True if `evidence` is the map line listing sym UNUSED for this object in
    section sec's layout, with sym's size, and the map doesn't also place it."""
    lines = _map_lines(mp)
    if sum(x[0] == sym[0] for x in elf.syms) != 1 or not isinstance(evidence, dict):
        return False
    n = evidence.get("line")
    if not isinstance(n, int) or not 0 < n <= len(lines) or lines[n - 1] != evidence.get("text"):
        return False
    m = _UNUSED.match(lines[n - 1])
    block = _map_objects(mp).get((elf.names[sec], name), {"placed": [], "unused": {}})
    return bool(m and (int(m[1], 16), m[2], m[3]) == (sym[2], sym[0], name)
                and _map_section(lines, n) == elf.names[sec]
                and not any(p[2] == sym[0] for p in block["placed"]))


def check_dead_data(e, elf, name, mp):
    """One dead-stripped data object of the manifest, checked against the object
    and the map: (section index, offset, bytes removed) or why it isn't proven."""
    sec = elf.names.index(e.get("section")) if e.get("section") in elf.names else None
    if sec is None or not _data_section(elf, sec):
        return f"{e.get('section')} is not a data section of the object"
    sym = [x for x in elf.syms if x[0] == e.get("symbol")]
    if len(sym) != 1:
        return "not exactly one symbol of that name in the object"
    sym = sym[0]
    if (sym[5], sym[1], sym[2]) != (sec, e.get("offset"), e.get("size")) or not sym[2] or sym[4] in (STT_FUNC, STT_SECTION):
        return "the object's symbol isn't the listed data object"
    ev = e.get("evidence")
    if not (isinstance(ev, list) and len(ev) == 1 and _dead_in_map(elf, mp, name, sec, sym, ev[0])):
        return f"map evidence missing or doesn't match {mp}"
    # Every allocated location that reaches into it is in a symbol the map lists UNUSED.
    lo, hi = sym[1], sym[1] + sym[2]
    want = set()
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] != sec or not (t is sym or lo <= t[1] + add < hi):
                continue
            src = [x for x in elf.syms if x[5] == r and x[2] and x[1] <= off < x[1] + x[2] and x[4] in (STT_OBJECT, STT_FUNC)]
            if len(src) != 1:
                return f"referenced from {elf.names[r]}+0x{off:x}, which no single function or object contains"
            want.add((elf.names[r], src[0][0]))
    refs = e.get("referencers") or []
    got = {(x.get("section"), x.get("symbol")) for x in refs if isinstance(x, dict)}
    if got != want:
        return f"the manifest's referencers {sorted(got)} aren't the object's {sorted(want)}"
    for x in refs:
        rs = [y for y in elf.syms if y[0] == x["symbol"] and y[5] < len(elf.names) and elf.names[y[5]] == x["section"]]
        if len(rs) != 1 or not _dead_in_map(elf, mp, name, rs[0][5], rs[0], x.get("evidence")):
            return f"{x['symbol']} references it, and the map doesn't list {x['symbol']} UNUSED for {name}"
    # What goes with it: up to the next symbol or the section end, zero padding only.
    nxt = min([x[1] for x in elf.syms if x[5] == sec and x[2] and x[1] >= hi] + [elf.sh[sec][5]])
    if e.get("removed") != nxt - lo or any(elf.data(sec)[hi:nxt]):
        return "the removed range isn't the object plus zero padding up to the next symbol"
    return (sec, lo, nxt - lo)


def _unused_line(elf, mp, name, sec, sym):
    """The map's single UNUSED line for sym in this object and section, as
    evidence _dead_in_map accepts (so its size is checked too), or None."""
    block = _map_objects(mp).get((elf.names[sec], name), {"placed": [], "unused": {}})
    ns = block["unused"].get(sym[0], [])
    if len(ns) != 1:
        return None
    ev = {"line": ns[0], "text": _map_lines(mp)[ns[0] - 1]}
    return ev if _dead_in_map(elf, mp, name, sec, sym, ev) else None


def _container(elf, r, off):
    src = [x for x in elf.syms if x[5] == r and x[2] and x[1] <= off < x[1] + x[2] and x[4] in (STT_OBJECT, STT_FUNC)]
    return src[0] if len(src) == 1 else None


def check_dead_layout(elf, sec, rem, mp, name, entries):
    """The stripped section must have the layout the link gave this object's
    section: every kept sized symbol at the offset (from the object's section
    start) and with the size the map places it at, and the next object's
    section in the layout 0 to 7 bytes (alignment) after the stripped end.
    No live relocation may reach the section through its section symbol, or a
    size-0 label of it, at or past the first removed byte (its addend isn't
    moved); a size-0 label any allocated relocation targets must be listed
    UNUSED for this object, and so must each of its referencers, as the
    manifest's `anchors` say. Returns why not, or None."""
    secname = elf.names[sec]
    objs = _map_objects(mp)
    block = objs.get((secname, name), {"placed": [], "unused": {}})["placed"]
    starts = [p for p in block if p[2] == secname]
    if len(starts) != 1 or block[0] != starts[0]:
        return f"the map doesn't place this object's {secname} exactly once"
    start = starts[0][0]
    for x in elf.syms:
        if x[5] != sec or not x[2] or x[4] == STT_SECTION or any(o <= x[1] < o + n for o, n in rem):
            continue
        new = x[1] - sum(n for o, n in rem if o + n <= x[1])
        got = [p for p in block[1:] if p[2] == x[0]]
        if len(got) != 1 or (got[0][0] - start, got[0][1]) != (new, x[2]):
            return f"the map doesn't place {x[0]} at 0x{new:x}, size 0x{x[2]:x}, of the stripped section"
    order = objs.get(("starts", secname), [])
    idx = [i for i, o in enumerate(order) if o[1] == name]
    if len(idx) != 1 or idx[0] + 1 >= len(order):
        return f"the map has no object after this one's {secname} to bound its end"
    gap = order[idx[0] + 1][0] - (start + elf.sh[sec][5] - sum(n for _, n in rem))
    if not 0 <= gap < 8:
        return f"the next object in the map starts {gap} bytes after the stripped end, not within alignment"
    first = min(o for o, _ in rem)
    anchors = {}
    for r, h in enumerate(elf.sh):
        if not h[2] & SHF_ALLOC:
            continue
        for off, si, add in elf.relocs(r):
            t = elf.syms[si]
            if t[5] != sec or t[2] and t[4] != STT_SECTION:
                continue
            src = _container(elf, r, off)
            dead = src is not None and _unused_line(elf, mp, name, r, src) is not None
            if t[4] != STT_SECTION:
                if _unused_line(elf, mp, name, sec, t) is None or not dead:
                    return f"its size-0 label {t[0] or '?'} is referenced, and it or {elf.names[r]}+0x{off:x} isn't listed UNUSED"
                anchors.setdefault(t[0], set()).add((elf.names[r], src[0]))
            if t[1] + add >= first and not dead:
                return f"live {elf.names[r]}+0x{off:x} reaches {secname} through its section symbol past a removed object"
    for e in entries:
        got = {a.get("symbol"): {(x.get("section"), x.get("symbol")) for x in a.get("referencers", [])}
               for a in e.get("anchors", []) if isinstance(a, dict)}
        if got != anchors:
            return f"{e.get('symbol')}: the manifest's anchors aren't the object's"
    return None


def check_weakdedup(problems):
    """Re-verify every extab entry the honest report left out (tools/honest_report.py)."""
    path = Path("build/G4SE01/weakdedup_manifest.json")
    if not path.is_file():
        problems.append(f"{path} missing: the honest report didn't run tools/honest_report.py's weak-duplicate check")
        return
    man = json.loads(path.read_text())
    expected = Path("config/G4SE01/build.sha1").read_text().split()[0]
    entries = man.get("entries", [])
    links = {}
    for key, rec in man.get("links", {}).items():
        links[key] = check_link_record(key, rec, expected) + (rec,)
    by_stripped, seen, dd_sections, rec_maps, dd_entries = {}, set(), set(), {}, {}
    for e in entries:
        tag = f"weakdedup {e.get('unit')}: {e.get('function')}"
        kind = e.get("kind", "weak_extab")
        if kind == "dead_data":
            tag = f"dead data {e.get('unit')}: {e.get('symbol')}"
        elif kind != "weak_extab":
            problems.append(f"{tag}: unknown kind of removal {kind!r}")
            continue
        ident = (e.get("object"), e.get("symbol") if kind == "dead_data" else e.get("extabindex"), kind)
        if ident in seen:
            problems.append(f"{tag}: listed twice in the manifest")
            continue
        seen.add(ident)
        key = e.get("link")
        if key not in links:
            problems.append(f"{tag}: no link record {key}")
            continue
        why, inputs, objects, rec = links[key]
        if why:
            problems.append(f"{tag}: stale or unproven link: {why}")
            continue
        obj = e["object"]
        name = Path(obj).name
        if not Path(obj).is_file() or _sha1(obj) != e["object_sha1"]:
            problems.append(f"{tag}: {obj} changed since the report")
            continue
        if obj not in inputs or sum(Path(i).name == name for i in inputs) != 1:
            problems.append(f"{tag}: {obj} is not a uniquely named input of the link")
            continue
        if key == "main" and Path(obj).stat().st_mtime > Path(rec["map"]).stat().st_mtime + 1:
            problems.append(f"{tag}: {obj} is newer than the link map")
            continue
        if key != "main" and json.loads(Path(key).read_text()).get("objects", {}).get(obj) != e["object_sha1"]:
            problems.append(f"{tag}: {obj} isn't the object the trial link included")
            continue
        try:
            elf = Elf(obj)
        except (OSError, ValueError, StopIteration) as err:
            problems.append(f"{tag}: {err}")
            continue
        if kind == "dead_data":
            if rec_maps.setdefault(e["unit"], rec["map"]) != rec["map"]:
                problems.append(f"{tag}: the unit's removals cite different links")
                continue
            why = check_dead_data(e, elf, name, rec["map"])
            if isinstance(why, str):
                problems.append(f"{tag}: {why}")
            else:
                dd_sections.add((e["unit"], why[0]))
                dd_entries.setdefault((e["unit"], e["section"]), []).append(e)
                by_stripped.setdefault((e["unit"], obj, e["stripped_object"]), []).append(why)
            continue
        # The object: the extabindex entry points at a WEAK FUNC and at the extab label.
        syms = {}
        for sym in elf.syms:
            syms.setdefault((sym[0], elf.names[sym[5]] if sym[5] < len(elf.names) else None), sym)
        fn = next((x for x in elf.syms if x[0] == e["function"] and x[5] < len(elf.names)), None)
        if not fn or fn[3] != 2 or fn[4] != 2:
            problems.append(f"{tag}: not a WEAK FUNC in {obj}")
            continue
        eti, etb = syms.get((e["extabindex"], "extabindex")), syms.get((e["extab"], "extab"))
        if not eti or not etb or eti[2] != 12 or etb[2] != e["extab_size"]:
            problems.append(f"{tag}: {e['extabindex']}/{e['extab']} aren't its extabindex/extab entries")
            continue
        rel = {off: elf.syms[s][0] for off, s, _ in elf.relocs(eti[5])}
        if rel.get(eti[1]) != e["function"] or rel.get(eti[1] + 8) != e["extab"]:
            problems.append(f"{tag}: {e['extabindex']} doesn't describe {e['function']} with {e['extab']}")
            continue
        # The map lines, read back from the map itself.
        lines = _map_lines(rec["map"])
        ev = e.get("evidence", [])
        texts = [lines[x["line"] - 1] if 0 < x["line"] <= len(lines) else None for x in ev]
        ok = len(ev) == 4 and all(t == x["text"] for t, x in zip(texts, ev))
        if ok:
            dup, found, u_eti, u_etb = ev
            m_dup = re.search(r">>> UNREFERENCED DUPLICATE (.+?)\s*$", dup["text"])
            m_found = re.search(r">>> \(func,weak\) found in (\S+)\s*$", found["text"])
            m_eti = re.match(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$", u_eti["text"])
            m_etb = re.match(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$", u_etb["text"])
            ok = bool(m_dup and m_found and m_eti and m_etb
                      and m_dup[1] == e["function"] and m_found[1] == name
                      and found["line"] > dup["line"]
                      and all(">>> (" in lines[i] for i in range(dup["line"], found["line"] - 1))
                      and (int(m_eti[1], 16), m_eti[2], m_eti[3]) == (12, e["extabindex"], name)
                      and _map_section(lines, u_eti["line"]) == "extabindex"
                      and (int(m_etb[1], 16), m_etb[2], m_etb[3]) == (e["extab_size"], e["extab"], name)
                      and _map_section(lines, u_etb["line"]) == "extab")
        if not ok:
            problems.append(f"{tag}: map evidence missing or doesn't match {rec['map']}")
            continue
        by_stripped.setdefault((e["unit"], obj, e["stripped_object"]), []).append((eti[5], eti[1], 12))
        by_stripped[(e["unit"], obj, e["stripped_object"])].append((etb[5], etb[1], etb[2]))
    # Every unit diffed against another object than its compiled one is in the
    # manifest, and that copy is the compiled object less exactly those entries.
    honest = Path("build/G4SE01/honest/objdiff.json")
    if not honest.is_file():
        problems.append(f"{honest} missing")
        return
    orig = {u["name"]: u.get("base_path") for u in json.loads(Path("objdiff.json").read_text())["units"]}
    proven = {u: (o, s) for u, o, s in by_stripped}
    root = Path.cwd()
    for u in json.loads(honest.read_text())["units"]:
        base, want = u.get("base_path"), orig.get(u["name"])
        if not base or not want or Path(base) == root / want:
            continue
        if u["name"] not in proven or Path(base) != root / proven[u["name"]][1]:
            problems.append(f"{u['name']}: the honest report diffs {base}, not its compiled object, without proven entries")
            continue
        obj, stripped = proven[u["name"]]
        removed = {}
        for sec, off, size in by_stripped[(u["name"], obj, stripped)]:
            removed.setdefault(sec, []).append((off, size))
        try:
            a = Elf(obj)
            diffs = [f"{a.names[sec]}: {why}" for (unit, sec) in sorted(dd_sections) if unit == u["name"]
                     for why in [check_dead_layout(a, sec, removed[sec], rec_maps[u["name"]], Path(obj).name,
                                                       dd_entries[(u["name"], a.names[sec])])] if why]
            diffs += expected_stripped(a, removed)(Elf(stripped))
        except (OSError, ValueError, StopIteration, IndexError, struct.error) as err:
            diffs = [str(err)]
        problems += [f"{u['name']}: {stripped}: {d}" for d in diffs]


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
    tracked = git("ls-files", "--", "build/").split()
    if tracked:
        problems.append(f"build artifacts tracked by git (never commit build/): {', '.join(tracked[:5])}")
    check_configure(problems)
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
