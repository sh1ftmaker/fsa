#include "game/GameAudio.h"
#include "game/BgRewrite.h"
#include "game/EmitterDelete.h"
#include "game/EnemyBase.h"
#include "game/GameMath.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_icicle.h"

// One animation: (cel, duration) pairs, ended by a cel of -1.
static const s16 lbl_8053C448[] = {0, 30, -1, -1};
static const s16 lbl_804642D8[] = {1, 4, 2, 4, 3, 4, 4, 4, 5, 4, -1, -1};
static const s16* lbl_80539EE0[] = {lbl_8053C448, lbl_804642D8};

// FIXME: 97.44%. Same instructions in a different register and schedule for the
// `blocked` flag and the j loop, and the .sdata2 literals (data split).
bool Icicle_8049CFC8::fn_8010B284() {
    JGeometry::TVec3f playerPos;
    JGeometry::TVec3f probe = mPos;
    bool blocked = false;
    for (s32 i = 0; i < 4; i++) {
        playerPos = *getPlayerPos_8023E724(i);
        if (field_0x4 == 8 && getPlayerLayer_8023DE58(i) == 0) {
            fn_801CD150(&playerPos, &playerPos);
        }
        if (mPos.y > playerPos.y || mPos.x - 40.0f > playerPos.x ||
            playerPos.x > mPos.x + 40.0f) {
            continue;
        }
        for (s32 j = 1; j <= 24; j++) {
            probe.y = mPos.y + 16.0f * j;
            if (probe.y >= playerPos.y) {
                break;
            }
            u16 tile = fn_80226850(&probe, field_0x4);
            if (tile >= 0x3E7 || (tile >= 0x250 && tile <= 0x26F)) {
                blocked = true;
                break;
            }
        }
        return !blocked;
    }
    return false;
}

// FIXME: 99.65%. Code matches; the .sdata2 literals (0.0f, 0.6f, 1.0f) are
// separate entries in ours, in other numbers than lbl_8053C470/74/78 (data split).
bool Icicle_8049CFC8::fn_8010B40C(s32 state) {
    switch (state) {
    case 1:
        field_0x23c = 0.0f;
        startSound_801F0E34(0x3007C, 0);
        break;
    case 2:
        deleteEmitter(&mEmitter100);
        deleteEmitter(&mEmitter104);
        if (field_0x4 == 0) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x28D, 0, 0, NULL, NULL, true, 0.6f);
            field_0x27c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x28E, 0, 0, NULL, NULL, true, 0.6f);
        }
        field_0x23c = 0.0f;
        startSound_801F0E34(0x30061, 0);
        break;
    case 3:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 1.0f);
        mSprite.startAnim_801D22DC(1, 0, false);
        field_0x23c = 0.0f;
        startSound_801F0E34(0x3007D, 0);
        break;
    case 4:
        mDrawEnabled = false;
        field_0x240 = 0.0f;
        mSprite.startAnim_801D22DC(0, 2, false);
        mPos.set(mSpawnPos);
        mPos.y = mSpawnPos.y - 8.0f;
        break;
    case 5:
        field_0xd8 = 0;
        field_0x238 = 0;
        break;
    }
    mSprite.fn_801D1F14(0.0f);
    field_0x230 = state;
    field_0x234 = 0;
    return true;
}

// FIXME: 98.47%. Same instructions except the order of the field_0x54.y and
// field_0x54.x loads (the original loads y first); the .sdata2 literals are data
// splits. A read-once local for y matches the order, but only as a trick.
void Icicle_8049CFC8::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (field_0x230 == 5 && !field_0x54.isZero()) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x54.x;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x54.y;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x54.z;
        params.mMtx = mtx;
    }
    if (field_0x54.isZero()) {
        mDrawEnabled = false;
    }
    fn_801F02BC("enemy_both2_sob.bin", mSprite.mCel + 0x19C, &params, 7, 0, false, 1.0f, 1.0f);
    if (field_0x54.isZero()) {
        mDrawEnabled = true;
    }
}

// FIXME: 70.00%. MWCC inlines fn_8010B284 (defined above, in address order)
// into case 0, where the original calls it. Without the inlining (91.85 %) the
// rest is register and stack-slot allocation in the box tests of cases 0 and 1
// (the original keeps the `found` flags in r0 and r30, and its box copies are
// laid out differently), plus the order of the fn_801CC640 call block.
void Icicle_8049CFC8::update() {
    ActorBase_804A5770::update();
    switch (field_0x230) {
    case 0:
        if (fn_8010B284()) {
            fn_8010B40C(1);
            break;
        }
        if (fn_801F285C(NULL, NULL, true, -1) >= 0) {
            fn_8010B40C(1);
            break;
        }
        {
            JGeometry::TVec2f pt(0.0f, 0.0f);
            JGeometry::TVec2f p(mPos.x, mPos.y - mPos.z);
            JGeometry::TBox2f box(mBox2);
            box.addPos(p);
            JGeometry::TVec3f out;
            bool found = false;
            s32 player = fn_8023AA18(&box, field_0x4, &pt);
            if (player >= 0) {
                found = true;
            } else if ((player = fn_8023AE34(&box, field_0x4, &pt)) >= 0) {
                fn_801CC820(player, NULL);
                found = true;
            } else if ((player = fn_8023AC74(&box, field_0x4, &pt)) >= 0) {
                fn_801CC538(player, &pt, &out, NULL);
                found = true;
            } else if ((player = fn_8023AD54(&box, field_0x4, &pt)) >= 0) {
                fn_801CC640(player, NULL, NULL);
                found = true;
            }
            if (found) {
                fn_8010B40C(1);
                break;
            }
        }
        {
            JGeometry::TVec2f p(mPos.x, mPos.y);
            JGeometry::TBox2f box(mBox);
            box.addPos(p);
            JGeometry::TVec2f out;
            bool inBox = fn_8023AB94(&box, field_0x4, &out) >= 0;
            if (inBox) {
                fn_8010B40C(2);
            }
        }
        break;
    case 1:
        field_0x23c += 0.1f;
        mPos.y = mSpawnPos.y + 0.5f * (9.8f * square(field_0x23c));
        if (fn_801F2B7C(field_0x114, -1, 0x18)) {
            fn_8010B40C(3);
            break;
        }
        {
            JGeometry::TVec3f hitDir;
            if (fn_801F2618(&hitDir, false)) {
                JGeometry::TVec3f probe = mPos;
                probe.y += 16.0f;
                u16 tile = fn_80226850(&probe, field_0x4);
                if (tile >= 0x3F0 || (tile >= 0x250 && tile <= 0x26F) || tile == 0x3ED) {
                    gBgRewrite_8053AC98->fn_802A381C(&probe, field_0x4, 0x34);
                }
                fn_8010B40C(3);
                break;
            }
        }
        field_0x234++;
        if (field_0x234 < 30) {
            break;
        }
        if (fn_801F285C(NULL, NULL, true, -1) >= 0) {
            fn_8010B40C(3);
            break;
        }
        {
            JGeometry::TVec2f pt(0.0f, 0.0f);
            JGeometry::TVec2f p(mPos.x, mPos.y - mPos.z);
            JGeometry::TBox2f box(mBox2);
            box.addPos(p);
            JGeometry::TVec3f out;
            bool found = false;
            s32 player = fn_8023AA18(&box, field_0x4, &pt);
            if (player >= 0) {
                found = true;
            } else if ((player = fn_8023AE34(&box, field_0x4, &pt)) >= 0) {
                fn_801CC820(player, NULL);
                found = true;
            } else if ((player = fn_8023AC74(&box, field_0x4, &pt)) >= 0) {
                fn_801CC538(player, &pt, &out, NULL);
                found = true;
            } else if ((player = fn_8023AD54(&box, field_0x4, &pt)) >= 0) {
                fn_801CC640(player, NULL, NULL);
                found = true;
            }
            if (found) {
                fn_8010B40C(3);
                break;
            }
        }
        {
            JGeometry::TVec2f p(mPos.x, mPos.y);
            JGeometry::TBox2f box(mBox);
            box.addPos(p);
            JGeometry::TVec2f out;
            bool inBox = fn_8023AB94(&box, field_0x4, &out) >= 0;
            if (inBox) {
                fn_8010B40C(2);
            }
        }
        break;
    case 2:
        fn_8010B40C(4);
        break;
    case 3:
        if (mSprite.fn_801D21F0()) {
            fn_8010B40C(4);
        }
        break;
    case 4:
        field_0x238++;
        if (field_0x238 > 0x12C) {
            fn_8010B40C(5);
        }
        break;
    case 5:
        if (vfunc_0x3C() > 0 && !vfunc_0x34()) {
            break;
        }
        mDrawEnabled = true;
        fn_801CD950(&field_0x240, 1.0f, 0.005f);
        mPos.y = mSpawnPos.y - 8.0f * (1.0f - field_0x240);
        if (field_0x240 >= 1.0f) {
            fn_8010B40C(0);
        }
        break;
    }
    field_0x54.x = field_0x54.y = field_0x54.z = field_0x240;
    mSprite.fn_801D1C18();
}

// FIXME: 99.69%. Code matches; the -8.0f, 8.0f and 0.0f literals are separate
// .sdata2 entries in ours, in a different order from lbl_8053C47C, lbl_8053C488
// and lbl_8053C470 (data split, not code).
void Icicle_8049CFC8::init() {
    ActorBase_804A5770::init();
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = -8.0f;
    mBox2.i.y = -8.0f;
    mBox2.f.x = 8.0f;
    mBox2.f.y = 8.0f;
    field_0x80.i.x = -8.0f;
    field_0x80.i.y = -8.0f;
    field_0x80.f.x = 8.0f;
    field_0x80.f.y = 8.0f;
    mSprite.fn_801D2608(lbl_80539EE0, 2);
    mSprite.startAnim_801D22DC(0, 2, false);
    mSprite.fn_801D1F14(0.0f);
    if (vfunc_0x3C() > 0) {
        mDrawEnabled = false;
        fn_8010B40C(5);
    }
}

Icicle_8049CFC8::~Icicle_8049CFC8() {
    releaseEmitter_801EE434(field_0x27c);
    field_0x27c = NULL;
}

// FIXME: 99.80%. Code matches; the 1.0f literal is a separate .sdata2 entry in
// ours, the original's is lbl_8053C478 (a data split, not code).
Icicle_8049CFC8::Icicle_8049CFC8()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x240(1.0f), mSprite(),
      field_0x27c(NULL) {}
