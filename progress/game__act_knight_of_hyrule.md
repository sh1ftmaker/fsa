# game/act_knight_of_hyrule
Matched: init, ~KnightOfHyrule_804AABA8, __sinit_act_knight_of_hyrule_cpp (3/6 functions; .text 98.99%, .rodata/.data/.sdata2/.bss 100%).
Sub-100%:
- vfunc_0x10 99.73% regalloc-only: r29/r30 swap for mActorEnd and the mActors[i] byte offset.
- draw 99.32% different: getActorManager_801F856C()->mActors[slot] indexing uses addi+lwzx instead of add+lwz 0x8.
- update 98.62% different: getActorManager_801F856C()->mActors[slot] indexing uses addi+lwzx instead of add+lwz 0x8; fn_802303FC's 3rd arg is declared bool in Player.h so it passes 1 instead of 0xFF; and f0..f5 register allocation in the camera box setup.
SHARED-EDITS:
- config/G4SE01/gen_game.tsv: removed generator rows for act_knight_of_hyrule.
- config/G4SE01/symbols.txt: renamed fn_802ED7D8 -> fn_802ED7D8__12cls_804AA350Fv and fn_802ED7F0 -> fn_802ED7F0__12cls_804AA350Ff.
- include/game/act/tu_802ED538.h: declared fn_802ED7D8() and fn_802ED7F0(f32) on cls_804AA350 and split field_0x274..field_0x278 out of padding.
- include/game/GameGlobals.h: split ShadowCircle_804A0160::field_0xd[3] into field_0xd[1], field_0xe, field_0xf.
