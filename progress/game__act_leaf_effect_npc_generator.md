# act_leaf_effect_npc_generator checkpoint
Baseline: total 5783 / 11557 functions, 1160172 bytes (2/6 unit functions). Now 5785, 1160180; unit 4/6.
Matched: dtor, draw, update, sinit.
Resistant (regalloc-only, FIXMEs in source):
- fn_8036D858 (member, 98.27 %): original end r31, i r28, offset r29, actor r30.
- init (99.02 %, class operands): register numbers differ and spawn pos is at sp+0x10 in the original (ours sp+0x14).
SHARED-EDITS: none besides the unit's own header, gen_game.tsv rows, symbols.txt rename.
Next: try loop-shape variants for the two regalloc diffs.
