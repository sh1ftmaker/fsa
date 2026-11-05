# game/tu_8037DF68.cpp (worker, 2026-11-05)

Baseline 8234 -> 8258 functions (0/54 -> 24/54 counted). dol sha1 ok, gate ok (all units / matching), gen_game ok.

Matched: cls_804AF32C ctor (1364 B, scripted from the asm: 284 constant stores) and dtor, the
__sinit (bss split like the other sinit units; the object is 0x310 bytes, not dtk's 0x40C),
fn_8037E4BC/E54C/E694/E8D4/E9F4/EB0C/EC98/EE8C/EFB4/F0CC/F1E4, fn_803846A4/BC, fn_80388CAC/EEC,
fn_8038911C/89254/8938C/894BC, fn_8038D454/D870.
Structurally close, with FIXME: fn_8037E7AC 97.2 (scheduling), fn_8037EB4C 97.4 (regalloc),
fn_80383E2C 87.0 (scheduling+regalloc, jump table ok), fn_8038D570 98.8 (li vs mr).
Unwritten: fn_8037F2FC (mTextBox is a bigger class than J2DTextBox, fields 0x104-0x120 unknown),
fn_8038D4A8 (vtable slot 0x94 call on mTextBox: no such virtual in J2DPane/J2DTextBox), fn_8038C60C
(needs fn_8038A984's signature), the big message functions (0x118+ bytes each, > 600 B).
Notes: auto-inlining of earlier functions works (fn_80388CAC/EEC inline fn_8037F1E4). Loop counters:
`int i` unrolls constant-count loops, `s32 i` (long) does not (found in tu_80403D84).
SHARED-EDITS: Message.h (BMG structs, window/manager fields carved, C++ declarations moved out of
the extern "C" block, fn_803837A4 / fn_80385960 / fn_80390EBC / fn_80390C6C / fn_80391678 declared),
GameGlobals.h (fn_8038D454 / fn_8038D870 out of extern "C", lbl_8053ACD8), tu_80390818.cpp
(fn_80390818 returns u8, local u8: still 100 %). Symbols renamed to the mangled names.
