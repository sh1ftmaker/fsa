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
Next: fn_801EE434 (0xBC), fn_801F2618, fn_801F2FAC, fn_801F32F4, vfunc_0x50, then the larger ones.

## 2026-11-05 leverage wave 1 (Opus 5.5)
Baseline 8397 -> 8401 (+4 honest). 100 %: fn_801EE434 (now C++
fn_801EE434__FP14JPABaseEmitter; it replaces the destructor's inline copy, the
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
SHARED-EDITS: symbols.txt fn_801EE434 -> fn_801EE434__FP14JPABaseEmitter,
fn_8022BB48 -> fn_8022BB48__FlPCQ29JGeometry8TVec3<f>; Player.h declares
fn_8022BB48; ActorBase.h startSound_801F0E34 returns JAISoundHandle*, fn_801EE434 out
of extern "C"; tools/gen_game.py EMIT_RELEASE (protected: needs approval).
Next: fn_801EE558 / fn_801EE8F4 (literal order), fn_801EEC98, drawSprite_801F06F0
(SpriteParams copy), fn_801F285C, fn_801F37AC, fn_801EF830, fn_801F52A4.
