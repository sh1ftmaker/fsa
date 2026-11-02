# dolphin/card/CARDMount.c

Status: Matching (9/9 functions, .text 100 %, .data 100 %).

## Changes
- Added `CARDProbe` (absent from mkdd; shape from zeldaret/tp
  libs/dolphin/src/card/CARDMount.c, using this file's `GameChoice` at 0x800030E3).
- symbols.txt renames (no address/size/type changes):
  - lbl_804B5BA0 -> __CARDBlock (2 x CARDControl, defined in CARDBios.c; same rename pending on another branch)
  - lbl_80539CC8 -> __CARDVendorID (u16 in .sdata, used as such by IsCard/DoMount; mkdd CARDNet.c `u16 __CARDVendorID = 0xFFFF`)
  - fn_800522B4 -> __CARDDefaultApiCallback (4-byte blr passed by CARDMountAsync, as in mkdd)
  - fn_80056048 -> CARDProbe (GameChoice & 0x80 ? FALSE : EXIProbe, matches tp)
  - fn_800568E4 -> CARDMount (CARDMountAsync(..., __CARDSyncCallback) then __CARDSync)
- configure.py: CARDMount.c -> Matching.

## SHARED-EDITS
- none. Note for CARDBios owner: __CARDDefaultApiCallback at 0x800522B4 lies in
  the gap between the previous unit's .text end and CARDBios .text start (0x800522B8).
