# JSystem/J2DGraph/J2DBloSaver.cpp

Baseline (progress.py first line): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: same code total (data-only change). Unit stays NonMatching: 10/11 functions, .text 98.60 %,
.ctors/.bss/.sbss now all 100 % (data was 12/28, .bss 0 %).

## Done this pass
- `.bss` 0x804BA4B0: the placeholder `lbl_804BA4B0` (size 0x10) is the compiler's
  destructor-chain object that `__sinit_J2DBloSaver_cpp` passes to `__register_global_object`
  for `TextureNameConnect`: renamed to `@315`, size 0xC, scope:local, the same form the other
  units' registration objects have in symbols.txt (`@397`.., `@264`, `@408`, all 0xC). The
  4 bytes after it are alignment padding before J2DManage's `p_name$52` at 0x804BA4C0.

## Resistant: __ct__11J2DTevStageFv (88.08 %), blocked by codegen (shared with J2DMaterialFactory)
```
24  lbz r4, 0x7(r31)          | lbz r0, 0x7(r31)
28  lbz r0, 0x1(r3)           | lbz r4, 0x1(r3)
34                            | lbz r0, j2dDefaultTevSwapMode@sda21   (hoisted above the stb)
44  lbz r4, 0x7(r31)          |
48  lbz r0, j2dDefaultTevSwapMode@sda21 | lbz r4, 0x7(r31)
```
The original doesn't let the load of `j2dDefaultTevSwapMode.mRasSel` pass the store to `_07`:
it treats the object as non-const. Declaring it non-const in J2DTypes.h gives 100 % (tested in
the J2DMaterialFactory worktree for the sibling ctor; pikmin2 does exactly this and calls it a
workaround), but its definition in J2DTevs.cpp is const in .sdata2, so that declaration would
contradict it: rejected. See progress/JSystem__J2DGraph__J2DMaterialFactory.md for the variants tried.

## Note
The source still has mkdd's placeholder bodies (writeMaterialBlock, countParts, setOffset, get,
TNC ctor) that only exist to emit the weak functions; the DOL has none of them (dead-stripped).

## SHARED-EDITS
None.
