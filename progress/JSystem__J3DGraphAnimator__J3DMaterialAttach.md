# JSystem/J3DGraphAnimator/J3DMaterialAttach.cpp

Baseline (start): `total: 3711 / 11561 functions, 710408 / 4442088 bytes of code (15.99 %)`
Now:              `total: 3915 / 11561 functions, 716252 / 4442088 bytes of code (16.12 %)` (includes the inherited branch)

Unit: 4/10 -> 10/10, .text and .data 100 %. Matching; gate ok (link SHA-1 ok).

## How
- Branch is built on the J3DMaterialFactory branch tip (426978c, same commits) because it needs
  its J3DTexMtx.h commit (out-of-line J3DTexMtxInfo::operator=) and the header stack under it.
  Merge J3DMaterialFactory first or together.
- Names: four animator ctors from the game's __construct_array calls (0x8016A5A0, paired dtors);
  0x800DFB04 entryTexMtxAnimator (createTexMtxForAnimator inlined, as tp), 0x800DFD94 entryTevRegAnimator.
- The rest of the pikmin2 file (alloc*/remove*/createTexMtxForAnimator, ...) is dead-stripped in FSA;
  the link is byte-identical with it.

## SHARED-EDITS
- J3DMaterialAnm.h: setTexMtxAnm / setTevColorAnm / setTevKColorAnm declared out of line (tp);
  0x800C768C/76CC/770C named. J3DMaterialAnm.cpp should get tp's three bodies (its owner).
- symbols.txt: j3dDefaultTexMtxInfo (0x80463470, J3DTevs .rodata).
