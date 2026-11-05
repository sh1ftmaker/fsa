# game/act_stage_info_npc.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 799960 / 4442088 bytes of code (18.01 %) (after rebase onto matching 3729a91)

## Status: 5/5 functions (code 716/716), NonMatching (.text/.data/.bss 100 %; extab partial)

- `StageInfoNpc_804AA7A8` (actor STGN, "Stage info NPC" in actors.tsv; named
  after its vtable 0x804AA7A8, stored by the spawner's weak ctor fn_801FF7BC).
- Template: act_boss_display_test_npc (explicit out-of-line dtor first, then
  empty draw/update, then init). init calls
  `fn_802D7934(getGameView_802D6A9C(), field_0x90 & 0xF)` and clears mAlive (0x11C).

## symbols.txt (placeholder renames)
- fn_802F4820 -> __dt__21StageInfoNpc_804AA7A8Fv
- fn_802F4880/84/88 -> draw/update/init__21StageInfoNpc_804AA7A8Fv
- fn_802F48C8 -> __sinit_act_stage_info_npc_cpp (local)
- lbl_804AA7A8 (0x58) -> __vt__21StageInfoNpc_804AA7A8 (0x54; last word padding)
- lbl_804D67B8 (0x100) -> @435..@443 (0xC) + lbl_804D6824..lbl_804D68A4 (nine 0x10 weak memPool_ copies)

## Remaining
- extab/extabindex: the 9 weak JASMemPool dtors' entries (weak-dedup report
  tooling, in review). Everything else is 100 % after the rebase onto
  matching 3729a91 (shared headers v2 named the ActorBase vtable slots).

## SHARED-EDITS
- None open (the v2 headers took this unit's requests; local TODO(shared)
  declarations removed).
