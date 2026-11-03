# game/act_rope (wave 27)
Baseline (progress.py): 6426 / 11557 functions, 1284740 bytes of code (28.92 %).
After: 6430 / 11557, 1285164 bytes (28.93 %). Unit 2/8 -> 6/8 functions.

Matched: draw, init, constructor (fn_801B7DE0 -> __ct__13Rope_804A3A68Fv), plus the earlier dtor, EnemyBase dtor, __sinit.
Resistant:
- vfunc_0x94 98.8 % (regalloc-only + sdata2 literal order 0.4f / 0.9090909f).
- vfunc_0x90 97.8 % (found/i swapped in r27/r29; see FIXME).
SHARED-EDITS: none outside the unit (gen_game.tsv rows for act_rope deleted; include/game/act/act_rope.h constructor now out of line; symbols.txt rename fn_801B7DE0).
Next: the int-register order in vfunc_0x90's search loop; the literal order in vfunc_0x94.
