# game/tu_801E667C

Baseline (progress.py): 5920 / 11557 functions, 1185564 bytes (26.69 %). After: 5924 / 11557, 1186756 bytes.

Matched 5/5 (all 100 %): cls_801E6AB8::fn_801E667C, fn_801E67C4, fn_801E6A84, ctor, generated __sinit.
Resistant: none in code. Data: .rodata 96.77 %, .sdata 85.71 %, .sdata2 85.71 % only because the target ranges include
4 bytes of link padding after the last object (our object ends at 0x3C / 0xC); extab is weak-duplicate (SpriteAnim dtor).

SHARED-EDITS: GameGlobals.h (GameRoot::fn_80138A30, Unk_8045A080::fn_80458F04/fn_80458F9C); symbols.txt renames of
fn_80138A30, fn_80458F04, fn_80458F9C to mangled members; new include/game/act/act_vaati_boss.h; gen_game.tsv row removed.
Next: set Matching after the trial link; lbl_8053A160 (s32 = -1) is defined here, read by act_horse_race_man / tu_80441318.
