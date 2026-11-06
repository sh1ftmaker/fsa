
## Small-function pass (Sonnet 5.5)
Baseline: 7547 / 11557 functions, unit 24/254. After: 7649 / 11557, unit 126/254 (+102, no other unit dropped).
Full ninja: dol sha1 equals build.sha1; gate --all-units / --base matching ok; gen_game --check ok; order_check ok.

Matched: 100 getters/setters/forwarders (<=128 B), fields added to Player.h (splits of the u8 hole arrays),
cls_804A71A8 fields 0x4A-0x4F/0x98/0x99/0xB4 and fn_80269420/fn_8026942C, cls_804AA8C8::fn_8030A3B0.

Resistant:
- (review) fn_80241920 now 100 % as `value >= 0 ? value : 0`; fn_802517E4 100 % with a u8 return (extrwi), carried through fn_8022D894 (tu_8022AD64) as u8; fn_8030A308/fn_80247534/fn_8022C8EC are u8 too (their asm has the extrwi / no conversion).
- fn_80246F28 (94.8 %, from before).

Unwritten small functions and why:
- fn_8024FEFC: the original calls fn_8024FEBC/fn_8024FE78 out of line; MWCC inlines them here.
- fn_80248FF0: inlines fn_80249070 (defined later in the file).
- fn_8024ECCC: contains an infinite loop (`b .`) assert; not guessed.
- fn_80241B44/DD0/E28, fn_80252F50/5C/68/74: use field_0xbec as a cls_804AA8D8* (game/tu_8030AE84.h) but fn_8023FC24 (matched) returns it as s32 via fn_8022BCD4 (tu_8022AD64); needs field type + wrapper return changed together. DD0/E28 also copy a TVec3f to the stack and pass it as a TVec2f*.
- fn_802422C4, fn_80252F48, fn_8025196C: result types unknown (no callers' evidence).
- __dt__12cls_804B20F8: defining it out of line removes tu_8045B010's @24@__dt__ thunk (tried, reverted).

## Second pass (Sonnet 5.5)
Baseline 8093 / 11557. After 8102 (+9: unit 128 -> 137/254). Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok.
Matched: fn_80247594, fn_80248778 (const; `players` declared first for r30/r31), fn_8024BAEC, fn_8024F54C (`arg == 0x1000002` gives subis/cmplwi/bne), fn_80248DD8, fn_802517F8 (cls_804A71A8 0x48/0x49 carved), fn_80247F58, fn_8024C4C4, fn_8024CA20.
Written below 100 %: fn_802416FC 97.1 (original has `beq add; b end` after fn_80270A8C, ours `bne end`; tried 3 shapes), fn_80248604 88.2 (index zero shared with the false store: li r6,0 / mr r4,r6), fn_8024E344/E3D8/E46C 99.86 (code identical; float literal pool order, @NNN vs lbl_ until earlier literals are used).
Still unwritten and why: fn_8024FEFC (MWCC inlines FEBC/FE78 where the original calls them; tried), fn_8024EFF8 (extsb on field_0x4 passed to fn_80458880(ll)), fn_8024ED4C/ECCC (`b .` asserts), fn_80242624/fn_80248DD8-style "set state" inline (li 1/0 + clrlwi. of an inlined bool setter; fn_80242624, fn_80242560 etc.), fn_8024D058 (inlines fn_80248604's loop plus fn_8024B2E0), fn_8024DC00 (PSVECMag inline vec ops).
SHARED-EDITS: Player.h (decls fn_80247F58/fn_80248DD8/fn_8024CA20/fn_802517F8/fn_8024E344/E46C/fn_8024B2E0; fn_80248778 const; field_0x124c bool, field_0xd34 bool, field_0xd4e bool + field_0xd4f[1]); cls_804A71A8.h (0x48/0x49 fields); GameGlobals.h (extern "C" u8 fn_8030381C(s32), void fn_803037D4(s32,u32), owner tu_80301CD0). symbols.txt placeholders renamed to mangled Player names.

## Player architecture + constructor/destructor (Opus 5.5, 2026-11-05)
Baseline 8372 / 11557 (after rebase on matching 8cf7a05c). After 8379 (+7), structural +8. Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok; no function anywhere drops (per-address compare with the main report).
docs/PLAYER.md: unit map, per-frame flow, state system (0x1240/0x1244/0x1248, three switch jump tables in fn_802870C8, 82-state handler table), big functions, boot-path coverage, recommended order.
Matched: __ct__15Player_804A6F38Fl, __dt__15Player_804A6F38Fv, __dt__12cls_804B20F8Fv (now emitted after our dtor), fn_80252F50/5C/68/74 (cls_804AA8D8 0x290-0x29C).
Written below 100 %: fn_80254660 99.91 (reloc-name only: lbl_80540098 256/3 is @NNN until earlier literals are in pool order).
Fields proven by the ctor/dtor: 0xBEC cls_804AA8D8*, 0x1250/0x1254 cls_804B20F8*, 0x1258 TVec3f, cls_804A64A8 at 0x1278/0x1290/0x12D0/0x12E8/0x130C/0x1330, cls_8049F140 at 0x1398/0x13A8 (arrays at 0x38/0x4E0/0x510/0x550/0x5FC confirmed); fn_80254660: 0xB90 f32, 0xC0C s32.
Next: fn_80254744 (per-slot reset, 0x11A0, 646 instrs on the battle path; proves most remaining fields), fn_802544B0, fn_80253E6C, then tu_80285E58 (fn_80285E58 / fn_802870C8).
SHARED-EDITS: Player.h (fields above; fn_8023FC24 and fn_8022BCD4 return cls_804AA8D8*; includes cls_804A64A8.h + new cls_8049F140.h); cls_804A64A8.h (dtor inline, tu_802144F0's out-of-line definition removed, still 100 %); new include/game/cls_8049F140.h (class moved out of tu_8013C5C8.cpp, ctor/dtor inline); tu_8022AD64.cpp fn_8022BCD4 return type; cls_804A71A8.h comment. symbols.txt: fn_8026F9E8 -> __ct__12cls_804A71A8FP15Player_804A6F38, fn_8018E890 -> __ct__Q29JGeometry8TBox2<f>Fv.

## Player track, agent A (Opus 5.5, 2026-11-05): reset, update A, state request, layout
Baseline 8384 / 11557 at start; matching moved to 8385 (tu_80285E58) and the branch was rebased. After 8388 (+3 honest: fn_80251AEC, fn_80251BFC, fn_8023F2BC), structural 8730.
Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok (tu_8023FBF0, tu_8022AD64); no function anywhere drops (per-address compare with the main report_honest.json, including percentages).
Written:
- fn_80251AEC 100 %, fn_80251BFC 100 % (0x1244 read once into a local for the last compare).
- fn_80254744 99.30 % (different + reloc-name): only the first 0x31 loop's setup registers differ (li r5,0 / mr r3,r5 vs li r6,0 / li r3,0) plus the literal pool. Inlines fn_80248778 (rewritten to read its fields, still 100 %), fn_80241794, fn_80241200 and fn_80251AEC(3).
- fn_802544B0 99.95 %, fn_80253E6C 99.99 %: reloc-name only (@NNN literals).
- fn_8023F2BC (tu_8022AD64) 100 %.
Unwritten: fn_8023F390 95.15 % "different": the inlined fn_8022F478 / getPlayerLayer_8023DE58 in its last loop come out with the opposite block order (fallback before the call); tried &&-chain, nested if, continue. Source kept in the scratchpad.
Choices a reviewer should check: the player-bit test is written out as `gGameRoot_8053AAF8->field_0x82 & (u8)(1 << field_0x4)` (GameRoot_8049EF20::fn_8023F98C's body; making it a header inline loses its out-of-line copy until fn_80235B68 is written); in two branches of fn_80254744 the operands are `(u8)(1 << field_0x4) & ...` (the original loads field_0x4 first there); file-local inlines viewStageFlag (fn_8019CE40's test on the view stage, used twice) and joinCue (four copies differing only in frames); `(s8)field_0x4` for the extsb before fn_80458880; the switch in fn_80254744 has `case 1:` sharing default's code (the original's search compares 1); an unused stack copy of mPos per pass in the second 0x31 loop.
SHARED-EDITS: Player.h (~110 fields carved, 0x350 TVec3f replaces f32 0x358 -> tu_8026FC20 fn_8027966C uses field_0x350.z; 0xEF0 s32[0x50]; member decls fn_80276538, fn_80276078, fn_80253E6C, fn_802490A8, fn_80251D50, fn_8024BDBC, fn_8024E0A0, fn_8026FD14, fn_80252FC0, fn_80264998; free fn_80236890, fn_80232A0C, fn_80231DE4); cls_804A71A8.h (fn_8026F710, f32 0x14/0x18); tu_8030AE84.h (fn_8030B774); GameGlobals.h (cls_804A03A8::fn_8015F8FC, fn_8043A170 decl, fn_8023F2BC/fn_8023F390 moved out of extern "C"); tu_8022AD64.cpp (fn_8023F2BC + playerAt inline); tu_8026FC20.cpp (field_0x350.z). symbols.txt: 17 placeholder renames to mangled names.

## Player track round 3, agent C (Opus 5.5, 2026-11-05)
Baseline 8389 / 11557 (honest), structural 8735. Rebased on matching after agent D's merge (cls_804A71A8.h conflict resolved keeping D's arrays + anyFlag48).
Written: fn_802490A8 97.4 (missing/extra, only the 0xCE9 test x13), fn_80252FC0 99.1 (regalloc), fn_8023F390 99.7 (tu_8022AD64, one mr/li), fn_80240670 97.8, fn_802486A0 97.7 (regalloc), fn_8023FC5C 96.6, fn_8023FFC4/8024018C/80240354 99.4 (regalloc), pad gates fn_802454C0..fn_80246D1C, fn_80244FFC, fn_80245238, fn_8024712C, fn_80247330, fn_80243234, fn_80243C64, fn_80244630 (96-97, the 0xCE9 test), fn_80246F28 94.8 -> 96.5.
Not written: fn_802487FC / fn_80248970 (li/li vs li/mr zero of the group loop, class different).
Open decision: playerFlags_802413F0's last test (see docs/PLAYER.md round 3).
Follow-up (coordinator decision): playerFlagsGate (if form) for the inlined gate; fn_802413F0 unchanged (100 %). fn_802490A8 99.88, gates 99.46-99.89, fn_80243234/3C64/4630 99.9, fn_8023FC5C 98.7, all regalloc-only.
fn_80254744 98.34 -> 98.32 is a literal-renumbering artifact (literals 3.0 / 16.0 now pooled earlier by fn_80252FC0); accepted by the coordinator.

## Player track round 7, agent L (Opus 5.5, 2026-11-05)
Rebased on matching 5a9b1485. Unit 146 -> 163/254 (+17 honest); total 8562 / 11557, structural 8979. Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok (tu_8023FBF0, tu_8030AE84); per-address compare with the main report: one drop, fn_80252FC0 99.11 -> 99.06 (registers only), because it now calls fn_80240B84 instead of repeating its body.
100 %: fn_8024157C, fn_80241618, fn_80241DD0, fn_80241E28, fn_80242560, fn_80242624, fn_80242BC0, fn_80242CB0, fn_80248604 (was 88.2), fn_8024AFF8, fn_8024B108, fn_8024B9E8, fn_8024D058, fn_8024D99C, fn_8024EFF8, fn_8025196C, fn_80252F48.
Below 100 % (with FIXME): fn_80248004 99.6 (regalloc), fn_8024762C 99.2 (missing/extra: one reused d60 test), fn_80240B84 98.9 (regalloc; now const, as fn_80240670), fn_802408C8 96.0 (missing/extra: loop entry test), fn_8024051C 95.2 (regalloc), fn_8024DC00 99.9 (reloc-name only). fn_80254744 98.32 -> 98.72 (calls fn_80248604).
File inlines added (bodies of later out-of-line copies that earlier functions inline): resetTrail (fn_80248604), requestState (fn_80251AEC), shared48C_80247BD4 / sharedBC0_80247B9C; the out-of-line copies stay 100 %. fn_8024BAEC keeps its direct body (100 %): the inlined copies share the index zero (li/mr), the direct body does not.
Proven: cls_804AA8D8::fn_8030B084 / fn_8030B110 take the position by value (every caller copies a whole TVec3f to the stack); fn_80242624 the same.
Not written: fn_802487FC / fn_80248970 (only a direct loop body gives their li/li zeroing, 97.3 % regalloc; through groupSize it is li/mr, "different"; tried split declarations in the inline, they move other functions); fn_80240DCC (89.6 % "different": three bools zeroed li/mr/mr in the original; source in the scratchpad); fn_8024C378 (needs Player 0x1268 as TVec3f, agent K); fn_802422C4 (needs cls_804A71A8 0x14/0x18 as a TVec2f); fn_80241B44 (takes a TBox2f*: needs tu_8022AD64's generator-owned caller changed); fn_8024ED4C / fn_8024ECCC (`b .` asserts).
SHARED-EDITS: Player.h (decls: fn_80240B84 const, fn_8024157C, fn_8025196C, fn_80252F48, fn_80241DD0, fn_80242624, fn_8024D058, fn_8024DC00, fn_8024EFF8(u32), free fn_8022AD64); tu_8030AE84.h + tu_8030AE84.cpp (fn_8030B084 / fn_8030B110 TVec3f by value, still 100 %); act/act_small_key.h (SmallKey fn_802C1674 / fn_802C1778 decls). symbols.txt: 11 renames (placeholders to mangled names).

## Player track round 10, agent Q (Opus 5.5, 2026-11-05)
Rebased on matching 67213dd2. Unit 163 -> 172/254 (+9 honest); total 8663 / 11557, structural 9103. Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok; per-address compare with the main report: no drops (anyFlag48's users in tu_80256C74 / tu_8025EB18 / tu_8026FC20 unchanged).
100 %: fn_8024ECCC, fn_8024ED4C (the `b .` halts are `if (c) { for (;;) {} }`), fn_802422C4, fn_80248FF0 (fn_80249070's body as file inline clear121F_80249070), fn_80252BD0, fn_80252CF0 (two inlined fn_8024B9E8), fn_80248E94, fn_8025197C (= field_0xbf0->anyFlag48(all)), fn_80241BA0 (box passed as `&TBox2f(*box)`).
Below 100 % (FIXME): fn_8024C378 98.08 (regalloc), fn_80240DCC 86.20 (scheduling+regalloc, through four layer-query inlines), fn_80248A24 99.52 / fn_80248B9C 99.39 (regalloc + literal names), fn_80241964 97.29 (scheduling+regalloc), fn_8024C828 99.42, fn_80242704 99.51, fn_8024CAC8 99.53 (regalloc), fn_8024BBB8 95.97 (missing/extra: field_0x3b4 loaded before getPlayerLayer into r30 in the original).
Not written: fn_802487FC / fn_80248970 (li/li zeroing; tried 7 groupSize shapes: declaring `i` before `count` improves fn_802486A0 / fn_80240670 / fn_8023FC5C but drops ~25 gated functions, reverted), fn_80241B44 (TBox2f*, caller in tu_8022AD64), fn_8024FEFC (callees inlined by MWCC).
SHARED-EDITS: Player.h (decls fn_80248A24, fn_80248B9C, fn_80241964; fn_8025197C param `all`); cls_804A71A8.h (anyFlag48(bool all = true)). symbols.txt: fn_80248A24 / fn_80248B9C / fn_80241964 -> Player members.

## Player track round 13, agent X (Opus 5.5, 2026-11-05): deferred cleanup + 15 functions
On matching b861c081. Unit 172 -> 174/254 (+2 honest: total 8731 -> 8733); structural +15 (9184).
Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok; per-address compare with the main report: 0 drops, 18 gains.
Workarounds replaced by the plain function at its own address (deferred inlines it into earlier callers; no function lost %):
- groupSize -> fn_80248970 (new, 97.3 regalloc), groupMode -> fn_802486A0, groupTurn -> fn_80240888, linePlace -> fn_80248778, follows_80240670 -> fn_80240670 (97.8 -> 98.8, now regalloc-only).
- requestState -> fn_80251AEC; shared48C / sharedBC0 -> fn_80247BD4 / fn_80247B9C (fn_8024762C 99.2 -> 99.8, now regalloc-only); clear121F -> fn_80249070; addCounter -> fn_8024ED4C; padAxis_* -> fn_8024712C / fn_80247330.
- resetTrail: fn_80248604's own body as a plain function drops it to 88.2 (its original asm has the inlined li/mr zero); it is `fn_8024BAEC();` (fn_8024BAEC is the same code, li/li as a direct body), every inlined copy unchanged.
Kept: playerFlagsGate (fn_802413F0's `!= 0` body in the gates drops 21 functions 99.5 -> 96.8, tried again under deferred); teamCheck, inState1B, the layer queries, inViewBox, planarDistance, normalizeVec, joinCue (no out-of-line copy in the unit).
Pragma rule 1 regions (each with the caller in the comment): fn_8024FEFC/FEBC/FE78 (fn_8024FFC8, fn_8024FEFC), fn_8024E0A0 (fn_80253E6C), fn_80250D30 (fn_80250030, fn_80252FC0).
100 %: fn_8024FEFC, fn_8024EE24.
Regalloc-only (FIXME): fn_8024E500 99.7, fn_8024DA64 99.3, fn_802422D0 99.7 (BoxTest.h containsPoint), fn_80250ACC 99.5, fn_80242940 99.6, fn_80240F28 98.4 (const), fn_8024E0A0 99.3, fn_8024C5A0 99.6, fn_8025250C 99.6, fn_80250D30 99.6, fn_80247C0C 99.4, fn_80251D50 98.6, fn_80248970 97.3. Almost all are the group loop's leader / self temporaries (one register in ours, two in the original); dropping the `team` / `self` locals drops ~40 functions 3 %, reverted.
Not written: fn_802487FC (still "different" 94.9: its direct fn_80248970 copy zeroes li/li in the original, li/mr in ours; 4 shapes), fn_80241B44 (writes a TBox2f through a TVec2f* parameter; needs the symbol and tu_8022AD64's fn_8022B8EC changed), fn_8024DCD0 and the larger ones.
SHARED-EDITS: Player.h (fn_80240F28 const; decl fn_80250FD4; free fn_8022AD88; update-A comment); BoxTest.h comment. symbols.txt: fn_80240F28 -> ...CFv, fn_80250FD4 -> fn_80250FD4__15Player_804A6F38Fv.

## Player track wave 14, agent Y (Opus 5.5, 2026-11-05)
Rebased on matching 2f6a1710. Unit 174 -> 181/254 (+7 honest); total 8803 -> 8810, structural 9270.
Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok; per-address compare with the main report: 0 drops, 13 gains.
100 %: fn_80248970 (was 97.3: `s32 leader;` declared first, self stays in its branch), fn_8024DCD0 (fn_8024E060's tail written `if (field_0x1216 == 1) return false; return true;`, still 100 %, so the inlined copy branches), fn_80242DAC (conveyor tiles; JGeometry TVec3f::normalize() replaces the normalizeVec file inline, fn_80252FC0 99.16 -> 99.73), fn_80241E64 (30 deleteEmitter, 0x135C-0x1370 in the original's order), fn_8024F0E8 (cue: pad rumble or the sound objects 0x1250 / 0x1254); fn_80248A24 / fn_80248B9C reach 100 % through the literal pool.
Below 100 % (FIXME): fn_8024CD20 99.64 (regalloc: the group loop's leader / own index), fn_802527B0 99.92 (literal name 20.0f only), fn_802520C0 97.09 (scheduling+regalloc + literal name).
Data: lbl_8046AE78 (Vec[4], +-15.999 by facing) as file-scope data; .rodata 46 % until fn_8024F63C / fn_80250090 write the three tables in front of it.
Group loop (two registers): measured 24 declaration orders and 6 shapes across the unit. Only `leader` first (fn_80248970 alone) is clean (+1). Declaring both leader and self first gives fn_802486A0 / fn_8024051C / fn_80240670 / fn_80240B84 100 % (+5) but drops ~36 gated functions 99.5 -> 98.7: in the inlined copies the original colours leader lowest (r27) and self highest (r30), the standalone copy leader highest; no source shape found that gives both. Not taken.
Not written: fn_802487FC (94.95 "different": the direct inlined copy zeroes li / li in the original, li / mr here; six shapes incl. ternary, declarations at the top), fn_80241B44 (unchanged reason), the big ones (fn_8024BDBC, fn_8024E694, fn_8024B2E0, fn_80250FD4, fn_8024F63C, fn_8024D134, fn_80250090).
SHARED-EDITS: Player.h (decl fn_802520C0(const TVec2<s32>*), fn_802527B0 comment); Map.h (bool fn_80225EF4(const TVec3f*, s32), C++). symbols.txt: fn_80225EF4 -> fn_80225EF4__FPCQ29JGeometry8TVec3<f>l, fn_802520C0 -> fn_802520C0__15Player_804A6F38FPCQ29JGeometry8TVec2<l>.

## Player track wave 16 (Opus 5.5, 2026-11-06): the seven big functions
On matching 831c008b. Unit 181 -> 189/254 (+8 honest: total 8920 -> 8928); structural 9439 -> 9446.
Full ninja sha ok; gate --all-units / --base matching ok; gen_game --check ok; order_check ok for tu_8023FBF0 (10 other units out of order, untouched here); per-address compare with the main report: 0 drops, 22 gains.
Written: fn_8024BDBC 99.53 (regalloc), fn_8024E694 99.82 (regalloc), fn_8024B2E0 92.26 (missing/extra -4), fn_8024D134 99.65 (regalloc), fn_8024F63C 99.07 (scheduling+regalloc), fn_80250090 99.21 (regalloc), fn_80250FD4 99.86 (regalloc), fn_802487FC 94.95 (different: li/li vs li/mr, needed as the plain function fn_8024D134 inlines). fn_802408C8 96.0 -> 99.05 (`int` loop index).
100 % through the literal pool: fn_80254660, fn_802544B0, fn_80253E6C, fn_802527B0, fn_8024E344, fn_8024E3D8, fn_8024E46C, fn_8024DC00.
Data: .rodata 100 % (lbl_8046AE08, lbl_8046AE28, lbl_8046AE58 as source data).
Not written: fn_80241B44 (reason unchanged).
SHARED-EDITS: Player.h (fn_8024E694 parameter name). Details: docs/PLAYER.md "Round 16 (tu_8023FBF0)".
