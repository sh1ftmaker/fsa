# act_leaf_effect_npc_generator checkpoint
Baseline: total 5783 / 11557 functions, 1160172 bytes (2/6 unit functions). Now 5785, 1160180; unit 4/6.
Matched: dtor, draw, update, sinit.
Resistant (regalloc-only, FIXMEs in source):
- fn_8036D858 (member, 98.27 %): original end r31, i r28, offset r29, actor r30.
- init (99.02 %): this in r27 (ours r28), counters r29/r28 (ours r30/r26).
SHARED-EDITS: none besides the unit's own header, gen_game.tsv rows, symbols.txt rename.
Next: try loop-shape variants for the two regalloc diffs.
