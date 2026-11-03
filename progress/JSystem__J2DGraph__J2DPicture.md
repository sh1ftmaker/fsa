# JSystem/J2DGraph/J2DPicture.cpp

Baseline (progress.py first line): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4785 / 11561 functions, 863688 / 4442088 bytes of code (19.44 %) (+1 function, +224 bytes, exactly this one)

Status: **Matching** (75/75, .text/.data/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok).
No GXWGFifo draw issue here: the only miss was a missing constructor.

## Matched this pass
- `__ct__10J2DPictureFUxRCQ29JGeometry8TBox2<f>PC7ResTIMGPC7ResTLUT` (was fn_800979E0, 0xE0):
  `J2DPicture(u64, const TBox2f&, const ResTIMG*, const ResTLUT*)`. Name and body from TP
  (symbols.txt and J2DPicture.cpp: J2DPane(tag, bounds), mPalette(NULL), clear textures,
  private_initiate, initinfo); the bytes call `J2DPane(u64, const TBox2f&)`, `private_initiate`
  with args 4/5, then `initinfo`; called from game unit tu_80400DC4. Placed before mkdd's
  (dead-stripped) `J2DPicture(u64, const TBox2f&)` so the weak `setBlendRatio` still follows it.

## SHARED-EDITS
- libs/sdk/include/JSystem/J2D/J2DPicture.h: added the declaration of that constructor
  (no layout change; progress total rose by exactly this function, nothing else moved).
