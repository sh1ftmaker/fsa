# game/act_fireball_generator.cpp

Baseline: total 5820 / 11557 functions, 1165664 bytes (26.24 %).
After: 5824 / 11557, 1166340 bytes (26.26 %).

Matched (5/5): dtor, vfunc_0x90 (switch (mState) with one case 0 gives the
original's redundant beq/b), init, ctor, __sinit (generated; gen_game.tsv row
deleted, generated lines unchanged).

Resistant: none. Remaining section deltas (data 99.38 % = vtable symbol 164 vs
168 bytes, sdata2 90.91 % = 20 vs 24 bytes, extab) are alignment padding and
weak-duplicate extab entries, same as the Matching act_tingle_buster/fuse_point.

SHARED-EDITS: config/G4SE01/gen_game.tsv (deleted the unit's one row);
config/G4SE01/symbols.txt renames of this unit's placeholders only.

Next: coordinator flips Matching after a trial link.
