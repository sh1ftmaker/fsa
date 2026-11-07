# game/act_shooting_gallery

Baseline: total: 9151 / 11557 functions, 1936596 / 4442032 bytes of code (43.60 %); unit 4/11.
Now: total: 9152 / 11557 functions, 1937028 / 4442032 bytes of code (43.61 %); unit 5/11 (`init`, `vfunc_0x44`, `fn_8019EC04`, `draw`, `__sinit_act_shooting_gallery_cpp` match at 100%).

Decompiled:
- `ShootingGallery_804A2B18::fn_8019F3AC` (`0x8019F3AC`, size `0xEA0`): 96.36 %, `missing/extra + reloc-name` (935 vs 936 instrs; `.sdata2` literals from `fn_8019F3AC` also restored `ShootingGallery_804A2B18::init` to 100 % and emitted `jumptable_804A2B6C`).

Sub-100 % functions in unit:
- `~ShootingGallery_804A2B18`: 99.08 %, `regalloc-only` (r30/r31 swapped in loop).
- `fn_8019F3AC`: 96.36 %, `missing/extra + reloc-name` (1 fewer instruction: box setup at the top and in case 3 schedules the `mPos.x`/`mPos.y` adds and stores differently; stack slots of the by-value position temporaries for `fn_8038AD24` and `fn_801EE8F4` and loop registers in cases 0, 3, and 5 differ; `lbl_8053ABB8` is referenced via `@sda21` in the original because it is split into `act_all_enemies_defeated.cpp`).
- `fn_8019EE74`: 94.81 %, `missing/extra` (2 fewer instructions: original stores `p` before offsetting `box` and reloads `mPos.x`/`mPos.y`; `gGameRoot_8053AAF8` and `0x19660D` swap r4/r5 in `mRandom.get_ufloat_1()`, and r29/r30 for `this`).
- `update`: 99.44 %, `regalloc-only` (r4/r5 swapped for `gGameRoot_8053AAF8` vs `0x19660D` in `mRandom.get_ufloat_1()`).
- `fn_8019E7A4`: 99.23 %, `regalloc-only` (f0 and f3 swapped for `0.0f` vs `field_0x290`).
- `fn_8019E480`: 98.68 %, `regalloc-only` (r30/r31 and f28-f31 in `setRotScaleMtx`).

SHARED-EDITS: none.
