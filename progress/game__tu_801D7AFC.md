# game/tu_801D7AFC (wave 26)

Baseline (progress.py): total 6340 / 11557 functions, 1270548 bytes. After: 6345 / 1271776.
Unit before: 1/7 functions; after: 6/7 (draw, fn_801D7DB4, init, dtor, ctor, __sinit).

## Matched
cls_804A4BB0 (vtable 0x804A4BB0): draw, fn_801D7DB4(f32) (out-of-line scale setter, inlined twice in init), init, dtor, ctor.
`32.0f * FLT_EPSILON` as a file-local bool inline `isTiny` (tu_8020FE50 precedent). Emitter scale: `setGlobalScale(field_0x54)`.

## Resistant
- update 99.47 % regalloc-only: target keeps layer/mode in r27 and the loop counter in r28; ours counter r27, mode r28, layer r29.
  Tried: no layer local (98 %, worse), layer reused as mode (same 99.47), declared-early mode, counter declared outside the loop.
- .sdata2 94.74 %: the target has an extra 0.0f after the -0.0f (lbl_8053F1C0 is 8 bytes: -0, 0); ours folds only -0.0f.
  Not found which source makes the compiler keep a second 0.0 (init's setter(0.0f) inline).
- extab/extabindex 52 %: weak-duplicate entries, as in other units.

## SHARED-EDITS
- config/G4SE01/gen_game.tsv: row of tu_801D7AFC deleted (its sinit comes from the GameAudio.h include).
- config/G4SE01/symbols.txt: placeholder renames of this unit's functions to cls_804A4BB0 members, lbl_804A4BB0 -> __vt__12cls_804A4BB0 (size 0xA8 -> 0xA4, as __vt__12cls_804A5038; the last word is padding). tu_801D6CEC calls fn_801D7DB4 at 0x801D7090 (asm regenerates, no source caller yet).
