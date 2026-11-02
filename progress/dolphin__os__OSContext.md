# dolphin/os/OSContext.c

Status: Matching (14/14 functions, .text/.data 100 %). No source changes.

- Matched by data/symbol fixes only:
  - `.text` split extended from 0x800417EC down to 0x80041598 (end of OSCache.c),
    so the static `__OSLoadFPUContext`, `__OSSaveFPUContext` and `OSSaveFPUContext`
    are in this unit (donor order). This fixed OSSwitchFPUContext's calls
    (target had `__OSSaveFPUContext_800416BC`). `OSLoadFPUContext` is not in
    FSA's binary (dead-stripped); the extra function in our object is harmless.
  - symbols.txt: added `__RAS_OSDisableInterrupts_begin` (0x80042638) and
    `__RAS_OSDisableInterrupts_end` (0x80042644) as labels, the `entry` points of
    OSDisableInterrupts in OSInterrupt.c (same layout in mp4 / lm-decomp
    symbols.txt). Fixed OSLoadContext (target relocs were OSDisableInterrupts+0 / +0xC).
  - `fn_80067334` -> `__cvt_fp2unsigned` (Runtime, size 0x5C, right before
    `__save_fpr` as in tww/mkdd; body is the fctiwz / addis 0x8000 converter).
  - `lbl_80495AB0` (0x1B4 blob) split into the 12 OSDumpContext format strings
    (placeholder names, data:string). Fixed OSDumpContext's r31-relative operands.
- Gate: with `--base $(git merge-base HEAD matching)` the only problems are a
  false positive: gate.py's parse_symbols keys by address, so the label at
  OSDisableInterrupts' address overwrites the function entry
  ("OSDisableInterrupts renamed / type changed"). OSDisableInterrupts itself is
  unchanged in symbols.txt.
- SHARED-EDITS: tools/gate.py should key symbols by (address, name) or skip
  `type:label` lines (same pattern as `_savefpr_14` at `__save_fpr`).
