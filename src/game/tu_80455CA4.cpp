#include "game/GameAudio.h"
#include "game/ActorManager.h"

void ActorSoundObj_804578F4::fn_80456120() {
    field_0x31 = 0;
    dispose();
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

ActorSoundObj_804578F4::~ActorSoundObj_804578F4() {}
