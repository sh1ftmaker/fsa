
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
