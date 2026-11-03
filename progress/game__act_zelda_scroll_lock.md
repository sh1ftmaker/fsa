# game/act_zelda_scroll_lock.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 800052 / 4442088 bytes of code (18.01 %) (rebased onto matching 3729a91, headers v2)

## Status: 5/5 functions, .text 100 %, .data 100 %, .bss 100 %; NonMatching only for extab (weak-dedup tooling)

- `ZeldaScrollLock_804AC708` (actor SZSC, "Zelda scroll-lock NPC", JP
  ゼルダスクロール禁止NPC, in actors.tsv; named after its vtable 0x804AC708,
  stored by the spawner's ctor fn_802013D8; `__nw__` size 0x234).
- Explicit empty destructor, draw (empty), update, init (mState = 0),
  `__sinit` (template: act_talking_npc_2).
- update is a two-state switch on 0x230: state 0 waits until
  fn_802DCD0C(fn_802D6A9C()) is false, then fn_8022ADE4() and state 1;
  state 1 calls fn_8022ADE4() every frame and, once vfunc_0x34() returns
  true, fn_8022ADCC() and fn_801F3668(this).


## symbols.txt (placeholder renames)
- fn_80347E90 / 7EF0 / 7EF4 / 7F88 -> __dt__ / draw / update / init__24ZeldaScrollLock_804AC708Fv
- fn_80347F94 -> __sinit_act_zelda_scroll_lock_cpp (local)
- lbl_804AC708 (0x58) -> __vt__24ZeldaScrollLock_804AC708 (0x54; last word is padding)
- lbl_8050A5F8 (0x100) -> @442..@450 (0xC each) + lbl_8050A664..lbl_8050A6E4
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
