# game/act_fire_bar.cpp

Baseline (progress.py): 6007 / 11557 functions, 1208940 / 4442032 bytes.
After: 6009 / 11557, 1209176 bytes.

Matched: __dt__, __ct__ (FireBarManager_804A3F90), __sinit (generator rows removed
from gen_game.tsv).
Resistant (both scheduling+regalloc, same instructions):
- update 93.61%: `mr r31, r3` scheduled before the first mFlagBit/mSpeed loads;
  second getSpeed() inline gets bit/speed in r30/r29 (target r29/r30); sin/cos
  table base and angle in r3/r4 swapped.
- init 96.95%: saved registers numbered differently (target r27 this-copy,
  r29 counter, r28 spawn flag); same r3/r4 swap for the sin/cos table.
SHARED-EDITS: none (new include/game/act/act_fire_bar.h is this unit's own).
Next: try other shapes for the sin/cos pair (shared by both).
