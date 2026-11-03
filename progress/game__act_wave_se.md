# game/act_wave_se

Baseline: total 5734 / 11557 functions, 1152552 bytes (25.95 %). After: 5738, 1152916.

Matched: draw, update, ~WaveSe_804A9EA8, init (5/5 incl. sinit). Data 100 %. extab/extabindex below 100 % like the other generated actor units.

SHARED-EDITS
- include/game/ActorManager.h: ActorSoundObj_804578F4 now derives JAUSoundObject (vtable 0x804B1E28: slots 0x8-0x20 are its, 0x30 is the deleting dtor), ctor (pos, heap, u8 count; asm of 0x804578F4 swaps r5/r6 into JAUSoundObject's ctor), placeholder virtuals 0x24-0x2C, padding to size 0x5C (`li r3, 0x5C` before __nw__).
- config/G4SE01/symbols.txt: fn_804578F4 -> __ct__22ActorSoundObj_804578F4FPQ29JGeometry8TVec3<f>P7JKRHeapUc (unit tu_80455CA4's placeholder).
- gen_game.tsv: removed this unit's sinit row.
