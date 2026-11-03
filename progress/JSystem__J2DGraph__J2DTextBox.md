# JSystem/J2DGraph/J2DTextBox.cpp

Baseline (progress.py first line): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4786 / 11561 functions, 863776 / 4442088 bytes of code (19.45 %)

Status: **Matching** (30/30 functions, .text/.data/.sdata2 100 %, link_check --unit SHA-1 OK, gate ok).

## Matched this pass
- `__ct__10J2DTextBoxFPCcPCcs` (was fn_8009E018, 0xA4): `J2DTextBox(const char* fontName, const char*, s16)`,
  font looked up through `J2DScreen::getNameResource`, default bindings Left/Top.
- `__ct__10J2DTextBoxFUxRCQ29JGeometry8TBox2<f>PCcPCcs18J2DTextBoxHBinding18J2DTextBoxVBinding` (was fn_8009E648, 0x94).

Evidence for the names: pikmin2's J2DTextBox.h (from its map's unused list) declares both
`J2DTextBox(const char*, const char*, s16)` and `J2DTextBox(u64, const TBox2f&, const char*, const char*, s16, HBinding, VBinding)`;
TWW has the older `__ct__10J2DTextBoxFPCcPCc`. The bytes: arg 1 goes to getNameResource, its result is initiate's ResFONT,
args 2/3 (and 4/5 for the box ctor) pass through to initiate; the box ctor calls `J2DPane(u64, const TBox2f&)`.
The same declarations were already in libs/sdk/include/JSystem/J2D/J2DPane.h.

## SHARED-EDITS
None (symbols.txt: only the two placeholder renames).
