#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_life_meter.h"
#include "game/act/act_princess_zelda.h"
#include "JSystem/JMath/JMath.h"

static u8 lbl_8053AA80;
static u8 lbl_8053AA81;

// .rodata
static const s16 lbl_80464060[] = {
    0, 3, 1, 3, 2, 3, 3, 3, 4, 3, 5, 3, 6, 3, 7, 3, -1, -1
};
static const s16 lbl_80464084[] = {
    8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14, 3, 15, 3, -1, -1
};
static const s16 lbl_804640A8[] = {
    16, 3, 17, 3, 18, 3, 19, 3, 20, 3, 21, 3, 22, 3, 23, 3, -1, -1
};
static const s16 lbl_804640CC[] = {
    24, 3, 25, 3, 26, 3, 27, 3, 28, 3, 29, 3, 30, 3, 31, 3, -1, -1
};
static const s16 lbl_804640F0[] = {
    16, 6, 8, 6, 0, 6, 24, 6, 16, 6, 8, 6, 0, 6, 24, 6, 32, 3, 33, 5, 34, 6, 35, 7, 36, 30, -1, -1
};

// .sdata2 small tables:
static const s16 lbl_8053C380[] = { 0, 3, -1, -1 };
static const s16 lbl_8053C388[] = { 8, 3, -1, -1 };
static const s16 lbl_8053C390[] = { 16, 3, -1, -1 };
static const s16 lbl_8053C398[] = { 24, 3, -1, -1 };

static const s16 lbl_8053C3A0[] = { 0, 3, -1, -1 };
static const s16 lbl_8053C3A8[] = { 8, 3, -1, -1 };
static const s16 lbl_8053C3B0[] = { 16, 3, -1, -1 };
static const s16 lbl_8053C3B8[] = { 24, 3, -1, -1 };

static const s16 lbl_80464128[] = {
    0, 20, 8, 20, 0, 20, 8, 20, 16, 3, -1, -1
};
static const s16 lbl_80464140[] = {
    1, 10, 0, 7, 2, 10, 0, 7, -1, -1
};
static const s16 lbl_80464154[] = {
    4, 10, 3, 7, 5, 10, 3, 7, -1, -1
};
static const s16 lbl_80464168[] = {
    7, 10, 6, 7, 8, 10, 6, 7, -1, -1
};
static const s16 lbl_8046417C[] = {
    10, 10, 9, 7, 11, 10, 9, 7, -1, -1
};

static const u32 lbl_80464204[] = {
    'PEAC', 'OKTA', 'AMOS', 'BZBR', 'OKTM', 'DGTL', 'HINO', 'HBMB', 'SLR1', 'TKTA',
    'SLR2', 'KETH', 'FLME', 'TKYU', 'TRAP', 'ARRW', 'Poon', 'SKUL', 'SLR3', 'POPO',
    'SBAL', 'SLR4', 'FIRE', 'ROPE', 'NZMI', 'OIWA', 'PAME', 'STAL', 'MIRA', 'WIZR',
    'ZORA', 'ZOSH', 'BMTY', 'PUKU', 'RIVR', 'GDMN', 'FAGN', 'TNDR', 'WNWN', 'FLYO',
    'LTZR', 'BEEE', 'NULL'
};

static const u16 lbl_804642B0[8] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x28, 0x29, 0x2A, 0x2B
};

static const char lbl_804642C0[] = "enemy_both2_sob.bin";

// .data tables of pointers
static const s16* lbl_8049CE70[14] = {
    lbl_80464060, lbl_80464084, lbl_804640A8, lbl_804640CC, lbl_804640F0,
    lbl_8053C380, lbl_8053C388, lbl_8053C390, lbl_8053C398,
    lbl_80464128, lbl_80464140, lbl_80464154, lbl_80464168, lbl_8046417C,
};

static const s16* lbl_8049CEA8[14] = {
    lbl_80464060, lbl_80464084, lbl_804640A8, lbl_804640CC, lbl_804640F0,
    lbl_8053C3A0, lbl_8053C3A8, lbl_8053C3B0, lbl_8053C3B8,
    lbl_80464128, lbl_80464140, lbl_80464154, lbl_80464168, lbl_8046417C,
};

cls_8049CEE0::cls_8049CEE0()
    : field_0x2f8(0),
      field_0x2fc(0),
      field_0x300(-1),
      field_0x304(-1),
      field_0x308(0),
      field_0x30c(0),
      field_0x310(-1),
      field_0x314(0),
      field_0x31c(-1),
      field_0x320(0),
      field_0x324(0),
      field_0x328(0),
      field_0x32c(0),
      field_0x330(1.0f),
      field_0x334(0.0f),
      field_0x338(1),
      field_0x339(1) {}

cls_8049CEE0::~cls_8049CEE0() {
    lbl_8053AA81 = 0;
}

void cls_8049CEE0::init() {
    ActorBase_804A5770::init();
    field_0x320 = field_0x90 & 0x1F;
    field_0x324 = (field_0x90 >> 4) & 0x1F;
    field_0x2fc = (field_0x90 >> 8) & 0xFF;
    mBox.i.x = -16.0f;
    mBox.i.y = -16.0f;
    mBox.f.x = 16.0f;
    mBox.f.y = 16.0f;
    mBox2.i.x = -16.0f;
    mBox2.i.y = -16.0f;
    mBox2.f.x = 16.0f;
    mBox2.f.y = 16.0f;
    field_0x80.i.x = -16.0f;
    field_0x80.i.y = -16.0f;
    field_0x80.f.x = 16.0f;
    field_0x80.f.y = 16.0f;
    mLife = 4;
    field_0xb8 = 2;
    field_0x11e = 1;
    if (field_0x320 == 0) {
        mSpriteAnim.fn_801D2608(lbl_8049CEA8, 14);
    } else {
        mSpriteAnim.fn_801D2608(lbl_8049CE70, 14);
    }
    mSpriteAnim.startAnim_801D22DC(2, 2, 0);
    mSpriteAnim.fn_801D1F14(0.0f);
    getGameView_802D6A9C()->fn_802D7440(0);
    field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', field_0x4, &mPos, 0, -1, -1);
}

static inline bool checkTile(u16 code) {
    for (int i = 0; i < 8; i++) {
        if (code == lbl_804642B0[i]) {
            return true;
        }
    }
    return false;
}

inline bool cls_8049CEE0::stepRail() {
    if (field_0x2fc <= 0) {
        return true;
    }
    s16 angle = 0;
    JGeometry::TVec3f moveVec(0.0f, 0.0f, 0.0f);
    s32 layer = field_0x4;
    JGeometry::TVec3f pos = mPos;
    if (fn_800FDEA0(&mRail, &pos, 1.0f, &moveVec, &layer, (u16*)&angle, 0x200) == 3) {
        return true;
    }
    field_0x3c.x = moveVec.x;
    field_0x3c.y = moveVec.y;
    field_0x3c.z = moveVec.z;
    mPos += field_0x3c;
    s32 dir = fn_801F681C((u16)angle);
    if (dir >= 0 && dir != mSpriteAnim.mAnim) {
        mSpriteAnim.startAnim_801D22DC(dir, 2, false);
        mSpriteAnim.fn_801D1F14(0.0f);
    }
    return false;
}

// FIXME: 93.84% (missing/extra + reloc-name: ours has 3 fewer instrs: 920 vs 923).
void cls_8049CEE0::update() {
    ActorBase_804A5770::update();
    field_0x330 = 1.0f;
    field_0x339 = 1;

    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;

    bool canHit = true;
    if (field_0x328 >= 8 || field_0x328 <= 4 || field_0x2f8 != 0) {
        canHit = false;
    }
    if (canHit) {
        if (fn_801F26CC(lbl_80464204) >= 0) {
            mLife -= 2;
            if (mLife > 0) {
                fn_801096C8(11);
            }
        }
    }

    if (field_0x328 >= 5 && field_0x328 <= 7) {
        JGeometry::TBox2f searchBox(mBox);
        searchBox.i.x += mPos.x;
        searchBox.f.x += mPos.x;
        searchBox.i.y += mPos.y;
        searchBox.f.y += mPos.y;

        fn_8022FEB8(&searchBox, field_0x4, mSerial, false, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player >= 0) {
            if (!(gGameRoot_8053AAF8->mPlayerMask & (1 << player))) {
                player = -1;
            } else if (!fn_8022C5EC(player, 0x120)) {
                player = -1;
            } else if (!fn_8038AD24(player, 0x170, mSerial, field_0x4, mPos, 0x20)) {
                player = -1;
            }
        } else {
            player = -1;
        }
        if (player >= 0) {
            field_0x304 = player;
            fn_801096C8(3);
        }
    }

    if (field_0x2f8 != 0) {
        field_0x30c++;
        if (field_0x30c >= 300) {
            field_0x2f8 = 0;
            field_0x30c = 0;
        }
    }

    switch (field_0x328) {
    case 0:
        if (field_0x2fc > 0 && field_0x320 == 1) {
            mRail.fn_800FE3EC(field_0x2fc, mPos, field_0x4);
        }
        fn_801096C8(1);
        break;

    case 1:
        if (lbl_8053AA81 != 0) {
            if (field_0x328 == 1) {
                fn_8022FE80(0, field_0x4, mSerial, false, false);
                if (fn_8022FD8C(mSerial) >= 0) {
                    if (fn_8038AD24(0, 0x170, mSerial, field_0x4, mPos, 0x28)) {
                        lbl_8053AA81 = 0;
                        fn_801096C8(4);
                    }
                }
            }
        } else {
            u16 angle;
            s32 dir;
            f32 dist;
            s32 unk;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &unk, 0, 1);
            if (dir >= 0 && dist <= 64.0f) {
                if (field_0x328 == 1) {
                    if (dir >= 0 && dir + 5 != mSpriteAnim.mAnim) {
                        mSpriteAnim.startAnim_801D22DC(dir + 5, 2, false);
                        mSpriteAnim.fn_801D1F14(0.0f);
                    }
                }
            }
            switch (field_0x320) {
            case 0: {
                fn_8022FEB8(&box, field_0x4, mSerial, true, false);
                s32 player = fn_8022FD8C(mSerial);
                if (player >= 0 && !fn_8022F514(player)) {
                    if (fn_8038AD24(player, 0x230, mSerial, field_0x4, mPos, 0x28)) {
                        field_0x300 = player;
                        fn_801096C8(2);
                    }
                }
                break;
            }
            }
        }
        break;

    case 2:
        fn_8022FF8C(field_0x300, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x300, mSerial)) {
            fn_801096C8(5);
        }
        break;

    case 3:
        fn_8022FF8C(field_0x304, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x304, mSerial)) {
            fn_801096C8(field_0x32c);
        }
        break;

    case 4: {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        camera->addFocusPoint_8030C210(&mPos);
        camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x840 == 0) {
            camera->field_0x840 = 1;
            camera->field_0x844.f.x = mPos.x;
            camera->field_0x844.i.x = mPos.x;
            camera->field_0x844.f.y = mPos.y;
            camera->field_0x844.i.y = mPos.y;
            camera->field_0x854 = 0.08f;
            camera->field_0x1d4 = -1.0f;
            camera->field_0x1d8 = -1.0f;
        }
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (fn_80388CAC(0, mSerial)) {
            lbl_8053AA81 = 0;
            fn_801096C8(1);
        }
        break;
    }

    case 5: {
        if (field_0x300 < 0) {
            fn_801096C8(1);
            break;
        }
        if (vfunc_0x34()) {
            fn_801096C8(15);
            break;
        }
        s32 player = field_0x300;
        if (player < 0) {
            break;
        }
        if (getGameView_802D6A9C()->fn_802DCD0C()) {
            fn_801096C8(9);
            break;
        }
        if (fn_8022D75C(player)) {
            if (fn_8023E10C(player) == 8) {
                fn_801096C8(7);
            } else {
                field_0x4 = getPlayerLayer_8023DE58(player);
                field_0x339 = 0;
                fn_801096C8(10);
            }
            break;
        }
        u16 angle = 0;
        s32 dir = 0;
        f32 dist = 0.0f;
        s32 unk = 0;
        fn_801EEC98(mSerial, &angle, &dir, &dist, &unk, player, 2);
        if (dist >= 32.0f) {
            fn_801CD950(&field_0x334, 1.1f, 0.02f);
        } else {
            fn_801CD950(&field_0x334, 0.0f, 0.04f);
        }
        field_0x3c.x = field_0x334 * JMath::sincosTable_.sinShort(angle);
        field_0x3c.y = field_0x334 * JMath::sincosTable_.cosShort(angle);
        mPos += field_0x3c;
        field_0x330 = magnitude(field_0x3c) / 1.1f;
        if (field_0x330 >= 1.0f) {
            field_0x330 = 1.0f;
        }
        if (dir >= 0 && dir != mSpriteAnim.mAnim) {
            mSpriteAnim.startAnim_801D22DC(dir, 2, false);
            mSpriteAnim.fn_801D1F14(0.0f);
        }
        break;
    }

    case 6:
        break;

    case 7:
        if (field_0x300 < 0) {
            fn_801096C8(1);
        }
        if (!fn_8022D75C(field_0x300) && field_0x4 == getPlayerLayer_8023DE58(field_0x300)) {
            fn_801096C8(8);
        }
        break;

    case 8:
        if (field_0x300 < 0) {
            fn_801096C8(1);
        }
        field_0x314++;
        if (field_0x314 >= 15) {
            field_0x314 = 0;
            fn_801096C8(5);
        }
        break;

    case 9:
        if (field_0x300 >= 0) {
            field_0x339 = 0;
            JGeometry::TVec3f ppos = *getPlayerPos_8023E724(field_0x300);
            field_0x98 = fn_80230CFC(field_0x300);
            switch (field_0x98) {
            case 0:
                ppos.x -= 16.0f;
                break;
            case 1:
                ppos.x += 16.0f;
                break;
            case 2:
                ppos.y -= 16.0f;
                break;
            case 3:
                ppos.y += 16.0f;
                break;
            }
            field_0x330 = 0.5f;
            fn_801CD950(&mPos.x, ppos.x, 2.0f);
            fn_801CD950(&mPos.y, ppos.y, 2.0f);
        }
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            fn_801096C8(5);
        }
        break;

    case 10:
        field_0x4 = getPlayerLayer_8023DE58(field_0x300);
        if (field_0x300 >= 0) {
            field_0x339 = 0;
            JGeometry::TVec3f ppos = *getPlayerPos_8023E724(field_0x300);
            field_0x98 = fn_80230CFC(field_0x300);
            switch (field_0x98) {
            case 0:
                ppos.x -= 16.0f;
                break;
            case 1:
                ppos.x += 16.0f;
                break;
            case 2:
                ppos.y -= 16.0f;
                break;
            case 3:
                ppos.y += 16.0f;
                break;
            }
            field_0x330 = 0.5f;
            s32 layer = fn_8023E10C(field_0x300);
            if (layer == getPlayerLayer_8023DE58(field_0x300)) {
                mPos.x = ppos.x;
                mPos.y = ppos.y;
                mPos.z = ppos.z;
            } else {
                fn_801CD950(&mPos.x, ppos.x, 5.0f);
                fn_801CD950(&mPos.y, ppos.y, 5.0f);
            }
        }
        if (!fn_8022D75C(field_0x300)) {
            fn_801096C8(5);
        }
        break;

    case 11:
        if (mSpriteAnim.fn_801D208C()) {
            fn_801096C8(field_0x32c);
        }
        break;

    case 12:
        if (mSpriteAnim.fn_801D208C()) {
            field_0x308++;
            if (field_0x308 >= 90) {
                fn_801096C8(13);
            }
        }
        break;

    case 13:
        mSpriteAnim.fn_801D1C18();
        if (lbl_8053AA80 != 0) {
            fn_801096C8(14);
        }
        break;

    case 14:
        if (field_0x300 < 0) {
            fn_801096C8(1);
        } else {
            fn_801096C8(5);
        }
        break;

    case 15:
        field_0x318++;
        if (field_0x318 < 15) {
            if (stepRail()) {
                fn_801096C8(16);
            }
        }
        break;

    case 16:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x31c = fn_8022FD8C(mSerial);
        if (field_0x31c >= 0 && (gGameRoot_8053AAF8->mPlayerMask & (1 << field_0x31c))) {
            if (fn_8038AD24(field_0x31c, 0x16E, mSerial, field_0x4, mPos, 0x28)) {
                fn_801096C8(17);
            }
        }
        break;

    case 17:
        fn_8022FF8C(field_0x31c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x31c, mSerial)) {
            fn_801096C8(16);
        }
        break;
    }

    if (field_0x338 != 0) {
        fn_80239914(&box, field_0x4, mSerial);
    }

    if (field_0x339 != 0) {
        if (!fn_8022C52C(field_0x300, 0x400)) {
            if (!checkTile(fn_8022461C(&mPos, field_0x4))) {
                JGeometry::TVec3f pos;
                fn_801F2618(&pos, false);
            }
        }
    }

    JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
    if (fn_80109AE0(&push)) {
        mPos += push;
    }

    mSpriteAnim.mFrameCtrl.setRate(field_0x330);
    mSpriteAnim.fn_801D1C18();
    fn_80109534();
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);

    if (mLife == 0 && field_0x328 < 12 && field_0x2f8 == 0) {
        fn_801096C8(12);
    }
}

void cls_8049CEE0::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    if (field_0x2f8 != 0) {
        params.field_0x10.a = 255.0f * JMath::sincosTable_.sinDegree(field_0x30c * 60);
    }
    if (field_0x320 == 0) {
        fn_801F02BC(lbl_804642C0, mSpriteAnim.mCel + 0x218, &params, 0x407, 0, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC(lbl_804642C0, mSpriteAnim.mCel + 0x9b, &params, 0x407, 0, false, 1.0f, 1.0f);
    }
}

static inline bool overlaps(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    overlap.i.setMax(b.i);
    overlap.f.setMin(b.f);
    bool result = false;
    if (overlap.f.x >= overlap.i.x && overlap.f.y >= overlap.i.y) {
        result = true;
    }
    return result;
}

bool cls_8049CEE0::fn_80109AE0(JGeometry::TVec3f* outPos) {
    outPos->zero();
    s32 serial = getActorManager_801F856C()->fn_801F7A08('CIRC');
    if (serial < 0) {
        return false;
    }
    ActorBase_804A5770* circle = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (circle == NULL) {
        return false;
    }
    if (!circle->mAlive) {
        return false;
    }
    if (field_0x4 != circle->field_0x4) {
        return false;
    }
    JGeometry::TBox2f box1;
    fn_801F31C4(&box1);
    JGeometry::TBox2f box2;
    circle->fn_801F31C4(&box2);
    box2.i.x += -16.0f;
    box2.i.y += -16.0f;
    box2.f.x += 16.0f;
    box2.f.y += 16.0f;
    if (overlaps(box1, box2)) {
        JGeometry::TVec3f cpos = circle->mPos;
        outPos->set(mPos.x - cpos.x, mPos.y - cpos.y, 0.0f);
        outPos->normalize();
        outPos->scale(1.6f);
        return true;
    }
    return false;
}

// FIXME: 99.49% (missing/extra + reloc-name: ours has 1 more instrs (263 vs 262)).
bool cls_8049CEE0::fn_801096C8(s32 state) {
    field_0x32c = field_0x328;
    field_0x328 = state;
    field_0x338 = 0;
    switch (state) {
    case 0:
        return false;
    case 1:
        field_0x338 = 1;
        mSpriteAnim.startAnim_801D22DC(7, 2, false);
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5: {
        s32 viewId = field_0x16c;
        if (viewId != -1) {
            getGameView_802D6A9C()->fn_802D7704(viewId, field_0x1a8, field_0x4);
        }
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    }
    case 6:
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    case 7: {
        JGeometry::TVec3f pos = mPos;
        pos.y += -24.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
        mSpriteAnim.startAnim_801D22DC(9, 0, false);
        break;
    }
    case 8: {
        JGeometry::TVec3f pos = mPos;
        pos.y += -24.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        break;
    }
    case 9:
        mSpriteAnim.startAnim_801D22DC(fn_80230CFC(field_0x300), 2, false);
        break;
    case 10:
        mSpriteAnim.startAnim_801D22DC(fn_80230CFC(field_0x300), 2, false);
        break;
    case 11:
        mSpriteAnim.startAnim_801D22DC(field_0x98 + 10, 0, false);
        field_0x2f8 = 1;
        break;
    case 12:
        mSpriteAnim.startAnim_801D22DC(4, 0, false);
        gAudioMgr_8053AF80->fn_80458880(10, (s8)field_0x300);
        for (s32 i = 0; i < 4; i++) {
            fn_80230AD4(i);
        }
        break;
    case 13: {
        for (s32 i = 0; i < 4; i++) {
            fn_80230224(i, 0);
            if (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) {
                fn_8023764C(i, 0);
            }
        }
        s32 limtSerial = field_0x310;
        if (limtSerial >= 0) {
            ActorBase_804A5770* limt = getActorManager_801F856C()->findActorBySerial_801F666C(limtSerial);
            if (limt != NULL && limt->mActorId == 'LIMT') {
                limt->mAlive = false;
            }
        }
        field_0x308 = 0;
        field_0x310 = -1;
        return true;
    }
    case 14:
        mSpriteAnim.startAnim_801D22DC(2, 2, false);
        lbl_8053AA80 = 0;
        mLife = 4;
        field_0x2f8 = 1;
        field_0x30c = 0;
        field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', field_0x4, &mPos, 0, -1, -1);
        break;
    case 15:
        if (field_0x2fc > 0 && field_0x320 == 0) {
            mRail.fn_800FE3EC(field_0x2fc, mPos, field_0x4);
        }
        fn_8023077C();
        {
            JGeometry::TVec3f pos = mPos;
            pos.y += -24.0f;
            spawnEmitter_8013CC50(field_0x4, &pos, 0x33A, 0, 0, NULL, NULL, true, 1.0f);
        }
        break;
    case 16:
        fn_80230534(false);
        mSpriteAnim.startAnim_801D22DC(7, 2, false);
        field_0x338 = 1;
        field_0x318 = 0;
        break;
    case 17:
        field_0x338 = 1;
        break;
    default:
        return false;
    }
    mSpriteAnim.fn_801D1F14(0.0f);
    return true;
}

static inline void setLifeMeter(LifeMeter_804B174C* meter, s32 serial, f32 yOffset, s32 index) {
    meter->field_0x250 = serial;
    meter->field_0x254 = yOffset;
    meter->field_0x24c = index;
}

void cls_8049CEE0::fn_80109534() {
    if (field_0x310 < 0) {
        field_0x310 = getActorManager_801F856C()->spawnActor_801F9484('LIMT', field_0x4, &mPos, 0, -1, -1);
        if (field_0x310 < 0) {
            return;
        }
    }
    LifeMeter_804B174C* meter =
        static_cast<LifeMeter_804B174C*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x310));
    if (meter == NULL) {
        field_0x310 = -1;
        return;
    }
    if (field_0x2f8 != 0 || field_0x328 == 12) {
        meter->mDrawEnabled = true;
    } else {
        meter->mDrawEnabled = false;
    }

    switch (field_0x328) {
    case 12:
        if (mSpriteAnim.mKey < 8) {
            setLifeMeter(meter, mSerial, -28.0f, mLife + 2);
        } else if (mSpriteAnim.mKey < 12) {
            setLifeMeter(meter, mSerial, -28.0f, mLife);
        } else {
            meter->mDrawEnabled = false;
        }
        break;
    default: {
        s32 life = mLife;
        if (life == 0) {
            return;
        }
        if (life == 4) {
            setLifeMeter(meter, mSerial, -28.0f, life);
        } else if (field_0x30c < 45 && field_0x2f8 != 0) {
            setLifeMeter(meter, mSerial, -28.0f, life + 2);
        } else {
            setLifeMeter(meter, mSerial, -28.0f, life);
        }
        break;
    }
    }
}
