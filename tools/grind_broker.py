#!/usr/bin/env python3
"""Compile broker for sandboxed grind sessions (tools/grind.py).

A session's Bash runs in Claude Code's sandbox, whose syscall filter kills wibo (the shim
that runs the Metrowerks compiler). So the two commands that compile, tools/try.py and
`tools/grind.py check`, hand their arguments to a broker thread of the worker process,
which runs them outside the sandbox with the worktree as its directory and passes the
output back. Nothing else leaves the sandbox. The broker refuses to run while tools/ or
configure.py differ from git, so a session cannot change what runs outside.

Protocol: build/grind/broker/ACTIVE exists while a broker serves this worktree; a client
writes req-<id>.json {"kind": "try"|"check", "argv": [...]} and waits for resp-<id>.json
{"rc": int, "out": str}.
"""

import json
import os
import subprocess
import sys
import time
from pathlib import Path

DIR = Path("build/grind/broker")
KINDS = {"try": ["python3", "tools/try.py"], "check": ["python3", "tools/grind.py", "check"]}


def maybe_delegate(kind, argv):
    """Called first thing by a compiling command: inside a served session, let the broker run it."""
    if os.environ.get("GRIND_BROKER_SERVER") or not (DIR / "ACTIVE").exists():
        return
    rid = f"{os.getpid()}-{time.time_ns()}"
    tmp = DIR / f"req-{rid}.tmp"
    tmp.write_text(json.dumps({"kind": kind, "argv": list(argv)}))
    tmp.rename(DIR / f"req-{rid}.json")
    resp = DIR / f"resp-{rid}.json"
    for _ in range(15000):  # 25 min
        if resp.exists():
            d = json.loads(resp.read_text())
            resp.unlink()
            sys.stdout.write(d["out"])
            sys.exit(d["rc"])
        if not (DIR / "ACTIVE").exists():
            break
        time.sleep(0.1)
    sys.exit("compile broker did not answer")


def serve(wt, stop):
    """Answer requests for worktree `wt` until `stop` (a threading.Event) is set."""
    d = Path(wt) / DIR
    d.mkdir(parents=True, exist_ok=True)
    for f in d.glob("re*-*"):
        f.unlink()
    (d / "ACTIVE").write_text(str(os.getpid()))
    env = dict(os.environ, GRIND_BROKER_SERVER="1")
    try:
        while not stop.is_set():
            reqs = sorted(d.glob("req-*.json"))
            if not reqs:
                time.sleep(0.05)
                continue
            for req in reqs:
                try:
                    r = json.loads(req.read_text())
                    cmd = KINDS[r["kind"]] + [str(a) for a in r["argv"]]
                    dirty = subprocess.run(["git", "status", "--porcelain", "--", "tools", "configure.py"], cwd=wt,
                                           capture_output=True, text=True).stdout.strip()
                    if dirty:
                        rc, out = 1, ("tools/ and configure.py must stay as they are (edits there are never kept): "
                                      "revert " + ", ".join(l[3:] for l in dirty.splitlines()[:4]) + "\n")
                    else:
                        p = subprocess.run(cmd, cwd=wt, capture_output=True, text=True, timeout=1500, env=env)
                        rc, out = p.returncode, p.stdout + p.stderr
                except Exception as e:  # answer anyway: the client is waiting
                    rc, out = 1, f"broker error: {e}\n"
                tmp = d / (req.name.replace("req-", "resp-") + ".tmp")
                tmp.write_text(json.dumps({"rc": rc, "out": out}))
                tmp.rename(d / req.name.replace("req-", "resp-"))
                req.unlink(missing_ok=True)
    finally:
        (d / "ACTIVE").unlink(missing_ok=True)
