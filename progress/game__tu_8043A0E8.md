
## Checkpoint (Sonnet 5.5 pass 1)
Baseline total 8015 -> 8027 (+12). Unit 0/38 -> 12/38.
Matched: fn_8043A0E8, A170, A208, A608, ABCC, D5F0, D644, D834, D874, E7CC, EDE8, 80440928.
Resistant (FIXME in source): A450 99.0 (stage-7 test `bne; b` vs our `beq`), CEA0 85.9 (original unrolled, no loop guard),
D514 98.1 / D57C 97.8 (float regalloc), DF24 96.0 (float regalloc; TBox2f form).
Unwritten: fn_804408F0 (fn_8043E7CC auto-inlines into it; original calls it out of line; would need -inline deferred),
all others (>128 B, not reached). Float-literal functions only match fully once the unit's sdata2 first-use order is complete.
SHARED-EDITS: include/game/GameGlobals.h (cls_804B1920 fields/members), include/game/cls_804B13D4.h (fn_8041E58C, fn_8041E708),
new include/game/cls_804B146C.h (vtable 0x804B146C, the 0x294 object; fn_80425128, fn_80425234); symbols.txt renames of those.
Next: CDA0/CF40/D034 (ptmf table at 0x804B18A0), A268, D39C, DB58, EDF8 (needs classes of the 0x29c..0x2b8 members).

## Checkpoint (Opus 5.5 pass 2, 2026-11-05)
Baseline total 8351 -> 8355 (+4). Unit 13/38 -> 17/38. Structural total 8705 after.
Matched: fn_8043A268 (new member, Fllsff), fn_8043A450 (the stage tests are one || chain: gives the `bne; b exit` tail),
fn_8043EDF8, fn_8043DC98 (draw; the 0xdc bit clears are `&= ~(1 << i)` with no u8 cast, two unrolled loops).
Structural (FIXME in source): fn_8043A638 99.5 regalloc (one shared s16 for delay/wait gives 99.87),
fn_8043CDA0 99.8 / fn_8043CF40 99.8 reloc-name only (literal pool), fn_8043D034 98.9 (one lfs scheduled later + pool),
fn_8043D8B4 99.7 regalloc (second loop counter r28), fn_8043D67C 95.4 missing/extra (cmplwi on 0x199/0x1B5 but
cmpwi switch; cmpwi on the type byte; neg/or/srwi bool of field_0xde & bit).
Data: lbl_804B1810 (kk_mainRank table) written; lbl_804B1820 split to 0x80 + @761..@764 (the ptmf constants at
0x804B18A0 that __sinit copies into lbl_804B18D0, now the real step table {D034, CF40, CEA0, CDA0});
fn_8044094C renamed __sinit_tu_8043A0E8_cpp (99.7: bss offsets of unwritten statics).
Literal pool: CDA0/CF40/D034/D514/D57C/DF24 reach 100 only once fn_8043ACE4 (0xEA0) and fn_8043BB84 (0xFA0)
and fn_8043CB24 are written (first uses of C8C..CC0).
SHARED-EDITS: GameGlobals.h (cls_804B1920 fields 0xc, 0x70, 0x78, 0xde, 0xec..0xf8, 0x108, 0x110/0x114, 0x255,
0x257, 0x298..0x2bc, members; fn_8043D67C returns bool; removed extern "C" fn_8043EDF8), cls_804B13D4.h
(fn_8041FC2C, fn_8041EDB0, fn_8041F720), cls_804B146C.h (fn_804263A8, fn_804257E4, fn_80425F6C), tu_80447618.h
(fn_80447618, fn_804479A0), new cls_804B1C58.h (vtable 0x804B1C58, ctor fn_804485C4; fn_80447FAC, fn_804482EC,
fn_80448334), new cls_804B1E1C.h (moved from tu_8045560C.cpp, unchanged; that unit still 6/7),
src/game/tu_80139BE8.cpp (field_0x14->fn_8043EDF8()), symbols.txt renames of those.
Next: fn_8043CB24, fn_8043D39C (needs fn_8043E80C), fn_8043DB58, fn_8043DF8C, fn_8043E398, fn_8043E80C,
the destructor fn_8043EED8 and constructor fn_8043F504; ACE4/BB84 for the literal pool.
