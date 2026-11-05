
## Session 2026-11-05 (Sonnet 5.5)
Baseline 7877 total; unit 10/59 -> 33/59 (honest total 7899). All tiny/small Player members done.
SHARED-EDITS (include/game/Player.h): field_0x300 -> TVec3f; field_0xd6c s32; 0xD78 TVec3f field_0xd78, 0xD84 s32, 0xD88 s32; 0x121C/0x121D u8s; new member decls fn_802632A4/80262234/80262B6C/80262B8C/8026173C/80261468/80258BBC/80259660/80283D2C/8025F484(const); fn_8025F684 stays bool (a u8 return dropped fn_8023EFEC in tu_8022AD64 from 100% to 89.7%).
symbols.txt: renamed fn_80258BBC, fn_80259660, fn_80283D2C (tu_80256C74/tu_8027E5BC) and this unit's thunks to mangled Player_804A6F38 names.
Notes: file-static isField0xd64One() inline reproduces the inlined subfic/cntlzw/extrwi test in F484/F500/F55C/F590; F684 is its out-of-line copy returning bool. Making F684 an inline member in Player.h was measured: F684 is not emitted, fn_8023EFEC inlines it (81%).
Next: fn_8025EE48 (288), fn_8025F2CC, fn_8025EFFC and larger ones.
