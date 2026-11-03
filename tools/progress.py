#!/usr/bin/env python3
"""Print match progress from the honest objdiff report (every unit diffed; see tools/honest_report.py).

The per-region and per-library figures come from tools/site_data.py's own
classification, so they are the progress website's numbers: functions in
dtk's unsplit chunks count toward the library their address belongs to
(lib_map.tsv), not only functions inside split units.

Usage: python3 tools/progress.py [build/G4SE01/report_honest.json]
"""

import json
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import site_data  # noqa: E402


def main() -> None:
    if len(sys.argv) > 1:
        site_data.REPORT = Path(sys.argv[1]).resolve()
    report, files, _ = site_data.build()

    m = report["measures"]
    print(
        f"total: {m['matched_functions']} / {m['total_functions']} functions, "
        f"{int(m.get('matched_code', 0))} / {int(m['total_code'])} bytes of code "
        f"({m['matched_code_percent']:.2f} %)"
    )

    regions = defaultdict(lambda: defaultdict(lambda: [0, 0, 0, 0, 0]))
    for (region, lib, _), rec in files.items():
        row = regions[region][lib or "-"]
        row[0] += rec["cm"]
        row[1] += rec["code"]
        row[2] += rec["fm"]
        row[3] += rec["fn"]
        row[4] += sum(1 for fn in rec["fns"] if fn[3] == -1)  # functions in no split unit

    for rid, rname, _ in site_data.REGIONS:
        libs = regions.get(rid)
        if not libs:
            continue
        cm, c, fm, f, unsplit = (sum(r[i] for r in libs.values()) for i in range(5))
        pct = 100 * cm / c if c else 0
        note = f"  ({unsplit} not in any split unit)" if unsplit else ""
        print(f"  {rname:30} {fm:5} / {f:<5} functions  {pct:6.2f} % of {c} bytes{note}")
        if rid == "game":
            continue  # game libraries are address bands and resource-name groups
        for lib, (lcm, lc, lfm, lf, lun) in sorted(libs.items(), key=lambda kv: -kv[1][1]):
            if lfm < lf or lcm < lc:
                lpct = 100 * lcm / lc if lc else 0
                lnote = f"  ({lun} unsplit)" if lun else ""
                print(f"      {lib:26} {lfm:5} / {lf:<5} functions  {lpct:6.2f} %{lnote}")


if __name__ == "__main__":
    main()
