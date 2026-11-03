# game/tu_801E5FC8

Baseline: `total: 5901 / 11557 functions, 1181784 / 4442032 bytes of code (26.60 %)`.
After: `total: 5905 / 11557 functions, 1182252 / 4442032 bytes of code (26.62 %)`. gate: ok.

Class cls_804A4C58 (vtable 0x804A4C58, EnemyBase_804A4808 child), header include/game/act/tu_801D8400.h.

## Matched (5/8)
fn_801E61C4, fn_801E6234, fn_801E6394, fn_801E63D8 and the generated __sinit.

## Resistant
- draw (0x801E5FC8) 98.39%, scheduling+regalloc: target loads the colour byte 0x4c into r7 and
  adds in place (`lbz r7; ... add r7, r7, r3`), ours `lbz r0; add r7, r0, r3`.
  Declaring mtx before params changes the stack layout (worse).
- fn_801E62D4 88.89%, fn_801E631C 93.33%, scheduling: target loads field_0x4 (lwz r3, 4(r3))
  right after the `addi r4, .., 0xc`, before the li r5..r10; ours after the first li's.
  Tried an inline wrapper and a `layer` local: no change.

## SHARED-EDITS
- include/game/ActorBase.h: SpriteParams_801F5AE0::field_0x14 is now a union (mWord / GXColor mColor),
  because draw adds to bytes 0x14-0x16 (lbz/add/stb at 0x801E6154-0x801E6188). An anonymous
  union/struct shifted the @NNN labels of every generated unit (gen_game.py --check); a named union
  type does not. Constructor stores are unchanged (act_rat, act_tingle_wallet, act_pot_smoke,
  tu_801DCC98 give the same try.py numbers).
- include/game/Effect.h: fn_801CB71C(JPABaseEmitter**, const TVec3f*, s32, s32) (owner tu_801CB270).
- include/game/GameGlobals.h: fn_801DA938/fn_801DA904 (take Sequencer_801DA7C0*), fn_801DAC6C/fn_801DAD2C
  (Singleton_801DAC6C), owners tu_801DA7C0 and the unit at 0x801DAC6C.
- config/G4SE01/gen_game.tsv: the tu_801E5FC8 row removed.
- config/G4SE01/symbols.txt: the 7 functions renamed to their mangled member names.

## Next
Unwritten units tu_801D8400 / tu_801DD1AC own the rest of cls_804A4C58 (the header has only the members used here;
the 0x2A8 Sequencer_801DA7C0 is 0x18 bytes, ends at 0x2C0).
