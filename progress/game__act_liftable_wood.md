# game/act_liftable_wood.cpp (WOOD, LiftableWood_804A87A8)

Baseline: 9000 / 11557 functions, 1846220 bytes (41.56 %); 2/6 matched in unit.
After: 9001 / 11557 functions (+3 structural), 1846884 bytes (41.58 %); 3/6 at 100%, 3 structural.

Functions:
- `vfunc_0x24__21LiftableWood_804A87A8Fv`: 99.32% (828 bytes, regalloc-only: target->ours r6->r7, f4->f3, r7->r6, f1->f0 in random velocity construction)
- `draw__21LiftableWood_804A87A8Fv`: 100.00% (664 bytes)
- `update__21LiftableWood_804A87A8Fv`: 97.70% (1200 bytes, missing/extra: inline TVec3f(mPos.x - 8.0f, mPos.y, mPos.z) temporary load order and setGlobalTranslation load order)
- `__dt__21LiftableWood_804A87A8Fv`: 100.00% (116 bytes)
- `init__21LiftableWood_804A87A8Fv`: 89.58% (260 bytes, missing/extra: inline TVec3f(mPos.x - 8.0f, mPos.y, mPos.z) temporary load order)
- `__sinit_act_liftable_wood_cpp`: 100.00% (548 bytes)

Sections: `.ctors`, `.rodata`, `.data`, `.bss`, `.sdata2` all 100.00%.

SHARED-EDITS:
- `config/G4SE01/gen_game.tsv`: removed `act_liftable_wood` rows.
- `include/game/BgRewrite.h`: added `void fn_8029D8F0(const JGeometry::TVec3f* pos, s32 layer, s32 layerOffset, s32 pattern);` to `BgRewrite_804A7C98`.
- `config/G4SE01/symbols.txt`:
  - renamed `fn_8029D8F0` to `fn_8029D8F0__18BgRewrite_804A7C98FPCQ29JGeometry8TVec3<f>lll` (called with `gBgRewrite_8053AC98` in `r3`).
  - split `lbl_8046CAF0` (`0x50` bytes) into `lbl_8046CAF0` (`0x10` bytes, `u32[4]`) and the 4 string literals `lbl_8046CB00` (`"wood_ura.bti"`), `lbl_8046CB10` (`"wood_yasi.bti"`), `lbl_8046CB20` (`"wood_yuki.bti"`), `lbl_8046CB30` (`"wood_omote.bti"`).
