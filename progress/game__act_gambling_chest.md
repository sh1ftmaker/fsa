# game/act_gambling_chest.cpp

Baseline: total: 6287 / 11557 functions, 1258900 / 4442032 bytes of code (28.34 %)
After: total: 6291 / 11557, 1260540 bytes (28.38 %).

Matched (8/8): dtor, fn_80310E48, fn_80310E98, fn_80310F50, draw, update, init, __sinit.
Resistant: none. Data: .rodata split in symbols.txt (two s32 tables, two strings).
SHARED-EDITS:
- config/G4SE01/gen_game.tsv: act_gambling_shop rows removed (generator-owned header edited).
- include/game/act/act_gambling_shop.h: GamblingShop_804AAA70::fn_8030FDC0(s32) (0x8030FDC0:
  stw r4,0x25c; 0x234 = 0x3C; state 6), called by the chest's update after an actor-id GMBL check.
- include/game/Player.h: fn_8022F340(s32) -> s32, fn_802362C4(s32, s32), owner tu_8022AD64.
- symbols.txt: fn_8030FDC0 and fn_80310E98 renamed to member names; lbl_80471438 split.
Next: the shop unit can now call fn_80310E98 / fn_80310E48 / fn_80310F50 on the chest.
