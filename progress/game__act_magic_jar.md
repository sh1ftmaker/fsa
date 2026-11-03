# game/act_magic_jar

Baseline (progress.py): 5949 / 11557 functions, 1193564 bytes of code (26.87 %).
After: 5952 / 11557, 1193848 bytes.

Matched: dtor, vfunc_0x40, draw, init, __sinit (generated; gen_game.tsv rows removed).
Resistant: update 99.63% (operands). Same instructions; stack slots differ: original probe vector
at 0x1c and TBox2f at 0x28, ours box 0x1c and probe 0x2c (tried: hoisted declaration, braces around the box).

SHARED-EDITS: include/game/Player.h (fn_8023B8E4, fn_80236B50, fn_802375B0, fn_80230EB8, fn_80237524 in
the extern "C" block); config/G4SE01/gen_game.tsv (act_magic_jar rows removed).
