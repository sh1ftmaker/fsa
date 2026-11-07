# game/act_thwomp_launch_npc.cpp (`NPCLaunchedByThwomp_804AC010`)

## Summary
- **Actor**: `JPEN` ("NPC launched by Thwomp", vtable `0x804AC010`, size `0x284`, inherits `ActorBase_804A5770`).
- **Functions matched (100%)**:
  - `NPCLaunchedByThwomp_804AC010::~NPCLaunchedByThwomp_804AC010()` (`0x80337650`) — 100%
  - `NPCLaunchedByThwomp_804AC010::fn_803376B0()` (`0x803376B0`) — 100%
  - `__sinit_act_thwomp_launch_npc_cpp` (`0x803388BC`) — 100%
- **Functions sub-100%**:
  - `NPCLaunchedByThwomp_804AC010::fn_80337A08()` (`0x80337A08`) — 99.62%, `regalloc-only` (`field_0x250` and `field_0x25c` use `r3`/`r4` instead of `r4`/`r3`).
  - `NPCLaunchedByThwomp_804AC010::draw()` (`0x80337C88`) — 95.06%, `missing/extra` (1 extra `lwz r0, 0x250(r31)` before `field_0x250 == 2` check).
  - `NPCLaunchedByThwomp_804AC010::update()` (`0x80337DEC`) — 99.88%, `operands + reloc-name` (stack slot order between `JGeometry::TVec3f` temporaries for `field_0x3c` and the by-value `mPos` argument to `fn_8038C678`).
  - `NPCLaunchedByThwomp_804AC010::init()` (`0x80338570`) — 95.03%, `scheduling+regalloc + reloc-name` (prologue constant/literal scheduling).
- **Data sections**:
  - `.data` (`0x804ABB40..0x804AC0A8`): 100% (`lbl_804ABB40`..`lbl_804ABFD0`, vtable, and jumptable). Note: `lbl_804ABB40`, `lbl_804ABB50`, and `lbl_804ABC80` are referenced by `game/act_thwomp_launch_npc_2.cpp` and `game/act_strange_block.cpp`, so the `.data` arrays have external linkage.
  - `.rodata` (`0x80471BB8..0x80471D60`): 100% (`lbl_80471BB8`, `lbl_80471CB8`, `"enemy_both2_sob.bin"`, `"enemy_gc_sob.bin"`).
- **Symbols updated**:
  - `fn_803376B0__28NPCLaunchedByThwomp_804AC010Fv` (`0x803376B0`)
  - `fn_80337A08__28NPCLaunchedByThwomp_804AC010Fv` (`0x80337A08`)
  - Split `lbl_80471BB8` (`0x1A8`) in `.rodata` into `lbl_80471BB8` (`0x100`), `lbl_80471CB8` (`0x80`), `lbl_80471D38` (`0x14`, string `"enemy_both2_sob.bin"`), and `lbl_80471D4C` (`0x11`, string `"enemy_gc_sob.bin"`).
