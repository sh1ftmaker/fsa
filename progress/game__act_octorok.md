# game/act_octorok.cpp

Baseline: total: 9133 / 11557 functions, 1928604 / 4442032 bytes of code (43.42 %)
After: total: 9133 / 11557 functions, 1928604 / 4442032 bytes of code (43.42 %), .text 98.37 %, .rodata 100.00 %, .data 100.00 %, .bss 100.00 %.

Matched (100 %):
- `__dt__16Octorok_8049F208Fv` (96 bytes)
- `__sinit_act_octorok_cpp` (548 bytes)

Resistant:
- `init__16Octorok_8049F208Fv`: 97.41 % (`scheduling+regalloc`, 736 bytes). Prologue constant load scheduling (`@1204@sda21` vs `li r7, 0`) and register swap (`r4`/`r6` vs `r6`/`r7`) in inlined `mRandom.get_ufloat_1()`.
- `update__16Octorok_8049F208Fv`: 99.94 % (`regalloc-only`, 3024 bytes). Single `r4`/`r5` register swap in one inlined `mRandom.get_ufloat_1()` expansion (`lwz r5, 0xb4(r6); addi r4, r3, 0x660d` vs `lwz r4, 0xb4(r6); addi r5, r3, 0x660d`).
- `draw__16Octorok_8049F208Fv`: 92.33 % (`scheduling+regalloc`, 840 bytes). Instruction scheduling and register allocation in `view->mVisibleBox` struct copy and sprite index arithmetic (`field_0x98 * 4 + step + 0x446` and `lbl_8049F1C0[field_0x98 * 2 + ((field_0x234 >> 3) & 1)]`).

SHARED-EDITS:
- `include/game/act/act_octorok.h`: replaced placeholder `u8 field_0x230[0x58]` with proven member layout `0x230`..`0x288` (`s32` state/timers/counters, `f32` speed/scales, `u8 field_0x278`, `JGeometry::TVec3f field_0x27c`; total class size `0x288` preserved).
- `config/G4SE01/gen_game.tsv`: removed both `act_octorok` rows now that the unit and its header are hand-written.
