#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/tu_801D8400.h"

// FIXME(shotgun): 90 %, missing/extra + reloc-name
bool cls_804A4C58::fn_801DE018() {
    bool inRange = false;
    if (mState == 5 || (mState >= 4 && mState < 8)) {
        inRange = true;
    }
    bool result = false;
    if (inRange) {
        JGeometry::TBox2f box;
        JGeometry::TVec2f out;
        box.set(-40.0f + mPos.x, -16.0f + mPos.y, 40.0f + mPos.x, 50.0f + mPos.y);
        s32 player = fn_8023AE34(&box, field_0x4, &out);
        if (player < 0) {
            player = fn_8023AA18(&box, field_0x4, &out);
        }
        if (player < 0) {
            player = fn_8023AC74(&box, field_0x4, &out);
        }
        if (player < 0) {
            player = fn_8023AD54(&box, field_0x4, &out);
        }
        if (player >= 0) {
            fn_801D9C58(5);
            result = true;
        }
    }
    return result;
}

// FIXME(shotgun): 98 %, missing/extra
bool cls_804A4C58::fn_801DD1AC() {
    bool clamped = false;
    if (!(mFlags & ENEMY_FLAG_NO_MAP_HIT)) {
        if (mPos.x < field_0x2d4[0].x) {
            mPos.x = field_0x2d4[0].x;
            clamped = true;
            mMapHitDir.set(1.0f, 0.0f, 0.0f);
        } else if (field_0x2d4[3].x < mPos.x) {
            mPos.x = field_0x2d4[3].x;
            clamped = true;
            mMapHitDir.set(-1.0f, 0.0f, 0.0f);
        }
        if (mPos.y > field_0x2d0) {
            mPos.y = field_0x2d0;
            clamped = true;
            mMapHitDir.set(0.0f, -1.0f, 0.0f);
        } else if (mState >= 0xB && mState < 0x11) {
            if (mPos.y < field_0x2d4[0].y) {
                mPos.y = field_0x2d4[0].y;
                clamped = true;
                mMapHitDir.set(0.0f, 1.0f, 0.0f);
            }
        }
    }
    return clamped;
}
