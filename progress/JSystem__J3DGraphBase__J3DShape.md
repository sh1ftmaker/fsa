# JSystem/J3DGraphBase/J3DShape.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4786 / 11561 functions, 864000 / 4442088 bytes of code (19.45 %)

Status: **Matching**. 11/11 functions (was 9/11); .text/.data/.sbss/.sdata2 100 %;
`link_check.py --unit` SHA-1 OK; gate ok.

## Changes
- symbols.txt, from the vtable at 0x8049A428 (slots: draw, drawFast, simpleDraw,
  simpleDrawCache, as tww/tp's J3DShape):
  - lbl_8049A428 -> __vt__8J3DShape
  - fn_800BD684 -> draw__8J3DShapeCFv (resets the cache, inlined loadPreDrawSetting, calls
    slot 0xC drawFast)
  - fn_800BD73C -> simpleDraw__8J3DShapeCFv (calls J3DShapeDraw::draw per mtx group)
  - lbl_8053BB90 (placeholder) 0x8 -> 0x4 (one float; rest is padding).
- draw/simpleDraw: `sOldVcdVatCmd = NULL;` written out, as tww's J3DShape::draw does. Through
  the `resetVcdVatCache()` inline MWCC keeps the 0 in r0 (`cmplw r0,r3`); the target folds it
  into the inlined loadPreDrawSetting's compare (`cmplwi r3,0`).
- sOldVcdVatCmd / sEnvelopeFlag defined where tp/tww define them (before loadPreDrawSetting and
  before drawFast), so makeVcdVatCmd's function statics come first in .sbss as in the DOL
  (the trial link failed on the .sbss order before).

## SHARED-EDITS
None.
