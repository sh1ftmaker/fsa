#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_room_transfer_lift.h"

static const char* lbl_80539EC8[] = {"lift_00.bti", NULL};

cls_8049CC40::cls_8049CC40()
    : field_0x230(NULL)
    , field_0x234(NULL)
{
    field_0x280 = 5.0f;
    field_0x284 = 5.0f;
    field_0x288 = 0.5f;
    field_0x28c = 0.5f;
    field_0x290 = 0;
    field_0x294 = 0;
    field_0x298 = 0;
    field_0x29c = 0;
    field_0x2a0 = 0;
    field_0x2a4 = 0;
}

void cls_8049CC40::init() {
    ActorBase_804A5770::init();
    // 0x3F3F3F3F is the resource type the original passes here for both lookups ('????').
    field_0x230 = gGameRoot_8053AAF8->field_0xa4->getResource(0x3F3F3F3F, lbl_80539EC8[0]);
    field_0x234 = gGameRoot_8053AAF8->field_0xa4->getResource(0x3F3F3F3F, "black.bti");
    field_0x29c = field_0x90 & 0xF;
    field_0x2a0 = (field_0x90 >> 4) & 0xF;
    mBox.set(-24.0f, -24.0f, 24.0f, 24.0f);
    mBox.set(-30.0f, -30.0f, 30.0f, 30.0f);
    field_0xb8 = 1;
}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

void cls_8049CC40::update() {
    ActorBase_804A5770::update();
    switch (field_0x290) {
    case 0: {
        for (int i = 0; i < 4; i++) {
            if (!isInside(i)) {
                return;
            }
        }
        field_0x290 = 1;
        field_0x274 = mPos;
        break;
    }
    case 1: {
        field_0x2a4++;
        JGeometry::TVec3f delta(0.0f, -0.1f, 0.0f);
        mPos += delta;
        field_0x280 *= 0.999f;
        field_0x284 *= 0.999f;
        for (int i = 0; i < 4; i++) {
            fn_8022DF48(i);
            fn_8023DF68(i, &delta);
        }
        if (field_0x2a4 >= 120) {
            field_0x290 = 2;
            field_0x2a4 = 0;
        }
        break;
    }
    case 2:
        getGameView_802D6A9C()->fn_802D9FF4(field_0x29c, field_0x2a0);
        gGameRoot_8053AAF8->fn_80136798(0xD, 0);
        field_0x290 = 3;
        break;
    case 3:
        // Empty state; nothing sets further transitions
        break;
    }
}

// Variant of game/RotMatrix.h's setRotScaleMtx with a scale per column (inlined twice in draw).
static inline void setRotScaleMtx(Mtx m, const JGeometry::TVec3f& rot, f32 scaleX, f32 scaleY, f32 scaleZ) {
    f32 sx = sin(rot.x);
    f32 sy = sin(rot.y);
    f32 sz = sin(rot.z);
    f32 cx = cos(rot.x);
    f32 cy = cos(rot.y);
    f32 cz = cos(rot.z);
    m[0][0] = scaleX * (cy * cz);
    m[1][0] = scaleX * (cy * sz);
    m[2][0] = scaleX * -sy;
    m[0][1] = scaleY * (sx * sy * cz - cx * sz);
    m[1][1] = scaleY * (sx * sy * sz + cx * cz);
    m[2][1] = scaleY * (sx * cy);
    m[0][2] = scaleZ * (cx * cz * sy + sx * sz);
    m[1][2] = scaleZ * (cx * sz * sy - sx * cz);
    m[2][2] = scaleZ * (cx * cy);
}

// FIXME: 97.08%, scheduling+regalloc. Same instructions as the target; the float registers (f25-f31)
// holding the sines, cosines and the two scales in the two inlined setRotScaleMtx copies differ, so the fmuls/stfs
// are ordered differently.
void cls_8049CC40::draw() {
    Mtx mtx1;

    ActorBase_804A5770::draw();
    mSpriteParams.field_0x1d = -1;
    PSMTXTrans(mtx1, 0.0f, 0.0f, 0.0f);
    f32 sx1 = field_0x288;
    f32 sz1 = field_0x28c;
    setRotScaleMtx(mtx1, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), sx1, 1.0f, sz1);
    mSpriteParams.mMtx = mtx1;

    SpriteParams_801F5AE0 params;
    Mtx mtx2;
    params.field_0x1d = 0;
    PSMTXTrans(mtx2, 0.0f, 0.0f, 0.0f);
    f32 sx2 = field_0x280;
    f32 sz2 = field_0x284;
    setRotScaleMtx(mtx2, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), sx2, 1.0f, sz2);
    params.mMtx = mtx2;

    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_80539EC8[0]);
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, &mSpriteParams);
    }
}

cls_8049CC40::~cls_8049CC40() {}

// A plain function whose out-of-line copy the linker dropped (update inlines it).
// Compiled first (the bottom of the file), it gives the .sdata2 pool its leading
// 8.0f ahead of draw's 0.0f / 1.0f, as in the original.
bool cls_8049CC40::isInside(s32 player) const {
    s32 layer = field_0x4;
    s32 playerLayer = getPlayerLayer_8023DE58(player);
    if (playerLayer != layer) {
        return false;
    }
    JGeometry::TVec3f pos = *getPlayerPos_8023E724(player);
    JGeometry::TBox2f box(mBox);
    box.addPos(mPos.x + 8.0f, mPos.y + 8.0f);
    if (contains(box, pos)) {
        return true;
    }
    return false;
}
