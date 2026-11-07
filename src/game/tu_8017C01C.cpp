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

// Tables of the 'FGAN' actor (.data 0x804A1210, not written as source here): emitter ids per
// slot, and the "FGAN" resource names / sprite offsets of each draw mode.
extern const u16 lbl_804A1210[];
extern const char* const lbl_804A1290[];
extern const f32 lbl_804A12A8[][2];
extern const char* const lbl_804A12D0[];
extern const char* const lbl_804A12E4[];
extern const char* const lbl_804A1304[];
extern u8 lbl_80539F88[];

// Sets the map's field_0x216 flag (gGameRoot2_8053AB10->0x20, TileMap_804AA858; 9 units
// set it to 1). Takes no arguments; the caller is the fn_8017C4C8 branch of
// the actor update.
void fn_8017ED08() {
    gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
}

// Out-of-line copy of JUtility::TColor::set(r, g, b, a).
void fn_8017ECF4(JUtility::TColor* color, u8 r, u8 g, u8 b, u8 a) {
    color->set(r, g, b, a);
}

// Out-of-line copy of TVec2<f32>::set(x, y).
void fn_8017ECE8(JGeometry::TVec2f* v, f32 x, f32 y) {
    v->set(x, y);
}

// FIXME: 89. The original stores the result's own register (1) to field_0x178; ours loads
// the constant twice (li r0 and li r4).
// Sets the camera's field_0x178 flag unless field_0x179 is set.
u8 fn_8017ECC0() {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x179 == 0) {
        result = 1;
        camera->field_0x178 = 1;
    }
    return result;
}

// FIXME: 93.55. The original stores the box corners in the order f.x, i.x, f.y, i.y (ours
// i.x, i.y, f.x, f.y) and keeps the constant 1 of field_0x840 in the result's register.
// Fixes the camera on pos (both corners of its box) with `value`, once.
u8 fn_8017EC70(const JGeometry::TVec3f* pos, f32 value) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x840 == 0) {
        result = 1;
        camera->field_0x840 = 1;
        camera->field_0x844.set(pos->x, pos->y, pos->x, pos->y);
        camera->field_0x854 = value;
        camera->field_0x1d4 = -1.0f;
        camera->field_0x1d8 = -1.0f;
    }
    return result;
}

// Out-of-line copy of TBox2<f32>::set(x0, y0, x1, y1).
void fn_8017EC5C(JGeometry::TBox2f* box, f32 x0, f32 y0, f32 x1, f32 y1) {
    box->set(x0, y0, x1, y1);
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
    if (camera->fn_8030C2A8(&corner)) {
        corner2.set(box->f.x, box->f.y, 0.0f);
        if (camera->fn_8030C2A8(&corner2)) {
            result = true;
        }
    }
    return result;
}

// FIXME: 83.75. The original keeps the constant 1 (stb 0x7f4) in r4 and copies it to the result
// (mr r5, r4), and stores the 0 for field_0x7f6 from the result's zero register; ours loads
// separate constants.
// Starts a scripted camera move (mode 4) to pos unless one is already set.
u8 fn_8017EB44(const JGeometry::TVec3f* pos) {
    s32 result = 0;
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.set(*pos);
        camera->field_0x7f6 = 0;
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
void fn_8017EB0C(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

void ActorBase_804A5770::fn_8017EAF8(const JGeometry::TVec3f& v) {
    field_0x3c = v;
}

// Out-of-line copy of TVec3<f32>::set(const TVec3&).
void fn_8017EADC(JGeometry::TVec3f* dst, const JGeometry::TVec3f* src) {
    dst->set(*src);
}

// Out-of-line copy of JPABaseEmitter::setGlobalEnvColor(r, g, b).
void fn_8017EACC(JPABaseEmitter* emitter, u8 r, u8 g, u8 b) {
    emitter->setGlobalEnvColor(r, g, b);
}

// Out-of-line copy of JPABaseEmitter::becomeInvalidEmitter().
void fn_8017EAB4(JPABaseEmitter* emitter) {
    emitter->becomeInvalidEmitter();
}

// FIXME(shotgun): 57.97 %, missing/extra + reloc-name. The original hoists the float constants and the stmw prologue differently.
void cls_804A1318::init() {
    field_0x94 = (field_0x90 >> 4) & 0xF;
    field_0x90 &= 0xF;
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
    field_0x29c = 0.0f;
    field_0x298 = 0.0f;
    field_0x294 = 0.0f;
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
    field_0x2ac = 1.0f;
    field_0x2b0 = 1.0f;
    field_0x2b4 = 1.0f;
    field_0x278 = 0;
    field_0x2a0.set(mPos);
    field_0x2a0.y += 16.0f;
    switch (field_0x90) {
    case 10:
        for (int i = 0; i < 4; i++) {
            mEmitters2bc[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1210[i * 2], 1, 2, NULL, NULL, true, 1.0f);
            mEmitters2cc[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_804A1210[i * 2 + 1], 1, 2, NULL, NULL, true, 1.0f);
            if (i != 0) {
                if (mEmitters2bc[i] != NULL) {
                    mEmitters2bc[i]->setStatus(4);
                }
                if (mEmitters2cc[i] != NULL) {
                    mEmitters2cc[i]->setStatus(4);
                }
            }
        }
        field_0x118 = 4;
        field_0x2b4 = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2ac = 0.0f;
        field_0x2ba = 2;
        mBox.i.x = -7.0f;
        mBox.i.y = -7.0f;
        mBox.f.x = 7.0f;
        mBox.f.y = 7.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        mState = 0x1A;
        break;
    case 11:
        field_0x2bb = lbl_80539F88[4];
        field_0x94 = 0;
        field_0x118 = 2;
        field_0x234 = 30;
        field_0x2ba = 2;
        mBox.i.x = -2.0f;
        mBox.i.y = -2.0f;
        mBox.f.x = 2.0f;
        mBox.f.y = 2.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        mState = 0x1B;
        break;
    case 0:
        field_0x2b4 = 0.0f;
        field_0x2b0 = 0.0f;
        field_0x2ac = 0.0f;
        field_0x290 = 0.0f;
        getGameView_802D6A9C()->fn_802D7440(false);
        // fall through
    default:
        mBox.i.x = -16.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 40.0f;
        field_0x80.i.x = -8.0f;
        field_0x80.i.y = -8.0f;
        field_0x80.f.x = 8.0f;
        field_0x80.f.y = 8.0f;
        field_0x114 = 2;
        field_0x2ba = 1;
        mState = 0;
        break;
    }
}

// FIXME(shotgun): 99.74 %, regalloc-only + reloc-name.
cls_804A1318::~cls_804A1318() {
    for (int i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters2bc[i]);
        releaseEmitter_801EE434(mEmitters2cc[i]);
    }
    releaseEmitter_801EE434(mEmitter2dc);
    if (field_0x90 == 0) {
        gGameRoot2_8053AB10->field_0x2c->field_0x4.set(0xFF, 0xFF, 0xFF, 0);
        getGameView_802D6A9C()->fn_802D7440(true);
        fn_8022ADCC();
    }
    if (field_0x90 == 10) {
        s32 serial = field_0x274;
        if (serial >= 0) {
            cls_804A1318* other = static_cast<cls_804A1318*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (other != NULL && other->mAlive == true) {
                other->field_0x28c = 3.0f;
                other->field_0x278 = 0;
                field_0x274 = -1;
            }
        }
    }
}

// FIXME(shotgun): 86.52 %, missing/extra + reloc-name.
void cls_804A1318::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    JGeometry::TVec3f pos = mPos;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    params.field_0x1f = 0;
    params.field_0x2d = 1;
    if (field_0x90 == 0 || field_0x90 == 1) {
        mPos.z = 0.0f;
        f32 scaleX = 0.0f;
        f32 scaleY = 0.0f;
        void* res;
        switch (field_0x27c) {
        case 0:
            res = fn_804032E8('FGAN', lbl_804A1290[field_0x268]);
            break;
        case 1:
            res = fn_804032E8('FGAN', lbl_804A12D0[field_0x268]);
            scaleX = lbl_804A12A8[field_0x268][0];
            scaleY = lbl_804A12A8[field_0x268][1];
            break;
        case 2:
            res = fn_804032E8('FGAN', lbl_804A12E4[field_0x268]);
            scaleY = -21.0f;
            break;
        case 3:
            res = fn_804032E8('FGAN', lbl_804A1304[field_0x268]);
            break;
        }
        if (field_0x90 == 0) {
            params.field_0x1c = 0;
            params.field_0x1d = -2;
            params.field_0x10.set(0, 0, 0, 100);
            f32 height = 0.8f * field_0x2b0;
            mtx[0][0] = field_0x2ac;
            mtx[0][1] = 0.0f;
            mtx[0][2] = 0.0f;
            mtx[1][0] = 0.0f;
            mtx[1][1] = height;
            mtx[1][2] = 0.0f;
            mtx[2][0] = 0.0f;
            mtx[2][1] = 0.0f;
            mtx[2][2] = height;
            params.mMtx = mtx;
            fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
        }
        mPos.set(pos);
        params.field_0x1d = field_0x2ba;
        params.field_0x30 = 14;
        mtx[0][0] = field_0x2ac;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x2b0;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x2b0;
        params.mMtx = mtx;
        params.field_0x10.set(0xFF, 0xFF, 0xFF, (u8)field_0x290);
        fn_801EFD34(res, &params, -1, scaleX, scaleY, 0.0f);
    }
}

// The 'FGAN' actor's (cls_804A1318, vtable 0x804A1318) vfunc_0x24.
void cls_804A1318::vfunc_0x24() {
    startSound_801F0E34(0x40047, 0);
    vfunc_0x28(field_0x2ba, 8.0f, -16.0f);
}

// FIXME(shotgun): update (fn_8017C40C, 0x2234 bytes) is not written.
