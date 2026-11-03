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
   ~2k-token card replaces them.
2. **`tools/try.py`.** One call compiles the file and diffs it (or one
   function in it), prints ≤20
   lines with a classification (regalloc only, scheduling, missing
   instructions, relocation name), counts attempts, and says STOP when the
   function's try budget (item 8: 3 small, 6 medium, a set budget for large)
   is used up without progress. Regalloc-only goes to the permuter queue.
3. **Per-file packets, fresh small contexts.** As so far: one subagent per
   file. Progress is counted in matched functions. A script
   builds the agent's input for that file: its asm, a cleaned m2c draft of
   every function (m2c is GPL-3, used as a tool, output is from our own
   binary), the file's data and `.sdata2` literals in order, callee
   declarations, header fields at the offsets used, and the closest matched
   file with its C. One file per agent; no long multi-file agents (context
   grew 30k → 290k). Writing the whole file at once is also what fixes
   `.sdata2` literal order, which per-function work can't.
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
8. **Model tiers (approved), applied per file.** A file is tiered by its
   hardest function. Generated-only files: no model. Files whose functions
   are all ≤32 instructions: a cheaper model. Files with functions up to 128
   instructions: strong model. Files with functions >128 instructions (334
   functions over 0x800, 36 % of bytes): last, strong model + permuter, hard
   budget. Within a file, a function stops after its try budget (3 tries
   small, 6 medium) with a FIXME and the agent moves on to the rest of the
   file.
9. **Headers before waves.** Header work and per-unit rebases cost ~70 % as
   much as game matching itself; stabilise headers, then run waves against a
   frozen header set.

## Phases

**G0 — groundwork (small).**
- Rules card; trim prompts in docs/prompts. **Done** (branch
  `infra/g0-tooling`): `docs/RULES_CARD.md` (~2k tokens); `prompts/unit.md`
  and `prompts/review.md` point at it instead of CLAUDE.md, PLAN.md and the
  briefs; AGENT_BRIEF.md and REVIEW_BRIEF.md name it as the entry point.
- `tools/try.py`. **Done** (same branch): compiles one unit's object (no
  link, ~1.5 s; 0.1 s when unchanged), diffs it with objdiff-cli, prints
  ≤20 lines with a class per function (regalloc-only, scheduling(+regalloc),
  reloc-name, reloc-data, missing/extra, different, missing) and the first
  differing rows; attempt log in `build/try/<unit>.json` (an attempt is a
  change to that function's own code, `@NNN` labels ignored); STOP when the
  function's item 8 budget is used without progress (3 tries ≤32
  instructions, 6 up to 128, `--budget N` above, default 10), with "leave a
  FIXME and carry on with the rest of the file". Permuter queueing of
  regalloc-only functions is left to G2.
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
5. Per-actor header convention `include/game/act/<unit>.h` (proposal
   below, "Per-actor headers"); move the existing classes out of their .cpp
   files.
6. Map/Collision, Camera.
7. EnemyBase.h, with tu_801CDC8C and tu_801D1C18 matched early.
8. Sound, UI2D, Message, Game.
Then freeze: later header edits go through the coordinator in batches.

**G2 — tooling.** Packet generator + driver, template generator (sinit,
simple dtor, vtable-slot empties, the per-player query block once one is
matched by hand), review packet, `run-wave`, permuter wiring.

**G3 — generated families.** ~700 functions / ~280 KB. The generator
writes its members into each file's source as a starting point; the file's
agent then does the rest of the file in G4. Review policy for
generated members (approved): the template gets a full adversarial review;
each member gets a scripted check that it differs from the template only by
identifiers, plus the normal gate. Members that need any hand edit leave the
generator and get a normal review.

**G4 — waves of whole files.** The 288 direct 19-slot actor units, smallest first; then the
EnemyBase family; then managers and systems; large functions last with a
budget. Measure tokens per matched function every wave and adjust tiers.

**G5 — the spawner** (`tu_801F9484`, 0x61AC bytes, 367 new + constructor
pairs), once most actor headers exist. Its 285 weak constructors sit in
reverse order of their first call in `fn_801F9484`; confirm on a build.

## Per-actor headers (G1 item 5)

The spawner's unit emits every actor's inline constructor, so each actor
class goes in `include/game/act/<unit>.h` (named like its .cpp): the class
with its inline `Foo() {}` constructor, its overrides and fields with
offsets and size, and any members other units call. Bodies, statics and
literals stay in the .cpp. The 16 act_*.cpp that declared their class in
the .cpp moved there in one byte-identical commit (branch
`infra/g1-actormgr`); every new actor follows.

## Expectations

- Function count rises fast in G3 (≈ +10 % of game functions); bytes move
  slowly until G4 is well under way.
- Units flip to Matching late: `.sdata2` order ties every function in a unit
  together.
- The ~2.3 MB of logic is the real job; the plan's aim is to make each of
  those functions cheap to attempt and quick to give up on.

## Open question: `-inline auto` vs `-inline auto,deferred` (2026-11-03)

Game code builds with `-inline auto`. Two research rounds
(scratchpad notes `research_jtorder.md`, `deferred/`) found evidence both ways:

- For `deferred`: in all 165 original game units with a switch jump table,
  other `.data` (the class vtable) comes before the first jump table. Under
  `auto`, MWCC emits a jump table with its function and the vtable at the end
  of the TU, so no source order gives the original layout (tu_80222CC4
  vfunc_0x90 stays at 99.91 %). `deferred` also puts tu_801D2C60's weak
  inline dtor first, as in the original.
- For `auto`: with every function reversed, `auto,deferred` keeps all 84
  Matching game units at 100 % and the link SHA-1 OK (one explicit dtor
  needed in act_tutorial_old_man; 20 generated sinit units renumber their
  `.bss` `@NNN`). But it inlines calls the original keeps out of line
  (tu_801CDC8C fn_801CF6B8/fn_801CF720, tu_801EDBA4 draw), net -2
  functions. Under `auto` a callee defined after its caller is never
  inlined, which is what the original shows there.

Neither flag explains all of it, so nothing changes yet, and per-unit flags
stay banned (rules card). Units blocked only by jump-table `.data` order or a
first-placed weak dtor keep a FIXME naming this section. Whoever picks it up:
look for what keeps those callees out of line under `deferred` (pragmas,
`-inline` depth/size limits, different callee signatures in the original).

More evidence (waves 24–25):

- tu_8021A570: the original inlines fn_8021ABC0's body into fn_8021A570
  although fn_8021ABC0 comes later in `.text`. Plain `auto` can't do that
  without an earlier inline body (written as a file-static helper that the
  out-of-line fn_8021ABC0 wraps). This points to `deferred`.
- tu_801291A0: the original keeps the small, earlier-defined fn_8012952C out
  of line (three `bl` in update), and `auto` inlines it (update 80.6 %).
  This points away from plain size-based auto inlining, so a size limit or an
  `#pragma dont_inline` region is a candidate.
- JASTrack setChannelPauseFlag: MWCC's recursive-inline depth depends on how
  the null test is written (declaration in the condition gives 7 bodies,
  `!= NULL` gives 6). Inline decisions are sensitive to source form, so
  "callee X stays out of line" is not by itself proof of a flag.

### Measured: whole-project `-inline auto,deferred` (2026-11-03)
Building every game unit with `-inline auto,deferred` instead of `-inline auto`: fully matching game units 103 -> 1, game functions at 100 % 1745 -> 1578 (168 lost, 1 gained: tu_80222CC4's vfunc_0x90). tu_801D2C60, act_shadow_link_bow and tu_8021A570 got worse. FSA's game code is `-inline auto`; the jump-table .data order in tu_80222CC4 / tu_80221B44 / act_shadow_link_bow and tu_801D2C60's leading dtor need another explanation (e.g. those functions living in a different file or header than assumed), not a flag change.

### Measured: reversed sources with `-inline deferred` (no auto) (2026-11-03)
All game sources reversed and built with `-inline deferred` alone: the deferred over-inlining (tu_801CDC8C, tu_801EDBA4) goes away and tu_80222CC4 / act_shadow_link_bow vfunc_0x90 reach 100 %, but callees the original auto-inlines are lost (act_stop_npc, tu_80131D98, tu_8021F4B0 dtors and 5 more): game functions 1745 -> 1738. Under plain `auto` vtables always end the TU (no ctor/dtor/key-function placement or flag changes that); under deferred a vtable comes with its first referencing function, which reproduces tu_800F7FCC's `vt1, jt, vt2`. FSA's originals combine deferred-style data order with auto-style inlining (only callees earlier in .text); no tested flag or pragma (`defer_codegen`, `inline_bottom_up`, `inline_depth`) gives both. Details: progress/game__tu_80222CC4.md.
