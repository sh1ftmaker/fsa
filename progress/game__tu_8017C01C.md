# game/tu_8017C01C (cls_804A1318, 'FGAN')

Baseline: 7964 / 11557. After: 7975 (14/21 in the unit, +11).

Matched: vfunc_0x24 (fn_8017C01C), fn_8017EAB4 (JPABaseEmitter::becomeInvalidEmitter copy), EACC (setGlobalEnvColor copy),
EADC/EB0C/EC38/EC5C/ECE8/ECF4 (JGeometry/TColor out-of-line copies), EB1C.

Resistant (FIXMEs in source): fn_8017EB44 83.75 % (constant 1 / zero register sharing), fn_8017EC70 93.55 %
(box store order f.x,i.x,f.y,i.y + reloc-name), fn_8017ECC0 89 % (result register stored to field_0x178),
fn_8017EB94 99.76 % (reloc-name only: sdata2 literals unsplit until the big functions exist; the stack-slot order
needed TVec3f corner2 declared first, camera before result).

Unwritten: fn_8017C078 draw (0x394), fn_8017C40C update (0x2234), fn_8017E640 dtor (0x130), fn_8017E770 init (0x344).

SHARED-EDITS: include/game/act/tu_801F9484.h cls_804A1318: virtual vfunc_0x24, field_0x2ba (s8, lbz+extsb, passed to vfunc_0x28).
Symbol renames in symbols.txt for the 14 matched fns.
