
#include "JSystem/JAudio/Interface/JAISeMgr.h"
#include "JSystem/JAudio/JAUSectionHeap.h"
#include "JSystem/JAudio/JAUAudioArchive.h"

JAUAudioArcLoader::JAUAudioArcLoader(JAUSection *section)
{
    JUT_ASSERT(section->isOpen());
    JUT_ASSERT(section->isBuilding());
    mSection = section;
}

bool JAUAudioArcLoader::load(const void *data)
{
    return parse(data);
}

bool JAUAudioArcLoader::load(const char *path)
{
    const void *archive = mSection->loadDVDFile(path, true, EXPAND_SWITCH_DEFAULT);
    return archive && load(archive);
}

void JAUAudioArcLoader::readWS(u32 bank_no, const void *data, u32 flag)
{
    mSection->newWaveBank(bank_no, data);
    if (flag)
    {
        mSection->loadWaveArc(bank_no, flag);
    }
}

void JAUAudioArcLoader::readBNK(u32 bankNum, const void *data)
{
    mSection->newBank(data, bankNum);
}

void JAUAudioArcLoader::readBSC(const void *data, u32 size)
{
    mSection->newSeSeqCollection(data, size);
}

void JAUAudioArcLoader::readBST(const void *data, u32 size)
{
    mSection->newSoundTable(data, size, true);
}

void JAUAudioArcLoader::readBSTN(const void *data, u32 size)
{
    mSection->newSoundNameTable(data, size, true);
}

void JAUAudioArcLoader::readBMS(u32 soundID, const void *data, u32 size)
{
    mSection->newStaticSeqData(soundID, data, size);
}

void JAUAudioArcLoader::readBMS_fromArchive(u32 soundID)
{
    mSection->newStaticSeqData(soundID);
}

void JAUAudioArcLoader::newVoiceBank(u32 waveBankNo, u32 bankNo)
{
    mSection->newVoiceBank(bankNo, waveBankNo);
}

void JAUAudioArcLoader::newDynamicSeqBlock(u32 size)
{
    JAUSectionHeap *sectionHeap = mSection->asSectionHeap();
    JUT_ASSERT(sectionHeap);
    sectionHeap->newDynamicSeqBlock(size);
}

void JAUAudioArcLoader::readBSFT(const void *data)
{
    mSection->newStreamFileTable(data, true);
}

void JAUAudioArcLoader::beginBNKList(u32 p1, u32 cnt)
{
    mSection->beginNewBankTable(p1, cnt);
}

void JAUAudioArcLoader::endBNKList()
{
    mSection->endNewBankTable();
}

void JAUAudioArcLoader::readMaxSeCategory(int categoryIndex, int activSE, int inactiveSE)
{
    JAISeMgr *seMgr = JASGlobalInstance<JAISeMgr>::getInstance();
    if (seMgr)
    {
        seMgr->getCategory(categoryIndex)->setMaxActiveSe(activSE);
        seMgr->getCategory(categoryIndex)->setMaxInactiveSe(inactiveSE);
    }
}
