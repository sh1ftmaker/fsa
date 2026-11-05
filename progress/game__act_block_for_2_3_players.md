# game/act_block_for_2_3_players.cpp

Baseline (progress.py first line at start): total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
After (rebased on 3729a91, gate ok): total: 4448 / 11561 functions (see progress.py)

## Status: 4/4 functions (code 764/764), NonMatching (extab blocked)

TMI2 actor ("Block for 2-3 players", actors.tsv, medium), class
`BlockFor2Or3Players_804AE728` (vtable 0x804AE728). Spawner: __nw__ 0x238,
inline ctor fn_8020020C (base ctor + vtable only): 8 unknown bytes at 0x230.
- update: if fn_80226850(&mPos, field_0x4) == 0x12, spawn ACTOR_BCK2 at mPos
  (spawnActor_801F9484, arg_0x90 0x100, -1, -1) and clear mAlive.
- init: clears mAlive (0x11C).
.text/.data/.bss/.ctors 100 %; extab/extabindex 40 %: the weak JASMemPool
dtor dedup (report tooling), not worked around.

## symbols.txt
- fn_80376850/8B0/91C -> __dt__/update__/init__28BlockFor2Or3Players_804AE728Fv
- fn_80376928 -> __sinit_act_block_for_2_3_players_cpp (local)
- lbl_804AE728 (0x58) -> __vt__28BlockFor2Or3Players_804AE728 (0x54, last word padding)
- lbl_8050D000 (0x100) -> @433..@441 (0xC) + 9 x lbl_ 0x10 memPool_ copies, 4 pad bytes unsplit
- ActorBase slot names: matching's (headers v2)

## SHARED-EDITS
None open. Rebased onto matching 3729a91 (shared headers v2): the local
TODO(shared) declarations are gone (GameGlobals.h / ActorBase.h now declare
them), and symbols.txt takes matching's ActorBase slot names.
