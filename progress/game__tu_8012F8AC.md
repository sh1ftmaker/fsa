# game/tu_8012F8AC

Baseline: total: 5667 / 11557 functions, 1147428 / 4442032 bytes of code (25.83 %)
After:    total: 5669 / 11557 functions, 1147584 / 4442032 bytes of code (25.83 %)

Matched: 4/4 (dtor, update, init, __sinit). Code 800/800. .text/.data/.bss/.ctors 100 %; extab/extabindex 47 % (weak-duplicate entries, report data 348/428).
Left NonMatching in configure.py because of the extab share.
update(): `switch (mState) { case 0: ...; case 1: break; }` (the empty case 1 is what produces the original compare chain).
Removed this unit's two rows from gen_game.tsv.

## SHARED-EDITS
Moved to include/game/tu_8012F3E4.h (new shared header, SHARED-EDIT; owner unit tu_8012F3E4).
- `extern "C" Unk_804BDDFC* fn_8012F554();` guarded-static getter of the object at 0x804BDDFC (s32 count + 32 s32 slots, -1 after ctor); returns its address (lbl_804BDDFC).
- `extern "C" bool fn_8012F3E4(Unk_804BDDFC*);` r3 = that object; loops count slots through findActorBySerial_801F666C and fn_8012F008, true when all pass.
- virtual vfunc_0x30(u8, bool) at vtable 0x30 is called with (0xFF, true); already in ActorBase.h.
