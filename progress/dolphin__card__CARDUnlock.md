# dolphin/card/CARDUnlock.c

Status: Matching (6/6 functions, .text/.data/.sdata 100 %).

- Matched: bitrev, DummyLen, ReadArrayUnlock, __CARDUnlock, InitCallback, DoneCallback.
- Fix: the only diff was the relocation to `__CARDBlock` (bss 0x804B5BA0, size 0x220
  = 2 x sizeof(CARDControl)), named `lbl_804B5BA0` in symbols.txt. Renamed it to
  `__CARDBlock`; no source changes were needed.
- SHARED-EDITS: none. Note for the CARDBios.c owner: `__CARDBlock` is defined there;
  its .bss range is not yet split into CARDBios.c.
