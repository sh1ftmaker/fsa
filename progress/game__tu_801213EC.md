
## Small-function pass (Claude Opus 5.5)
Baseline total 7726 / 11557; unit 1/59. After: total 7750, unit 25/59 (try.py). Rows of tu_801213EC removed from gen_game.tsv (unit now hand-written).

Matched: vfunc_0x54/58/5C/60/64/68/6C/70/74/7C/84/88/8C/90/94/C0/C4, fn_80121DB0, fn_801227D4, fn_80122804, fn_80122834, fn_8012285C (const), plus reloc-name-only (literals pending the rest of .sdata2): fn_80121DC8, E40, F34, FA8, 8012202C, 801220C4, vfunc_0xB0/B8/AC/A8.

Resistant:
- fn_80121D68 92.8 missing/extra: original passes field_0x35c (lwz) to fn_800FE3EC's u8 arg with no clrlwi.
- fn_80121ABC 99.1 regalloc: array pointer r29 / counter r30 swapped.
- vfunc_0xBC 91.8, fn_80122898 96.8: TBox2f copy then adds of mPos; target stores f.x before the y load. addPos(x,y) gives 86.6 / worse.
- vfunc_0xB4 92.9 (missing/extra, not examined), vfunc_0x78 (fixed in review: const on base and both overrides, 100 %).

Unwritten: fn_80121438, 801217EC, 80121BB8, 80122148, 80122548, 80122730, 80122F6C, 8012309C, 80123394, vfunc_0x98 (80123AB4), vfunc_0x80 (80123B94), fn_80123E9C (vfunc_0x4C), fn_80123F9C, update, fn_80124448, fn_80124600, dtor, ctor, __sinit.

SHARED-EDITS: include/game/tu_801213EC.h (fields 0x268-0x3C4 declared, slots 0x54/0x58 bool, 0x5C s32, 0x98-0xC4 added, helper decls); act_escort_woman.h (0x54/0x58 bool, 0x5C s32, her field_0x340/0x35c removed: now base fields); act_zora_escort.h (field_0x340[] -> field_0x3c4[0x3D4-0x3C4]); GameGlobals.h (extern u8 lbl_8053AAA0, owner tu_8011CEC8); config gen_game.tsv row removed; symbols.txt renames to mangled member names.
