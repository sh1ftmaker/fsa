
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

## Wave 2 (Opus 5.5, 2026-11-05)
Baseline 8429 (matching 9309b311), unit 53/109. After: 8435, unit 59/109 (+6 honest); structural 8809 -> 8822 (+13).
100 %: fn_802D79A4, fn_802D79E8, fn_802D7D00 (85 calls), fn_802D9C1C, fn_802D9C4C, fn_802D9C7C.
Structural (FIXME in source): __ct__ 99.9 reloc-name (512.0f pool position); fn_802D3A18 99.5 (z stored twice
in the target: x, z, z, y); fn_802D46D8 78.3 sched+regalloc (table lis/addi order); fn_802D4C10 94.8
sched+regalloc (view + i*2 before the row); fn_802D6C10 98.1 regalloc; fn_802D6D70 84.6 missing 3 (b.x hoisted,
mView reloaded before the id test); fn_802D6F38 96.4 (free-slot loop has no entry test in the target; a for/break
gets unrolled); fn_802D7BE8 99.4 regalloc (as 7B34); fn_802D8128 / 8228 96.5 sched+regalloc (field_0x18 read after
the column checks, field_0x8 before field_0x4: looks like an inlined (stage, level) lookup, cf. fn_802D9C7C, whose
default "500" differs from fn_802D8D50's "001"); fn_802DCD1C 94.2 / fn_802DD3D4 99.0 (result returned via
clrlwi r3, rN, 24 in the target).
Data: .data 73 % (string tables lbl_804A9530/9660/96D0/9710/9830 as source), .sdata2 94 % (their literals open
the pool), .rodata 85 % (lbl_8046E1CC masks + the map CSV strings). Still to write: lbl_8046E148 (4 x
(x, y, 0) floats), lbl_8046E178 (6 tags 'CKSW' 'ONOF' 'OFFS' 'GLBS' 'LCLS' 'SWT4'), lbl_8046E190 (u16 pairs,
used by fn_802D4CCC), the rodata tail strings, .sdata lbl_8053A1A8 (vfunc_0x10).
Layout proven: the u64 screen masks start at 0x1048 (was field_0x1010 [..][7]); field_0x1323c f32[4] carved
out of the name buffer (vfunc_0x10 / vfunc_0xC asm); field_0x131fa is fn_802D8228's buffer; field_0x130c8 u32.
SHARED-EDITS: include/game/GameView.h (field_0x1010 -> field_0x1048 [same bytes 0x1048-0x2948, padding
0x3C8-0x1048]; field_0x130c8 s32 -> u32; field_0x131fa u8 -> char; field_0x1321a 0x32 -> 0x22 + f32
field_0x1323c[4]; fn_802D7BE8 value s32 -> u32; declared fn_802D3A18, fn_802D8328(bool, bool), fn_802D8D50(bool)).
symbols.txt: fn_802D7BE8 Flll -> FllUl, fn_802D3A18 / 8328 / 8D50 -> member names; .sdata2 string objects
merged to their real sizes: lbl_80540FDC "001" (dropped dtk's lbl_80540FDD), lbl_8054106C "a" (size 2),
lbl_80541128 "500" (dropped lbl_80541129) — no code referenced the dropped labels.
Next: vfunc_0x10 (0x802DD54C, calls fn_802D3A18, uses .sdata lbl_8053A1A8), fn_802D9CD8, fn_802DA630/788,
fn_802DCE5C; fn_802D49A8 / 4ADC inline fn_802D7AC4 (later-defined) — left unwritten per the rule;
fn_802D4768 / 4874 similar slot lists. fn_802D7E5C / 7EB0 need the 0x3C8 block carved (u64 at 0x3C8 + i * 0x320).
