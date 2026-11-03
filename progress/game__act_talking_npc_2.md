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

## Trial link (wave 6): why the DOL differs, and the fix (a SHARED-EDIT)
Baseline (wave 6): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)

`link_check.py --unit game/act_talking_npc_2.cpp` gave SHA-1 c690295651...
Diffing the trial ELF (dtk elf2dol) against orig main.dol and the trial map
against splits.txt: every .text/.data byte of this unit is right; the only
layout change is in .bss. Our compiled .bss is 0xFC (9 x 0xC chain objects +
9 x 0x10 weak memPool_ copies), exactly 0x8050EB58-0x8050EC54 as in the
original. The original's next unit, tu_803B8C74, starts its .bss at
0x8050EC60, but dtk's split object of tu_803B8C74 has .bss align 8, so in the
trial link it lands at 0x8050EC58 (-8); every later .bss symbol moves -8 and
.sdata/.sdata2 (32-aligned) move -0x20, which changes all their references.
So the 12 bytes 0x8050EC54-0x8050EC60 are tu_803B8C74's alignment (>= 16),
not data of this unit; in the good link dtk's 0x108-byte split of this unit
hid it.

Proof: with `align:16` added to tu_803B8C74's `.bss` line in splits.txt
(`.bss start:0x8050EC60 end:0x80510E60 align:16`), rm config.json, rebuild,
the trial link is byte-identical (SHA-1 d86b3d9f..., evidence recorded) and
the honest report shows every section of this unit at 100 % (extab and
extabindex included). align:32 would also fit (0x8050EC60 is 32-aligned);
16 is the minimum the layout proves. Reverted here because that stanza is
another unit's; the unit stays NonMatching until the coordinator applies it.

## SHARED-EDITS
- splits.txt, game/tu_803B8C74.cpp stanza: `.bss start:0x8050EC60
  end:0x80510E60 align:16` (see above). Then this unit can flip to Matching
  (run link_check --unit, ninja report, gate).
- tu_801F9484 (spawner): its weak ctor for this class can be named
  `__ct__…` (see the first section for the address).
