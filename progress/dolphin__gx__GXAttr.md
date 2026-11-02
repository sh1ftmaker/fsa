# dolphin/gx/GXAttr.c

Status: Matching. 16/16 functions, .text/.data/.sdata 100 %.

- The large "99.9 %" functions (GXSetVtxDesc, GXSetVtxAttrFmt(v), GXSetTexCoordGen2)
  only missed because their switch jump tables were unsplit; added
  `.data 0x80496EE0-0x80497158` (8 jumptables) to the unit's splits stanza.
- Added the five functions oot-gc dead-stripped (GXSetVtxDescv, GXGetVtxDesc,
  GXGetVtxDescv, GXGetVtxAttrFmt, GXGetVtxAttrFmtv) from dolsdk2004/tp
  (same order as FSA), asserts dropped; named their fn_ placeholders.

SHARED-EDITS: none.
