# JSystem/JAudio/System/JASDrumSet.cpp

## w10-jas (2026-11-02): Matching
Baseline: progress.py total 4784 / 11561, 863464 bytes; unit 9/11 (~JASDrumSet 21.9 %, TPerc ct 64.3 %),
.sbss 80 %, .sdata2 85.7 %.
Now: 11/11, every section 100 %, Matching; `link_check --unit` SHA-1 OK; gate: ok. Total 4786, 863604 bytes.
- ~JASDrumSet is empty (SMG's JASDrumSet.cpp): FSA's dtor only resets the vtables and deletes; mkdd's
  loop deleting the TPercs is a later/earlier version. TPerc has no destructor in FSA (none in .text),
  so its declaration and mkdd's body (delete[] mVelomap) are gone; nothing else used it.
- TPerc::TPerc with SMG's member-initializer list (stores in member order, mEffects included).
- Names: lbl_8053A350/lbl_8053A354 -> getParam's statics oscp$246 / init$247 (local); .sdata2
  lbl_8053B1B0/B4/B8 -> @284/@285/@296 (1.0f, 0.0f, 0.5f; lbl_8053B1B8 0x8 -> 0x4, dtk had swallowed
  the padding).

## Earlier state (previous worker)

Baseline: `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
After (incl. cherry-picked memPool_ commits): 3717 / 11561, 713508 bytes. Gate ok.

Unit: 2/11 -> 6/11 functions; .data/.bss/.ctors 100 %.

## Done (symbols/splits)
- Vtable at 0x80491A50 is {dtor, getParam, getType}, stored by the ctor after
  the base vtable at 0x804919BC: lbl_80491A50 -> __vt__10JASDrumSet (0x18 -> 0x14),
  lbl_804919BC -> __vt__7JASInst ({__dt__7JASInstFv, 0, 0}).
- fn_80030514 -> __dt__10JASDrumSetFv, fn_80030570 -> getParam (vtable slots),
  fn_800306DC -> setPerc, fn_800306EC -> TPerc::TPerc, fn_800307D0 -> __sinit.
- Splits: .data 0x80491A50-0x80491A68, .bss 0x804B2880-0x804B28A8
  (static osc + __sinit registration; lbl_804B2898 0x10 -> 0xC),
  .sbss 0x8053A350-0x8053A358 (oscp, init guard).

## Resistant / next
FSA's JASDrumSet is a different version from mkdd's source:
- TPerc layout: 0x0 volume, 0x4 pitch, 0x8 pan, 0xC u16 release,
  0x10 JASInstEffect* list head, 0x14 velomap count, 0x18 velomap.
  Ours has mEffects[4] at 0x10, so setVeloRegionCount/getVeloRegion use
  0x20/0x24 (target 0x14/0x18). The TPerc ctor zeroes 0x10/0x14/0x18 and sets
  release 1000.
- fn_8003078C (0x30): appends an effect to the singly linked list at
  TPerc+0x10 (walks `*(effect+0)` to the end). Needs JASInstEffect's next
  pointer at +0; no donor has this version, name unknown (TPerc::setEffect?).
- No TPerc::~TPerc in FSA; ~JASDrumSet (0x5C) only resets vtables and deletes.
- getParam 91 % (TPerc offsets, effect list walk).
Changing TPerc touches JASDrumSet.h, used by JASBNKParser too.

## SHARED-EDITS
- memPool_ commits cherry-picked from the JASChannel branch.
