# game/tu_801EE24C.cpp (ActorBase_804A5770), hdr/actor branch

Matched (honest): ~ActorBase_804A5770 (0x1A4), draw, savePrevPos, update,
vfunc_0x10, init.

Resistant:
- __ct__18ActorBase_804A5770Fv 99.86 %: only its .sdata2 float literals (@NNN vs
  lbl_8053F740/F774/F7F4/F7A4/F7F8) differ; they pair once the unit's other
  functions put .sdata2 in the original order. lbl_8053F7F8 (40.0f) is
  size 0x8 in symbols.txt (40.0f plus a zero pad word): it needs a size fix
  (approved_symbol_changes or a placeholder split) for the literal to pair.
- vfunc_0x18__18ActorBase_804A5770Fv (fn_801F5790) 99.23 %: float register allocation
  (player z f5 vs f7) besides the literals.

Symbols renamed: the functions above, __vt__18ActorBase_804A5770 (0x804A5770),
__ct__20ActorSub1DC_801EAD68Fv (0x801EAD68), __ct__Q29JGeometry8TVec3<f>Fv
(0x8003B2CC, the weak empty ctor __construct_array uses for field_0x1ac).

Next: the remaining ~95 functions of the unit (vfunc_0x24..0x50 first),
then the .sdata2 order resolves the ctor.
