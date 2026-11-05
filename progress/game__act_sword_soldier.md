
## Small-function pass (2026-11-05)
Baseline: total 7726 / 11557; unit 10/46. After: total 7750; unit 34/46 (24 small functions
written, all <= 0x40 bytes, address order kept; symbols renamed to what the source emits).
Not 100 % (data only, instructions identical, .sdata2 not laid out yet): fn_80154378 99.74,
fn_80154528 99.58 (0.5f), fn_8015465C 99.67 (10430.378f), fn_80154744 99.72.
Open: 12 functions > 128 bytes; fn_8015394C (vtable slot 0x48) needs lbl_804BF850 (s32[4], bss),
lbl_8053AB44/48 (sbss), lbl_80539F68 (sdata, 200) and 0x2a4 (s32) in the class.
Review fix: fn_80154490/8015449C/801544AC/801544B4/801544C0 act on the SLR4 actor fn_801542AC finds, so
they are ChiefSoldier_804A0B90 members (act_chief_soldier.h gains field_0x28c/0x290); fn_801544D0,
fn_801545C0, fn_801545E0, fn_80154614 touch only ActorBase fields (other units call them on other
classes) and are ActorBase members.
SHARED-EDITS: include/game/ActorBase.h (4 member declarations), include/game/act/act_chief_soldier.h.
