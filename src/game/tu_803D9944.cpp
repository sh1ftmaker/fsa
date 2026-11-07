#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/Message.h"
#include "game/GameState.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/act/tu_803D9944.h"

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

cls_804B0478::~cls_804B0478() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// Indexed by update's states 7 to 10 (see the sub-tables there); owned by this unit
// (.data 0x804B03E8, 0x90 bytes) but not written here yet.
extern s32 lbl_804B03E8[];
extern s32 lbl_8053A210[]; // .sdata, 2 entries: state 5

// FIXME(shotgun): 90.68 %, different + reloc-name
void cls_804B0478::update() {
    switch (mState) {
    case 0:
        if (field_0x240 < 60) {
            field_0x240++;
        } else {
            field_0x240 = 0;
            field_0x244 = -1;
            field_0x248 = 0;
            for (s32 i = 0; i < 4; i++) {
                if (getPlayerLayer_8023DE58(i) < 8) {
                    s32 id = 0x3ae;
                    if (fn_80237060(0) == 2) {
                        id = 0x3af;
                    }
                    spawnEmitter_8013CC50(getPlayerLayer_8023DE58(i), getPlayerPos_8023E724(i), id, 1, 1, NULL, NULL, true, 1.0f);
                    id = 0x3ac;
                    if (fn_80237060(0) == 2) {
                        id = 0x3ad;
                    }
                    mEmitters[i] = spawnEmitter_8013CC50(getPlayerLayer_8023DE58(i), getPlayerPos_8023E724(i), id, 1, 1, NULL, NULL, true, 1.0f);
                }
            }
            field_0x238 = 60;
            mState = 1;
        }
        break;
    case 1: {
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        for (s32 i = 0; i < 4; i++) {
            addCalc2_801F71A4(&field_0x260[i].y, getPlayerPos_8023E724(i)->y, 0.3f, 1.0f);
        }
        if (field_0x238 == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (getPlayerLayer_8023DE58(i) >= 8) {
                    const JGeometry::TVec3f* p = getPlayerPos_8023E724(i);
                    field_0x260[i].x = p->x;
                    field_0x260[i].y = p->y;
                    field_0x260[i].z = p->z;
                }
            }
            JGeometry::TBox2f box = mBox;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.i.y += mPos.y;
            box.f.y += mPos.y;
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            field_0x244 = fn_8022FD8C(mSerial);
            if (field_0x244 >= 0) {
                // The original passes an uninitialised vector here.
                JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
                if (fn_8038BD08(field_0x244, 0x406, field_0x4, pos, 0x28)) {
                    mState = 2;
                }
            }
        }
        break;
    }
    case 2:
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        if (fn_80388CAC(field_0x244, mSerial)) {
            if (gGameRoot_8053AAF8->fn_80138A30() == 1) {
                startSound_801F0E34(0x5006c, 0);
            }
            fn_80384C58(field_0x244, mSerial);
            field_0x238 = 0x15e;
            field_0x23c = 0x5a;
            for (s32 i = 0; i < 4; i++) {
                if ((bool)(gGameRoot_8053AAF8->mPlayerMask & (1 << i))) {
                    if (fn_80237774(i) < fn_80237810(i) - 4) {
                        fn_80236554(i, fn_80237810(i) - fn_80237774(i) - 4, true);
                    }
                }
            }
            mState = 3;
        }
        break;
    case 3:
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        if (field_0x23c == 30 && gGameRoot_8053AAF8->fn_80138A30() > 1) {
            startSound_801F0E34(0x5006c, 0);
        }
        if (field_0x23c == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (mEmitters[i] != NULL) {
                    mEmitters[i]->becomeInvalidEmitter();
                    mEmitters[i] = NULL;
                }
            }
            field_0x248 = 1;
        }
        if (field_0x238 == 0x15c) {
            field_0x251 = 1;
        }
        if (field_0x238 == 0) {
            field_0x238 = 0x46;
            lbl_8053AAA0 = 1;
            lbl_8053AB98 = 1;
            lbl_8053ABE8 = 1;
            mState = 4;
        }
        break;
    case 4:
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        if (field_0x238 == 0) {
            lbl_8053AEC8->field_0x59 = field_0x250;
            field_0x250 = 0xff;
            if (lbl_80529DEC.field_0x24f != 0) {
                lbl_80529DEC.field_0x24f--;
            } else {
                lbl_80529DEC.field_0x24f = 0;
            }
            lbl_8053AED0->fn_8043D67C(0x33a, 0, lbl_80529DEC.field_0x24f);
            mAlive = false;
        }
        break;
    case 5:
        field_0x244++;
        if (field_0x244 == 2) {
            field_0x251 = 1;
        }
        if (field_0x234 == 0) {
            field_0x24c = lbl_8053A210[field_0x240];
            field_0x240++;
            field_0x240 = 1;
            if (field_0x240 == 0) {
                field_0x234 = 2;
            } else {
                field_0x234 = 4;
            }
        }
        if (field_0x244 >= 10) {
            mState = 6;
        }
        break;
    case 6:
        field_0x234 = 0;
        field_0x240 = 0;
        mState = 7;
        // fall through
    case 7:
        field_0x244++;
        if (field_0x234 == 0) {
            field_0x24c = lbl_804B03E8[field_0x240];
            field_0x240++;
            field_0x234 = 2;
            if (field_0x240 > 2) {
                field_0x240 = 0;
                mState = 8;
            }
        }
        break;
    case 8:
        field_0x244++;
        if (field_0x234 == 0) {
            field_0x24c = lbl_804B03E8[3 + field_0x240];
            field_0x240++;
            field_0x234 = 2;
            if (field_0x240 > 2) {
                field_0x240 = 0;
            }
        }
        if (field_0x244 >= 300) {
            field_0x240 = 0;
            mState = 9;
        }
        break;
    case 9:
        field_0x244++;
        if (field_0x234 == 0) {
            field_0x24c = lbl_804B03E8[6 + field_0x240];
            field_0x240++;
            field_0x234 = 2;
            if (field_0x240 > 7) {
                field_0x240 = 7;
            }
        }
        if (field_0x244 >= 0x15e) {
            field_0x240 = 0;
            field_0x234 = 0;
            mState = 10;
        }
        break;
    case 10:
        if (field_0x234 == 0) {
            field_0x24c = lbl_804B03E8[14 + field_0x240];
            field_0x234 = lbl_804B03E8[23 + field_0x240];
            field_0x240++;
            if (field_0x240 > 9) {
                mAlive = false;
            }
        }
        break;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

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
