# game/tu_801ED3DC

Baseline: total: 6229 / 11557 functions, 1252968 / 4442032 bytes of code (28.21 %). Now 6238 / 11557.

## Matched (10/12 with __sinit)
fn_801ED458, 5C8, 798, 7B0, 7F0, 80C, 8F0, 968, 974, __sinit (cls_801ED3DC stopwatch, cls_801ED828 countdown).

## Resistant
- fn_801ED3DC (84 %, scheduling: loads of seconds/centis not hoisted; the 98 % helper-argument-order variant was removed as a trick): target r10=lbl_8053AEF8, r8=1, r7=seconds, r9=flag byte; ours r8, r7, r10, r6. Tried hoisting via member helper (62 %), static helper with params (98 %, arg order swapped for load order), all-in-one helper (same).
- fn_801ED828 (79.8 %): target converts efbHeight first through r3 with the magic in r0, lr saved after the first lhz; ours keeps the mode in r8, magic in r3, converts fbWidth first. Locals/order/return vs break made no difference; int `/ 2` is worse.

## SHARED-EDITS
- include/game/GameGlobals.h: cls_804B1920::fn_8043CB24 (r3 lbl_8053AED0; args from asm: r4 value, r5 byte at 0x244, r6 byte (<=5) at 0x23A, r7 digits byte at 0x226, r8 bool, f1-f3).
- symbols.txt: fn_8043CB24 -> fn_8043CB24__12cls_804B1920FlUcUcUcbfff (callers act_digging_game, tu_8010089C follow).
- include/game/tu_804455E0.h (new, owning unit of the 0x48-byte cls_804B1BB0 stored in lbl_8053AEF8 by 0x804461D0).
- gen_game.tsv: removed the tu_801ED3DC row.
