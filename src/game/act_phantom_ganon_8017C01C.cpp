#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/DrawShadow.h"
#include "game/SpriteParams.h"
#include "game/ActorId.h"
#include "game/Player.h"
#include "game/act_light_environment_control_802EF76C.h"
#include "game/act/act_sword_soldier.h"
#include "game/act/act_wizzrobe.h"
#include "game/EmitterDelete.h"

// The 'FGAN' actor's .data tables (0x804A1210): emitter ids per slot, the frame counts of
// each state, and the "FGAN" resource names / sprite offsets of each draw mode.
static u16 lbl_804A1210[8] = {0x374, 0x375, 0x378, 0x379, 0x372, 0x373, 0x376, 0x377};
static u16 lbl_804A1220[8] = {0x368, 0x369, 0x36C, 0x36D, 0x366, 0x367, 0x36A, 0x36B};
static s32 lbl_804A1230[6] = {4, 4, 4, 4, 2, 4};
static s32 lbl_804A1248[5] = {2, 2, 2, 2, 5};
static s32 lbl_804A125C[8] = {4, 4, 4, 4, 4, 4, 4, 4};
static s32 lbl_804A127C[5] = {4, 4, 4, 8, 4};
static const char* lbl_804A1290[6] = {"dawn_00.bti", "dawn_04.bti", "dawn_08.bti", "dawn_12.bti", "dawn_16.bti", "dawn_18.bti"};
static f32 lbl_804A12A8[5][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}, {-21.0f, 0.0f}, {21.0f, 0.0f}, {21.0f, 0.0f}};
static const char* lbl_804A12D0[5] = {"kenhuri_05.bti", "kenhuri_07.bti", "kenhuri_09_l.bti", "kenhuri_11_r.bti", "kenhuri_13_r.bti"};
static const char* lbl_804A12E4[8] = {"wait_00_u.bti", "wait_04_u.bti", "wait_08_u.bti", "wait_12_u.bti", "wait_16_u.bti", "wait_20_u.bti", "wait_24_u.bti", "wait_28_u.bti"};
static const char* lbl_804A1304[5] = {"yarare_02.bti", "yarare_06.bti", "yarare_10.bti", "yarare_14.bti", "yarare_22.bti"};
u8 lbl_80539F88[8] = {2, 6, 1, 5, 11, 0, 0, 0};
u16 lbl_80539F90[4] = {0x37D, 0x37F, 0x37C, 0x37E}; // emitter id per player

// Not auto-inlined: update (0x8017C40C) calls every helper from fn_8017ED08 down to
// invalidateEmitter_8017EAB4 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// Sets the map's field_0x216 flag (gGameRoot2_8053AB10->0x20, TileMap_804AA858; 9 units
// set it to 1). Takes no arguments; the caller is the fn_8017C4C8 branch of
// the actor update.
void fn_8017ED08() {
    gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
}

// Out-of-line copy of JUtility::TColor::set(r, g, b, a); update() passes the
// result (r3) straight on to fn_8013A884.
JUtility::TColor* fn_8017ECF4(JUtility::TColor* color, u8 r, u8 g, u8 b, u8 a) {
    color->set(r, g, b, a);
    return color;
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8017ECE8(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

u8 fn_8017ECC0() {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x179 == 0) {
        camera->field_0x178 = result = 1;
    }
    return result;
}

void pool_8053DDC8(Camera_804AA9E4 *camera);

// Fixes the camera on pos (both corners of its box) with `value`, once.
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->mBlendBoxOn_0x840 == 0) {
        camera->mBlendBoxOn_0x840 = result = 1;
        camera->mBlendBox_0x844.i.x = camera->mBlendBox_0x844.f.x = pos->x;
        camera->mBlendBox_0x844.i.y = camera->mBlendBox_0x844.f.y = pos->y;
        camera->mBlendWeight_0x854 = value;
        pool_8053DDC8(camera);
        camera->mMinHalfWidth_0x1d8 = -1.0f;
    }
    return result;
}

// Out-of-line copy of TBox2<f32>::set(x0, y0, x1, y1).
// update() copies from its result (mr r4, r3).
JGeometry::TBox2f* fn_8017EC5C(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box->set(x0, y0, x1, y1);
    return box;
}

// Out-of-line copy of TBox2<f32>'s copy (word by word).
void fn_8017EC38(JGeometry::TBox2f* dst, const JGeometry::TBox2f* src) {
    *dst = *src;
}

// Whether both corners of box are on screen. corner2 is declared before corner and
// camera before result only to steer the stack slots and register order to the original's.
bool fn_8017EB94(const JGeometry::TBox2f* box) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    bool result = false;
    JGeometry::TVec3f corner2;
    JGeometry::TVec3f corner(box->i.x, box->i.y, 0.0f);
    if (camera->isOnScreen_8030C2A8(&corner)) {
        corner2.set(box->f.x, box->f.y, 0.0f);
        if (camera->isOnScreen_8030C2A8(&corner2)) {
            result = true;
        }
    }
    return result;
}

// FIXME: 83.75 %, missing/extra; behaviour clean.
// The original keeps the constant 1 (stb 0x7f4) in r4 and copies it to the result
// (mr r5, r4), and stores the 0 for mOverrideBlend_0x7f6 from the result's zero register; ours loads
// separate constants.
// Starts a scripted camera move (mode 4) to pos unless one is already set.
u8 fn_8017EB44(const JGeometry::TVec3f* pos) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->mShowWholeArea_0x7f5 == 0) {
        camera->mFocusOverride_0x7f4 = 1;
        camera->mFocusMode_0x7f8 = 4;
        camera->mFocusPos_0x7fc.set(*pos);
        camera->mOverrideBlend_0x7f6 = 0;
        result = 1;
    }
    return result;
}

// Sets the sub-object's field_0x284 flag and its field_0x28c value, once.
bool fn_8017EB1C(f32 value) {
    cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
    if (sub->field_0x284 == 0) {
        sub->field_0x284 = 1;
        sub->field_0x28c = value;
    }
    return false;
}

// Out-of-line copy of TVec3<f32>::set(x, y, z).
// update() copies from its result (mr r4, r3).
JGeometry::TVec3f* fn_8017EB0C(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
    return v;
}

void ActorBase_804A5770::setVel_8017EAF8(const JGeometry::TVec3f& v) {
    mVel_0x3c = v;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void setVec3_8017EADC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of JPABaseEmitter::setGlobalEnvColor(r, g, b).
void fn_8017EACC(JPABaseEmitter* emitter, u8 r, u8 g, u8 b) {
    emitter->setGlobalEnvColor(r, g, b);
}

// Out-of-line copy of JPABaseEmitter::becomeInvalidEmitter().
void invalidateEmitter_8017EAB4(JPABaseEmitter* emitter) {
    emitter->becomeInvalidEmitter();
}
#pragma auto_inline reset

// FIXME(shotgun): 88.16 %, missing/extra + reloc-name; behaviour near.
// The original's emitter loop keeps the id table base and a byte offset in separate registers
// (the offset shares the zero constant's register); ours folds them into one pointer.
void PhantomGanon_804A1318::init() {
    field_0x94 = (mSpawnParam_0x90 >> 4) & 0xF;
    mSpawnParam_0x90 &= 0xF;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x268 = 0;
    field_0x196 = 1;
    field_0x294.z = 0.0f;
    field_0x294.y = 0.0f;
    field_0x294.x = 0.0f;
    mPos.z = 1.0f;
    field_0x280 = 2.0f;
    field_0x28c = 0.0f;
    for (int i = 0; i < 4; i++) {
        mEmitters2bc[i] = NULL;
        mEmitters2cc[i] = NULL;
    }
    mEmitter2dc = NULL;
    field_0x2bb = lbl_80539F88[0];
    field_0x274 = -1;
    field_0x27c = 2;
    field_0x26c = 0xD2;
    field_0x270 = 0;
    mLife = 10000;
    field_0x2b8 = 0;
    field_0x290 = 255.0f;
    field_0x284 = 0.0f;
    field_0x288 = 0.0f;
    field_0x2ac.x = 1.0f;
    field_0x2ac.y = 1.0f;
    field_0x2ac.z = 1.0f;
    field_0x278 = 0;
    field_0x2a0.set(mPos);
    field_0x2a0.y += 16.0f;
    switch (mSpawnParam_0x90) {
    case 0:
        field_0x2ac.z = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x2ac.x = 0.0f;
        field_0x290 = 0.0f;
        getGameView_802D6A9C()->fn_802D7440(false);
        // fall through
    case 1:
    default:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        mMapBox_0x80.i.x = -8.0f;
        mMapBox_0x80.i.y = -8.0f;
        mMapBox_0x80.f.x = 8.0f;
        mMapBox_0x80.f.y = 8.0f;
        mParamAtcp_0x114 = 2;
        field_0x2ba = 1;
        mState = 0;
        break;
    case 10:
        for (int i = 0; i < 4; i++) {
            mEmitters2bc[i] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1210[i * 2], 1, 2, NULL, NULL, true, 1.0f);
            mEmitters2cc[i] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1210[i * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
            if (i != 0) {
                if (mEmitters2bc[i] != NULL) {
                    mEmitters2bc[i]->setStatus(4);
                }
                if (mEmitters2cc[i] != NULL) {
                    mEmitters2cc[i]->setStatus(4);
                }
            }
        }
        mParamAtcn_0x118 = 4;
        field_0x2ac.z = 0.0f;
        field_0x2ac.y = 0.0f;
        field_0x2ac.x = 0.0f;
        field_0x2ba = 2;
        mBox.i.x = -7.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 7.0f;
        mBox.f.y = 7.0f;
        mMapBox_0x80.i.x = -2.0f;
        mMapBox_0x80.i.y = -2.0f;
        mMapBox_0x80.f.x = 2.0f;
        mMapBox_0x80.f.y = 2.0f;
        mState = 0x1A;
        break;
    case 11:
        field_0x2bb = lbl_80539F88[4];
        field_0x94 = 0;
        mParamAtcn_0x118 = 2;
        field_0x234 = 30;
        field_0x2ba = 2;
        mBox.i.x = -2.0f;
        mBox.i.y = -2.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 2.0f;
        mMapBox_0x80.i.x = -2.0f;
        mMapBox_0x80.i.y = -2.0f;
        mMapBox_0x80.f.x = 2.0f;
        mMapBox_0x80.f.y = 2.0f;
        mState = 0x1B;
        break;
    }
}

// Pool helper (R2: one-statement body): a plain function its caller inlined and whose own copy
// the linker dropped; it stands where the original's .sdata2 order puts it, its literal at 0x8053DDC8 (tools/pool.py).
void pool_8053DDC8(Camera_804AA9E4 *camera) {
    camera->mMargin_0x1d4 = -1.0f;
}

PhantomGanon_804A1318::~PhantomGanon_804A1318() {
    s32 serial;
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2bc[i]);
        releaseEmitter_801EE434(mEmitters2cc[i]);
    }
    releaseEmitter_801EE434(mEmitter2dc);
    if (mSpawnParam_0x90 == 0) {
        gGameRoot2_8053AB10->field_0x2c->field_0x4.set(0xFF, 0xFF, 0xFF, 0);
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    if (mSpawnParam_0x90 == 10) {
        serial = field_0x274;
        if (serial >= 0) {
            PhantomGanon_804A1318* other = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (other != NULL && other->mAlive == true) {
                other->field_0x28c = 3.0f;
                other->field_0x278 = 0;
                field_0x274 = -1;
            }
        }
    }
}

// The 'FGAN' actor's update (vtable slot 0x14).
// FIXME: 98.31 %, missing/extra; behaviour near.
void PhantomGanon_804A1318::update() {
    // The original calls the TBox2 copy ctor out of line here (0x801546F4); ours inlines it.
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f offset;
    fn_8017ECE8(&offset, field_0x2a0.x, field_0x2a0.y);
    s32 dir = -1;
    u16 angle = 0;
    JGeometry::TVec3f vel;
    zeroVec_801546BC(&vel);
    if (mSpawnParam_0x90 == 0) {
        fn_8017ECC0();
    }
    if (mSpawnParam_0x90 == 0) {
        fn_8016A3AC()->field_0x1c6 = 1;
    }
    if (updateBossDeath_0x2C(8.0f, -16.0f, 50.0f)) {
        fn_8017EC70(&mPos, 1.0f);
        fn_8017ED08();
        return;
    }
    if (mSpawnParam_0x90 == 0) {
        mLife = 10000;
    }
    JGeometry::TVec3f pos;
    setVec3_8017EADC(&pos, &mPos);
    pos.y -= 16.0f;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
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
    updateHitReaction_801F3D94(0);
    if (mState == 0x14 && (mSpawnParam_0x90 == 0 || mSpawnParam_0x90 == 1)) {
        field_0x250++;
        if (field_0x250 > 400) {
            field_0x268 = 5;
            field_0x280 = 0.0f;
            field_0x270 = 0;
            field_0x234 = lbl_804A1230[field_0x268];
            mState = 0x15;
            return;
        }
        if (checkPlayerAttacks_801F2FAC()) {
            if (mPos.z == 0.0f && field_0x254 != 0) {
                field_0x268 = 0;
                field_0x254 = 0;
                field_0x270 += 10000 - mLife;
                field_0x26c -= 10000 - mLife;
                if (field_0x270 > 75) {
                    field_0x250 = 400;
                }
                startSound_801F0E34(0x40046, 0);
                if (field_0x26c <= 0) {
                    mState = 0x16;
                }
            }
            zeroVec_801546BC(&mVel_0x3c);
        }
    }
    if (mSpawnParam_0x90 == 0 || mSpawnParam_0x90 == 1) {
        addCalc2_801F71A4(&mVel_0x3c.x, field_0x294.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&mVel_0x3c.y, field_0x294.y, 0.3f, 1.0f);
        addCalc2_801F71A4(&mPos.z, field_0x288, 0.3f, field_0x280);
    }
    fn_801545F0(&mPos, &mVel_0x3c);
    if (mEmitter2dc != NULL) {
        setEmitterPos_8013CB44(mEmitter2dc, &mPos, 0, 1, true);
        fn_8017EACC(mEmitter2dc, 0x9F, 0x7F, 0xFF);
    }
    switch (mState) {
    case 0: {
        JGeometry::TBox2f area;
        fn_8017EC38(&box, fn_8017EC5C(&area, -30.0f, -30.0f, 30.0f, 30.0f));
        addVec2_80154558(&box.i, &offset);
        addVec2_80154558(&box.f, &offset);
        if (field_0x250 == 0) {
            if (!fn_8017EB94(&box)) {
                break;
            }
            gAudioMgr_8053AF80->stopBgm_80458F9C(60);
            field_0x250 = 1;
        }
        if (getActorManager_801F856C()->fn_801F68E0() == 0) {
            break;
        }
        fn_8022ADE4();
        JUtility::TColor color;
        fn_8013A884(fn_8017ECF4(&color, 0xFF, 0xFF, 0xFF, 0));
        field_0x250 = 0;
        field_0x254 = 0;
        mState = 1;
    }
    case 1:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        field_0x254++;
        if (field_0x254 < 100) {
            break;
        }
        if (field_0x254 == 100) {
            startSound_801F0E34(0x40074, 0);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x362, 0, 4, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x363, 0, 4, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x364, 0, 4, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x365, 0, 4, NULL, NULL, true, 1.0f);
        }
        field_0x254 = 101;
        field_0x250++;
        if (field_0x250 < 300) {
            break;
        }
        startSound_801F0E34(0x40042, 0);
        field_0x27c = 2;
        field_0x250 = 0;
        field_0x254 = 0;
        field_0x2ac.y = 5.0f;
        field_0x2ac.x = 0.0f;
        mState = 2;
    case 2:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x268++;
            if (field_0x268 > 7) {
                field_0x268 = 0;
            }
            field_0x234 = lbl_804A125C[field_0x268];
        }
        addCalc2_801F71A4(&field_0x290, 255.0f, 0.5f, 5.0f);
        addCalc2_801F71A4(&field_0x2ac.y, 1.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&field_0x2ac.x, 1.0f, 0.3f, 0.1f);
        if (field_0x2ac.x >= 0.99f && field_0x290 >= 254.0f) {
            JGeometry::TVec3f one;
            setVec3_8017EADC(&field_0x2ac, fn_8017EB0C(&one, 1.0f, 1.0f, 1.0f));
            field_0x290 = 255.0f;
            field_0x238 = 100;
            mState = 3;
        }
        break;
    case 3:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x268++;
            if (field_0x268 > 7) {
                field_0x268 = 0;
            }
            field_0x234 = lbl_804A125C[field_0x268];
        }
        if (field_0x238 == 0) {
            field_0x27c = 1;
            startSound_801F0E34(0x40043, 0);
            field_0x268 = 0;
            mState = 4;
        }
        break;
    case 4:
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        if (field_0x234 == 0) {
            field_0x234 = lbl_804A1248[field_0x268];
            field_0x27c = 1;
            field_0x268++;
            if (field_0x268 == 4) {
                field_0x234 = 50;
                mState = 5;
                field_0x250 = 0xFF;
                field_0x254 = 0xFF;
            }
        }
        break;
    case 5: {
        fn_8017EB44(&mPos);
        fn_8017EB1C(0.05f);
        field_0x250 -= 2;
        if (field_0x250 < 0x9F) {
            field_0x250 = 0x9F;
        }
        field_0x254 -= 3;
        if (field_0x254 < 0x7F) {
            field_0x254 = 0x7F;
        }
        JUtility::TColor fade;
        fn_8013A884(fn_8017ECF4(&fade, field_0x250, field_0x254, 0xFF, 0));
        if (field_0x234 != 0) {
            break;
        }
        field_0x268--;
        field_0x234 = lbl_804A1248[field_0x268];
        if (field_0x268 >= 0) {
            break;
        }
        field_0x250 = 0;
        field_0x254 = 0;
        JUtility::TColor tint;
        fn_8013A884(fn_8017ECF4(&tint, 0x9F, 0x7F, 0xFF, 0));
        field_0x268 = 0;
        gAudioMgr_8053AF80->startBgm_80458FF0(0x1000004, false, 0);
        getActorManager_801F856C()->fn_801F6884();
        mState = 6;
    }
    case 6:
        field_0x238 = 200.0f + 100.0f * randFloat_80154744();
        field_0x248 = 50.0f + 50.0f * randFloat_80154744();
        if (mEmitter2dc == NULL) {
            mEmitter2dc = spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x380, 0, 0, NULL, NULL, true, 1.0f);
            fn_8017EACC(mEmitter2dc, 0x9F, 0x7F, 0xFF);
        }
        field_0x280 = 2.0f;
        field_0x268 = 0;
        field_0x234 = lbl_804A125C[field_0x268];
        field_0x28c = 3.0f;
        field_0x278 = 0;
        field_0x27c = 2;
        mState = 7;
    case 7: {
        addCalc2_801F71A4(&field_0x290, 255.0f, 1.0f, 10.0f);
        if (field_0x234 == 0) {
            field_0x268++;
            if (field_0x268 > 7) {
                field_0x268 = 0;
            }
            field_0x234 = lbl_804A125C[field_0x268];
        }
        f32 dx = field_0x2a0.x - mPos.x;
        f32 dy = field_0x2a0.y - mPos.y;
        if (mPos.z >= 40.0f) {
            u16 heading = 8192.0f - 16384.0f * randFloat_80154744();
            if (field_0x250 != 0) {
                heading += (u16)atan2Short_8015465C(dx, dy);
            }
            if (field_0x23c == 0) {
                field_0x294.x = 2.0f * sinShort_801544FC(heading);
                field_0x294.y = 2.0f * cosShort_801544E4(heading);
                field_0x23c = 50.0f + 50.0f * randFloat_80154744();
            }
        }
        if (sqrtf_80154378(dx * dx + dy * dy) > 64.0f) {
            if (field_0x250 == 0) {
                zeroVec_801546BC(&field_0x294);
                field_0x250 = 1;
            }
        } else if (gGameRoot_8053AAF8->getPlayerCount_80138A30() <= 1) {
            field_0x250 = 0;
            if (field_0x248 == 0) {
                findTargetPlayer_801EEC98(mSerial, &angle, &dir, NULL, NULL, 0, 1);
                if (dir >= 0) {
                    mState = 8;
                    break;
                }
            }
        }
        if (field_0x238 == 0) {
            mState = 11;
        }
        break;
    }
    case 8:
        zeroVec_801546BC(&field_0x294);
        zeroVec_801546BC(&mVel_0x3c);
        field_0x268 = 0;
        field_0x234 += 30;
        setVec3_8017EADC(&pos, &mPos);
        pos.x -= 20.0f;
        pos.y -= 70.0f;
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0xCB, 1, 5, NULL, NULL, true, 2.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0xCC, 1, 5, NULL, NULL, true, 2.0f);
        field_0x25c = 0;
        mState = 9;
    case 9:
        if (field_0x234 == 0) {
            field_0x27c = 1;
            setBox_80154590(&mBox, -16.0f, -8.0f, 64.0f, 56.0f);
            if (field_0x268 == 0) {
                startSound_801F0E34(0x40043, 0);
                if (field_0x264 == 0) {
                    findTargetPlayer_801EEC98(mSerial, &angle, &dir, NULL, NULL, 0, 1);
                } else {
                    f32 dist;
                    findTargetPlayer_801EEC98(mSerial, &angle, &dir, &dist, NULL, 0, 0);
                }
                field_0x264 = 0;
                if (dir < 0) {
                    mState = 6;
                    break;
                }
                mVel_0x3c.x = 13.0f * sinShort_801544FC(angle);
                mVel_0x3c.y = 13.0f * cosShort_801544E4(angle);
            }
            field_0x268++;
            field_0x234 = lbl_804A1248[field_0x268];
            if (field_0x268 == 4) {
                field_0x234 = 50;
                mState = 10;
            }
        }
        if (field_0x27c == 1) {
            setVec3_8017EADC(&pos, &mPos);
            mPos.y -= mPos.z;
            mPos.z = 0.0f;
            damagePlayersInBox_801F2B7C(mParamAtcp_0x114, -1, 6);
            setVec3_8017EADC(&mPos, &pos);
        }
        break;
    case 10:
        if (field_0x234 != 0) {
            if (isOutsideView_801F6758(48.0f, &mPos, mLayer_0x4)) {
                zeroVec_801546BC(&field_0x294);
                zeroVec_801546BC(&mVel_0x3c);
            }
            break;
        }
        if (FABS(mVel_0x3c.x - field_0x294.x) > 0.1f) {
            break;
        }
        if (FABS(mVel_0x3c.y - field_0x294.y) > 0.1f) {
            break;
        }
        field_0x268--;
        if (field_0x268 >= 0) {
            field_0x234 = lbl_804A1248[field_0x268];
            break;
        }
        field_0x268 = 0;
        field_0x248 = 50.0f + 50.0f * randFloat_80154744();
        setBox_80154590(&mBox, -16.0f, -8.0f, 16.0f, 40.0f);
        field_0x268 = 0;
        field_0x234 = lbl_804A125C[field_0x268];
        field_0x28c = 3.0f;
        field_0x278 = 0;
        field_0x250 = 0;
        field_0x23c = 0;
        field_0x27c = 2;
        mState = 7;
        break;
    case 11: {
        u16 heading = 32768.0f + (4096.0f - 8192.0f * randFloat_80154744());
        field_0x294.x = 4.0f * sinShort_801544FC(heading);
        field_0x294.y = 4.0f * cosShort_801544E4(heading);
        mState = 12;
    }
    case 12:
        if (field_0x2a0.y - 60.0f < mPos.y) {
            break;
        }
        field_0x294.x = 0.0f;
        field_0x294.y = 0.0f;
        zeroVec_801546BC(&mVel_0x3c);
        field_0x268 = 0;
        field_0x2b8 = 0;
        mState = 13;
    case 13: {
        setVec3_8017EADC(&pos, &mPos);
        pos.x -= 20.0f;
        pos.y -= 110.0f;
        pos.z = 0.0f;
        field_0x274 = getActorManager_801F856C()->spawnActor_801F9484('FGAN', mLayer_0x4, &pos, 10, -1, -1);
        field_0x27c = 2;
        if (field_0x274 < 0) {
            break;
        }
        field_0x238 = 40;
        PhantomGanon_804A1318* child = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274));
        if (child == NULL) {
            break;
        }
        field_0x258 = 0;
        startSound_801F0E34(0x3002C, 0);
        child->field_0x274 = mSerial;
        mState = 14;
        break;
    }
    case 14: {
        if (field_0x238 != 0) {
            break;
        }
        field_0x250 = 0;
        field_0x268 = 0;
        field_0x234 = lbl_804A1248[field_0x268];
        s32 serial = field_0x274;
        if (serial >= 0) {
            PhantomGanon_804A1318* child = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            field_0x27c = 1;
            startSound_801F0E34(0x40043, 0);
            if (child != NULL) {
                JGeometry::TVec3f savedPos = mPos;
                setVec3_8017EADC(&mPos, &child->mPos);
                findTargetPlayer_801EEC98(mSerial, &angle, &dir, NULL, NULL, 0, 2);
                setVec3_8017EADC(&mPos, &savedPos);
                if (gGameRoot_8053AAF8->getPlayerCount_80138A30() <= 1) {
                    if (field_0x258 == 0) {
                        angle += field_0x2b8;
                    } else {
                        angle -= field_0x2b8;
                    }
                }
                field_0x258++;
                field_0x258 &= 1;
                field_0x2b8 = 0x800;
                vel.x = field_0x28c * sinShort_801544FC(angle);
                vel.y = field_0x28c * cosShort_801544E4(angle);
                child->field_0x2bb = lbl_80539F88[0];
                child->setVel_8017EAF8(vel);
                child->mDir_0x98 = angleToDir_801F681C(angle);
            } else {
                field_0x274 = -1;
            }
        }
        mState = 15;
        break;
    }
    case 15:
        if (field_0x268 < 4 && field_0x234 == 0) {
            field_0x234 = lbl_804A1248[field_0x268];
            field_0x268++;
            if (field_0x268 == 4) {
                field_0x234 = 20;
                mState = 16;
            }
        }
        break;
    case 16:
        if (field_0x234 != 0) {
            break;
        }
        mState = 17;
    case 17: {
        f32 dist;
        findTargetPlayer_801EEC98(mSerial, &angle, &dir, &dist, NULL, 0, 0);
        if (dir >= 0 && dist < 50.0f) {
            field_0x264 = 1;
            mState = 8;
            break;
        }
        s32 serial = field_0x274;
        if (serial >= 0) {
            if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
                field_0x274 = -1;
                mState = 6;
                break;
            }
        } else {
            mState = 6;
            break;
        }
        if (field_0x234 != 0) {
            break;
        }
        if (field_0x27c == 1) {
            field_0x268--;
            if (field_0x268 < 0) {
                field_0x268 = 0;
                field_0x27c = 2;
            }
            field_0x234 = lbl_804A1248[field_0x268];
        } else {
            field_0x268++;
            if (field_0x268 > 7) {
                field_0x268 = 0;
            }
            field_0x234 = lbl_804A125C[field_0x268];
        }
        break;
    }
    case 18:
        field_0x268 = 0;
        field_0x234 = lbl_804A127C[field_0x268];
        setVec3_8017EADC(&pos, &mPos);
        if (mEmitter2dc != NULL) {
            invalidateEmitter_8017EAB4(mEmitter2dc);
            mEmitter2dc = NULL;
        }
        if (getActorManager_801F856C()->fn_801F68E0() == 0) {
            break;
        }
        if (field_0x260 < 2) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, mLayer_0x4, &mPos, 99, -1, -1);
            field_0x260++;
        }
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x370, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &pos, 0x371, 0, 1, NULL, NULL, true, 1.0f);
        field_0x234 = 10;
        field_0x27c = 3;
        mState = 19;
    case 19:
        setVec3_8017EADC(&pos, &mPos);
        pos.y -= 16.0f;
        fn_8017EC70(&pos, 1.0f);
        if (field_0x234 != 0) {
            break;
        }
        if (field_0x268 < 4) {
            field_0x268++;
            field_0x234 = lbl_804A127C[field_0x268];
            break;
        }
        field_0x288 = 0.0f;
        field_0x268 = 0;
        field_0x27c = 0;
        field_0x234 = lbl_804A1230[field_0x268];
        field_0x280 = 3.0f;
        field_0x250 = 0;
        field_0x254 = 0;
        getActorManager_801F856C()->fn_801F6884();
        field_0xb8 = 1;
        mState = 20;
        break;
    case 20:
        if (mPos.z < 0.1f) {
            mPos.z = 0.0f;
        }
        if (field_0x254 == 0) {
            if (field_0x268 < 5) {
                if (field_0x234 != 0) {
                    break;
                }
                field_0x268++;
                field_0x234 = lbl_804A1230[field_0x268];
                break;
            }
            field_0x254 = 1;
            break;
        }
        if ((field_0x250 & 0x1F) == 0) {
            startSound_801F0E34(0x40045, 0);
        }
        if (field_0x234 != 0) {
            break;
        }
        field_0x268 ^= 1;
        if (field_0x268 == 4) {
            field_0x234 = 4;
        } else {
            field_0x234 = 8;
        }
        break;
    case 21:
        if (field_0x234 == 0) {
            field_0x268--;
            field_0x234 = lbl_804A1230[field_0x268];
            field_0x280 = 1.0f;
        }
        if (field_0x268 > 0) {
            break;
        }
        field_0x250 = 0;
        field_0xb8 = 0;
        mState = 6;
        break;
    case 26: {
        int i;
        for (i = 0; i < 4; i++) {
            if (i != field_0x24c) {
                if (mEmitters2bc[i] != NULL) {
                    mEmitters2bc[i]->setStatus(4);
                }
                if (mEmitters2cc[i] != NULL) {
                    mEmitters2cc[i]->setStatus(4);
                }
            } else {
                if (mEmitters2bc[i] != NULL) {
                    mEmitters2bc[i]->clearStatus(4);
                }
                if (mEmitters2cc[i] != NULL) {
                    mEmitters2cc[i]->clearStatus(4);
                }
            }
        }
        if (mVel_0x3c.x == 0.0f && mVel_0x3c.y == 0.0f) {
            break;
        }
        for (i = 0; i < 4; i++) {
            if (mEmitters2bc[i] != NULL) {
                setEmitterPos_8013CB44(mEmitters2bc[i], &mPos, 1, 2, true);
            }
            if (mEmitters2cc[i] != NULL) {
                setEmitterPos_8013CB44(mEmitters2cc[i], &mPos, 1, 2, true);
            }
        }
        if (field_0x238 != 0) {
            break;
        }
        if (field_0x254 <= 50) {
            if (field_0x254 == 0) {
                startSound_801F0E34(0x3002B, 0);
            }
            field_0x254++;
        }
        if (field_0x254 > 50 && isOutsideView_801F6758(16.0f, &mPos, mLayer_0x4)) {
            u16 spread = 0;
            for (i = 0; i < 4; i++) {
                deleteEmitter(&mEmitters2bc[i]);
                deleteEmitter(&mEmitters2cc[i]);
            }
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1220[field_0x24c * 2], 1, 2, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1220[field_0x24c * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
            findTargetPlayer_801EEC98(mSerial, &spread, NULL, NULL, NULL, 0, 1);
            spread += 0xD000;
            for (i = 0; i < 7; i++) {
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484('FGAN', mLayer_0x4, &mPos, 11, -1, -1);
                if (serial != 0) {
                    startSound_801F0E34(0x30037, 0);
                    ActorBase_804A5770* shard = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (shard != NULL) {
                        shard->field_0x94 = field_0x24c;
                        vel.x = 5.0f * sinShort_801544FC(spread);
                        vel.y = 5.0f * cosShort_801544E4(spread);
                        shard->setVel_8017EAF8(vel);
                    }
                }
                spread += 0x1000;
            }
            s32 serial = field_0x274;
            if (serial >= 0) {
                PhantomGanon_804A1318* parent = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (parent != NULL) {
                    parent->field_0x28c = 3.0f;
                    parent->field_0x278 = 0;
                    field_0x274 = -1;
                }
            }
            mAlive = false;
            break;
        }
        f32 dist;
        findTargetPlayer_801EEC98(mSerial, NULL, &dir, &dist, NULL, field_0x24c, 2);
        s32 player = findOverlappingPlayer_801F285C(NULL, NULL, true, -1);
        if (player >= 0 && (field_0x24c == player || field_0x24c == getPlayerLeader_8022F3DC(player))) {
            PhantomGanon_804A1318* parent = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274));
            if (parent != NULL) {
                field_0x24c++;
                field_0x250++;
                parent->field_0x278++;
                if (parent->field_0x278 > 4) {
                    parent->field_0x278 = 4;
                }
                if (parent->field_0x28c < 5.0f) {
                    mHitTimer_0xd8 = 0;
                    fn_8023B6CC(player);
                    fn_80230FD0(player, 0x20038);
                    if (field_0x24c >= gGameRoot_8053AAF8->getPlayerCount_80138A30()) {
                        f32 dx = parent->mPos.x - mPos.x;
                        f32 dy = parent->mPos.y - (8.0f + parent->mPos.z) - mPos.y;
                        field_0x24c = gGameRoot_8053AAF8->getPlayerCount_80138A30() - 1;
                        angle = atan2Short_8015465C(dx, dy);
                        mVel_0x3c.x = parent->field_0x28c * sinShort_801544FC(angle);
                        mVel_0x3c.y = parent->field_0x28c * cosShort_801544E4(angle);
                        parent->field_0x28c = parent->field_0x28c + 0.26f * field_0x250;
                        if (parent->field_0x28c > 5.0f) {
                            parent->field_0x28c = 5.0f;
                        }
                        spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_80539F90[field_0x24c], 1, 2, NULL, NULL, true, 1.0f);
                        field_0x2bb = lbl_80539F88[4];
                        field_0x238 = 30;
                        field_0x23c = 100;
                        break;
                    }
                    if (isPlayerActive_8023F264(field_0x24c) && mLayer_0x4 == getPlayerLayer_8023DE58(field_0x24c) &&
                        !fn_80230188(field_0x24c)) {
                        findTargetPlayer_801EEC98(mSerial, &angle, NULL, NULL, NULL, field_0x24c, 2);
                        field_0x2bb = lbl_80539F88[field_0x24c];
                        field_0x258++;
                        field_0x258 &= 1;
                        field_0x2b8 = 0x800;
                        spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_80539F90[field_0x24c], 1, 2, NULL, NULL, true, 1.0f);
                        mVel_0x3c.x = parent->field_0x28c * sinShort_801544FC(angle);
                        mVel_0x3c.y = parent->field_0x28c * cosShort_801544E4(angle);
                        field_0x238 = 30;
                        field_0x23c = 50;
                        break;
                    }
                }
            }
        }
        s32 hit = false;
        if (field_0x23c == 0 && damagePlayersInBox_801F2B7C(-100, -1, 1)) {
            startSound_801F0E34(0x40042, 0);
            startSound_801F0E34(0x30037, 0);
            hit = true;
        }
        if (!hit && mLife != 0) {
            break;
        }
        for (i = 0; i < 4; i++) {
            deleteEmitter(&mEmitters2bc[i]);
            deleteEmitter(&mEmitters2cc[i]);
        }
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1220[field_0x24c * 2], 1, 2, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1220[field_0x24c * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
        PhantomGanon_804A1318* parent = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x274));
        if (parent != NULL && parent->mAlive == true) {
            parent->field_0x28c = 3.0f;
            field_0x274 = -1;
        }
        mAlive = false;
        break;
    }
    case 27:
        field_0x94 &= 3;
        mEmitters2bc[0] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1210[field_0x94 * 2], 1, 2, NULL, NULL, true, 0.5f);
        mEmitters2cc[0] = spawnEmitter_8013CC50(mLayer_0x4, &mPos, lbl_804A1210[field_0x94 * 2 + 1], 1, 2, NULL, NULL, true, 0.5f);
        mState = 28;
    case 28:
        if (mEmitters2bc[0] != NULL) {
            setEmitterPos_8013CB44(mEmitters2bc[0], &mPos, 1, 2, true);
            mEmitters2bc[0]->setGlobalAlpha(field_0x290);
        }
        if (mEmitters2cc[0] != NULL) {
            setEmitterPos_8013CB44(mEmitters2cc[0], &mPos, 1, 2, true);
            mEmitters2cc[0]->setGlobalAlpha(field_0x290);
        }
        if (damagePlayersInBox_801F2B7C(-100, -1, 1)) {
            deleteEmitter(&mEmitters2bc[0]);
            deleteEmitter(&mEmitters2cc[0]);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x36E, 1, 2, NULL, NULL, true, 0.5f);
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x36F, 1, 2, NULL, NULL, true, 0.5f);
            startSound_801F0E34(0x30037, 0);
            mAlive = false;
            break;
        }
        if (field_0x234 != 0) {
            break;
        }
        addCalc2_801F71A4(&field_0x290, 0.0f, 0.5f, 5.0f);
        if (field_0x290 < 10.0f) {
            mAlive = false;
        }
        break;
    case 22:
        if (getActorManager_801F856C()->fn_801F68E0() == 0) {
            break;
        }
        field_0x268 = 0;
        field_0x24c = 0;
        field_0x250 = 0;
        field_0x234 = 0;
        field_0x27c = 3;
        field_0x268 = 4;
        field_0x280 = 10.0f;
        field_0x288 = 40.0f;
        field_0x2ba = 0;
        mState = 23;
    case 23:
        if (FABS(mPos.z - field_0x288) <= 1.0f) {
            if (mEmitter2dc != NULL) {
                invalidateEmitter_8017EAB4(mEmitter2dc);
                mEmitter2dc = NULL;
            }
            getActorManager_801F856C()->fn_801F6884();
            die_0x24();
        }
        break;
    case 24:
        break;
    case 25:
        addCalc2_801F71A4(&field_0x2ac.y, 5.0f, 0.3f, 3.0f);
        addCalc2_801F71A4(&field_0x2ac.x, 0.0f, 0.3f, 0.1f);
        if (field_0x2ac.x < 0.1f) {
            mAlive = false;
        }
        break;
    }
    if (mState == 7 || mState == 17 || mState == 8 || mState == 9 || mState == 10 || mState == 21) {
        field_0x288 = 40.0f + 4.0f * cosShort_801544E4(field_0x24c);
        field_0x24c += 0x300;
    }
    if (mState == 16 || mState == 17 || mState == 9 || mState == 10) {
        f32 z = mPos.z;
        mPos.y -= z;
        mPos.z = 0.0f;
        s32 serial = findOverlappingActor_801F2718('FGAN');
        mPos.z = z;
        mPos.y += mPos.z;
        if (serial < 0) {
            return;
        }
        PhantomGanon_804A1318* other = static_cast<PhantomGanon_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (other == NULL) {
            return;
        }
        if (other->mSpawnParam_0x90 != 10) {
            return;
        }
        bool dead = false;
        if (mState == 9 || mState == 10) {
            dead = false;
        } else if (other->field_0x2bb == lbl_80539F88[4]) {
            if (field_0x278 >= 4) {
                mState = 18;
                dead = true;
                startSound_801F0E34(0x40044, 0);
            } else {
                other->field_0x24c = 0;
                other->field_0x254 = 1;
                other->field_0x238 = 0;
                other->field_0x23c = 0;
                spawnEmitter_8013CC50(mLayer_0x4, &other->mPos, lbl_80539F90[0], 1, 2, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x3002B, 0);
                mState = 14;
            }
        }
        if (dead) {
            other->mLife = 0;
        }
    }
}

// FIXME(shotgun): 94.66 %, scheduling+regalloc; behaviour drift.
void PhantomGanon_804A1318::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f pos = mPos;
    f32 scaleY = 0.0f;
    f32 scaleX = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1f = 0;
    params.field_0x2d = 1;
    if (mSpawnParam_0x90 == 0 || mSpawnParam_0x90 == 1) {
        mPos.z = 0.0f;
        void* res;
        switch (field_0x27c) {
        case 0:
            res = getBossArchiveResource_804032E8('FGAN', lbl_804A1290[field_0x268]);
            break;
        case 1:
            res = getBossArchiveResource_804032E8('FGAN', lbl_804A12D0[field_0x268]);
            scaleX = lbl_804A12A8[field_0x268][0];
            scaleY = lbl_804A12A8[field_0x268][1];
            break;
        case 2:
            res = getBossArchiveResource_804032E8('FGAN', lbl_804A12E4[field_0x268]);
            scaleY = -21.0f;
            break;
        case 3:
            res = getBossArchiveResource_804032E8('FGAN', lbl_804A1304[field_0x268]);
            break;
        }
        if (mSpawnParam_0x90 == 0) {
            params.field_0x1c = 0;
            params.field_0x1d = -2;
            params.field_0x10.set(0, 0, 0, 100);
            f32 height = 0.8f * field_0x2ac.y;
            mtx[0][0] = field_0x2ac.x;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = height;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = height;
            params.mMtx = mtx;
            drawSpriteRes_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
        }
        mPos.set(pos);
        params.field_0x1d = field_0x2ba;
        params.field_0x30 = 14;
        mtx[0][0] = field_0x2ac.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x2ac.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x2ac.y;
        params.mMtx = mtx;
        params.field_0x10.set(0xFF, 0xFF, 0xFF, (u8)field_0x290);
        drawSpriteRes_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
    }
}

// The 'FGAN' actor's (PhantomGanon_804A1318, vtable 0x804A1318) die_0x24.
void PhantomGanon_804A1318::die_0x24() {
    startSound_801F0E34(0x40047, 0);
    startBossDeath_0x28(field_0x2ba, 8.0f, -16.0f);
}

