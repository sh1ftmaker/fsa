#include "game/GameAudio.h"
#include "game/act/tu_801DE714.h"
#include "game/ActorManager.h"

void cls_804A51E4::fn_8021FDB8() const {
    s32 serial = mTargetSerial;
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* target = fn_801F856C()->fn_801F666C(serial);
    if (target == NULL) {
        return;
    }
    f32 dz = 0.0f;
    switch (mAnim.mCel) {
    case 0:
    case 3:
    case 6:
        dz = 1.0f;
        break;
    case 2:
    case 5:
        dz = -1.0f;
        break;
    }
    JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z + dz);
    switch (target->field_0x90 & 0xFF) {
    case 0:
    case 1:
        pos.z -= 3.0f;
        if (!fn_801E1D9C()) {
            switch (field_0x98) {
            case 0:
                pos.x += 3.0f;
                break;
            case 1:
                pos.x -= 3.0f;
                break;
            }
        }
        break;
    case 2:
    case 3:
        pos.z -= 5.0f;
        if (!fn_801E1D9C()) {
            switch (field_0x98) {
            case 0:
                pos.x += 3.0f;
                break;
            case 1:
                pos.x -= 3.0f;
                break;
            }
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        pos.z -= 13.0f;
        if (!fn_801E1D9C()) {
            switch (field_0x98) {
            case 0:
                pos.x += 3.0f;
                break;
            case 1:
                pos.x -= 3.0f;
                break;
            }
        }
        break;
    }
    target->mPos = pos;
}
