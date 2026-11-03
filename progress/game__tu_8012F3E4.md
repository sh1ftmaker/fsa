# game/tu_8012F3E4

Baseline: total: 6046 / 11557 functions, 1220300 / 4442032 bytes of code (27.47 %)

Matched: fn_8012F494 (clear), fn_8012F530, fn_8012F554 (getter), dtor, __sinit_tu_8012F3E4_cpp (5/6).
Resistant: cls_8012F64C::fn_8012F3E4 (90.23 %, scheduling+regalloc). Original keeps the count in r31 and
`this` in r30; ours has this in r31, count in r30 (so the saves/lwz order differ). Tried: count local with
early return, i declared first, while loop, if-nested, u32 index.
Data: bss re-split (record @447, static lbl_804BDDFC$446 0x104, 9 pool records @468-476, 9 pools 0x10), sbss init$448.
SHARED-EDITS: tu_8012F3E4.h (struct now defined, members), tu_8012F8AC.cpp and act_ice_floor_check_npc.cpp call sites.
