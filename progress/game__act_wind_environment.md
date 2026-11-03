# game/act_wind_environment.cpp

Baseline (progress.py): total: 6287 / 11557 functions, 1258900 / 4442032 bytes of code (28.34 %).
Now: 6291 / 11557, 1259800 bytes. Unit: 4/7 functions 100 % (+ __sinit), 2 resistant.

## Matched
update, startSound (ActorSoundObj_804578F4 slot 0x14, weak copy landed here), dtor, ctor, __sinit.

## Resistant
- fn_80106ADC__24WindEnvironment_8049CC98Fl 97.0 % (missing/extra): the u16 tile range test
  `subi; clrlwi 16; cmplwi 1; ble` compiles to two compares; the single-test forms add a clrlwi and make
  MWCC inline the function into update.
- init 99.7 % regalloc-only: centre x,y in f2/f0 instead of f0/f1.

## SHARED-EDITS
- include/game/ActorManager.h: ActorSoundObj_804578F4 gains `virtual JAISoundHandle* startSound(JAISoundID)` (slot 0x14, 0x80106EB0, defined here).
- config/G4SE01/gen_game.tsv: unit's row removed (sinit unchanged).

## Next
Data (vtable, sdata2) match once the two functions do; set Matching after.
