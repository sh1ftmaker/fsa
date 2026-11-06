# game/act_ice_wall

Baseline: 8976 / 11557 functions (41.16 %).
Unit status: 6/6 functions written. 2 at 100 %, 4 structurally matched (≥ 95.8 %, scheduling/regalloc/reloc-name only).

Functions:
- __dt__16IceWall_8049DE18Fv: 100.00 %
- __sinit_act_ice_wall_cpp: 100.00 %
- init__16IceWall_8049DE18Fv: 99.86 % (scheduling + reloc-name)
- __ct__16IceWall_8049DE18Fv: 99.81 % (reloc-name: 1.0f literal in .sdata2)
- draw__16IceWall_8049DE18Fv: 99.28 % (scheduling+regalloc + reloc-name)
- update__16IceWall_8049DE18Fv: 95.85 % (scheduling+regalloc + reloc-name)

All functions match in instruction count and opcodes; differences are register allocation and scheduling of stack box setup / float literal relocations in .sdata2.

SHARED-EDITS:
None.
