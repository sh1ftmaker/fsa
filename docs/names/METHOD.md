# Naming method

How game-code names are chosen, graded and applied. The manifest is
`docs/names/names.tsv`; `tools/rename.py` applies it and reverts it. Actor
IDs and their English names are in `actors.tsv` (see README.md).

## The rule

A wrong name is worse than none. Every name in the manifest cites evidence
someone else can re-check, and the address stays in every name that is not
proven:

| confidence | meaning | form | applied |
|---|---|---|---|
| high | the real name is proven: a game string naming it, an assert / OSReport / `__FILE__` text, an RTTI or symbol/map entry, or a donor's identical function in the same library (JSystem, SDK, MSL) | `RealName` (address dropped) | yes |
| med | strong evidence of what it is, not of what Nintendo called it: behaviour read from the asm and documented in a shared header, a resource string it loads, the actor ID it is constructed for, a donor function of another game with the same body | `Descriptive_<addr>` | yes |
| low | a guess, or evidence that the next reader could reasonably dispute | `Descriptive_<addr>` | yes, by owner decision (below) |

**Low is applied, and stays low.** Owner decision (2026-11-05): "promote the
low-confidence names too; keep a library of what's low confidence". The grade
is the evidence's honest grade and is never relabelled to get a row applied;
whether `tools/rename.py` applies a row is its own column, `apply` (yes|no).
A low name is the weakest in the tree and the first to revisit. It keeps the
address like every name below high, so the old name is recoverable and
`gate.py` still treats it as a placeholder. Where the name itself was the
problem (`gGameRoot2_8053AB10`: "GameRoot2" echoes the provisional type and
says nothing about the role), the row is kept and the library says so; no
replacement is invented without evidence. [`docs/names/LOW_CONFIDENCE.md`](LOW_CONFIDENCE.md)
lists every low row (new name, address, kind, old name, why it is low, what
would confirm or replace it); it is generated, so after changing the manifest
run `python3 tools/names_low.py` (`--check` verifies). `apply` = no remains
available for a row that must be held back (nothing uses it today).

"Address" is the vtable for classes (else the constructor, as in
RULES_CARD.md), the function's own address for functions and the object's
address for globals. Because every provisional name carries it, the old dtk
name is always recoverable from the new one, and `gate.py` keeps treating
the symbol as a placeholder (`_80xxxxxx`), so it stays freely renameable.
Dropping the address makes the symbol real for `gate.py` (renaming it later
needs `approved_symbol_changes.txt`), which is why only proven names do it.

## Evidence sources, tested

Each source was tried on samples before relying on it.

- **Strings in the DOL.** Game code has almost none that name code: the only
  source file name is `RailBase.cpp` (already used), there are no assert or
  JUT panic texts in game units, and `GC4Swords.MAP` is referenced by the
  DOL but is not on the disc (the US disc root has only a `CVS` directory:
  `:pserver:soejima-srd@helios:/home/cvsproject/gc4sword`). OSReport texts
  exist in a few places (`***** particle.jpc load *****` in the effect
  system's constructor). So strings prove very few real names.
- **RTTI / typeinfo.** None: the game is built with `-RTTI off` (PLAN.md
  "Game code"); vtables carry no name strings.
- **Resource names a function loads** (`.blo`, `.bti`, `.jpc`, `.arc`
  entries, J2D pane tags). Good behavioural evidence, med confidence:
  tested on three menu screens: `tu_80414F6C` loads `bat_stage_select.blo`
  (BattleStageSelect_804B1378); the two connect screens load their screens
  from an archive by index, so this source names only some units.
- **Actor IDs.** A class whose constructor the spawner (`fn_801F9484`) calls
  for exactly one 4-character ID is that actor. `config/G4SE01/game_units.tsv`
  records "ID (vtable V set by ctor C)"; `actors.tsv` gives the English name
  and its own confidence. Rule (the same as `tools/gen_game.py`
  `class_name`): one ID per vtable, actors.tsv high or med, CamelCase of the
  English name (punctuation dropped; a parenthetical dropped only when the
  name would exceed 32 characters) plus `_<vtable>`. This names the actor,
  not Nintendo's class, so it is med, never high.
- **Donors.** JSystem/SDK/MSL names are already real (donor files compiled
  to the same bytes). For game code, TWW/TP (both EAD, JSystem era) are
  checked for the same body: `fn_801F71A4` is line for line TWW's
  `cLib_addCalc2` (src/SSystem/SComponent/c_lib.cpp:56) and `fn_801F714C`
  is `cLib_addCalcAngleS2`'s algorithm on u16. FSA has no SComponent library
  (both sit inside the actor manager's unit), so the TWW name is cited but
  not taken as proven: `addCalc2_801F71A4`, med. A donor name is high only
  when the function is in the same library the donor's is (e.g. a JSystem
  file), with the same body.
- **Structural evidence.** Callers, callees and fields, as documented (with
  offsets and call counts) in the shared headers under `include/game/`,
  which were reviewed with the units that proved them. A getter returning
  `lbl_8053AB10->0x28`, the effect system, is `getEffectSystem_8013C824`; a
  spawner that switches on the ActorId and calls init() is `spawnActor`. A
  class whose unit calls JAISeMgr / JAUSectionHeap / JASDriver setup and owns
  the sound pointer is the audio manager (`AudioMgr_8045A080`; TP's
  `Z2AudioMgr` has the same job, but FSA's name is unknown).
- **Runtime (Dolphin).** Available (headless EGL setup, `tools/boot_path.py`
  coverage), but not needed for this sweep; worth it later for functions
  whose role only shows in play (which code runs when an item is used).

Not evidence: a name m2c or another AI suggested, a Japanese label read by
machine translation alone (that is actors.tsv's `mt`, at most med there and
only for technical labels), or a role inferred from a single caller.

## What is never renamed

- Library, SDK and JSystem symbols (real names already), `@NNN` compiler
  symbols, `__sinit_*`, mangled template instantiations.
- Unit (file) names: `tu_<addr>.cpp` renames touch `splits.txt`,
  `configure.py`, objdiff units, claims and progress files; left for a
  separate, coordinator-run step.
- Names spelled out in tools or protected files: `tools/**/*.py`,
  `configure.py` code (its `#` comment lines are renamed),
  `config/G4SE01/{config.yml,splits.txt,approved_symbol_changes.txt,
  provenance_reviewed.txt}` and a `blob` line's symbol in `blobs.txt`.
  `rename.py` reports such a row as BLOCKED and skips it; renaming it needs
  the tool to change first (refer to the entity by address, as
  `tools/gen_game.py` and `tools/tu_map/to_splits.py` do), a coordinator
  decision.

Generator-owned files (the units listed in `gen_game.tsv`: their source,
and the act header for dtor families) are renamed like any other source,
because `tools/gen_game.py` follows the manifest: the entities its templates
spell out are looked up in symbols.txt by address (the emitter release
0x801EE434, the player manager 0x8053AC90, Player.h's anchor 0x8023E724), a
class name is the naming rule's name brought to the current names, and its
records (the names in `gen_game.tsv` and the pinned pre-generation blobs)
are read through `rename.to_current()`: an applied row's old name becomes
the new one, an unapplied row's new name the old one, as symbols.txt shows.
The records are never rewritten, so `gen_game.tsv` and the pins keep the
names they were generated with and `--revert` restores everything exactly;
`gen_game.py --check` still fails on any edit that is not a manifest rename.

## How a name flows through the build

The rename changes text only; the DOL SHA-1 must stay equal.

- `config/G4SE01/symbols.txt` holds CodeWarrior-mangled names: a class
  appears length-prefixed (`__dt__12cls_804B1378Fv`,
  `fn_80402F04__12cls_804AF86CFP7J2DPane`). `rename.py` rewrites
  `<len(old)>old` to `<len(new)>new`, and refuses a class that occurs inside
  a template argument (its template's own length prefix would change; none
  does today). A function name is the part before `__`.
- Headers and sources: the plain identifier, word-bounded; `old__...`
  counts, `old.h` / `old.cpp` (include paths, file names) do not.
- objdiff pairs functions by name in both objects, so a consistent rename
  keeps every percentage; compare reports by address
  (`tools/names_compare.py BEFORE AFTER`).
- Docs and progress notes are renamed too, so they stay greppable; this
  file, LOW_CONFIDENCE.md and names.tsv are not (they cite the old names on purpose).
  `site/` (generated) and `config/` other than symbols.txt are left alone.

## Applying, re-applying, reverting

```sh
python3 tools/rename.py            # dry run: APPLY / done / BLOCKED / REFUSED per row
python3 tools/rename.py --apply    # old -> new
python3 tools/rename.py --revert   # new -> old (exact: git diff is empty afterwards)
python3 tools/rename.py --apply --verify   # then ninja and the DOL SHA-1
```

The manifest is the only input, so the coordinator re-runs `--apply` after
every merge: rows already applied are skipped, old names a merged branch
brought back are renamed. Checks before anything is written: every new name
is an identifier, unique, keeps `_<address>` below high, is not another
row's old name; symbols.txt places the old or new name at the row's address
(and never at another one); no row is blocked. Any REFUSED row stops the
whole run.

`rename.py` takes the manifest's `apply` column (7 columns:
address, kind, old, new, confidence, apply, evidence) and nothing else decides
whether a row is applied; `to_current()` follows symbols.txt for those rows.

Verification for an applied sweep: `ninja` (SHA-1 OK), `python3
tools/gate.py --all-units`, `python3 tools/gen_game.py --check`, `python3
tools/order_check.py`, and `python3 tools/names_low.py --check`, `tools/names_compare.py` against the report
built just before the rename (no function may drop).

## Adding names

Add a row with the evidence (string address and text, donor file and line,
msg#, actor ID, header and offset), dry-run, apply. Prefer fewer solid names
over many guesses; a disputed row goes to `low` with the reason (still applied, see above), then regenerate the library.
