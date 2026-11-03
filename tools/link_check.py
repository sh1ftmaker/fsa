#!/usr/bin/env python3
"""Which Matching units link into byte-identical main.dol bytes?

`ninja` links every Matching unit's compiled object and checks the DOL's
SHA-1. When that fails it says nothing about *which* unit is at fault; this
tool does. It links by hand (outside ninja, into build/link_check/), starting
from the same response file and linker script:

  1. Every Matching unit whose compiled object breaks the link (duplicate
     symbols: the source defines functions that sit outside the unit's
     splits; undefined symbols: the compiled object does not define a symbol
     the rest of the DOL references by its symbols.txt name) is swapped for
     dtk's split of the original bytes, and the link is retried.
  2. The linked DOL is compared with orig/sys/main.dol over each Matching
     unit's split ranges (splits.txt). A unit that differs is swapped out
     too and the link repeated, so a size change in one unit doesn't make
     every later unit look wrong.
  3. It stops when the DOL's SHA-1 equals build.sha1, i.e. every unit still
     linked from its compiled object is proven byte-identical in place.

Run `ninja build/G4SE01/config.json all_source` first (`ninja` does both).
Needs GNU nm on PATH.

Usage:
  python3 tools/link_check.py            # table of units and the final SHA-1
  python3 tools/link_check.py --json out.json
  python3 tools/link_check.py --unit game/tu_80214244.cpp

--unit links the current Matching set with one more unit's compiled object
in place of dtk's split of the original (a NonMatching unit before its
flip). Every link here writes a map with -mapunused. If the trial DOL's
SHA-1 matches build.sha1, the map, DOL, link inputs and the object's SHA-1
are recorded in build/G4SE01/weakdedup_evidence/ as the proof
tools/honest_report.py needs to leave out the extab entries of weak
duplicates the link discarded (CLAUDE.md, "Measuring progress"); otherwise
any earlier evidence for the unit is removed.

Measuring is untouched: this reads objdiff.json for the Matching flags and
writes only under build/link_check/.
"""

import argparse
import hashlib
import json
import re
import struct
import subprocess
import sys
from pathlib import Path

VERSION = "G4SE01"
BUILD = Path("build") / VERSION
OUT = Path("build") / "link_check"
SPLITS = Path("config") / VERSION / "splits.txt"
SHA1 = Path("config") / VERSION / "build.sha1"
ORIG_DOL = Path("orig/sys/main.dol")


def linker_cmd():
    """The link command ninja would run (fully expanded), from build.ninja."""
    elf = BUILD / "fsa_game_code.elf"
    out = subprocess.run(["ninja", "-t", "commands", str(elf)], capture_output=True, text=True, check=True).stdout
    cmd = out.strip().splitlines()[-1]
    if "mwldeppc" not in cmd:
        sys.exit(f"unexpected link command: {cmd}")
    return cmd


def link_inputs():
    """The link's object list, in order, from build.ninja (ninja deletes the .rsp)."""
    text = Path("build.ninja").read_text().replace("$\n", "")
    # (the map is an implicit output: `build <elf> | <elf>.MAP: link ...`)
    m = re.search(rf"^build {re.escape(str(BUILD / 'fsa_game_code.elf'))}(?: \|[^:]*)?:\s+link\s+(.*?)(?:\||$)", text, re.M)
    if not m:
        sys.exit("no link step in build.ninja; run configure.py")
    return m.group(1).split()


def read_dol(path):
    d = path.read_bytes()
    offs = struct.unpack(">18I", d[0:72])
    addrs = struct.unpack(">18I", d[72:144])
    sizes = struct.unpack(">18I", d[144:216])
    return d, [(addrs[i], sizes[i], offs[i]) for i in range(18) if sizes[i]]


def dol_bytes(dol, start, end):
    data, secs = dol
    for addr, size, off in secs:
        if addr <= start and end <= addr + size:
            return data[off + start - addr: off + end - addr]
    return None  # bss or out of range


def parse_splits():
    units, cur = {}, None
    for line in SPLITS.read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            cur = line[:-1]
            units[cur] = []
        elif cur and "start:" in line:
            m = re.match(r"\s*(\S+)\s+start:0x([0-9A-Fa-f]+)\s+end:0x([0-9A-Fa-f]+)", line)
            units[cur].append((m[1], int(m[2], 16), int(m[3], 16)))
    return units


def defined_symbols(obj):
    out = subprocess.run(["nm", "-g", "--defined-only", str(obj)], capture_output=True, text=True).stdout
    return {l.split(None, 2)[2] for l in out.splitlines() if len(l.split(None, 2)) == 3}


def link(cmd, objects, tag):
    OUT.mkdir(parents=True, exist_ok=True)
    rsp = OUT / f"{tag}.rsp"
    rsp.write_text("\n".join(objects) + "\n")
    elf = OUT / f"{tag}.elf"
    elf.unlink(missing_ok=True)
    c = re.sub(r" -map \S+", "", cmd)  # ninja's own map, when configure.py asks for one
    c = re.sub(r"-o \S+", f"-o {elf} -map {OUT / (tag + '.map')}", c)
    if "-mapunused" not in c.split():
        c = c.replace(" -o ", " -mapunused -o ", 1)
    c = re.sub(r"@\S+", f"@{rsp}", c)
    p = subprocess.run(c, shell=True, capture_output=True, text=True)
    return elf if p.returncode == 0 and elf.exists() else None, p.stdout + p.stderr


def map_layout(path):
    """(section, object basename, address, size) for every object in the link map."""
    rows, sec = [], None
    for line in path.read_text(errors="replace").splitlines():
        m = re.match(r"^(\S+) section layout$", line)
        if m:
            sec = m[1]
            continue
        if line.startswith("Memory map"):
            break
        m = re.match(r"^\s+[0-9a-f]{8} ([0-9a-f]{6,8}) ([0-9a-f]{8})\s+\d+ (\S+)\s+(\S+)", line)
        if sec and m and m[3] == sec:
            rows.append((sec, m[4], int(m[2], 16), int(m[1], 16)))
    return rows


def expected_starts(objects, splits):
    """Object basename -> {section: set of original start addresses}."""
    by_stem = {re.sub(r"\.[^./]+$", "", u): v for u, v in splits.items()}
    exp = {}
    for o in objects:
        base = Path(o).name
        m = re.match(r"auto_(?:\d\d|fn)_([0-9A-F]{8})_(\w+)\.o$", base)
        d = exp.setdefault(base, {})
        if m:
            sec = m[2]
            sec = sec if sec in ("extab", "extabindex") else "." + sec
            d.setdefault(sec, set()).add(int(m[1], 16))
            continue
        stem = re.sub(r"^build/[^/]+/(?:obj|src)/", "", o)[:-2]
        for sec, start, _ in by_stem.get(stem, []):
            d.setdefault(sec, set()).add(start)
    dupes = {b for b in exp if sum(Path(o).name == b for o in objects) > 1}
    return {b: d for b, d in exp.items() if b not in dupes}


def size_note(row, splits, unit):
    sec, _, _, size = row
    want = [e - s for s_, s, e in splits.get(unit, []) if s_ == sec]
    return f"size 0x{size:X}" + (f" (split 0x{want[0]:X})" if want else "")


def parse_errors(log):
    errs = []
    for block in log.split("Linker Error:")[1:]:
        text = " ".join(l.lstrip("#").strip() for l in block.splitlines() if l.startswith("#"))
        m = re.search(r"multiply-defined: '(.*?)' in (\S+)\s+Previously defined in (\S+)", text)
        if m:
            errs.append(("dup", m[1], [m[2], m[3]]))
            continue
        m = re.search(r"undefined: '(.*?)'\s+Referenced from '.*?' in (\S+)", text)
        if m:
            errs.append(("undef", m[1], [m[2]]))
    return errs


EVIDENCE = BUILD / "weakdedup_evidence"


def sha1_file(path):
    return hashlib.sha1(Path(path).read_bytes()).hexdigest()


def trial_link(unit_arg, cmd, objects, cfg):
    """Link with one unit's compiled object in; record the map as evidence if the SHA-1 matches."""
    stem = re.sub(r"\.[^./]+$", "", unit_arg)
    unit = next((u for u in cfg["units"] if unit_arg in (u.get("metadata", {}).get("source_path"), u["name"])
                 or u["name"].split("/", 1)[-1] == stem), None)
    if not unit or not unit.get("base_path"):
        sys.exit(f"{unit_arg}: no such unit with source in objdiff.json")
    base = unit["base_path"]
    if not Path(base).is_file():
        sys.exit(f"{base} missing: run ninja build/G4SE01/report_honest.json first")
    slug = unit["name"].split("/", 1)[-1].replace("/", "__")
    if base not in objects:
        split = str(BUILD / "obj" / (unit["name"].split("/", 1)[-1] + ".o"))
        if split not in objects:
            sys.exit(f"neither {base} nor {split} is a link input")
        objects = [base if o == split else o for o in objects]
    obj_sha = sha1_file(base)
    tag = f"trial_{slug}"
    elf, log = link(cmd, objects, tag)
    expected = SHA1.read_text().split()[0]
    sha = None
    if elf:
        dol = OUT / f"{tag}.dol"
        subprocess.run([str(Path("build/tools/dtk")), "elf2dol", str(elf), str(dol)], check=True)
        sha = hashlib.sha1(dol.read_bytes()).hexdigest()
    EVIDENCE.mkdir(parents=True, exist_ok=True)
    rec_path = EVIDENCE / f"{slug}.json"
    for old in (rec_path, EVIDENCE / f"{slug}.MAP", EVIDENCE / f"{slug}.dol"):
        old.unlink(missing_ok=True)
    Path("build/G4SE01/report_honest.json").unlink(missing_ok=True)  # ninja rewrites it with the new evidence
    if sha != expected or sha1_file(base) != obj_sha:
        why = "link failed:\n" + log[-2000:] if not elf else (
            f"DOL SHA-1 {sha} != build.sha1 {expected}" if sha != expected else f"{base} changed during the link")
        print(f"{unit['name']}: trial link with {base} is not byte-identical: {why}\nno evidence recorded")
        return 1
    m, d = EVIDENCE / f"{slug}.MAP", EVIDENCE / f"{slug}.dol"
    (OUT / f"{tag}.map").replace(m)
    dol.replace(d)
    rec = {"unit": unit["name"], "objects": {base: obj_sha}, "inputs": objects,
           "map": str(m), "map_sha1": sha1_file(m), "dol": str(d), "dol_sha1": sha}
    tmp = rec_path.with_suffix(".tmp")
    tmp.write_text(json.dumps(rec, indent=1))
    tmp.replace(rec_path)
    print(f"{unit['name']}: links byte-identical with {base} (SHA-1 {sha}); "
          f"evidence in {rec_path}\nnow run: ninja build/G4SE01/report_honest.json")
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--json", help="write per-unit results here")
    ap.add_argument("--unit", help="trial-link this unit's compiled object and record weak-dedup evidence")
    args = ap.parse_args()

    cmd = linker_cmd()
    objects = link_inputs()
    cfg = json.loads(Path("objdiff.json").read_text())
    if args.unit:
        return trial_link(args.unit, cmd, objects, cfg)
    matching = {}  # compiled object path -> unit info
    for u in cfg["units"]:
        if u.get("metadata", {}).get("complete") and u.get("base_path"):
            matching[u["base_path"]] = {
                "name": u["metadata"].get("source_path", u["name"]),
                "unit": u["name"].split("/", 1)[1],
                "target": u["target_path"],
            }
    missing = [p for p in matching if p not in objects]
    if missing:
        sys.exit(f"Matching objects not in the link order: {missing[:5]}")
    splits = parse_splits()
    target_syms = {p: defined_symbols(m["target"]) for p, m in matching.items()}
    orig = read_dol(ORIG_DOL)
    expected = SHA1.read_text().split()[0]

    exp_starts = expected_starts(objects, splits)
    swapped = {}  # compiled path -> reason
    rnd = 0
    while True:
        rnd += 1
        objs = [matching[o]["target"] if o in swapped else o for o in objects]
        live = {p for p in matching if p not in swapped}
        by_base = {}
        for p in live:
            by_base.setdefault(Path(p).name, []).append(p)
        elf, log = link(cmd, objs, "link")
        if elf is None:
            errs = parse_errors(log)
            if not errs:
                print(log)
                sys.exit("link failed without errors this tool understands")
            blamed = 0
            for kind, sym, objs_named in errs:
                culprits = [p for o in objs_named for p in by_base.get(o, [])]
                if kind == "undef":
                    culprits += [p for p in live if sym in target_syms[p]]
                for p in culprits:
                    if p not in swapped:
                        swapped[p] = f"link: {kind} {sym}"
                        blamed += 1
            print(f"round {rnd}: link failed, {len(errs)} errors, swapped out {blamed} units", file=sys.stderr)
            if not blamed:
                print(log)
                sys.exit("link errors not attributable to a Matching unit")
            continue

        dol_path = OUT / "link.dol"
        subprocess.run([str(Path("build/tools/dtk")), "elf2dol", str(elf), str(dol_path)], check=True)
        sha = hashlib.sha1(dol_path.read_bytes()).hexdigest()
        if sha == expected:
            print(f"round {rnd}: linked, sha1 matches", file=sys.stderr)
            break

        # Layout: the first object in each section that sits at the wrong
        # address was pushed there by the one before it (wrong size or
        # alignment, including .bss, which has no bytes to compare).
        blame = {}
        rows = map_layout(OUT / "link.map")
        done = set()
        for i, (sec, base, addr, size) in enumerate(rows):
            want = exp_starts.get(base, {}).get(sec)
            if sec in done or not want or addr in want:
                continue
            done.add(sec)
            prev = rows[i - 1] if i and rows[i - 1][0] == sec else None
            for p in (by_base.get(prev[1], []) if prev else []):
                blame.setdefault(p, f"layout: {sec} {size_note(prev, splits, matching[p]['unit'])}, "
                                    f"next object {base} at 0x{addr:08X} instead of 0x{min(want):08X}")
            for p in by_base.get(base, []):
                blame.setdefault(p, f"layout: {sec} at 0x{addr:08X} instead of 0x{min(want):08X}")
            if not blame:
                print(f"misplaced {base} {sec} at 0x{addr:08X} (want 0x{min(want):08X}), "
                      f"after {prev[1] if prev else '?'}: not a Matching unit", file=sys.stderr)
        if blame:
            for p, why in blame.items():
                swapped[p] = why
            print(f"round {rnd}: linked, layout differs, swapped out {len(blame)} units", file=sys.stderr)
            continue

        linked = read_dol(dol_path)
        bad = []
        for p in sorted(live):
            unit = matching[p]["unit"]
            for sec, start, end in splits.get(unit, []):
                a, b = dol_bytes(orig, start, end), dol_bytes(linked, start, end)
                if a != b:
                    first = next((i for i in range(min(len(a or b""), len(b or b""))) if a[i] != b[i]), 0)
                    bad.append((p, f"bytes: {sec} differs at 0x{start + first:08X}"))
                    break
        print(f"round {rnd}: linked, sha1 {sha}, {len(bad)} units differ in bytes", file=sys.stderr)
        if not bad:
            break
        for p, why in bad:
            swapped[p] = why

    # Blame is approximate (an undefined weak symbol blames every unit whose
    # original defines it), so give each swapped unit one more try on its own.
    if sha == expected:
        for p in sorted(swapped, key=lambda p: matching[p]["name"]):
            trial = {q: r for q, r in swapped.items() if q != p}
            objs = [matching[o]["target"] if o in trial else o for o in objects]
            elf, _ = link(cmd, objs, "verify")
            if elf is None:
                continue
            dol = OUT / "verify.dol"
            subprocess.run([str(Path("build/tools/dtk")), "elf2dol", str(elf), str(dol)], check=True)
            if hashlib.sha1(dol.read_bytes()).hexdigest() == expected:
                print(f"{matching[p]['name']}: links identical on its own, restored", file=sys.stderr)
                swapped = trial

    rows = []
    for p, m in sorted(matching.items(), key=lambda kv: kv[1]["name"]):
        rows.append({"unit": m["name"], "identical": p not in swapped, "reason": swapped.get(p, "")})
    for r in rows:
        print(f"{'OK  ' if r['identical'] else 'FAIL'} {r['unit']}  {r['reason']}")
    ok = sum(r["identical"] for r in rows)
    print(f"\n{ok}/{len(rows)} Matching units link byte-identical; "
          f"final DOL sha1 {sha} {'== build.sha1' if sha == expected else '!= build.sha1 ' + expected}")
    if args.json:
        Path(args.json).write_text(json.dumps({"sha1": sha, "expected": expected, "units": rows}, indent=1))
    return 0 if ok == len(rows) and sha == expected else 1


if __name__ == "__main__":
    sys.exit(main())
