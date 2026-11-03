# game/act_bandit_herding_npc

Baseline: total: 5820 / 11557 functions, 1165664 / 4442032 bytes (26.24 %); unit 2/5.
Now: 5822 functions; unit 4/5 (dtor, draw, init, __sinit match).

Resistant: update (94.33 %, missing/extra). FIXME in source: the original
materialises (field_0x238 >= field_0x234) as a bool (srawi/srwi/subfc/adde, cmplwi 1)
and keeps the 1 stored to fn_80204638()->field_0x0 in r0 (mine r30); box test loads
x into f2, y into f1 (mine f1, f2).

SHARED-EDITS: include/game/GameGlobals.h (cls_80204638 + fn_80204638 getter);
config/G4SE01/gen_game.tsv (both act_bandit_herding_npc rows removed).
