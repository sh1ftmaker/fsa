#ifndef JAUDIO_JASSIMPLEWAVEBANK_H
#define JAUDIO_JASSIMPLEWAVEBANK_H

#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JAudio/System/JASWaveInfo.h"

#include "types.h"

class JASSimpleWaveBank : public JASWaveBank, public JASWaveArc
{
public:
    struct TWaveHandle : public JASWaveHandle
    {
        TWaveHandle() { 
            mHeap = NULL;
        }
        int getWavePtr() const;
        const JASWaveInfo *getWaveInfo() const { return &mInfo; }
        bool compareHeap(JASHeap *heap) const { return mHeap == heap; }

        JASWaveInfo mInfo; // 04
        JASHeap *mHeap;    // 2C
    };

    JASSimpleWaveBank();
    virtual ~JASSimpleWaveBank();
    void setWaveTableSize(u32, JKRHeap *);
    JASWaveHandle *getWaveHandle(u32) const;
    void setWaveInfo(u32, const JASWaveInfo &);
    JASWaveArc *getWaveArc(u32);
    virtual u32 getArcCount() const { return 1; }

    TWaveHandle *mWaveTable;
    u32 mWaveCount;
};

#endif
