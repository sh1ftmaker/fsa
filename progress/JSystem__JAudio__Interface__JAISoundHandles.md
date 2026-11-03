# JSystem/JAudio/Interface/JAISoundHandles.cpp

Baseline (progress.py): total 4784 / 11561 functions, 863464 / 4442088 bytes (19.44 %).
After: total 4785 / 11561 functions, 863676 bytes (19.44 %). gate: ok.

Unit: 3/6 -> 4/4 functions (two left the range, see SHARED-EDITS); .text, .ctors,
.bss 100 %. `link_check --unit` SHA-1 OK; flipped to Matching.

## Matched
- fn_80026158 = `__sinit_JAISoundHandles_cpp` (the unit's .ctors entry; builds the
  TChannelMgr/JASTrack/JASChannel memPool_s, as mkdd's __sinit_JAISoundHandles_cpp).
- .bss 0x804B23B0-0x804B23D8 split into this unit (its __register_global_object
  chain); placeholder lbl_804B23B0 (0x28) split into three 0xC placeholders
  lbl_804B23B0/BC/C8 (+4 bytes alignment pad).
- getHandleSoundID / getHandleUserData: the `JAISoundHandle *handle` temporaries
  marked "fakematch" are gone; tp's/Petari's plain `handles_[i]` form matches.
  getHandleUserData and stopAllSounds are dead-stripped in FSA (range is
  getHandleSoundID+getFreeHandle+setPos = 0xF8 bytes).

## Resistant
None.

## SHARED-EDITS
- JAISoundInfo.cpp is its own TU in FSA (as in mkdd: JAISoundHandles, then
  JAISoundInfo): `.text 0x8002622C-0x800262AC` (fn_8002622C =
  `__ct__12JAISoundInfoFb`, sets lbl_80491808 and JASGlobalInstance<JAISoundInfo>
  ::sInstance; fn_80026248 = `__dt__12JAISoundInfoFv`, clears sInstance if it is
  this), `.data 0x80491808-0x80491830` (lbl_80491808 = `__vt__12JAISoundInfo`).
  I trimmed JAISoundHandles' .text to end at 0x8002622C; the coordinator should add a
  JAISoundInfo.cpp unit (libs/jsystem source exists; FSA's dtor has no assert) with
  that stanza.
