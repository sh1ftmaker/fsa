# chore/pool-owners (Opus, 2026-11-06)

Baseline (matching bd727044): 9112 / 11557 functions. Leverage task: write the unwritten functions
that own a unit's first .sdata2 literals so the reloc-name-only functions after them flip.

## tu_801213EC (EscortBase_8049E0C8)
- fn_80121438 (circle push, 'CIRC'): 99.76 %, reloc-name only.
- fn_801217EC (life meters): 99.69 %, regalloc-only (+ reloc-name); with dead-stripped plain helpers
  respawnLifeMeters / spawnLifeMeter (proven by the redundant field_0x332 / index tests inlined into it).
  LifeMeter_804B174C::fn_8019CEF8's stores are a file-local copy (followActor): the original inlines a
  header inline whose out-of-line copy is tu_80197070's; move it to act_life_meter.h once tu_80197070's
  fn_80198828 (its out-of-line caller) is written.
- Pool now matches from 0x20. Still blocked: slot 0 (8.0f) comes from a function not in the binary that
  is emitted before fn_80121438 (no candidate found: spawnLifeMeter at the bottom pools 16, 8, double);
  slot 0x40 (10430.378f) likewise sits between fn_80121F34 and fn_80121FA8. No flips.

## act_kaepora_gaebora
- vfunc_0xA0: 90.52 % missing/extra (__fsel; the original stores the fsel result before rounding it).
  Pool matches through 0x28: vfunc_0x88, vfunc_0x9C, vfunc_0x94 -> 100 %.

## tu_801CB270
- fn_801CB85C: 96.55 % scheduling+regalloc (rotation y before x).
- fn_801CC350: 100 % (C_VECReflect declared in libs/sdk mtx.h).
- tileCenter: dead-stripped plain helper (4 identical copies in fn_801CCD34 / fn_801CCCC4; its 8.0f
  sits between fn_801CC350's and fn_801CC9F8's literals). Whole-file auto_inline off replaced by a
  rule-1 region around fn_801CD9F0 / fn_801CD970; call lists checked equal to the original's.
- fn_801CC9F8, fn_801CCB1C: 100 %. Flipped: fn_801CD9F0, fn_801CCD34, fn_801CCCC4. 23 -> 30 of 51.

## Not done
- tu_801EB00C: needs fn_801EB00C (fsel again), fn_801EB1F4 and fn_801EB418 (~400 instructions) for
  two constructors. tu_80118384: fn_80118E20 / fn_801194C4 / fn_80119628 (~470 instructions).

## SHARED-EDITS
- libs/sdk/include/dolphin/mtx.h: + C_VECReflect declaration (SDK function at 0x80047130).
- include/game/EnemyBase.h: fn_801CC350, fn_801CC9F8 declared with C++ linkage (now defined).
- include/game/GameGlobals.h: fn_801CCB1C declared with C++ linkage.
- symbols.txt: fn_80121438, fn_801217EC, fn_801CB85C, fn_801CC350, fn_801CC9F8, fn_801CCB1C mangled.
