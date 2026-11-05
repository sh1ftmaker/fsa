
## Small-function pass (Sonnet/Opus)
Baseline total 7547; unit 13/109 -> 43/109; total 7577.
Resistant (FIXME in source): fn_802D6AC4 91.0 / fn_802D7384 88.1 / fn_802D7804 80.0 / fn_802D7F40 85.2 (scheduling);
fn_802DE93C 91.0 / fn_802DEBA0 89.8 (view reload, like fn_802DE9A0).
Unwritten small: fn_802D7440, 7704, 7E5C (u64 mask, needs cast), 7EB0, 7F84, 8050, 9C1C/9C4C/9C7C (own data tables), 802DEA34, 802DEDFC, dtors.
SHARED-EDITS: include/game/GameView.h moved fn_802DE764/7CC/93C/EB58/EBA0/EBF8 out of extern "C" (C++ linkage); symbols.txt renamed to __Flll / __Fv.
