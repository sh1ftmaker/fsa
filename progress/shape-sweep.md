# shape-sweep (Opus, 2026-11-06)

Baseline (matching dbc6c66c): 9004 / 11557. After: 9018 (+14 honest), structural 9558 -> 9569.
Cross-file sweep of the wave-18 shapes over every sub-100 game function (objdiff target vs ours:
subfic/cntlzw/extrwi. bytes, bne +8 / b early-outs, `lwz rS` before a bl then cmpw with r3).

To 100 %: tu_8013AAD4 fn_8013BBE0, tu_80158624 fn_80159330 (isGameMode); tu_8025EB18 fn_8025EE48,
fn_8025F2CC (isField0xd64One `? true : false`), fn_8025FCC8 (member || chain); tu_801CDC8C fn_801D0B44,
tu_8027E5BC fn_80283D2C / fn_8027E7E0, act_special_conversation_npc x3, act_block fn_8028C364
(inlined getters); tu_8023FBF0 fn_802416FC (one early-out condition); tu_801ED3DC fn_801ED3DC (const).
Raised, FIXMEs updated: see the git log of chore/shape-sweep.

SHARED-EDITS: include/game/ActorBase.h getSerial() (0x198 mSerial, s32) + comment on the getters;
include/game/tu_801ED3DC.h fn_801ED3DC const (symbols.txt rename ...CFll, placeholder).

Not generalising (now in RULES_CARD): getter where the asm reads after the call (act_princess_zelda),
local read twice into one register (act_horse_race_man); srwi 5 word form (act_fire_rod_fire);
bne+8/b in the middle of early-outs (tu_8013C5C8); const for load-order-only (GameView fn_802D7F40).
Open: act_navi vfunc_0x9C / init and act_panel_rewrite_npc vfunc_0x10 (beq over b around a bool /
store block) — no natural form found.
