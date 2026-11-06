# game/tu_801EE24C.cpp (ActorBase_804A5770), hdr/actor branch

Matched (honest): ~ActorBase_804A5770 (0x1A4), draw, savePrevPos, update,
vfunc_0x10, init.

Resistant:
- __ct__18ActorBase_804A5770Fv 99.86 %: only its .sdata2 float literals (@NNN vs
  lbl_8053F740/F774/F7F4/F7A4/F7F8) differ; they pair once the unit's other
  functions put .sdata2 in the original order. lbl_8053F7F8 (40.0f) is
  size 0x8 in symbols.txt (40.0f plus a zero pad word): it needs a size fix
  (approved_symbol_changes or a placeholder split) for the literal to pair.
- vfunc_0x18__18ActorBase_804A5770Fv (fn_801F5790) 99.23 %: float register allocation
  (player z f5 vs f7) besides the literals.

Symbols renamed: the functions above, __vt__18ActorBase_804A5770 (0x804A5770),
__ct__20ActorSub1DC_801EAD68Fv (0x801EAD68), __ct__Q29JGeometry8TVec3<f>Fv
(0x8003B2CC, the weak empty ctor __construct_array uses for field_0x1ac).

Next: the remaining ~95 functions of the unit (vfunc_0x24..0x50 first),
then the .sdata2 order resolves the ctor.

## 2026-11-05 pass (Sonnet 5.5)
Baseline 8112 -> 8121. Written: fn_801EE3C0 (now C++ free fn_801EE3C0__FUs), fn_801EE508, fn_801EF510, fn_801F3128, fn_801F317C (const), fn_801F31C4, fn_801F3518, fn_801F355C, fn_801F35AC, fn_801F3604, fn_801F3668, fn_801F4694 (const).
Resistant: fn_801EE3C0 78.6 % (target truncates each `tile - lo` with clrlwi; ours extends the u16 once). fn_801EF510 / fn_801F3128 / fn_801F4694: instructions identical, only rodata/sdata2 relocation names (lbl_80469FC8, lbl_8053F74C, lbl_80469F38) pending the unit's data order.
SHARED-EDITS: include/game/ActorBase.h (fn_801EE3C0 moved out of extern "C"; fn_801F4694 / fn_801F317C const); symbols.txt renames fn_801EE3C0__FUs, fn_801F4694...CFv, fn_801F317C...CF....
Next: releaseEmitter_801EE434 (0xBC), fn_801F2618, fn_801F2FAC, fn_801F32F4, vfunc_0x50, then the larger ones.

## 2026-11-05 leverage wave 1 (Opus 5.5)
Baseline 8397 -> 8401 (+4 honest). 100 %: releaseEmitter_801EE434 (now C++
releaseEmitter_801EE434__FP14JPABaseEmitter; it replaces the destructor's inline copy, the
destructor inlines it), fn_801F0D20, fn_801F32F4, fn_801F33C4.
Structural (FIXME each): fn_801EE24C 99.95 reloc-name (200.0f), startSound_801F0E34
99.25 regalloc, fn_801F2B7C 98.2 regalloc+reloc (inlines fn_801F2740),
fn_801F2DB4 99.4, fn_801F2618 98.7, fn_801F2FAC 98.6 (bool narrowed with
clrlwi at the return: a u8 return type matches the bodies 100 % but lowers ~20
bool-using callers, EnemyBase vfunc_0x60/0x64 etc.; left bool), fn_801F305C
97.9 (inlines fn_801F2FAC), vfunc_0x50 94.4 regalloc, fn_801F2740 85.0 (an
unexplained clrlwi at the return + prologue scheduling).
Constructor: cannot reach 100 % until the whole .sdata2 is in order. The pool
starts with four aggregate objects emitted before the literals and in reverse
function order (lbl_8053F728/72C 'NULL' words from fn_801F52A4, lbl_8053F730
s16[4] and lbl_8053F738 u8[4] from fn_801EF830; .rodata shows the same reverse
order), then the floats in first-use order; objdiff pairs @NNN literals by
offset, so fn_801EF830 and fn_801F52A4 (and every literal-introducing
function before the ctor) must be written first.
SHARED-EDITS: symbols.txt releaseEmitter_801EE434 -> releaseEmitter_801EE434__FP14JPABaseEmitter,
fn_8022BB48 -> fn_8022BB48__FlPCQ29JGeometry8TVec3<f>; Player.h declares
fn_8022BB48; ActorBase.h startSound_801F0E34 returns JAISoundHandle*, releaseEmitter_801EE434 out
of extern "C"; tools/gen_game.py EMIT_RELEASE (protected: needs approval).
Next: fn_801EE558 / fn_801EE8F4 (literal order), fn_801EEC98, drawSprite_801F06F0
(SpriteParams copy), fn_801F285C, fn_801F37AC, fn_801EF830, fn_801F52A4.

## 2026-11-05 leverage wave 7, ActorBase round 2 (Opus 5.5)
Baseline 8623 honest / 9046 structural -> 8623 / 9050 (+0 honest, +4 structural); no
function anywhere drops (compared by address against the main report).
Written (all with FIXMEs):
- fn_801EF830 93.3 % missing/extra (now C++ fn_801EF830__FlPCQ29JGeometry8TVec3<f>P21SpriteParams_801F5AE0lPCclPv):
  it draws the actor's SHADOW ("kage_sob.bin" frame kind scaled by lbl_80469FA8[kind],
  kind 7 a flattened copy of the sprite, tile 0x39 an animated "enemy_both2_sob.bin" frame).
  One extra instruction: MWCC hoists the .rodata base (table + both strings) into r30, the
  original addresses each with its own lis/addi; params 0x1C/0x1D copy order. Tried: table
  file-scope / extern / local, a drawSob inline helper (no change).
- fn_801F52A4 99.1 % regalloc-only (now member fn_801F52A4__18ActorBase_804A5770Fv): item
  drops by EnemyParam "dasu" (lbl_80469D48 chance, lbl_80469D78 [12][8] drops as
  DropKind_801F52A4 enum values). Array initialisers: the original has six separate 4-byte
  objects (lbl_8053F728/72C 'NULL' first in .sdata2; lbl_80543F40-4C zero in .sbss2, not yet
  in this unit's splits); MWCC emits one 8-byte @NNN per array here. Unresolved; file-scope
  consts fold.
- fn_801EE558 99.4 / fn_801EE8F4 99.6 scheduling+regalloc (pos load order in the prologue).
  They inline fn_801F320C: new static inline isPlayerOnLayer (fn_801F320C now calls it),
  following the existing isPlayerBelowLayer8 precedent.
- vfunc_0x28 99.7, fn_801F2A10 99.9: instructions identical, literal names only.
Data: lbl_8053F730 u16[4] / lbl_8053F738 u8[4] are file-scope statics: that puts them at the
start of .sdata2 as in the original (function-local statics land in function order).
Ctor: still needs every literal user before it (all floats up to lbl_8053F7F8) in order; not
reachable this round.
Not written: drawSprite_801F06F0 (copies *sp.mMtx into a stack Mtx after PSMTXIdentity with a
12-word struct copy: the Mtx wrapper type is unknown), fn_801EEC98, the rest.
SHARED-EDITS: symbols.txt renames fn_801EF830 -> C++ name, fn_801F52A4 -> member;
ActorBase.h (fn_801EF830 out of extern "C", fn_801F52A4 member, comments, fn_801F2A10 param
names); Player.h declares extern "C" bool fn_8022D534(s32) (owner tu_8022AD64; no fields
touched); libs/sdk JMath.h TSinCosTable::cosDegree (from tp JMATrigonometric.h).

## 2026-11-05 leverage wave 13, ActorBase round 3 (Opus 5.5, deferred inlining)
Baseline 8731 honest -> 8732 (+1); no function anywhere drops (by address
against the round's baseline report).
Written:
- fn_801EEC98 100 % (104 callers; layerBox for the view box, std::sqrtf).
- fn_801F11B4 99.4 regalloc, vfunc_0x24 99.75 regalloc (death), vfunc_0x2C 98.8
  regalloc (boss death explosion), fn_801F285C 88.8 scheduling (box before the
  register saves, as fn_801F2740), fn_801F3D94 99.0 missing/extra 1 (hit
  reaction), fn_801F37AC 94.4 missing/extra (hit handler: .rodata base hoist +
  bool narrowing), fn_801F23BC 98.7 (bool narrowing only).
- lbl_80469EF8 (hurt voices) as data; fn_801F4694 now pairs (rodata offsets).
Simplified: isPlayerBelowLayer8 / isPlayerOnActorLayer inline workarounds are
gone; fn_801F329C (now const, symbols.txt renamed) and fn_801F320C are plain
functions their callers inline under deferred, every % kept.
Re-measured under deferred, unchanged: the ctor (99.91, reloc-name only: 1.0f
needs fn_801EF574 written first, 6.0f/doubles drawSprite, ...); the bool
narrowing family (fn_801F2618, fn_801F2FAC, fn_801F2DB4, fn_801F305C, now also
fn_801F23BC, fn_801F37AC): u8 returns make all five bodies 100 % (fn_801F305C
with `u8 hit; if (hit == false)`), but 15 written callers drop (EnemyBase
vfunc_0x5C/0x60/0x64 100 -> 84-86, Terrorpin/Beamos/Thief vfunc_0x60/0x64,
act_sword_soldier fn_801545C0, several update()s): needs a joint retype of
those virtuals; experiment saved, not merged.
Shared headers: isStageFlagSet / fn_8019CE40 return BOOL (cmpwi at every
caller; fn_8019CE40 stays 100, Player fn_80254744 98.72 -> 98.83);
ActorManager::fn_801F6718 takes s32 a/b/c (callers pass raw words; callee 100,
symbols.txt renamed).
Not written (drafts in the round's scratchpad): drawSprite_801F06F0,
fn_801EF574, fn_801F02BC, fn_801EFD34 all copy a SpriteParams_801F5AE0 from
*params into a stack one that is later passed by address. The original loads
every field first (with spills) and then stores; ours interleaves load/store,
which shifts registers and the frame (60-75 %). With the local's address not
passed to any call ours does load-all too, so MWCC's alias rule differs from
the original here; tried operator=, copy-ctor, temporaries, const/restrict
source, reference parameters, a TColor copy ctor: no change. drawSprite also
copies *sp.mMtx with a 12-word struct copy (a struct-of-Mtx type; no
candidate in the headers without a cast).
.rodata: fn_801F37AC and fn_801EF830 address each table with its own lis/addi
where ours hoists ...rodata.0; with the tables extern the hoist goes away, so
the tables (0x80469D48-0x8046A048) probably belong to another unit (split
change, not done).
SHARED-EDITS: ActorBase.h (fn_801F329C const, fn_801F172C member decl,
fn_801F23BC param names to/from, hit-field comments, docs); EnemyBase.h
(fn_801CB5CC extern "C", fn_801CCDA4); StageFlag.h + tu_80379F84.h +
tu_80197070.cpp (BOOL); ActorManager.h + tu_801F6058.cpp (fn_801F6718 ints);
symbols.txt (fn_801F329C CFl, fn_801F172C member, fn_801F6718 FllPC...ll).
