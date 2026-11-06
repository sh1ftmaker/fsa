#include "game/GameAudio.h"
#include "Inagaki/GameSoundTable.h"
#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAIStream.h"

namespace GameAudio {


inline void CustomSoundTable::getSoundInfo_(JAISoundID soundID, JAISound* sound) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x10:
            sound->params_.mProperty._0 = data->mVolume / 127.f;
            break;
        case 0x12:
            sound->params_.mProperty._0 = data->mVolume / 127.f;
            break;
        case 0x11:
            sound->params_.mProperty._0 = data->mVolume / 127.f;
            break;
        }
    }
}

// 0x8045AD64
u16 CustomSoundTable::getBgmSeqResourceID(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x11:
            return data->mSeqResourceID;
        }
    }
    return -1;
}

// 0x8045AD30
u32 CustomSoundTable::getSoundType(JAISoundID soundID) const {
    switch (soundID.mId.mBytes.mSectionId) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return -1;
    }
}

// 0x8045ACDC
int CustomSoundTable::getCategory(JAISoundID soundID) const {
    if (soundID.mId.mBytes.mSectionId == 1) {
        return getPTrack(soundID) - 1;
    }
    return soundID.mId.mBytes.mGroupId;
}

// 0x8045ABC8
u16 CustomSoundTable::getAudibleSw(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x10: {
            u16 acc = 0xF0;
            u32 swBit = getSwBit(soundID);
            if ((swBit & 0x1) == 0) {
                acc += 0x800;
            }
            if ((swBit & 0x2) == 0) {
                acc += 0x400;
            }
            if ((swBit & 0x4) == 0) {
                acc += 0x200;
            }
            if (swBit & 0x20) {
                acc += 0x100;
            }
            if (swBit & 0x800000) {
                acc += 0x1;
            }
            if (soundID.mId.mBytes.mGroupId == 6) {
                acc &= 0xFF7F;
            }
            return acc;
        }
        }
    }
    return -1;
}

// 0x8045AB44
u8 CustomSoundTable::getPTrack(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x11:
            return data->mPTrack;
        }
    }
    return 0xFF;
}

// 0x8045AAAC
u32 CustomSoundTable::getSwBit(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x11:
            return data->mSwBit;
        case 0x10:
            return data->mSwBit;
        }
    }
    return -1;
}

// 0x8045A9FC
u8 CustomSoundTable::getLevelFlag(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        if (typeID & 0x10) {
            return (data->mPriority >> 2) != 0;
        }
        if (typeID & 0x12) {
            return (data->mPriority >> 2) != 0;
        }
    }
    return 0;
}

// 0x8045A944
u32 CustomSoundTable::getPriority(JAISoundID soundID) const {
    JAUSoundTableItem* data = mSoundTable->getData(soundID);
    const u8 typeID = mSoundTable->getTypeID(soundID);

    if (data != NULL) {
        switch (typeID) {
        case 0x10:
            return 0xFF - data->_1;
        case 0x12:
            return 0xFF - data->_1;
        case 0x11:
            return 0xFF - data->_1;
        }
    }
    return 0;
}

// 0x8045A81C
void CustomSoundTable::getSeInfo(JAISoundID soundID, JAISe* se) const {
    getSoundInfo_(soundID, se);
}

// 0x8045A700
void CustomSoundTable::getSeqInfo(JAISoundID soundID, JAISeq* seq) const {
    getSoundInfo_(soundID, seq);
}

// 0x8045A6FC
void CustomSoundTable::getStreamInfo(JAISoundID soundID, JAIStream* stream) const {}

// 0x8045A6F4
s32 CustomSoundTable::getStreamFileEntry(JAISoundID soundID) {
    return -1;
}

}  // namespace GameAudio
