# JSystem/JAudio/System/JASAudioThread.cpp

Status: **Matching** (7/7 functions, every section 100 % in the honest report), `link_check.py --unit`
and the full `ninja` link SHA-1 OK. New unit, found by the JASAudioReseter review.

- Source: mkdd's own JASAudioThread.cpp (CC0, already imported). One change: the constructor is
  `inline`, as in tp/SMG; FSA's create (0x8002A740) inlines it and has no out-of-line copy.
- Splits: .text 0x8002A740-0x8002AAF8, .ctors 0x8045BD44-0x8045BD48, .rodata 0x8045CB28-0x8045CB40
  ("UPDATE-DAC"), .data 0x804918D0-0x804918E0 (vtable, referenced by create and the dtor),
  .bss 0x804B2530-0x804B2540 (the JASChannel pool's __register_global_object node),
  .sbss 0x8053A318-0x8053A320 (snIntCount + the unreferenced duplicate pool guard, as JASChannel's),
  .sdata2 0x8053B0B8-0x8053B0C0 ("SFR_DSP", loaded by run).
- Names from the code: run/DMACallback/DSPCallback (passed to initAI/boot), the dtor (vtable slot),
  __sinit (.ctors), __vt__14JASAudioThread. Placeholder lbl_804B2530 resized 0x10 -> 0xC (as
  JASBasicBank's node); placeholder lbl_8053A31C removed (the map shows the object's .sbss is
  snIntCount plus the 1-byte duplicate guard, which the link keeps in auto_10_8053A280_sbss).
