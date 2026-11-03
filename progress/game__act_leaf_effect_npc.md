# game/act_leaf_effect_npc
Baseline: total: 5932 / 11557 functions, 1188620 / 4442032 bytes of code (26.76 %)
Matched: __dt, draw, __sinit (3/5).
Resistant:
- update 85%, missing/extra: original jumps from the kind==1 branch into the loop's non-state-1 spawn block (3 spawn copies); ours inlines a 4th.
- init 94%, different: original probe points in 8-byte stack slots (frame 0x30, y stored before x); ours 12-byte TVec3f (frame 0x40).
SHARED-EDITS: gen_game.tsv rows removed (2); include/game/act/act_leaf_effect_npc.h fields; src/game/act_leaf_effect_npc_generator.cpp field_0x230[0] -> field_0x230.
Next: sdata2 literals show @NNN vs lbl_ (1.0f etc.).
