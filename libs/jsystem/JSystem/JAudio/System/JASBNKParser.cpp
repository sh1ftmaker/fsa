#include "JSystem/JAudio/System/JASBNKParser.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASDrumSet.h"

JASBank *JASBNKParser::createBank(const void *stream, JKRHeap *heap)
{
    return createBasicBank(stream, heap);
}

u32 JASBNKParser::sUsedHeapSize;

JASBasicBank *JASBNKParser::createBasicBank(const void *stream, JKRHeap *heap)
{
    if (heap == NULL)
        heap = JASDram;

    u32 freeSize = heap->getFreeSize();
    const TFileHeader *header = (const TFileHeader *)stream;
    JASBasicBank *bank = NULL;
    switch (header->mVersion)
    {
    case 0:
        bank = Ver0::createBasicBank(stream, heap);
        break;
    case 1:
        bank = Ver1::createBasicBank(stream, heap);
        break;
    }

    sUsedHeapSize += freeSize - heap->getFreeSize();
    return bank;
}

inline JASBNKParser::Ver1::TChunk *JASBNKParser::Ver1::findChunk(const void *stream, u32 id)
{
    TFileHeader *header = (TFileHeader *)stream;
    void *end = (void *)((u32)stream + header->mSize);
    TChunk *chunk = (TChunk *)((u32)stream + 0x20);
    while (chunk < end)
    {
        if (chunk->mID == id)
            return chunk;
        chunk = (TChunk *)(((u32)chunk + 0xb + chunk->mSize) & ~3);
    }
    return NULL;
}

inline JASInstEffect *JASBNKParser::Ver1::createEffector(const void *stream, JKRHeap *heap)
{
    const u32 *data = (const u32 *)stream;
    switch (*data++)
    {
    case 'Rand':
    {
        const TRandData *randData = (const TRandData *)data;
        JASInstRand *rand = new (heap, 0) JASInstRand();
        rand->setType(randData->mType);
        rand->setFloor(randData->mFloor);
        rand->setCeiling(randData->mCeiling);
        return rand;
    }
    case 'Sens':
    {
        const TSenseData *senseData = (const TSenseData *)data;
        JASInstSense *sense = new (heap, 0) JASInstSense();
        sense->setType(senseData->mType);
        sense->setParams(senseData->mTrigger, senseData->mCenterKey, senseData->mMin, senseData->mMax);
        return sense;
    }
    default:
        return NULL;
    }
}

// FIXME: 96.7 %: FSA recomputes the inlined findChunk's stream + 0x20 and
// stream + header->mSize for each of the three calls (ENVT, OSCT, LIST); our
// build computes them once, which shifts the registers of the whole function.
JASBasicBank *JASBNKParser::Ver1::createBasicBank(const void *stream, JKRHeap *heap)
{
    if (heap == NULL)
        heap = JASDram;

    JASBasicBank *bank = new (heap, 0) JASBasicBank();
    if (bank == NULL)
        return NULL;

    TEnvtChunk *envtChunk = (TEnvtChunk *)findChunk(stream, 'ENVT');
    TOscChunk *oscChunk = (TOscChunk *)findChunk(stream, 'OSCT');
    TListChunk *listChunk = (TListChunk *)findChunk(stream, 'LIST');

    u8 *envt = new (heap, 2) u8[envtChunk->mSize];
    JASCalc::bcopy(envtChunk->mData, envt, envtChunk->mSize);

    u32 *ptr = &oscChunk->mCount;
    u32 count = *ptr++;
    JASOscillator::Data *oscData = new (heap, 0) JASOscillator::Data[count];
    for (int i = 0; i < count; i++, ptr += sizeof(TOsc) >> 2)
    {
        TOsc *op = (TOsc *)ptr;
        JASOscillator::Data *data = &oscData[i];
        data->mTarget = op->mTarget;
        data->_04 = op->mRate;
        data->mScale = op->mScale;
        data->_14 = op->mVertex;
        data->mTable = (JASOscillator::Point *)(envt + op->mAttackEnvelopeOffset);
        data->rel_table = (JASOscillator::Point *)(envt + op->mReleaseEnvelopeOffset);
    }

    TListChunk *list = listChunk;
    for (int i = 0; i < list->count; i++)
    {
        if (list->mOffsets[i] == 0)
            continue;

        u32 *data = (u32 *)((u32)stream + list->mOffsets[i]);
        switch (*data++)
        {
        case 'Inst':
        {
            JASBasicInst *instp = new (heap, 0) JASBasicInst();
            u32 oscCount = *data++;
            for (int j = 0; j < oscCount; j++)
            {
                u32 index = *data++;
                instp->setOsc(j, &oscData[index]);
            }

            u32 effectCount = *data++;
            for (int j = 0; j < effectCount; j++)
            {
                JASInstEffect *effect = createEffector((const void *)((u32)stream + *data++), heap);
                instp->appendEffect(new (heap, 0) JASPtrListAdaptor<JASInstEffect>(effect));
            }

            u32 keyRegionCount = *data++;
            instp->setKeyRegionCount(keyRegionCount, heap);
            for (int j = 0; j < keyRegionCount; j++)
            {
                JASBasicInst::TKeymap *keymap = instp->getKeyRegion(j);
                keymap->setHighKey(data[0] >> 24);
                u32 veloCount = data[1];
                data += 2;
                keymap->setVeloRegionCount(veloCount, heap);
                for (int k = 0; k < veloCount; k++)
                {
                    JASVelo *velo = keymap->getVeloRegion(k);
                    TVeloData *veloData = (TVeloData *)data;
                    velo->_0 = veloData->_0;
                    velo->_4 = JSULoHalf(veloData->_4);
                    velo->_8 = veloData->mVolume;
                    velo->_c = veloData->mPitch;
                    data += 4;
                }
            }

            TInstData *instData = (TInstData *)data;
            instp->setVolume(instData->mVolume);
            instp->setPitch(instData->mPitch);
            bank->setInst(i, instp);
            break;
        }
        case 'Perc':
        {
            JASDrumSet *drump = new (heap, 0) JASDrumSet();
            u32 count = *data++;
            for (int j = 0; j < count; j++)
            {
                u32 offset = *data++;
                if (offset == 0)
                    continue;

                JASDrumSet::TPerc *percp = new (heap, 0) JASDrumSet::TPerc();
                u32 *ptr = (u32 *)((u32)stream + offset);
                TPercData *percData = (TPercData *)(ptr + 1);
                percp->setVolume(percData->mVolume);
                percp->setPitch(percData->mPitch);
                percp->setPan(percData->mPan / 127.0f);
                percp->setRelease(percData->mRelease);

                ptr = &percData->mEffectCount;
                u32 effectCount = *ptr++;
                for (int k = 0; k < effectCount; k++)
                {
                    JASInstEffect *effect = createEffector((const void *)((u32)stream + *ptr++), heap);
                    percp->appendEffect(new (heap, 0) JASPtrListAdaptor<JASInstEffect>(effect));
                }

                u32 veloCount = *ptr++;
                percp->setVeloRegionCount(veloCount, heap);
                for (int k = 0; k < veloCount; k++)
                {
                    JASVelo *velo = percp->getVeloRegion(k);
                    TVeloData *veloData = (TVeloData *)ptr;
                    velo->_0 = veloData->_0;
                    velo->_4 = JSULoHalf(veloData->_4);
                    velo->_8 = veloData->mVolume;
                    velo->_c = veloData->mPitch;
                    ptr += 4;
                }
                drump->setPerc(j, percp);
            }
            bank->setInst(i, drump);
            break;
        }
        }
    }

    return bank;
}

JASBasicBank *JASBNKParser::Ver0::createBasicBank(const void *stream, JKRHeap *heap)
{
    if (heap == NULL)
        heap = JASDram;

    const THeader *header = (const THeader *)stream;
    JASBasicBank *bank = new (heap, 0) JASBasicBank();
    if (bank == NULL)
        return NULL;

    for (int i = 0; i < 0x80; i++)
    {
        TInst *tinst = header->mOffsets.mInstOffset[i].ptr(header);
        if (tinst == NULL)
            continue;

        JASBasicInst *instp = new (heap, 0) JASBasicInst();
        instp->setVolume(tinst->mVolume);
        instp->setPitch(tinst->mPitch);

        int oscIdx = 0;
        for (int j = 0; j < 2; j++)
        {
            TOsc *tosc = tinst->mOscOffset[j].ptr(header);
            if (tosc == NULL)
                continue;

            JASOscillator::Data *osc = findOscPtr(bank, header, tosc);
            if (osc != NULL)
            {
                instp->setOsc(oscIdx, osc);
            }
            else
            {
                osc = new (heap, 0) JASOscillator::Data();
                osc->mTarget = tosc->mTarget;
                osc->_04 = tosc->mRate;

                JASOscillator::Point *points = tosc->mPointOffset.ptr(header);
                if (points != NULL)
                {
                    const JASOscillator::Point *end = getOscTableEndPtr(points);
                    int size = end - points;
                    JASOscillator::Point *table = new (heap, 0) JASOscillator::Point[size];
                    JASCalc::bcopy(points, table, size * sizeof(JASOscillator::Point));
                    osc->mTable = table;
                }
                else
                {
                    osc->mTable = NULL;
                }

                points = tosc->mRelPointOffset.ptr(header);
                if (points != NULL)
                {
                    const JASOscillator::Point *end = getOscTableEndPtr(points);
                    int size = end - points;
                    JASOscillator::Point *table = new (heap, 0) JASOscillator::Point[size];
                    JASCalc::bcopy(points, table, size * sizeof(JASOscillator::Point));
                    osc->rel_table = table;
                }
                else
                {
                    osc->rel_table = NULL;
                }

                osc->mScale = tosc->mScale;
                osc->_14 = tosc->mVertex;
                instp->setOsc(oscIdx, osc);
            }
            oscIdx++;
        }

        for (int j = 0; j < 2; j++)
        {
            TRand *trand = tinst->mRandOffset[j].ptr(header);
            if (trand == NULL)
                continue;

            JASInstRand *rand = new (heap, 0) JASInstRand();
            rand->setType(trand->mType);
            rand->setFloor(trand->mFloor);
            rand->setCeiling(trand->mCeiling);
            instp->appendEffect(new (heap, 0) JASPtrListAdaptor<JASInstEffect>(rand));
        }

        for (int j = 0; j < 2; j++)
        {
            TSense *tsense = tinst->mSenseOffset[j].ptr(header);
            if (tsense == NULL)
                continue;

            JASInstSense *sense = new (heap, 0) JASInstSense();
            sense->setType(tsense->mType);
            sense->setParams(tsense->mTrigger, tsense->mCenterKey, tsense->mMin, tsense->mMax);
            instp->appendEffect(new (heap, 0) JASPtrListAdaptor<JASInstEffect>(sense));
        }

        instp->setKeyRegionCount(tinst->mKeyRegionCount, heap);
        for (int j = 0; j < tinst->mKeyRegionCount; j++)
        {
            JASBasicInst::TKeymap *keymap = instp->getKeyRegion(j);
            TKeymap *tkeymap = tinst->mKeymapOffset[j].ptr(header);
            keymap->setHighKey(tkeymap->mHighKey);
            keymap->setVeloRegionCount(tkeymap->mVeloRegionCount, heap);
            for (int k = 0; k < tkeymap->mVeloRegionCount; k++)
            {
                JASVelo *velo = keymap->getVeloRegion(k);
                TVmap *tvmap = tkeymap->mVmapOffset[k].ptr(header);
                velo->_0 = tvmap->_0;
                velo->_4 = JSULoHalf(tvmap->_4);
                velo->_8 = tvmap->mVolume;
                velo->_c = tvmap->mPitch;
            }
        }

        bank->setInst(i, instp);
    }

    for (int i = 0; i < 12; i++)
    {
        TPerc *tperc = header->mOffsets.mPercOffset[i].ptr(header);
        if (tperc == NULL)
            continue;

        JASDrumSet *setp = new (heap, 0) JASDrumSet();
        for (int j = 0; j < 0x80; j++)
        {
            TPmap *tpmap = tperc->mPmapOffset[j].ptr(header);
            if (tpmap == NULL)
                continue;

            JASDrumSet::TPerc *percp = new (heap, 0) JASDrumSet::TPerc();
            percp->setVolume(tpmap->mVolume);
            percp->setPitch(tpmap->mPitch);
            if (tperc->mMagic == 'PER2')
            {
                percp->setPan(tperc->mPan[j] / 127.0f);
                percp->setRelease(tperc->mRelease[j]);
            }

            for (int k = 0; k < 2; k++)
            {
                TRand *trand = tpmap->mRandOffset[k].ptr(header);
                if (trand == NULL)
                    continue;

                JASInstRand *rand = new (heap, 0) JASInstRand();
                rand->setType(trand->mType);
                rand->setFloor(trand->mFloor);
                rand->setCeiling(trand->mCeiling);
                percp->appendEffect(new (heap, 0) JASPtrListAdaptor<JASInstEffect>(rand));
            }

            percp->setVeloRegionCount(tpmap->mVeloRegionCount, heap);
            for (int k = 0; k < tpmap->mVeloRegionCount; k++)
            {
                JASVelo *velo = percp->getVeloRegion(k);
                TVmap *tvmap = tpmap->mVmapOffset[k].ptr(header);
                velo->_0 = tvmap->_0;
                velo->_4 = JSULoHalf(tvmap->_4);
                velo->_8 = tvmap->mVolume;
                velo->_c = tvmap->mPitch;
            }

            setp->setPerc(j, percp);
        }

        bank->setInst(i + 0xe4, setp);
    }

    return bank;
}

JASOscillator::Data *JASBNKParser::Ver0::findOscPtr(JASBasicBank *bank, const THeader *header, const TOsc *osc)
{
    const TOffsetData *data = &header->mOffsets;
    for (int i = 0; i < 0x80; i++)
    {
        TInst *tinst = data->mInstOffset[i].ptr(header);
        if (tinst == NULL)
            continue;

        for (int j = 0; j < 2; j++)
        {
            if (tinst->mOscOffset[j].ptr(header) != osc)
                continue;

            JASInst *inst = bank->getInst(i);
            if (inst == NULL)
                continue;

            JASInstParam param;
            inst->getParam(0x3c, 0x7f, &param);
            if (j < param._20)
                return param._1c[j];
        }
    }
    return NULL;
}

const JASOscillator::Point *JASBNKParser::Ver0::getOscTableEndPtr(const JASOscillator::Point *points)
{
    const JASOscillator::Point *ptr = points;
    while (true)
    {
        s16 mode = ptr->_0;
        ptr++;
        if (mode > 10)
            break;
    }
    return ptr;
}
