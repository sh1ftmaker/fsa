# JSystem/J3DGraphAnimator/J3DShapeTable.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %) (no regression)

Status: **Matching**. 2/2 functions, .text 100 %; `link_check.py --unit` SHA-1 OK; `gate.py`: ok.

## Changes
- splits.txt: J3DShapeTable.cpp's .text now ends at 0x800E4870 (was 0x800E49E0). The four
  global `__as__` functions at 0x800E4870-0x800E49E0 (J3DTexMtxInfo, J3DIndTexMtxInfo,
  J3DFogInfo, J3DNBTScaleInfo) are J3DGraphBase/J3DStruct.cpp, not J3DShapeTable:
  - mkdd (MarioClub_us splits.txt) has exactly this layout: J3DShapeTable.cpp
    (initShapeNodes 0x64, sortVcdVatCmd 0x94), then `JSystem/J3D/GraphBase/J3DStruct.cpp`
    starting with __as__13J3DTexMtxInfo (0x7C), setEffectMtx (dead-stripped in FSA),
    __as__16J3DIndTexMtxInfo (0x24), __as__10J3DFogInfo (0xAC), __as__15J3DNBTScaleInfo (0x24);
    the sizes are FSA's byte for byte.
  - tp has the same functions (global, non-weak) in J3DStruct.cpp.
  - They are the only copies in FSA and global, i.e. out-of-line definitions, not weak copies.
- New stanza `JSystem/J3DGraphBase/J3DStruct.cpp: .text 0x800E4870-0x800E49E0` (no data:
  setEffectMtx's .sdata2 constant was dead-stripped with it). Without a configure.py entry
  dtk prints "Missing configuration" and links the split asm (link SHA-1 OK).

## SHARED-EDITS
- configure.py (coordinator): add `Object(NonMatching, "JSystem/J3DGraphBase/J3DStruct.cpp",
  extra_cflags=["-O4,p"])` to the J3D block and a source (tp's J3DStruct.cpp operator= set,
  mkdd's 0xAC J3DFogInfo copy). Owner to be assigned.
