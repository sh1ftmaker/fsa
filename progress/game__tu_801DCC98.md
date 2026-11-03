# game/tu_801DCC98

Baseline: total: 5839 / 11557 functions, 1169096 / 4442032 bytes of code (26.32 %)
After: total: 5842 / 11557 functions, 1169564 / 4442032 bytes (26.33 %)

Class cls_804A5038 (EnemyBase_804A4808 child, vtable 0x804A5038; include/game/act/tu_801DCC98.h).

Matched: init, ~cls (0x801DCE9C), ctor, __sinit.
Resistant: update (0x801DCC98), 99.58 %, regalloc-only: the position-in-box test
keeps mPos.x in f2 / mPos.y in f1 in the target, ours f1 / f2. Tried: locals x,y (96.8 %),
swapped upper-bound compare operands (99.3 %, wrong fcmpo operand order).
Remaining unit gaps: extab/extabindex 47.6 % (ours also holds the sinit's weak JAS pool
functions' entries; same on other generated units).
SHARED-EDITS: none (gen_game.tsv row for tu_801DCC98 deleted; symbols.txt renames).
