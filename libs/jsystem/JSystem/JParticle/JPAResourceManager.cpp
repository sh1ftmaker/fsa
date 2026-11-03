#include "JSystem/JParticle/JPAResource.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JParticle/JPABlock.h"
#include "JSystem/JParticle/JPATexture.h"
#include <string.h>

JPAResourceManager::JPAResourceManager(u16, u16, JKRHeap *)
{
    // UNUSED
}

JPAResourceManager::JPAResourceManager(const void *p_jpc, JKRHeap *heap)
{
    pResAry = nullptr;
    pTexAry = nullptr;
    resMaxNum = 0;
    resRegNum = 0;
    texMaxNum = 0;
    texRegNum = 0;
    pHeap = heap;
#line 49
    JUT_ASSERT((p_jpc != 0) && (pHeap != 0));
    JPAResourceLoader loader((const u8 *)p_jpc, this);
}

void JPAResourceManager::load(const char *, u16)
{
    // UNUSED
}

void JPAResourceManager::load(const void *, u16)
{
    // UNUSED
}

JPAResource *JPAResourceManager::getResource(u16 idx) const
{
    for (u16 i = 0; i < resRegNum; i++)
        if (pResAry[i]->getUsrIdx() == idx)
            return pResAry[i];
    return nullptr;
}

bool JPAResourceManager::checkUserIndexDuplication(u16) const
{
    // UNUSED, but exists in TP
}

const ResTIMG *JPAResourceManager::swapTexture(const ResTIMG *img, const char *swapName)
{
    const ResTIMG *ret = nullptr;
    JUTTexture *tex = nullptr;

    for (s32 i = 0; i < texRegNum; i++)
    {
        if (strcmp(swapName, pTexAry[i]->getName()) == 0)
        {
            tex = pTexAry[i]->getJUTTexture();
            ret = tex->getTexInfo();
            tex->storeTIMG(img, (u8)0);
            break;
        }
    }

    return ret;
}

void JPAResourceManager::registRes(JPAResource *res)
{
#line 151
    JUT_ASSERT(resRegNum < resMaxNum);
    pResAry[resRegNum] = res;
    resRegNum++;
}

void JPAResourceManager::registTex(JPATexture *res)
{
#line 166
    JUT_ASSERT(texRegNum < texMaxNum);
    pTexAry[texRegNum] = res;
    texRegNum++;
}

void JPAResourceManager::registTexDupCheck(const u8 *, JKRHeap *)
{
    // UNUSED
}

u32 JPAResourceManager::getResUserWork(u16 usrIdx) const
{
    u32 ret = 0;

    JPAResource *res = getResource(usrIdx);
    if (res != nullptr)
        ret = res->getDyn()->getResUserWork();

    return ret;
}
