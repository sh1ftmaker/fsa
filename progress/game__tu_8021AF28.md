# game/tu_8021AF28.cpp checkpoint

Baseline (progress.py): 6340 / 11557 functions, 1270548 / 4442032 bytes (28.60 %).
After: 6344 / 11557, 1270980 bytes (28.61 %). Unit: 5/8 (was 1/8).

## Matched
draw (fn_8021AF28), fn_8021B548, fn_8021B5A0, fn_8021B5C4, __sinit.

## Resistant
- fn_8021AFB8 93.4 % (missing/extra): the original shares one `li r30, 0` between
  the SpriteParams reset's zero stores and the `shift` variable (saved over the
  fn_80219E04 call); ours uses r6 for the zeros and a later li r30. Tried: shift
  declared before params (regs swap), ternary (worse), inline helper for the
  second draw (helped: 90 -> 93.4).
- fn_8021B1AC 98.9 %: the same one-instruction difference.
- fn_8021B668 93.3 % (scheduling, STOP): original loads field_0x4 into r31 before
  `mr r29, r3`; inline helper for the depth made it worse.

## SHARED-EDITS
- config/G4SE01/gen_game.tsv: removed the tu_8021AF28 row.
- config/G4SE01/symbols.txt: placeholder renames only (draw/fn_8021AFB8/fn_8021B1AC/
  fn_8021B548/fn_8021B5A0/fn_8021B5C4/fn_8021B668/fn_80219E04 -> __12cls_804A6550Fv).
- New include/game/act/act_kaepora_gaebora.h (owner act_kaepora_gaebora; class
  cls_804A6550 named after its vtable 0x804A6550, EnemyBase_804A4808 descendant).
  fn_80219E04 is declared there as a member (defined in the unwritten owner unit).

## Next
act_kaepora_gaebora.cpp owner: fill the header's unknown fields (0x2A8-0x2C4), size.
