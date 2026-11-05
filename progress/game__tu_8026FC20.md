
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
- fn_8027A0D4 passes &field_0x130c as the particle callback argument of spawnEmitter_8013CC50 (type unknown).
SHARED-EDITS: include/game/Player.h only (fields split out of padding arrays, member declarations for the unmangled
functions of this unit, `class JPABaseEmitter;` forward declaration).
Symbol renames (config/G4SE01/symbols.txt): unmangled fn_80xxxxxx of this unit to ...__15Player_804A6F38F...

## 2026-11-05 round 4, agent E (Opus 5.5)
Rebased onto matching (baseline 8416 / 11557). After: 8429 (unit 54 -> 67 of 120).
New 100 %: fn_8026FD14, fn_80270088, fn_80270360, fn_802724E0, fn_80272DCC, fn_80272E98, fn_802730D8,
fn_802732C4, fn_80273E7C, fn_80274238, fn_80274728, fn_80274988, fn_80277EBC.
Structurally done / close (FIXMEs in the source):
- fn_80270ABC 98.26 scheduling (const this hoist; callees in tu_8023FBF0 would need const)
- fn_802738F0 99.93, fn_80276538 99.56, fn_80276078 99.57 reloc-name only (literal pool, see below)
- fn_80273518 99.74 regalloc + reloc-name
- fn_80274238 now 100 % (u8 return, review fix; 13 new honest in total).
- fn_80273C30 94.20 / fn_80273D44 94.87: fn_80256F44 returns u8 in the original; fixing it needs
  tu_80256C74 / Player.h (u8) and tu_8022AD64's forwarder fn_8023AAF8 (u8), else that forwarder drops.
Next: fn_802710C8 (0x1098) structure-first unlocks the .sdata2 order for every later float user
(fn_80272224, fn_80272C20, fn_80273FB8, fn_80276538 ...). Then fn_80273FB8, fn_80272224, fn_80272768,
fn_802728AC (rodata float tables 0x8046BD68 / BD48), fn_8027BC90, fn_8027CBB0.
SHARED-EDITS: include/game/Player.h only: TBox2f fields 0x4A0, 0x4B0, 0x5DC, 0x5EC (from u8 padding,
size unchanged), member declarations (fn_80270088, fn_80270360, fn_802738F0, fn_80273E7C (u8),
fn_80274728, fn_80274988, fn_802765C0), extern "C" bool fn_8023434C(s32) (owner tu_8022AD64).
Symbol renames: fn_80270088, fn_80270360, fn_802738F0, fn_80273E7C, fn_80274728, fn_80274988,
fn_802765C0 -> Player_804A6F38 members.

## 2026-11-05 round 5, agent G (Opus 5.5)
Rebased onto matching (8475). After: 8482 (unit 67 -> 74 of 120), structural +6.
New 100 %: fn_802738F0 (literal pool), fn_80270948, fn_80270DDC, fn_80270F00, fn_80272224,
fn_80272768, fn_802728AC. fn_80273518 now regalloc-only (99.82).
Structurally close: fn_802710C8 98.54 (4 dead stfs of the knockback TVec2 missing, mPos x/y load
order in the angle copies, regalloc), fn_80272C20 97.24 (extra `i >= 8` after the point loop).
Unwritten: fn_8027057C (inlines fn_80270DDC defined after it; see docs/PLAYER.md round 5).
SHARED-EDITS: include/game/Player.h (isDown() inline member decl; fn_802710C8, fn_80270DDC,
fn_80270948, fn_8027057C, fn_802728AC member decls; 0xD10 u8[12] -> TVec3f;
extern "C" fn_80236468(s32, s32) owner tu_8022AD64); include/game/tu_80379F84.h (SaveRow 0x7A[4],
0x7E, 0x7F, 0x80 from 0x7A[7], size unchanged).
Symbol renames: fn_802710C8, fn_80270948, fn_80270DDC, fn_8027057C, fn_802728AC -> Player members.
