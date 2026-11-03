#!/usr/bin/env python3
"""Write the progress website's data (site/data/) from the honest report.

Inputs (all local; CI can't build the game, so this runs before each commit):
  build/G4SE01/report_honest.json  honest per-unit and per-function measures
  objdiff.json                     unit -> source path, Matching flag
  config/G4SE01/splits.txt         unit -> address ranges
  config/G4SE01/lib_map.tsv        library ranges not split yet (docs/LIBMAP.md)
  config/G4SE01/tu_map.tsv         the game-code file map (docs/PLAN.md)
  build/G4SE01/fsa_game_code.dol   hashed against config/G4SE01/build.sha1
  docs/roadmap.yml                 phases and milestones (hand-maintained statuses)

Every function in the DOL is placed in region -> library/system -> file:
split units by their path, dtk's auto splits (`auto_*`, not started yet) by
address through lib_map.tsv and tu_map.tsv. So the totals are the whole DOL.

Outputs:
  site/data/summary.json       totals, regions, libraries, files
  site/data/lib/<id>.json      functions per file of one library (loaded on demand)
  site/data/history.json       one entry per commit (appended or replaced)
  site/data/roadmap.json       docs/roadmap.yml with each milestone's live progress
  site/data/badge-*.json       shields.io endpoint badges for the README
  site/badges/*.svg            README visuals (tools/readme_assets.py)
  README.md                    the text between its <!-- progress|regions|now:start/end --> markers

Only names, addresses, sizes and match flags are written: never bytes,
asm or data contents of main.dol.

Usage: python3 tools/site_data.py [--subject "<commit subject>"] [--stage]

--stage runs `git add` on everything above, so a merge commit carries the
site data, the README visuals and the README together.
"""

import argparse
import bisect
import csv
import datetime
import hashlib
import json
import re
import subprocess
from collections import defaultdict
from pathlib import Path

import readme_assets  # tools/ is on sys.path when this file runs as a script

ROOT = Path(__file__).resolve().parent.parent
REPORT = ROOT / "build/G4SE01/report_honest.json"
DOL = ROOT / "build/G4SE01/fsa_game_code.dol"
SHA1 = ROOT / "config/G4SE01/build.sha1"
OUT = ROOT / "site/data"
ROADMAP = ROOT / "docs/roadmap.yml"
PREFIX = "fsa_game_code/"

REGIONS = [  # id, name, short description
    ("game", "Game code", "FSA's own code: 0x800F39EC-0x8045BD00 plus one file before JSystem"),
    ("jsystem", "JSystem", "Nintendo EAD's engine libraries (JAudio, J2D, J3D, JKernel, ...)"),
    ("sdk", "Dolphin SDK", "The GameCube SDK: OS, DVD, CARD, PAD, AI, DSP, ..."),
    ("gx", "GX", "The GameCube graphics library"),
    ("runtime", "MSL, Runtime, TRK", "Metrowerks C library, C++ runtime and the MetroTRK debugger stub"),
]


def git(*args):
    r = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True)
    return r.stdout.strip() if r.returncode == 0 else ""


def classify(path, libfield=""):
    """(region, library) for a split path such as dolphin/os/OS.c."""
    p = path
    if p.startswith("dolphin/gx/") or libfield == "SDK:gx":
        return "gx", "gx"
    if p.startswith("dolphin/"):
        return "sdk", p.split("/")[1]
    if libfield.startswith("SDK:"):
        return "sdk", libfield[4:]
    if p.startswith("JSystem/") or libfield.startswith("JSystem:") or libfield == "J3D":
        lib = p.split("/")[1] if p.startswith("JSystem/") else libfield.split(":")[-1]
        return "jsystem", "J3D" if lib.startswith("J3D") else lib
    if "/MSL/" in p or libfield == "MSL":
        return "runtime", "MSL_C"
    if "/Runtime/" in p or libfield == "Runtime":
        return "runtime", "Runtime"
    if p.startswith("TRK_MINNOW_DOLPHIN") or libfield == "TRK":
        return "runtime", "MetroTRK"
    return "game", None


def game_group(name, start):
    """Game files have no real names yet: group them by the resource names
    they reference (tu_map.tsv's hints), or by address when there is none."""
    hint = re.sub(r"^tu_[0-9A-Fa-f]+_?", "", name or "")
    for pre, group in (("enemy_", "enemy_*_sob refs"), ("mg_", "mg_* minigames"), ("kk_", "kk_* HUD refs"),
                       ("player", "player_sob refs"), ("THP", "THP")):
        if hint.startswith(pre):
            return group
    if hint:
        return "other hints"
    if start < 0x800F39EC:
        return "linked before JSystem"
    band = max(start & ~0x7FFFF, 0x80100000)
    return f"unnamed {band:08X}+"


def read_splits():
    """split path -> list of (section, start, end)."""
    units, cur = {}, None
    for line in (ROOT / "config/G4SE01/splits.txt").read_text().splitlines():
        if line and not line[0].isspace() and line.endswith(":"):
            cur = line[:-1]
            units[cur] = []
        elif cur and "start:" in line:
            sec = line.split()[0]
            m = re.search(r"start:(0x[0-9A-Fa-f]+) end:(0x[0-9A-Fa-f]+)", line)
            units[cur].append((sec, int(m.group(1), 16), int(m.group(2), 16)))
    return units


def read_ranges(name, start_col, end_col):
    rows = []
    with open(ROOT / "config/G4SE01" / name) as f:
        for row in csv.DictReader(f, delimiter="\t"):
            if not row[start_col].startswith("0x"):
                continue
            rows.append((int(row[start_col], 16), int(row[end_col], 16), row))
    rows.sort(key=lambda r: r[0])
    return rows


class RangeMap:
    def __init__(self, rows):
        self.rows = rows
        self.starts = [r[0] for r in rows]

    def find(self, addr):
        i = bisect.bisect_right(self.starts, addr) - 1
        if i >= 0 and self.rows[i][0] <= addr < self.rows[i][1]:
            return self.rows[i][2]
        return None


def sha1_ok():
    if not DOL.exists():
        return None
    want = SHA1.read_text().split()[0]
    return hashlib.sha1(DOL.read_bytes()).hexdigest() == want


def slug(text):
    return re.sub(r"[^A-Za-z0-9]+", "-", text).strip("-").lower()


def build():
    report = json.loads(REPORT.read_text())
    cfg = {u["name"]: u for u in json.loads((ROOT / "objdiff.json").read_text())["units"]}
    splits = read_splits()
    split_by_unit = {re.sub(r"\.[^./]*$", "", p): p for p in splits}
    libmap = RangeMap(read_ranges("lib_map.tsv", "text_start", "text_end"))
    tumap = RangeMap(read_ranges("tu_map.tsv", "start", "end"))

    # file key -> record; functions collected per file
    files = {}

    def file_rec(region, lib, path, display):
        key = (region, lib, path)
        if key not in files:
            files[key] = {"path": path, "name": display, "region": region, "lib": lib, "status": "U",
                          "code": 0, "cm": 0, "fn": 0, "fm": 0, "data": 0, "dm": 0, "fns": [], "addr": None}
        return files[key]

    def place_auto(addr):
        """region, lib, path, display name for an unsplit function."""
        row = libmap.find(addr)
        if row and row["library"] != "game":
            action = row["action"]
            path = action[len("extend:"):] if action.startswith("extend:") else row["proposed_source"]
            region, lib = classify(path, row["library"])
            return region, lib, path, path.rsplit("/", 1)[-1]
        row = tumap.find(addr)
        if row:
            name = row["suggested_name"]
            return "game", game_group(name, int(row["start"], 16)), f"game/{name}", name
        row = libmap.find(addr)
        if row:  # a game file linked before JSystem (lib_map.tsv's "game" row)
            name = "tu_" + row["text_start"][2:]
            return "game", game_group(name, int(row["text_start"], 16)), f"game/{name}", name
        return None

    data_unsplit = [0, 0]
    loose = []  # dtk's pad_ pieces between splits: they join the file before them
    for u in report["units"]:
        name = u["name"]
        m = u.get("measures", {})
        meta = (cfg.get(name) or {}).get("metadata") or {}
        auto = meta.get("auto_generated") or u.get("metadata", {}).get("auto_generated")
        if auto:
            data_unsplit[0] += int(m.get("total_data", 0))
            data_unsplit[1] += int(m.get("matched_data", 0))
            for f in u.get("functions", []):
                addr = int(f["metadata"]["virtual_address"])
                where = place_auto(addr)
                if where is None:
                    loose.append((addr, f))
                    continue
                region, lib, path, display = where
                rec = file_rec(region, lib, path, display)
                add_fn(rec, f, addr, tail=True)
            continue
        short = name[len(PREFIX):] if name.startswith(PREFIX) else name
        path = split_by_unit.get(short, short)
        region, lib = classify(path)
        text = [s for s in splits.get(path, []) if s[0] in (".text", ".init")]
        start = min((s[1] for s in text), default=0)
        if region == "game":
            # a split game unit can span several tu_map.tsv rows (merged by
            # tools/tu_map/to_splits.py): the first row with a hint names the group
            end = max((s[2] for s in text), default=start)
            rows = [r[2] for r in tumap.rows if start <= r[0] < end] if start else []
            groups = [game_group(r["suggested_name"], start) for r in rows]
            named = [g for g in groups if not g.startswith(("unnamed", "linked before"))]
            stem = re.sub(r"\.[^.]*$", "", path.rsplit("/", 1)[-1])
            lib = named[0] if named else (groups[0] if groups else game_group(stem, start))
        rec = file_rec(region, lib, path, path.rsplit("/", 1)[-1])
        rec["status"] = "M" if meta.get("complete") else "N"
        rec["src"] = meta.get("source_path", "")
        rec["data"] += int(m.get("total_data", 0))
        rec["dm"] += int(m.get("matched_data", 0))
        for f in u.get("functions", []):
            add_fn(rec, f, int(f["metadata"]["virtual_address"]), tail=False)
    if loose:
        owners = sorted((fn[1], rec) for rec in files.values() for fn in rec["fns"])
        starts = [o[0] for o in owners]
        for addr, f in loose:
            rec = owners[max(bisect.bisect_left(starts, addr) - 1, 0)][1]
            add_fn(rec, f, addr, tail=True)
    return report, files, data_unsplit


def add_fn(rec, f, addr, tail):
    size = int(f["size"])
    fuzzy = f.get("fuzzy_match_percent", 0.0)
    matched = fuzzy == 100.0
    name = f.get("metadata", {}).get("demangled_name") or f["name"]
    rec["code"] += size
    rec["fn"] += 1
    if matched:
        rec["cm"] += size
        rec["fm"] += 1
    rec["addr"] = addr if rec["addr"] is None else min(rec["addr"], addr)
    # [name, address, size, matched-ness: 100 = matched, else floor of fuzzy %, -1 = not split yet]
    rec["fns"].append([name, addr, size, -1 if tail else (100 if matched else int(fuzzy))])


def write(report, files, data_unsplit, subject):
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / "lib").mkdir(exist_ok=True)
    for old in (OUT / "lib").glob("*.json"):
        old.unlink()
    m = report["measures"]
    total = int(m["total_code"])
    ok = sha1_ok()
    cfg_units = json.loads((ROOT / "objdiff.json").read_text())["units"]
    split_units = [u for u in cfg_units if not (u.get("metadata") or {}).get("auto_generated")]
    matching = sum(1 for u in split_units if (u.get("metadata") or {}).get("complete"))

    regions = []
    reg_tot = {}
    for rid, rname, rdesc in REGIONS:
        libs = defaultdict(list)
        for (region, lib, _), rec in files.items():
            if region == rid:
                libs[lib].append(rec)
        out_libs = []
        for lib, recs in libs.items():
            lid = f"{rid}-{slug(lib)}"
            recs.sort(key=lambda r: r["addr"] or 0)
            detail = {}
            out_files = []
            for i, r in enumerate(recs):
                r["fns"].sort(key=lambda x: x[1])
                detail[str(i)] = r["fns"]
                row = {"n": r["name"], "p": r["path"], "a": r["addr"], "s": r["status"],
                       "c": r["code"], "cm": r["cm"], "f": r["fn"], "fm": r["fm"]}
                if r["data"]:
                    row["d"], row["dm"] = r["data"], r["dm"]
                if any(fn[3] == -1 for fn in r["fns"]) and r["status"] != "U":
                    row["t"] = sum(fn[2] for fn in r["fns"] if fn[3] == -1)  # unsplit tail bytes
                out_files.append(row)
            (OUT / "lib" / f"{lid}.json").write_text(json.dumps({"id": lid, "files": detail}, separators=(",", ":")))
            out_libs.append({"id": lid, "n": lib, "c": sum(r["code"] for r in recs), "cm": sum(r["cm"] for r in recs),
                             "f": sum(r["fn"] for r in recs), "fm": sum(r["fm"] for r in recs), "files": out_files})
        out_libs.sort(key=lambda l: -l["c"])
        rt = {k: sum(l[k] for l in out_libs) for k in ("c", "cm", "f", "fm")}
        reg_tot[rid] = rt
        regions.append({"id": rid, "n": rname, "desc": rdesc, **rt, "libs": out_libs})

    placed = sum(r["c"] for r in reg_tot.values())
    if placed != total:
        raise SystemExit(f"placed {placed} bytes of code, report says {total}")

    head = git("rev-parse", "HEAD")
    now = datetime.datetime.now(datetime.timezone.utc).replace(microsecond=0).isoformat()
    totals = {
        "code": total, "code_matched": int(m.get("matched_code", 0)),
        "data": int(m["total_data"]), "data_matched": int(m.get("matched_data", 0)),
        "fns": m["total_functions"], "fns_matched": m.get("matched_functions", 0),
        "units": len(split_units), "matching": matching,
        "linked": matching if ok else None, "sha1_ok": ok,
        "files": len(files), "files_unsplit": sum(1 for r in files.values() if r["status"] == "U"),
    }
    summary = {"generated": now, "base": head, "sha1": SHA1.read_text().split()[0],
               "totals": totals, "regions": regions}
    (OUT / "summary.json").write_text(json.dumps(summary, separators=(",", ":")))
    rm = roadmap(regions, totals, now)
    (OUT / "roadmap.json").write_text(json.dumps(rm, separators=(",", ":")))
    readme_assets.write_all(summary, rm)

    # history: one entry per commit. The commit being made has no hash yet, so
    # each entry records its parent ("base"); later runs fill in "commit".
    hist_path = OUT / "history.json"
    hist = json.loads(hist_path.read_text()) if hist_path.exists() else []
    if hist and hist[-1].get("base") == head and not hist[-1].get("commit"):
        hist.pop()  # rerun before the same commit: replace
    children = {}
    for line in git("rev-list", "--parents", "-n", "500", "HEAD").splitlines():
        parts = line.split()
        if len(parts) >= 2:
            children.setdefault(parts[1], parts[0])
    for e in hist:
        if not e.get("commit") and e.get("base") in children:
            e["commit"] = children[e["base"]]
    hist.append({
        "base": head, "date": now, "subject": subject or "",
        "code": totals["code_matched"], "code_total": total,
        "fns": totals["fns_matched"], "fns_total": totals["fns"],
        "data": totals["data_matched"], "data_total": totals["data"],
        "matching": matching,
        "regions": {rid: [t["cm"], t["c"], t["fm"], t["f"]] for rid, t in reg_tot.items()},
    })
    hist_path.write_text(json.dumps(hist, separators=(",", ":"), indent=None).replace("},{", "},\n{"))
    return summary


STATUSES = ("done", "active", "next", "later")
MEASURES = {
    "code": "of the code matched",
    "fns": "of the functions matched",
    "units": "of the split files Matching",
    "split": "of the code split into units",
    "linked": "of all units Matching and linked byte-identically",
}


def roadmap(regions, totals, now):
    """docs/roadmap.yml plus live progress. Selectors (`covers`, `measure.of`)
    are `<region>` or `<region>:<library name>` and resolve to treemap ids:
    `<region>` and `<region>/<library id>`. Unknown selectors and statuses
    stop the run, so a typo can't silently show 0 %."""
    import yaml  # PyYAML; only needed here, where the site data is made
    rm = yaml.safe_load(ROADMAP.read_text())
    libs = {}
    for r in regions:
        for l in r["libs"]:
            libs[f"{r['id']}:{l['n']}"] = (r["id"], l)
    reg = {r["id"]: r for r in regions}

    def resolve(sel):
        """selector -> (treemap id, [library records])"""
        if sel in reg:
            return sel, [(sel, l) for l in reg[sel]["libs"]]
        if sel in libs:
            rid, l = libs[sel]
            return f"{rid}/{l['id']}", [(rid, l)]
        raise SystemExit(f"docs/roadmap.yml: unknown selector {sel!r} "
                         f"(regions: {', '.join(reg)}; libraries as region:name, e.g. jsystem:J3D)")

    def measure(kind, sels):
        if kind == "linked":
            return {"done": totals["linked"] or 0, "total": totals["units"]}
        recs, seen = [], set()
        for sel in sels:
            for rid, l in resolve(sel)[1]:
                if l["id"] not in seen:
                    seen.add(l["id"])
                    recs.append(l)
        files = [f for l in recs for f in l["files"]]
        if kind == "code":
            return {"done": sum(l["cm"] for l in recs), "total": sum(l["c"] for l in recs)}
        if kind == "fns":
            return {"done": sum(l["fm"] for l in recs), "total": sum(l["f"] for l in recs)}
        if kind == "units":
            split = [f for f in files if f["s"] != "U"]
            return {"done": sum(1 for f in split if f["s"] == "M"), "total": len(split)}
        if kind == "split":
            return {"done": sum(f["c"] - f.get("t", 0) for f in files if f["s"] != "U"),
                    "total": sum(f["c"] for f in files)}
        raise SystemExit(f"docs/roadmap.yml: unknown measure kind {kind!r} ({', '.join(MEASURES)})")

    owners = defaultdict(list)  # treemap id -> milestone ids covering it
    phases = []
    for ph in rm["phases"]:
        if ph["status"] not in STATUSES:
            raise SystemExit(f"docs/roadmap.yml: phase {ph['id']}: status {ph['status']!r}")
        ms_out = []
        for ms in ph["milestones"]:
            if ms["status"] not in STATUSES:
                raise SystemExit(f"docs/roadmap.yml: {ms['id']}: status {ms['status']!r}")
            covers = [resolve(c)[0] for c in ms.get("covers") or []]
            for c in covers:
                owners[c].append(ms["id"])
            out = {"id": ms["id"], "title": ms["title"], "desc": ms["desc"], "status": ms["status"],
                   "covers": covers}
            if ms.get("measure"):
                kind = ms["measure"]["kind"]
                out["measure"] = {"kind": kind, "label": MEASURES.get(kind, kind),
                                  **measure(kind, ms["measure"].get("of") or ms.get("covers") or [])}
            if ms.get("landed"):
                out["landed"] = ms["landed"]
            ms_out.append(out)
        phases.append({"id": ph["id"], "title": ph["title"], "summary": ph.get("summary", ""), "hue": ph.get("hue", "gold"),
                       "status": ph["status"], "milestones": ms_out})
    return {"generated": now, "policies": rm.get("policies", []), "phases": phases, "owners": owners}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--subject", default="", help="subject line of the commit these files go into")
    ap.add_argument("--stage", action="store_true", help="git add every file this writes")
    args = ap.parse_args()
    if not REPORT.exists():
        raise SystemExit(f"{REPORT} missing: run ninja first")
    report, files, data_unsplit = build()
    s = write(report, files, data_unsplit, args.subject)
    t = s["totals"]
    sizes = sum(p.stat().st_size for p in OUT.rglob("*.json"))
    print(f"site/data: {t['fns_matched']}/{t['fns']} functions, {t['code_matched']}/{t['code']} bytes, "
          f"{t['files']} files ({t['files_unsplit']} not split), SHA-1 {'OK' if t['sha1_ok'] else 'NOT OK'}, "
          f"{sizes // 1024} KiB")
    if args.stage:
        r = subprocess.run(["git", "add", "--", *readme_assets.OUTPUTS], cwd=ROOT, capture_output=True, text=True)
        if r.returncode != 0:
            raise SystemExit(f"git add failed:\n{r.stderr}")


if __name__ == "__main__":
    main()
