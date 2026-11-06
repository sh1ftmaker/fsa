# game/act_frostare_tail (A/B run, Opus 5.5, 2026-11-05)

Baseline: total 8776 / 11557 functions. After: 8785 (+9: 8 in this unit, +1 tu_801E5FC8 draw).
Unit: 9/12 functions at 100 % (ctor, dtor, init, draw, vfunc_0x60, 0x74, 0x90, 0x9C, __sinit).
Resistant (FIXME in source):
- fn_801DC264 98.39 % regalloc-only (result r5 / field_0xdc r4 swapped); 3 tries.
- vfunc_0x5C 97.14 % scheduling+regalloc (layer flag / rupee serial r27/r28 swapped, velocity temps' stack slots swapped).
- vfunc_0x88 97.86 % regalloc-only (f0/f1 swapped around fabs + fadds).
SHARED-EDITS: cls_804A4F90 moved from tu_801F9484.h to game/act/act_frostare_tail.h (now EnemyBase_804A4808-derived, mHitCount u8 at 0x2A8, size 0x2AC unchanged);
Singleton_801DAC6C::field_0x84 s16 -> u16 (lhz in the tail); fn_801DC264 and Singleton_801DAC6C::fn_801DAD2C return u8 (tu_801E5FC8 draw local u8 -> 100 %).
Next: unit data (.data vtable trailing word, .sdata2 8.0f object size 8) not checked; vfunc_0x5C register order.
