# game/tu_800FE8C0

Baseline: total: 5853 / 11557 functions, 1171280 / 4442032 bytes of code (26.37 %)
After: 5854 / 11557, 1171332 bytes.

Matched: fn_800FEB78__12cls_800FEB78Fi (100 %).
Resistant: fn_800FE8C0__12cls_800FEB78Fv 99.22 % regalloc-only: in the preamble
the original has `lwz r4,0(r3); lis r5,rodata@ha`, ours has r5/r4 swapped
(tried: file-scope vs local tables, swapping the compare operands, moving the
`durations = NULL` declaration up: all equal or worse).
Data: .rodata bytes identical; ours is 0xD4 long, the original split 0xD8
(4 bytes alignment tail). Tables are lbl_<addr> statics.
SHARED-EDITS: none beyond this unit (gen_game.tsv row removed; symbols.txt
renamed the two functions; new include/game/cls_800FE8C0.h).
Callers (not written): act_shadow_link_demo, act_shadow_link_2, tu_801C6A4C
(object at +0x278 of the actor).
