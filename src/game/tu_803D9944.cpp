#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/tu_803D9944.h"

cls_804B0478::~cls_804B0478() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// FIXME(shotgun): 97.1 %, missing/extra
void cls_804B0478::init() {
    mBox.i.x = -1280.0f;
    mBox.i.y = -1280.0f;
    mBox.f.x = 1280.0f;
    mBox.f.y = 1280.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x24c = 0;
    field_0x250 = 1;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    mEmitters[2] = NULL;
    mEmitters[3] = NULL;
    field_0x251 = 0;
    field_0x11f = 1;
    if ((s32)field_0x90 == 0) {
        s32 player = fn_8022EA0C(0);
        if (player >= 0) {
            const JGeometry::TVec3f* p = getPlayerPos_8023E724(player);
            mPos.x = p->x;
            mPos.y = p->y;
            mPos.z = p->z;
            mPos.y += 16.0f;
            field_0x4 = getPlayerLayer_8023DE58(player);
        }
    }
    for (int i = 0; i < 4; i++) {
        const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
        field_0x260[i].x = p->x;
        field_0x260[i].y = p->y;
        field_0x260[i].z = p->z;
        field_0x260[i].y += 16.0f;
    }
    field_0x254.x = mPos.x;
    field_0x254.y = mPos.y;
    field_0x254.z = mPos.z;
    if (field_0x90 == 0) {
        field_0x248 = 1;
        mState = 0;
    } else {
        mState = 5;
    }
}

// FIXME(shotgun): not written (size 0x7BC): update (0x803D9AA4)

// FIXME(shotgun): 79.5 %, missing/extra
void cls_804B0478::draw() {
    if (field_0x248 != 0) {
        return;
    }
    s32 base = 0x34c;
    if (fn_80237060(0) >= 2) {
        base = 0x364;
    }
    if (field_0x90 == 0) {
        f32 x = mPos.x;
        f32 y = mPos.y;
        f32 z = mPos.z;
        for (int i = 0; i < 4; i++) {
            if (getPlayerLayer_8023DE58(i) >= 8) {
                mPos.x = field_0x260[i].x;
                mPos.y = field_0x260[i].y;
                mPos.z = field_0x260[i].z;
                s32 id = base + field_0x24c;
                fn_803075AC(i - 0x22b, getPlayerLayer_8023DE58(i), &mPos, id, false, 2, -1, true, 0, 1);
            }
        }
        mPos.x = x;
        mPos.y = y;
        mPos.z = z;
    } else {
        fn_803075AC(mSerial, field_0x4, &mPos, base + field_0x24c, false, 2, -1, true, 0, 1);
    }
}
