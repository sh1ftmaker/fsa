# Crack pass

Tricks to revisit at the end; each verified byte-identical with the unit Matching.

Every trick line in the source carries a `FAKEMATCH(crack-pass):` comment. `tools/gate.py`
rejects a Matching unit containing "fakematch", so these units stay NonMatching until the
trick is replaced by an honest fix (or the project decides to accept it).

## JSystem/JAudio/System/JASTrack.cpp (unblocks game/tu_80021840.cpp)

- **Trick:** a dummy function template `JASTrack_orderDummy<T>` at the end of
  `libs/sdk/include/JSystem/JAudio/System/JASTrack.h`, compiled only when
  `JASTRACK_ORDER_DUMMY` is defined (JASTrack.cpp defines it before its first include, so
  no other unit sees it and their `@NNN` labels don't shift), and an unused
  `static void JASTrack_orderDummyUse()` at the end of JASTrack.cpp that calls it. Both are
  unreferenced, so the linker dead-strips them (map: `UNUSED ... JASTrack_orderDummyUse__Fv`,
  `UNUSED ... JASTrack_orderDummy<i>__Fi`).
- **Why:** with `-sym on` MWCC puts weak functions in one `.text` section per defining file,
  ordered by first emission; `__sinit` and implicit dtors join the section of the last thing
  the end-of-TU template pass instantiated. Without the trick that is the memPool_
  definition in JASHeapCtrl.h, so `__sinit` lands after `free`/`__dl` (every `bl` into them
  shifts by 0x18C and the link breaks). A function template from JASTrack.h, instantiated
  after the template statics, moves `__sinit` into JASTrack.h's section right after
  `MoveParam_` ct, which is FSA's order.
- **Honest fix would need:** evidence of how FSA's headers really placed this: e.g. a real
  function template or weak inline in FSA's JASTrack.h that JASTrack.cpp instantiates and an
  earlier-linked unit also emits (searched: no JASTrack.h function exists out of line
  outside JASTrack.o in FSA, no bl from JASTrack to one, nothing in mkdd/TP/SMG JASTrack.h),
  or a different home for the `JASPoolAllocObject_MultiThreaded<T>::memPool_` definition that
  every pool user still sees (see progress/JSystem__JAudio__System__JASTrack.md).
- **Verification (2026-11-03):** JASTrack.cpp Matching: `ninja` + `sha1sum -c
  config/G4SE01/build.sha1` OK (d86b3d9f785566e34264035e140ef3daf3cc19f0). JASTrack.cpp and
  game/tu_80021840.cpp both Matching: OK, same SHA-1; honest report 100 % for both units;
  `tools/gen_game.py --check` clean. Set back to NonMatching with the trick in the source:
  `python3 tools/gate.py --all-units --base origin/matching` prints `gate: ok`.
