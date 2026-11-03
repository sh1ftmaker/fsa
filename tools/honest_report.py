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
    object swapped), untracked by git, and re-running that link
    (link_check.py --verify-unit; cached by everything the link depends on)
    must give the same map and a DOL matching build.sha1. The cache
    (build/G4SE01/weakdedup_verified.json) is a local speed-up, trusted like
    any build output and forgeable by whoever can write build/; only
    tools/gate.py, which re-links without reading it, proves a trial removal;
  * the object's file name is unique among that link's inputs (the map
    names objects by file name only).
Data the link dead-stripped (the second kind of removal): an object
keeps every data object its compiled functions use, also those of functions
the original linker dead-stripped (C_MTXPerspective's two constants, the
jump table of GXReadGPMetric, dvdfs's dead OSPanic strings), while dtk's
split of the original has only what survived. Such a data object is left
out of the copy only when the same link map (same link requirements as
above) lists it `UNUSED` for this object with its size, every allocated
location that references it lies in a function or object the map also
lists `UNUSED` for this object, it goes with zero padding only, and the map
places every kept symbol of its section at the offset the stripped copy
gives it (see dead_data()). Debug sections don't count as references.
Weak data the link kept from another object (the third and last kind): an
inline class's weak vtable, emitted by every TU that uses it. It is left
out only when the same link map's tree lists it as an UNREFERENCED
DUPLICATE (object,weak) found in this object, kept in one other link input
whose copy has the same bytes and relocations (by target name), every
reference into ours names the symbol, and the map gives the section the
stripped layout (see weak_data()); the symbol becomes undefined, as in
dtk's split.

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

PLACED_LINE = re.compile(r"^\s+([0-9a-f]{8}) ([0-9a-f]{6}) ([0-9a-f]{8})\s+(\d+) (\S+)\s+(\S+)\s*$")

def parse_map(path):
    """Evidence from an MWLD map written with -map -mapunused.

    Returns (dups, unused, placed): dups[(object, function)] = [(line, text), (line, text)]
    for `>>> UNREFERENCED DUPLICATE <function>` followed by
    `>>> (func,weak) found in <object>`; unused[(section, object, label)] =
    (line, text) for `UNUSED ... <label> <object>` in `<section> section layout`;
    placed[(section, object)] = [(offset, size, label, line, text)] for every
    label the link kept, in the order of the layout (the first is the
    object's section itself, alignment 1, label = the section name);
    starts[section] = [(offset, object, line, text)] for every object's
    section start line, in the order of the layout.
    Line numbers are 1-based."""
    dups, unused, placed, starts = {}, {}, defaultdict(list), defaultdict(list)
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
                continue
            m = PLACED_LINE.match(line)
            if m and section:
                placed[(section, m[6])].append((int(m[1], 16), int(m[2], 16), m[5], n, line))
                if m[5] == section and m[4] == "1":
                    starts[section].append((int(m[1], 16), m[6], n, line))
    return dups, unused, placed, starts


DUP_LINE = re.compile(r"^\s*(\d+)\] >>> UNREFERENCED DUPLICATE (\S+)\s*$")
KEPT_LINE = re.compile(r"^\s*(\d+)\] (\S+) \((\w+),(\w+)\) found in (\S+)\s*$")
FOUND_LINE = re.compile(r"^\s*(\d+)\] >>> \((\w+),(\w+)\) found in (\S+)\s*$")


def parse_weak_objects(path):
    """Weak data objects the link discarded, from an MWLD map's link tree:
    {(object, name): [(kept object, [(line, text) of the kept copy's line, the
    `>>> UNREFERENCED DUPLICATE <name>` line right after it, and this object's
    `>>> (object,weak) found in <object>` line])]}, one item per tree node that
    lists it. The kept copy's line is `<name> (object,<bind>) found in <kept>`,
    at the same tree depth; the found lines follow the duplicate line, with only
    other `>>> (...) found in` lines between them."""
    out = defaultdict(list)
    prev, cur = None, None
    with open(path, encoding="latin-1") as f:
        for n, line in enumerate(f, 1):
            line = line.rstrip("\n")
            m = DUP_LINE.match(line)
            if m:
                k = KEPT_LINE.match(prev[1]) if prev else None
                cur = None
                if k and k[1] == m[1] and k[2] == m[2] and k[3] == "object":
                    cur = (m[1], m[2], k[5], prev, (n, line))
                prev = (n, line)
                continue
            m = FOUND_LINE.match(line)
            if m and cur:
                if m[1] == cur[0] and (m[2], m[3]) == ("object", "weak"):
                    out[(m[4], cur[1])].append((cur[2], [cur[3], cur[4], (n, line)]))
                prev = (n, line)
                continue
            cur = None
            prev = (n, line)
    return out


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

    def weak_objects(self):
        if getattr(self, "_weak_objects", None) is None:
            self._weak_objects = parse_weak_objects(self.map)
        return self._weak_objects

    def kept_object(self, kept):
        """The link input whose file name is `kept` (the map names objects by
        file name only), or why it can't stand for the kept copy."""
        paths = [i for i in self.inputs if Path(i).name == kept]
        if len(paths) != 1:
            return None, f"{kept} is not a unique file name among the link inputs"
        if not Path(paths[0]).is_file():
            return None, f"{paths[0]} missing"
        if self.kind == "main" and Path(paths[0]).stat().st_mtime > self.map.stat().st_mtime + 1:
            return None, f"{paths[0]} is newer than {self.map}"
        return paths[0], None

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
        if not link.problem:
            why = relink(unit_name, base, rec, inputs)
            if why:
                notes.append(f"{unit_name}: ignoring {ev}: {why}")
                return None
    except (OSError, KeyError, ValueError, AttributeError) as e:
        notes.append(f"{unit_name}: ignoring {ev}: {e}")
        return None
    link.evidence_file = str(ev)
    return link


VERIFIED = BUILD / "weakdedup_verified.json"
_link_state = None


def link_state(main_inputs):
    """SHA-1 of everything a trial link's result depends on besides the swapped
    object: build.ninja (linker, flags), the linker script, build.sha1 and
    every main link input's path and contents."""
    global _link_state
    if _link_state is None:
        h = hashlib.sha1()
        for f in ("build.ninja", BUILD / "ldscript.lcf", SHA1):
            h.update(Path(f).read_bytes() if Path(f).is_file() else b"-")
        for i in main_inputs:
            h.update(f"{i}={sha1(i) if Path(i).is_file() else '-'}\n".encode())
        _link_state = h.hexdigest()
    return _link_state


def relink(unit_name, base, rec, inputs):
    """Link the trial again (tools/link_check.py --verify-unit) and return why the
    record isn't what that link produces, or None. Results are cached in
    build/G4SE01/weakdedup_verified.json by (link state, object SHA-1, map SHA-1),
    so a record is re-linked only when anything that link depends on changed."""
    obj_sha = sha1(base)
    inputs_sha = hashlib.sha1("\n".join(inputs).encode()).hexdigest()
    key = f"{link_state(main_link_inputs())} {obj_sha} {inputs_sha} {rec['map_sha1']}"
    try:
        cache = json.loads(VERIFIED.read_text())
    except (OSError, ValueError):
        cache = {}
    if key in cache:
        return cache[key]
    out = subprocess.run([sys.executable, "tools/link_check.py", "--verify-unit", unit_name],
                         capture_output=True, text=True)
    try:
        v = json.loads(out.stdout.strip().splitlines()[-1])
    except (ValueError, IndexError):
        return f"the trial link couldn't be re-run: {(out.stdout + out.stderr)[-300:]}"  # not cached
    expected = SHA1.read_text().split()[0]
    why = None
    if v["dol_sha1"] != expected:
        why = f"re-running the trial link gives DOL SHA-1 {v['dol_sha1']}, not build.sha1"
    elif v["map_sha1"] != rec["map_sha1"]:
        why = "re-running the trial link gives another map"
    elif v["object_sha1"] != obj_sha or rec["objects"].get(base) != obj_sha:
        why = "the object isn't the one the trial link included"
    elif v["inputs_sha1"] != inputs_sha:
        why = "the trial link now has other inputs"
    state = key.split()[0]
    cache = {k: v for k, v in cache.items() if k.split()[0] == state}  # drop other link states
    cache[key] = why
    tmp = VERIFIED.with_suffix(".tmp")
    tmp.write_text(json.dumps(cache, indent=1))
    tmp.replace(VERIFIED)
    return why


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


SHT_PROGBITS, SHF_ALLOC, SHF_EXECINSTR = 1, 2, 4
UNUSED_LINE = re.compile(r"^\s+UNUSED\s+([0-9a-f]+) \.+ (\S+) (\S+)\s*$")


def dead_data(elf, name, unused, placed, starts, notes, unit, extra=None):
    """Data the link dead-stripped from this object: (removals, entries), where
    removals = {section index: [(offset, size)]}. A data object (a sized,
    non-function symbol in an allocated PROGBITS section other than .text,
    extab and extabindex) is removed only when
      * its name is unique among the object's symbols, and the map lists it
        `UNUSED` with its size, for this object, in its section's layout;
      * every allocated location that references it (a relocation whose target,
        symbol value plus addend, lies in it) lies in a symbol the map also
        lists `UNUSED` for this object in that section's layout (a dead
        function, or other dead data), so no live code or data uses it;
      * the bytes up to the next symbol (or the section end) are zero padding
        with no relocation in them,
        removed with it, and the map places every kept symbol of the section
        at the offset (from the object's section start) and size it has in
        the stripped copy, so the copy has the layout the link gave it;
      * no relocation from live code or data reaches the section through its
        section symbol at or past the first removed byte (its addend would
        have to move); a size-0 label in the section (an anchor such as
        `...data.0`, which MWCC uses with offsets computed in code) that any
        allocated relocation targets must itself be listed UNUSED (size 0)
        for this object, and so must every symbol that references it;
      * in the map's layout of this section, the next object's section
        starts 0 to 7 bytes (alignment) after this object's start plus the
        stripped size, so the removed tail is really absent from the link.
    References from debug sections (not allocated) don't keep data alive.
    `extra` = {section: [(offset, size)]}: weak data removed from the same
    sections (weak_data()), part of the layout the map must confirm."""
    count = defaultdict(int)
    for s in elf.syms:
        count[s["name"]] += 1

    def listed(sec, sym):
        """The map's UNUSED line for this symbol, if it proves the symbol was stripped."""
        if count[sym["name"]] != 1:
            return None
        u = unused.get((elf.names[sec], name, sym["name"]))
        if not u or any(b[2] == sym["name"] for b in placed.get((elf.names[sec], name), [])):
            return None
        m = UNUSED_LINE.match(u[1])
        return u if m and int(m[1], 16) == sym["size"] else None

    def container(sec, off):
        for s in elf.syms:
            if s["shndx"] == sec and s["size"] and s["value"] <= off < s["value"] + s["size"] \
                    and s["type"] in (1, STT_FUNC):
                return s
        return None

    def data_section(i):
        h = elf.sh[i]
        return (h[1] == SHT_PROGBITS and h[2] & SHF_ALLOC and not h[2] & SHF_EXECINSTR
                and elf.names[i] not in ("extab", "extabindex"))

    def ev(line):
        return {"line": line[0], "text": line[1]}

    cands = {}  # (section, offset) -> symbol
    for s in elf.syms:
        if s["shndx"] < elf.shnum and data_section(s["shndx"]) and s["size"] \
                and s["type"] != STT_FUNC and s["type"] != STT_SECTION and listed(s["shndx"], s):
            cands[(s["shndx"], s["value"])] = s
    if not cands:
        return {}, []
    refs = defaultdict(list)  # candidate key -> [(section, referencing symbol or None)]
    live_section_refs = defaultdict(list)  # section -> offsets live code reaches via the section symbol
    anchors = defaultdict(dict)  # section -> {size-0 label's index: [(section, referencing symbol, dead)]}
    for r in range(elf.shnum):
        if not elf.sh[r][2] & SHF_ALLOC:
            continue
        for off, si, _, add in elf.relocs(r):
            t = elf.syms[si]
            if t["shndx"] >= elf.shnum or not data_section(t["shndx"]):
                continue
            at = t["value"] + add
            hits = [k for k, c in cands.items() if k[0] == t["shndx"] and
                    (c is t or c["value"] <= at < c["value"] + c["size"])]
            src = container(r, off)
            dead = src is not None and listed(r, src)
            if (t["type"] == STT_SECTION or not t["size"]) and not dead:
                live_section_refs[t["shndx"]].append(at)
            if t["type"] != STT_SECTION and not t["size"]:
                anchors[t["shndx"]].setdefault(si, []).append((r, src, dead))
            for k in hits:
                refs[k].append((r, src, dead))
    removals, entries = defaultdict(list), []
    for (sec, val), s in sorted(cands.items(), key=lambda x: x[0]):
        if any(not dead for _, _, dead in refs[(sec, val)]):
            who = sorted({src["name"] if src else f"{elf.names[r]}+?" for r, src, dead in refs[(sec, val)] if not dead})
            notes.append(f"{unit}: {s['name']} kept: the map lists it UNUSED but live {', '.join(who)} references it")
            continue
        nxt = min([x["value"] for x in elf.syms if x["shndx"] == sec and x["size"] and x["value"] >= val + s["size"]]
                  + [elf.sh[sec][5]])
        o = elf.sh[sec][4]
        if any(elf.d[o + val + s["size"]:o + nxt]) or any(val + s["size"] <= off < nxt for off, *_ in elf.relocs(sec)):
            notes.append(f"{unit}: {s['name']} kept: non-zero bytes or a relocation between it and the next symbol")
            continue
        removals[sec].append((val, nxt - val))
        entries.append({"section": elf.names[sec], "symbol": s["name"], "offset": val, "size": s["size"],
                        "removed": nxt - val, "evidence": [ev(listed(sec, s))],
                        "referencers": [{"section": elf.names[r], "symbol": src["name"], "evidence": ev(dead)}
                                        for r, src, dead in refs[(sec, val)]]})
    for sec, rem in list(removals.items()):
        why, anchor_ev = None, []
        for si, rs in sorted(anchors[sec].items()):
            a = elf.syms[si]
            if not listed(sec, a) or not all(dead for _, _, dead in rs):
                why = f"its size-0 label {a['name'] or '?'} is referenced, and it or a referencer isn't listed UNUSED"
                break
            anchor_ev.append({"symbol": a["name"], "evidence": ev(listed(sec, a)),
                              "referencers": [{"section": elf.names[r], "symbol": src["name"], "evidence": ev(dead)}
                                              for r, src, dead in rs]})
        why = why or layout_problem(elf, sec, rem + (extra or {}).get(sec, []), placed.get((elf.names[sec], name), []),
                                    live_section_refs[sec], starts.get(elf.names[sec], []), name)
        if isinstance(why, str):
            notes.append(f"{unit}: dead data in {elf.names[sec]} kept: {why}")
            del removals[sec]
            entries = [e for e in entries if e["section"] != elf.names[sec]]
            continue
        for e in entries:
            if e["section"] == elf.names[sec]:
                e["layout"] = why
                e["anchors"] = anchor_ev
    return dict(removals), entries


def layout_problem(elf, sec, rem, block, live_refs, order, name):
    """Why removing `rem` from section `sec` wouldn't give the layout the link
    gave this object's section, or the map lines that prove it does: every kept
    sized symbol must sit, in the stripped copy, at the offset from the
    object's section start the map places it at, with the map's size."""
    first = min(a for a, _ in rem)
    if any(at >= first for at in live_refs):
        return "live code reaches the section through its section symbol past a removed object"
    secname = elf.names[sec]
    starts = [b for b in block if b[2] == secname]
    if len(starts) != 1 or block[0] is not starts[0]:
        return f"the map doesn't place this object's {secname} exactly once"
    start = starts[0][0]
    names = defaultdict(list)
    for b in block[1:]:
        names[b[2]].append(b)
    lines = [{"line": starts[0][3], "text": starts[0][4]}]
    for x in elf.syms:
        if x["shndx"] != sec or not x["size"] or x["type"] == STT_SECTION:
            continue
        removed = any(a <= x["value"] < a + n for a, n in rem)
        if removed:
            if names.get(x["name"]):
                return f"the map places {x['name']}, which would be removed"
            continue
        new = x["value"] - sum(n for a, n in rem if a + n <= x["value"])
        got = names.get(x["name"], [])
        if len(got) != 1 or got[0][0] - start != new or got[0][1] != x["size"]:
            return f"the map doesn't place {x['name']} at 0x{new:x} (size 0x{x['size']:x}) of the stripped section"
        lines.append({"line": got[0][3], "text": got[0][4]})
    # The end: the next object's section in this layout follows the stripped size.
    idx = [i for i, o in enumerate(order) if o[1] == name]
    if len(idx) != 1 or idx[0] + 1 >= len(order):
        return f"the map has no object after this one's {secname} to bound its end"
    nxt = order[idx[0] + 1]
    gap = nxt[0] - (start + elf.sh[sec][5] - sum(n for _, n in rem))
    if not 0 <= gap < 8:
        return f"the next object ({nxt[1]}) starts {gap} bytes after the stripped end, not within alignment"
    lines.append({"line": nxt[2], "text": nxt[3]})
    return lines


STB_LOCAL, STT_OBJECT = 0, 1


def symbol_relocs(elf, s):
    """The relocations inside data object s as (offset in s, type, target name,
    addend), sorted, or None if one targets a local, section or unnamed symbol
    (a name that means nothing outside this object)."""
    out = []
    for off, si, typ, add in elf.relocs(s["shndx"]):
        if s["value"] <= off < s["value"] + s["size"]:
            t = elf.syms[si]
            if t["bind"] == STB_LOCAL or t["type"] == STT_SECTION or not t["name"]:
                return None
            out.append((off - s["value"], typ, t["name"], add))
    return sorted(out)


def weak_data(elf, name, link, notes, unit):
    """Weak data objects the link kept from another object: (removals, entries),
    removals = {section index: [(offset, size)]}, without the layout check (see
    dedup_unit). A data object (a sized symbol in an allocated PROGBITS section
    other than .text, extab and extabindex) is removed only when
      * it is WEAK OBJECT in this object, and its name is unique among the
        object's symbols;
      * the map's link tree lists `<name> (object,...) found in <kept>`, then
        `>>> UNREFERENCED DUPLICATE <name>` and `>>> (object,weak) found in
        <this object>`, with one kept object for every tree node that lists it,
        and <kept> is another, uniquely named input of the same link (not
        newer than ninja's map);
      * the map places the kept copy, with this size, in <kept>'s layout of
        the same section, and <kept>'s symbol of that name (exactly one,
        defined) has the same section name, size, bytes and relocations
        (offset, type, target name, addend; no local or section targets);
      * every allocated relocation whose target (symbol value plus addend)
        lies in it or its padding targets the symbol itself, by name: the
        link resolves those to the kept copy, so they stay;
      * the bytes up to the next symbol (or the section end) are zero padding
        with no relocation in them,
        removed with it.
    In the stripped copy its symbol becomes undefined, as in dtk's split of
    the original, which references the kept copy as an external symbol."""
    count = defaultdict(int)
    for s in elf.syms:
        count[s["name"]] += 1

    def data_section(i):
        h = elf.sh[i]
        return (h[1] == SHT_PROGBITS and h[2] & SHF_ALLOC and not h[2] & SHF_EXECINSTR
                and elf.names[i] not in ("extab", "extabindex"))

    def ev(line):
        return {"line": line[0], "text": line[1]}

    wobjs = link.weak_objects()
    _, unused, placed, _ = link.parsed()
    removals, entries = defaultdict(list), []
    for s in elf.syms:
        if not (s["shndx"] < elf.shnum and data_section(s["shndx"]) and s["size"]
                and s["bind"] == STB_WEAK and s["type"] == STT_OBJECT):
            continue
        sec, secname = s["shndx"], elf.names[s["shndx"]]
        nodes = wobjs.get((name, s["name"]))
        if not nodes:
            continue
        why = None
        kept = {k for k, _ in nodes}
        if count[s["name"]] != 1:
            why = "its name isn't unique in the object"
        elif (secname, name, s["name"]) in unused:
            why = "the map also lists it UNUSED for this object (dead data, not a weak duplicate)"
        elif len(kept) != 1 or name in kept:
            why = f"the map's kept copies are in {sorted(kept)}"
        if why is None:
            kept_name = kept.pop()
            kept_path, why = link.kept_object(kept_name)
        if why is None:
            ks = None
            try:
                ke = Elf(Path(kept_path).read_bytes())
                ks = [x for x in ke.syms if x["name"] == s["name"] and 0 < x["shndx"] < ke.shnum]
            except (OSError, ValueError, StopIteration) as e:
                why = f"{kept_path}: {e}"
            if why is None and (len(ks) != 1 or ke.names[ks[0]["shndx"]] != secname or ks[0]["size"] != s["size"]):
                why = f"{kept_path} doesn't define exactly one {secname} {s['name']} of size 0x{s['size']:x}"
        if why is None:
            ks = ks[0]
            mine = elf.d[elf.sh[sec][4] + s["value"]:][:s["size"]]
            theirs = ke.d[ke.sh[ks["shndx"]][4] + ks["value"]:][:ks["size"]]
            if mine != theirs:
                why = f"its bytes differ from the kept copy in {kept_path}"
            elif symbol_relocs(elf, s) is None or symbol_relocs(elf, s) != symbol_relocs(ke, ks):
                why = f"its relocations differ from the kept copy's in {kept_path} (or name a local symbol)"
        if why is None:
            kp = [b for b in placed.get((secname, kept_name), []) if b[2] == s["name"]]
            if len(kp) != 1 or kp[0][1] != s["size"]:
                why = f"the map doesn't place the kept copy in {kept_name}'s {secname} with size 0x{s['size']:x}"
        nxt = min([x["value"] for x in elf.syms if x["shndx"] == sec and x["size"] and x["value"] >= s["value"] + s["size"]]
                  + [elf.sh[sec][5]])
        if why is None:
            o = elf.sh[sec][4]
            if any(elf.d[o + s["value"] + s["size"]:o + nxt]) or \
                    any(s["value"] + s["size"] <= off < nxt for off, *_ in elf.relocs(sec)):
                why = "non-zero bytes or a relocation between it and the next symbol"
        refs = 0
        if why is None:
            for r in range(elf.shnum):
                if not elf.sh[r][2] & SHF_ALLOC:
                    continue
                for off, si, _, add in elf.relocs(r):
                    t = elf.syms[si]
                    if t["shndx"] != sec or not s["value"] <= t["value"] + add < nxt:
                        continue
                    if r == sec and s["value"] <= off < s["value"] + s["size"]:
                        continue  # inside it: compared with the kept copy, removed with it
                    if t is not s:
                        why = f"{elf.names[r]}+0x{off:x} reaches it through {t['name'] or 'the section symbol'}, not by its name"
                        break
                    refs += 1
                if why:
                    break
        if why:
            notes.append(f"{unit}: weak {s['name']} kept: {why}")
            continue
        removals[sec].append((s["value"], nxt - s["value"]))
        entries.append({"section": secname, "symbol": s["name"], "offset": s["value"], "size": s["size"],
                        "removed": nxt - s["value"], "references": refs,
                        "kept_object": kept_path, "kept_object_sha1": sha1(kept_path),
                        "kept_placed": {"line": kp[0][3], "text": kp[0][4]},
                        "evidence": [ev(x) for x in nodes[0][1]]})
    return dict(removals), entries


def weak_layout_problem(elf, sec, rem, block, starts, name):
    """layout_problem for a section that loses weak data: nothing may reach the
    section through its section symbol or a size-0 label at or past the first
    removed byte (dead code included: its addend wouldn't move)."""
    first = min(a for a, _ in rem)
    for r in range(elf.shnum):
        if not elf.sh[r][2] & SHF_ALLOC:
            continue
        for off, si, _, add in elf.relocs(r):
            t = elf.syms[si]
            if t["shndx"] == sec and (t["type"] == STT_SECTION or not t["size"]) and t["value"] + add >= first:
                return f"{elf.names[r]}+0x{off:x} reaches it through {t['name'] or 'its section symbol'} past a removed object"
    return layout_problem(elf, sec, rem, block, [], starts, name)


def strip(elf, removals, undef=()):
    """Remove byte ranges {section: [(offset, size)]}, with their relocations;
    later symbols and relocations move down, removed symbols become absolute 0,
    or undefined if their symbol table offset is in undef (weak data whose
    kept copy is in another object)."""
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
                # SHN_UNDEF: resolved elsewhere; SHN_ABS: out of the section
                struct.pack_into(">H", d, s["off"] + 14, 0 if s["off"] in undef else 0xFFF1)
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
    why = link.proves(base) if link else "no link includes this object"
    if why:
        if entries:
            notes.append(f"{unit}: {len(entries)} weak extab entries kept: {why}")
        return None, []
    dups, unused, placed, starts = link.parsed()
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
            "kind": "weak_extab",
            "unit": unit, "object": base, "object_sha1": sha1(base), "function": fn["name"],
            "extabindex": eti["name"], "extab": etb["name"], "extab_size": etb["size"],
            "link": getattr(link, "evidence_file", None) or link.kind,
            "evidence": [{"line": n, "text": t} for n, t in (*dup, u_eti, u_etb)],
        })
    if len(found) < len(entries):
        notes.append(f"{unit}: {len(entries) - len(found)} weak extab entries kept: "
                     f"{link.map} has no discarded-duplicate evidence for them")
    # Weak data first: dead data in the same section is checked against the
    # layout without both, and a section whose weak removals the map doesn't
    # confirm keeps its weak data (dead data is then checked again without it).
    weak_rem, weak_found = weak_data(elf, name, link, notes, unit)
    while True:
        dd_notes = []
        data_removals, data_found = dead_data(elf, name, unused, placed, starts, dd_notes, unit, weak_rem)
        bad = {}
        for sec, rem in weak_rem.items():
            why = weak_layout_problem(elf, sec, rem + data_removals.get(sec, []),
                                      placed.get((elf.names[sec], name), []), starts.get(elf.names[sec], []), name)
            if isinstance(why, str):
                bad[sec] = why
            else:
                for e in weak_found:
                    if e["section"] == elf.names[sec]:
                        e["layout"] = why
        if not bad:
            break
        for sec, why in bad.items():
            notes.append(f"{unit}: weak data in {elf.names[sec]} kept: {why}")
            del weak_rem[sec]
            weak_found = [e for e in weak_found if e["section"] != elf.names[sec]]
    notes += dd_notes
    for sec, rem in list(data_removals.items()) + list(weak_rem.items()):
        removals[sec] += rem
    for e in data_found:
        found.append({"kind": "dead_data", "unit": unit, "object": base, "object_sha1": sha1(base),
                      "function": None, **e, "link": getattr(link, "evidence_file", None) or link.kind})
    for e in weak_found:
        found.append({"kind": "weak_data", "unit": unit, "object": base, "object_sha1": sha1(base),
                      "function": None, **e, "link": getattr(link, "evidence_file", None) or link.kind})
    if not found:
        return None, []
    undef = {s["off"] for s in elf.syms if any(e["symbol"] == s["name"] for e in weak_found)
             and s["shndx"] in weak_rem and s["size"]}
    out = STRIPPED / Path(base).relative_to(BUILD)
    out.parent.mkdir(parents=True, exist_ok=True)
    tmp = out.with_suffix(".tmp")
    tmp.write_bytes(strip(elf, removals, undef))
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
    data = [e for e in entries if e["kind"] == "dead_data"]
    weak = [e for e in entries if e["kind"] == "weak_data"]
    print(f"weakdedup: {len(entries) - len(data) - len(weak)} extab entries of discarded weak duplicates, "
          f"{len(weak)} discarded weak data objects and "
          f"{len(data)} dead-stripped data objects removed from {units} units ({MANIFEST})", file=sys.stderr)


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
