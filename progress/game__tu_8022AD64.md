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
