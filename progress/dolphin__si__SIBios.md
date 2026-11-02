# dolphin/si/SIBios.c

Status: Matching. 24/24 functions, .text/.data/.bss/.sdata/.sbss all 100 %.

No source changes were needed; all misses were splits/symbols:
- splits: added .sbss 0x8053A780-0x8053A790 (the two function-static
  cmdTypeAndStatus words and __PADFixBits, which SIBios.c defines).
- symbols:
  - fn_80074B24 -> AlarmHandler (local): its body is the donor AlarmHandler
    (alarm - Alarm, Packet[chan], __SITransfer), and SITransfer passes its
    address to OSSetAlarm.
  - .bss gap after RDSTHandler split into InputBufferValid (0x804B9A48),
    InputBuffer (0x804B9A58), InputBufferVcount (0x804B9A78) and
    cmdFixDevice$327 (0x804B9A88), per the mkdd/tp declaration order and
    sizes (cmdFixDevice$327 is the name in mkdd's and tp's maps). Without them
    objdiff resolved Packet+0x1B0.. offsets to different symbols, failing
    SIInterruptHandler, SIEnablePollingInterrupt, SIGetResponse(Raw) and
    GetTypeCallback.
  - lbl_80498448 (SIGetTypeString's string literals) resized 0xB0 -> 0xA9 so
    the 7 bytes of alignment padding before SISamplingRate's .data are a gap,
    not part of the strings.

Note: symbols.txt has cmdTypeAndStatus$371 (from the initial import) while
mkdd, tp, tww and ttyd maps all say cmdTypeAndStatus$372 and our compile
emits $372. objdiff matches it anyway; left as is (real-name rule).

SHARED-EDITS: none.
