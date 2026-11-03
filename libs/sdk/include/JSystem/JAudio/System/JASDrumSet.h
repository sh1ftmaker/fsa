#ifndef JAUDIO_JASDRUMSET_H
#define JAUDIO_JASDRUMSET_H

#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JKernel/JKRHeap.h"

struct JASDrumSet : public JASInst
{
    class TPerc
    {
    public:
        TPerc();
        void setVolume(f32 vol) { mVolume = vol; }
        void setPitch(f32 pitch) { mPitch = pitch; }
        void setPan(f32 pan) { mPan = pan; }

        void appendEffect(JASPtrListAdaptor<JASInstEffect> *effect);
        void setRelease(u32 release);
        JASVelo *getVeloRegion(int index);
        void setVeloRegionCount(u32 count, JKRHeap *heap);

        // FSA layout (as in SMG): 0x1C bytes, effects in a JASPtrList at
        // 0x10 (appendEffect 0x8003078C).
        f32 mVolume;  // 00
        f32 mPitch;   // 04
        f32 mPan;     // 08
        u16 mRelease; // 0c

        JASPtrList<JASInstEffect> mEffects; // 10
        u32 mVelomapCount;                  // 14
        JASVelo *mVelomap;                  // 18
    };

    JASDrumSet();
    virtual ~JASDrumSet();
    void newPercArray(u8 num, JKRHeap *heap);
    virtual bool getParam(int, int, JASInstParam *) const;
    void setPerc(int index, JASDrumSet::TPerc *perc);
    virtual u32 getType() const { return 'PERC'; }


    static const u32 sPercCount = 0x80;
    //u8 mBuffer[0x200];
    TPerc *mPercArray[sPercCount];
    //u8 mPercNumMax;

};

#endif
