# game/tu_802BA334.cpp

Baseline: total: 4319 / 11561 functions, 772344 / 4442088 bytes of code (17.39 %)
Now:      total: 4320 / 11561 functions, 772892 / 4442088 bytes of code (17.40 %)

## Status: 1/1 functions (code 548/548), NonMatching (data blocked, see below)

The file's own functions were all dead-stripped. What remains is its `__sinit`,
which constructs the nine JAudio memPools its headers instantiate. The same
sinit appears in about 523 game units. tu_800F39EC is byte-identical.

MWCC emits the guarded pool inits grouped by template (the _MultiThreaded
pools first). Inside each group they come in reverse order of instantiation,
which I checked with small test compiles. So the plain pools have to be
instantiated in this order: JAISe, JAISeq, JAISoundChild, JAIStream,
cls_804B21EC, JAUDopplerAudible<4>. The includes give the first four. The
other two come from local stand-ins marked `TODO(shared)`.

## symbols.txt (placeholder renames)
- `fn_802BA334` -> `__sinit_tu_802BA334_cpp` (local)
- `lbl_804D2768` (0x100) split: `@408`..`@416` (9 x 0xC destructor-chain
  objects, the names our object emits) + nine unnamed 0x10 objects from `lbl_804D27D4` (the weak memPool_ copies; 4 bytes of padding follow)
- Unidentified pool `cls_804B21EC`, named the same way as unit/game__tu_80457C08:
  `lbl_804B21EC`, `lbl_8053A2B0`, `fn_80021BBC` -> its memPool_, guard and dtor.

## Why data cannot reach 100 %
- `lbl_804D27D4` (0x94) is this object's 9 weak duplicate `memPool_` copies
  (9 x 0x10 + 4 pad). The linker keeps the space but resolves the names to the
  first copy in tu_80021840. Naming them would duplicate global names and
  redirect relocations, so they stay unnamed.
- extab/extabindex: our object also has entries for the 9 weak JASMemPool
  dtors, which the original link dropped along with the dtors. Every game
  unit with this sinit has the same problem, so it needs a coordinator or
  report decision.

## SHARED-EDITS
- `cls_804B21EC`: a weak hypothesis is `JAUAudible<4>`. mkdd's map has
  JAUAudible<4> and JAUDopplerAudible<4> as the two JAU pools shared with game
  files, and its other four JAUAudience-only pools correspond to FSA's
  0x8052E78C..BC. Not proven.
- A game sound header that instantiates (in order) JAISe, JAISeq,
  JAISoundChild, JAIStream, cls_804B21EC and JAUDopplerAudible<4> pools. It
  replaces both local stand-ins here and in about 523 game units.
- tu_80457C08's `lbl_8052DB60` (0x80) is that unit's 8 weak memPool_ copies,
  not another unit's data.

## After the rebase onto matching (shared headers, 92b4117)
- The JAI/JAU includes and the local cls_804B21EC / Doppler stand-in were
  replaced by `#include "game/GameAudio.h"`. The __sinit still matches.
- `.bss` is now 100 %: the 0x94 tail is split into nine unnamed 0x10 objects,
  which is the real layout.
- Only extab/extabindex are still short (the weak JASMemPool dtors). The
  report tooling will handle that.

## Wave 6 (2026-11): why the trial link fails; verified unlinkable

Baseline (wave 6): total: 4501 / 11561 functions, 808092 / 4442088 bytes of code (18.19 %)
End:               unchanged (no source, symbol or flag change).

`python3 tools/link_check.py --unit game/tu_802BA334.cpp` gives a different DOL
because MWLD drops the compiled object whole: the trial map names it only
in UNREFERENCED DUPLICATE lines, and every section shrinks by exactly this
unit's splits. Our object has only locals and weak duplicates, so nothing
in it is reachable; dtk's split survives only because dtk's `.comment`
marks all its symbols force-active. FORCEFILES fails with multiply-defined
memPool_ even when the competing copies are weak (also for the Matching
tu_80214244.o), so marking the pools weak in symbols.txt would not help;
no linker version 1.0-2.6 behaves differently. Full experiment log in
docs/COORDINATOR.md ("Dead-stripped units"), committed on
unit/game__tu_802BA334. Unit stays NonMatching; no link-setup change proposed.

SHARED-EDITS: docs/COORDINATOR.md (on unit/game__tu_802BA334 only).
