# game/tu_8022AD64 (Player.h, G1 item 4)

Baseline 4985 functions (matching fb9ae32) -> 5015 (merged as 910c8d5).

## Matched
- tu_8022AD64 (26): flag accessors fn_8022ADB0..fn_8022B260 and fn_8022ADFC;
  fn_8023F264 (the guard); query templates fn_8022B7C4 (getter call),
  fn_8022B860 (void setter call, one argument), fn_8023DE58 (field),
  fn_8023E724 (field address, fallback &lbl_804CF460).
- tu_801CB270: fn_801CCF34, fn_801CD228, fn_801CD290.
- tu_801EE24C: fn_801F329C.

## Resistant
- tu_801EE24C fn_801F320C 94.2 % (scheduling): lwz field_0x4 issued after
  `mr r28, r3` instead of right after the prologue's `stw r31`; cmpw operands
  swapped. 7 tries (budget 6).

## Notes for G2 (query generator)
- Shapes (exact, after normalising call targets): 107 x "bl getter; return
  r3, fallback li r3", 23 x the same with one extra int argument, 19 + 17
  void variants, 7 x two extra args, 7 x fallback -1; 333 functions contain
  the guard, 37 of them more than once (loops/compound queries, by hand).
- Template: `if (lbl_8053AC90 == NULL) return F; if (isPlayerActive(player))
  return lbl_8053AC90->mPlayers[player]->getter(args); return F;` (void:
  `return;` and no final return).
- Inverted layout (fn_80230B70, fn_80230CFC): the null test's `bne` skips
  `li r3, F; blr`, then the guard's `bne` skips a second `li r3, F; blr`,
  and the field load comes last. Source: `if (lbl_8053AC90 == NULL) return
  F; if (!isPlayerActive(player)) return F; return mPlayers[player]->field;`.
  Tell it from the main template by the `bne` after `clrlwi. r0, r0, 24`
  (the template has `beq` to the fallback).
- Missing final return (fn_802340A0): the null path returns F, but after a
  failed guard the code branches straight to the epilogue with no store to
  r3, which still holds the index. The source has no final `return`; the
  function is not void (its caller uses r3). Emit the template without the
  last `return F;` (a scratch compile gives this exact code with only the
  warning "return value expected"), never `return 0;` or void.
- The getter must be a Player_804A6F38 member renamed in symbols.txt
  (`fn_X__15Player_804A6F38Fv`, s32 args mangle `l`).
- Getters live in tu_8023FBF0 and the units up to 0x8027E5BC; 100 queries
  call getters on sub-objects or in other units (call-other).
- MWCC (-inline auto) does not inline functions defined later in the TU:
  anything the target inlines from later in the file is a header/static
  inline (isPlayerActive, the flag getters in fn_8022ADFC).
- Not done: lbl_8053AC90 is not defined here; the .sbss stanza
  0x8053AC90-0x8053ACB0 also holds lbl_8053AC98/ACA0/ACA8, which other units
  (tu_8029D898, tu_802ACBE8, tu_802B1558) assign; check the split before
  defining them. __sinit (fn_8023F9B8) is left to the sinit generator.

## SHARED-EDITS (this branch owns Player.h; listed for the record)
- include/game/Player.h (new): PlayerManager_804A6F28 (0x74) and
  Player_804A6F38 (0x13BC) layouts, isPlayerActive, 64 extern "C"
  declarations. Evidence per field and declaration in the header.
- include/game/GameGlobals.h: includes game/Player.h; the per-player block
  (fn_8023F264, fn_8023DE58, fn_8023E724, fn_802397B4) and the
  fn_8022ADE4/fn_8022ADCC block moved to Player.h with the same types.
- config/G4SE01/symbols.txt: placeholder renames only (both classes'
  ctor/dtor/vtable, fn_8023F984/fn_8023FC08/fn_8023FC10 members, the three
  tu_801CB270 functions' `__Fl`).
- Follow-up (infra/g1-player-nits): fn_8023CA88/fn_8023CB24 return s8, not
  u8 (callers extsb; extern "C", no symbol change); fn_802340A0's comment.

# Wave-boot session (2026-11-05)

Baseline (progress.py): total: 7123 / 11557 functions, 1392832 / 4442032 bytes of code (31.36 %); unit 214/373.
Now: total 7187 / 11557, 1402608 bytes (31.58 %); unit 278/373.

## Matched this session (64)
- 28 + 13 forwarders / all-players loops that are identical to a matched template (checked by normalised asm):
  callee placeholders renamed to Player_804A6F38 members in symbols.txt.
- Field getters (new Player.h fields 0x14, 0x2c4, 0x43f, 0xb18, 0xb78, 0xb9f, 0xcc8, 0xcd4, 0xce0; arrays
  field_0x38[0x31] TVec3f and field_0x284[0x31] u8 (counts from fn_80252CF0 shifting entries up: last stores at 0x278 and 0x284)).
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
