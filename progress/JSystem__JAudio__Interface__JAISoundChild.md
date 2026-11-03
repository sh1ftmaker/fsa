# JSystem/JAudio/Interface/JAISoundChild.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4785 / 11561 functions, 863732 bytes (19.44 %). gate: ok.

Unit: 3/4 -> 4/4 functions; .text, .ctors, .bss, .sdata2 100 %.
`link_check --unit` SHA-1 OK; flipped to Matching.

## Matched
- fn_80025F54 = `__sinit_JAISoundChild_cpp` (the unit's .ctors entry; constructs the
  TChannelMgr/JASTrack/JASChannel/JAISoundChild memPool_s, mkdd order).
- .bss 0x804B2380-0x804B23B0 split into this unit (the __register_global_object
  chain); placeholder lbl_804B2380 (0x30) split into four 0xC placeholders
  lbl_804B2380/8C/98/A4.

## Resistant
None.

## SHARED-EDITS
None.
