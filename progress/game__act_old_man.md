# game/act_old_man (cls_804AEED0, JIJI)

Baseline: 7964 / 11557 functions. After: 7985 (21/22 functions at 100%, 1 at 99.56% regalloc-only).

Matched (100%):
- `__sinit_act_old_man_cpp` (0x8037CE5C, 2344 bytes): 54-entry PTMF table `lbl_804AEC48` initialised from `.data` `@704`..`@757`.
- `cls_804AEED0::cls_804AEED0()` (0x8037CE00, 92 bytes)
- `cls_804AEED0::~cls_804AEED0()` (0x8037CD78, 136 bytes)
- `cls_804AEED0::init()` (0x8037C788, 1520 bytes)
- `cls_804AEED0::update()` (0x8037BC30, 2904 bytes)
- All 16 step/timer and helper methods (`fn_8037B374`..`fn_8037B7EC`)

Sub-100%:
- `cls_804AEED0::draw()` (0x8037B7F8, 1080 bytes): 99.56%, `regalloc-only` (`target->ours r8->r7, r7->r8, r7->r0, r0->r8` on the `field_0x25c + lbl_80475498[field_0x264][field_0x248] + field_0x238` addition).

Sections: `.ctors`, `.rodata`, `.data`, `.bss`, `.sdata`, `.sdata2` all 100.00%; `.text` 99.95%.

SHARED-EDITS:
- `include/game/act/tu_801F9484.h`: `cls_804AEED0` layout (`0x230`..`0x278`, size `0x27C`), virtuals `init`/`update`/`draw`, step methods `fn_8037B374`..`fn_8037B7EC`, dead-stripped helper `updateFacing(s32)`.
- `config/G4SE01/symbols.txt`: member-mangled renames for `cls_804AEED0` methods, `fn_8037CE5C` -> `__sinit_act_old_man_cpp`, and split `lbl_804AE9C0` into `@704`..`@757` (54 PTMF initialiser entries).
