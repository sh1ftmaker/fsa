
## Small-function pass (Sonnet/Opus)
Baseline total 7547; unit 13/109 -> 43/109; total 7577.
Resistant (FIXME in source): fn_802D6AC4 91.0 / fn_802D7384 88.1 / fn_802D7804 80.0 / fn_802D7F40 85.2 (scheduling);
fn_802DE93C 91.0 / fn_802DEBA0 89.8 (view reload, like fn_802DE9A0).
Unwritten small: fn_802D7440, 7704, 7E5C (u64 mask, needs cast), 7EB0, 7F84, 8050, 9C1C/9C4C/9C7C (own data tables), 802DEA34, 802DEDFC, dtors.
SHARED-EDITS: include/game/GameView.h moved fn_802DE764/7CC/93C/EB58/EBA0/EBF8 out of extern "C" (C++ linkage); symbols.txt renamed to __Flll / __Fv.

## Second pass (Sonnet 5.5)
Baseline 8093 / 11557, unit 43/109. After 8103, unit 53/109 (+10). Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok.
Matched: fn_802D6B24, fn_802D7618, fn_802D766C (`layer < 8 ? 0 : layer - 7`; the `>= 8 ? ... : 0` form compiles differently), fn_802D7704, fn_802D7AC4, fn_802D7F84, fn_802D8050, fn_802DCDF4, fn_802DEDFC (free, forwards to view->fn_802DCE5C), __dt__17GameView_804A9950Fv (empty dtor).
Below 100 %: fn_802D75BC / fn_802D76A8 91.3 (same scheduling as fn_802D6AC4), fn_802D7B34 99.0 (regalloc only: shift count r31 / index r30 swapped; idx declared first), fn_802DEA34 93.5 (same family as fn_802DEABC: target reloads the view before the 2nd pair).
Unwritten: fn_802D9C1C/9C4C/9C7C and fn_802D46D8 (tables in the unit's own .data/.rodata, not split), fn_802D79A4/79E8 (unit .sdata2 literals), fn_802D7E5C/7EB0 (u64 mask at 0x3c8 + i*0x320 needs a struct carve; high word sign-extension), fn_802D7440 (lbl_8050EC80 table, 0x7c/0x7d), larger functions.
Note: the 0x1048-based u64 reads are field_0x1010[row][col][max(layer,7)]; fn_802D7AC4 as an explicit if, fn_802D7B34 as 7 + (layer < 8 ? 0 : layer - 7).
SHARED-EDITS: GameGlobals.h (extern "C" void fn_80308D0C(), owner tu_80301CD0); GameView.h (fn_802DEA34 moved out of extern "C" to C++ linkage, fn_802DEDFC declared); symbols.txt: fn_802DEA34 -> __Fv, fn_802DEDFC -> __FPCl.
