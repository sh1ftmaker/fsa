
## tu_8013AAD4 worker run (2026-11-05)
Baseline: total 7914 / 11557 functions. After: 7937 (+23). Unit 3/29 -> 26/29.
Matched: everything except the three below.
Resistant:
- fn_8013AC54 98.87 % scheduling+regalloc: original loads field_0x8 into r0 then field_0xc into r5; ours loads field_0xc first.
- fn_8013AC90 99.73 % regalloc-only: position x,y,z in f3,f4,f5 (ours rotated).
- GameRoot2::fn_8013BBE0 97.41 %: mode==0xC materialised as byte (subfic/cntlzw/extrwi.) in original; ours cmpwi/bne (same as tu_80158624 FIXME).
- .data 98.66 %: vtable object is 12 bytes in ours, 0x10 in the target (4-byte section-end pad). Not Matching.
SHARED-EDITS: GameGlobals.h (GameRoot2 vptr/dtor/ctor/fields 0x4,0x14,0x18,0x1C,0x34,0x38, members; cls_804A03A8 vptr+dtor+ctor+3 members, size pad 0x290; ShadowCircle_804A0160::fn_80158E88 now a member (tu_80158624.cpp definition changed); lbl_80543F20; fn_80239D38 extern "C"), Camera.h (fn_8030DD8C, fn_8030DFE8; ScreenRect fields u32), libs/sdk GXDraw.h (GXDrawSphere1). symbols.txt placeholder renames only.
