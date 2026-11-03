# game/act_switch_or
Baseline: total: 5708 / 11557 functions, 1150032 / 4442032 bytes (25.89 %)
After: 5711 / 11557, 1150388 bytes.
Matched: 5/5 functions (init, update, draw, dtor, sinit). Code 1000/1000.
Data 380/440: extab/extabindex 40% (our object carries weak-duplicate extab entries from the memPool dtors; not code). Left NonMatching.
update: switch(mState){case 0 ... case 1: default: ...} needed for the branch layout.
SHARED-EDITS: none (include/game/act/act_switch_or.h is this unit's own header; gen_game.tsv rows for act_switch_or deleted).
