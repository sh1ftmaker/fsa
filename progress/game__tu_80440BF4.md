# game/tu_80440BF4

Baseline (progress.py): total: 6229 / 11557 functions, 1252968 / 4442032 bytes (28.21 %).
After: 6237 / 11557, 1254248 bytes (28.24 %). Unit: 9/9 functions match.

Matched: cls_804B1988 (vtable 0x804B1988, "button.blo" overlay): fn_80440BF4/C6C (pane visibility), fn_80440CE4/D78 (visible + changeTexture), fn_80440E0C (alpha loop), fn_80440EE8 (draw), dtor, ctor. First try each; no FIXMEs.
Resistant: none. extab/extabindex are weak-duplicate entries in our object (NonMatching, so unmeasured); .data/.sdata2 < 100 % are the weak PaneCtrl_804AF86C vtable and the 4-byte alignment of 0.0f (dtk's lbl_80543D38 is 8 bytes), as in tu_8044C028.
SHARED-EDITS: none besides this unit's gen_game.tsv row (removed) and symbols.txt renames (fn_80440xxx -> mangled members of cls_804B1988, lbl_804B1988 -> __vt__12cls_804B1988). lbl_8053AED8 (sbss of unwritten tu_8043A0E8) is declared extern in include/game/act/tu_80440BF4.h.
Next: flip Matching after a trial link.
