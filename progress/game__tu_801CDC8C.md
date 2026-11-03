# game/tu_801CDC8C checkpoint
Baseline: total: 6007 / 11557 functions, 1208940 / 4442032 bytes (27.22 %). Unit 25/57 functions.
G4 revisit: both 10430.378f conversions (fn_801D0280 73.67->74.08, fn_801D02E0 44.77->45.23) now use the rules-card shape. Neither flips to 100 %: remaining diff is scheduling (target loads mPos/pos before the atanTable_ lis/addi) plus .sdata2 literal order.
Tried (fn_801D02E0): named dx/dy locals, no change, reverted.
Resistant: all 12 FIXMEs in the file unchanged; no further tries (budget focus on angle conversions).
SHARED-EDITS: none.
