#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JAudio/System/JASTrack.h"  // FSA's __sinit constructs the JASTrack/JASChannel pools

#ifdef DEBUG
#pragma sym on
#endif

JAISoundHandle *JAISoundHandles::getHandleSoundID(JAISoundID soundID)
{
    for (int i = 0; i < numHandles_; i++)
    {
        if (handles_[i].isSoundAttached())
        {
            if (handles_[i]->getID() == soundID)
            {
                return &handles_[i];
            }
        }
    }

    return NULL;
}

JAISoundHandle *JAISoundHandles::getHandleUserData(u32 id)
{
    for (int i = 0; i < numHandles_; i++)
    {
        if (handles_[i].isSoundAttached())
        {
            if (handles_[i]->getUserData() == id)
            {
                return &handles_[i];
            }
        }
    }

    return NULL;
}

JAISoundHandle *JAISoundHandles::getFreeHandle()
{
    for (int i = 0; i < numHandles_; i++)
    {
        if (!handles_[i].isSoundAttached())
        {
            return &handles_[i];
        }
    }

    return NULL;
}

void JAISoundHandles::stopAllSounds()
{
    for (int i = 0; i < numHandles_; i++)
    {
        JAISoundHandle *handle = &handles_[i];
        if (handle->isSoundAttached())
        {
            (*handle)->stop();
        }
    }
}

void JAISoundHandles::setPos(const JGeometry::TVec3f &pos)
{
    for (int i = 0; i < numHandles_; i++)
    {
        JAISoundHandle *handle = &handles_[i];
        if (handle->isSoundAttached())
        {
            (*handle)->setPos(pos);
        }
    }
}

#ifdef DEBUG
#pragma sym off
#endif
