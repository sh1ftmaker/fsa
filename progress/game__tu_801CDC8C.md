# game/tu_801CDC8C checkpoint
Baseline: total: 6007 / 11557 functions, 1208940 / 4442032 bytes (27.22 %). Unit 25/57 functions.
G4 revisit: both 10430.378f conversions (fn_801D0280 73.67->74.08, fn_801D02E0 44.77->45.23) now use the rules-card shape. Neither flips to 100 %: remaining diff is scheduling (target loads mPos/pos before the atanTable_ lis/addi) plus .sdata2 literal order.
Tried (fn_801D02E0): named dx/dy locals, no change, reverted.
Resistant: all 12 FIXMEs in the file unchanged; no further tries (budget focus on angle conversions).
SHARED-EDITS: none.
# leverage wave 1 (2026-11-05)
init (99.64) and __ct__ (99.87): only reloc-name differs (lbl_8053F010 / lbl_8053EFDC vs @NNN). The .sdata2 literal order follows first use over the whole unit (8.0f is first used by the unwritten vfunc_0x80). The code is identical; no honest fix until the 17 unwritten functions exist. No change.
# leverage wave 4 (2026-11-05, Opus)
Baseline total 8539 / 11557; after 8545 (+6). Unit 31 -> 37/57; every function now written; .sdata2 100 %.
Wrote the 17 missing functions. The player searches are inlined into earlier functions, so their bodies
are inline helpers (canTargetPlayer, isPlayerOnLayer, nearestPlayer, playerDistSq, isPlayerNear,
directionTo, velocityToward, landStep) used by both the exported function and its inliners.
Flipped: init, __ct__, fn_801CFCF0 (literal order), fn_801CEA60, fn_801CF798, fn_801D0E58.
Structurally done (regalloc/scheduling/operands): vfunc_0x80 99.9, fn_801CE800 99.8 (frame: TVec2f-sized
slots), fn_801CECF4 99.7, fn_801CF0F4 89.7, fn_801CFD68 99.4, fn_801D00B4 88.9, fn_801D0338 99.3,
fn_801D05F8 98.9, fn_801D08B4 99.7, fn_801D0B44 99.9.
Close: fn_801CF1B0 99.2, fn_801CFA70 98.5 (layer copy), fn_801CF888 95.8 (bool materialised via mfcr),
fn_801CF144 61.7 (1 extra mr reorders the body; structure right, needed for the 16.0f literal).
Lead: fn_801CFA70 / fn_801CF888 / fn_802265FC / fn_801CCD34 probably take TVec2f in the original.
Tried: canTargetPlayer bool order (onLayer declared first) - no change.
SHARED-EDITS: EnemyBase.h (inline helper decls; fn_801CCD34, fn_801CD354, extern "C" fn_801CCFE4 decls).
