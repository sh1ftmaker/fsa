# game/tu_80301CD0 (Opus, 2026-11-05)

Baseline (rebased on matching): total 8297 / 11557 functions. After: 8308 (+11 net; +12 in this
unit, fn_803099FC moved here from tu_803099FC as __ct__12cls_803099FCFv).

Matched (100 %): fn_80301CD0, fn_80301D30, fn_8030361C, fn_803037D4, fn_8030381C,
fn_803049F0, fn_80304B00, fn_80304C10, fn_803074A4, fn_80308D0C, ~cls_804AA8B8 (0x80309014),
__ct__12cls_803099FCFv (0x803099FC).

Structurally done / close (FIXME in source):
- fn_80303750 99.39 regalloc-only
- fn_80304D64 98.47 regalloc-only
- fn_80308E90 95.36 regalloc-only
- __sinit 99.64 reloc-name (section anchor ...bss.0 vs lbl_804DBC18)
- fn_803075AC 94.01 / fn_80307830 94.97 missing/extra, fn_80307AC8 92.17 scheduling: the callees
  convert d (u8 -> 0/1) once before the loop. Declaring d u8 makes 7830 and AC8 100 % and 75AC
  96.27 %, but drops act_chief_soldier / act_pot_smoke / tu_802E6EC4 draw (they pass d as bool).
  Unresolved conflict; left as bool.

Findings:
- Loop counters are `int`, not s32 (long): with s32 the 8x-unrolled reset loops get an extra
  `cmpwi 0, 16; bge` guard (fn_80308D0C 93.8 % -> 100 % with int).
- lbl_804DB798 (0x480) is not in this object's .bss: with it out, all sinit offsets match.
- 0x803099FC (empty ctor of the 0x28-byte sprite element, passed to __construct_array) is
  emitted after __sinit: it belongs to this unit (split moved by 4 bytes).
- GBA transfer queue: 0x11C-byte entries, name, word count, GBA address (0x0600C000 ...).

Next: the constructor fn_8030905C (proves the remaining .sbss types: ACDC..ACFC are u8[4],
lbl_80506D48 is s32[4][4], lbl_805071E8 s16[4][0x80] then the 8 pool dtor chains), then
fn_8030801C / fn_803085A4 (bitfield records lbl_80506DE8, cls_804AA858Sub11C), fn_803064A8,
fn_803066F0, fn_80303870, fn_80303CE4, fn_80304FB0.
