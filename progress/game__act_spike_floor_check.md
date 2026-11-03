# game/act_spike_floor_check

Baseline: total 5819 / 11557 functions, 1165268 bytes (26.23 %). After: 5822, 1166120.

Matched: 5/5 (dtor, draw, update, init, generated sinit). No resistant functions.
update: `mTimer--; if (mTimer == 0)` (not `--mTimer`) and `case 1: default:` for the
type switch reproduce the original compare sequence.

SHARED-EDITS:
- include/game/Map.h: cls_804AA858::fn_802FA160(void* res, u32 srcOffset, s32 slot,
  u32 dstOffset, u32 size) (asm: r3 = lbl_8053AB10->0x20, r4 res, r5 src offset, r6 slot*4
  index, r7 dst offset, r8 size, 0 = getResSize).
- config/G4SE01/symbols.txt: fn_802FA160 -> fn_802FA160__12cls_804AA858FPvUllUlUl.
- config/G4SE01/gen_game.tsv: removed both act_spike_floor_check rows.

Next: set Matching (coordinator).
