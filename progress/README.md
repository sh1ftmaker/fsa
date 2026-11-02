# Per-unit progress

- `claims.json`: unit -> {owner, branch, worktree, started}. Edit only through
  `tools/claim.py` (it takes a lock).
- `<unit path with / replaced by __>.md`: the owner's checkpoint, rewritten
  after every function: matched, resistant (with the differing instructions),
  SHARED-EDITS needed, next targets.
- `LOG.tsv`: one row per agent run: unit, date, model, minutes, functions
  matched before -> after, notes.
