# game/tu_8037ACC4

Baseline: `total: 6229 / 11557 functions, 1252968 / 4442032 bytes of code (28.21 %)`.
After: 6243 functions, 1253972 bytes (28.23 %). Unit: 1/16 -> 15/16.

## Matched
All but fn_8037AEAC; .data, .bss, .sbss, .ctors 100 %.

## Resistant
fn_8037AEAC (92.5 %, missing/extra): original does `mr r31, r3` in each
branch right after the two calls; ours one `mr` after the join, scheduled
after the lbl_8053AD70 load. if/else, ternary, inline getter all the same.
extab/extabindex below 100 % (likely tied to it).

## SHARED-EDITS
- splits.txt: tu_8036FB60 .sbss end 0x8053AD80 -> 0x8053AD70; tu_8037DF68 gets
  .sbss 0x8053AD78-0x8053AD80 (lbl_8053AD78, stored only there).
- symbols.txt: callee placeholders in tu_8044C7F0/tu_80453AF8 renamed to
  mangled member names (fn_8044CB00.. / fn_80453B14.. / ctors).
- gen_game.tsv: unit row removed.
- New headers: cls_804AE9B0.h, cls_804B1D58.h, cls_804B1DA8.h.
