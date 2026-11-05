
## Small-function pass (2026-11-05)
Baseline: total 7726 / 11557; unit 10/46. After: total 7750; unit 34/46 (24 small functions
written, all <= 0x40 bytes, address order kept; symbols renamed to what the source emits).
Not 100 % (data only, instructions identical, .sdata2 not laid out yet): fn_80154378 99.74,
fn_80154528 99.58 (0.5f), fn_8015465C 99.67 (10430.378f), fn_80154744 99.72.
Open: 12 functions > 128 bytes; fn_8015394C (vtable slot 0x48) needs lbl_804BF850 (s32[4], bss),
lbl_8053AB44/48 (sbss), lbl_80539F68 (sdata, 200) and 0x2a4 (s32) in the class, which clashes with
fn_80154490's 0x2a5 byte: the helpers act on an SLR4 actor found by fn_801542AC, not on this class.
SHARED-EDITS: none.
