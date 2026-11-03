#ifndef JAUDIO_JASBNKPARSER_H
#define JAUDIO_JASBNKPARSER_H

#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASOscillator.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport.h"

#include "types.h"

class JASInstEffect;

// FSA has both bank parsers of tp/SMG's JASBNKParser.cpp: createBasicBank
// (0x8002F294) dispatches on the file header's version to Ver1 (0x8002F34C)
// and Ver0 (0x8002FB90). Ver0 is mkdd's parser (MarioClub map names: TInst,
// TOsc, TRand, TSense, TKeymap, TVmap, TPerc, TPmap), Ver1 the chunked format
// of tp/SMG (Ver1::findChunk and Ver1::createEffector are inlined in FSA).
namespace JASBNKParser
{
    struct TFileHeader
    {
        /* 0x0 */ u32 id; // 'IBNK'
        /* 0x4 */ u32 mSize;
        /* 0x8 */ u32 mGlobalId;
        /* 0xC */ u32 mVersion;
    };

    namespace Ver1
    {
        struct TOsc
        {
            /* 0x00 */ u32 id; // 'Osci'
            /* 0x04 */ u8 mTarget;
            /* 0x08 */ f32 mRate;
            /* 0x0C */ u32 mAttackEnvelopeOffset;
            /* 0x10 */ u32 mReleaseEnvelopeOffset;
            /* 0x14 */ f32 mScale;
            /* 0x18 */ f32 mVertex;
        };

        struct TRandData
        {
            /* 0x00 */ u8 mType;
            /* 0x04 */ f32 mFloor;
            /* 0x08 */ f32 mCeiling;
        };

        struct TSenseData
        {
            /* 0x00 */ u8 mType;
            /* 0x01 */ u8 mTrigger;
            /* 0x02 */ u8 mCenterKey;
            /* 0x04 */ f32 mMin;
            /* 0x08 */ f32 mMax;
        };

        struct TVeloData
        {
            /* 0x00 */ u8 _0;
            /* 0x04 */ u32 _4;
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
        };

        struct TInstData
        {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
        };

        struct TPercData
        {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
            /* 0x08 */ u8 mPan;
            /* 0x0A */ u16 mRelease;
            /* 0x0C */ u32 mEffectCount;
        };

        struct TChunk
        {
            /* 0x0 */ u32 mID;
            /* 0x4 */ u32 mSize;
        };

        struct TEnvtChunk : TChunk
        {
            /* 0x8 */ u8 mData[0];
        };

        struct TOscChunk : TChunk
        {
            /* 0x8 */ u32 mCount;
            /* 0xC */ TOsc mOsc[0];
        };

        struct TListChunk : TChunk
        {
            /* 0x8 */ u32 count;
            /* 0xC */ u32 mOffsets[0];
        };

        TChunk *findChunk(const void *stream, u32 id);
        JASInstEffect *createEffector(const void *stream, JKRHeap *heap);
        JASBasicBank *createBasicBank(const void *stream, JKRHeap *heap);
    } // namespace Ver1

    namespace Ver0
    {
        template <typename T>
        struct TOffset
        {
            T *ptr(const void *stream) const { return JSUConvertOffsetToPtr<T>(stream, offset); }

            /* 0x0 */ u32 offset;
        };

        struct TOsc
        {
            /* 0x00 */ u8 mTarget;
            /* 0x04 */ f32 mRate;
            /* 0x08 */ TOffset<JASOscillator::Point> mPointOffset;
            /* 0x0C */ TOffset<JASOscillator::Point> mRelPointOffset;
            /* 0x10 */ f32 mScale;
            /* 0x14 */ f32 mVertex;
        };

        struct TVmap
        {
            /* 0x00 */ u8 _0;
            /* 0x04 */ u32 _4;
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
        };

        struct TRand
        {
            /* 0x00 */ u8 mType;
            /* 0x04 */ f32 mFloor;
            /* 0x08 */ f32 mCeiling;
        };

        struct TSense
        {
            /* 0x00 */ u8 mType;
            /* 0x01 */ u8 mTrigger;
            /* 0x02 */ u8 mCenterKey;
            /* 0x04 */ f32 mMin;
            /* 0x08 */ f32 mMax;
        };

        struct TKeymap
        {
            /* 0x00 */ u8 mHighKey;
            /* 0x04 */ u32 mVeloRegionCount;
            /* 0x08 */ TOffset<TVmap> mVmapOffset[0];
        };

        struct TInst
        {
            /* 0x00 */ u32 id;
            /* 0x04 */ u8 _04[4];
            /* 0x08 */ f32 mVolume;
            /* 0x0C */ f32 mPitch;
            /* 0x10 */ TOffset<TOsc> mOscOffset[2];
            /* 0x18 */ TOffset<TRand> mRandOffset[2];
            /* 0x20 */ TOffset<TSense> mSenseOffset[2];
            /* 0x28 */ u32 mKeyRegionCount;
            /* 0x2C */ TOffset<TKeymap> mKeymapOffset[0];
        };

        struct TPmap
        {
            /* 0x00 */ f32 mVolume;
            /* 0x04 */ f32 mPitch;
            /* 0x08 */ TOffset<TRand> mRandOffset[2];
            /* 0x10 */ u32 mVeloRegionCount;
            /* 0x14 */ TOffset<TVmap> mVmapOffset[0];
        };

        struct TPerc
        {
            /* 0x000 */ u32 mMagic; // 'PERC' or 'PER2'
            /* 0x004 */ u8 _004[0x84];
            /* 0x088 */ TOffset<TPmap> mPmapOffset[0x80];
            /* 0x288 */ s8 mPan[0x80];
            /* 0x308 */ u16 mRelease[0x80];
        };

        struct TOffsetData
        {
            /* 0x000 */ u32 mMagic; // 'BANK'
            /* 0x004 */ TOffset<TInst> mInstOffset[0x80];
            /* 0x204 */ u8 _204[0x190];
            /* 0x394 */ TOffset<TPerc> mPercOffset[12];
        };

        struct THeader
        {
            /* 0x00 */ u8 _00[0x20];
            /* 0x20 */ TOffsetData mOffsets;
        };

        JASBasicBank *createBasicBank(const void *stream, JKRHeap *heap);
        JASOscillator::Data *findOscPtr(JASBasicBank *bank, const THeader *header, const TOsc *osc);
        const JASOscillator::Point *getOscTableEndPtr(const JASOscillator::Point *points);
    } // namespace Ver0

    JASBank *createBank(const void *stream, JKRHeap *heap);
    JASBasicBank *createBasicBank(const void *stream, JKRHeap *heap);

    inline u32 getBankNumber(const void *stream)
    {
        const TFileHeader *header = (const TFileHeader *)stream;
        return header->mGlobalId;
    }

    extern u32 sUsedHeapSize;
}; // namespace JASBNKParser

#endif // JAUDIO_JASBNKPARSER_H
