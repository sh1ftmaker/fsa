# game/tu_801E9AFC.cpp (worker, 2026-11-05)

Baseline 8234 -> 8241 functions (0/14 -> 7/14 counted).

Matched: fn_801E9AFC (getter), ctor, fn_801E9C00 (99.7, literal reloc), fn_801EA304/344,
fn_801EA6D8, fn_801EA75C (99.9), fn_801EA8C0, __sinit.
Structurally close, with FIXME: fn_801E9B58 97.3 (mr r0 copy), fn_801E9C74 91.0 (bool via byte),
fn_801EA384 99.2 (regalloc only), fn_801EA594 89.9 (shift loop, 1 instr).
Unwritten: fn_801EA108 (quad draw; original reloads the sin/cos table per vertex, ours 52 %, copy of
best try in scratchpad u1_with108.cpp).
.sdata2 / .data stay < 100 %: 6.0f at 0x8053F650 belongs to tu_801E6D48/801E801C's pool.
SHARED-EDITS: GameGlobals.h (stubs cls_801E9B54/cls_801E9AFC, fn_801E9AFC, fn_801EA108 removed ->
include/game/tu_801E9AFC.h), ActorBase.h (+fn_801E9B58), act/tu_801E801C.h (+4 members, gen_game rows
removed), act/tu_801E6D48.h comment, tu_801E6D48.cpp (calls the member). Symbols renamed to mangled names.
