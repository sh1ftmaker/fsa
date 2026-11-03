# game/act_gba_appear
Baseline: total 6340 / 11557 functions, 1270548 bytes (28.60 %). After: 6342, 1270756.
Matched: init, dtor (+ sinit). 3/5 functions.
Resistant:
- draw 96.74% scheduling+regalloc: scale in f29 / sines f28 f27 f26 / cosines f31 f30 f3 in the original; ours f31 and others (same as act_hardhat_beetle's setRotScaleMtx). Bit test needs `(x & bit) ? true : false`.
- update 92.22% missing/extra: original frame 0x80 vs ours 0x70 (four distinct by-value TVec3f copy slots 0x8/0x14/0x20/0x2C; ours share), and pos copy scheduled before the camera load.
SHARED-EDITS: include/game/GameGlobals.h: `extern "C" void fn_8043E7CC(cls_804B1920*, JGeometry::TVec3f)` (owner tu_8043A0E8; r3 = *lbl_8053AED0, by-value position).
Names: GBAAppear_804B16E0 (GBAO "Appears from GBA", actors.tsv med, ctor fn_80201B78 stores the vtable, alloc 0x244). symbols.txt: draw/update/init/dtor/__vt__ renamed.
Removed this unit's gen_game.tsv row (sinit).
Next: the by-value helpers storePos/callE7CC are file-local; likely GameState / cls_804B1920 inlines.
