# game/act_popo.cpp checkpoint

Baseline: total: 5920 / 11557 functions, 1185564 / 4442032 bytes of code (26.69 %)
After: 5921 / 11557, 1185628 bytes.

Matched: __dt, draw, __sinit (generated lines unchanged; gen_game.tsv rows for act_popo deleted).
Resistant (both regalloc/scheduling, no STOP printed, ~10 min spent):
- update 98.72% regalloc-only: RNG multiplier 0x19660D in r5 / value in r0 in target; ours r0 / r3.
- init 94.66% scheduling+regalloc: target begins `li r6,0`, RNG constant built after 0x10000 constant; ours builds RNG constant first (zero ends in r7).
Tried: file-local rand helper (no change), nested if vs switch(field_0x230){case 0:} (switch matches the `beq; b` shape), timers as s32[3] + loop (fixes the `addi r4,r31,8` base quirk).
SHARED-EDITS: none (include/game/act/act_popo.h is this unit's; gen_game.tsv rows deleted).
