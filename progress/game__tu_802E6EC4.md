# game/tu_802E6EC4
Baseline: total: 5706 / 11557 functions, 1149900 / 4442032 bytes of code (25.89 %)
After: 5709 / 11557, 1150248 bytes.
Matched: all 5 (dtor, draw, update, init, __sinit). .text/.data/.bss/.ctors 100 %.
Not flipped to Matching: extab/extabindex 40 % (weak-dup entries not stripped even with link_check --unit evidence).
draw: the original converts a flag twice (branch to 0/1 then neg/or/srwi); an s32 local set 0/1 passed to the bool param reproduces it.
SHARED-EDITS: none. gen_game.tsv: removed both tu_802E6EC4 rows.
