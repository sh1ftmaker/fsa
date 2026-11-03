#ifndef JAUDIO_JASBASICWAVEBANK_H
#define JAUDIO_JASBASICWAVEBANK_H

#include "JSystem/JAudio/System/JASWaveArcLoader.h"
#include "JSystem/JAudio/System/JASWaveInfo.h"

#include "types.h"

class JASBasicWaveBank : public JASWaveBank
{
public:
    struct TWaveHandle : public JASWaveHandle
    {
        TWaveHandle() { 
            mHeap = NULL;
            mWaveID = 0;
        }
        ~TWaveHandle() {}
        int getWavePtr() const;
        const JASWaveInfo *getWaveInfo() const { return &mInfo; }
        bool compareHeap(JASHeap *heap) const { return mHeap == heap; }

        JASWaveInfo mInfo; // 04
        JASHeap *mHeap;    // 2C
        u32 mWaveID;       // 30
    };

    struct TWaveInfo {
        TWaveInfo()
        {
            mNext = NULL;
            mPrev = NULL;
        }

        TWaveHandle mHandle; // _00
        TWaveInfo* mNext;    // _34
        TWaveInfo* mPrev;    // _38
    };

    struct TGroupWaveInfo
    {
        TGroupWaveInfo()
        {
            _0 = 0xffff;
            _4 = -1;
        }

        u32 _0;
        int _4;
    };

    struct TWaveGroup : public JASWaveArc
    {
        TWaveGroup() {}
        TWaveGroup(JASBasicWaveBank *);
        virtual ~TWaveGroup();
        void setWaveCount(u32, JKRHeap *);
        virtual void onLoadDone();
        virtual void onEraseDone();
        u32 getWaveID(int) const;

        void setWaveInfo(int index, u32, const JASWaveInfo &waveInfo);

        JASBasicWaveBank *mBank;    // 74
        TWaveInfo *mCtrlWaveArray;  // 78
        u32 mWaveCount;             // 7C

        u32 getWaveCount() const { return mWaveCount; }
    };

    JASBasicWaveBank();
    ~JASBasicWaveBank();
    TWaveGroup *getWaveGroup(u32 group);
    void setGroupCount(u32, JKRHeap *);
    void setWaveTableSize(u32, JKRHeap *);
    void incWaveTable(const JASBasicWaveBank::TWaveGroup *);
    void decWaveTable(const JASBasicWaveBank::TWaveGroup *);
    JASWaveHandle *getWaveHandle(u32) const;
    JASWaveArc *getWaveArc(u32 group) { return getWaveGroup(group); }
    u32 getArcCount() const { return mGroupCount; }

    mutable OSMutex mMutex;       // 04
    TWaveInfo **mWaveTable;       // 1C
    u32 mHandleCount;             // 24
    TWaveGroup **mWaveGroupArray; // 20
    u32 mGroupCount;              // 28
    static u32 mNoLoad;
}; // Size: 0x2c

#endif
