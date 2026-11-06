
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
Baseline 8468 (matching c13ba094), unit 53/109. After: 8474, unit 59/109 (+6 honest); structural +13 (8860 after).
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

## Round 2 (Opus 5.5, 2026-11-05)
Baseline matching 3a0cfee0: total 8628, structural 9056 (+105 close); unit 59/109. After: 8631 / 9063 (+108 close);
unit 62/109 (+3 honest, +7 structural). Full ninja SHA-1 OK; gate --all-units, gate --base matching, gen_game --check,
order_check OK; no function anywhere drops (by address against fsa-decomp's report_honest.json).
100 %: fn_802D7440 (save-file time-limit flag), fn_802D7E5C (`(1 << bit) & mask`), fn_802D7EB0 (clear with
`(1 << bit) ^ 0xFFFFFFFF`: zero-extended complement, set sign-extended).
Structurally done / close (FIXME in source): fn_802D4768 96.7, fn_802D4874 97.1, fn_802D49A8 97.1, fn_802D4ADC 95.5
(sched+regalloc, the fn_802D4C10 address-add family); vfunc_0x10 98.7 (missing 2: the inlined fn_802DEBA0 reload);
fn_802D9CD8 96.3 (li vs mr); fn_802DA010 97.2 / fn_802DB6F0 97.3 / fn_802DCE5C 96.5 (missing 2-4: the bool
materialisation of (save byte == 1) before the inlined fn_802D7440); fn_802DA630 94.5 (regalloc, li vs mr,
reloc-name literals); fn_802DA788 98.1 (extra 1: the target reuses the loop counter's li 0).
Inline-before-use pattern (proven 4 times): MWCC only inlines what is defined earlier, yet earlier functions inline the
bodies of later out-of-line copies. So the original had inlines defined first; written as inline helpers + the
out-of-line copy calling them: screenMask (copy fn_802D7AC4; inlined by 4768/4874/49A8/4ADC), isStage93OrFrom13
(copy fn_802DEBA0; vfunc_0x10), clearScreen (copy fn_802DCDF4; fn_802DA788). Both copies stay at 100 %.
Shared inlines without a copy (identical in fn_802DA010, fn_802DA8DC, fn_802DB6F0, fn_802DCE5C): restartTimeLimit
(fn_802D752C(0) + fn_802D7440(saved == 1)) and clearScreenState (0x178 low bits, 0x308, 0x328/0x12F68, 0x6748/0x2948).
Codegen findings: a shift loop with its test at the top and a plain `b` back (0x802D48FC) is `for (;;) { if (i == 0)
break; ... }` (while/for get rotated and unrolled); bit-table word clears whose word offset is stepped in a register are
`for (bit = 0; bit < 1024; bit += 32) t[bit / 32] = 0` (a word loop unrolls by 8 with folded offsets); the camera's
field_0x844 corners are stored f.x, i.x, f.y, i.y: chained `i.x = f.x = p->x` (TBox2::set stores i first; same
pattern unmatched in tu_8017C01C fn_8017EC70).
Layout proven: field_0x3c8 = u64 [4][10][10] (fn_802DEE2C clears 0x3C8/0x6E8/0xA08/0xD28 + row*0x50 + col*8);
field_0x2c = s32[2] (the step passed to fn_802DEDFC by fn_802DA788).
Unwritten: fn_802DA8DC — full draft 83.8 % (75 instructions long): the two TVec2f-slot clear loops (0x802DAE38,
0x802DAF04) unroll by 2 with offsets stepped in registers in the target, fully in ours (u32/s32 counters, zero() do not
change it); everything else lines up. vfunc_0xC and fn_802DBD60 not started (time).
SHARED-EDITS: include/game/GameView.h (field_0x2c s32 -> s32[2] replacing field_0x30 padding; field_0x3c8 u8[0xC80]
-> u64[4][10][10]; fn_802DA630 bool -> u8 (returns the lbz as is; one caller, unwritten tu_80134C04, clrlwi's);
declared fn_802D3D48, fn_802D92F0 members, inline screenMask, clearScreen, restartTimeLimit, clearScreenState);
include/game/tu_80379F84.h (SaveRow 0x7C/0x7D carved from field_0x7a padding); include/game/act/tu_803276C4.h
(cls_804AB528 s32 field_0x230 carved from padding: lwz/stw in tu_803276C4, tested == 2 by fn_802DA630).
symbols.txt: fn_802D3D48 / fn_802D92F0 -> __17GameView_804A9950Fv member names; lbl_8053A1A8 size 0x8 -> 0x4 (the f32
step 0.2f; the other 4 bytes are section padding): .sdata 100 %.

## Round 3 (Opus 5.5, 2026-11-05, deferred inlining)
Baseline matching b861c081: total 8731, unit 62/109. After: 8734, unit 65/109 (+3 honest: fn_802D46D8, fn_802DA788,
fn_802DEE2C); structural 9171 (+117 close). Full ninja SHA-1 OK; gate --all-units, gate --base matching, gen_game --check,
order_check OK; no function anywhere drops (by address against the baseline and fsa-decomp's report_honest.json).
- Deferred cleanup: the inline-helper + wrapper pairs are gone; fn_802D7AC4, fn_802DCDF4, fn_802DEBA0 are plain
  functions with their bodies, inlined where the original inlines them; every % held. restartTimeLimit and
  clearScreenState stay inline members (no out-of-line copy exists; as plain functions they would emit extra copies).
- getGameView_802D6A9C(): the whole file now reaches the view through this unit's accessor (inlined under deferred).
  Proof: with gGameRoot->field_0x4->mView written out, the TVec2f-slot loop unrolls fully (75 extra instructions in
  fn_802DA8DC / fn_802DBD60 / vfunc_0xC); through the accessor's inline nodes it unrolls by two as in the target.
  The global switch lost nothing and brought fn_802D46D8 to 100 % (78.3 before).
- fn_802D7440's flag is u8 (inlined, (byte == 1) is narrowed with extrwi 8 and compared with 1 again; a bool folds it):
  the "2 instructions short" FIXMEs are gone (fn_802DA010 / fn_802DB6F0 now regalloc-only, fn_802DCE5C one li).
- Shared inlines without a copy (each proven by identical instruction runs): clearStageTables (fn_802DEE2C + tables)
  and clearSlots (447 instructions identical in fn_802DA8DC, fn_802DBD60, vfunc_0xC; as a single inline it is too
  large to be inlined), placePlayers (fn_802DA010, DA788, DA8DC, DB6F0, vfunc_0xC; it also fixed the stack layout:
  fn_802DA788 100 %).
- New: fn_802DA8DC 98.6, fn_802DBD60 98.8, vfunc_0xC 97.6 (structurally close, FIXMEs), fn_802DEE2C 100 %.
- Unwritten: fn_802DED40 / fn_802DEC40 = view->fn_802D7B34 / fn_802D7BE8 inlined into free functions, 78 / 80 %
  as forwards (the fn_802DE9A0 view-reload family); fn_802D3D48, fn_802D4CCC, fn_802D8328, fn_802D8D50, fn_802D92F0.
SHARED-EDITS: include/game/GameView.h (fn_802D7440 bool -> u8; removed screenMask / clearScreen inline decls; added
clearStageTables, clearSlots, placePlayers inline decls and fn_802DEE2C(void)); include/game/GameGlobals.h
(extern "C" fn_80303870(s32) [tu_80301CD0], extern "C" bool fn_803B8EBC(s32) [tu_803B8C74], void fn_8023F6C4()
[tu_8022AD64, defined there]); symbols.txt: fn_802D7440 Fb -> FUc, fn_802DEE2C -> fn_802DEE2C__Fv.
