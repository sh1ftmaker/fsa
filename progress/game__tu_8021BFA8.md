# game/tu_8021BFA8.cpp

Baseline (progress.py first line at start): total: 5667 / 11557 functions, 1147428 / 4442032 bytes of code (25.83 %)
After: total: 5671 / 11557 functions, 1147660 / 4442032 bytes of code (25.84 %)

Status: NonMatching. 5/5 functions at 100 %; .sdata2 82.93 %, extab/extabindex 18.18 % (weak-dedup extab tooling, as tu_8021BCBC).

## Matched
- cls_8021BFA8::fn_8021BFA8 / fn_8021C00C (stepper over byte tables; 3,0,0 reset), cls_8021C024::fn_8021C024 / fn_8021C078 (12,0,0 reset). Both return field_0x0 (reloaded after the store).
- symbols.txt: four functions renamed to what the source emits. gen_game.tsv row deleted.

## Resistant
- .sdata2 (82.93%): ours is 0x11 bytes with 8-byte alignment; the target's 0x18 is 7 zero bytes of alignment padding up to the next unit's 8-aligned .sdata2 at 0x8053FCF8 (linked DOL identical). Tables and order match; not a source difference.

## SHARED-EDITS
none

## Next
extab once the tooling lands.
