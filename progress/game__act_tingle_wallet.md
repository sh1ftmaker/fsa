# game/act_tingle_wallet.cpp

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %)
After: total: 5841 / 11557 functions, 1169568 / 4442032 bytes of code (26.33 %)

Status: 4/5 functions (was 2/5). .text 97.72 %, data sections at 100 %. Not Matching. gate: ok.

## Matched
dtor, update (the three counters are s32 mTimers[3] in a loop: the original's odd `addi r4, r31, 8` addressing comes out of the unrolled loop), init.

## Resistant
draw 92.04 % (scheduling+regalloc, reloc-name): the target loads `lfs f1, 1.0f` and stores params.field_0x10 before `lis lbl_80469120@ha` / `addi r0, r1, 0x10`. The string shows as @NNN vs lbl_80469120 (data fix).

## SHARED-EDITS
- config/G4SE01/gen_game.tsv: removed both act_tingle_wallet rows (generated lines unchanged).
- include/game/act/act_tingle_wallet.h (generator-owned): fields filled in.

## Next
Try other orderings of the draw prologue; fields 0x230.. need names from a real use (0x230 state, 0x94 u16 set by something else triggers fall).
