# game/tu_801D2C60
Baseline: 5980 / 11557 functions, 1201588 bytes. After: 5986 / 1202768.
Matched: all 7 (dtor, fn_801D2CF0, draw, update, init, ctor, __sinit).
Resistant: none. Remaining sub-100 sections are project-wide (weak extab, vtable pad 0xA4/0xA8, sdata2 pad, @NNN literals).
SHARED-EDITS: include/game/GameGlobals.h (fn_801CB538 decl), include/game/act/tu_801D2C60.h (owner unit), gen_game.tsv row removed, symbols.txt placeholder renames.
Next: coordinator flips Matching after trial link.

## Data re-check (2026-11-03): matched_data 48 % (308/640: only .bss/.ctors/.rodata/.sdata count)
All 102 Matching game units are 100 % in every section, so each of these must go:
1. extab/extabindex 52 % and .text placement: the original emits `__dt__12cls_804A48B0Fv` FIRST
   (0x801D2C60, first extab entry). The dtor is an inline (weak) one: tu_802201D8's derived dtor inlines it
   (stores __vt__12cls_804A48B0, then EnemyBase/J3DFrameCtrl vtables, bl __dt__18ActorBase). Under
   `-inline auto` weak functions go after the ctor (ours: fn_801D2CF0, draw, update, init, ctor, weak
   EnemyBase/J3DFrameCtrl/cls dtors, __sinit). Only `-inline auto,deferred` puts it first (GAME_PLAN section).
   An out-of-line dtor defined first would contradict the derived-class inlining. Also breaks the link SHA-1.
2. .data 99.4 %: `__vt__12cls_804A48B0` is 0xA4 in our object (2 header words + 39 slots, last slot +0xA0 =
   vfunc_0xA0__18EnemyBase); dtk sized it 0xA8 because the next unit's data starts 8-aligned at 0x804A4958
   (+0xA4 is 0, alignment). Needs an approved_symbol_changes.txt line (coordinator-only), same pattern as
   __vt__7J2DPane / __vt__10JAIAudible: `__vt__12cls_804A48B0  0xA8  0xA4`.
3. .sdata2 90.9 %: `lbl_8053F078` is -1.0f (4 bytes) + 4 bytes alignment pad before tu_801D3320's .sdata2
   (8-aligned); dtk sized it 0x8. Same kind of approval: `lbl_8053F078  0x8  0x4`.
Verdict: not flippable under `-inline auto`; 2 and 3 are coordinator resizes with the evidence above.

Source-organisation re-check (2026-11-03, research/abs-jumptable): under `-inline auto` weak (inline) functions are
always emitted after the strong ones, so the leading inline dtor needs deferred codegen, like the jump-table units'
vtable order; see progress/game__tu_80222CC4.md for why deferred itself is ruled out (it inlines later callees the
original keeps out of line). The resizes in item 2/3 above still need coordinator approval (not added by me).

Compiler sweep (2026-11-03, lead/lead-jumptable): no GC 1.0-3.0a5.2 version/flag gives deferred layout with auto
inlining (docs/GAME_PLAN.md). The crack-pass `#pragma defer_codegen on` trick (docs/CRACK_PASS.md) is a candidate for
the leading inline dtor here too; not tried.
