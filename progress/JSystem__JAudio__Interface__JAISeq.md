# JSystem/JAudio/Interface/JAISeq.cpp

Baseline: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After:    total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
Unit: 18/18 functions, code 4060/4060, data 84/164 -> 164/164 (honest report).

## Status: NonMatching: report 100 %, but the trial link is NOT byte-identical (weak-function placement)

## Data gaps and fixes (committed)
1. .bss (0 %): placeholder `lbl_804B22E8` (0x40) is the five 0xC
   destructor-chain objects __sinit registers (@900..@904) plus 4 bytes of
   padding. Split into lbl_804B22E8/22F4/2300/230C/2318 (0xC each); also
   matches __sinit's relocations.
2. .sdata2: `lbl_8053AFC8` 0x8 -> 0x4 (0.5f plus padding before JAISeqMgr's
   8-aligned .sdata2 at 0x8053AFD0).

## Blocker: link (codegen / weak-function order, not another unit)
`link_check.py --unit` gives DOL SHA-1 e720eb02... != build.sha1. FSA keeps
JAISeq's copy of `__dt__18JASCriticalSectionFv` at 0x8002449C, *in the middle*
of the unit: right after JAISound_tryDie_ (its first user) and before mixOut_.
Our compile (-sym on, as all JAudio) emits that weak dtor (and the JSULink /
InterruptsDisable::Lock dtors) in their own .text sections, which the linker
places after getTempoMgr, so everything from 0x8002449C on shifts by
0x54 (first diffs: 0x80023ff4 `bl` to the dtor, 0x8002449C prologue, and
game code at 0x80011694/AC/F4/0x80011744 calling 0x8002449C).
Every other JAudio object we build also puts the CS dtor in its own section.
Experiment (reverted, not committed): appending "-sym off" for this object
only emits ~JASCriticalSection right after JAISound_tryDie_ (and the other
weak copies after their first users), and `link_check --unit` reports
byte-identical (SHA-1 d86b3d9f...). Per-unit flags are not allowed, so this
needs a coordinator decision on which JAudio TUs FSA built with -sym on
(JAISeMgr.cpp shows the same thing).

## SHARED-EDITS
- PROPOSED (coordinator, configure.py): JAISeq.cpp (and JAISeMgr.cpp) were not built
  with -sym on in FSA; with "-sym off" the unit links SHA-1 OK and can flip to Matching.
- config/G4SE01/symbols.txt: placeholders lbl_804B22E8 split (5 x 0xC) and lbl_8053AFC8 0x8 -> 0x4 (this unit's own data).

## Resistant
- Link only (see above). All 18 functions and all data are 100 % in the honest report.
