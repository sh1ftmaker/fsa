# game/act_debug_warp_settings.cpp

Baseline: total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
Now:      total: 4449 / 11561 functions, 799960 / 4442088 bytes of code (18.01 %) (after rebase onto matching 3729a91)

## Status: 5/5 functions (code 716/716), NonMatching (.text/.data/.bss 100 %; extab partial)

- `DebugWarpSettings_8049C9E8` (actor DWAP, "Debug warp settings" in
  actors.tsv; named after its vtable 0x8049C9E8, stored by the spawner's weak
  ctor fn_80200310; the spawner allocates 0x240 bytes).
- Template: act_boss_display_test_npc (explicit out-of-line dtor first).
  Own members 0x230..0x23C (s32; 0x234 unused here). init: 0x238 = bits
  16-23 of field_0x90, 0x23C = bits 8-15, 0x230 = 0. update calls
  fn_801F3668(this).

## symbols.txt (placeholder renames)
- fn_8010307C -> __dt__26DebugWarpSettings_8049C9E8Fv
- fn_801030DC/E0, fn_80103100 -> draw/update/init__26DebugWarpSettings_8049C9E8Fv
- fn_80103124 -> __sinit_act_debug_warp_settings_cpp (local)
- lbl_8049C9E8 (0x58) -> __vt__26DebugWarpSettings_8049C9E8 (0x54; last word padding)
- lbl_804BB0F8 (0x100) -> @435..@443 (0xC) + lbl_804BB164..lbl_804BB1E4 (nine 0x10 weak memPool_ copies)

## Remaining
- extab/extabindex: the 9 weak JASMemPool dtors' entries (weak-dedup report
  tooling, in review). Everything else is 100 % after the rebase onto
  matching 3729a91 (shared headers v2 named the ActorBase vtable slots).

## SHARED-EDITS
- None open (the v2 headers took this unit's requests; local TODO(shared)
  declarations removed).
