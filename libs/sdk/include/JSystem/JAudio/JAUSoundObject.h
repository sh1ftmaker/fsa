#ifndef JAUDIO_JAUSOUNDOBJECT_H
#define JAUDIO_JAUSOUNDOBJECT_H

#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/Interface/JAISoundHandles.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JGeometry/Vec.h"
#include "types.h"

class JAUSoundObject : public JAISoundHandles
{
public:
    JAUSoundObject(JGeometry::TVec3f *, u8, JKRHeap *);
    ~JAUSoundObject();

    virtual void process();
    virtual void dispose();
    virtual bool stopOK(JAISoundHandle &);
    virtual JAISoundHandle *startSound(JAISoundID);
    virtual JAISoundHandle *startSoundIndex(JAISoundID, u8);
    virtual JAISoundHandle *startLevelSound(JAISoundID);
    virtual JAISoundHandle *startLevelSoundIndex(JAISoundID, u8);

    void stopSound(JAISoundID, u32);
    JAISoundHandle *getLowPrioSound(JAISoundID);
    JAISoundHandle *getFreeHandleNotReserved();

    bool mIsAllocated;                // 0C
    u32 _10;                          // 10, bit i set: handle i is reserved
    JGeometry::TVec3<f32> *mSoundPos; // 14
};

class JAUDopplerSoundObject : public JAUSoundObject
{ 
public:
    JAUDopplerSoundObject(JGeometry::TVec3f *, u8, JKRHeap *);
    ~JAUDopplerSoundObject();

    virtual void process();
    virtual JAISoundHandle *startSound(JAISoundID soundID);
    virtual JAISoundHandle *startLevelSound(JAISoundID soundID);

    JGeometry::TVec3f _18;
    JGeometry::TVec3f _24;
};

#endif
