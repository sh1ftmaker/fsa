# game/act_sandstorm.cpp checkpoint

Baseline: total 5866 / 11557 functions, 1174344 bytes (26.44 %). After: 5870 / 11557, 1175440 bytes.

Matched: draw, update, dtor, init (all 100 %), plus the generated __sinit. 5/5.
Resistant: none.
SHARED-EDITS: include/game/GameGlobals.h GameRoot_8049EF20 gained `s32 field_0x4c`
(lwz 0x4C, compared with cmpwi 0xB in update; split from field_0x8[]); gen_game.tsv row removed.
Next: coordinator trial link for Matching.
