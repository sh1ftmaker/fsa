#!/usr/bin/env python3
"""Write the objdiff report with every unit actually diffed.

objdiff's `report generate` counts a unit whose objdiff.json metadata says
`complete: true` (our `Matching` flag) as 100 % matched without diffing it.
So build/G4SE01/report.json can't disprove a wrong Matching flag. This
writes build/G4SE01/report_honest.json from a copy of objdiff.json where no
unit is complete, with the same report arguments. tools/progress.py,
tools/gate.py and tools/coord.py read this one; report.json stays for
decomp.dev-style tooling.

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
    project = out.parent / "honest"
    project.mkdir(parents=True, exist_ok=True)
    (project / "objdiff.json").write_text(json.dumps(cfg, indent=1))
    cmd = [objdiff, "report", "generate", "-p", str(project), "-o", str(out), *extra]
    sys.exit(subprocess.run(cmd).returncode)


if __name__ == "__main__":
    main()
