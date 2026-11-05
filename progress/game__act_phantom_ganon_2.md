# game/act_phantom_ganon_2 (agent checkpoint)
Baseline 7849 / 11557 functions. Now 7863 (+14 of 24 in this unit).

Matched (100 %): vfunc_0x24 (0x80187144), ActorBase fn_8018A590/598/5A0/5A8/5B0,
fn_8018A5B8, 5F0, 614, 63C, 660, 684, 6A0, 6AC.
Resistant: fn_80189DEC 98.9 % (regalloc), ~cls_804A1A98 99.7 % (regalloc),
__sinit 99.61 % (needs the 0x10 bytes of .bss before the pool records).
Not written: draw 0x801871A0 (SpriteParams + data tables at 0x804A1960),
update 0x80187530, init 0x8018A070, setLife 0x8018A588, emitter helpers 0x8018A5C8/5D0/5E0.
SHARED-EDITS: include/game/ActorBase.h (5 member declarations, fn_8018A590-5B0);
include/game/act/tu_801F9484.h (stub cls_804A1A98 removed; now in act_phantom_ganon_2.h);
src/game/tu_801F9484.cpp (include added).
