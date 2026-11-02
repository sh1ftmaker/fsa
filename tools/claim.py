#!/usr/bin/env python3
"""Claim or release a unit (progress/claims.json, under a lock).

Usage:
  python3 tools/claim.py list
  python3 tools/claim.py take <unit> <owner> [branch] [worktree]
  python3 tools/claim.py release <unit>
"""

import datetime
import fcntl
import json
import sys
from pathlib import Path

PATH = Path(__file__).resolve().parent.parent / "progress" / "claims.json"


def main():
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    with open(PATH, "r+") as f:
        fcntl.flock(f, fcntl.LOCK_EX)
        claims = json.load(f)
        cmd = sys.argv[1]
        if cmd == "list":
            for unit, c in sorted(claims.items()):
                print(f"{unit:48} {c['owner']:24} {c.get('branch', '')} {c['started']}")
            return
        unit = sys.argv[2]
        if cmd == "take":
            if unit in claims:
                sys.exit(f"{unit} is already claimed by {claims[unit]['owner']}")
            claims[unit] = {
                "owner": sys.argv[3],
                "branch": sys.argv[4] if len(sys.argv) > 4 else "",
                "worktree": sys.argv[5] if len(sys.argv) > 5 else "",
                "started": datetime.datetime.now().isoformat(timespec="minutes"),
            }
        elif cmd == "release":
            claims.pop(unit, None)
        else:
            sys.exit(__doc__)
        f.seek(0)
        f.truncate()
        json.dump(claims, f, indent=2, sort_keys=True)
        f.write("\n")


if __name__ == "__main__":
    main()
