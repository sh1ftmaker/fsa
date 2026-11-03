# game/tu_801D3CBC (ShadowLink2_804A43E8 draw functions)

Baseline progress.py: total: 6426 / 11557 functions, 1284740 / 4442032 bytes of code (28.92 %).

Matched: __sinit (was already). Resistant (no function at 100 %):
- fn_801D3CBC 98.33 % (missing/extra): original has one extra unreachable `b` after the
  switch dispatch; an empty case for 0x16/0x18/0xE changes the compare tree (tried).
- fn_801D3DAC 97.26 % (scheduling): original loads field_0x4 for viewLayerFlag among the
  SpriteParams ctor stores; and field_0x560 + mState together before the palette stb.
- fn_801D41B8 87.61 % (scheduling+regalloc): pos copy order (x,y,z into f6,f5,f4 then stores).
sdata2 86 %: 0x8053F0C0 (0xC) and 0x8053F0C4 (0x1E) not defined (unknown users).

SHARED-EDITS: GameGlobals.h (lbl_8053EDF8/EDFC/EE00 extern const s32, fn_80307830);
act_shadow_link_2.h (3 members); gen_game.tsv row removed; symbols.txt renames to mangled names.
