# dolphin/gx/GXPerf.c

Matched: GXSetGPMetric, GXClearGPMetric, GXReadXfRasMetric (3/3 code, 100 %).

Fix: GXSetGPMetric's two switch jump tables (jumptable_80497858,
jumptable_804978B4 in symbols.txt) were not split into this unit. Added
`.data start:0x80497858 end:0x80497944` to splits.txt; the compiled @414/@413
land in the same order and contents.

Resistant: .data is 83.69 %, not code. Our object also contains @520 (0x5C
bytes), the jump table of GXReadGPMetric, a function FSA's linker dead-stripped
(text is contiguous with only 3 functions; 0x80497948 starts another unit's
data). The stripped function's table is gone from the DOL, so objdiff sees a
92-byte insert. Source is the real SDK source; removing GXReadGPMetric to
force the data match would be a trick, so the unit stays NonMatching.

SHARED-EDITS: none.
Next: a policy decision on dead-stripped jump tables (tooling/report setting)
would let this unit be marked Matching.
