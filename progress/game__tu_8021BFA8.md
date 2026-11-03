# game/tu_8021BFA8.cpp

Baseline (progress.py first line at start): total: 5667 / 11557 functions, 1147428 / 4442032 bytes of code (25.83 %)
After: total: 5671 / 11557 functions, 1147660 / 4442032 bytes of code (25.84 %)

Status: NonMatching. 5/5 functions at 100 %; .sdata2 82.93 %, extab/extabindex 18.18 % (weak-dedup extab tooling, as tu_8021BCBC).

## Matched
- cls_8021BFA8::fn_8021BFA8 / fn_8021C00C (stepper over byte tables; 3,0,0 reset), cls_8021C024::fn_8021C024 / fn_8021C078 (12,0,0 reset). Both return field_0x0 (reloaded after the store).
- symbols.txt: four functions renamed to what the source emits. gen_game.tsv row deleted.

## Resistant
- .sdata2: original section is 24 bytes (E0:4, E4:8, EC:12); ours is 17 (4 + 5 padded to 4 + 5). Order matches. Local function statics were worse (names differ, 18 %). Cause of the 7 missing tail bytes unknown (maybe the original tables are longer than their indexed range).

## SHARED-EDITS
none

## Next
Find the sdata2 tail layout; extab once the tooling lands.
