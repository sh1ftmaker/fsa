
## 2026-11-05 Sonnet 5.5 pass
Baseline total 7797 -> 7824; unit 1/73 -> 28/73 (honest report). 27 further functions are 99.0-99.9 % with only
reloc-name (sdata2 literals not yet in the original order/split: resolves when neighbouring functions are written).
Resistant: fn_8027E7E0 98.85 % (scheduling+regalloc): original loads field_0x3b4 into r30 before the field_0x11fc
load for the fn_8023DE58 call; ours loads it after (tried flipping the != operands, no change).
Not written (callees unknown / need new class for the 0xF4/0x24 "stop emitter" pointer at 0x1350/0x1354/0x1358/0x132c/0x135c:
fn_80280DD8, fn_80281A18, fn_802811C0), the rest need unnamed callees (fn_8027EB04 and larger bodies).
SHARED-EDITS: include/game/Player.h (fields 0x318..0x13BC split, member declarations), config/G4SE01/symbols.txt (renames to
Player_804A6F38 members, incl. fn_80248604, fn_8024D134, fn_8024E3D8 of tu_8023FBF0), config/G4SE01/gen_game.tsv (tu_8027E5BC row removed).
