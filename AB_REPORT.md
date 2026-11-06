# A/B report: game/tu_803B8C74.cpp (Opus)

Baseline 8776 / 11557 honest functions; after: 8783 / 11557 (+7). Structural
9227 -> 9238 (+11). No function anywhere dropped: I compared every function by
address against fsa-decomp's report_honest.json. Full `ninja` passes
(fsa_game_code.dol SHA-1 OK). `gate.py --all-units` and `gate.py --base faa79b43`
both print ok, and `gen_game.py --check` passes. `order_check` finds no problem in
this unit; it reports 11 units out of order, and main (faa79b43) reports the same 11.

## 100 % (7 of 13)
- fn_803B8C74__FUc: stores the current save file's byte 0x66.
- fn_803B8C90: reads the same byte back.
- fn_803B9120: battle stage open check (stages < 3 always open, otherwise bit 31 of the current file).
- cls_804AFB30::fn_803B933C: resets save file `index` of a buffer through a stack copy.
- __dt__12cls_804AFB30Fv
- fn_803B950C(SaveFile*, SaveFileBody*): clears a file and its body, then calls setDefaults.
- __dt__12cls_804AFB3CFv
- Both vtables (.data) are at 100 %.

## Structurally done (FIXME in source)
- fn_803B90E4: 98.7 %, regalloc-only (file * 0xC8 and the table base swap registers).
- fn_803B8EBC: 96.5 %, regalloc-only.
- fn_803B8CAC: 96.3 %, regalloc-only.
- fn_803B93E8: 92.4 %, scheduling+regalloc (the two destination pointers swap between r30 and r31).

## What the asm proved
- lbl_8050EC80 is one object, cls_804AFB30 (vtable 0x804AFB30, destructor 0x803B94C4), size 0x1FE0:
  - u64 serial at 0x8
  - current file index at 0x10
  - SaveFile_803B950C mFiles[3] (0xC8 each) at 0x20
  - SaveFileBody_803B950C mBodies[3] (0x9C4 each) at 0x280, 32-aligned
- The `__sinit` loop runs fn_803B950C(&mFiles[i], &mBodies[i]) for each file. tu_803BC6F0 calls the same function.
- cls_804AFB3C (vtable 0x804AFB3C) is a second object in the same .bss, at lbl_8050EC80 + 0x1FEC, 0xDC bytes. It holds two 0x45-byte halves, 0x90 apart, plus two s32[8] tables initialised from lbl_80476548 (8 x 10000).
- The flag word is SaveFile+0x08, with bit stage*4+sub. The tests are BOOL inlines with a range check: one for a given file, one for the current file.
- The defaults set bit 2 of stages 0-6 and bit 1 of stage 7, through a 7-iteration loop plus one statement in the inline SaveFile::setDefaults().
- In fn_803B933C and fn_803B93E8, the destination pointer is computed before the memset.
- These real types are in a new header, include/game/tu_803B8C74.h. This unit no longer includes tu_80379F84.h. Other units still read lbl_8050EC80 through the old SaveRow_8050EC80[] view, which works but should be retired in a follow-up.

## Left
- 0x803B9160 (cls_804AFB3C's constructor) is unwritten. It reaches 88.8 % as natural loops, but our build fully unrolls the 16-byte clearing loop, while the original keeps a counted loop unrolled by 8. Its rodata table lbl_80476548 stays unwritten with it.
- __sinit (0x803B961C) is unwritten. To write it, the two objects and their chain nodes must be defined here. That needs these .bss changes in symbols.txt:
  - mark the chain nodes as `@NNN` local symbols at 0x8050EC60 and 0x80510C60
  - add an lbl_80510C6C symbol of size 0xDC
  - resize lbl_8050EC80 to 0x1FE0
- The register differences in fn_803B8CAC and fn_803B90E4 are unresolved.

## SHARED-EDITS
- None to shared headers.
- symbols.txt: this unit's placeholder renames to the mangled names the source emits, and the two vtables renamed `__vt__12cls_804AFB30` / `__vt__12cls_804AFB3C`.
