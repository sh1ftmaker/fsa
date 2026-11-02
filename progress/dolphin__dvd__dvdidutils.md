# dolphin/dvd/dvdidutils.c

Baseline: total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
Final:    total: 1173 / 11560 functions, 212912 / 4434096 bytes of code (4.80 %)

Matched 1/1 (DVDCompareDiskID); unit set Matching. No source changes.

- Only diff was the call target: fn_8006D7E8 -> strncmp (global). Evidence: it sits in MSL
  string.c between strchr (0x8006D7B8) and strncpy, and its body is exactly MSL's strncmp
  (p1/p2 = str-1, n++, `while (--n)` with lbzu, `return c1 - c2`, break on !c1, return 0;
  marioparty4 MSL_C string.c). DVDCompareDiskID calls it with n=4 and n=2 like every donor.
- No data sections in this unit.

SHARED-EDITS: symbols.txt rename of fn_8006D7E8 -> strncmp lives in the MSL string.c range
(not this unit's code); later MSL string.c / other strncmp callers benefit.
