# JSystem/JAudio/System/JASInstSense.cpp

Extended over fn_80030B78 (0x80030B78..0x80030BD0), its own `__sinit`:
.ctors 0x8045BD68 sits right after JASInstRand's (0x8045BD64), its .bss chain
object 0x804B28B8 right after JASInstRand's, and mkdd has no file between
JASInstSense and JASSimpleWaveBank. Same as JASInstRand: FSA's version
includes JASChannel.h, so its __sinit builds the JASChannel pool.
2/2 -> 3/3, stays Matching; `link_check` SHA-1 OK.

- fn_80030B78 -> `__sinit_JASInstSense_cpp` (scope:local).
- .ctors 0x8045BD68-0x8045BD6C, .bss 0x804B28B8-0x804B28C8 (placeholder
  lbl_804B28B8 resized 0x10 -> 0xC, +4 bytes pad).
