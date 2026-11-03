# game/tu_8021CDE4.cpp

Baseline: total: 5751 / 11557 functions, 1155176 / 4442032 bytes of code (26.01 %)
Now:      total: 5754 / 11557 functions, 1155788 / 4442032 bytes of code (26.02 %)

## Matched (4/4, code and data 100 %; extab/extabindex are the weak-pool entries)
- cls_804A46F4::fn_8021CDE4(int) state change; fn_8021CFF8() / fn_8021D020() predicates.
- `__sinit` from the GameAudio.h include (row removed from gen_game.tsv).

## Resistant
none.

## SHARED-EDITS
- NEW include/game/act/act_shadow_link_2.h: cls_804A46F4 (vtable 0x804A46F4, dtor_801C5854), partial layout up to 0x5A0; lbl_8046A748 table extern.
- NEW include/game/cls_80214244.h (moved out of src/game/tu_80214244.cpp, which now includes it).
- include/game/RailBase.h: extern "C" fn_800FE294(TVec3f* out, cls_800FE3EC*, u8).
- include/game/EnemyBase.h: extern "C" fn_801CD0D0 / fn_801CD150 (const TVec3f* in, TVec3f* out).
- symbols.txt placeholder renames: fn_8021CDE4/CFF8/D020 and fn_801C3B7C to `__12cls_804A46F4F..` members.

## Next
act_shadow_link_2.cpp defines fn_801C3B7C (member declared in the header) and the rest of the class.
