#include "game/ActorBase.h"
#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/cls_804A1630.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "std/math.h"
#include "game/RotMatrix.h"
#include "game/SpriteParams.h"

// FIXME(shotgun): draw's texture tables, inside the lbl_804A1440 block (.data 0x804A1460 on):
// texture names and y offsets by frame, a set for the normal states and one for states 9
// and 15. Extern-declared under their addresses; sizes from the gaps between them.
extern const char* const lbl_804A1460[30];
extern const f32 lbl_804A14D8[30];
extern const char* const lbl_804A1550[28];
extern const f32 lbl_804A15C0[28];

// setRotScaleMtx with a scale per axis (draw mirrors the sprite with x = -1).
static inline void setRotScaleMtx3(Mtx m, const JGeometry::TVec3f& rot, f32 sx, f32 sy, f32 sz) {
    f32 sinx = sin(rot.x);
    f32 siny = sin(rot.y);
    f32 sinz = sin(rot.z);
    f32 cosx = cos(rot.x);
    f32 cosy = cos(rot.y);
    f32 cosz = cos(rot.z);
    m[0][0] = sx * (cosy * cosz);
    m[1][0] = sx * (cosy * sinz);
    m[2][0] = sx * -siny;
    m[0][1] = sy * (sinx * siny * cosz - cosx * sinz);
    m[1][1] = sy * (sinx * siny * sinz + cosx * cosz);
    m[2][1] = sy * (sinx * cosy);
    m[0][2] = sz * (cosx * cosz * siny + sinx * sinz);
    m[1][2] = sz * (cosx * sinz * siny - sinx * cosz);
    m[2][2] = sz * (cosx * cosy);
}

// The marker frames of the SHA actor (.data 0x804A1440, not written as source here), by row.
extern const s32 lbl_804A1440[];
extern s32 lbl_80539F98;
extern s32 lbl_80539F9C;
extern s32 lbl_80539FA0;
extern s32 lbl_80539FA4;
extern s32 lbl_80539FA8[2];
extern s32 lbl_80539FB0[2];
extern s32 lbl_8053AB70;
extern u8 lbl_8053AB74;
extern u8 lbl_8053AB75;

struct Unk_804A13E0 {
    f32 field_0x00[2][2];
    f32 field_0x10[2][2];
    s32 field_0x20[2][4];
    s32 field_0x40;
    s32 field_0x44[3];
    s32 field_0x50[2][2];
};

extern const Unk_804A13E0 lbl_804A13E0;

u8 fn_8017ECC0();
void fn_8017ED08();
bool fn_8017EB1C(f32 value);
u8 fn_8017EB44(const JGeometry::TVec3f* pos);
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value);

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_801826E4(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// The view's box for an actor layer: mVisibleBox for layers 0-7, field_0xdc
// for the rest.
JGeometry::TBox2f* fn_801826A4(s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    if (layer < 8) {
        return &view->mVisibleBox;
    }
    return &view->field_0xdc;
}

// Out-of-line copy of TVec2<f32>::set(const TVec2&).
void fn_80182690(JGeometry::TVec2f* dst, const JGeometry::TVec2f* src) {
    dst->set(*src);
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_80182674(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

void ActorBase_804A5770::fn_80182660(const JGeometry::TVec3f& pos) {
    mPos = pos;
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80182650(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// A second out-of-line copy of TVec3<f32>::set(x, y, z).
void fn_80182640(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// FIXME(shotgun): 98.9 %, missing/extra + reloc-name.
void cls_804A1630::init() {
    field_0x90 &= 0xF;
    field_0x94 = 0;
    field_0x80.i.x = -1.0f;
    field_0x80.i.y = -1.0f;
    field_0x80.f.x = 1.0f;
    field_0x80.f.y = 1.0f;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x196 = 1;
    field_0x278 = 0;
    field_0x274 = -1;
    for (s32 i = 0; i < 4; i++) {
        field_0x294[i].set(mPos);
        field_0x2c4[i].set(mPos);
        field_0x2f4[i] = 0;
        field_0x304[i] = 0;
    }
    field_0x98 = 0;
    field_0x26c = 0;
    field_0x314 = 0.0f;
    mEmitter318[0] = NULL;
    mEmitter318[1] = NULL;
    field_0x270 = -1;
    field_0x288.set(mPos);
    field_0x30c = 1.0f;
    field_0x310 = 255.0f;
    field_0x27c.z = 0.0f;
    field_0x27c.y = 0.0f;
    field_0x27c.x = 0.0f;
    field_0x308 = 0.0f;
    switch (field_0x90) {
    case 0:
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        getGameView_802D6A9C()->fn_802D7440(false);
        gAudioMgr_8053AF80->fn_80458F9C(60);
        lbl_8053AB70 = 0;
        lbl_80539F98 = 8;
        lbl_80539F9C = 180;
        if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
            lbl_80539F9C = 110;
        }
        lbl_8053AB74 = 0;
        lbl_8053AB75 = 0;
        field_0x268 = -1;
        for (s32 i = 0; i < 4; i++) {
            fn_80304C10(i, "gb_ch_obj_tr_sch0.szs", 0x6017000, 1, 0);
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9714(
            gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_sch0.szs"), 0, 0, 0x7000, 0);
        field_0xb0 = 0x9040004;
        field_0x98 = 1;
        mPos.z = 500.0f;
        field_0x308 = 500.0f;
        mState = 0;
        break;
    case 1:
        field_0xb0 = 0x9040004;
        field_0x1a8 = false;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        mState = 0x10;
        break;
    case 4:
        field_0x98 = 1;
        // fall through
    case 5:
        field_0xb0 = 0x5000000;
        field_0x1a8 = false;
        mBox.i.x = -32.0f;
        mBox.i.y = -32.0f;
        mBox.f.x = 32.0f;
        mBox.f.y = 32.0f;
        mBox2.i.x = -48.0f;
        mBox2.i.y = -48.0f;
        mBox2.f.x = 48.0f;
        mBox2.f.y = 48.0f;
        mState = 0x11;
        break;
    case 6:
        field_0x1a8 = false;
        field_0x30c = 0.2f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f) + 0.1f;
        mState = 0x15;
        break;
    case 7:
        field_0x308 = 500.0f;
        field_0x1a8 = false;
        field_0xb0 = 0x4000000;
        mState = 0x17;
        break;
    case 8:
        field_0x26c = (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 0x13;
        field_0x1a8 = false;
        mState = 0x19;
        break;
    case 9:
        field_0x1a8 = false;
        field_0x234 = 20;
        field_0x24c = 1;
        if (field_0x98 == 0) {
            field_0x26c = 0x16;
        } else {
            field_0x26c = 0x17;
        }
        mPos.y -= 22.0f;
        mState = 0x19;
        break;
    case 10:
        field_0x26c = (s32)(2.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) + 0x10;
        field_0x1a8 = false;
        mState = 0x19;
        break;
    default:
        if (field_0x90 == 2) {
            lbl_80539FA0 = mSerial;
        } else if (field_0x90 == 3) {
            lbl_80539FA4 = mSerial;
        }
        field_0x1a8 = false;
        field_0xb0 = 0x4000000;
        mBox.i.x = -16.0f;
        mBox.i.y = -16.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        mBox2.i.x = -16.0f;
        mBox2.i.y = -16.0f;
        mBox2.f.x = 16.0f;
        mBox2.f.y = 16.0f;
        mState = 0x16;
        break;
    }
}

// The actor's destructor: unless field_0x90 is set, it turns the view's flag
// off and calls fn_8022ADCC; then it releases its two emitters.
cls_804A1630::~cls_804A1630() {
    if (field_0x90 == 0) {
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    for (s32 i = 0; i < 2; i++) {
        if (mEmitter318[i] != NULL) {
            releaseEmitter_801EE434(mEmitter318[i]);
        }
    }
}

// FIXME(shotgun): 97.75 %, missing/extra + reloc-name.
void cls_804A1630::fn_80181BC8(s32 row, s32 mode) {
    s32 frame = 0;
    for (s32 i = 0; i < 4; i++) {
        if (!fn_8023F264(i)) {
            continue;
        }
        if (field_0x4 != getPlayerLayer_8023DE58(i)) {
            continue;
        }
        switch (mode) {
        case 0:
            if (getPlayerPos_8023E724(i)->x < mPos.x) {
                row = 0;
            } else {
                row = 1;
            }
            field_0x2f4[i] = 0;
            field_0x294[i].set(*getPlayerPos_8023E724(i));
            field_0x2c4[i].set(*getPlayerPos_8023E724(i));
            field_0x294[i].z = 0.0f;
            field_0x2c4[i].z = 0.0f;
            field_0x304[i] = 0;
            break;
        case 1:
            if (field_0x304[i] == 1) {
                row += 4;
            }
            if (field_0x2f4[i] < 4) {
                frame = 0;
            } else if (field_0x2f4[i] < 8) {
                frame = 1;
            } else if (field_0x2f4[i] < 11) {
                frame = 2;
            } else if (field_0x2f4[i] < 14) {
                frame = 3;
            } else if (field_0x2f4[i] < 17) {
                frame = 4;
            } else if (field_0x2f4[i] < 20) {
                frame = 5;
            } else {
                field_0x2f4[i] = 20;
                frame = 0;
                row = 3;
            }
            addCalc2_801F71A4(&field_0x294[i].y, field_0x2c4[i].y, 0.3f, 1.0f);
            if (field_0x304[i] == 0) {
                f32 dx = getPlayerPos_8023E724(i)->x - mPos.x;
                if (std::sqrtf(dx * dx) < 128.0f) {
                    f32 dy = getPlayerPos_8023E724(i)->y - field_0x288.y;
                    if (std::sqrtf(dy * dy) < 80.0f) {
                        field_0x2c4[i].y = 30.0f + getPlayerPos_8023E724(i)->y;
                        field_0x304[i] = 1;
                    }
                }
            } else {
                field_0x2f4[i]++;
            }
            break;
        }
        if (mode == 0 || field_0x304[i] == 1) {
            fn_80230498(i, &field_0x294[i], frame + lbl_804A1440[row], 0xFF, -0x80, -0x80, true);
        }
    }
}

// FIXME(shotgun): 94.03 %, missing/extra + reloc-name
void cls_804A1630::update() {
    JGeometry::TBox2f box40(mBox);
    JGeometry::TVec2f pos1c(mPos.x, mPos.y);
    JGeometry::TBox2f* viewBox = fn_801826A4(field_0x4);
    f32 dist = 0.0f;
    s32 dir = -1;
    s32 player = -1;

    mLife = 10000;
    if (field_0x90 == 0) {
        fn_8017ECC0();
    }
    if (vfunc_0x2C(0.0f, -24.0f, 50.0f)) {
        fn_8017ED08();
        if (field_0x188 == 1) {
            lbl_8053AB74 = 1;
        }
    }
    fn_801F3D94(0);
    if (field_0x23c != 0 && field_0x260 == 2 && fn_801F2DB4('HBMB', 'NULL', NULL) && mLife != 10000) {
        field_0x3c.zero();
        field_0x27c.zero();
        mLife = 10000;
        field_0x23c = 0;
        if (mEmitter318[0] != NULL) {
            mEmitter318[0]->becomeInvalidEmitter();
            mEmitter318[0] = NULL;
        }
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x164, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x40033, 0);
        field_0x238 = 0;
        if (lbl_80539F98 <= 0) {
            field_0x238 = 300;
        }
        if (mEmitter318[1] != NULL) {
            mEmitter318[1]->becomeInvalidEmitter();
            mEmitter318[1] = NULL;
        }
        mState = 8;
    }
    if (mState != 9 && mState != 11 && mState != 12 && mState != 0x15 && mState != 15 && mState != 0x19) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x27c.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x27c.y, 0.3f, 1.0f);
        addCalc2_801F71A4(&mPos.z, field_0x308, 0.5f, 6.0f);
    }
    mPos.add(field_0x3c);
    if (field_0x90 == 0 && (mState == 0 || (u32)(mState - 1) <= 5 || mState == 7)) {
        if (mEmitter318[0] == NULL) {
            mEmitter318[0] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x165, 0, 0, NULL, NULL, true, 1.0f);
        } else {
            mEmitter318[0]->setGlobalTranslation(mPos);
        }
    }
    if (field_0x90 == 0 || field_0x90 == 7) {
        if (lbl_80539F98 <= 0) {
            mBox.set(-58.0f, -24.0f, 58.0f, 24.0f);
            mBox2.set(-58.0f, -24.0f, 58.0f, 24.0f);
        }
        fn_801EEC98(mSerial, NULL, &dir, &dist, &player, 0, 1);
    }
    if (mState != 0 && mState != 1 && mState != 0x17 && mState != 0x18 && mState != 0x19) {
        if (lbl_8053AB75 == 1) {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        } else if (field_0x90 == 0) {
            fn_8017EB1C(0.05f);
            if (field_0x268 >= 0) {
                if (field_0x4 != getPlayerLayer_8023DE58(field_0x268)) {
                    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&field_0x288);
                    fn_8017EB44(&field_0x288);
                } else {
                    fn_8017EC70(getPlayerPos_8023E724(field_0x268), 1.0f);
                    fn_8017EB1C(0.05f);
                }
            }
        }
    }
    if (field_0x270 >= 0) {
        cls_804A1630* other = static_cast<cls_804A1630*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x270));
        if (other != NULL) {
            JGeometry::TVec3f pos(mPos);
            pos.y -= 8.0f;
            if (field_0x98 != 0) {
                pos.x -= 34.0f;
            } else {
                pos.x += 34.0f;
            }
            other->fn_80182660(pos);
            if ((field_0x260 == 2 && mState == 5) || mState == 7) {
                other->field_0x254 = 0;
            } else {
                other->field_0x254 = 1;
            }
            other->field_0x98 = field_0x98;
            if (lbl_80539F98 > 0 && mPos.z < 10.0f && mState != 10 && mState != 12 && mState != 11) {
                field_0x274 = other->field_0x274;
            }
        }
    }
    switch (mState) {
    case 0:
        field_0x270 = getActorManager_801F856C()->spawnActor_801F9484('GROK', field_0x4, &mPos, 1, -1, -1);
        if (field_0x270 < 0) {
            break;
        }
        mPos.y = field_0x288.y;
        if (field_0x4 == getPlayerLayer_8023DE58(0)) {
            mPos.y = getPlayerPos_8023E724(0)->y;
        }
        field_0x26c = 0;
        field_0x234 = 0;
        fn_8022ADE4();
        mState = 1;
        // fall through
    case 1:
        box40 = JGeometry::TBox2f(-15.0f, -15.0f, 15.0f, 15.0f);
        pos1c = JGeometry::TVec2f(field_0x288.x, field_0x288.y);
        box40.i.add(pos1c);
        box40.f.add(pos1c);
        if (field_0x98 == 0) {
            mPos.x = viewBox->i.x - 300.0f;
        } else {
            mPos.x = 300.0f + viewBox->f.x;
        }
        if (field_0x260 != 1) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i) && field_0x4 == getPlayerLayer_8023DE58(i)) {
                    f32 dx = getPlayerPos_8023E724(i)->x - field_0x288.x;
                    f32 dy = getPlayerPos_8023E724(i)->y - field_0x288.y;
                    if (std::sqrtf(dx * dx + dy * dy) < 64.0f) {
                        field_0x260 = 1;
                        field_0x268 = i;
                        break;
                    }
                }
            }
        }
        if (field_0x260 == 1 && getActorManager_801F856C()->fn_801F68E0() != 0) {
            for (s32 i = 0; i < 4; i++) {
                fn_80230C6C(i);
                fn_8023B6CC(i);
            }
            mState = 2;
        }
        break;
    case 2:
        field_0x234 = 0;
        field_0x308 = 500.0f;
        if (field_0x98 == 0) {
            field_0x27c.x = 6.0f;
        } else {
            field_0x27c.x = -6.0f;
        }
        if (field_0x260 == 1) {
            mPos.y = field_0x288.y;
            if (field_0x4 == getPlayerLayer_8023DE58(field_0x268)) {
                if (field_0x268 < 0) {
                    field_0x268 = 0;
                }
                mPos.y = getPlayerPos_8023E724(field_0x268)->y;
            }
            if (field_0x98 == 0) {
                fn_80181BC8(1, 0);
            } else {
                fn_80181BC8(0, 0);
            }
        } else {
            if (field_0x24c == 0) {
                field_0x24c = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            }
            if (player >= 0) {
                mPos.y = getPlayerPos_8023E724(player)->y;
            }
        }
        field_0x240 = 30;
        field_0x25c = 0;
        field_0x238 = 10;
        mState = 3;
        // fall through
    case 3:
        if (field_0x260 == 2 && field_0x25c < 5 && mPos.x < viewBox->f.x - 96.0f && mPos.x > 80.0f + viewBox->i.x && field_0x240 == 0) {
            f32 z = 50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 300.0f;
            f32 y = mPos.y + (24.0f - 48.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            JGeometry::TVec3f spawnPos(mPos.x, y, z);
            if (field_0x98 == 0) {
                spawnPos.y += 16.0f;
            }
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('OIWA', field_0x4, &spawnPos, 1, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* rock = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (rock != NULL) {
                    rock->fn_8017EAF8(JGeometry::TVec3f(0.0f, 0.0f, -1.0f));
                    field_0x25c++;
                }
            }
            field_0x240 = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
        }
        if (field_0x260 == 1) {
            if (field_0x98 == 0) {
                fn_80181BC8(1, 0);
            } else {
                fn_80181BC8(0, 0);
            }
        }
        if (field_0x238 == 0) {
            if ((field_0x98 == 0 && mPos.x > 300.0f + viewBox->f.x) || (field_0x98 != 0 && mPos.x < viewBox->i.x - 300.0f)) {
                field_0x98 ^= 1;
                field_0x24c++;
                mState = 2;
                if (field_0x24c > 1) {
                    field_0x3c.zero();
                    field_0x27c.zero();
                    mState = 4;
                }
            }
        }
        if (field_0x234 == 0) {
            field_0x234 = 4;
            field_0x26c++;
            if (field_0x26c > 13) {
                field_0x26c = 0;
                startSound_801F0E34(0x40030, 0);
            }
        }
        break;
    case 4: {
        ActorBase_804A5770* target;
        field_0x234 = 0;
        field_0x250 = 0;
        field_0x26c = 0;
        if (field_0x260 == 1) {
            if (field_0x4 == getPlayerLayer_8023DE58(field_0x268)) {
                mPos.y = getPlayerPos_8023E724(field_0x268)->y;
            } else {
                mPos.y = field_0x288.y;
            }
        } else {
            if (field_0x98 == 0) {
                if (lbl_80539FA4 >= 0) {
                    target = getActorManager_801F856C()->findActorBySerial_801F666C(lbl_80539FA4);
                }
            } else {
                if (lbl_80539FA0 >= 0) {
                    target = getActorManager_801F856C()->findActorBySerial_801F666C(lbl_80539FA0);
                }
            }
            if (lbl_80539F98 <= 0) {
                if (player >= 0) {
                    mPos.y = getPlayerPos_8023E724(player)->y;
                }
            } else if (target != NULL) {
                mPos.y = target->mPos.y;
            }
        }
        mPos.y += 32.0f;
        if (field_0x98 == 0) {
            field_0x27c.x = 7.0f;
            mPos.x -= 100.0f;
        } else {
            field_0x27c.x = -7.0f;
            mPos.x += 100.0f;
        }
        startSound_801F0E34(0x40032, 0);
        mState = 5;
        // fall through
    }
    case 5:
        field_0x308 = 32.0f;
        if (fabs(field_0x308 - mPos.z) < 1.0f) {
            field_0x27c.zero();
            if (mEmitter318[1] == NULL) {
                mEmitter318[1] = spawnEmitter_8013CC50(field_0x4, &mPos, 0x15D, 0, 0, NULL, NULL, true, 1.0f);
            }
            mState = 6;
        } else {
            field_0x26c = 12;
        }
        break;
    case 6:
        if (lbl_80539F98 <= 0) {
            if (field_0x98 == 0) {
                field_0x27c.x = 6.0f;
            } else {
                field_0x27c.x = -6.0f;
            }
        } else {
            if (field_0x98 == 0) {
                field_0x27c.x = 4.0f;
            } else {
                field_0x27c.x = -4.0f;
            }
        }
        startSound_801F0E34(0x40031, 0);
        field_0x23c = 40;
        mState = 7;
        // fall through
    case 7:
        if (mEmitter318[1] != NULL) {
            mEmitter318[1]->setGlobalTranslation(mPos);
        }
        if (field_0x23c == 0) {
            field_0x308 = 500.0f;
            if (mEmitter318[1] != NULL) {
                mEmitter318[1]->becomeInvalidEmitter();
                mEmitter318[1] = NULL;
            }
            if (field_0x98 == 0) {
                field_0x27c.x = 7.0f;
            } else {
                field_0x27c.x = -7.0f;
            }
            if (field_0x234 == 0) {
                field_0x234 = 4;
                field_0x26c++;
                if (field_0x26c > 13) {
                    field_0x26c = 0;
                    startSound_801F0E34(0x40030, 0);
                }
            }
        }
        if (field_0x260 == 1) {
            fn_80181BC8(2, 1);
        }
        if ((field_0x98 == 0 && mPos.x > 300.0f + viewBox->f.x) || (field_0x98 != 0 && mPos.x < viewBox->i.x - 300.0f)) {
            if (mEmitter318[1] != NULL) {
                mEmitter318[1]->becomeInvalidEmitter();
                mEmitter318[1] = NULL;
            }
            field_0x3c.zero();
            field_0x27c.zero();
            field_0x24c = 1;
            if (lbl_80539F98 > 0) {
                field_0x98 ^= 1;
                if (field_0x260 != 1) {
                    field_0x264++;
                    field_0x264 &= 1;
                    if (field_0x264 != 0) {
                        field_0x3c.zero();
                        field_0x27c.zero();
                        mState = 4;
                        return;
                    }
                }
            }
            if (field_0x260 == 1) {
                gAudioMgr_8053AF80->fn_80458FF0(0x1000004, false, 0);
                field_0x260 = 2;
                getActorManager_801F856C()->fn_801F6884();
                lbl_8053AB75 = 1;
            }
            mState = 2;
        }
        break;
    case 8:
        field_0x27c.zero();
        field_0x3c.z = 10.0f;
        field_0x26c = 0;
        field_0x234 = 0;
        mState = 9;
        // fall through
    case 9:
        if (mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
        } else {
            field_0x3c.z -= 0.5f;
            if (field_0x3c.z < 0.0f && lbl_80539F98 > 0) {
                if (field_0x98 == 0) {
                    field_0x27c.x = 3.5f;
                } else {
                    field_0x27c.x = -3.5f;
                }
                addCalc2_801F71A4(&field_0x3c.x, field_0x27c.x, 0.1f, 1.0f);
            }
        }
        if (field_0x238 != 0) {
            addCalc2_801F71A4(&field_0x3c.x, 0.0f, 0.1f, 1.0f);
            addCalc2_801F71A4(&field_0x3c.y, 0.0f, 0.1f, 1.0f);
        }
        if (lbl_80539F98 > 0 && field_0x3c.x != 0.0f && field_0x274 >= 0) {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274);
            if (target != NULL) {
                if ((field_0x98 == 0 && target->field_0x90 == 3) || (field_0x98 != 0 && target->field_0x90 == 2)) {
                    field_0x23c = 0;
                    target->field_0x94 = 1;
                    mState = 10;
                    break;
                }
            }
        }
        // fall through
    case 13:
        if (field_0x234 == 0) {
            field_0x234 = 4;
            if (field_0x26c < 13) {
                field_0x26c++;
                if (mState == 9 && field_0x26c == 13 && lbl_80539F98 > 0) {
                    field_0x238 = 20;
                }
            }
        }
        if (field_0x26c >= 13) {
            if (field_0x238 == 0) {
                field_0x3c.zero();
                field_0x27c.zero();
                field_0x24c = -1;
                if (lbl_80539F98 <= 0) {
                    field_0x24c = 1;
                }
                field_0x234 = 0;
                field_0x308 = 500.0f;
                if (field_0x98 == 0) {
                    field_0x27c.x = 6.0f;
                } else {
                    field_0x27c.x = -6.0f;
                }
                field_0x240 = 200;
                field_0x25c = 0;
                field_0x238 = 10;
                mState = 3;
            } else if (lbl_80539F98 <= 0 && fn_801F2FAC()) {
                field_0x3c.zero();
                field_0x27c.zero();
                lbl_80539F9C -= 10000 - mLife;
                if (lbl_80539F9C > 0) {
                    field_0x23c = 0;
                    if (mEmitter318[0] != NULL) {
                        mEmitter318[0]->becomeInvalidEmitter();
                        mEmitter318[0] = NULL;
                    }
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x164, 0, 0, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x40033, 0);
                    mState = 8;
                } else {
                    mState = 14;
                }
            }
        }
        break;
    case 10:
        field_0x27c.zero();
        field_0x3c.zero();
        if (mEmitter318[0] != NULL) {
            mEmitter318[0]->becomeInvalidEmitter();
            mEmitter318[0] = NULL;
        }
        field_0x234 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x26c = 12;
        startSound_801F0E34(0x40034, 0);
        mState = 11;
        // fall through
    case 11:
        if (field_0x274 < 0) {
            mState = 13;
        } else {
            ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274);
            if (target == NULL) {
                mState = 13;
            } else {
                s32 dirInv = field_0x98 ^ 1;
                JGeometry::TVec3f pos(target->mPos);
                if (field_0x98 != 0) {
                    pos.x += 20.0f;
                } else {
                    pos.x -= 20.0f;
                }
                pos.y += 16.0f;
                pos.z += 10.0f;
                addCalc2_801F71A4(&mPos.x, pos.x, 1.0f, 5.0f);
                addCalc2_801F71A4(&mPos.y, pos.y, 1.0f, 5.0f);
                addCalc2_801F71A4(&mPos.z, pos.z, 1.0f, 5.0f);
                if (fabs(mPos.x - pos.x) < 2.0f && fabs(mPos.z - pos.z) < 1.0f) {
                    pos.set(target->mPos);
                    getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
                    for (s32 i = 0; i < 30; i++) {
                        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('GROK', field_0x4, &pos, 6, -1, -1);
                        if (serial >= 0) {
                            cls_804A1630* piece = static_cast<cls_804A1630*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                            if (piece != NULL) {
                                s16 angle = fn_801F6874(dirInv);
                                angle += (s32)(12288.0f - 24576.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                                f32 sinVal = JMASinShort(angle);
                                f32 cosVal = JMACosShort(angle);
                                JGeometry::TVec3f vel;
                                vel.x = sinVal * (3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f);
                                vel.y = cosVal * (3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f);
                                vel.z = 8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 2.0f;
                                piece->field_0x27c.set(vel);
                                piece->fn_8017EAF8(vel);
                            }
                        }
                    }
                    if (target->field_0x90 == 2) {
                        lbl_8053AB70 = 4;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15E, 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x15F, 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x160, 1, 2, NULL, NULL, true, 1.0f);
                    }
                    if (target->field_0x90 == 3) {
                        lbl_8053AB70 = 5;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x161, 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x162, 1, 2, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x163, 1, 2, NULL, NULL, true, 1.0f);
                    }
                    field_0x258 = lbl_80539F98;
                    startSound_801F0E34(0x30001, 0);
                    mState = 12;
                }
            }
        }
        break;
    case 12:
        if (field_0x258 != lbl_80539F98) {
            field_0x258 = lbl_80539F98;
            fn_801544D0(20);
        }
        if (lbl_8053AB70 < 0) {
            field_0x3c.zero();
            field_0x27c.zero();
            field_0x26c = 0;
            field_0x234 = 0;
            field_0x238 = 0;
            startSound_801F0E34(0x40039, 0);
            lbl_8053AB70 = 0;
            mState = 13;
        } else if (field_0x234 == 0) {
            field_0x24c++;
            field_0x24c &= 1;
            if (field_0x24c == 0) {
                field_0x26c = 12;
            } else {
                field_0x26c = 13;
            }
            field_0x234 = 8;
        }
        break;
    case 14:
        field_0x308 = 80.0f;
        field_0x26c = 0;
        field_0x234 = 0;
        field_0x238 = 0;
        field_0x24c = 0;
        field_0x234 = 4;
        field_0x314 = 1.0f;
        mState = 15;
        // fall through
    case 15:
        if (field_0x234 == 0 && field_0x26c < 13) {
            field_0x234 = 4;
            field_0x26c++;
            if (field_0x26c == 6) {
                field_0x234 = 280;
                vfunc_0x24();
            } else if (field_0x26c == 7) {
                field_0x308 = 0.0f;
                field_0x314 = 0.1f;
            }
        }
        addCalc2_801F71A4(&mPos.x, field_0x288.x, field_0x314, 10.0f);
        addCalc2_801F71A4(&mPos.y, field_0x288.y, field_0x314, 10.0f);
        addCalc2_801F71A4(&mPos.z, field_0x308, field_0x314, 10.0f);
        break;
    case 16:
        if (lbl_8053AB74 == 1) {
            mAlive = false;
            break;
        }
        if (lbl_80539F98 <= 0) {
            mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
            mBox2.set(-8.0f, -8.0f, 8.0f, 8.0f);
            field_0xb0 = 0x9000000;
            field_0x274 = -1;
        }
        if (field_0x254 == 0) {
            if (lbl_80539F98 <= 0) {
                fn_801F2B7C(field_0x114, -1, 6);
            }
        } else if (lbl_80539F98 > 0) {
            field_0x274 = fn_801F2718('GROK');
            if (field_0x274 >= 0) {
                ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274);
                if (target != NULL && target->field_0x90 != 2 && target->field_0x90 != 3) {
                    field_0x274 = -1;
                }
            }
        }
        break;
    case 17:
        if (lbl_80539F98 <= 0) {
            mAlive = false;
            break;
        }
        mPos.x = field_0x288.x + lbl_804A13E0.field_0x00[field_0x98][0];
        mPos.y = field_0x288.y + lbl_804A13E0.field_0x00[field_0x98][1];
        if (lbl_8053AB70 == (s32)field_0x90) {
            field_0x238 = 1000;
            if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
                field_0x238 = 240;
            }
            field_0xb0 = 0x9040304;
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
            field_0x26c = 0;
            field_0x234 = 0;
            field_0x24c = 0;
            field_0x254 = 0;
            field_0x250 = -1;
            mState = 18;
        }
        break;
    case 18: {
        if (field_0x238 == 0) {
            field_0x234 = 2;
            mState = 20;
            break;
        }
        if (field_0x234 == 0) {
            field_0x234 = lbl_804A13E0.field_0x40;
            field_0x26c = lbl_804A13E0.field_0x20[field_0x98][field_0x24c];
            field_0x24c++;
            field_0x24c &= 3;
        }
        box40 = mBox2;
        box40.i.add(pos1c);
        box40.f.add(pos1c);
        bool hit = false;
        if (fn_801F2DB4('HBMB', 'NULL', NULL)) {
            if (mLife != 10000) {
                hit = true;
            }
        } else if (fn_801A4F5C(mSerial, &box40, &pos1c) != 0) {
            hit = true;
        }
        if (!hit) {
            break;
        }
        field_0x3c.zero();
        s32 count = 2;
        lbl_80539F98--;
        field_0x254++;
        if (lbl_80539F98 <= 0) {
            lbl_80539F98 = 0;
            count = 4;
            startSound_801F0E34(0x40038, 0);
        } else {
            startSound_801F0E34(0x40033, 0);
        }
        for (s32 i = 0; i < count; i++) {
            JGeometry::TVec3f spawnPos(mPos);
            spawnPos.x += lbl_804A13E0.field_0x10[field_0x98][0];
            spawnPos.x += 4.0f - 8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            spawnPos.y += lbl_804A13E0.field_0x10[field_0x98][1];
            spawnPos.y += 4.0f - 8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('GROK', field_0x4, &spawnPos, 10, -1, -1);
            if (serial >= 0) {
                ActorBase_804A5770* piece = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (piece != NULL) {
                    fn_801F6874(field_0x98);
                    JGeometry::TVec3f vel;
                    vel.x = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    if (field_0x98 == 1) {
                        vel.x *= -1.0f;
                    }
                    vel.y = -(1.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    vel.z = 0.0f;
                    piece->fn_8017EAF8(vel);
                    piece->field_0x98 = field_0x98;
                }
            }
        }
        field_0x26c = 0;
        field_0x234 = 0;
        field_0x24c = 0;
        field_0x250 = -2;
        mState = 19;
        // fall through
    }
    case 19:
        if (field_0x234 == 0) {
            field_0x234 = lbl_80539FB0[0];
            field_0x26c = lbl_804A13E0.field_0x50[field_0x98][field_0x24c];
            field_0x24c++;
            field_0x24c &= 1;
            if (field_0x24c == 0) {
                if (lbl_80539F98 <= 0) {
                    field_0x234 = 100;
                    s32 serial = getActorManager_801F856C()->spawnActor_801F9484('GROK', field_0x4, &mPos, 9, -1, -1);
                    if (serial >= 0) {
                        ActorBase_804A5770* piece = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                        if (piece != NULL) {
                            piece->field_0x98 = field_0x98;
                        }
                    }
                    field_0x26c = lbl_80539FA8[field_0x98];
                    mState = 20;
                } else if (field_0x254 >= 4) {
                    field_0x234 = 2;
                    mState = 20;
                } else {
                    mState = 18;
                }
            }
        }
        break;
    case 20:
        if (field_0x234 == 1) {
            for (s32 i = 0; i < 30; i++) {
                JGeometry::TVec3f spawnPos(mPos);
                spawnPos.x += 4.0f - 8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                spawnPos.y += lbl_804A13E0.field_0x10[field_0x98][1];
                spawnPos.y += 16.0f - 32.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484('GROK', field_0x4, &spawnPos, 8, -1, -1);
                if (serial >= 0) {
                    ActorBase_804A5770* piece = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (piece != NULL) {
                        JGeometry::TVec3f vel;
                        vel.x = 1.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                        if (field_0x98 == 1) {
                            vel.x *= -1.0f;
                        }
                        vel.y = -(3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 3.0f);
                        vel.z = 0.0f;
                        piece->fn_8017EAF8(vel);
                        piece->field_0x98 = field_0x98;
                    }
                }
            }
            getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
        }
        if (field_0x234 == 0) {
            f32 offset = 80.0f;
            if (field_0x98 == 0) {
                offset = -80.0f;
            }
            addCalc2_801F71A4(&mPos.x, field_0x288.x + offset, 1.0f, 10.0f);
            if (fabs(mPos.x - (field_0x288.x + offset)) < 1.0f) {
                lbl_8053AB70 = field_0x250;
                mState = 17;
            }
        }
        break;
    case 21:
        field_0x3c.z -= 1.0f;
        if (mPos.z < 0.0f) {
            mPos.z = 0.0f;
            field_0x3c.x *= 0.5f;
            field_0x3c.y *= 0.5f;
            field_0x3c.z = 0.5f * field_0x27c.z;
            if (field_0x250 == 0) {
                field_0x250 = 1;
                field_0x238 = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
            }
        }
        if (field_0x238 != 0) {
            addCalc2_801F71A4(&field_0x310, 255.0f, 0.3f, 10.0f);
        }
        if (field_0x238 == 1) {
            mAlive = false;
        }
        break;
    case 22:
        break;
    case 23:
        if (dir >= 0 && dist <= 64.0f) {
            mPos.x = viewBox->i.x - 64.0f;
            mPos.y = getPlayerPos_8023E724(player)->y;
            field_0x98 = 0;
            field_0x27c.x = 4.0f;
            mState = 24;
        }
        break;
    case 24:
        if (field_0x234 == 0) {
            field_0x234 = 4;
            field_0x26c++;
            if (field_0x26c > 13) {
                field_0x26c = 0;
                startSound_801F0E34(0x40030, 0);
            }
        }
        if (viewBox->f.x < mPos.x) {
            mAlive = false;
        }
        break;
    case 25:
        if (field_0x90 == 9) {
            if (field_0x24c != 0) {
                if (field_0x98 == 0) {
                    mPos.x = 20.0f + field_0x288.x;
                    field_0x26c = 0x16;
                } else {
                    mPos.x = field_0x288.x - 20.0f;
                    field_0x26c = 0x17;
                }
                field_0x24c = 0;
            }
            if (field_0x234 != 0) {
                if (field_0x234 == 1) {
                    field_0x3c.x = 2.0f;
                    if (field_0x98 == 1) {
                        field_0x3c.x *= -1.0f;
                    }
                }
                break;
            }
        }
        field_0x3c.y += 0.25f;
        if (fn_801F2618(NULL, false)) {
            if (field_0x250 == 0) {
                field_0x250 = 1;
                field_0x3c.y = -(1.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            } else {
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
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 > 0) {
        field_0x248--;
    }
}

// 0x8017EF98.
// FIXME(shotgun): 92.32 %, missing/extra + reloc-name
void cls_804A1630::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f saved(mPos);
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (mState == 0 || mState == 1 || mState == 0x17) {
        return;
    }
    if (field_0x90 == 1) {
        return;
    }
    if (field_0x90 == 2 || field_0x90 == 3) {
        if (field_0x94 == 0) {
            return;
        }
        JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
        f32 x = -16.0f;
        setRotScaleMtx(mtx, rot, 1.0f);
        if (field_0x90 == 3) {
            x = 16.0f;
            setRotScaleMtx3(mtx, rot, -1.0f, 1.0f, 1.0f);
        }
        params.mMtx = mtx;
        void* res = fn_804032E8('GROC', "geki_ana.bti");
        params.field_0x1d = -2;
        params.field_0x1c = 1;
        fn_801EFD34(res, &params, -1, x, -6.0f, 0.0f);
        return;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4]) {
        u32 kind = field_0x90;
        if (kind == 8) {
            if (field_0x250 == 0 || !(field_0x24c & 2)) {
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + 0x192, 0, 2, -1, true, 0, 0);
            }
            return;
        }
        if (kind == 4 || kind == 5) {
            if (lbl_8053AB70 == kind) {
                fn_80307AC8(mSerial, field_0x4, &mPos, field_0x26c + 0x29, 0, 0, -1);
            }
            return;
        }
        if (kind == 10) {
            if (field_0x250 == 0 || !(field_0x24c & 2)) {
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + 0x192, 0, 2, -1, true, 0, 0);
            }
            return;
        }
        if (kind == 9) {
            if (field_0x250 == 0 || !(field_0x24c & 2)) {
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x26c + 0x192, 0, 2, -1, true, 0, 0);
                mPos.x = saved.x;
                mPos.y = saved.y;
                mPos.z = saved.z;
            }
            return;
        }
    }
    if (field_0x90 == 6) {
        params.field_0x1c = 1;
        params.field_0x1d = 2;
        f32 scale = field_0x30c;
        mtx[0][0] = scale;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = scale;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = scale;
        params.mMtx = mtx;
        params.field_0x10.a = field_0x310;
        drawSprite_801F06F0("enemy_gc_sob.bin", 0xB1, &params, -1, 1.0f, 1.0f);
        return;
    }
    JGeometry::TVec3f rot(0.0f, 0.0f, 0.0f);
    s32 frame = field_0x26c;
    if (lbl_80539F98 <= 0) {
        frame = field_0x26c + 0xE;
    }
    void* res;
    f32 offset;
    if (mState != 9 && mState != 15) {
        if (mState == 12) {
            if (field_0x26c == 12) {
                frame = 0x1C;
            } else if (field_0x26c == 13) {
                frame = 0x1D;
            }
        }
        res = fn_804032E8('GROC', lbl_804A1460[frame]);
        offset = lbl_804A14D8[frame];
    } else {
        res = fn_804032E8('GROC', lbl_804A1550[frame]);
        offset = lbl_804A15C0[frame];
    }
    setRotScaleMtx(mtx, rot, 1.0f);
    if (field_0x98 == 0) {
        setRotScaleMtx3(mtx, rot, -1.0f, 1.0f, 1.0f);
    }
    params.mMtx = mtx;
    params.field_0x10.r = 0;
    params.field_0x10.g = 0;
    params.field_0x10.b = 0;
    params.field_0x10.a = 100;
    mPos.z = 0.0f;
    params.field_0x1d = -1;
    params.field_0x1c = 1;
    fn_801EFD34(res, &params, -1, 0.0f, -8.0f + offset, 0.0f);
    mPos.x = saved.x;
    mPos.y = saved.y;
    mPos.z = saved.z;
    params.field_0x10.r = 0xFF;
    params.field_0x10.g = 0xFF;
    params.field_0x10.b = 0xFF;
    params.field_0x10.a = 0xFF;
    setRotScaleMtx(mtx, rot, 1.0f);
    frame = field_0x26c;
    if (lbl_80539F98 <= 0) {
        frame = field_0x26c + 0xE;
    }
    if (mState != 9 && mState != 15) {
        if (mState == 12) {
            if (field_0x26c == 12) {
                frame = 0x1C;
            } else if (field_0x26c == 13) {
                frame = 0x1D;
            }
        }
        res = fn_804032E8('GROC', lbl_804A1460[frame]);
        offset = lbl_804A14D8[frame];
    } else {
        res = fn_804032E8('GROC', lbl_804A1550[frame]);
        offset = lbl_804A15C0[frame];
    }
    params.field_0x1d = 1;
    if (mState == 15) {
        params.field_0x1d = 0;
    } else {
        params.field_0x1f = 0;
    }
    if (field_0x98 == 0) {
        setRotScaleMtx3(mtx, rot, -1.0f, 1.0f, 1.0f);
    }
    params.mMtx = mtx;
    if (lbl_80539F98 <= 0) {
        params.field_0x30 = 0x11;
    }
    fn_801EFD34(res, &params, -1, 0.0f, -8.0f + offset, 0.0f);
}

void cls_804A1630::vfunc_0x24() {
    startSound_801F0E34(0x40038, 0);
    vfunc_0x28(2, 0.0f, -24.0f);
}
