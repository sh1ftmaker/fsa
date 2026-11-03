# game/act_message_check_npc

Baseline: total 5820 / 11557 functions, 1165664 bytes (26.24 %). After: 5825 / 11557, 1166356 (26.26 %).

Matched (6/6): dtor, draw, update, init, ctor, __sinit. Class MessageCheckNPC_8049D0E8 (vtable 0x8049D0E8;
actors.tsv DBMS "Message-check NPC", med).
Resistant: none in .text. extab 60.7 %, extabindex 52.2 %, .data 98.75 %: our object also emits the weak
SpriteAnim_801D267C / J3DFrameCtrl dtors and vtable (stripped at link), shifting @etb/@eti order and the
.data base. Same as tu_8021FDB8.
SHARED-EDITS: config/G4SE01/gen_game.tsv (removed the unit's sinit row); symbols.txt renames of own placeholders.
Next: coordinator trial link, then Matching flag.
