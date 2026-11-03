# game/act_torch
Baseline: total 6046 / 11557 functions, 1220300 bytes. After: 6049 / 11557, 1221220.

Matched: update, __dt__, init (100%), __sinit (generated, 100%).
Resistant: draw__14Torch_804A8520Fv 99.88% (operands). Only the stack slots of the
TColor temporaries differ: original pairs copy (sp+0x20/0x18/0x10/0x8) with its
temporary 4 bytes above; ours has temporaries 0x24/0x20/0x1c/0x18 and copies
0x14/0x10/0xc/0x8. Tried named local, inline wrapper, scale after pos.
.data 97.67% is the vtable's reference to draw.
SHARED-EDITS: none (gen_game.tsv: removed act_torch sinit row).
