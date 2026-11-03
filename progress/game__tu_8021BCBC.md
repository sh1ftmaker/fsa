# game/tu_8021BCBC.cpp

Baseline: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
Now:      total: 4321 / 11561 functions, 772984 / 4442088 bytes of code (17.40 %)

## Status: 2/3 functions (code 640/748), NonMatching

A 12-byte class `cls_8021BCBC`, named after its first method because it has no
vtable. It has a value (0x0 = table[index] + 0x3B5), an index (0x4, u32, 0..3)
and a counter (0x8). The `.rodata` table {0,1,2,3} is a file static (100 %).
Callers: tu_801E4D54, act_vaati_boss, tu_801D6CEC.

Matched: fn_8021BCBC (step: counter wraps at 4, then index wraps at 4),
`__sinit` (9 JAudio pools, the same stand-ins as tu_802BA334).

Resistant: fn_8021BD18(int), a switch 0->2, 1->0, 2->1, 3->3, default 0
(92.41 %). Diff:
```
    C                 | li r0, 0x0     insert   (2nd zero for index)
   20  bge 0xa4       | bge 0x98       (case 1 goes straight to the store)
   3C                 | li r0, 0x0     insert   (case 1 block)
```
The original reuses the 0 stored to field_0x8 as the index for case 1 and
the default. Its case 1 block is only a dead `b`. Tried without success: direct
member stores per case, a local initialised before or after the field_0x8
store, `case 1: break;`, the default label grouped with case 1, the
parameter reused, an uninitialised local, and `field_0x4 = field_0x8`.

## symbols.txt (placeholder renames)
- fn_8021BCBC / fn_8021BD18 -> `fn_<addr>__12cls_8021BCBC{Fv,Fi}`
- fn_8021BD84 -> `__sinit_tu_8021BCBC_cpp` (local); lbl_8046A678 scope:local
- lbl_804CDF40 (0x100) -> `@425`..`@433` + nine unnamed 0x10 objects from `lbl_804CDFAC` (the weak memPool_ copies; 4 bytes of padding follow)
- cls_804B21EC pool, guard and dtor, as in game__tu_80457C08 / tu_802BA334

## Data
- extab/extabindex/.bss are blocked by the weak-dtor problem, as in every
  game unit with this sinit (see tu_802BA334).

## SHARED-EDITS
- cls_804B21EC and the JAUDopplerAudible<4> stand-in (see tu_802BA334).
- cls_8021BCBC is used by three other units. It may belong in a shared
  header once its owner is identified.

## After the rebase onto matching (shared headers, 92b4117)
- The JAI/JAU includes and the local cls_804B21EC / Doppler stand-in were
  replaced by `#include "game/GameAudio.h"`. The __sinit still matches.
- `.bss` is now 100 %: the 0x94 tail is split into nine unnamed 0x10 objects,
  which is the real layout.
- Only extab/extabindex are still short (the weak JASMemPool dtors). The
  report tooling will handle that.
