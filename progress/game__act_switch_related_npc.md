# game/act_switch_related_npc.cpp

Baseline: total: 5932 / 11557 functions, 1188620 / 4442032 bytes of code (26.76 %)
After: 5933 / 11557, 1188624 bytes (draw matched; update/init resistant).

Matched: dtor, draw (+ generated __sinit). Rodata 100 % (4 u16 tables as function-local statics lbl_8046D470/7C/98/B4).
Resistant:
- update (89.66 %, different): same instruction set; regalloc (this r28 vs r29, extra r26) and table-pointer/lhzx scheduling. Tried: no layer local, ternary step, s32 slot with (u16) casts.
- init (97.86 %, scheduling+regalloc): mPos x/y copy loads y first in the original.
SHARED-EDITS: none beyond this unit (gen_game.tsv rows for the unit deleted; header + symbols.txt renamed cls_804A8C28 -> SwitchRelatedNpc_804A8C28, evidence actors.tsv SWNP).
Next: try restructuring update's case 2 so layer/cell share a register.
