# dolphin/pad/Padclamp.c

Baseline: total: 1149 / 11560 functions, 207532 / 4434096 bytes of code (4.68 %)
After:    total: 1150 / 11560 functions, 207956 / 4434096 bytes of code (4.69 %)

## Matched (4/4, unit flagged Matching)
ClampStick, ClampCircle, PADClamp, PADClampCircle; .text/.rodata/.sdata2 100 %.

## What was wrong
No code changes. ClampCircle's sqrtf pool (0.0f, 0.5, 3.0, int->double magic at
0x8053B328..0x8053B348) sat in auto_11_8053B308_sdata2. Added the .sdata2 range
to the unit's split and named the pool entries @88/@89/@90/@92 (scope:local, as
mwcc emits them). Renamed lbl_8045D440 -> ClampRegion (static, size 0xA; the
remaining 6 bytes are alignment padding): referenced by PADClamp/PADClampCircle,
bytes 1E B4 0F 48 28 0F 3B 1F 38 2C match the donor ClampRegion values.
sqrtf's _half/_three localstatics are emitted by our compile but unreferenced
(dead-stripped by the linker in the original) - not a mismatch.

## SHARED-EDITS
None (splits.txt stanza and symbols.txt placeholders only).

## Notes
Full `ninja` link fails on PPCArch multiply-defined symbols (PPCMfmsr etc.) -
unrelated to this unit.
