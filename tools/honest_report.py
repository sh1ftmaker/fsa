#!/usr/bin/env python3
"""Write the objdiff report with every unit diffed and nothing taken on trust.

objdiff 3.8.3's `report generate` diffs a `complete` (our `Matching`) unit
like any other when its base object (our compiled .o) exists; it assumes
100 % only for a complete unit without a base_path, and fails if base_path
names a missing file. Older objdiff versions assumed 100 % for every
complete unit, which is why this report exists. It still guards against
both remaining ways to count unseen or mis-paired code, by writing
build/G4SE01/report_honest.json from a copy of objdiff.json where
  * no unit is complete, so a unit with no compiled object counts 0 %, and
  * symbol_mappings are dropped (objdiff honors them: a mapping made in the
    GUI pairs differently named symbols and counts them as matched),
with the same report arguments. tools/progress.py, tools/gate.py and
tools/coord.py read this one; report.json stays for decomp.dev-style tooling
(its matched measures are the same while every Matching unit has a base).

Weak duplicates the link discarded (CLAUDE.md, "Measuring progress"): a
translation unit that instantiates a weak function (e.g. the JASMemPool<T>
destructors of the JAudio pools every game unit's __sinit constructs) gets
its own copy plus an extab/extabindex entry for it. The linker keeps one
copy and drops the others' entries, so the original object's split lacks
them. Before diffing, a unit's compiled object is copied without such an
entry only when ALL of these hold:
  * the extabindex entry's function relocation targets a WEAK FUNC symbol;
  * a link map lists that function as `UNREFERENCED DUPLICATE` found in
    this object as (func,weak);
  * the same map lists the entry's extab and extabindex labels as UNUSED
    for this object, in the extab and extabindex section layouts;
  * that map comes from a link whose inputs include this exact compiled
    object (path, and content unchanged since: mtime for ninja's link,
    SHA-1 for a recorded trial link), and the link's DOL matches build.sha1;
    a trial link's record must be exactly what link_check.py --unit writes
    for this unit against the current main link (same inputs with the one
    object swapped), untracked by git, and tools/gate.py re-runs that link
    and requires the same map and SHA-1;
  * the object's file name is unique among that link's inputs (the map
    names objects by file name only).
Evidence comes from ninja's own link (Matching units) or from a trial link
recorded by `tools/link_check.py --unit <unit>` (a NonMatching unit linked
in place of dtk's split, in build/G4SE01/weakdedup_evidence/). The report
then diffs the stripped copy (build/G4SE01/weakdedup/...), and every removal
is listed with its evidence lines in build/G4SE01/weakdedup_manifest.json,
which tools/gate.py re-verifies. Everything else is diffed as compiled.

Usage (ninja runs it): tools/honest_report.py <objdiff-cli> <output> [report args...]
"""

import hashlib
import json
import re
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

VERSION = "G4SE01"
BUILD = Path("build") / VERSION
MAIN_ELF = BUILD / "fsa_game_code.elf"
MAIN_MAP = BUILD / "fsa_game_code.elf.MAP"
MAIN_DOL = BUILD / "fsa_game_code.dol"
SHA1 = Path("config") / VERSION / "build.sha1"
EVIDENCE = BUILD / "weakdedup_evidence"
STRIPPED = BUILD / "weakdedup"
MANIFEST = BUILD / "weakdedup_manifest.json"


def sha1(path):
    return hashlib.sha1(Path(path).read_bytes()).hexdigest()


def main_link_inputs():
    """The main link's object list, in order, from build.ninja."""
    text = Path("build.ninja").read_text().replace("$\n", "")
    m = re.search(rf"^build {re.escape(str(MAIN_ELF))}(?: \|[^:]*)?:\s+link\s+(.*?)(?:\||$)", text, re.M)
    return m.group(1).split() if m else []


# ---------------------------------------------------------------- link maps

def parse_map(path):
    """Evidence from an MWLD map written with -map -mapunused.

    Returns (dups, unused): dups[(object, function)] = [(line, text), (line, text)]
    for `>>> UNREFERENCED DUPLICATE <function>` followed by
    `>>> (func,weak) found in <object>`; unused[(section, object, label)] =
    (line, text) for `UNUSED ... <label> <object>` in `<section> section layout`.
    Line numbers are 1-based."""
    dups, unused = {}, {}
    cur, section = None, None
    with open(path, encoding="latin-1") as f:
        for n, line in enumerate(f, 1):
            line = line.rstrip("\n")
            m = re.search(r">>> UNREFERENCED DUPLICATE (.+?)\s*$", line)
            if m:
                cur = (n, line, m[1])
                continue
            m = re.search(r">>> \((\w+),(\w+)\) found in (\S+)", line)
            if m and cur:
                if (m[1], m[2]) == ("func", "weak"):
                    dups.setdefault((m[3], cur[2]), [(cur[0], cur[1]), (n, line)])
                continue
            cur = None
            m = re.match(r"^(\S+) section layout\s*$", line)
            if m:
                section = m[1]
                continue
            m = re.match(r"^\s+UNUSED\s+[0-9a-f]+ \.+ (\S+) (\S+)\s*$", line)
            if m and section:
                unused.setdefault((section, m[2], m[1]), (n, line))
    return dups, unused


def unit_slug(unit_name):
    """fsa_game_code/game/tu_80214244 -> game__tu_80214244 (evidence file names)."""
    return unit_name.split("/", 1)[-1].replace("/", "__")


def trial_inputs(unit_name, base, main_inputs):
    """The inputs of `link_check.py --unit`'s trial link: the main link's inputs
    with dtk's split of this unit swapped for its compiled object (None if the
    unit is already linked from its object, or has no split in the link)."""
    split = str(BUILD / "obj" / (unit_name.split("/", 1)[-1] + ".o"))
    if base in main_inputs or split not in main_inputs:
        return None
    return [base if o == split else o for o in main_inputs]


def git_tracked(path):
    out = subprocess.run(["git", "ls-files", "--", str(path)], capture_output=True, text=True)
    return bool(out.stdout.strip())


class Link:
    """One link whose map may prove removals: ninja's link or a recorded trial link."""

    def __init__(self, kind, map_path, inputs, dol=None, dol_sha1=None, map_sha1=None, objects=None):
        self.kind, self.map, self.inputs = kind, Path(map_path), inputs
        self.dol, self.dol_sha1, self.map_sha1 = dol, dol_sha1, map_sha1
        self.objects = objects or {}  # trial links: object path -> SHA-1 at link time
        self._parsed = None
        self.problem = self._check()

    def _check(self):
        expected = SHA1.read_text().split()[0]
        if not self.map.is_file():
            return f"{self.map} missing"
        if self.kind == "main":
            if not MAIN_ELF.is_file() or not Path(self.dol).is_file():
                return f"{MAIN_ELF} or {self.dol} missing"
            self.dol_sha1 = sha1(self.dol)
            if (MAIN_ELF.stat().st_mtime > self.map.stat().st_mtime + 1
                    or Path(self.dol).stat().st_mtime + 1 < self.map.stat().st_mtime):
                return f"{self.map} is not from the link that wrote {self.dol}"
        else:
            # The trial DOL itself isn't kept; tools/gate.py re-runs the link.
            if sha1(self.map) != self.map_sha1:
                return f"{self.map} changed since the trial link"
        if self.dol_sha1 != expected:
            return f"the link's DOL SHA-1 {self.dol_sha1} != build.sha1 {expected}"
        return None

    def proves(self, obj):
        """Why this link can't prove anything about obj, or None if it can."""
        if self.problem:
            return self.problem
        if obj not in self.inputs:
            return f"{obj} is not an input of this link"
        if sum(Path(i).name == Path(obj).name for i in self.inputs) != 1:
            return f"{Path(obj).name} is not a unique file name among the link inputs"
        if self.kind == "main":
            if Path(obj).stat().st_mtime > self.map.stat().st_mtime + 1:
                return f"{obj} is newer than {self.map}"
        elif self.objects.get(obj) != sha1(obj):
            return f"{obj} changed since the trial link"
        return None

    def parsed(self):
        if self._parsed is None:
            self._parsed = parse_map(self.map)
        return self._parsed

    def record(self):
        return {"kind": self.kind, "map": str(self.map), "map_sha1": sha1(self.map),
                "dol": str(self.dol) if self.dol else None, "dol_sha1": self.dol_sha1,
                "inputs_sha1": hashlib.sha1("\n".join(self.inputs).encode()).hexdigest()}


def trial_link(unit_name, base, main_inputs, notes):
    """The Link recorded for this unit by `link_check.py --unit`, if its evidence
    is exactly what that tool writes for the current main link, else None."""
    slug = unit_slug(unit_name)
    ev = EVIDENCE / f"{slug}.json"
    if not ev.is_file():
        return None
    inputs = trial_inputs(unit_name, base, main_inputs)
    try:
        rec = json.loads(ev.read_text())
        why = None
        if git_tracked(ev) or git_tracked(EVIDENCE / f"{slug}.MAP"):
            why = "evidence is tracked by git (build artifacts are never committed)"
        elif rec.get("unit") != unit_name or rec.get("map") != str(EVIDENCE / f"{slug}.MAP"):
            why = "evidence names another unit or map"
        elif list(rec.get("objects", {})) != [base]:
            why = f"evidence is not for {base}"
        elif inputs is None or rec.get("inputs") != inputs:
            why = "evidence is from another link (the Matching set or link order changed); rerun link_check.py --unit"
        if why:
            notes.append(f"{unit_name}: ignoring {ev}: {why}")
            return None
        link = Link("trial", rec["map"], inputs, dol_sha1=rec.get("dol_sha1"),
                    map_sha1=rec.get("map_sha1"), objects=rec["objects"])
    except (OSError, KeyError, ValueError, AttributeError) as e:
        notes.append(f"{unit_name}: ignoring {ev}: {e}")
        return None
    link.evidence_file = str(ev)
    return link


# --------------------------------------------------------------- ELF objects

class Elf:
    """Just enough of a big-endian ELF32 relocatable object."""

    def __init__(self, data):
        d = self.d = bytearray(data)
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 2:
            raise ValueError("not a big-endian ELF32 object")
        self.shoff, = struct.unpack_from(">I", d, 0x20)
        self.shentsize, self.shnum, shstrndx = struct.unpack_from(">HHH", d, 0x2E)
        self.sh = [list(struct.unpack_from(">IIIIIIIIII", d, self.shoff + i * self.shentsize))
                   for i in range(self.shnum)]
        self.names = [self.cstr(self.sh[shstrndx][4] + self.sh[i][0]) for i in range(self.shnum)]
        self.symtab = next(i for i in range(self.shnum) if self.sh[i][1] == 2)
        strtab = self.sh[self.sh[self.symtab][6]][4]
        self.syms = []
        for k in range(self.sh[self.symtab][5] // 16):
            o = self.sh[self.symtab][4] + k * 16
            nm, val, size, info, _, shndx = struct.unpack_from(">IIIBBH", d, o)
            self.syms.append(dict(name=self.cstr(strtab + nm), value=val, size=size,
                                  bind=info >> 4, type=info & 15, shndx=shndx, off=o))
        self.rela = {self.sh[i][7]: i for i in range(self.shnum) if self.sh[i][1] == 4}

    def cstr(self, o):
        return self.d[o:self.d.index(0, o)].decode("latin-1")

    def section(self, name):
        return self.names.index(name) if name in self.names else None

    def relocs(self, sec):
        r = self.rela.get(sec)
        if r is None:
            return []
        out = []
        for k in range(self.sh[r][5] // 12):
            off, info, add = struct.unpack_from(">IIi", self.d, self.sh[r][4] + k * 12)
            out.append((off, info >> 8, info & 0xFF, add))
        return out


STB_WEAK, STT_FUNC, STT_SECTION = 2, 2, 3


def weak_entries(elf):
    """(extabindex symbol, extab symbol, function symbol) for every extabindex
    entry of a weak function, or a reason the object can't be stripped safely."""
    eti, etb = elf.section("extabindex"), elf.section("extab")
    if eti is None or etb is None:
        return [], None
    for sec in range(elf.shnum):
        for _, s, _, _ in elf.relocs(sec):
            if elf.syms[s]["type"] == STT_SECTION and elf.syms[s]["shndx"] in (eti, etb):
                return [], "a relocation targets the extab/extabindex section symbol"
    rel = {off: s for off, s, _, _ in elf.relocs(eti)}
    out = []
    for s in elf.syms:
        if s["shndx"] != eti or s["size"] != 12:
            continue
        fn, tab = rel.get(s["value"]), rel.get(s["value"] + 8)
        if fn is None or tab is None or s["value"] + 4 in rel:
            continue
        fn, tab = elf.syms[fn], elf.syms[tab]
        if fn["bind"] == STB_WEAK and fn["type"] == STT_FUNC and tab["shndx"] == etb and tab["size"]:
            out.append((s, tab, fn))
    return out, None


def strip(elf, removals):
    """Remove byte ranges {section: [(offset, size)]}, with their relocations;
    later symbols and relocations move down, removed symbols become absolute 0."""
    d, sh = elf.d, elf.sh
    for sec, rem in removals.items():
        rem.sort()

        def shift(a):
            return sum(sz for ad, sz in rem if ad + sz <= a)

        def removed(a):
            return any(ad <= a < ad + sz for ad, sz in rem)
        o, size = sh[sec][4], sh[sec][5]
        keep = bytes(b for i, b in enumerate(d[o:o + size]) if not removed(i))
        d[o:o + len(keep)] = keep
        d[o + len(keep):o + size] = bytes(size - len(keep))
        sh[sec][5] = len(keep)
        if sec in elf.rela:
            r = elf.rela[sec]
            kept = [(off - shift(off), s, t, a) for off, s, t, a in elf.relocs(sec) if not removed(off)]
            for k, (off, s, t, a) in enumerate(kept):
                struct.pack_into(">IIi", d, sh[r][4] + k * 12, off, (s << 8) | t, a)
            d[sh[r][4] + len(kept) * 12:sh[r][4] + sh[r][5]] = bytes(sh[r][5] - len(kept) * 12)
            sh[r][5] = len(kept) * 12
        for s in elf.syms:
            if s["shndx"] != sec:
                continue
            if s["size"] and removed(s["value"]):
                struct.pack_into(">II", d, s["off"] + 4, 0, 0)
                struct.pack_into(">H", d, s["off"] + 14, 0xFFF1)  # SHN_ABS: out of the section
            else:
                struct.pack_into(">I", d, s["off"] + 4, s["value"] - shift(s["value"]))
    for i in range(elf.shnum):
        struct.pack_into(">IIIIIIIIII", d, elf.shoff + i * elf.shentsize, *sh[i])
    return bytes(d)


def dedup_unit(unit, base, link, notes):
    """Write the stripped copy of base and return (path, manifest entries), or (None, [])."""
    try:
        elf = Elf(Path(base).read_bytes())
    except (OSError, ValueError) as e:
        return None, []
    entries, why = weak_entries(elf)
    if why:
        notes.append(f"{unit}: {why}; nothing stripped")
        return None, []
    if not entries:
        return None, []
    why = link.proves(base) if link else "no link includes this object"
    if why:
        notes.append(f"{unit}: {len(entries)} weak extab entries kept: {why}")
        return None, []
    dups, unused = link.parsed()
    name = Path(base).name
    removals, found = defaultdict(list), []
    for eti, etb, fn in entries:
        dup = dups.get((name, fn["name"]))
        u_eti = unused.get(("extabindex", name, eti["name"]))
        u_etb = unused.get(("extab", name, etb["name"]))
        if not (dup and u_eti and u_etb):
            continue
        removals[eti["shndx"]].append((eti["value"], 12))
        removals[etb["shndx"]].append((etb["value"], etb["size"]))
        found.append({
            "unit": unit, "object": base, "object_sha1": sha1(base), "function": fn["name"],
            "extabindex": eti["name"], "extab": etb["name"], "extab_size": etb["size"],
            "link": getattr(link, "evidence_file", None) or link.kind,
            "evidence": [{"line": n, "text": t} for n, t in (*dup, u_eti, u_etb)],
        })
    if len(found) < len(entries):
        notes.append(f"{unit}: {len(entries) - len(found)} weak extab entries kept: "
                     f"{link.map} has no discarded-duplicate evidence for them")
    if not found:
        return None, []
    out = STRIPPED / Path(base).relative_to(BUILD)
    out.parent.mkdir(parents=True, exist_ok=True)
    tmp = out.with_suffix(".tmp")
    tmp.write_bytes(strip(elf, removals))
    tmp.replace(out)
    for e in found:
        e["stripped_object"] = str(out)
    return str(out), found


def weak_dedup(cfg):
    """Point base_path at stripped copies where a link proves the removal; write the manifest."""
    notes, entries, links = [], [], {}
    main = Link("main", MAIN_MAP, main_link_inputs(), dol=MAIN_DOL)
    if main.problem:
        notes.append(f"main link can't prove removals: {main.problem}")
    EVIDENCE.mkdir(parents=True, exist_ok=True)  # an input of report_honest.json
    for unit in cfg["units"]:
        base = unit.get("base_path")
        if not base or not Path(base).is_file():
            continue
        link = main if base in main.inputs else trial_link(unit["name"], base, main.inputs, notes)
        path, found = dedup_unit(unit["name"], base, link, notes)
        if path:
            unit["base_path"] = path
            entries += found
            key = getattr(link, "evidence_file", None) or link.kind
            links[key] = link.record()
    STRIPPED.mkdir(parents=True, exist_ok=True)
    live = {e["stripped_object"] for e in entries}
    for old in STRIPPED.rglob("*.o"):
        if str(old) not in live:
            old.unlink()
    manifest = {"build_sha1": SHA1.read_text().split()[0], "links": links,
                "entries": entries, "notes": notes}
    tmp = MANIFEST.with_suffix(".tmp")
    tmp.write_text(json.dumps(manifest, indent=1))
    tmp.replace(MANIFEST)
    units = len({e["unit"] for e in entries})
    print(f"weakdedup: {len(entries)} extab entries of discarded weak duplicates removed "
          f"from {units} units ({MANIFEST})", file=sys.stderr)


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    objdiff, out, extra = sys.argv[1], Path(sys.argv[2]), sys.argv[3:]
    root = Path.cwd()
    cfg = json.loads((root / "objdiff.json").read_text())
    weak_dedup(cfg)
    for unit in cfg["units"]:
        for key in ("target_path", "base_path"):
            if unit.get(key):
                unit[key] = str(root / unit[key])
        unit.setdefault("metadata", {})["complete"] = False
        unit.pop("symbol_mappings", None)
    project = out.parent / "honest"
    project.mkdir(parents=True, exist_ok=True)
    (project / "objdiff.json").write_text(json.dumps(cfg, indent=1))
    cmd = [objdiff, "report", "generate", "-p", str(project), "-o", str(out), *extra]
    sys.exit(subprocess.run(cmd).returncode)


if __name__ == "__main__":
    main()
