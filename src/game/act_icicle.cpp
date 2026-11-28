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

Icicle_8049CFC8::Icicle_8049CFC8()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x240(1.0f), mSprite(),
      field_0x27c(NULL) {}

Icicle_8049CFC8::~Icicle_8049CFC8() {
    releaseEmitter_801EE434(field_0x27c);
    field_0x27c = NULL;
}

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
    mMapBox_0x80.i.x = -8.0f;
    mMapBox_0x80.i.y = -8.0f;
    mMapBox_0x80.f.x = 8.0f;
    mMapBox_0x80.f.y = 8.0f;
    mSprite.setKeyTables_801D2608(lbl_80539EE0, 2);
    mSprite.startAnim_801D22DC(0, 2, false);
    mSprite.setFrame_801D1F14(0.0f);
    if (getScreenFlagBit_0x3C() > 0) {
        mDrawEnabled = false;
        fn_8010B40C(5);
    }
}

// Remaining diff: f2/f3 register swap and one scheduled store in checkPlayer().
inline bool Icicle_8049CFC8::checkAttack() {
    JGeometry::TVec2f pt(0.0f, 0.0f);
    JGeometry::TVec2f p(mPos.x, mPos.y - mPos.z);
    JGeometry::TBox2f box(mBox2);
    box.addPos(p);
    JGeometry::TVec3f out;
    s32 player = fn_8023AA18(&box, mLayer_0x4, &pt);
    if (player >= 0) {
        return true;
    } else if ((player = fn_8023AE34(&box, mLayer_0x4, &pt)) >= 0) {
        fn_801CC820(player, NULL);
        return true;
    } else if ((player = fn_8023AC74(&box, mLayer_0x4, &pt)) >= 0) {
        fn_801CC538(player, &pt, &out, NULL);
        return true;
    } else if ((player = fn_8023AD54(&box, mLayer_0x4, &pt)) >= 0) {
        fn_801CC640(player, NULL, NULL);
        return true;
    }
    return false;
}

inline bool Icicle_8049CFC8::checkPlayer() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    JGeometry::TVec2f out;
    if (fn_8023AB94(&box, mLayer_0x4, &out) >= 0) {
        return true;
    }
    return false;
}

// FIXME: 98.73 %, scheduling+regalloc; behaviour clean.
void Icicle_8049CFC8::update() {
    ActorBase_804A5770::update();
    switch (field_0x230) {
    case 0:
        if (fn_8010B284()) {
            fn_8010B40C(1);
            break;
        }
        if (findOverlappingPlayer_801F285C(NULL, NULL, true, -1) >= 0) {
            fn_8010B40C(1);
            break;
        }
        if (checkAttack()) {
            fn_8010B40C(1);
            break;
        }
        if (checkPlayer()) {
            fn_8010B40C(2);
        }
        break;
    case 1:
        fall();
        if (damagePlayersInBox_801F2B7C(mParamAtcp_0x114, -1, 0x18)) {
            fn_8010B40C(3);
            break;
        }
        {
            JGeometry::TVec3f hitDir;
            if (sweepMapBox_801F2618(&hitDir, false)) {
                JGeometry::TVec3f probe = mPos;
                probe.y += 16.0f;
                u16 tile = getRawTile_80226850(&probe, mLayer_0x4);
                if (tile >= 0x3F0 || (tile >= 0x250 && tile <= 0x26F) || tile == 0x3ED) {
                    gBgRewrite_8053AC98->writeTile_802A381C(&probe, mLayer_0x4, 0x34);
                }
                fn_8010B40C(3);
                break;
            }
        }
        field_0x234++;
        if (field_0x234 < 30) {
            break;
        }
        if (findOverlappingPlayer_801F285C(NULL, NULL, true, -1) >= 0) {
            fn_8010B40C(3);
            break;
        }
        if (checkAttack()) {
            fn_8010B40C(3);
            break;
        }
        if (checkPlayer()) {
            fn_8010B40C(2);
        }
        break;
    case 2:
        fn_8010B40C(4);
        break;
    case 3:
        if (mSprite.isStopped_801D21F0()) {
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
        if (getScreenFlagBit_0x3C() > 0 && !isScreenFlagSet_0x34()) {
            break;
        }
        mDrawEnabled = true;
        moveToward_801CD950(&field_0x240, 1.0f, 0.005f);
        mPos.y = mSpawnPos.y - 8.0f * (1.0f - field_0x240);
        if (field_0x240 >= 1.0f) {
            fn_8010B40C(0);
        }
        break;
    }
    field_0x54.setAll(field_0x240);
    mSprite.update_801D1C18();
}

static inline void setScaleMtx(Mtx m, f32 sx, f32 sy, f32 sz) {
    m[0][0] = sx;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = sy;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = sz;
}

void Icicle_8049CFC8::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (field_0x230 == 5 && !field_0x54.isZero()) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        setScaleMtx(mtx, field_0x54.x, field_0x54.y, field_0x54.z);
        params.mMtx = mtx;
    }
    if (field_0x54.isZero()) {
        mDrawEnabled = false;
    }
    drawSpriteScaled_801F02BC("enemy_both2_sob.bin", mSprite.mCel + 0x19C, &params, 7, 0, false, 1.0f, 1.0f);
    if (field_0x54.isZero()) {
        mDrawEnabled = true;
    }
}

bool Icicle_8049CFC8::fn_8010B40C(s32 state) {
    switch (state) {
    case 1:
        field_0x23c = 0.0f;
        startSound_801F0E34(0x3007C, 0);
        break;
    case 2:
        deleteEmitter(&mEmitter100);
        deleteEmitter(&mEmitter104);
        if (mLayer_0x4 == 0) {
            spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x28D, 0, 0, NULL, NULL, true, 0.6f);
            field_0x27c = spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x28E, 0, 0, NULL, NULL, true, 0.6f);
        }
        field_0x23c = 0.0f;
        startSound_801F0E34(0x30061, 0);
        break;
    case 3:
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x28B, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(mLayer_0x4, &mPos, 0x28C, 0, 0, NULL, NULL, true, 1.0f);
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
        mHitTimer_0xd8 = 0;
        field_0x238 = 0;
        break;
    }
    mSprite.setFrame_801D1F14(0.0f);
    field_0x230 = state;
    field_0x234 = 0;
    return true;
}

// FIXME: 97.59%. Same instructions in a different register and schedule for the
// `blocked` flag and the j loop.
// Not auto-inlined: update calls fn_8010B284 out of line in the original
// (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
bool Icicle_8049CFC8::fn_8010B284() {
    JGeometry::TVec3f playerPos;
    JGeometry::TVec3f probe = mPos;
    for (s32 i = 0; i < 4; i++) {
        playerPos = *getPlayerPos_8023E724(i);
        if (mLayer_0x4 == 8 && getPlayerLayer_8023DE58(i) == 0) {
            fn_801CD150(&playerPos, &playerPos);
        }
        if (mPos.y > playerPos.y || mPos.x - 40.0f > playerPos.x ||
            playerPos.x > mPos.x + 40.0f) {
            continue;
        }
        s32 j;
        bool blocked = false;
        for (j = 1; j <= 24; j++) {
            probe.y = mPos.y + 16.0f * j;
            if (probe.y >= playerPos.y) {
                break;
            }
            u16 tile = getRawTile_80226850(&probe, mLayer_0x4);
            if (tile >= 0x3E7 || (tile >= 0x250 && tile <= 0x26F)) {
                blocked = true;
                break;
            }
        }
        return !blocked;
    }
    return false;
}
#pragma auto_inline reset

// Its literals 0.1f, 0.5f and 9.8f lead the unit's .sdata2 pool, ahead of
// fn_8010B284's
// (docs/RULES_CARD.md, literals before their first visible use (3)).
// FAKEMATCH(crack-pass): pool helper failing R2 (1 call site, 4-line definition): split out only where the .sdata2 order needs it.
void Icicle_8049CFC8::fall() {
    field_0x23c += 0.1f;
    mPos.y = mSpawnPos.y + 0.5f * (9.8f * square(field_0x23c));
}
