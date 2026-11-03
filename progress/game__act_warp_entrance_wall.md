# game/act_warp_entrance_wall
Baseline: total: 5949 / 11557 functions, 1193564 / 4442032 bytes of code (26.87 %)
Matched: dtor, __sinit (2/5 before and after; text 99.3 %).
Resistant (none exact):
- fn_803798D0 (overlap probe) 98.08 % missing/extra: target keeps the slot's actor in r3 and the helper bool in r0; ours r4/r3 plus one extra li.
- update 99.84 % operands: stack slots (target pos temp 0x8, box 0x14; ours box 0x8, pos 0x18).
- init 99.18 % regalloc-only: shape 0 case f0/f1 swapped.
SHARED-EDITS: gen_game.tsv rows for this unit removed; symbols.txt placeholder rename fn_803798D0 -> fn_803798D0__25WarpEntranceWall_804AE948Fv.
