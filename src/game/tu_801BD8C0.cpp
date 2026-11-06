#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_801BD8C0.h"
#include "game/DrawShadow.h"

static const u16 lbl_8053EC88[2] = {0x19E, 0x19C};

cls_804A4038::cls_804A4038() {
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
}

cls_804A4038::~cls_804A4038() {
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

void cls_804A4038::init() {
    EnemyBase_804A4808::init();
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox2.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    field_0x194 = 1;
    field_0x114 = 2;
    for (s32 i = 0; i < 2; i++) {
        mEmitters[i] = spawnEmitter_8013CC50(field_0x4, &mPos, lbl_8053EC88[i], 0, 0, NULL, NULL, true, 1.0f);
    }
}

void cls_804A4038::update() {
    mPlayersHit = fn_801F2B7C(field_0x114, -1, 1);
    if (field_0x90 == 0) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x54D, 1, 1, NULL, NULL, true, 1.0f);
    }
}

void cls_804A4038::draw() {
    drawShadow(field_0x4, &mPos, JUtility::TColor(0, 0, 0, 0x80), 32.0f);
}

void cls_804A4038::fn_801BD8C0(const JGeometry::TVec3f* pos, s16 angle) {
    mPos.x = pos->x;
    mPos.y = pos->y;
    mPos.z = pos->z;
    s16 yaw = fn_801CB7CC(angle);
    for (s32 i = 0; i < 2; i++) {
        setEmitterPos_8013CB44(mEmitters[i], pos, 0, 0, true);
        if (i == 0) {
            JPAGetXYZRotateMtx(0, yaw, 0, mEmitters[i]->mGlobalRot);
        }
    }
}
