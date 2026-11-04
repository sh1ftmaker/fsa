#include "game/GameAudio.h"
#include "game/ActorManager.h"

void ActorSoundObj_804578F4::fn_80456120() {
    field_0x31 = 0;
    dispose();
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
