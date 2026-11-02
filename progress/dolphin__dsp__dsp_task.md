# dolphin/dsp/dsp_task.c

Baseline (progress.py first line at start): total: 1132 / 11560 functions, 202096 / 4434096 bytes of code (4.56 %)
After: total: 1134 / 11560 functions, 202688 / 4434096 bytes of code (4.57 %)

Status: Matching. 4/4 functions, .text/.data/.sbss 100 %. gate: ok.

## What was wrong (no code changes)
- `fn_80051E04` -> `__DSP_debug_printf`: empty varargs stub (stwu -0x70, saves r3-r10/f1-f8),
  called by __DSP_boot_task with the "DSP is booting task" format strings; dsp_debug.c in every donor.
  (dsp_debug.c is not a configured unit; it stays in auto split auto_03_80051E04_text.)
- `lbl_8053A688` -> `__DSP_tmp_task`: DSPInit (dsp.c) stores tmp first in the donors'
  `__DSP_first_task = __DSP_last_task = __DSP_curr_task = __DSP_tmp_task = NULL` chain
  (FSA dsp.s: 8AC8, 8AD4, 8ACC, 8AD0); sits right before last/first/curr in .sbss.
- `lbl_80496968` (0x140 blob) split into the 7 debug strings @266..@271, @294 (same form as EXIBios/vi).
- splits.txt: added `.sbss 0x8053A688..0x8053A698` (tmp, last, first, curr).

## Notes
- FSA links JAudio's osdsp_task.c `__DSPHandler`, so dsp_task.c's weak `__DSPHandler`, `__DSP_add_task`,
  `__DSP_rude_task`, `__DSP_rude_task_pending` are dead-stripped in the DOL; they stay in the source like the donors.
  No jump table / extra .data in our object (.data 0x13B vs 0x140 padding only).

## SHARED-EDITS
- symbols.txt rename `lbl_8053A688` -> `__DSP_tmp_task` is referenced by dsp.c (DSPInit), owned elsewhere.
- symbols.txt rename `fn_80051E04` -> `__DSP_debug_printf` (dsp_debug.c, unconfigured unit).
