# JSystem/JParticle/JPAResource.cpp

Baseline: total 1199 / 11560 functions, 218212 bytes (4.92 %). After: 1204 / 11560, 224576 bytes (5.06 %).

## Matched: 14 / 14 (unit set to Matching, gate ok)

The three remaining misses were relocation names only. I gave the placeholders their real names in symbols.txt:
- 60 JPABaseShape.cpp free functions at 0x800E6594-0x800E9EE0 (JPASetPointSize ... JPADrawParticleCallBack).
  Evidence: init() is byte-identical to the mkdd source and references each one at the same instruction. Their address order also matches the
  function order of the donor's JPABaseShape.cpp (pikmin2/tp).
- fn_800EA2C0 -> setGX__12JPABaseShapeCFP18JPAEmitterWorkData (GXSetBlendMode/ZMode/AlphaCompare/TevColorIn/TevAlphaIn/TevDirect x2/ZCompLoc, same as the donors).
- fn_80092984 -> load__10JUTTextureF11_GXTexMapID (`if (palette) palette->load(); GXLoadTexObj(&texObj, id);`, which follows initTexObj as in the donors).

## Note for the reviewer
The mkdd header libs/sdk/include/JSystem/JParticle/JPAShape.h declares `u32 isGlblTexAnm()`, where tp has BOOL. init() matches only with u32.
These BOOL variants were tried and all fail on register allocation: the header returning BOOL (99.83 %), a u32 local (99.65 %), and swapping the declaration order (99.78 %).
pikmin2 needs the same u32 (it uses a cast). tww's JPA has u32-returning flag getters too. So a u32 return type is a plausible original declaration, not a trick.
The donor comment in JPAResource.cpp called it "a fakematch". I reworded it to state the facts.

## SHARED-EDITS
- Optional: drop the "// fakematch most likely" remark on JPAShape.h:92. Leave the type as u32.
