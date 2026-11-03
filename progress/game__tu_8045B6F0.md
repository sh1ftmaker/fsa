# game/tu_8045B6F0.cpp

Rebased onto matching (e8fc2c4): the thunk moved to tu_8045B010, so .text is
0x8045B6F8..0x8045BA84, and the unit owns .bss 0x8052DEE0..0x8052E088.

## Status: 1/1 function, .text/.ctors/.bss 100 %, NonMatching

- `__sinit_tu_8045B6F0_cpp` (was `fn_8045B6F8`): a global table of 49
  `JAISoundID`s, `lbl_8052DEE0`, dynamically initialized. tu_8045B010's
  fn_8045B030 reads it as `table[(u8)i]`. The unit also has the eight pools of
  `game/GameAudioBase.h`.
- .bss: `lbl_8052DEE0` (0xC4), `@379`..`@386` (destructor chain), then
  the eight weak memPool_ copies as `lbl_8052E004`..`lbl_8052E074` (0x10
  each). The last 4 bytes, up to 0x8052E088, are padding.
- extab/extabindex 20 %: these are entries for the deduplicated weak pool
  dtors. Per the coordinator, report tooling will handle them, so the flag
  stays NonMatching until then.
