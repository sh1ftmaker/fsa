# game/act_ganon checkpoint (2026-11-05)

Baseline 8093 total, unit 0/19. After: 8105, unit 12/19 (all 11 small functions plus __sinit_act_ganon_cpp).

Class Ganon_804A25F0 ('GNON') moved from the spawner header (tu_801F9484.h) to include/game/act/act_ganon.h.
Written: vfunc_0x24 (fn_80192038), dtor (fn_80196834), out-of-line copies fn_80196CF4 (JPABaseEmitter::setGlobalScale),
fn_80196D18 (emitter particle scale), D24/D60/D70 (TVec2/TVec3 set copies), D30 (fn_80134DE4 forward), D8C (GameView fn_802D79E8 forward),
DF0 (lbl_80529DEC byte 0x24f), E00 (lbl_8053AED0 field_0xdc |= 0x10), __sinit with the two bss TVec3f[4] tables.
Unwritten (all over 160 B): fn_80192090, fn_80192BD8, fn_80192D98 (update, 0x3288), fn_80196020, fn_80196208, fn_801964AC, fn_801968C8.
SHARED-EDITS: tu_801F9484.h (class removed), tu_801F9484.cpp (includes act_ganon.h).
Note: lbl_804C1BC0 (s32[4], type guessed from an lwzx indexed by player) exists to give the sinit the right bss base.
