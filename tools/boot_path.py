#!/usr/bin/env python3
"""Rank the units executed on the boot path (boot -> title -> main menu -> Shadow Battle).

Input: per-phase coverage dumps written by the patched Dolphin (`covdump` ctl command):
one byte per guest instruction of MEM1 (0x80000000 + 4*i), non-zero = executed.
A phase may be the union of several dumps (`a.bin+b.bin`).

    python3 tools/boot_path.py                       # default dumps in ../bootpath-data/cov
    python3 tools/boot_path.py --phase "boot->title=p1.bin" --phase ... --out docs/BOOT_PATH.md

Executed-but-unmatched bytes = executed instruction bytes inside functions whose
honest fuzzy_match_percent (build/G4SE01/report_honest.json) is below 100.
Rerun after `ninja build/G4SE01/report_honest.json` to refresh the match columns.
"""
import argparse
import json
import os
import re
import sys
from bisect import bisect_right

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_COV = os.path.join(os.path.dirname(ROOT), "bootpath-data", "cov")
DEFAULT_PHASES = [
    ("boot -> title", "p1_boot_title.bin"),
    ("title -> main menu", "p2_title_menu.bin"),
    ("main menu -> Shadow Battle start", "p3a_menu_gbaconnect.bin+p3b_connect_battle.bin"),
]
MEM1 = 0x80000000


def load_cov(spec, covdir):
    cov = None
    for part in spec.split("+"):
        path = part if os.path.isabs(part) else os.path.join(covdir, part)
        data = open(path, "rb").read()
        cov = bytearray(data) if cov is None else bytearray(a | b for a, b in zip(cov, data))
    return cov


def parse_splits(path):
    """unit -> list of (start, end) code ranges (.init/.text)."""
    units, cur = {}, None
    for line in open(path):
        if not line.strip() or line.startswith("Sections:"):
            cur = None if not line.strip() else cur
            continue
        if not line[0].isspace():
            cur = line.split(":")[0].strip()
            units[cur] = []
            continue
        m = re.match(r"\s+(\.init|\.text)\s+start:(0x[0-9A-Fa-f]+)\s+end:(0x[0-9A-Fa-f]+)", line)
        if m and cur:
            units[cur].append((int(m.group(2), 16), int(m.group(3), 16)))
    return {u: r for u, r in units.items() if r}


def parse_symbols(path):
    funcs = []
    pat = re.compile(r"^(\S+) = (\.init|\.text):(0x[0-9A-Fa-f]+); // type:function size:(0x[0-9A-Fa-f]+)")
    for line in open(path):
        m = pat.match(line)
        if m:
            funcs.append((int(m.group(3), 16), int(m.group(4), 16), m.group(1)))
    funcs.sort()
    return funcs


def load_report(path):
    """function address -> fuzzy %, unit name -> (match %, source path)."""
    rep = json.load(open(path))
    fuzzy, units = {}, {}
    for u in rep["units"]:
        name = os.path.splitext(u["name"].split("/", 1)[1])[0]
        md = u.get("metadata", {})
        src = md.get("source_path")
        has_src = bool(src) and os.path.exists(os.path.join(ROOT, src))
        meas = u.get("measures", {})
        units[name] = (float(meas.get("matched_code_percent", 0.0)), has_src)
        for f in u.get("functions", []):
            va = int(f.get("metadata", {}).get("virtual_address", 0))
            if va:
                fuzzy[va] = float(f.get("fuzzy_match_percent", 0.0)) if has_src else 0.0
    return fuzzy, units


def unit_of(units):
    ranges = sorted((s, e, u) for u, rs in units.items() for s, e in rs)
    starts = [r[0] for r in ranges]

    def find(addr):
        i = bisect_right(starts, addr) - 1
        if i >= 0 and ranges[i][0] <= addr < ranges[i][1]:
            return ranges[i][2]
        return None
    return find


def analyse(cov, funcs, find_unit, fuzzy):
    """unit -> dict(funcs, bytes, unmatched)"""
    stats, sinit = {}, {}
    covered_in_funcs = 0
    for addr, size, name in funcs:
        lo = (addr - MEM1) >> 2
        n = sum(1 for b in cov[lo:lo + (size >> 2)] if b)
        if not n:
            continue
        covered_in_funcs += n
        unit = find_unit(addr) or "?"
        if name.startswith("__sinit_"):
            # static constructors run for every unit at boot; tracked apart
            sinit[unit] = sinit.get(unit, 0) + 4 * n
            continue
        s = stats.setdefault(unit, {"funcs": 0, "bytes": 0, "unmatched": 0})
        s["funcs"] += 1
        s["bytes"] += 4 * n
        if fuzzy.get(addr, 0.0) < 100.0:
            s["unmatched"] += 4 * n
    total = 4 * sum(1 for b in cov if b)
    return stats, total, 4 * covered_in_funcs, sinit


def is_game(unit):
    return unit.startswith("game/")


def lib_group(unit):
    parts = unit.split("/")
    if parts[0] == "JSystem" and len(parts) > 1:
        return "JSystem/" + parts[1]
    return parts[0]


def meta(units_meta, unit):
    return units_meta.get(os.path.splitext(unit)[0], (0.0, False))


def unit_row(unit, s, units_meta, extra=""):
    pct, has_src = meta(units_meta, unit)
    name = f"`{unit}`" + ("" if has_src else " **(no source)**")
    return f"| {name} | {s['funcs']} | {s['bytes']:,} | {s['unmatched']:,} | {pct:.1f} %{extra} |"


HEADER = "| unit | functions executed | executed bytes | executed, not matched | unit match % |"
SEP = "|---|---:|---:|---:|---:|"


def lib_summary(stats, units_meta):
    groups = {}
    for u, s in stats.items():
        if is_game(u) or u == "?":
            continue
        g = groups.setdefault(lib_group(u), {"units": 0, "funcs": 0, "bytes": 0, "unmatched": 0})
        g["units"] += 1
        for k in ("funcs", "bytes", "unmatched"):
            g[k] += s[k]
    out = ["| library | units | functions | executed bytes | executed, not matched |", "|---|---:|---:|---:|---:|"]
    for g, v in sorted(groups.items(), key=lambda kv: (-kv[1]["unmatched"], -kv[1]["bytes"])):
        out.append(f"| {g} | {v['units']} | {v['funcs']} | {v['bytes']:,} | {v['unmatched']:,} |")
    return out


METHOD = """\
# Boot path: units executed from boot to the start of a Shadow Battle

Generated by `tools/boot_path.py` from per-instruction execution coverage recorded in
Dolphin. Rerun it after matching progresses (`ninja build/G4SE01/report_honest.json`
first); the coverage dumps themselves do not change.

## Method

- **Coverage.** A patched Dolphin (Jit64) emits, in front of every guest instruction it
  compiles, a store `g_fsa_cov[(pc & 0x01FFFFFF) >> 2] = 1` (one byte per MEM1
  instruction, 6 MiB). The flag is set only when the instruction is actually reached,
  so code behind a branch that is never taken stays unmarked (the second instruction
  of a fused compare+branch pair is the only thing it misses). The ctl fifo of the
  scripted-run patch gained `covdump PATH` and `covreset`.
- **Run.** OGL/EGL headless, GBA BIOS in `User/GBA/gba_bios.bin`, existing memory card
  with Quest Log 1. Dumps live in `../bootpath-data/cov/` (not in git):
  1. `p1_boot_title.bin`: power-on until the title screen ("PRESS START") is idle.
  2. `p2_title_menu.bin`: `covreset`, Start, until the main menu (Hyrulean Adventure /
     Shadow Battle / Options) is idle.
  3. main menu -> battle, union of two dumps:
     `p3a_menu_gbaconnect.bin` (`covreset` at the main menu; Shadow Battle -> Quest Log 1 ->
     START -> Two-Player Game -> GBA connection screen, savestate there) and
     `p3b_connect_battle.bin` (Dolphin restarted from that savestate with SI ports 1-2 set
     to *GBA (Integrated)* (mGBA, real BIOS), `covreset` after the state load, wait for the
     GBA JOY-boot, A on GBA 1 -> Select Area -> A (The Field) -> countdown -> both Links
     controllable on the field; dump ~18 s after the area pick).
- **Mapping.** `config/G4SE01/symbols.txt` functions are attributed to units by
  `config/G4SE01/splits.txt`. *Executed bytes* = 4 x executed instructions of a unit;
  *executed, not matched* counts those in functions below 100 % fuzzy in the honest report
  (a unit without source counts all of them). *Unit match %* = honest `matched_code_percent`.
  **(no source)** marks units whose source file does not exist yet.
- Coverage of a phase is everything executed during it, so per-phase tables overlap
  (the per-frame main loop, audio, rendering appear in every phase). The work order
  counts each unit once, in the wave of the first phase that executes it.
- Static initializers (`__sinit_*`) run for every unit before `main`, so they are kept
  out of the unit tables (a unit whose only executed code is its `__sinit_` is not "on
  the path"); the totals show them separately.

## Dolphin patch (not committed; dolphin checkout `../dolphin`, uncommitted working-tree change)

- `Source/Core/Core/PowerPC/Jit64/Jit.cpp`: global `u8 g_fsa_cov[0x01800000 / 4]`; in
  `Jit64::DoJit`'s per-instruction loop, right after `HandleFunctionHooking`, for MEM1
  addresses: `MOV(64, R(RSCRATCH), ImmPtr(&g_fsa_cov[(op.address & 0x01FFFFFF) >> 2]));
  MOV(8, MatR(RSCRATCH), Imm8(1));` (RAX is never register-allocated and MOV leaves the
  host flags alone, so a carry kept in CF survives).
- `Source/Core/DolphinNoGUI/MainNoGUI.cpp` (the `DOLPHIN_CTL` fifo of the scripted-run
  patch): `covdump PATH` writes the 6 MiB array, `covreset` zeroes it.
- Requires `CPUCore = 1` (Jit64). Rebuild: `ninja -C ../dolphin/build dolphin-emu-nogui`.

## Reproducing the Shadow Battle run

Shadow Battle is GBA-only ("P2 - P4, GBA only"): every player is a Game Boy Advance on
SI ports 1-N. Dolphin's integrated mGBA works for this with the **real GBA BIOS**
(SHA-1 `300c20df6731a33952ded8c436f7f186d25d3492`) in `User/GBA/gba_bios.bin`; a
replacement BIOS (SHA-1 `598c0b2c...`) shows the GBAs as connected but the JOY-boot never
completes. GBA input comes from `Config/GBA.ini` (`[GBA1]`, `Device = Pipe/0/gba0`, buttons
`Buttons/A`, `Buttons/START`, `D-Pad/Up`, ...). Plugging the GBAs in by config while a
savestate of the connection screen loads works (SI `DoState` restores the saved GC pad,
then `UpdateDevices` swaps in the configured GBA). After ~20 s both GBAs are booted, A on
GBA 1 opens Select Area, A picks The Field, and the battle starts after a 10 s countdown.
Helper scripts (run.sh, bridge.sh, press.sh, shot.sh) and the savestates are in
`../bootpath-data/` next to the dumps.
"""


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--phase", action="append", help="NAME=dump[+dump...] (repeatable, in order)")
    ap.add_argument("--covdir", default=DEFAULT_COV)
    ap.add_argument("--report", default=os.path.join(ROOT, "build/G4SE01/report_honest.json"))
    ap.add_argument("--out", default=os.path.join(ROOT, "docs/BOOT_PATH.md"))
    ap.add_argument("--extra", default=os.path.join(ROOT, "docs/BOOT_PATH.notes.md"),
                    help="hand-written notes appended verbatim if the file exists")
    args = ap.parse_args()

    phases = [tuple(p.split("=", 1)) for p in args.phase] if args.phase else DEFAULT_PHASES
    units = parse_splits(os.path.join(ROOT, "config/G4SE01/splits.txt"))
    funcs = parse_symbols(os.path.join(ROOT, "config/G4SE01/symbols.txt"))
    fuzzy, units_meta = load_report(args.report)
    find_unit = unit_of(units)

    out = [METHOD]
    first_seen = {}
    union = {}
    summary = ["## Totals", "", "| phase | executed bytes | in game units | game: not matched | in libraries | libraries: not matched | game units | game `__sinit_` bytes (units) | outside known functions |",
               "|---|---:|---:|---:|---:|---:|---:|---:|---:|"]
    sections = []
    union_cov = None
    for wave, (pname, spec) in enumerate(phases, 1):
        cov = load_cov(spec, args.covdir)
        union_cov = bytearray(cov) if union_cov is None else bytearray(a | b for a, b in zip(union_cov, cov))
        stats, total, in_funcs, sinit = analyse(cov, funcs, find_unit, fuzzy)
        game = {u: s for u, s in stats.items() if is_game(u)}
        libs = {u: s for u, s in stats.items() if not is_game(u) and u != "?"}
        for u in game:
            first_seen.setdefault(u, wave)
        summary.append(
            f"| {wave}. {pname} | {total:,} | {sum(s['bytes'] for s in game.values()):,} | "
            f"{sum(s['unmatched'] for s in game.values()):,} | {sum(s['bytes'] for s in libs.values()):,} | "
            f"{sum(s['unmatched'] for s in libs.values()):,} | {len(game)} | "
            f"{sum(v for u, v in sinit.items() if is_game(u)):,} ({sum(1 for u in sinit if is_game(u))}) | {total - in_funcs + stats.get('?', {}).get('bytes', 0):,} |")

        sec = [f"## Phase {wave}: {pname}", "",
               f"Dump(s): `{spec}`. {len(game)} game units executed, "
               f"{sum(1 for u in game if not meta(units_meta, u)[1])} of them without source.", "",
               "### Game units (ranked by executed, not matched bytes)", "", HEADER, SEP]
        for u, s in sorted(game.items(), key=lambda kv: (-kv[1]["unmatched"], -kv[1]["bytes"])):
            sec.append(unit_row(u, s, units_meta))
        sec += ["", "### Libraries (summary)", ""] + lib_summary(libs, units_meta) + [""]
        sections += sec
    stats, _, _, _ = analyse(union_cov, funcs, find_unit, fuzzy)
    for u, s in stats.items():
        if is_game(u):
            union[u] = s

    order = sorted((u for u in union if union[u]["unmatched"] > 0),
                   key=lambda u: (first_seen.get(u, 99), -union[u]["unmatched"]))
    wo = ["## Work order (game units)", "",
          "Each game unit once, in the wave of the first phase that executes it; inside a wave by "
          "executed-but-unmatched bytes over the whole path. Units whose executed code is already "
          f"fully matched are left out ({sum(1 for u in union if union[u]['unmatched'] == 0)} units).", "",
          "| # | wave | unit | functions executed | executed bytes | executed, not matched | unit match % |",
          "|---:|---:|---|---:|---:|---:|---:|"]
    for i, u in enumerate(order, 1):
        s = union[u]
        pct, has_src = meta(units_meta, u)
        name = f"`{u}`" + ("" if has_src else " **(no source)**")
        wo.append(f"| {i} | {first_seen.get(u)} | {name} | {s['funcs']} | {s['bytes']:,} | {s['unmatched']:,} | {pct:.1f} % |")
    wo.append("")
    wo += ["### By executed-but-unmatched bytes alone (top 40, whole path)", "",
           "| # | unit | executed, not matched | wave |", "|---:|---|---:|---:|"]
    for i, u in enumerate(sorted(order, key=lambda u: -union[u]["unmatched"])[:40], 1):
        wo.append(f"| {i} | `{u}`{'' if meta(units_meta, u)[1] else ' **(no source)**'} | {union[u]['unmatched']:,} | {first_seen.get(u)} |")
    wo.append("")

    text = "\n".join([out[0]] + summary + [""] + wo + sections)
    if os.path.exists(args.extra):
        text += "\n" + open(args.extra).read()
    open(args.out, "w").write(text.rstrip() + "\n")
    print(f"wrote {args.out}: {len(union)} game units on the path, {len(order)} with unmatched executed code")


if __name__ == "__main__":
    sys.exit(main())
