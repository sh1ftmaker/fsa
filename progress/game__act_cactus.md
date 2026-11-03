# game/act_cactus.cpp

Baseline (progress.py): total 5783 / 11557 functions, 1160172 bytes.
After: 5786 / 11557, 1160932 bytes.

Matched: all 5 functions (dtor, draw, update, init, __sinit) at 100%, .text/.rodata/.data/.bss/.ctors 100%.
Residual report quirks: extab/extabindex 47.06% and .sdata2 94.74% (contents identical in the disassembly; same quirk on other matched units). Matching flag left for the coordinator.

Resistant: none.

SHARED-EDITS: include/game/Player.h (fn_802379E4, fn_80239638, fn_8023BB50 declarations, owner tu_8022AD64);
config/G4SE01/gen_game.tsv (removed both act_cactus rows).

Next: flip Matching after the trial link. Fields 0x234/0x238 are cleared by init and never read.
