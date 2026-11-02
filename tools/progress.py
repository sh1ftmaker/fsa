#!/usr/bin/env python3
"""Print match progress from the honest objdiff report (every unit diffed; see tools/honest_report.py).

Usage: python3 tools/progress.py [build/G4SE01/report_honest.json]
"""

import json
import sys
from collections import defaultdict


def main() -> None:
    path = sys.argv[1] if len(sys.argv) > 1 else "build/G4SE01/report_honest.json"
    with open(path) as f:
        report = json.load(f)
    with open("objdiff.json") as f:
        sources = {
            u["name"]: (u.get("metadata") or {}).get("source_path") or ""
            for u in json.load(f)["units"]
        }

    m = report["measures"]
    print(
        f"total: {m['matched_functions']} / {m['total_functions']} functions, "
        f"{int(m.get('matched_code', 0))} / {int(m['total_code'])} bytes of code "
        f"({m['matched_code_percent']:.2f} %)"
    )

    libs = defaultdict(lambda: [0, 0, 0, 0])
    for unit in report["units"]:
        source = sources.get(unit["name"], "")
        if not source:
            continue
        lib = "/".join(source.split("/")[:3])
        um = unit["measures"]
        row = libs[lib]
        row[0] += int(um.get("matched_code", 0))
        row[1] += int(um.get("total_code", 0))
        row[2] += um.get("matched_functions", 0)
        row[3] += um.get("total_functions", 0)

    for lib, (code, total_code, fns, total_fns) in sorted(libs.items()):
        pct = 100 * code / total_code if total_code else 0
        print(f"  {lib:36} {fns:5} / {total_fns:<5} functions  {pct:6.2f} % of {total_code} bytes")


if __name__ == "__main__":
    main()
