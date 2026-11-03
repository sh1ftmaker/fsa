# game/tu_8012FBCC

Baseline: 5708 / 11557 functions, 1150032 bytes (25.89 %). After: 5710 / 11557, 1150364 bytes.

Matched (4/4): ~cls_8049EA00, cls_8049EA00::fn_8012FC2C(const s32*) (the 3 flag
tests are `(mFlags >> n) & 1`, which gives extrwi), init, __sinit.
Resistant: none. Matching flag left NonMatching: extab/extabindex show 47 %
in try.py (same as sibling tu_8012F8AC).

SHARED-EDITS: GameGlobals.h: extern "C" fn_8012FF9C() -> cls_8012FFF4*,
fn_801300B4(cls_8012FFF4*, s32) (owner tu_8012FF9C, unwritten). gen_game.tsv: the
2 tu_8012FBCC rows removed. symbols.txt: fn_8012FC2C renamed to the mangled member.
