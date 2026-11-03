# game/tu_8012FF9C

Baseline: total: 5866 / 11557 functions, 1174344 / 4442032 bytes of code (26.44 %)
After:    total: 5872 / 11557 functions, 1175228 / 4442032 bytes of code (26.46 %)

Matched 6/6 (getter fn_8012FF9C, ~cls_8012FFF4, fn_80130030, fn_801300B4, fn_801300D8, __sinit). Code 884/884, .bss/.sbss 100 %.
extab/extabindex 47 % (weak-duplicate entries, same as siblings); Matching flag left NonMatching.
Not generator-owned (no gen_game.tsv rows).

## Notes
- cls_8012FFF4 is 8 bytes (.sbss; symbol's 0xC was section padding). fn_801300B4 indexes from &mSerial by mCount (as the asm does).
- Actor id compared in fn_80130030 is 'OGST' (0x4F475354), not OPGL.

## SHARED-EDITS
- GameGlobals.h: removed the cls_8012FFF4 / fn_8012FF9C / fn_801300B4 extern "C" block; now in new include/game/tu_8012FF9C.h.
- src/game/tu_8012FBCC.cpp: call becomes fn_8012FF9C()->fn_801300B4(mSerial) (try.py 4/4 still).
- symbols.txt: renames to mangled names; bss/sbss split into @406/@440-448/lbl chunks, init$407, lbl_8053AAC4$405.
