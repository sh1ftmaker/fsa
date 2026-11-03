# game/act_push_switch_in_pyramid

Baseline: total 6046 / 11557 functions, 1220300 bytes (27.47 %). After: 6048, 1221200 (27.49 %).

Matched: dtor, update (100 %, needs an empty `case 2:` for the extra branch), init. Data/rodata/sdata2/bss at 100 %.

Resistant: draw (94.04 %, scheduling+regalloc): the squared-length test of field_0x54 uses
f3 for the psq pair and f2 for z; the target has f2 pair, f3 z. Tried: `!(sq <= ..)`, local copies,
file-local inline predicate, `sq > ..` (worse). Nothing else differs.

SHARED-EDITS: config/G4SE01/gen_game.tsv (both rows of this unit removed; generated lines unchanged).
Next: draw register order.
