# game/act_local_switch.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 800028 / 4442088 bytes of code (18.01 %) (rebased onto matching 3729a91, headers v2)

## Status: 5/5 functions, .text 100 %, .data 100 %, .bss 100 %; NonMatching only for extab (weak-dedup tooling)

- `LocalSwitch_804AA008` (actor LCLS, "Local switch" in actors.tsv; named
  after its vtable 0x804AA008, stored by the spawner's ctor fn_80200E1C;
  `__nw__` size 0x230, so no members of its own).
- Explicit empty destructor, draw (empty), update, init (calls update()
  virtually), `__sinit` (template: act_talking_npc_2).
- update: `if (fn_802D7F40(getGameView_802D6A9C(), (field_0x90 >> 22) & 0x1F))
  vfunc_0x30(0xFF, 1);`


## symbols.txt (placeholder renames)
- fn_802E8DFC / 8E5C / 8E60 / 8EBC -> __dt__ / draw / update / init__20LocalSwitch_804AA008Fv
- fn_802E8EE8 -> __sinit_act_local_switch_cpp (local)
- lbl_804AA008 (0x58) -> __vt__20LocalSwitch_804AA008 (0x54; last word is padding)
- lbl_804D54A8 (0x100) -> @439..@447 (0xC each) + lbl_804D5514..lbl_804D5594
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
