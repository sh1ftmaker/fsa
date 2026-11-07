#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JAudio/System/JASTrack.h"
#include "Inagaki/GameSoundTable.h"

// FIXME(shotgun): 92.9 %, extra/missing insns (the original also stores 0x54/0x58 to stack temporaries)
ActorSoundObj_804578F4::ActorSoundObj_804578F4(JGeometry::TVec3f* pos, JKRHeap* heap, u8 numHandles)
    : JAUSoundObject(pos, numHandles, heap) {
    field_0x31 = 1;
    field_0x34 = pos;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
    field_0x44 = 0;
    field_0x45 = 0;
    field_0x50 = 0;
    field_0x51 = 0;
    field_0x52 = 0;
    field_0x4c = 1.0f;
    field_0x48 = 0.0f;
    field_0x54 = 0x400A5;
    field_0x58 = 0x3006D;
    field_0x30 = 0;
}

ActorSoundObj_804578F4::~ActorSoundObj_804578F4() {}

void ActorSoundObj_804578F4::fn_80457840(JGeometry::TVec3f* pos) {
    field_0x34 = pos;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
}

// FIXME(shotgun): 96.2 %, missing/extra + reloc-name
void ActorSoundObj_804578F4::fn_804561D8(JAISoundID id, u8 a, u32 fade, u8 c) {
    field_0x44 = c;
    JAISoundHandle* handle = fn_80456334(id, fade);
    JAISound* sound = handle->sound_;
    if (sound != NULL) {
        f32 pan;
        f32 vol;
        switch (a) {
        case 3:
            pan = 0.0f;
            vol = 0.4f;
            break;
        case 2:
            pan = 1.0f;
            vol = 0.4f;
            break;
        case 0:
        case 1:
            pan = 0.5f;
            vol = 0.4f;
            break;
        default:
            pan = 0.5f;
            vol = 0.0f;
            break;
        }
        sound->getAuxiliary().movePan(pan, 0);
        if (c != 0) {
            handle->sound_->getAuxiliary().moveVolume(1.0f, 0);
        } else {
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
        }
        if (field_0x44 != 1) {
            if (gAudioMgr_8053AF80->field_0x6 == 1) {
                handle->sound_->getAuxiliary().moveVolume(0.35f, 0);
            } else {
                handle->sound_->getAuxiliary().moveVolume(vol, 0);
            }
        }
    }
}

void ActorSoundObj_804578F4::fn_80456154() {
    JGeometry::TVec3f* pos = field_0x34;
    if (pos != NULL) {
        field_0x38.x = pos->x;
        field_0x38.y = pos->y;
        field_0x38.z = pos->z;
        mSoundPos = &field_0x38;
    } else {
        mSoundPos = NULL;
    }
    if (mSoundPos != NULL) {
        f32 y = mSoundPos->y;
        mSoundPos->y = mSoundPos->z;
        mSoundPos->z = y;
    }
    if (field_0x50 != 0) {
        field_0x50--;
    }
    if (field_0x51 != 0) {
        field_0x51--;
    }
}

void ActorSoundObj_804578F4::fn_80456120() {
    field_0x31 = 0;
    dispose();
}

// A sound may stop unless its sound-table switch bits have 0x8000 set.
// FIXME(shotgun): 89.0 %, scheduling+regalloc
bool ActorSoundObj_804578F4::stopOK(JAISoundHandle& handle) {
    u32 bits = gAudioMgr_8053AF80->field_0x40->getSwBit(handle->getID());
    return !(bits & 0x8000);
}

void ActorSoundObj_804578F4::fn_80455F40(JAISoundHandle* handle) {
    if (handle == NULL) {
        return;
    }
    JAISound* sound = handle->sound_;
    if (sound == NULL) {
        return;
    }
    JASTrack* track = sound->getTrack();
    if (field_0x44 == 1) {
        track->writePort(0xA, 1);
        switch (field_0x45) {
        case 2:
            track->writePort(0x9, 0x78);
            track->writePort(0xB, 0);
            break;
        case 3:
            track->writePort(0xB, 1);
            track->writePort(0x9, 0x3C);
            break;
        // Case 1 has the default's body; the empty label is what makes the
        // switch's compare tree match.
        case 1:
        default:
            track->writePort(0x9, 0);
            track->writePort(0xB, 0);
            break;
        }
    } else {
        track->writePort(0xA, 0);
        switch (field_0x45) {
        case 2:
            track->writePort(0x9, 0x78);
            track->writePort(0xB, 0);
            break;
        case 3:
            track->writePort(0xB, 1);
            track->writePort(0x9, 0x3C);
            break;
        // Case 1 has the default's body; the empty label is what makes the
        // switch's compare tree match.
        case 1:
        default:
            track->writePort(0x9, 0);
            track->writePort(0xB, 0);
            break;
        }
    }
}

// fn_80455CA4: pans and sets the volume of a started sound from a screen position
// (pos x, y in 0..1).
// FIXME(shotgun): 96.3 %, missing/extra
void fn_80455CA4(ActorSoundObj_804578F4* obj, JAISoundHandle* handle, JGeometry::TVec2f* pos, JAISoundID* id) {
    f32 x = pos->x;
    if (x < 0.0f) {
        handle->sound_->getAuxiliary().movePan(0.0f, 0);
        handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
    } else if (x > 1.0f) {
        handle->sound_->getAuxiliary().movePan(1.0f, 0);
        handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
    } else {
        f32 y = pos->y;
        if (y < 0.0f) {
            handle->sound_->getAuxiliary().movePan(x, 0);
            handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
        } else if (y > 1.0f) {
            handle->sound_->getAuxiliary().movePan(x, 0);
            handle->sound_->getAuxiliary().moveVolume(0.4f, 0);
        } else if (x < 0.1f || y < 0.1f) {
            f32 m = x <= y ? x : y;
            f32 vol = 10.0f * m + 0.4f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
            handle->sound_->getAuxiliary().movePan(pos->x, 0);
        } else if (x > 0.9f || y > 0.9f) {
            f32 m = x >= y ? x : y;
            f32 vol = 10.0f * (1.0f - m) + 0.4f;
            if (vol > 1.0f) {
                vol = 1.0f;
            }
            handle->sound_->getAuxiliary().moveVolume(vol, 0);
            handle->sound_->getAuxiliary().movePan(pos->x, 0);
        } else {
            handle->sound_->getAuxiliary().movePan(x, 0);
            if (id->mId.mBytes.mGroupId != 0) {
                u32 v = *id;
                if (v - 0x30000 != 9 && v - 0x30000 != 0x51 && v - 0x30000 != 0x4B && v - 0x50000 != 0x6D) {
                    if (obj->field_0x44 != 1) {
                        if (gAudioMgr_8053AF80->field_0x6 == 1) {
                            handle->sound_->getAuxiliary().moveVolume(0.35f, 0);
                        } else {
                            handle->sound_->getAuxiliary().moveVolume(1.0f, 0);
                        }
                    }
                }
            }
        }
    }
}
