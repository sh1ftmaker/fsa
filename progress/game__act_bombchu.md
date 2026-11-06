# game/act_bombchu (2026-11-06, Claude Opus 5.5)

Baseline: total 8960 / 11557 functions. After: 8965 / 11557 (+5); unit 3/9 -> 8/9.

Matched (100 %): init (0x803264C0), vfunc_0x54 (0x80326208), draw (0x80325754),
vfunc_0x4C (0x80325548), fn_8032506C (0x8032506C).
Resistant: update (0x80325A30) 99.88 %, try.py `operands`: registers and stack
slots only (state 2 hit mask / loop index r29<->r30; inlined vfunc_0x48 result
vs frames table slots 0x1C / 0x2C).
Proven: faceMoveDirection, a dead-stripped plain helper inlined into update,
opens the .sdata2 pool (0.0f, pi, 3pi/2, pi/2); made `inline` the pool breaks.
Unit not Matching: vtable 0x58 vs 0x5C and .rodata 0x24 vs 0x28 (trailing
4-byte gaps), extab carries the 9 weak JASMemPool dtors (needs a trial link).
SHARED-EDITS: none (act_bombchu.h is this unit's header; symbols.txt renames
of the six fn_ placeholders only).
