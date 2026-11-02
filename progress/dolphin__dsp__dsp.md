# dolphin/dsp/dsp.c

Baseline (progress.py first line at start):
total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
After: total: 1172 / 11560 functions, 212664 / 4434096 bytes of code (4.80 %)
(progress.py already counted all 6 functions; DSPInit was 99.69 % in fdiff.)

## Status: Matching (.text, .data, .sdata, .sbss 100 %)

All 6 linked functions match: DSPCheckMailToDSP, DSPCheckMailFromDSP,
DSPReadMailFromDSP, DSPSendMailToDSP, DSPAssertInt, DSPInit.

- DSPInit's only miss: its three string relocations pointed at
  lbl_804968E8+0x48/+0x68/+0x74. lbl_804968E8 (0x80) was dtk's blob over four
  strings; split into lbl_804968E8 (0x45, the __DSPVersion string),
  lbl_80496930 (0x1E, "DSPInit(): Build Date: %s %s\n"), lbl_80496950 (0xC,
  "Apr 17 2003"), lbl_8049695C (0x9, "12:34:16"); placeholders resized/split only.
- No source change. DSPReset, DSPHalt, DSPGetDMAStatus, DSPAddTask are
  dead-stripped in FSA (they use no data); they stay in the source like the
  donors. __DSP_tmp_task was already named.
- configure.py flag -> Matching.

## SHARED-EDITS
symbols.txt: lbl_804968E8 (0x80) split into 4 string placeholders (above).
