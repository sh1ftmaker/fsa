# game/act_wave_se

Baseline: total 5734 / 11557 functions, 1152552 bytes (25.95 %). After: 5738, 1152916.

Matched: draw, update, ~WaveSe_804A9EA8, init (5/5 incl. sinit). Data 100 %. extab/extabindex below 100 % like the other generated actor units.

SHARED-EDITS
- include/game/ActorManager.h: ActorSoundObj_804578F4 : JAUSoundObject, JKRDisposer (disposer at 0x18, vtable+0x24 stored there; 0x2C is its thunk), ctor (pos, heap, u8), virtual dtor, padding to 0x5C.
- config/G4SE01/symbols.txt: fn_804578F4 -> __ct__22ActorSoundObj_804578F4FPQ29JGeometry8TVec3<f>P7JKRHeapUc (unit tu_80455CA4's placeholder).
- gen_game.tsv: removed this unit's sinit row.
