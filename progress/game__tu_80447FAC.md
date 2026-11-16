# game/tu_80447FAC
Measured on `matching` d1808afe4 (`progress.py | head -1`): 9864 -> 9866 / 11557 functions.
Matched: ctor, dtor (data defined). Resistant: fn_80447FAC, fn_80448334 (regalloc-only: loop counter / tag-table offset swap r27<->r28).
.sdata2 66.67%: target 8 bytes (0.0f + 4 bytes pad/dead literal), no source can place it; .data 96.73%: vtable symbol size 0x10 vs our 0xC.
