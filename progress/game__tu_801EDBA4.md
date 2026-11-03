# game/tu_801EDBA4

Baseline: `total: 5920 / 11557 functions, 1185564 / 4442032 bytes of code (26.69 %)`.
After: `total: 5922 / 11557 functions, 1185924 / 4442032 bytes of code (26.70 %)`. gate: ok.

Class VaatiBoss_804A4958 (vtable 0x804A4958, EnemyBase_804A4808 child; actor BOGF "Vaati (boss)" in
actors.tsv, game_units.tsv), header include/game/act/act_vaati_boss.h (owner unit act_vaati_boss, unwritten).

## Matched (3/4)
draw (0x801EDBA4), fn_801EDC0C (100%, incl. data/rodata), generated __sinit. .data lbl_804A5760 and
.rodata match (file-level non-const pointer table, defined before fn_801EDC0C so the strings sort first).

## Resistant
- fn_801EDD0C 32.8%, missing/extra + reloc-name: the target picks max(1.0f + field_0x554, 0.001f) with
  one fsel; ours compiles every ?: form tried as fcmpo/cror/branch (same gap as
  EnemyBase_804A4808::fn_801CE010). The scaled-matrix loop then also schedules differently
  (target: 12 loads then 12 fmuls into f4-f13/f30/f31, ours interleaves) and .sdata2 literal
  order follows. Stack layout (params 0x38, mtx 0x8), flags 7/0x407, state-5 branch all match.

## SHARED-EDITS
- include/game/GameGlobals.h: extern const f32 lbl_8053F0B8 (90.0f) / lbl_8053F0BC (1.7f), owner tu_801D3CBC
  (the target loads them as symbols and multiplies -12.0f * 1.7f at run time).
- config/G4SE01/gen_game.tsv: tu_801EDBA4 row removed.
- config/G4SE01/symbols.txt: draw/fn_801EDC0C/fn_801EDD0C renamed to member names;
  fn_801D6CEC -> fn_801D6CEC__12cls_801D6CECFPQ29JGeometry8TVec3<f> (tu_801D6CEC, unwritten).

## Next
Find the source form that yields fsel; then re-check fn_801EDD0C's matrix scale loop.
