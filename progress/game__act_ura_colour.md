# game/act_ura_colour.cpp

Baseline (progress.py first line at start): total: 4428 / 11561 functions, 794084 / 4442088 bytes of code (17.88 %)
After (rebased on 3729a91, gate ok): total: 4449 / 11561 functions (see progress.py)

## Status: 5/5 functions (code 748/748), NonMatching (extab blocked)

URAC actor ("Ura (dark-world) colour", actors.tsv, medium), class
`UraColour_804A9A70` (vtable 0x804A9A70). Spawner: __nw__ 0x230, inline ctor
fn_8020203C (base ctor + vtable only), no members of its own.
- update: fn_802D77BC(fn_802D6A9C(), field_0x4, 1) (ORs 1 into the view's u16 at
  0x131B8 + 2 * field_0x4); init: calls update() virtually; draw: empty.
.text/.data/.bss/.ctors 100 %; extab/extabindex 47 %: weak JASMemPool dtor
dedup (report tooling), not worked around.

## symbols.txt
- fn_802E07C8/828/82C/864 -> __dt__/draw__/update__/init__18UraColour_804A9A70Fv
- fn_802E0890 -> __sinit_act_ura_colour_cpp (local)
- lbl_804A9A70 (0x58) -> __vt__18UraColour_804A9A70 (0x54, last word padding)
- lbl_804D4468 (0x100) -> @437..@445 (0xC) + 9 x lbl_ 0x10 memPool_ copies, 4 pad bytes unsplit
- ActorBase slot names: matching's (headers v2)

## SHARED-EDITS
None open. Rebased onto matching 3729a91 (shared headers v2): the local
TODO(shared) declarations are gone (GameGlobals.h / ActorBase.h now declare
them), and symbols.txt takes matching's ActorBase slot names.
