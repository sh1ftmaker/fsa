# game/act_pot_2_generator
Baseline: total 5708 / 11557 functions, 1150032 bytes.
Matched: dtor, draw, update, __sinit (4/5).
Resistant: init 98.05% regalloc-only (this r30/id r31/code r29 in the original; ours swaps this/id, code in r27/r25). Tried s32->u32 id, moving the id declaration (worse).
SHARED-EDITS: none.
Next: try an inlined per-layer helper for init's two loops.
