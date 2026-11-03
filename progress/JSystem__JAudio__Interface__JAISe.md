# JSystem/JAudio/Interface/JAISe.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4797 / 11561 functions, 863820 bytes (19.45 %). gate: ok.

Unit: 9/22 -> 22/22 functions; .text, .data, .bss, .ctors, .sdata2 100 %.
`link_check --unit` SHA-1 OK; flipped to Matching.

## Matched (all by symbols/splits, no code change)
- mixOut_, stopTrack_, startTrack_, prepare_getSeqData_ are inlined in FSA's build
  (no own symbols in the target; our compiled ctor/mixOut/prepare_ match as is).
- Vtable `__vt__5JAISe` slots name the placeholders: fn_800225C8/D0/D8 =
  getNumChild/getChild/releaseChild, fn_8002279C = asSe (slots 14-16, 19; bodies
  li r3,0 / li r3,0 / blr / blr); fn_800228B4..EC = the `@16@` this-adjust thunks
  (subi r3,r3,0x10; b <target>), named after the function each branches to.
- fn_800227A8 = `__sinit_JAISe_cpp` (the unit's .ctors entry; constructs the
  TChannelMgr/JASTrack/JASChannel/JAISe memPool_s, mkdd order).
- .bss 0x804B2288-0x804B22B8 split into this unit (the __register_global_object
  chain __sinit passes in r5); placeholder lbl_804B2288 (0x30) split into four
  0xC placeholders lbl_804B2288/94/A0/AC (one per registered pool).

## Resistant
None.

## SHARED-EDITS
None (symbols/splits edits are all inside this unit's ranges).
