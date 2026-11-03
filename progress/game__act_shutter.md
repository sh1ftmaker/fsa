# game/act_shutter
Baseline: total: 6007 / 11557 functions, 1208940 / 4442032 bytes of code (27.22 %)
After: 6008 / 11557, 1208944 bytes.

Matched: __dt__, draw, __sinit (3/5).
Resistant (scheduling+regalloc, 94.7% update, 94.4% init): in every
fn_802A3884/fn_802A3948 call with the cell above (&p, layer + 1), the original
loads field_0x4 and does the addi before the fsubs of p.y (lwz, fsubs, lfs x,
lfs 0, addi, stfs...); ours puts the lwz/addi after the stfs of p. Tried:
TVec3f ctor, member-wise assignment (worse), p.set().
SHARED-EDITS: config/G4SE01/gen_game.tsv (act_shutter rows removed),
include/game/act/act_shutter.h (generator-owned, fields added).
