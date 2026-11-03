# game/tu_801DAC6C

Baseline: total: 6229 / 11557 functions, 1252968 / 4442032 bytes of code (28.21 %)
After: total: 6237 / 11557 functions, 1253820 / 4442032 bytes (28.23 %)

Class Singleton_801DAC6C (include/game/act/tu_801DAC6C.h): list of up to 30 actor serials
plus two watched serials, a guarded static at 0x804C92D0 built by the getter fn_801DAC6C.
Not in gen_game.tsv (bss is 0x188, not the template's 0x100): the __sinit is the 9-pool
one via GameAudio.h and matches.

Matched (8/12): getter, fn_801DAD88, fn_801DADD0, fn_801DAF14, fn_801DAF3C, fn_801DAF44,
fn_801DAF4C, __sinit.
Resistant:
- fn_801DACAC 81% scheduling: orig loads serial (lwz r31) and li r3,0 before saving r30 /
  mr r30,r4; cmpwi precedes the 2nd/3rd stfs.
- fn_801DAD2C 91% scheduling: orig lwz r31 before `stw r30` / `li r30,0`.
- fn_801DAE24 87%: orig regs r31=count, r30=-1, r29=ptr, r28=i; ours reversed.
- fn_801DAEAC 96% STOP: orig `mr r5, r6` (i starts as found's zero, i=r5, count r8); ours li r8.
Gaps: .sbss: lbl_8053AC30 (u8 counter 0-4 used by tu_801E2540/tu_802D39B8) sits at +8 in the
original (8-aligned), ours at +1; real type unknown. .sdata2: original has 8 bytes (two 0.0f),
ours one. extab/extabindex ~57%: same as other generated units (weak pool entries).

SHARED-EDITS: include/game/GameGlobals.h (removed Singleton_801DAC6C/fn_801DAC6C/fn_801DAD2C
decls, now in act/tu_801DAC6C.h; added fn_801DC264 [act_frostare_tail unit, s32] and
fn_801D8400 [tu_801D8400 unit, cls_804A4C58*] extern "C" decls); src/game/tu_801E5FC8.cpp
(call becomes fn_801DAC6C()->fn_801DAD2C(); try.py unchanged at 5/8); symbols.txt renames.
Next: find the real type of lbl_8053AC30 and the second sdata2 float.
