# chore/pool-owners3 (Opus, 2026-11-06)

Base matching fceaffb8: 9133 / 11557. After: 9141 (+8), zero drops by address (no function's % falls);
SHA-1 OK, gate --all-units / --base matching ok, gen_game --check ok, order_check: none of this branch's
units (tu_80158624 was out of order and is fixed).

## act_icicle (2 -> 5 of 8)
- fall(): the dead-stripped step update's state 1 inlines (`t += 0.1; y = spawn.y + 0.5 * 9.8 t^2`), a plain
  member at the bottom: its 0.1f, 0.5f, 9.8f lead the pool. ct, init, fn_8010B40C -> 100 %. update still
  inlines it (no bl), 70.00 -> 70.13 %.

## act_snow_wall (2 -> 5 of 6)
- melt() (case 0 after the fire test: 16.0f, 1.0f) and shrink() (case 1: 0.5f, -0.005f, 0.1f, -0.05f) as
  plain members below draw; update inlines both. ct, init, update -> 100 %. The whole switch as one helper
  does not inline (update 17 %). Where the original drew the helpers' edges is not proven: these are the
  case bodies that hold the literals. Reviewer call.

## tu_80158624 (5 -> 7 of 10)
- fn_80158624 written: the searchlight beam (ShadowCircle member; callers act_searchlight, tu_8018A8D8),
  96.86 % missing/extra (one shared `li` zero; registers, a few load orders, stack slots, the matrix's
  zero-store order). The two rim loops are one static inline drawRing (two identical inlined copies).
  `2.0f * (i * PI) / 32.0f` and the matrix's 1.0f before its zeros give the pool's literal order.
- drawShadow_80158B5C, fn_80158E88 -> 100 %. File reordered to descending addresses.

## Looked at, not done
- act_cannon / act_warp_cannon: the leading block is not one hop helper. Cannon's pool needs everything of
  update's (sin, hop, rise, owner search, 1.5f, swing) plus init's int->float double and fn_80107360's
  16.0f ahead of fn_80107360; warp's needs update's set plus init's 16.0f. No proven body. act_cannon's
  source is also out of descending order; reordering it moves draw to 99.96 (a drop), so left as is.
- act_snowball, act_great_fairy_spfr: the pool splits into 4-6 runs interleaving the lowest function's and
  update's literals; would need several single-copy helpers (case bodies of fn_8010C294 / update). Not proven.
- act_sword_soldier: fn_8014FD5C only adds 0.0f / 1.0f; the 4 reloc-name functions sit behind
  fn_80150284 (3276 instr, unwritten). Not a lever.
- tu_801213EC: the leading 8.0f still has no owner (spawnLifeMeter's double comes after fn_80121438's run).
- tu_80118384: 1.0f at 0x8 could be the 0x3EC four-emitter block (two identical copies, fn_80119998 case 0
  and fn_80119628 case 2) as a plain helper between fn_80118E20 and fn_801194C4, but 0.05f needs
  fn_80118E20 first (static fade object + TVector erase; JGadget vector template).

## SHARED-EDITS
- include/game/act/act_icicle.h: + Icicle_8049CFC8::fall().
- include/game/act/tu_801F9484.h: + cls_8049E8A0::melt(), shrink().
- include/game/GameGlobals.h: + ShadowCircle_804A0160::fn_80158624 declaration.
- config/G4SE01/symbols.txt: fn_80158624 -> fn_80158624__21ShadowCircle_804A0160FPCQ29JGeometry8TVec3<f>PCQ29JGeometry8TVec3<f>Q28JUtility6TColorf.
