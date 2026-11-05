# game/act_old_man (cls_804AEED0, JIJI)

Baseline: 7964 / 11557 functions. After: 7981 (17/22 functions in the unit).

Matched: the 15 step/timer methods (fn_8037B374..fn_8037B7EC), destructor, constructor.
The .sdata tables are file statics in address order, non-const (.sdata, not .sdata2);
lbl_8053A204 is [2][4] (indexed field_0x248/2, field_0x23c); lbl_8053A20C has 3 bytes (symbol size).

Unwritten (big): fn_8037B7F8 draw (0x438), fn_8037BC30 update (0xB58), fn_8037C788 init (0x5F0),
fn_8037CE5C = the unit's __sinit (0x928, copies a 0x100-byte .data table into .bss 0x8050D900
with struct copies; unrelated to the actor class). fn_8037B3E0 is at 99.76 %: only the sdata2
literal relocations (@NNN vs lbl_) differ, the unit's literals are not all split until the big
functions exist.

SHARED-EDITS: include/game/act/tu_801F9484.h cls_804AEED0 (virtual init/update/draw, 16 step
methods, fields 0x230..0x23C, 0x248, 0x273, 0x274 cls_8031709C*, 0x278 JPABaseEmitter*).
Symbol renames in symbols.txt (fn_8037Bxxx -> member-mangled, fn_8037CD78 -> __dt__12cls_804AEED0Fv).
