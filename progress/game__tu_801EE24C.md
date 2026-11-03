# game/tu_801EE24C.cpp (ActorBase), hdr/actor branch

Matched (honest): ~ActorBase (0x1A4), draw, savePrevPos, update,
vfunc_0x10, init.

Resistant:
- __ct__9ActorBaseFv 99.86 %: only its .sdata2 float literals (@NNN vs
  lbl_8053F740/F774/F7F4/F7A4/F7F8) differ; they pair once the unit's other
  functions put .sdata2 in the original order.
- vfunc_0x18__9ActorBaseFv (fn_801F5790) 99.23 %: float register allocation
  (player z f5 vs f7) besides the literals.

Symbols renamed: the functions above, __vt__9ActorBase (0x804A5770),
__ct__11ActorSub1DCFv (0x801EAD68), __ct__Q29JGeometry8TVec3<f>Fv
(0x8003B2CC, the weak empty ctor __construct_array uses for field_0x1ac).

Next: the remaining ~95 functions of the unit (vfunc_0x24..0x50 first),
then the .sdata2 order resolves the ctor.
