# game/act_view_mode
Baseline: total 5708 / 11557 functions, 1150032 bytes (25.89 %). After: 5710 / 11557, 1150400.
Matched: __dt, draw, update (declared `s32 bit` once at top; per-block locals gave r29/r30 swapped), __sinit.
Resistant: init (99.33 %, scheduling): epilogue `lwz r0,0x14(r1)` / `lwz r31,0xc(r1)` order swapped. Tried statement order, `& 0xFFFF` vs (u16).
SHARED-EDITS: Player.h: `s32 fn_802373EC(s32 player)` (defined in tu_8022AD64; compared with 1 and 2 by act_warp/act_view_mode).
Next: init epilogue; extab still 45 %.
