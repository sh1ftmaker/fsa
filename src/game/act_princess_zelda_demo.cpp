#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_demo.h"
#include "game/ActorId.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/cls_8049D740.h"
#include "game/GameRootSub20.h"
#include "game/SpriteParams.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): 63.6 %, missing/extra
PrincessZeldaDemo_8049D208::PrincessZeldaDemo_8049D208() {
    field_0x230 = 0;
    field_0x231 = 0;
    field_0x234 = 4;
    field_0x238 = 1;
    field_0x23c = -1;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = -1;
    field_0x260[0] = 0;
    field_0x260[1] = 0;
    field_0x268[0] = 0.0f;
    field_0x268[1] = 0.0f;
    field_0x270 = 0.37f;
    field_0x274 = 96.0f;
    field_0x278 = 0.0f;
    field_0x364 = NULL;
    field_0x368 = NULL;
    field_0x36c = NULL;
    for (s32 i = 0; i < 4; i++) {
        field_0x370[i] = NULL;
        if (i < 2) {
            field_0x380[i] = NULL;
        }
    }
}

PrincessZeldaDemo_8049D208::~PrincessZeldaDemo_8049D208() {
    releaseEmitter_801EE434(field_0x364);
    field_0x364 = NULL;
    releaseEmitter_801EE434(field_0x368);
    field_0x368 = NULL;
    releaseEmitter_801EE434(field_0x36c);
    field_0x36c = NULL;
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(field_0x370[i]);
        field_0x370[i] = NULL;
        if (i < 2) {
            releaseEmitter_801EE434(field_0x380[i]);
            field_0x380[i] = NULL;
        }
    }
}

// The animation key tables (lbl_8049D1A0, 26 of them; lbl_80539EF8, 2) are in this unit's data.
extern const s16* lbl_8049D1A0[26];
extern const s16* lbl_80539EF8[2];

// FIXME(shotgun): 98.6 %, missing/extra
void PrincessZeldaDemo_8049D208::init() {
    cls_8049D140::init();
    field_0x234 = field_0x90 & 0xFF;
    field_0x244 = (field_0x90 >> 8) & 0xF;
    field_0x248 = (field_0x90 >> 12) & 0xF;
    field_0x254 = (field_0x90 >> 17) & 0x1F;
    field_0x250 = (field_0x90 >> 22) & 0x1F;
    mAnim0.fn_801D2608(lbl_8049D1A0, 26);
    mAnim0.startAnim_801D22DC(8, 2, false);
    mAnim0.fn_801D1F14(0.0f);
    mAnim1.fn_801D2608(lbl_80539EF8, 2);
    mAnim1.startAnim_801D22DC(0, 2, false);
    mAnim1.fn_801D1F14(0.0f);
    if (field_0x234 == 2) {
        if (getGameView_802D6A9C()->fn_802D800C(field_0x250)) {
            fn_8010F478(0x3d);
        } else {
            mDrawEnabled = false;
        }
    }
}

void PrincessZeldaDemo_8049D208::vfunc_0x10() {
    cls_801192AC* list = fn_8011921C();
    ActorBase_804A5770** goal = list->fn_8011871C(list->mBegin, ACTOR_DMGL);
    if (goal != fn_8011921C()->mEnd) {
        field_0x27c = (*goal)->mPos;
    }

    ActorBase_804A5770** node = fn_8011921C()->mBegin;
    do {
        node = fn_8011921C()->fn_8011871C(node, ACTOR_DMMD);
        if (node == fn_8011921C()->mEnd) {
            break;
        }
        cls_8049D740* maiden = static_cast<cls_8049D740*>(*node);
        if (maiden->mMode < 6) {
            field_0x288[maiden->mMode] = maiden->mPos;
            node++;
        }
    } while (node != fn_8011921C()->mEnd);

    field_0x2d0 = mPos;
}

void PrincessZeldaDemo_8049D208::update() {
    cls_8049D140::update();
    switch (field_0x234) {
    case 0:
    case 1:
        fn_80110E9C();
        break;
    case 2:
        fn_8011093C();
        break;
    case 3: // nothing sets this state
        break;
    }
    mAnim0.fn_801D1C18();
    mAnim1.fn_801D1C18();
    if (field_0x230 != 0) {
        fn_8010F4DC();
    }
}

void PrincessZeldaDemo_8049D208::draw() {
    cls_8049D140::draw();
    if (field_0x230 == 0) {
        if (mAnim0.mAnim >= 0x13) {
            fn_8010F7EC(0x21b);
        } else if (mAnim0.mAnim >= 0xf) {
            fn_8010F7EC(0x274);
        } else {
            fn_8010F7EC(0x9b);
        }
    }
}

s32 PrincessZeldaDemo_8049D208::vfunc_0x58() {
    return field_0x238;
}

bool PrincessZeldaDemo_8049D208::fn_8010FA54() const {
    switch (field_0x238) {
    case 0x2f:
    case 0x3d:
    case 0x3e:
        return true;
    }
    return false;
}

void PrincessZeldaDemo_8049D208::fn_8010F9F0() {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(0x31);
        break;
    case 2:
        fn_8010E2D4(0x31);
        break;
    case 3: // nothing sets this state
        break;
    }
    field_0x238 = 0x31;
}

// Not auto-inlined: draw calls fn_8010F7EC, update calls fn_8010F4DC out of line in the original
// (docs/INLINING.md, pragma rule 1); fn_8010F4DC calls fn_8010F668 out of line twice.
#pragma auto_inline off
// Draws the maiden's sprite (frame `arg` of the current animation): bobbing in z while state 0x2F.
// FIXME(shotgun): 71 %, missing/extra + reloc-name
void PrincessZeldaDemo_8049D208::fn_8010F7EC(s32 arg) {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1d = field_0x24c;
    if (field_0x238 == 0x2F) {
        field_0x260[1] += 0x258;
        if (field_0x260[1] >= 0x10000) {
            field_0x260[1] -= 0x10000;
        }
        PSMTXIdentity(mtx);
        mtx[2][3] = 4.0f * JMath::sincosTable_.sinShort(field_0x260[1]);
        params.mMtx = mtx;
    }
    if (arg == 0x21B) {
        fn_801F02BC("enemy_gc_sob.bin", arg + mAnim0.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_both2_sob.bin", arg + mAnim0.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
    }
}

// The rotation matrix about `axis` by `rad` (the 3x3 part), with its own normalisation.
// FIXME(shotgun): 61 %, missing/extra + reloc-name
void fn_8010F668(Mtx m, const Vec* axis, f32 rad) {
    f32 sq = axis->x * axis->x + axis->y * axis->y + axis->z * axis->z;
    f32 x;
    f32 y;
    f32 z;
    if (sq <= 32.0f * FLT_EPSILON) {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    } else {
        f32 inv = sq;
        if (!(sq <= 0.0f)) {
            inv = JGeometry::TUtil<f32>::inv_sqrt(sq);
        }
        x = axis->x * inv;
        y = axis->y * inv;
        z = axis->z * inv;
    }
    f32 s = sin(rad);
    f32 c = cos(rad);
    f32 t = 1.0f - c;
    m[0][0] = t * (x * x) + c;
    m[0][1] = y * (t * x) - s * z;
    m[0][2] = z * (t * x) + s * y;
    m[1][0] = y * (t * x) + s * z;
    m[1][1] = t * (y * y) + c;
    m[1][2] = z * (t * y) - s * x;
    m[2][0] = z * (t * x) - s * y;
    m[2][1] = z * (t * y) + s * x;
    m[2][2] = t * (z * z) + c;
}

// Spins the crystal ring: turns by 0.01 rad per frame and hands the shared crystal its matrix.
// FIXME(shotgun): 69 %, different + reloc-name
void PrincessZeldaDemo_8049D208::fn_8010F4DC() {
    field_0x268[0] += 0.01f;
    Mtx mtx;
    PSMTXTrans(mtx, mPos.x, mPos.y, mPos.z);
    Vec axisX = {1.0f, 0.0f, 0.0f};
    fn_8010F668(mtx, &axisX, 0.08726646f);
    Mtx rot;
    for (s32 i = 0; i < 3; i++) {
        for (s32 j = 0; j < 4; j++) {
            rot[i][j] = 0.0f;
        }
    }
    Vec axisZ = {0.0f, 0.0f, 1.0f};
    fn_8010F668(rot, &axisZ, field_0x268[0]);
    PSMTXConcat(mtx, rot, mtx);
    field_0x240++;
    if (field_0x240 >= 200) {
        field_0x240 = 0;
    }
    s32 frame = field_0x240;
    f32 scale = field_0x270;
    GameRootSub20_801902C0::Crystal& crystal = fn_801902C0()->mCrystals[6];
    if (crystal.mModel != NULL && crystal.mActive == 0) {
        crystal.mActive = 1;
        crystal.mPos.x = scale;
        crystal.mPos.y = scale;
        crystal.mPos.z = scale;
        PSMTXCopy(mtx, crystal.mMtx);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 1;
    }
}

#pragma auto_inline reset

// FIXME(shotgun): 94.4 %, missing/extra
void PrincessZeldaDemo_8049D208::fn_8010F478(s32 arg) {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(arg);
        break;
    case 2:
        fn_8010E2D4(arg);
        break;
    }
    field_0x238 = arg;
}
