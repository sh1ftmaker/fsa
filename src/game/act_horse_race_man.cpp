#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_horse_race_man.h"

static const s16 lbl_80469C88[6] = {0, 20, 1, 15, -1, -1};
static const s16 lbl_80469C94[6] = {2, 20, 3, 15, -1, -1};
static const s16 lbl_80469CA0[6] = {4, 20, 5, 15, -1, -1};
static const s16 lbl_80469CAC[6] = {6, 20, 7, 15, -1, -1};
static const s32 lbl_80469CB8[4] = {0x366, 0x367, 0x368, 0x369};

static const s16* lbl_804A5620[4] = {
    lbl_80469C88, lbl_80469C94, lbl_80469CA0, lbl_80469CAC,
};

HorseRaceMan_804A5630::HorseRaceMan_804A5630() {
    field_0x2a8 = gGameRoot_8053AAF8->fn_80138A30();
    field_0x2ac = 0;
    field_0x2b0 = -1;
    field_0x2b4 = 0;
    mCountdown.fn_801ED974();
    fn_80216178(&mTable);
    lbl_8053A160 = -1;
}

HorseRaceMan_804A5630::~HorseRaceMan_804A5630() {
    lbl_8053A160 = -1;
}

void HorseRaceMan_804A5630::init() {
    EnemyBase_804A4808::init();
    field_0x2ac = (field_0x90 >> 22) & 0x1F;
    mBox.set(1.0f, 1.0f, -1.0f, -1.0f);
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.set(1.0f, 1.0f, -1.0f, -1.0f);
    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_HURT | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mLife = 1000;
    mLifeBackup = mLife;
    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;
    mAnim.fn_801D2608(lbl_804A5620, 4);
    mAnim.startAnim_801D22DC(field_0x98, 2, false);
}

void HorseRaceMan_804A5630::vfunc_0x94() {
    mAnim.fn_801D2264(field_0x98, 2);
    EnemyBase_804A4808::vfunc_0x94();
}

void HorseRaceMan_804A5630::vfunc_0x9C(u8 mapHit) {
    mCountdown.fn_801ED8F0();
    fn_80215F30(&mTable);
    EnemyBase_804A4808::vfunc_0x9C(mapHit);
    JGeometry::TBox2f box(mPos.x - 10.0f, mPos.y - 10.0f, mPos.x + 10.0f, mPos.y + 10.0f);
    fn_80239914(&box, field_0x4, mSerial);
    s32 serial = mSerial;
    s32 layer = field_0x4;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    boxes->mLists[boxes->field_0x0].fn_8020D8C0(&box, layer, serial);
    lbl_8053A160 = fn_80215A94(&mTable);
}

void HorseRaceMan_804A5630::draw() {
    if (mSpriteParams.mMtx == NULL) {
        fn_801EF830(field_0x4, &mPos, NULL, 0, "kage_sob.bin", 0, NULL);
    }
    s32 frame = mAnim.mCel + 0x319;
    if (field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, NULL, 1.0f, 1.0f);
    }
    mCountdown.fn_801ED828();
    if (mState >= 17 || mState < 7) {
        return;
    }
    mTable.fn_80215ECC();
}
