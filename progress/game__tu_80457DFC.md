
## 2026-11-05 small-function pass
Baseline 7964 functions (unit 1/33). After: 7977 (unit 14/33, +13).
Matched: fn_80457DFC, fn_80457E04, fn_80457F60, fn_80457F6C, fn_80458090, fn_804580DC (bool), fn_8045873C, fn_80458F9C, fn_804594E4, fn_8045959C, fn_804595E4, ctor, fn_8045A0D8.
Resistant: fn_80459624 79.35 % (scheduling+regalloc: original stores each coordinate right after its load; ours loads x and z first). Tried TVec3f/Vec param, non-const pointer, TVec3f member set().
Not written: fn_8045A0E4 (JASPoolAllocObject<JAUDopplerAudible<4>>::newMemPool(int); MWCC rejects explicit member instantiation; it will appear when fn_80459AE4, its caller, is written).
SHARED-EDITS: include/game/GameGlobals.h Unk_8045A080 fields named (0x0..0x8d), forward decls, member decls for the functions above; fn_804580DC returns bool. symbols.txt renamed 10 placeholders to mangled member names. gen_game.tsv row removed.
Next: the rest of the unit (fn_80457E54, 80457F80, 8045815C, ...).
