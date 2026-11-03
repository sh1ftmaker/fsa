# game/act_play_specified_se.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 800036 / 4442088 bytes of code (18.01 %) (rebased onto matching 3729a91, headers v2)

## Status: 5/5 functions, .text 100 %, .data 100 %, .bss 100 %; NonMatching only for extab (weak-dedup tooling)

- `PlaySpecifiedSe_804A9F00` (actor SEN2, "Play specified SE" in actors.tsv;
  named after its vtable 0x804A9F00, stored by the spawner's ctor
  fn_802011F0; `__nw__` size 0x23C).
- Explicit empty destructor, draw (empty), update, init, `__sinit`
  (template: act_talking_npc_2 / act_tutorial_old_man).
- init splits the spawn parameter (0x90): bit 26 -> 0x230, low 8 bits ->
  0x234 (index), bits 8-19 -> 0x238 (frame delay). update counts 0x238 down;
  at 0, unless 0x230 is set, calls fn_801F0D20(this, lbl_80541250[0x234]),
  then clears mAlive.
- lbl_80541250 (.sdata2) is a one-entry `static const u32[]` {3}; kept as a
  placeholder name (no evidence that it is a sound-id table beyond the
  actor's name).

## symbols.txt (placeholder renames)
- fn_802E7634 / 7694 / 7698 / 7700 -> __dt__ / draw / update / init__24PlaySpecifiedSe_804A9F00Fv
- fn_802E7728 -> __sinit_act_play_specified_se_cpp (local)
- lbl_804A9F00 (0x58) -> __vt__24PlaySpecifiedSe_804A9F00 (0x54; last word is padding)
- lbl_80541250: size 0x8 -> 0x4, scope:local (the second word is padding)
- lbl_804D51A8 (0x100) -> @440..@448 (0xC each) + lbl_804D5214..lbl_804D5294
  (nine 0x10 weak memPool_ copies)

## Why data is not 100 %
- extab/extabindex: the 9 weak JASMemPool dtors' entries (weak-dedup report
  tooling, in review). Everything else is 100 %.

## Rebase onto headers v2 (3729a91)
- Local `// TODO(shared)` declarations removed; the unit uses ActorBase.h /
  GameGlobals.h (fn_801F0D20 and fn_801F3668 are now ActorBase members).
- The former FIXME'd update (vfunc_0x30(u8, bool) / bool vfunc_0x34()) now
  matches; the vtable matches with the renamed slot functions.

## SHARED-EDITS
- tu_801F9484 (spawner): its weak ctor for this class can be named
  `__ct__…` (see the first section for the address).
