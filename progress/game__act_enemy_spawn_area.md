# game/act_enemy_spawn_area.cpp

Baseline: total 5866 / 11557 functions, 1174344 bytes.
Now: 5867, 1174348. Unit 3/5 (dtor, draw, sinit match).

Resistant:
- update 99.56% scheduling+regalloc: random-offset sequences have r4/r5 swapped (global ptr vs LCG constant); counters i/gone in second loop are r31/r30 here, r30/r31 in the original.
- init 92.24%: original loads field_0x90 once for the index shift (r5) before the 0x234 store and keeps zero in r4; ours uses r0 and schedules the shift later. 15 explicit mSerials[n] = -1 stores (a loop is not unrolled by the compiler).

SHARED-EDITS: gen_game.tsv rows for act_enemy_spawn_area removed (dtor, sinit); include/game/act/act_enemy_spawn_area.h gains the fields.
