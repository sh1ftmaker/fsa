# game/tu_8037DF68.cpp (worker, 2026-11-05)

Baseline 8234 -> 8261 functions (0/54 -> 27/54 counted). dol sha1 ok, gate ok (all units / matching), gen_game ok.

Matched: cls_804AF32C dtor,
fn_8037E4BC/E54C/E694/E8D4/E9F4/EB0C/EC98/EE8C/EFB4/F0CC/F1E4, fn_803846A4/BC, fn_80388CAC/EEC,
fn_8038911C/89254/8938C/894BC, fn_8038D454/D870, fn_8038470C (1356 B), fn_80389BF0, fn_80389F78 (1204 B).
Structurally close, with FIXME: fn_8037E7AC 97.2 (scheduling), fn_8037EB4C 97.4 (regalloc),
fn_80383E2C 87.0 (scheduling+regalloc, jump table ok), fn_8038D570 98.8 (li vs mr),
fn_80389964 97.7 (1 instr), fn_803895EC 99.3 (1 instr), fn_8038A42C 99.7 / fn_8038A984 99.7 / fn_8038B8A8 99.98 (regalloc only).
Unwritten: fn_8037F2FC (mTextBox is a bigger class than J2DTextBox, fields 0x104-0x120 unknown),
fn_8038D4A8 (vtable slot 0x94 call on mTextBox: no such virtual in J2DPane/J2DTextBox), fn_8038C60C
(81.7 %: the inlined fn_803846BC loop comes out in another order), fn_8038AD24 / BD08 / C678 (openers
that inline several of the above), the big message functions (> 1200 B).
Notes: auto-inlining of earlier functions works (fn_80388CAC/EEC inline fn_8037F1E4). Loop counters:
`int i` unrolls constant-count loops, `s32 i` (long) does not (found in tu_80403D84).
SHARED-EDITS: Message.h (BMG structs, window/manager fields carved, C++ declarations moved out of
the extern "C" block, fn_803837A4 / fn_80385960 / fn_80390EBC / fn_80390C6C / fn_80391678 declared),
GameGlobals.h (fn_8038D454 / fn_8038D870 out of extern "C", lbl_8053ACD8), tu_80390818.cpp
(fn_80390818 returns u8, local u8: still 100 %). Symbols renamed to the mangled names.

Review (2026-11-05): the constants constructor cls_804AF32C::cls_804AF32C (fn_8037DF68, 1364 B) and the
lbl_8050DB0C definition were REMOVED (the ctor was an asm transcription: ~284 stores into an anonymous
s16 field_0x4c[]). The unit's __sinit is therefore no longer matched (95.7 %, missing the ctor call and
atexit registration). TODO: rewrite the ctor with real field names/types once the class's fields are
understood from the code that reads them (20 units read lbl_8050DB0C). symbols.txt keeps the 0x310 size
and the bss split. The `u16 bits = flags` and `boxPos = pos` copies were tried without and lose
0.3-4 % or the match (fn_80389F78), so they stay with a comment; `(s8)text[4]` was dropped (same %).
