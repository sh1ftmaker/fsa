# JSystem/JParticle/JPAResourceManager.cpp

Baseline (progress.py first line at start): total: 4784 / 11561 functions, 863464 / 4442088 bytes of code (19.44 %)
After: total: 4786 / 11561 functions, 863712 / 4442088 bytes of code (19.44 %)

Status: **Matching**. 6/6 functions (was 4/6); .text 100 %; `link_check.py --unit` SHA-1 OK;
gate ok.

## Changes
- swapTexture and getResUserWork written (were empty stubs), tp's bodies. getResource in tp's
  form (`pResAry[i]->getUsrIdx() == usrIdx`); it is inlined into getResUserWork and the old
  form gave that copy different registers. Includes JPABlock.h (JPADynamicsBlock) and
  JPATexture.h, <string.h> for strcmp.
- symbols.txt:
  - fn_800F37E8 -> swapTexture__18JPAResourceManagerFPC7ResTIMGPCc: strcmp of the name against
    each pTexAry[i] (0x44 -> name at 0xC), keeps JUTTexture::mTexInfo, calls
    JUTTexture::storeTIMG(img, 0), returns the old ResTIMG; position after getResource as in tp.
  - fn_800F38C4 -> getResUserWork__18JPAResourceManagerCFUs: inlined getResource (usrIdx at
    0x3C), then pDyn->mData->mResUserWork (0x2C -> 0x0 -> 0xC); last function as in tp.
  Both are called from game unit tu_8013C5C8.

## SHARED-EDITS
None.
