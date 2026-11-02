# JSystem/JParticle/JPAFieldBlock.cpp

Baseline (start): `total: 1307 / 11560 functions, 241984 / 4434096 bytes of code (5.46 %)`
Final:            `total: 1337 / 11560 functions, 251904 / 4434096 bytes of code (5.68 %)`

Status: **Matching** (29/29 functions, .text/.data/.sdata2 100 %, `gate: ok`, main.dol SHA-1 OK).
No source changes were needed; every miss was a placeholder name or a short split.

## Changes
- symbols.txt `fn_8008440C` -> `__nw__FUlP7JKRHeapi`: body is byte-identical to the
  already-named `__nwa__FUlP7JKRHeapi` (heap ? heap->alloc : sCurrentHeap->alloc) and sits
  third in the JKRHeap operator trio `__nw__FUl`/`__nw__FUli`/`__nw__FUlP7JKRHeapi`, same as
  mkdd's symbols order.
- symbols.txt `lbl_80539D44` -> `__float_epsilon`: value 0x34000000 (1.1920929e-07), preceded
  by 0x7FFFFFFF / inf / FLT_MAX at 0x80539D38..40 = MSL float.c's `__float_nan`, `__float_huge`,
  `__float_max`, `__float_epsilon` (mkdd float.c). Neighbours left as placeholders (not my unit).
- splits.txt: .text end 0x800EEA34 -> 0x800EEDBC (start of JPAKeyBlock) to cover the weak
  vtable functions.
- symbols.txt fn_800EEA34..fn_800EED60 -> `__dt__12JPAFieldBaseFv`,
  `prepare__12JPAFieldBaseFP18JPAEmitterWorkDataP13JPAFieldBlock`, and the nine `__dt__` of
  Spin/Drag/Random/Convection/Vortex/Newton/Magnet/Air/Gravity (scope:weak). Evidence: each dtor
  stores its own `__vt__` first; they fill vtable slot 2 (dtor) and Base slot 3 (prepare, a blr).
- configure.py: JPAFieldBlock.cpp -> Matching.

## Note
JPAFieldBase::calcAffect / calcFadeAffect are emitted out-of-line by our build but absent from
the DOL (inlined everywhere, dead-stripped at link); link is byte-identical.

SHARED-EDITS: none (only symbols.txt placeholder renames; `__nw__FUlP7JKRHeapi` and
`__float_epsilon` are referenced from other units too and improved them).
