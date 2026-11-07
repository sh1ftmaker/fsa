# chore/pool-owners2 (Opus, 2026-11-06)

Baseline (matching 0aee7d9a): 9123 / 11557. After: 9128 (+5), zero drops by address; SHA-1 OK,
gate --all-units / --base matching ok, gen_game --check ok, order_check: no unit of this branch.

## tu_801EB00C (3 -> 5 of 9)
- fn_801EB00C (push towards field_0x0, const): 97.42 % missing/extra, one frsp after __fsel (as in
  act_kaepora_gaebora). fn_801EB1F4 (turning step, now a const member): 98.21 % regalloc-only.
  fn_801EB418 (emitter ring): 89.25 % missing/extra (u8 fade flag -> bool neg/or/srwi and the alpha's
  clrlwi in the original; loop-invariant order). Pool now matches: both constructors 100 %.
- Constructors swapped into descending order (was out of order).
- Left: fn_801EB710 (unwritten; uses only existing literals).

## tu_80118384 (5 of 25, no flips)
- fn_80119628 99.88 %, fn_801194C4 99.94 % (reloc-name only).
- Pool: the original has 1.0f at 0x8 ahead of fn_801194C4's double (0x10, gap at 0xC; doubles are not
  refilled into gaps), and no function of the unit makes 1.0f before fn_801194C4: likely a dead-stripped
  function, unknown. 0.05f (0x4) is fn_80118E20's: it clears a JGadget TVector<ActorBase*> at
  cls_801192AC 0xC (alloc 0xC, begin 0x10, end 0x14, capacity 0x18, extend 0x1C: fn_8011921C's static
  init stores extend_default there; fn_801190E4 is reserve(20) with TDestructed_deallocate_). The repo's
  JGadget/vector.h is void*-only, so that needs a template fix first.
- fn_801183BC / fn_80118454 / fn_80119DD0 are reloc-name on the .sbss guard, not the pool: the original
  .sbss is lbl_8053AA90 (1-byte allocator object fn_8011921C copies), AA91 (fadeObject guard), AA92
  (fn_8011921C guard). Writing fn_8011921C with the vector static should line the guard up.

## act_hyrule_castle_barrier (5 -> 8 of 8)
- getEndPos1 / getEndPos2 / getMidPos made plain functions at the bottom of the .cpp (RULES_CARD
  "Literals before their first visible use" (3)): init, fn_8011EEEC, fn_8011F314 -> 100 %.
  (The helpers still return the address of a temporary, as before; a later cleanup.)

## Next (diag2 over all units)
- act_cannon / act_warp_cannon share a leading pool block (0.1, 1.0, 12, 0.5, 4, 0.2, 0.05): probably a
  shared dead-stripped hop helper (case 3 of Cannon::update, with the two function statics); 3 + 3 fns.
- act_icicle, act_snowball, act_snow_wall, act_great_fairy_spfr: update's literals lead the pool ahead
  of the lowest function's: look for a plain helper update inlines.
- Unwritten owners by ratio: act_sword_soldier fn_8014FD5C (330 instr, 4 fns), tu_80158624
  fn_80158624 (326, 2), tu_803F0A54 fn_803F726C (889, 8).

## SHARED-EDITS
- include/game/cls_801EB8CC.h: fn_801EB00C const, + fn_801EB1F4 (const member); comments.
- include/game/GameGlobals.h: cls_80119E5C fn_80119628 / fn_801194C4 comment.
- include/game/act/act_hyrule_castle_barrier.h: the three helpers declared, defined in the .cpp.
- symbols.txt: fn_801EB00C__12cls_801EB8CCCF..., fn_801EB1F4__12cls_801EB8CCCF... (placeholder renames).
