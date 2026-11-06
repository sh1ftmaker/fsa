# game/act_pot

Unit: 5/8 functions at 100 % (init, __dt, vfunc_0x48, vfunc_0x4C, __sinit).
Resistant (FIXME in source):
- update__12Pot_804A9428Fv: 99.01 % missing/extra (duplicate null check in blz2 loop, emitter direction branch order).
- vfunc_0x24__12Pot_804A9428Fv: 95.67 % missing/extra (stack frame size 0xA0 vs target 0x80; local vector layout).
- draw__12Pot_804A9428Fv: 91.51 % scheduling+regalloc (frame base addition operand order).
SHARED-EDITS: include/game/GameGlobals.h: declared searchlight/broken pot position lbl_804C0EA0 and layer lbl_8053AB68.
