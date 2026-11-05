# game/act_vaati_buster.cpp

Baseline (progress.py first line at start): total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
After (rebased on 3729a91, gate ok): total: 4447 / 11561 functions (see progress.py)

## Status: 3/3 functions (code 688/688), NonMatching (extab blocked)

GFBS actor ("Vaati Buster", actors.tsv, medium), class `VaatiBuster_8049E3A8`
(vtable 0x8049E3A8; spawner case: inline ctor fn_80201630, __nw__ 0x230, so no
members of its own). Overrides the destructor and init():
init = `ActorBase_804A5770::init(); fn_802D7440(getGameView_802D6A9C(), false);`.

.text, .ctors, .data, .bss 100 %. extab/extabindex 40 %: the deduplicated
JASMemPool dtor entries (report tooling, in progress); not worked around.

## symbols.txt
- fn_8012679C -> init__20VaatiBuster_8049E3A8Fv
- fn_801267C8 -> __dt__20VaatiBuster_8049E3A8Fv
- fn_80126828 -> __sinit_act_vaati_buster_cpp (local)
- lbl_8049E3A8 (0x58) -> __vt__20VaatiBuster_8049E3A8 (0x54; the last word is
  .data alignment padding, our object's vtable is 0x54)
- lbl_804BD160 (0x100) -> @427..@435 (0xC dtor-chain objects) + 9 x lbl_ 0x10
  weak memPool_ copies (0x804BD1CC..0x804BD24C), 4 pad bytes unsplit
- ActorBase slot names: matching's (headers v2)

## SHARED-EDITS
None open. Rebased onto matching 3729a91 (shared headers v2): the local
TODO(shared) declarations are gone (GameGlobals.h / ActorBase.h now declare
them), and symbols.txt takes matching's ActorBase slot names.
