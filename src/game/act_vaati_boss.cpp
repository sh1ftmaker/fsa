#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Camera.h"
#include "game/GameView.h"
#include "game/ViewLayer.h"
#include "game/Map.h"
#include "game/act/act_vaati_boss.h"
#include "JSystem/JKernel/JKRArchive.h"

// Shared constants (declared in GameGlobals.h): game/act_vaati_boss_801EDBA4.cpp passes the first to
// fn_802F5FF4 and multiplies -12.0f by the second at run time. No function of this unit
// reads them. They lead this unit's .sdata2 (0x8053F0B8, before 0x8053F0C0 / 0x8053F0C4
// and the function literals): under deferred codegen file-scope constants are written
// before every literal of the file, so they cannot end game/act_shadow_link_2_801D3CBC.cpp's pool.
const f32 lbl_8053F0B8 = 90.0f;
const f32 lbl_8053F0BC = 1.7f;
// 0x8053F0C0 / 0x8053F0C4: the words 12 and 0x1E; game/act_vaati_boss_801E3618.cpp reads them (lwz at 0x801E3EB4, lbz at 0x801E3D7C).
const s32 lbl_8053F0C0 = 12;
const u8 lbl_8053F0C4 = 0x1E;

static const s16 lbl_80469498[] = {
    0, 10,
    1, 10,
    2, 10,
    3, 10,
    -1, -1,
};

static const s16 lbl_804694AC[] = {
    4, 4,
    5, 4,
    6, 4,
    7, 4,
    -1, -1,
};

static const s16* lbl_8053A138[] = {
    lbl_80469498,
    lbl_804694AC,
};

cls_801D6A88::cls_801D6A88() {
    field_0xc.fn_8021BD18(2);
    fn_801D789C();
}

cls_801D6A58::cls_801D6A58() {
    fn_801D7874();
}

cls_801E6AB8::~cls_801E6AB8() {}

VaatiBoss_804A4958::VaatiBoss_804A4958() {
    field_0x5b0 = -1;
    field_0x5b4 = 0;
    field_0x5b8 = 0;
    field_0x5bc.x = 0.0f;
    field_0x5bc.y = 0.0f;
    field_0x5c4.x = 0.0f;
    field_0x5c4.y = 0.0f;
    field_0x5cc = 0;
    field_0x5cd = 0;
    field_0x5ce = 0;
    field_0x5cf = 0;
    field_0x5d0 = 0;
    field_0x5d1 = 0;
    field_0x5d2 = 0;
    field_0x5d4 = 0;
    field_0x5d8 = 0;
    field_0x5dc = 0;
    field_0x5e0.x = 0.0f;
    field_0x5e0.y = 0.0f;
    field_0x5e8 = 0;
    field_0x5e9 = 0;
    field_0x5ec.x = 0.0f;
    field_0x5ec.y = 0.0f;
    field_0x5f4.x = 0.0f;
    field_0x5f4.y = 0.0f;
    field_0x5fc = 0;
    field_0x600.set(0.0f, 0.0f, 0.0f);
    field_0x60c.set(0.0f, 0.0f, 0.0f);

    int i = 0;
    for (; i < 4; i++) {
        field_0x2a8[i].fn_801D789C();
    }

    i = 0;
    for (; i < 16; i++) {
        field_0x308[i].fn_801D7874();
    }

    field_0x448.mOwner = this;
}

void VaatiBoss_804A4958::init() {
    EnemyBase_804A4808::init();
    field_0x5dc = mSpawnParam_0x90 & 0xF;
    if (field_0x5dc != 0) {
        field_0x5dc += 7;
    }

    field_0x5e0.set((f32)(((mSpawnParam_0x90 >> 24) & 0x3F) * 8) + 8.0f,
                    (f32)(((mSpawnParam_0x90 >> 16) & 0x3F) * 8) + 8.0f);
    field_0x5e8 = (mSpawnParam_0x90 >> 10) & 0x3F;
    field_0x5e9 = (mSpawnParam_0x90 >> 4) & 0x3F;

    fn_801E4790(false);
    fn_801E4758();

    field_0xb0 = 0;
    mDir_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_FLOOR | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    mParamAtcp_0x114 = 2;
    mLife = 1000;
    mLifeBackup = mLife;
    field_0x196 = 1;

    field_0x54.x = 1.7f;
    field_0x54.y = 1.7f;
    field_0x54.z = 1.7f;

    field_0x60c.x = mPos.x;
    field_0x60c.y = mPos.y;
    field_0x60c.z = mPos.z;
    mPos.z = 500.0f;

    cls_801EBA14 rec;
    rec.field_0x0 = 1;
    rec.field_0x4 = 1;
    field_0x464.field_0xc = rec;
    field_0x464.fn_801EB710(mLayer_0x4, &mPos);

    field_0x5d8 = mLayer_0x4;
    field_0x5b8 = randInt_801CD664(4);

    field_0x5bc.x = mPos.x;
    field_0x5bc.y = mPos.y;
    field_0x5ec.set(field_0x5e0.x + (f32)field_0x5e8 * 8.0f + 32.0f, field_0x5e0.y);
    field_0x5f4.set(field_0x5ec.x, field_0x5e0.y + -32.0f);

    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        field_0x5cc = 6;
    } else {
        field_0x5cc = 3;
    }

    mState = 0;
    mStateTimer = 0;
    field_0x250 = -1;
    mSpeed = 0.0f;
    mTargetSpeed = 0.0f;

    mAnim.setKeyTables_801D2608(lbl_8053A138, 2);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.setRandomFrame_801D1D60();

    getGameView_802D6A9C()->fn_802D7440(0);

    for (s32 i = 0; i < 4; i++) {
        queueGbaFile_80304C10(i, "gb_ch_obj_tr_sch5.szs", 0x6017000, 1, 0);
    }

    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_sch5.szs");
    gGameRoot2_8053AB10->field_0x20->loadSpriteImage_802F9714(res, 0, 0, 0x7000, 0);
}

void VaatiBoss_804A4958::onFirstUpdate_0x54() {
    gAudioMgr_8053AF80->stopBgm_80458F9C(60);
}

// FIXME: 97.99 %, regalloc-only; behaviour clean.
// regalloc-only + reloc-name. Register allocation of the random-velocity arithmetic.
void VaatiBoss_804A4958::fn_801D63D0() {
    for (s32 i = 0; i < 5; i++) {
        s32 serial = getActorManager_801F856C()->spawnActorSimple_801F8544('RUPY', mLayer_0x4, &mPos, 2);
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mVel_0x3c = JGeometry::TVec3f(
                    (gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 2.0f - 1.0f) * 1.2f,
                    (gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 2.0f - 1.0f) * 1.2f,
                    3.0f + gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 3.0f
                );
            }
        }
    }
}

// FIXME(shotgun): 99.73 %, regalloc-only; behaviour drift.
void VaatiBoss_804A4958::setState_801D5D84(s32 state) {
    switch (state) {
    case 1:
    case 2:
        mTargetSpeed = 0.0f;
        break;
    case 3:
        field_0x448.fn_80217CD8();
        startSound_801F0E34(0x40089, 0);
        mTargetSpeed = 0.0f;
        break;
    case 4:
        mTargetSpeed = 0.0f;
        field_0x250 = randInt_801CD664(1) + 30;
        break;
    case 5:
        fn_801E4D74();
        mTargetSpeed = 0.7f;
        if (field_0x5b0 == -1) {
            field_0x250 = randInt_801CD664(0xB5) + 0x3C;
        } else {
            field_0x250 = randInt_801CD664(0xF1) + 0x78;
        }
        break;
    case 6:
        fn_801E4D54();
        field_0x464.field_0x5c = 1;
        field_0x464.field_0x58 = 0.0f;
        mTargetSpeed = 0.0f;
        break;
    case 7:
        mTargetSpeed = 0.0f;
        break;
    case 8: {
        mLife = 0x32;
        mFlags |= 0x2000;
        mFlags &= ~0x300;
        field_0x5b0 = -1;
        field_0xb0 = 3;
        JGeometry::TBox2f box;
        fn_801E4618(&box, isUpperLayer_801CD2F8(mLayer_0x4));
        field_0x60c.set(0.5f * (box.f.x - box.i.x) + box.i.x, 0.5f * (box.f.y - box.i.y) + box.i.y, 0.0f);
        field_0x5bc.x = (box.f.x - box.i.x) * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + box.i.x;
        field_0x5bc.y = (box.f.y - box.i.y) * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + box.i.y;
        field_0x5d0 = randInt_801CD664(2) + 1;
        mTargetSpeed = 0.0f;
        break;
    }
    case 9:
        mTargetSpeed = 0.0f;
        break;
    case 10:
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            mTargetSpeed = 1.4f;
            field_0x250 = randInt_801CD664(0xB5) + 0xB4;
        } else {
            mTargetSpeed = gGameRoot_8053AAF8->field_0x7e != 0 ? 1.4f : 1.0f;
            field_0x250 = randInt_801CD664(0x1E1) + 0xF0;
        }
        break;
    case 11:
        if (field_0x4c4.field_0x44 != 4) {
            field_0x4c4.fn_801D7638();
            field_0x4c4.fn_801D76B4();
        }
        field_0x448.fn_80217A74();
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = 0.0f;
        mVel_0x3c.z = 0.0f;
        field_0x5d1 = 0;
        break;
    case 12:
        mTargetSpeed = gGameRoot_8053AAF8->field_0x7e != 0 ? 1.4f : 1.0f;
        field_0x250 = randInt_801CD664(0x79) + 0x78;
        field_0x4c4.fn_801D74E0(&mPos, mLayer_0x4);
        break;
    case 13:
        mTargetSpeed = 0.0f;
        if (field_0x4c4.field_0x44 == 3) {
            field_0x4c4.field_0x58 = 1;
        }
        field_0x5cf = 0;
        break;
    case 14:
        mTargetSpeed = 0.0f;
        break;
    case 15:
        mTargetSpeed = 0.0f;
        field_0x5cf = 0;
        field_0x4c4.fn_801D74E0(&mPos, mLayer_0x4);
        startSound_801F0E34(0x40068, 0);
        break;
    case 16:
        mTargetSpeed = 0.0f;
        break;
    case 17:
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        break;
    case 18:
        mTargetSpeed = 0.0f;
        startSound_801F0E34(0x40061, 0);
        break;
    case 19:
        mTargetSpeed = 7.0f;
        mAnim.startAnim_801D22DC(1, 2, false);
        startSound_801F0E34(0x40062, 0);
        break;
    case 20:
        mTargetSpeed = 0.0f;
        mAnim.startAnim_801D22DC(0, 2, false);
        break;
    case 23: {
        field_0x5fc = field_0x5dc;
        field_0x600.z = 0.0f;
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            field_0x5fc = field_0x5d8;
            field_0x600.z = 31.9f;
        }
        JGeometry::TBox2f box;
        fn_801E4618(&box, isUpperLayer_801CD2F8(field_0x5fc));
        field_0x600.x = (box.f.x - box.i.x) * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + box.i.x;
        field_0x600.y = (box.f.y - box.i.y) * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + box.i.y;
        fn_801E44CC(&box, &field_0x600, NULL);
        mSpeed = 0.0f;
        mTargetSpeed = 0.0f;
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = 0.0f;
        mVel_0x3c.z = 0.0f;
        field_0x5d1 = 0;
        if (!isUpperLayer_801CD2F8(mLayer_0x4)) {
            field_0x448.fn_80217A74();
            field_0x448.fn_80217DE8();
            field_0x448.fn_80217BC8();
            startSound_801F0E34(0x5003E, 0);
        } else {
            field_0x448.fn_80217AB8();
            fn_801CDCD0(field_0x5fc, &field_0x60c, 0x5003D);
        }
        break;
    }
    case 24:
        mBox.i.x = 1.0f;
        mBox.i.y = 1.0f;
        mBox.f.x = -1.0f;
        mBox.f.y = -1.0f;
        mBox2.i.x = mBox.i.x;
        mBox2.i.y = mBox.i.y;
        mBox2.f.x = mBox.f.x;
        mBox2.f.y = mBox.f.y;
        mMapBox_0x80.i.x = 1.0f;
        mMapBox_0x80.i.y = 1.0f;
        mMapBox_0x80.f.x = -1.0f;
        mMapBox_0x80.f.y = -1.0f;
        mTargetSpeed = 0.0f;
        mSpeed = 0.0f;
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = 0.0f;
        mVel_0x3c.z = 0.0f;
        field_0x5d1 = 0;
        break;
    case 25:
        startSound_801F0E34(0x4008A, 0);
        field_0x448.fn_80217ED4();
        break;
    case 26:
        field_0x568.fn_801E6A84(0);
        markDead_801F3668();
        mAlive = true;
        break;
    }
    mStateTimer = 0;
    mState = state;
}

inline bool VaatiBoss_804A4958::calcTargetFar_801D5B70(JGeometry::TVec2f* out) {
    bool found = false;
    s32 player = findNearestPlayer_801D05F8(&mPos, 160.0f);
    if (player >= 0) {
        const JGeometry::TVec3f* p = getPlayerPos_8023E724(player);
        JGeometry::TVec2f target;
        target.x = p->x;
        target.y = p->y;
        found = true;
        if (out != NULL) {
            JGeometry::TVec2f d(target.x - mPos.x, target.y - mPos.y);
            if (d.squared() <= JGeometry::TUtilf::epsilon()) {
                d.set(1.0f, 0.0f);
            }
            d.normalize();
            d.x *= 10000.0f;
            d.y *= 10000.0f;
            out->x = mPos.x;
            out->y = mPos.y;
            out->x += d.x;
            out->y += d.y;
        }
    }
    return found;
}

// Not auto-inlined: updateState_0x90 calls fn_801D5B70 out of line in the original.
#pragma auto_inline off
void VaatiBoss_804A4958::fn_801D5B70() {
    if (mStateTimer >= field_0x250) {
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            if (field_0x5d0 == 0) {
                setState_801D5D84(0x15);
            } else {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.6f) {
                    if (calcTargetFar_801D5B70(&field_0x5c4)) {
                        setState_801D5D84(0x11);
                        return;
                    }
                }
                setState_801D5D84(0xF);
            }
        } else if (field_0x5d0 == 0) {
            if (mLife <= 12) {
                setState_801D5D84(0xC);
            } else {
                setState_801D5D84(0x17);
            }
        } else {
            setState_801D5D84(0xC);
        }
    }
}
#pragma auto_inline reset

// Collects the players (other than the current target) that are in play and not
// flagged in field_0x2a8, and picks one at random; -1 when there is none.
static inline s32 pickTarget(VaatiBoss_804A4958* boss) {
    s32 chosen = -1;
    s32 found[4];
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i != boss->field_0x5b0 && !boss->field_0x2a8[i].isSet() && boss->canTargetPlayer_801D0B44(i)) {
            found[count] = i;
            count++;
        }
    }
    if (count > 0) {
        chosen = found[randInt_801CD664(count)];
    }
    return chosen;
}

// FIXME(shotgun): 99.72 %, regalloc-only; behaviour drift.
void VaatiBoss_804A4958::updateState_0x90(u8 mapHit) {
    switch (mState) {
    case 0:
        if (fn_801CD228(mLayer_0x4) && getActorManager_801F856C()->fn_801F68E0() != 0) {
            setState_801D5D84(1);
        }
        break;
    case 1:
        if (mPos.z <= 90.0f) {
            mPos.z = 90.0f;
            setState_801D5D84(2);
        }
        break;
    case 2:
        if (mStateTimer >= 180) {
            setState_801D5D84(3);
        }
        break;
    case 3:
        if (mStateTimer >= 60) {
            getActorManager_801F856C()->fn_801F6884();
            gAudioMgr_8053AF80->startBgm_80458FF0(0x1000004, true, 0);
            setState_801D5D84(4);
        }
        break;
    case 4:
        if (mStateTimer >= field_0x250) {
            field_0x5b0 = pickTarget(this);
            setState_801D5D84(5);
        }
        break;
    case 5:
        if (field_0x5b0 != -1 && (field_0x2a8[field_0x5b0].isSet() || !canTargetPlayer_801D0B44(field_0x5b0))) {
            field_0x5b0 = pickTarget(this);
            setState_801D5D84(5);
        } else if (mStateTimer >= field_0x250) {
            s32 chosen = pickTarget(this);
            if (field_0x5b0 == -1 && chosen == -1) {
                break;
            }
            field_0x5b0 = chosen;
            setState_801D5D84(5);
        }
        break;
    case 6:
        if (mPos.z >= 500.0f) {
            mPos.z = 500.0f;
            setState_801D5D84(7);
        }
        break;
    case 7:
        if (!(mFlags & 1) && mStateTimer >= 30) {
            setState_801D5D84(8);
        }
        break;
    case 8:
        if (mPos.z <= 31.9f) {
            mPos.z = 31.9f;
            setState_801D5D84(9);
        }
        break;
    case 9:
        if (mStateTimer >= 120) {
            setState_801D5D84(10);
        }
        break;
    case 10:
        fn_801D5B70();
        break;
    case 11:
        if (mStateTimer >= 180) {
            if (isUpperLayer_801CD2F8(mLayer_0x4)) {
                setState_801D5D84(15);
            } else {
                field_0x448.fn_80217CD8();
                setState_801D5D84(12);
            }
        }
        break;
    case 12:
        if (mStateTimer >= field_0x250) {
            setState_801D5D84(13);
        }
        break;
    case 13:
        if (field_0x5cf != 0) {
            field_0x5cf--;
            if (field_0x5cf == 0) {
                field_0x4c4.fn_801D72C0(&mPos);
                startSound_801F0E34(0x40060, 0);
                setState_801D5D84(14);
            }
        } else if (mVel_0x3c.x < 0.001f && mVel_0x3c.y < 0.001f) {
            field_0x5cf = 10;
        }
        break;
    case 14:
        if (mStateTimer >= 6) {
            if (field_0x5d0 != 0 && (mFlags & 0x10000)) {
                field_0x5d0--;
            }
            setState_801D5D84(10);
        }
        break;
    case 15:
        if (field_0x5cf != 0) {
            field_0x5cf--;
            if (field_0x5cf == 0) {
                field_0x4c4.fn_801D70BC(&mPos, findNearestPlayer_801D05F8(&mPos, 0.0f));
                startSound_801F0E34(0x40069, 0);
                setState_801D5D84(16);
            }
        } else if (mVel_0x3c.x < 0.001f && mVel_0x3c.y < 0.001f) {
            field_0x5cf = 10;
        }
        break;
    case 16:
        if (mStateTimer >= 6) {
            if (field_0x5d0 != 0 && (mFlags & 0x10000)) {
                field_0x5d0--;
            }
            setState_801D5D84(10);
        }
        break;
    case 17:
        if (mStateTimer >= 15) {
            setState_801D5D84(18);
        }
        break;
    case 18:
        if (mStateTimer >= 15) {
            setState_801D5D84(19);
        }
        break;
    case 19:
        if (mStateTimer >= 50) {
            setState_801D5D84(20);
        }
        break;
    case 20:
        if (mStateTimer >= 15) {
            setState_801D5D84(10);
        }
        break;
    case 21:
        if (__fabsf(mPos.x - field_0x5ec.x) < 0.1f && __fabsf(mPos.y - field_0x5ec.y) < 0.1f) {
            setState_801D5D84(22);
        }
        break;
    case 22:
        if (__fabsf(mPos.x - field_0x5f4.x) < 0.1f && __fabsf(mPos.y - field_0x5f4.y) < 0.1f) {
            setState_801D5D84(23);
        }
        break;
    case 23:
        if (mStateTimer >= 50) {
            field_0x5b4 = 0;
            field_0x5b8 = randInt_801CD664(4);
            field_0x5bc.x = field_0x600.x;
            field_0x5bc.y = field_0x600.y;
            mLayer_0x4 = field_0x5fc;
            mPos = field_0x600;
            mPrevPos = field_0x600;
            fn_801E4790(isUpperLayer_801CD2F8(mLayer_0x4));
            if (isUpperLayer_801CD2F8(mLayer_0x4)) {
                field_0x5d0 = randInt_801CD664(4) + 3;
                field_0x448.fn_80217A30();
            } else {
                field_0x5d0 = randInt_801CD664(2) + 1;
                field_0x448.fn_802179EC();
                field_0x448.fn_80217DE8();
                field_0x448.fn_80217CD8();
            }
            setState_801D5D84(10);
        } else if (isUpperLayer_801CD2F8(field_0x5fc)) {
            field_0x448.fn_80217874((f32)(50 - mStateTimer));
        } else {
            field_0x448.fn_8021775C((f32)(50 - mStateTimer));
        }
        break;
    case 24:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            gGameRoot_8053AAF8->startRumble_80134DE4(3, 0);
            setState_801D5D84(25);
        }
        break;
    case 25:
        if (mStateTimer >= 450) {
            setState_801D5D84(26);
        } else if (mStateTimer == 200) {
            mDrawEnabled = false;
        }
        break;
    case 26:
        field_0x568.fn_801E667C();
        break;
    }
}

void VaatiBoss_804A4958::updateAnim_0x94() {
    switch (mState) {
    case 6:
    case 7:
    case 0xB:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        break;
    default:
        mAnim.update_801D1C18();
        break;
    }
}

// The step of the vtable slot 0x9C (0x801D5180, a plain `fn_801D5180` symbol in the original).
void VaatiBoss_804A4958::updateBehavior_0x9C(u8 mapHit) {
    if (!(mFlags & 0x2000)) {
        switch (mState) {
        case 1:
        case 2:
        case 3:
            startSound_801F0E34(0x4005E, 0);
            break;
        }
        if (field_0x5ce != 0) {
            field_0x5ce--;
            if (field_0x5ce == 0) {
                mFlags |= 0x4000;
            }
        }
        JGeometry::TVec3f center(mPos.x, mPos.y, 90.0f);
        field_0x464.fn_801EB418(&center);
        if (field_0x464.fn_801EB648()) {
            startSound_801F0E34(0x40051, 0);
        }
        fn_801E5754();
        fn_801E4D88();
    } else {
        field_0x4c4.fn_801D6E20(&mPos);
        if (mLife > 0 && !(mFlags & 7) && mState != 0xB && mState != 0x17) {
            startSound_801F0E34(0x4005E, 0);
            switch (mState) {
            case 0xC:
            case 0xD:
                startSound_801F0E34(0x4005F, 0);
                break;
            }
        }
    }
    bool step = false;
    switch (mState) {
    case 6:
    case 7:
        step = true;
        break;
    default:
        if (!(mFlags & 7)) {
            step = true;
        }
        break;
    }
    if (step) {
        fn_801CE2BC(mapHit);
    }
    field_0x448.fn_8021798C();
}

// The step of the vtable slot 0x98 (0x801D4ED4, a plain `fn_801D4ED4` symbol in the original).
// The sin/cos products sit in other float registers (f1/f0 and the cos in f3 in the original).
// FIXME: 99.82 %, regalloc-only; behaviour drift.
void VaatiBoss_804A4958::move_0x98() {
    bool step = false;
    if (field_0x5d1 != 0) {
        field_0x5d1--;
        if (field_0x5d1 == 0) {
            field_0x448.fn_80217CD8();
            s16 angle = field_0x5d2;
            JGeometry::TVec2<f32> vel(1.4f * JMath::sincosTable_.sinShort(angle),
                                      1.4f * JMath::sincosTable_.cosShort(angle));
            mVel_0x3c.x = vel.x;
            mVel_0x3c.y = vel.y;
        }
    }
    switch (mState) {
    case 0:
    case 0xB:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        break;
    case 6:
        if (mFlags & 1) {
            mPos.z = mPrevPos.z;
            mVel_0x3c.z = mPrevVel.z;
        }
        step = true;
        break;
    case 7:
        mPos.z = 500.0f;
        break;
    default: {
        JGeometry::TVec2<f32> old = field_0x5bc;
        switch (field_0x5b8) {
        case 0:
            field_0x5bc.x += 2.5f;
            break;
        case 1:
            field_0x5bc.x -= 2.5f;
            break;
        case 2:
            field_0x5bc.y += 2.5f;
            break;
        case 3:
            field_0x5bc.y -= 2.5f;
            break;
        }
        bool turn = false;
        JGeometry::TVec3f probe(field_0x5bc.x, field_0x5bc.y, 0.0f);
        if (isBlocked_0x4C(mLayer_0x4, &probe)) {
            field_0x5bc.x = old.x;
            field_0x5bc.y = old.y;
            turn = true;
        } else {
            field_0x5b4++;
            if (field_0x5b4 >= 0x3C) {
                turn = true;
            }
        }
        if (turn) {
            field_0x5b8 = fn_801CD580(field_0x5b8);
            field_0x5b4 = 0;
        }
        if (!(mFlags & 7)) {
            step = true;
        }
        break;
    }
    }
    if (step) {
        fn_801CE32C();
    }
    switch (mState) {
    case 1:
        if (mPos.z < 90.0f) {
            mPos.z = 90.0f;
        }
        break;
    default:
        if (!isUpperLayer_801CD2F8(mLayer_0x4)) {
            f32 z = mPos.z;
            if (z > 500.0f) {
                z = 500.0f;
            } else if (z < 31.9f) {
                z = 31.9f;
            }
            mPos.z = z;
        }
        break;
    }
    fn_801E4A18(&field_0x52c.field_0x0);
    fn_801E4A18(&field_0x52c.field_0x14);
    fn_801E4A18(&field_0x52c.field_0x28);
}

void VaatiBoss_804A4958::fn_801D4C70() {
    switch (mState) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 0x11:
        mVel_0x3c.x = 0.0f;
        mVel_0x3c.y = 0.0f;
        return;
    case 0x15: {
        JGeometry::TVec2f out;
        // calcVelocityToward_801D01A0 reads only (x, y) of its target; the original passes this 8-byte TVec2f slot.
        calcVelocityToward_801D01A0((const JGeometry::TVec3f*)&field_0x5ec, &out, 1.5f);
        mVel_0x3c.x = out.x;
        mVel_0x3c.y = out.y;
        return;
    }
    case 0x16: {
        JGeometry::TVec2f out;
        calcVelocityToward_801D01A0((const JGeometry::TVec3f*)&field_0x5f4, &out, 0.5f);
        mVel_0x3c.x = out.x;
        mVel_0x3c.y = out.y;
        return;
    }
    }
    if (field_0x5d1 != 0) {
        JGeometry::TVec2f v(4.0f * JMASSin(field_0x5d2), 4.0f * JMASCos(field_0x5d2));
        mVel_0x3c.x = v.x;
        mVel_0x3c.y = v.y;
        return;
    }
    JGeometry::TVec2f d(0.0f, 0.0f);
    bool follow = true;
    f32 tx;
    f32 ty;
    switch (mState) {
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
        follow = false;
        break;
    case 0x12:
    case 0x13:
    case 0x14:
        tx = field_0x5c4.x;
        ty = field_0x5c4.y;
        break;
    default:
        if (field_0x5b0 >= 0) {
            const JGeometry::TVec3f* p = getPlayerPos_8023E724(field_0x5b0);
            tx = p->x;
            ty = p->y;
        } else {
            tx = field_0x5bc.x;
            ty = field_0x5bc.y;
        }
        break;
    }
    if (follow) {
        JGeometry::TVec2f dir(tx - mPos.x, ty - mPos.y);
        dir.normalize();
        if (mState == 0x12) {
            dir.x *= -2.0f;
            dir.y *= -2.0f;
        } else {
            dir.x *= mSpeed;
            dir.y *= mSpeed;
        }
        d = dir;
    }
    addCalc2_801F71A4(&mVel_0x3c.x, d.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&mVel_0x3c.y, d.y, 0.3f, 1.0f);
}

void VaatiBoss_804A4958::calcVelocity_0x88() {
    fn_801D4C70();
    switch (mState) {
    case 1:
        mVel_0x3c.z = -2.0f;
        break;
    case 6:
        mVel_0x3c.z = 4.0f;
        break;
    case 8:
        mVel_0x3c.z += -0.1f;
        break;
    default:
        mVel_0x3c.z = 0.0f;
        break;
    }
    if (mState == 5) {
        field_0x5d4 += 0x300;
        mPos.z = 90.0f + 5.0f * JMASSin(field_0x5d4);
    }
}

void VaatiBoss_804A4958::onHitReactionEnd_0x74() {
    if (mState == 5) {
        fn_801E4D74();
    }

    if (checkFlag(0x8000)) {
        mFlags &= ~0x8000;
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            setState_801D5D84(0x15);
        } else {
            setState_801D5D84(0x17);
        }
    } else if (mState == 11) {
        if (isUpperLayer_801CD2F8(mLayer_0x4)) {
            setState_801D5D84(0xF);
        } else {
            field_0x448.fn_80217CD8();
            setState_801D5D84(0xC);
        }
    } else if (isUpperLayer_801CD2F8(mLayer_0x4)) {
        switch (mState) {
        case 15:
        case 16:
        case 18:
        case 19:
        case 20:
            setState_801D5D84(10);
            break;
        }
    } else if (mLife > 0 && mState != 23) {
        field_0x448.fn_80217CD8();
    }
}

// The camera step of the vtable slot 0xA0 (0x801D47DC, a plain `fn_801D47DC` symbol in the original):
// follows Vaati (its ground position) with the camera, by state.
// FIXME(shotgun): 99.41 %, missing/extra; behaviour drift.
// Ours adds one frsp after the fsel.
void VaatiBoss_804A4958::vfunc_0xA0() {
    const JGeometry::TBox2f* box = layerBox(mLayer_0x4);
    JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
    f32 top = 0.001f + box->i.y;
    focus.y = __fsel(top - focus.y, top, focus.y);
    switch (mState) {
    case 0:
    case 0x17:
        break;
    case 1: {
        JGeometry::TVec3f p(32.0f + box->i.x, 32.0f + box->i.y, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&p);
        JGeometry::TVec3f q(box->f.x - 32.0f, 32.0f + box->i.y, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&q);
        JGeometry::TVec3f r(32.0f + box->i.x, box->f.y - 32.0f, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&r);
        JGeometry::TVec3f t(box->f.x - 32.0f, box->f.y - 32.0f, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&t);
        break;
    }
    case 2:
    case 3: {
        cls_804A03A8* shadow = gGameRoot2_8053AB10->field_0x30;
        if (shadow->field_0x284 == 0) {
            shadow->field_0x284 = 1;
            shadow->field_0x28c = 0.05f;
        }
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->mShowWholeArea_0x7f5 == 0) {
            cam->mFocusOverride_0x7f4 = 1;
            cam->mFocusMode_0x7f8 = 4;
            cam->mFocusPos_0x7fc.x = focus.x;
            cam->mFocusPos_0x7fc.y = focus.y;
            cam->mFocusPos_0x7fc.z = focus.z;
            cam->mOverrideBlend_0x7f6 = 0;
        }
        break;
    }
    case 0x19: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->mBlendBoxOn_0x840 == 0) {
            cam->mBlendBoxOn_0x840 = 1;
            cam->mBlendBox_0x844.i.x = cam->mBlendBox_0x844.f.x = focus.x;
            cam->mBlendBox_0x844.i.y = cam->mBlendBox_0x844.f.y = focus.y;
            cam->mBlendWeight_0x854 = 1.0f;
            cam->mMargin_0x1d4 = -1.0f;
            cam->mMinHalfWidth_0x1d8 = -1.0f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        break;
    }
    case 0x1A: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->mShowWholeArea_0x7f5 == 0) {
            cam->mFocusOverride_0x7f4 = 1;
            cam->mFocusMode_0x7f8 = -1;
            cam->mOverrideBlend_0x7f6 = 0;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        break;
    }
    default: {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        if (gGameRoot2_8053AB10->field_0x24->field_0x179 == 0) {
            gGameRoot2_8053AB10->field_0x24->field_0x178 = 1;
        }
        break;
    }
    }
}

void VaatiBoss_804A4958::die_0x24() {}

VaatiBoss_804A4958::~VaatiBoss_804A4958() {}
