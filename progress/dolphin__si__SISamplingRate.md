# dolphin/si/SISamplingRate.c

Baseline (progress.py first line at start):
total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1173 / 11560 functions, 212892 / 4434096 bytes of code (4.80 %)

## Status: Matching (.text, .data, .sbss 100 %)

Matched: SIRefreshSamplingRate (already), SISetSamplingRate (new): 1/1 -> 2/2.

- .text start moved 0x80075508 -> 0x80075424: SISetSamplingRate (already a
  real symbol, size 0xE4) sat in auto split auto_03_80075424 between SIBios's
  end and this unit; it references XYNTSC and SamplingRate (this unit's
  statics), so the auto split had globalized them as *_804984F8 / *_8053A790.
- lbl_80498528 (0x68) split into XYPAL (0x30, the 12 {line,count} PAL entries
  {296,2},{15,21},...,{156,2} = donor table, referenced as XYNTSC+0x30) and
  lbl_80498558 (0x33, the OSReport string
  "SISetSamplingRate: unknown TV format. Use default.").
- No source change; configure.py flag -> Matching.

## SHARED-EDITS
symbols.txt: lbl_80498528 -> XYPAL + lbl_80498558 (placeholder split only).
