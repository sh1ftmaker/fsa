# JSystem/J3DGraphBase/J3DPacket.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4788 / 11561 functions, 863640 / 4442088 bytes of code (19.44 %)

Status: **Matching**. 31/31 functions (was 27/31), .text/.data/.sbss 100 %;
`link_check.py --unit` SHA-1 OK; `gate.py`: ok.

## Changes
- symbols.txt only (no source change). The four placeholders are named by the vtable
  slots in this unit's .data (FSA layout: 2 header words, entry, draw, dtor):
  - fn_800BC460 -> entry__9J3DPacketFP13J3DDrawBuffer (slot 2 of __vt__9J3DPacket,
    __vt__13J3DDrawPacket, __vt__14J3DShapePacket; `li r3,1; blr` = `return true`)
  - fn_800BC488 -> draw__9J3DPacketFv (slot 3 of __vt__9J3DPacket; `blr`)
  - fn_800BC48C -> __dt__9J3DPacketFv (slot 4 of __vt__9J3DPacket; stores __vt__9J3DPacket)
  - fn_800BC4D4 -> __dt__13J3DDrawPacketFv (slot 4 of __vt__13J3DDrawPacket; stores
    __vt__13J3DDrawPacket then __vt__9J3DPacket)
  Same names in tww's symbols.txt (entry__9J3DPacketFP13J3DDrawBuffer, draw__9J3DPacketFv,
  __dt__9J3DPacketFv).

## SHARED-EDITS
None.
