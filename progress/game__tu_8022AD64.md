# game/tu_8022AD64.cpp checkpoint

Baseline (progress.py): total: 7123 / 11557 functions, 1392832 / 4442032 bytes of code (31.36 %); unit 214/373.
Now: total 7187 / 11557, 1402608 bytes (31.58 %); unit 278/373.

## Matched this session (64)
- 28 + 13 forwarders / all-players loops that are identical to a matched template (checked by normalised asm):
  callee placeholders renamed to Player_804A6F38 members in symbols.txt.
- Field getters (new Player.h fields 0x14, 0x2c4, 0x43f, 0xb18, 0xb78, 0xb9f, 0xcc8, 0xcd4, 0xce0; arrays
  field_0x38[0x30] TVec3f (48 = fn_80252CF0's count) and field_0x284[0x34] u8 (extent guessed)).
- fn_80230B70 / fn_80230CFC use `isPlayerActive(player) == false` (materialised bool shape, RULES_CARD).
- fn_8023F728 / fn_8023F78C (boot path, called by fn_8013A1A4), setters fn_80230BF4, fn_8023A654, fn_8023E690.

## gen_game.py
Could not be run for this unit: it refuses units claimed in progress/claims.json (ours is). The 41 template
twins were therefore hand-added with a scratch script (not committed); gen_game.py --check still passes.
Proposal: let gen_game accept the claimant's own unit so the remaining twins can join the manifest.

## Not done (95 functions, no STOP reached, out of time)
Larger functions (>50 instrs) and the singular small ones: fn_8022FE80, fn_80233C9C, fn_80238430
(returns &field_0x324 TVec2f vs a TVec3f fallback), fn_8023F984 (weak inline copy), PlayerManager ctor/dtor,
fn_8023F9B8 (__sinit, 0x238, not the 9-pool template).

## SHARED-EDITS
include/game/Player.h (members, fields, free decls moved out of extern "C" blocks, void->bool for
fn_8024EFA0 / fn_80272C20 / fn_80230E1C), include/game/GameGlobals.h (fn_8023F728/78C moved out of extern "C",
extern s32 lbl_8053A190).
