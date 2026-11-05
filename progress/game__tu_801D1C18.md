# game/tu_801D1C18 checkpoint
Leverage wave 1 (2026-11-05). Baseline: total 8397 / 11557. After: 8398 (+1: fn_801D2608 99.48 -> 100).
- fn_801D2608: min/max ternaries written `mMinCel >= p[0] ? p[0] : mMinCel` / `mMaxCel >= p[0] ? mMaxCel : p[0]` (the relation direction decides the cmpw/blt shape).
- calcCel: reading the table once (`const s16* p = mTables[mAnim]`, post-loop `mTables[mAnim][0]`, no `table` local) fixed the register assignment: fn_801D1C18 98.33 -> 99.85, fn_801D1D60 98.74 -> 99.89, fn_801D1F14 98.54 -> 99.87, fn_801D22DC 98.65 -> 99.88. Left: the store order of the xoris word vs the 0x43300000 word for `start` (scheduling only; sum/dur/end locals and cast variants did not change it).
- Not touched further: fn_801D208C 94.4, fn_801D21F0 86.2 (prologue scheduling).
SHARED-EDITS: none.
