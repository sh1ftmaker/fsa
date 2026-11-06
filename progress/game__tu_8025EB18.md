
## Session 2026-11-05 (Sonnet 5.5)
Baseline 7877 total; unit 10/59 -> 33/59 (honest total 7899). All tiny/small Player members done.
SHARED-EDITS (include/game/Player.h): field_0x300 -> TVec3f; field_0xd6c s32; 0xD78 TVec3f field_0xd78, 0xD84 s32, 0xD88 s32; 0x121C/0x121D u8s; new member decls fn_802632A4/80262234/80262B6C/80262B8C/8026173C/80261468/80258BBC/80259660/80283D2C/8025F484(const); fn_8025F684 stays bool (a u8 return dropped fn_8023EFEC in tu_8022AD64 from 100% to 89.7%).
symbols.txt: renamed fn_80258BBC, fn_80259660, fn_80283D2C (tu_80256C74/tu_8027E5BC) and this unit's thunks to mangled Player_804A6F38 names.
Notes: file-static isField0xd64One() inline reproduces the inlined subfic/cntlzw/extrwi test in F484/F500/F55C/F590; F684 is its out-of-line copy returning bool. Making F684 an inline member in Player.h was measured: F684 is not emitted, fn_8023EFEC inlines it (81%).
Next: fn_8025EE48 (288), fn_8025F2CC, fn_8025EFFC and larger ones.

## Session 2026-11-05 round 6 (Opus 5.5, agent I)
Baseline 8491 total; unit 33/59 -> 40/59 (honest total 8498, +7, no drops by address).
100 %: fn_8025FF70, fn_802600A0, fn_80261480, fn_80262188, fn_802635D8, fn_802636D4, fn_80263BA0.
Structurally done: fn_8025EB18 99.71 (regalloc), fn_8025FDD0 99.10 (regalloc).
Structurally close: fn_8025F698 95.94, fn_8025EE48 96.32, fn_8025EFFC 95.94, fn_8025F2CC 96.81, fn_8025FCC8 87.39
(FIXMEs in source: the field_0xd64 == 1 byte is materialised (extrwi.) in the original; 0x11 not folded into 0x12-0x16).
.rodata: all tables written (99.89 %, the last 4 bytes are the gap before tu_80264998's data). .sdata2 40 %.
SHARED-EDITS (include/game/Player.h): declarations fn_8025EB18, fn_8025FCC8, fn_8026022C, fn_802604CC,
fn_80260C08, fn_80261480 (r3 is the Player in each). symbols.txt: those six renamed to __15Player_804A6F38Fv.
Next: fn_802604CC / fn_8026152C (literal order 0.5, 32), then fn_80261750 (0x19 update).

## Session 2026-11-05 round 10 (Opus 5.5, agent R)
Baseline 8652 total; unit 40/59 -> 43/59 (honest total 8655, +3; the only drop against the main
checkout's report, tu_804023D0 fn_8040245C, comes from matching having moved past this branch).
100 %: fn_802632A8, fn_80263860, fn_8026441C.
Structurally done: fn_8026022C 95.46, fn_80260C08 96.63 (scheduling+regalloc), fn_8026152C 99.17
(regalloc), fn_80262254 99.64 (scheduling+regalloc). Close: fn_80261750 99.65 (1 lwz), fn_80262BAC
98.88 (2 lfs), fn_802604CC 88.23 (41 stack-temporary stores of TVec2 helpers missing; kept for the
.sdata2 order). .sdata2 96 %.
SHARED-EDITS: include/game/BgRewrite.h members fn_8029E888 (u8), fn_802A01F0 (bool), fn_802A02A8 (s32),
fn_802A3164 (bool), all (const TVec3f* pos, s32 player, s32 layer); include/game/cls_804A71A8.h
inline pushedAlongX() (tu_802684A4's file inline pushedSideways has the same body);
include/game/act/act_thief.h Thief_804A5A30::fn_80206168() (void). symbols.txt: those five renamed to
their mangled names.
Needed fields (agent Q / act_cucco owner): Cucco_804AB8C0 u8 at 0x2A1 (fn_80263078 tests == 1).
Next: fn_80263C64 (0x7AC, 1.25 / 1.5), fn_80263078; fn_802604CC's TVec2 temporaries.

## Session 2026-11-06 round 17 (Opus 5.5)
Baseline 8960 total; unit 43/59 -> 45/59 (honest total 8962, +2, no drops by address against the
main checkout's report_honest.json). Every function of the unit is now written.
100 %: fn_80263C64 (state 0x12 update, new), fn_8026152C (VecEpsilon.h nearZero).
Structurally done: fn_80263078 99.78 (regalloc; state 0x13 enter, new). fn_80262BAC 98.88 -> 99.23.
fn_8025FCC8 87.39 -> 87.02 (file inlines replaced by plain functions).
SHARED-EDITS: include/game/VecEpsilon.h (nearZero: the difference first; all its users rise, none
drop); include/game/act/act_cucco.h (u8 field_0x2a1 at 0x2A1 carved from field_0x230[0xC4] ->
[0x71] + field_0x2a2[0x52], size unchanged 0x304; int accessor getField0x2a1()) with both act_cucco
rows removed from config/G4SE01/gen_game.tsv (gen_game --check ok).
Next: fn_80261750 (1 lwz), fn_8025FDD0 / fn_8025EB18 (float registers of the view-corner offset),
fn_802604CC.

