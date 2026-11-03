# game/act_boss_display_test_npc.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 799900 / 4442088 bytes of code (18.01 %) (after rebase onto matching 3729a91)

## Status: 5/5 functions (code 656/656), NonMatching (.text/.data/.bss 100 %; extab partial)

- `BossDisplayTestNpc_8049C7F0` (actor BOTS, "Boss display test NPC" in
  actors.tsv; named after its vtable 0x8049C7F0, stored by the spawner's weak
  ctor fn_802020F0 after `__ct__18ActorBase_804A5770Fv`).
- Same shape as act_tutorial_old_man (empty init/update/draw), except the
  destructor is first in .text: it is an explicit out-of-line `~X() {}`
  defined before the others (an implicit dtor is emitted after them).

## symbols.txt (placeholder renames)
- fn_800FEDD0 -> __dt__27BossDisplayTestNpc_8049C7F0Fv
- fn_800FEE30/34/38 -> draw/update/init__27BossDisplayTestNpc_8049C7F0Fv
- fn_800FEE3C -> __sinit_act_boss_display_test_npc_cpp (local)
- lbl_8049C7F0 (0x58) -> __vt__27BossDisplayTestNpc_8049C7F0 (0x54; last word padding)
- lbl_804BACF8 (0x100) -> @433..@441 (0xC) + lbl_804BAD64..lbl_804BADE4 (nine 0x10 weak memPool_ copies)

## Remaining
- extab/extabindex: the 9 weak JASMemPool dtors' entries (weak-dedup report
  tooling, in review). Everything else is 100 % after the rebase onto
  matching 3729a91 (shared headers v2 named the ActorBase vtable slots).

## SHARED-EDITS
- None open (the v2 headers took this unit's requests; local TODO(shared)
  declarations removed).
