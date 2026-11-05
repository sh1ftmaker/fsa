# game/tu_8045BA84.cpp

Rebased onto matching (e8fc2c4) and switched to the shared headers.

## Status: 2/2 functions, .text/.ctors/.bss 100 %, NonMatching

- `fn_8045BA84__FPC3VecP3Vec`:
  `lbl_8053AB10->field_0x24->fn_8030C5F0(pos, true, true, out)`, using
  `GameRoot2_8049F108` and `cls_8030C058` from `game/GameGlobals.h`.
- `__sinit_tu_8045BA84_cpp`: the nine pools from `game/GameAudio.h`. The
  Doppler pool comes from that header's stand-in, which replaces the old
  FIXME.
- .bss 0x8052E088..0x8052E184: `@415`..`@423` (9 x 0xC destructor chain),
  then the nine weak memPool_ copies as `lbl_8052E0F4`..`lbl_8052E174` (0x10 each).
- extab/extabindex 30.77 %: these are entries for the deduplicated weak pool
  dtors. Per the coordinator, report tooling will handle them, so the flag
  stays NonMatching until then.
