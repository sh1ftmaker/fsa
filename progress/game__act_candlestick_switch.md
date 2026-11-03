# game/act_candlestick_switch.cpp (LWSW, CandlestickSwitch_804A8B78)

Baseline: 5949 / 11557 functions, 1193564 bytes. After: 5952 / 11557, 1194644 bytes.

Matched: update, __dt__, init, __sinit (4/5).

Resistant: none (draw fixed by reading the scale into a local first).
scale (0x254) before the 0.0f for pos.z and holds (8, y, x, scale) in f3, f2,
f0, f1; ours loads 0.0f first, so the float registers shift. Tried
TVec3f(x, y, z), set(), field-by-field assignment, a local TColor.

SHARED-EDITS: include/game/Player.h (fn_8023AB94 declaration, signature from
fn_8023AB94's body and 5 callers); config/G4SE01/gen_game.tsv (row removed);
symbols.txt placeholders renamed (4 functions, vtable 0x58 -> 0x54).

Notes: .sdata2 93.33 % is the 4-byte trailing alignment pad after the 7th
literal (our object is 0x1C, original 0x20); extab/extabindex follow draw.
