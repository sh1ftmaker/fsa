# shape-sweep2 (Opus, 2026-11-06)

Cross-file sweep of the forms proven since sweep 1 (ActorBase / GameView round 4) over every
sub-100 game function: objdiff target vs ours scanned for loads off `this` above the prologue
saves, cntlzw/extrwi vs srwi, 0x2020 resource types, rodata-initialised aggregates, member loads
before calls; plus the matching FIXMEs. Claimed units and tu_8022AD64 skipped.

+8 at 100 % (no function anywhere drops, by address against the main report):
- act_octorok_rock vfunc_0x4C 68.69, act_dropped_item vfunc_0x4C 85.12 (tile-code != chain,
  act_bombchu's form);
- tu_801CDC8C fn_801CF0F4 89.70, tu_8020CCA8 fn_8020D2AC 94.47, tu_801DAC6C fn_801DAD2C 91.30 /
  fn_801DACAC 81.19 (const);
- tu_801DAC6C fn_801DAE24 86.85, tu_80215A94 fn_8021606C 91.62 (const + the loop tests the member).
Raised (FIXMEs updated): act_recovery_heart 61.51 -> 99.96, act_small_key fn_802C1970 85.34 -> 98.52,
act_somaria_block 57.54 -> 96.79, tu_8017A44C UnknownSWBM vfunc_0x4C 72.66 -> 98.97,
act_two_sword_motion 84.07 -> 92.08, tu_802E2074 init 86.53 -> 96.17 (resource type), tu_801ED3DC
fn_801ED828 79.80 -> 90.12, tu_801CDC8C fn_801CF144 61.74 -> 75.30, tu_8023FBF0 fn_80240DCC 86.20 -> 92.26.

SHARED-EDITS: const on EnemyBase.h fn_801CF0F4 / fn_801CF144, act/tu_801DE714.h fn_8020D2AC,
act/tu_801DAC6C.h fn_801DACAC / fn_801DAD2C / fn_801DAE24, tu_801ED3DC.h fn_801ED7B0 / fn_801ED828,
tu_80215A94.h fn_8021606C, Player.h fn_80240818 / fn_80240DCC (+ fn_80270ABC comment); symbols.txt
renames to ...CF... for each (placeholders). Layouts unchanged.

Not generalising (RULES_CARD): Player fn_80270ABC is 100 % const but its callee fn_80247BD4 const
drops fn_8024762C (99.8 -> 99.2), left plain; act_icicle fn_8010B284 const 97.4 -> 84.2; `return a ||
b` for fn_80240DCC / fn_801D08B4; getField_0x4() in act_thief / tu_8021775C / act_pot: no change.
