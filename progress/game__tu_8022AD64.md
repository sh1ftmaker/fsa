# game/tu_8022AD64 (Player.h, G1 item 4)

Baseline 4972 functions (matching d3b2a89) -> 5002.

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
  `return;` and no final return). The getter must be a Player_804A6F38 member
  renamed in symbols.txt (`fn_X__15Player_804A6F38Fv`, s32 args mangle `l`).
- Getters live in tu_8023FBF0 and the units up to 0x8027E5BC; 100 queries
  call getters on sub-objects or in other units (call-other).
- MWCC (-inline auto) does not inline functions defined later in the TU:
  anything the target inlines from later in the file is a header/static
  inline (isPlayerActive, the flag getters in fn_8022ADFC).
- Not done: lbl_8053AC90 is not defined here; the .sbss stanza
  0x8053AC90-0x8053ACB0 also holds lbl_8053AC98/ACA0/ACA8, which other units
  (tu_8029D898, tu_802ACBE8, tu_802B1558) assign; check the split before
  defining them. __sinit (fn_8023F9B8) is left to the sinit generator.
