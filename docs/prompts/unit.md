You own the unit **{unit}** (source {source}) in the Four Swords Adventures byte-matching decompilation. Your worktree: {worktree} (branch {branch}). Work only there.

Read docs/AGENT_BRIEF.md in your worktree first and follow it exactly (loop, donors, lessons, what you may change, no tricks, finishing checklist); also CLAUDE.md and docs/PLAN.md. Start with `timeout 1800 ninja build/G4SE01/report.json`, then record `python3 tools/progress.py`'s first line in your checkpoint as your baseline (you'll need it to show nothing regressed), then `python3 tools/fdiff.py {unit}`.

Current state: {status}

{notes}

Housekeeping: don't stage progress/claims.json (a symlink to the coordinator's file); objdiff.json is generated and untracked. dtk re-sorts symbols.txt on every split: commit only your own symbol changes. Other agents own other units at the same time; touch only your unit's files and list anything shared under SHARED-EDITS. Your work will be adversarially reviewed: no tricks, every name evidenced by the bytes. Budget about 90 minutes; stop earlier if you're stuck on everything that's left. Wrap every build in `timeout`. End with the final report described in the brief.
