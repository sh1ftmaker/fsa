# dolphin/card/CARDBlock.c

Baseline (tools/progress.py first line at start):
total: 1149 / 11560 functions, 207532 / 4434096 bytes of code (4.68 %)

Status: Matching. 5/5 functions, .text 100 % (872 bytes); no data sections.

No source changes were needed. The two misses were unnamed placeholders for the
file's static callbacks, which the existing source (donor CARDBlock.c) already compiles
to identical bytes:
- symbols: fn_80054A50 -> WriteCallback (local, 0xD4), fn_80054B24 -> EraseCallback (local, 0xC8).
  Evidence: they sit between __CARDGetFatBlock and __CARDAllocBlock in the donor's order and
  match byte-for-byte (fdiff 100 %); EraseCallback is the callback passed by __CARDUpdateFatBlock.
- configure.py: CARDBlock.c -> Matching.

Note: __CARDFreeBlock is in the source but not in FSA (dead-stripped; CARDDelete isn't linked).
Kept, as in the original file.

SHARED-EDITS: none.
