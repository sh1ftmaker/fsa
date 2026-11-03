# Brief for adversarial reviewers

You review one unit branch (`unit/<name>`) before it is merged into
`matching`. You have not seen the author's reasoning, and you shouldn't trust
its report: assume the author wanted the percentage to go up and look for
every way it might have cheated, cut corners or guessed.
**Entry point: `docs/RULES_CARD.md`**: it states what's allowed (distilled
from CLAUDE.md, PLAN.md and AGENT_BRIEF.md, which you only open to cite a
section).

You don't fix anything and you don't commit. You produce a verdict.

## What to check

1. **Measurements are real.** In the branch's worktree:
   `python3 tools/try.py <unit>` (compiles and diffs only this unit),
   the main checkout's `python3 tools/gate.py` (builds the report, links),
   `python3 tools/fdiff.py <unit> <function>` for a full diff. Confirm the claimed functions are 100 %,
   the unit's `Matching` flag (if set) holds for code and data, and
   `tools/progress.py` total is not below `matching`'s (build `matching` in a
   scratch worktree if you need the baseline).
2. **Nothing measuring was touched.** `git diff matching...unit/<name> --stat`:
   only the unit's source, headers used only by it, its `splits.txt` stanza,
   `symbols.txt` renames, its `configure.py` flag, its checkpoint. Anything
   else is a finding.
3. **No tricks the gate can't see.** Read every changed function against its
   target asm. Look for: statements or temporaries that exist only to steer
   register allocation or scheduling; reordered or duplicated code no
   programmer would write; volatile or casts used to force codegen; dead
   stores; magic numbers where a donor uses a named constant or field;
   struct offsets reached by arithmetic; types chosen to force a sign/zero
   extension rather than because the data is that type; anything copied
   from the donor that doesn't fit FSA's version but happens to compile.
   Compare with the donor file: a difference from the donor needs a reason.
   Pointer-cast word copies (`*(u32*)this = *(u32*)&other`) are tricks with
   no exception: a J3D `operator=` exception was evaluated and rejected
   (docs/COORDINATOR.md, "Evaluated and rejected").

4. **Data and symbols.** `splits.txt` ranges are the unit's real data (check
   neighbours aren't swallowed or left with gaps); `symbols.txt` changes
   only rename placeholders, never move or resize; every new name has
   evidence (donor symbol, SDK header, string). A guessed name is a finding.
5. **Readability.** Real types and names, no m2c style, loops not gotos,
   enums/defines for flags and registers where the donor has them, comments
   only where they explain why. `// FIXME:` notes on unmatched functions say
   what differs.

## Verdict

Write `progress/<unit>.review.md` in the main checkout's scratch area given
to you, and report:

- **ACCEPT**: no findings that block merging (minor style notes allowed).
- **CHANGES**: list each finding with file:line, what's wrong, and what a
  fix looks like. A trick or an unsupported name is always CHANGES.
- **REJECT**: the branch reaches its numbers by cheating or breaks other
  units.

Be specific and cite lines. Don't pad the review with praise.
