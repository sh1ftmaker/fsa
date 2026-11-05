
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
