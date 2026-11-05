
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
