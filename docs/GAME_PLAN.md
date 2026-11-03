# Game code plan

Status: **approved** (2026-11-02). The user chose tiered models, template +
scripted-check review for generated members, and to start G0 + G1 now.
Built from three research reports (structure, shapes, token cost), each measured
on `matching` at 4947/11557 functions. Game code: 243 / 6789 functions,
1.04 % of 3,573,784 bytes.

## What the game code looks like

- **No donor engine.** No f_op/d_/mDo strings, actors are created by a switch
  on 4-character IDs, and 972 game strings match nothing in mkdd, tp, tww, sms
  or pikmin beyond generic substrings. Donors help only with JSystem inlines.
  No .map, RELs or RTTI; the only game file name is RailBase.cpp. Name sources
  are actor IDs and EnemyParam.csv, 1,550 resource-name strings, library
  vtable slots, and weak library code kept in game units.
- **Hierarchy.** 475 game vtables, no multiple inheritance. 377 (79 %) descend
  from `ActorBase_804A5770` (19 slots); 288 of those are direct 19-slot
  children. The main intermediate base is an enemy base (`lbl_804A4808`, 39
  slots, 52 classes, 7.2 % of game text) whose destructor is a header inline.
  98 vtables are non-actors; 78 have only a virtual destructor. 393
  pointer-to-member tables (state machines) sit in 26 units.
- **The spawner** `tu_801F9484` inlines the constructors of 293 classes from
  289 units, in `case` order. So every actor class must be declared in a
  header, and the 12 `act_*.cpp` files that declare their class in the .cpp
  must move it out.
- **Cross-unit references.** 13,443 references to 3,031 game symbols; today's
  `include/game` covers 22 %. 29 owner headers fully cover half the units and
  75 cover three quarters. The real bottleneck is about six struct layouts:
  GameRoot, GameRoot2, GameView, the player singleton, the UI struct
  `lbl_80529DEC`, and EnemyBase.
- **Single biggest blocker:** `*lbl_8053AAF8 + 0xB4` is a
  `JMath::TRandom_fast_`; its code is inlined 656 times in 132 units, none of
  which can match until the header has it.

## Shapes

| Shape | Unmatched | Bytes | How |
|---|---|---|---|
| 9-pool audio `__sinit` | 465 | ~254 KB (7.1 %) | generator (26 + 45 variants with extra statics by hand) |
| Simple actor destructor `~cls_V() {}` | 194 | ~12 KB | generator (reads the vtable, renames it, 0x58 → 0x54) |
| Empty functions | 190 | small | generator only where they fill a vtable slot |
| Spawner-inlined actor constructors | 271 | | one header per actor; 236 are `Foo() {}` |
| `tu_8022AD64` per-player queries | 269 | ~50 KB | match one by hand, generate 268 |
| Getters/setters, call wrappers | ~510 | small | cheap model; types need judgment |
| Jump-table switches (81 actor `update`s) | 248 | 667 KB (18.6 %) | hand |
| Remaining logic, float-heavy, large | | ~2.3 MB | hand, strong model + permuter |

25 % of game functions are exact clones of another apart from names (1,687;
five families hold 1,186), and 847 unmatched functions already have a matched
twin. But all template-friendly shapes together are only ~10 % of the bytes,
and they complete almost no units, because every actor still needs its
`init`/`update` by hand. Templates raise the function count; the bytes are in
hand-matched medium and large functions.

## Hazards for any mass approach

1. **`.sdata2` literal order.** ~37 % of functions (~72 % of bytes) load float
   constants, and their relocations pair only once the whole unit's literals
   are in the original order. Partly done units plateau below 100 %.
2. **`@NNN` renumbering.** Compiler label numbers depend on the whole file; a
   generator re-reads them from our compiled object after every change.
3. **Include and pool order.** `GameAudio.h` before any other JAudio header.
4. **Flipping to Matching** needs `link_check.py --unit`; each flip makes other
   trial links stale.
5. **Spawner constructors** may be emitted only in the spawner.
6. **ActorBase slots.** Adding a slot shifts 162+ vtables; settle it first.
7. **`symbols.txt` churn.** dtk re-sorts it on every split, so generated output
   lands per unit, never as one mass edit.
8. No per-unit flags, no fakematch constructs in generated source.

## Where the tokens go, and the fix

Measured over the decomp so far (296M input-token equivalents): unit agents
46 %, reviewers 25 %, coordinator 16 %, infra 13 %. Cache reads are 60–88 % of
every slice, so cost ≈ turns × context size. The game functions matched so far
cost ~165k each including review (~275k with header work), and they were the
easiest ones. At that rate the remaining 6,546 cost ~2.5B; with the changes
below, ~0.5–0.65B.

1. **Rules card instead of docs.** Agents re-read CLAUDE.md, briefs, PLAN.md
   and COORDINATOR.md (+9k units, +24k reviewers, paid on every turn). A
   ~1.5k-token card replaces them.
2. **`tools/try.py`.** One call compiles and diffs one function, prints ≤20
   lines with a classification (regalloc only, scheduling, missing
   instructions, relocation name), counts attempts, and says STOP after 4
   tries without progress. Regalloc-only goes to the permuter queue.
3. **Per-function packets, fresh small contexts.** A script builds each
   worker's input: the asm, a cleaned m2c draft (m2c is GPL-3, used as a tool,
   output is from our own binary), callee declarations, referenced data,
   header fields at the offsets used, and the closest matched function with its
   C. One unit per agent; no long multi-unit agents (context grew 30k → 290k).
4. **Template generator** for the clone families: compile, diff and gate with
   no model.
5. **Review packets.** A script gives the reviewer the gate result, report
   deltas, diff scope, each changed function beside its asm, and the name
   evidence. The reviewer judges without rebuilding, in batches with separate
   verdicts. Every submission still gets an adversarial review.
6. **`coord.py run-wave`.** A script runs waves; the coordinator handles
   exceptions only and starts fresh each wave.
7. **Permuter** (decomp-permuter, MIT) for regalloc-only diffs, under `sbg`,
   niced, with a time budget, instead of 150–200-turn agents.
8. **Model tiers (approved).** Clones: no model. ≤32 instructions: a cheaper
   model, 3 tries. 32–128: strong model, 6 tries. >128 (334 functions over
   0x800, 36 % of bytes): last, strong model + permuter, hard budget, FIXME
   allowed.
9. **Headers before waves.** Header work and per-unit rebases cost ~70 % as
   much as game matching itself; stabilise headers, then run waves against a
   frozen header set.

## Phases

**G0 — groundwork (small).**
- Rules card; trim prompts in docs/prompts.
- `tools/try.py`.
- Rename `lbl_8049CC34` → `__vt__12J3DFrameCtrl`, `fn_80105DD4` →
  `__dt__12J3DFrameCtrlFv` (every store of that vtable is followed by
  `J3DFrameCtrl::init`, pikmin2's inline ctor; fixes relocations in 70 units).
  Goes through review like any rename.

**G1 — shared headers** (one agent per header, reviewed, merged in order).
1. ActorBase.h: settle the 19 slots; declare the ~60 non-virtual members
   (`fn_801F0E34` alone is used by 222 units).
2. GameGlobals.h: `TRandom_fast_` at +0xB4, `JKRArchive*` at +0xA4,
   GameRoot2 +0x10/+0x20/+0x2C/+0x30.
3. ActorManager.h: the remaining manager functions.
4. Player.h, GameView.h, Effect.h (225–325 dependent units each).
5. Per-actor header convention `include/game/act/<actor>.h`; move the 12
   existing classes out of their .cpp files.
6. Map/Collision, Camera.
7. EnemyBase.h, with tu_801CDC8C and tu_801D1C18 matched early.
8. Sound, UI2D, Message, Game.
Then freeze: later header edits go through the coordinator in batches.

**G2 — tooling.** Packet generator + driver, template generator (sinit,
simple dtor, vtable-slot empties, the per-player query block once one is
matched by hand), review packet, `run-wave`, permuter wiring.

**G3 — generated families.** ~700 functions / ~280 KB. Review policy for
generated members (approved): the template gets a full adversarial review;
each member gets a scripted check that it differs from the template only by
identifiers, plus the normal gate. Members that need any hand edit leave the
generator and get a normal review.

**G4 — waves.** The 288 direct 19-slot actor units, smallest first; then the
EnemyBase family; then managers and systems; large functions last with a
budget. Measure tokens per matched function every wave and adjust tiers.

**G5 — the spawner** (`tu_801F9484`, 0x61AC bytes, 367 new + constructor
pairs), once most actor headers exist.

## Expectations

- Function count rises fast in G3 (≈ +10 % of game functions); bytes move
  slowly until G4 is well under way.
- Units flip to Matching late: `.sdata2` order ties every function in a unit
  together.
- The ~2.3 MB of logic is the real job; the plan's aim is to make each of
  those functions cheap to attempt and quick to give up on.
