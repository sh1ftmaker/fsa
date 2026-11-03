# game/tu_8011E96C

Baseline: total 5783 / 11557 functions, 1160172 bytes (26.12 %).
After: 5784 / 11557, 1160388 bytes.

Matched: ~dtor, init, __sinit (3/4).
Resistant: update (97.45 %, missing/extra). Original reloads field_0x4 after
each fn_8023DE58 call in the state-1 layer check (mine hoists it), and has one
extra unreachable `b` after the switch dispatch (default/empty case did not add it).
SHARED-EDITS: none (gen_game.tsv rows for this unit deleted; header is generator-owned).
Next: try other forms of the layer check / switch.
