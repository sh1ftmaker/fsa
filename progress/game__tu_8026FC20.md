
## 2026-11-05 small-function pass (Sonnet 5.5)
Baseline progress.py: 7622 / 11557. After: 7669 (unit 7 -> 54 of 120).
Resistant: fn_8027CAA8 78.55 % (scheduling: original stores the float / li 8 before the fn_8024F0E8 argument setup),
fn_802729F0 81.51 % (72-entry compare loop: original unrolls 8 trips of 9 with a pointer bump).
Left unwritten on purpose:
- fn_80276538, fn_8027A0D4-style literal users (fn_80279FE8): new float literals (7, -15, 15, 6) enter our .sdata2 pool
  before 0.0f (second in the original), which turns every later 0.0f user (fn_8027966C, fn_8027CB24...) into reloc-name.
  Write them when the earlier literal users (fn_8026FD14 ...) exist.
- fn_80270948, fn_80270DDC, fn_80270F00: inline fn_80270A8C (defined earlier; the original inlines it) plus more; not tried.
- fn_802771C8, fn_802775C8, fn_8027A178, fn_8027CBB0: stop-emitter sequence on a JPABaseEmitter (+0xF4 |= 1, +0x24 = 1)
  that the TWW JPAEmitter.h layout does not have (see act_force_gem_wooden's FIXME).
- fn_8027A0D4 passes &field_0x130c as the particle callback argument of fn_8013CC50 (type unknown).
SHARED-EDITS: include/game/Player.h only (fields split out of padding arrays, member declarations for the unmangled
functions of this unit, `class JPABaseEmitter;` forward declaration).
Symbol renames (config/G4SE01/symbols.txt): unmangled fn_80xxxxxx of this unit to ...__15Player_804A6F38F...
