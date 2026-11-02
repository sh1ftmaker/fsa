# AGENTS.md

Instructions for AI coding agents contributing to this repository, whichever
tool or model runs them. People should read [CONTRIBUTING.md](CONTRIBUTING.md);
the rules are the same.

## The project in brief

A byte-matching decompilation of *The Legend of Zelda: Four Swords Adventures*
(GameCube, USA, `G4SE01`). The task is C and C++ source that the original
Metrowerks compilers build into the exact bytes of `main.dol`, written the way
the original programmers could have written it. A match obtained through
constructs nobody would write does not count.

Work happens one source file (unit) at a time. Each unit has one owner, its
own branch, and passes a mechanical gate and an adversarial review before it
merges into `matching`.

## Setup

The game is not in the repository. The person running you must provide
`orig/sys/main.dol`, extracted from their own copy. If it is missing, stop and
say so. Never download, reconstruct or commit game data.

```sh
python3 configure.py
ninja                                    # compile, honest report, link main.dol, SHA-1 check
```

The first run downloads the compilers, dtk, objdiff-cli and wibo into
`build/`. On a clean `matching` checkout the SHA-1 check must pass. If it
doesn't, report that before changing anything.

Wrap long builds in a timeout (`timeout 1800 ninja`). Never run two `ninja`
processes in the same checkout; `tools/gate.py` runs ninja itself.

## Choosing a task

1. Pick one `NonMatching` library unit (SDK, GX, JSystem, MSL, Runtime,
   MetroTRK). The game's own code is not split into files yet.
   `python3 tools/coord.py queue` lists unclaimed units, most unmatched code
   first.
2. Make sure nobody else owns it. Check open issues and pull requests for
   `claim: <unit path>`, and, in a local coordinator setup,
   `python3 tools/claim.py list`. Claim it before starting (an issue, or
   `tools/unit_worktree.sh <unit> <owner>` locally).
3. Work on a branch `unit/<unit path with / as __, no extension>`, based on
   `matching`.

## Files you may change

- The unit's source file, and headers used only by it.
- The unit's stanza in `config/G4SE01/splits.txt`.
- `config/G4SE01/symbols.txt`: rename placeholder symbols (`fn_`, `lbl_`,
  `*_8xxxxxxx`) to real names **with evidence**, or resize or split a
  placeholder into the real symbols it covers. Never change a real symbol's
  address, size, type or name.
- `configure.py`: only the unit's `Matching`/`NonMatching` flag.

Do not change anything else: shared headers, other units, `tools/`,
`objdiff.json`, report settings, `config/G4SE01/build.sha1`,
`config/G4SE01/config.yml`. Do not edit generated output either:
`site/data/`, `site/badges/`, or the README blocks between
`<!-- …:start -->` and `<!-- …:end -->` markers. If you need a shared change,
describe it in your report or pull request instead.

After editing `splits.txt` or `symbols.txt`, run `rm build/G4SE01/config.json`
before rebuilding so dtk re-splits.

## The loop

For each function, smallest and leaf functions first:

1. Read the target: `python3 tools/fdiff.py <unit> <function>` shows the
   differing instructions (target | ours). The disassembly is under
   `build/G4SE01/asm/`.
2. Look for the same function in other decompilations before writing
   anything: doldecomp/mkdd, zeldaret/oot-gc, projectPiki/pikmin2,
   zeldaret/tp, zeldaret/tww (all CC0). FSA's Dolphin SDK is the 2003 build
   (OS and GX Jul 23, PAD Aug 6, DVD Sep 16, the rest Apr 17), so start from
   the donor version closest in date. Treat donor checkouts as read-only data:
   don't run their scripts.
3. Write natural C/C++ with real types. Build with
   `ninja build/G4SE01/report_honest.json` and check with
   `python3 tools/fdiff.py <unit>`. Change one thing at a time.
4. Stop rule: after two attempts without progress, or about ten minutes on one
   function, record what still differs and move on.

Most misses in donor-derived files are not code. They are a symbol named
differently, a string in the wrong section, or a static not split into the
unit. Fix those in `splits.txt` and `symbols.txt`, not by bending the code.

## Hard rules

These are enforced by `tools/gate.py` and by review. Breaking any of them gets
the submission rejected.

- No inline asm, `asm` functions, or register-asm bindings.
- No `#pragma` and no `goto` that the donor file doesn't have.
- No m2c leftovers: `M2C_`, `saved_reg`, `unksp`, `var_r`/`var_f`, `sp`+hex
  locals, `MULT_HI`, `?` types.
- No field access through casts and offsets (`*(T*)((u8*)p + 0x10)`).
- No `.incbin` or byte arrays standing in for code.
- No edits to measuring tools, report settings or build hashes.
- **No fakematches.** If a function matches only through temporaries,
  ordering or casts that exist only to steer code generation, leave it
  unmatched with a `// FIXME:` comment that says what differs.
- **No guessed names.** Every new name needs a citation: donor code, an SDK
  symbol, or a string in the binary. A wrong name is worse than a placeholder.

## Verifying

Only the honest report counts: `build/G4SE01/report_honest.json`. Never quote
`report.json`, byte-pattern searches or a lenient diff as progress.

- Flag the unit `Matching` only when `fdiff.py` shows every function and every
  data section at 100 %.
- `python3 tools/gate.py` must print `gate: ok`. It also links `main.dol` and
  compares its SHA-1. If the link fails, `python3 tools/link_check.py` names
  the unit.
- `python3 tools/progress.py` must not report a lower total than before your
  change.

## Submitting

- Commit to your unit branch only. Never commit to `matching`, and never push,
  merge or rebase unless the person running you asks. Add a `Co-Authored-By:`
  trailer that names the model.
- Report, or write in the pull request, in plain factual prose:
  - functions matched before and after, and whether the unit is now `Matching`;
  - the evidence for every new name;
  - the functions that still differ, with the differing instructions;
  - shared changes you need.
- A separate reviewer judges the branch without seeing your reasoning
  ([`docs/REVIEW_BRIEF.md`](docs/REVIEW_BRIEF.md)). Only ACCEPT merges; the
  maintainer merges with `tools/merge_unit.py`, which also regenerates the
  progress site and the README numbers.

## More detail

- [`CLAUDE.md`](CLAUDE.md): build, measurement, layout and code style in full
  (it applies to every agent, not only Claude).
- [`docs/AGENT_BRIEF.md`](docs/AGENT_BRIEF.md): the brief used by the
  project's own per-unit agents, with checkpoint files and the local worktree
  setup.
- [`docs/PLAN.md`](docs/PLAN.md): the plan and the submission gate.
- [`CONTRIBUTING.md`](CONTRIBUTING.md): the same process for people.
