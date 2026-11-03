# game/tu_8012DAC0.cpp
Baseline: total: 5751 / 11557 functions, 1155176 / 4442032 bytes of code (26.01 %)
After: 5752 / 11557, 1155276 bytes.
Matched: dtor, init, __sinit (generated).
Resistant: update (99.12 %, missing/extra): the original switch dispatch has an extra unreachable `b` to the epilogue at +0x34. Tried default: break, return instead of break, local for field_0x244 (the local fixed the r30 use).
SHARED-EDITS: none.
Next: name the fields once the class's purpose is known (writes tiles through BgRewrite::fn_802A381C).
