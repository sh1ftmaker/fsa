You own the unit **{unit}** (source {source}) in the Four Swords Adventures byte-matching decompilation. Your worktree: {worktree} (branch {branch}). Work only there.

Read {worktree}/docs/RULES_CARD.md and follow it exactly; it replaces AGENT_BRIEF.md, PLAN.md and COORDINATOR.md (don't read those, or CLAUDE.md again). Start with `timeout 1800 ninja build/G4SE01/report_honest.json`, record `python3 tools/progress.py | head -1` in your checkpoint as your baseline, then `python3 tools/try.py {unit}`. Use `python3 tools/try.py {unit} <function>` for every attempt; when it prints STOP for a function, leave a FIXME saying what differs and carry on with the rest of the file.

Current state: {status}

{notes}

Other agents own other units at the same time: touch only your unit's files and list anything shared under SHARED-EDITS. Your work will be adversarially reviewed. Budget about 90 minutes; stop earlier if everything left has hit STOP. End with the report the card describes.
