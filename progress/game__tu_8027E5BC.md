
## 2026-11-05 Sonnet 5.5 pass
Baseline total 7797 -> 7824; unit 1/73 -> 28/73 (honest report). 27 further functions are 99.0-99.9 % with only
reloc-name (sdata2 literals not yet in the original order/split: resolves when neighbouring functions are written).
Resistant: fn_8027E7E0 98.85 % (scheduling+regalloc): original loads field_0x3b4 into r30 before the field_0x11fc
load for the getPlayerLayer_8023DE58 call; ours loads it after (tried flipping the != operands, no change).
Not written (callees unknown / need new class for the 0xF4/0x24 "stop emitter" pointer at 0x1350/0x1354/0x1358/0x132c/0x135c:
fn_80280DD8, fn_80281A18, fn_802811C0), the rest need unnamed callees (fn_8027EB04 and larger bodies).
SHARED-EDITS: include/game/Player.h (fields 0x318..0x13BC split, member declarations), config/G4SE01/symbols.txt (renames to
Player_804A6F38 members, incl. fn_80248604, fn_8024D134, fn_8024E3D8 of tu_8023FBF0), config/G4SE01/gen_game.tsv (tu_8027E5BC row removed).

## 2026-11-05 Opus 5.5 literal-order pass
Baseline total 8180 -> 8188; unit 28/73 -> 36/73 (honest report).
Written: fn_8027EEB4 (99.31 %, regalloc only, FIXME), fn_8027F1C8 (100 %), fn_8027F854 (100 %, with the two
.rodata offset tables lbl_8046BF98 / lbl_8046C018 as file statics). With these the .sdata2 pool lines up through
1.0f / the 0x4330 double, which flipped fn_802810E4, fn_80281E84, fn_80282958, fn_80282A24, fn_80283848,
fn_80284328 (reloc-name only before).
Pool facts for the next agent: the original pool is creation order (the int->float double is created at the end
of the function that first converts). 0x80540378 (1.5f) and 0x80540380 (3.0f) are created before their first
visible use (80280B30 / 80280074): probably dropped by the optimizer in fn_8027FA08 (which creates 0.5f, 4.0f).
fn_80284430 (4.0f) only lines up once fn_8027FA08 exists and those two sit in front of it.
fn_8027EEB4: the per-component epsilon compare is two nested bool inlines in TUtil::epsilonEquals style
(bool equal = false; if (...) equal = true;), which gives the original's three materialised bools.
SHARED-EDITS: include/game/Player.h (fields 0x11B4 TVec3f, 0x1388 JPABaseEmitter*; members fn_8027EEB4,
fn_8027F1C8, fn_8027F854, fn_802487FC (tu_8023FBF0); free fn_8022C6AC, fn_802341D0 (tu_8022AD64));
include/game/cls_804A71A8.h (0xA0 s32, 0xB7 u8; owner tu_802684A4); symbols.txt renames fn_8022C6AC__Fl,
fn_802341D0__Fl, fn_802487FC__15Player_804A6F38Fv and the three new members.

## 2026-11-05 Opus 5.5 round 7 (agent K)
Baseline total 8539 -> 8548 (+9); unit 36/73 -> 45/73 (honest report). 100 %: fn_8027EB04, fn_80280DD8,
fn_802811C0, fn_80281760, fn_80281A18, fn_80281A90, fn_80281D50, fn_80282E6C, fn_802857F0.
Structurally written (FIXME / reloc-name): fn_8027FA08 99.94, fn_802808E8 99.97, fn_80281E98 99.65,
fn_80282094 99.41, fn_8028538C 99.90 (stack slots), fn_80283D2C 99.38 (regs), fn_802859C4 99.54 and
fn_80285690 98.27 (fn_8022C5EC / fn_8022C52C declared s32, return u32; fix lives in generator-owned tu_8022AD64).
Pool blocker: 1.5 / 0.5 / 3.0 sit before fn_8027FA08's 4.0 (likely a dead-stripped function with the state-3
walk's literal order); every later literal user stays reloc-name until it is found. Details: docs/PLAYER.md Round 7.
SHARED-EDITS: include/game/Player.h (0x580 TBox2f carved; members fn_80252F48 (tu_8023FBF0), fn_80275D9C
(tu_8026FC20), fn_802808E8; extern "C" fn_8022CFE8, fn_8022C76C, fn_8022C82C (tu_8022AD64, unwritten));
config/G4SE01/symbols.txt renames fn_80252F48__15Player_804A6F38Fv, fn_80275D9C__15Player_804A6F38Fv,
fn_802808E8__15Player_804A6F38Ff.

## 2026-11-05 Opus 5.5 round 12 (agent U)
Baseline total 8693 -> 8715 (+22); unit 45/73 -> 67/73 (honest report); all 73 functions written, .rodata 100 %.
Pool blocker solved: followStep() (dead-stripped, inlined by fn_8028538C; the state 3 movement) defined
between fn_8027F854 and fn_8027FA08 creates 1.5 / 0.5 / 3.0 in the original order and gives fn_8028538C its
stack slots (fn_8028538C, fn_8027FA08, fn_80284430 -> 100 %).
100 %: fn_80280074, fn_802804E0, fn_80280B30, fn_80280E44, fn_80282ACC, fn_80283948, fn_80283E70, fn_802844F8,
fn_80284978, fn_80284E04, fn_80285098, fn_8028538C, fn_8027FA08, fn_80284430, fn_80285690, fn_802859C4 and
the reloc-name ones (fn_802808E8, fn_80281E98, fn_80282094, fn_8028414C, fn_80284294, fn_8028434C).
Structural (FIXME in source): fn_8028126C 99.92 (r29/r30 swapped), fn_80282178 99.33 (addi r4 = &mPos hoisted
above the layer compare before fn_802F601C), fn_80282FE0 99.25 (TVec2f distance: original stores the vector
and squares unfused), plus the older fn_8027E7E0 98.85, fn_8027EEB4 99.31, fn_80283D2C 99.44.
.sdata2 98.99 %: only lbl_80540408's 4-byte tail (symbols.txt size 8; probably the next unit's alignment gap).
SHARED-EDITS: include/game/Player.h (members followStep, fn_80280074, fn_802804E0, fn_80242DAC (tu_8023FBF0));
config/G4SE01/symbols.txt (fn_80280074 / fn_802804E0 / fn_80242DAC renamed to Player_804A6F38 members;
lbl_8046BDD8 0xA0 split into lbl_8046BDD8 0x30, lbl_8046BE08 0x10, lbl_8046BE18 0x30, lbl_8046BE48 0x30);
docs/PLAYER.md Round 12.
