# game/act_invisible_npc_wall_tomi
Baseline: total: 5751 / 11557 functions, 1155176 / 4442032 bytes (26.01 %).
Now: 3/4 functions (dtor, init, sinit match); 5753 before the small-key stub.

Matched: __dt, init (switch-free; mode field 0x234 added), sinit.
Resistant: update (82 %, missing/extra). Case 1 needs SmallKey_804A8CD8::field_0x240 (== 4),
field_0x3c.y < 0 and fn_802C2BD0(key); act_small_key.h is generated (gen_game.tsv) and
gate rejects edits. With the header edit (field_0x240 s32, fn_802C2BD0(SmallKey_804A8CD8*))
update was 4/4 at 100 %. Fix: when act_small_key drops its gen_game rows, add them and
fill in the FIXME.
SHARED-EDITS: include/game/Map.h CollisionBoxes_8020D58C::addBox(s32) inline (needed so the
serial is evaluated before the call, as in the original).
