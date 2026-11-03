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

Usage (ninja runs it): tools/honest_report.py <objdiff-cli> <output> [report args...]
"""

import json
import subprocess
import sys
from pathlib import Path


def main():
    if len(sys.argv) < 3:
        sys.exit(__doc__)
    objdiff, out, extra = sys.argv[1], Path(sys.argv[2]), sys.argv[3:]
    root = Path.cwd()
    cfg = json.loads((root / "objdiff.json").read_text())
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
