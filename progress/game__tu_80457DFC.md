
## 2026-11-05 small-function pass
Baseline 7964 functions (unit 1/33). After: 7977 (unit 14/33, +13).
Matched: fn_80457DFC, fn_80457E04, fn_80457F60, fn_80457F6C, fn_80458090, fn_804580DC (bool), fn_8045873C, fn_80458F9C, fn_804594E4, fn_8045959C, fn_804595E4, ctor, fn_8045A0D8.
Resistant: fn_80459624 79.35 % (scheduling+regalloc: original stores each coordinate right after its load; ours loads x and z first). Tried TVec3f/Vec param, non-const pointer, TVec3f member set().
Not written: fn_8045A0E4 (JASPoolAllocObject<JAUDopplerAudible<4>>::newMemPool(int); MWCC rejects explicit member instantiation; it will appear when fn_80459AE4, its caller, is written).
SHARED-EDITS: include/game/GameGlobals.h AudioMgr_8045A080 fields named (0x0..0x8d), forward decls, member decls for the functions above; fn_804580DC returns bool. symbols.txt renamed 10 placeholders to mangled member names. gen_game.tsv row removed.
Next: the rest of the unit (fn_80457E54, 80457F80, 8045815C, ...).

## 2026-11-05 leverage wave 3 (Opus)
Baseline 8491 (structural 8888). After: 8495 (+4), structural 8893 (+5). Unit 18/33.
What it is: the game's audio manager (Twilight Princess Z2AudioMgr's role): .sdata points at
"GC4Sword_usa/AudioRes/JaiInit.baa", ".../Waves/", ".../Seqs/JaiSeqs.arc"; owns JAUSoundMgr (0x30),
JAISeMgr (0x34), the audience (0x3C), section heap (0x44); music handle 0x80, second sequence 0x84,
system-sound objects 0x14 (shared) and 0x18[4] (per player).
Header moved: AudioMgr_8045A080 now lives in include/game/Unk_8045A080.h, included at the end of
game/GameAudio.h (needs JAISound.h for the JAISoundHandle members); GameGlobals.h keeps the forward
declaration and gAudioMgr_8053AF80. Unit .sdata written (20 tuning values + 3 string pointers, 100 %).
Matched: fn_80457F80 (bool, JAISoundID, s8), fn_80458408, fn_80458F04, fn_80459508 (member, renamed).
Structural: fn_80458508 98.55 missing/extra (switch tree pivot 1 vs 2); fn_80458658 99.65 regalloc;
fn_80458744 99.81 reloc-name (jumptable_804B1EF0; tables defined at file end so they are addressed
per symbol as in the original, which puts them after the jump tables in .data); fn_80458FF0 99.91
reloc-name (jumptable_804B1FB4, same cause; inlines fn_80458408).
Left unwritten, with reasons:
- fn_80457E54, fn_8045815C: they inline fn_80458508 (defined later; sdata tempo read after the
  getTempoMgr call, per-branch handle check). Drafts in the scratchpad were 81.5 / 91.8 %.
- fn_80459274, fn_804593D4: with fn_80458744 written MWCC inlines it into both; the original calls it.
  Bodies are simple (category pause / moveVolume 3); unknown why the original did not inline.
- fn_80458880 (393 callers): inlines fn_80458F04 (defined later). Prototype corrected to (u32, s8):
  extsb on entry, cmplwi equality tests; symbol renamed FUlSc. No caller changed %.
- fn_80459674 (per-frame update; calls 8880, inlines 959C and the music-start block shared with
  fn_80458FF0's else branch: a dead-stripped helper candidate). fn_80459AE4, the audience class
  (fn_8045A338 dtor, fn_8045A3D8; see GameAudio.h FIXME), fn_8045A0E4, __sinit.
Fields: field_0x8b is bool (fn_80458FF0 passes it to fn_8045815C without conversion).
SHARED-EDITS: include/game/GameGlobals.h (struct moved out), include/game/GameAudio.h (+include),
new include/game/Unk_8045A080.h; symbols.txt renames (4 lines above).

## 2026-11-05 leverage wave 10 (Opus): AudioMgr round 2
Baseline 8652 (honest). Unit 18/33 -> 19/33.
Inlining resolved (reviewer's lead): fn_80458508 and fn_80458F04 are inlined into earlier
functions (fn_80457E54, fn_8045815C; fn_80458880) while 7 / 6 other units call them out of
line. Tested on MWCC GC/2.6 -inline auto: a later definition is never inlined, whether the
class declaration and/or the definition say `inline` (no copy emitted either), so the bodies
were inline ahead of their users. Written as the playerFlags_802413F0 / fn_802413F0
precedent: inline members moveMusicTempo_80458508 / startSecondSeq_80458F04 defined at the
top of the .cpp (declared in the class; not in the widely included header, per RULES_CARD),
and fn_80458508 / fn_80458F04 call them (unchanged %: 98.55 / 100).
Written:
- fn_80457E54 100 % (inlines moveMusicTempo_80458508).
- fn_8045815C 99.97 % reloc-name only (0.5f pooled after 0.7f, used first by unwritten
  fn_80459674). Needed JAISound::getID() by value (TP JAudio2 form; mkdd has const&): the
  original copies the ID through a temporary. SHARED-EDIT libs/sdk/include/.../JAISound.h.
- fn_80458880 (393 callers) 99.66 % missing/extra (1 instr) + reloc-name. Two more inlines
  proven by the code: getPlayerPan (re-tests player < 0 inside the player >= 0 branch, and
  &aux is computed before the pan) and stopSounds13To1C (its ten JAISoundID temporaries take
  the lowest stack slots, below fn_80458880's own and startSecondSeq's, as inlined bodies'
  do). Left: case 1 / default of getPlayerPan reload 0.5f in the original. Try budget spent.
Not written:
- fn_80459274, fn_804593D4: bodies drafted (category pause / moveVolume 3, structure clear),
  but MWCC auto-inlines fn_80458744 into them. Probe: adding one statement to fn_80458744
  still inlines, two stop it, so the original fn_80458744 is a little longer by MWCC's
  measure; static local tables, a table pointer, extra case labels do not change it. Without
  a natural longer form, left unwritten.
- -inline deferred (rejected anyway) reverses function order and inlines fn_80458744 into
  fn_80457E04, which the original calls: not the original's mode.
SHARED-EDITS: include/game/Unk_8045A080.h (inline decls moveMusicTempo_80458508,
startSecondSeq_80458F04, getPlayerPan; comments), libs/sdk/include/JSystem/JAudio/Interface/
JAISound.h (getID by value).
