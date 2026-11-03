# game/tu_8021C2B4

Baseline (progress.py): 5820 / 11557 functions, 1165664 bytes. After: 5821 / 11557, 1165780 bytes.

## Matched
- `__ct__12cls_8021C5B0Fv` (100 %; first 0x30 bytes are an inlined base ctor, then the cls_8021CBA0 ctor call)
- `__sinit_tu_8021C2B4_cpp` (generated rows removed from gen_game.tsv)

## Resistant (regalloc-only)
- `fn_8021C2B4__12cls_8021C2B4Fv` 99.72 %: vel.x/vel.y computation after fn_8021CB18: target does fsubs f3,f4,f3 / fmuls f2,f6,f2 / fmuls f3,f6,f3; ours uses f4, f3, f2.
- `fn_8021C468__12cls_8021C2B4Fv` 99.88 %: target `fmuls f0, f1, f0` (atan2 result * 10430.378f), ours `fmuls f0, f0, f1`.

## SHARED-EDITS
- gen_game.tsv: tu_8021C2B4 row deleted.
- symbols.txt: renames of fn_8021C2B4/C468/C5B0 and tu_8021C848's fn_8021C848/C9C4/CA70/CB18/CBA0 to member-mangled names.
- GameGlobals.h: fn_801C0FEC as a member of cls_801C0D70; symbols.txt renamed to its mangled name (owner tu_801C0D70).
- New headers cls_8021C2B4.h, cls_8021CBA0.h (cls_8021CBA0 is tu_8021C848's class).

## Next
Write tu_8021C848 (define cls_8021CBA0 members; header already declares them).
