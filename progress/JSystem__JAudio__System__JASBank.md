# JSystem/JAudio/System/JASBank.cpp

**Update (after review): Matching, 3/3.** Rebased onto matching (renames approved in
approved_symbol_changes.txt). noteOn/noteOnOsc take an `int` program number (`>= 0xf0`), header and
symbols.txt renamed; all sections 100 %, `link_check --unit` SHA-1 OK, gate: ok. JASTrack's callers stay 100 %.
The notes below describe the state before the rename.

Baseline (w10-jas start): progress.py total 4784 / 11561, 863464 bytes; unit 1/3 (noteOn 99.64 %, __sinit missing).
Now: 2/3, .bss/.ctors/.rodata/.sdata2 100 %, .text 99.75 %; NonMatching (noteOn needs a reviewed rename).
gate: ok. Total 4785, 863552 bytes.

## Done
- fn_80031844 -> __sinit_JASBank_cpp (local; .ctors 0x8045BD6C; JASChannel pool, as in our object).
- .bss 0x804B28C8-0x804B28D4: the pool's destructor-chain node, lbl_804B28C8 (0x10 placeholder incl.
  padding) -> @381, 0xC.

## Resistant
- noteOn (99.64 %), blocked by a symbol rename (SHARED-EDIT below), not by codegen. Diff:
  `cmpwi r4, 0xf0; blt` (FSA) vs `cmplwi r4, 0xef; ble` (ours): FSA compares the program number signed.
  With tp's/SMG's signature (`int` program number, `if (prog >= 0xf0)`) noteOn and noteOnOsc are 100 %,
  and the unit is 3/3, all sections 100 %, `link_check --unit` SHA-1 OK. The gate refuses it only
  because the two real names change. JASTrack's two callers stay 100 % with the header change.

## SHARED-EDITS
- Rename (wrong mangled signature; evidence: FSA's signed compare, tp GZ2E01 symbols.txt:11518-11519 and
  SMG RMGE01 symbols.txt:1521/39422 both `i`):
  noteOn__7JASBankFPC7JASBankUlUcUcUsPFUlP10JASChannelPQ26JASDsp8TChannelPv_vPv ->
  noteOn__7JASBankFPC7JASBankiUcUcUsPFUlP10JASChannelPQ26JASDsp8TChannelPv_vPv;
  noteOnOsc__7JASBankFUlUcUcUsPFUlP10JASChannelPQ26JASDsp8TChannelPv_vPv ->
  noteOnOsc__7JASBankFiUcUcUsPFUlP10JASChannelPQ26JASDsp8TChannelPv_vPv.
  Source to go with it: JASBank.h `noteOn(const JASBank *, int, ...)`, `noteOnOsc(int, ...)`;
  JASBank.cpp `int instId`, `if (instId >= 0xf0)`, `noteOnOsc(int wavePtr, ...)`. Then flip to Matching.
