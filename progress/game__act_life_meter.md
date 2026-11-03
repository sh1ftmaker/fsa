# game/act_life_meter
Baseline: total 5932 / 11557 functions, 1188620 / 4442032 bytes (26.76 %).
Matched: dtor, init, __sinit (3/5); .data/.rodata/.sdata2 100 %.
Resistant:
- draw 98 % (different): range check is `cmpwi 5; bge` (switch 0..4), original `cmpwi 4; bgt`.
- update 99.32 % (missing/extra): `mr. r30, r3` vs original `cmplwi r3,0; mr r30,r3` after fn_801F666C.
SHARED-EDITS: include/game/Map.h (cls_804AA858::fn_802F745C), symbols.txt rename fn_802F745C, gen_game.tsv rows removed.
