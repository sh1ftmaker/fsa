# game/act_bg_rewrite_check_switch.cpp

Baseline (progress.py first line at start): total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
After (rebased on 3729a91, gate ok): total: 4449 / 11561 functions (see progress.py)

## Status: 5/5 functions (code 748/748), NonMatching (extab blocked)

BGWC actor ("BG-rewrite check switch", actors.tsv, medium), class
`BgRewriteCheckSwitch_804A8998` (vtable 0x804A8998). Spawner: __nw__ 0x234,
inline ctor fn_80202FDC (base ctor + vtable only), so one uninitialized word
at 0x230 (`field_0x230`). Overrides dtor, init (empty), update, draw (empty).

Matched: all five. .data/.bss/.ctors 100 %; extab blocked by
the weak JASMemPool dtor dedup (report tooling).

update: `if (getGameView_802D6A9C()->field_0x131de) { vfunc_0x30(0xFF, true); fn_801F3668(); }`, matches with headers v2.

## symbols.txt
- fn_802BA048/0A8/0AC/10C -> __dt__/draw__/update__/init__29BgRewriteCheckSwitch_804A8998Fv
- fn_802BA110 -> __sinit_act_bg_rewrite_check_switch_cpp (local)
- lbl_804A8998 (0x58) -> __vt__29BgRewriteCheckSwitch_804A8998 (0x54, last word is padding)
- lbl_804D2668 (0x100) -> @433..@441 (0xC) + 9 x lbl_ 0x10 (memPool_ copies), 4 pad bytes unsplit
- ActorBase slot names: matching's (headers v2)

## SHARED-EDITS
None open. Rebased onto matching 3729a91 (shared headers v2): the local
TODO(shared) declarations are gone (GameGlobals.h / ActorBase.h now declare
them), and symbols.txt takes matching's ActorBase slot names.
