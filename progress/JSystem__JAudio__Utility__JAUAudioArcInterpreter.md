# JSystem/JAudio/Utility/JAUAudioArcInterpreter.cpp

Baseline (progress.py): 4501 / 11561 functions, 808092 bytes (18.19 %).
After: 4519 / 11561, 810228 bytes (18.24 %), including the cherry-picked
JAUAudioArcLoader commits (+17). gate: ok.

Unit: 4/5 -> 5/5 functions, .text 100 %, .data 97.14 %. Stays NonMatching until
one coordinator approval (below).

## Matched
- readCommand_: FSA has tp's 'vbnk' command (two u32s -> newVoiceBank(waveBankNo,
  bankNo)), between 'bmsa' and 'dsqb'. Needs the newVoiceBank slot in
  JAUAudioArchive.h (commit cherry-picked from unit/JSystem__JAudio__Utility__JAUAudioArcLoader,
  together with that unit's source commit, because the header change removes
  JAUAudioArcLoader_withoutCopy, which the old loader source defined).

## Resistant (data only, needs approval)
FSA's vtable is 0x44 bytes: 0x804953C0 header(2) + dtor + 13 pure slots +
readCommandMore at 0x80495400; the 4 bytes at 0x80495404 are alignment before
JAUAudioArcLoader's vtable at 0x80495408. symbols.txt has the real name with dtk's
size 0x40 and the placeholder lbl_80495400 (0x8) covering readCommandMore + pad.
.data split already extended to 0x80495408.
Tested: with `__vt__22JAUAudioArcInterpreter` size 0x44 and lbl_80495400 removed,
fdiff is 100 % everywhere and gate.py with the unit Matching fails only on
"symbols.txt: __vt__22JAUAudioArcInterpreter size/type changed (0x40,object -> 0x44,object)"
(the link passed). Proposed approved_symbol_changes.txt line:
`__vt__22JAUAudioArcInterpreter  0x40  0x44  <review> (2 header words + dtor + 13 pure + readCommandMore = 0x44; lbl_80495400 held readCommandMore and 4 bytes of padding)`
Then: resize, delete lbl_80495400, flip to Matching.

## SHARED-EDITS
- libs/sdk/include/JSystem/JAudio/JAUAudioArchive.h (same commit as the ArcLoader branch).
- approved_symbol_changes.txt line above (coordinator).
