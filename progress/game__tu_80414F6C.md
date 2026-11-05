# game/tu_80414F6C (cls_804B1378, battle stage select "bat_stage_select.blo") — 2026-11-05, Opus

Baseline total 8338 -> 8346 (+8). Unit 8/10 functions at 100 %; .sdata2, .rodata, .bss 100 %.

Matched: fn_80414F6C, fn_80414F78 (state 2), fn_80415028 (state 1, input + roulette),
fn_80416038 (state 0, slide-in), fn_80416BD0 (reset), fn_804170B0 (PTMF dispatch),
__dt__12cls_804B1378Fv, __sinit_tu_80414F6C_cpp (PTMF template @705-@707 + copy lbl_804B1354, 9 JAS pools).

Resistant:
- __ct__12cls_804B1378Fv 99.96 % regalloc-only: `font` r27 vs r29.
- fn_804170EC 99.93 % operands: frame 0x60 vs 0x50 (int->float temp slots).
- .data 87.5 %: our weak __vt__12cls_804AF86C copy is emitted before __vt__12cls_804B1378, the original has ours first (lbl_804B1384 = the weak copy).
- extab/extabindex 0 %: weak-dedup evidence not produced (link_check --unit not run).

BLOCKER: gate.py --base matching trips check_byte_arrays on lbl_8048CE78 ([10][10] u64 pane tags, 99
literals, a real table read in nested loops). Needs an owner-approved `array` line in blobs.txt.

SHARED-EDITS: cls_804B1378.h (full layout); cls_804B1600.h (fn_8042EBEC / fn_8042EC7C return BOOL:
every caller tests with cmpwi; tu_8042E964 bodies changed to BOOL, its scores unchanged; extern
lbl_8053AEC4 added, tu_8042E964's definition made non-static: other units read it via sda21);
GameGlobals.h (fn_803B9120 decl, tu_803B8C74 block); tu_80379F84.h (SaveRow field_0x81 carved).
