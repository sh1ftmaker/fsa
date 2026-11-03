# game/tu_8020CCA8 (cls_804A51E4, TINK)

Baseline: total: 6287 / 11557 functions, 1258900 / 4442032 bytes of code (28.34 %)

All 5 functions written (draw + four members); none byte-exact yet.
- draw 98.83% scheduling+regalloc: y/x loads of field_0x54 swapped (as PushSwitchInPyramid draw).
- fn_8020CF7C 88.89%: lwz field_0x4 scheduled earlier in the original.
- fn_8020CFC4 98.11%, fn_8020D16C 97.50%: first lwz field_0x4 before the prologue stw r0.
- fn_8020D2AC 94.47%: mPos copy loads z first in the original.
Tried: local `layer` (worse: callee-saved reg), getField_0x4() (no change), TVec3f copy forms.

SHARED-EDITS: include/game/act/tu_801DE714.h (6 member decls); symbols.txt renames
(draw__12cls_804A51E4Fv, fn_8020CF7C/CFC4/D16C/D2AC__12cls_804A51E4Fv, fn_801E1D7C__12cls_804A51E4Fv);
gen_game.tsv row for tu_8020CCA8 removed.
