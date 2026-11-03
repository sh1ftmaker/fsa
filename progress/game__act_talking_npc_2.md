# game/act_talking_npc_2.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 800016 / 4442088 bytes of code (18.01 %) (rebased onto matching 3729a91, headers v2)

## Status: 5/5 functions, .text 100 %, .data 100 %, .bss 100 %; NonMatching only for extab (weak-dedup tooling)

- `TalkingNpc2_804AFAD8` (actor TLK2, "Talking NPC 2" in actors.tsv; named
  after its vtable 0x804AFAD8, stored by the spawner's ctor fn_80203054 after
  `__ct__18ActorBase_804A5770Fv`; `__nw__` size 0x240).
- Explicit (out-of-line) empty destructor first, then draw, update (empty),
  init, then `__sinit` (9 pools via game/GameAudio.h).
- init splits the spawn parameter (0x90): low 16 bits -> 0x234, bits 16-27
  -> 0x238, bits 28-31 -> 0x23C (u8), and calls fn_8037F2FC (tu_8037DF68,
  a text/message-box setup: it caches r3 at 0x3C of lbl_8053AD78 and
  re-measures a J2DTextBox string when it changes) with the position
  (x, y, z, z) and `JUtility::TColor(0xFF0000FF)` by value (the temp at
  sp+0x8 and the by-value copy at sp+0xC are exactly what the target has).
- Field 0x230 is not touched by this unit (declared for layout only).

## symbols.txt (placeholder renames)
- fn_803B8970 -> __dt__20TalkingNpc2_804AFAD8Fv
- fn_803B89D0/D4/D8 -> draw/update/init__20TalkingNpc2_804AFAD8Fv
- fn_803B8A50 -> __sinit_act_talking_npc_2_cpp (local)
- lbl_804AFAD8 (0x58) -> __vt__20TalkingNpc2_804AFAD8 (0x54; last word is padding)
- lbl_8050EB58 (0x108) -> @441..@449 (0xC each) + lbl_8050EBC4..lbl_8050EC44
  (nine 0x10 weak memPool_ copies); 0x8050EC54-0x8050EC60 stays alignment padding.

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
