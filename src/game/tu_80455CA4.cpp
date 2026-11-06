#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "JSystem/JAudio/System/JASTrack.h"

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
