# AB report: game/act_frostare_tail (Opus 5.5)

Totals (honest): 8776 -> 8785 functions (+9). Structural 9227 -> 9238. No function dropped anywhere (per-address compare against fsa-decomp's report_honest.json); full `ninja` ends `fsa_game_code.dol: OK`; gate --all-units ok; gate --base faa79b43 ok; gen_game --check ok; order_check: act_frostare_tail in order (11 other units out of order, all pre-existing).

## 100 % (9 of 12)
__ct__12cls_804A4F90Fv, __dt__12cls_804A4F90Fv (fn_801DC990), init (fn_801DC89C), draw (fn_801DC2D4), vfunc_0x60 (fn_801DC364), vfunc_0x74 (fn_801DC6B4), vfunc_0x90 (fn_801DC7D0), vfunc_0x9C (fn_801DC790), __sinit (already). Plus tu_801E5FC8 draw__12cls_804A4C58Fv 98.39 -> 100 % (side effect of the u8 return chain).

## Structurally done (FIXME in source)
- fn_801DC264__12cls_804A4F90Fv: 98.39 %, regalloc-only (result and field_0xdc swap r4/r5).
- vfunc_0x5C (fn_801DC3C4, 0x2F0 bytes): 97.14 %, scheduling+regalloc (layer flag / rupee serial in r27/r28 swapped; the two velocity temporaries' stack slots swapped; declaring named locals in reverse order fixed the slots (97.18 %) but I left it out as steering).
- vfunc_0x88 (fn_801DC758): 97.86 %, regalloc-only (f0/f1 around fabs/fadds).

## Proven
- cls_804A4F90 ('BOIT', Frostare tail) derives from EnemyBase_804A4808 (vtable 0x804A4F90 holds EnemyBase slots; dtor inlines EnemyBase's); moved from tu_801F9484.h into new include/game/act/act_frostare_tail.h; u8 mHitCount at 0x2A8 (ctor stb 0; vfunc_0x74 cmplwi 10), size 0x2AC.
- Vtable slots renamed in symbols.txt: init, draw, vfunc_0x5C/0x60/0x74/0x88/0x90/0x9C, __dt__.
- Animation tables lbl_80469AB8 / lbl_80469ACC and pointer table lbl_8053A150 written as source data.
- fn_801DC264 returns u8, as does Singleton_801DAC6C::fn_801DAD2C (unchanged at 91.30 %), and tu_801E5FC8's draw keeps it in a u8 (now 100 %). Singleton field_0x84 is u16 (lhz/sth; bit 0 set on the tenth hit, cleared in state 1).
- vfunc_0x90's switch has an empty `case 0` (MWCC's cmpwi 1 / cmpwi 3 dispatch).

## SHARED-EDITS
include/game/act/tu_801F9484.h (class removed, includes act_frostare_tail.h), include/game/act/tu_801DAC6C.h (field_0x84 u16; fn_801DAD2C u8), src/game/tu_801DAC6C.cpp (fn_801DAD2C u8), src/game/tu_801E5FC8.cpp (u8 local), config/G4SE01/gen_game.tsv (act_frostare_tail row removed: file is hand-written).

## Left
The three functions above; unit data sections (.data 99.38 %: vtable trailing zero word; .sdata2 96.30 %: 8.0f object of size 8) not investigated; the unit is not flipped to Matching.
