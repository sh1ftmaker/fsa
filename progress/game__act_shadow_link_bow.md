# game/act_shadow_link_bow

Baseline: total: 6340 / 11557 functions, 1270548 / 4442032 bytes (28.60 %). After: 6345 / 11557, 1271140 bytes.

Matched (6/8): __dt__, vfunc_0x88, fn_80222668 (state change), init, __ct__, __sinit.
Near: vfunc_0x90 99.85 % (reloc-name: jump table @NNN vs jumptable_804A6C14, .data order, as tu_80221B44).
Resistant: fn_80222730 98.67 % scheduling+regalloc. Original draws y jitter (15) before x jitter (7),
keeps reach/x/bottom in f24/f25/f26 (ours f25/f24/f26 swapped pairs), loads box.f.y before box.i.y;
.sdata2 pool order 32 vs 64. Tried: member assignment, locals, ctor arg orders, decl orders (98.67 best).
SHARED-EDITS: include/game/act/act_octorok_rock.h (field_0x23c s32 at 0x23C, split of the unknown array; evidence: fn_80222730 stores
(s32)(reach/speed) there on the 'OKTM' actor); config gen_game.tsv: removed act_octorok_rock rows (2) and act_shadow_link_bow row (1).

Jump-table order re-check (2026-11-03): same cause as tu_80222CC4 (see its progress file); blocked on the
project-wide `-inline` decision. fn_80222730 (98.67 %) also keeps the unit NonMatching.

Source-organisation re-check (2026-11-03, research/abs-jumptable): no layout under `-inline auto` puts the vtable
first (vtables always end the TU there; jump tables go with their function). Full measurements and the
deferred-vs-inlining contradiction in progress/game__tu_80222CC4.md. Still NonMatching.
