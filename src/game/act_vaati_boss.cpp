#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_vaati_boss.h"
#include "JSystem/JKernel/JKRArchive.h"

// Shared constants (declared in GameGlobals.h): game/tu_801EDBA4.cpp passes the first to
// fn_802F5FF4 and multiplies -12.0f by the second at run time. No function of this unit
// reads them. They lead this unit's .sdata2 (0x8053F0B8, before 0x8053F0C0 / 0x8053F0C4
// and the function literals): under deferred codegen file-scope constants are written
// before every literal of the file, so they cannot end game/tu_801D3CBC.cpp's pool.
const f32 lbl_8053F0B8 = 90.0f;
const f32 lbl_8053F0BC = 1.7f;
// 0x8053F0C0 / 0x8053F0C4: the words 12 and 0x1E; game/tu_801E3618.cpp reads them (lwz at 0x801E3EB4, lbz at 0x801E3D7C).
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

// FIXME: 99.90%. reloc-name. The original calls fn_801D77B4 (an identical copy of cls_801D7730's constructor, tu_801D6CEC) for the member at 0x4C4; ours calls __ct__12cls_801D7730Fv.
VaatiBoss_804A4958::VaatiBoss_804A4958() {
    field_0x5b0 = -1;
    field_0x5b4 = 0;
    field_0x5b8 = 0;
    field_0x5bc = 0.0f;
    field_0x5c0 = 0.0f;
    field_0x5c4 = 0.0f;
    field_0x5c8 = 0.0f;
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
    field_0x5e0 = 0.0f;
    field_0x5e4 = 0.0f;
    field_0x5e8 = 0;
    field_0x5e9 = 0;
    field_0x5ec = 0.0f;
    field_0x5f0 = 0.0f;
    field_0x5f4 = 0.0f;
    field_0x5f8 = 0.0f;
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

// FIXME: 96.98%. scheduling+regalloc + reloc-name. Float conversion temporaries and the stores of field_0x5f0 / field_0x5f4 / field_0x5f8 are scheduled differently.
void VaatiBoss_804A4958::init() {
    EnemyBase_804A4808::init();
    field_0x5dc = field_0x90 & 0xF;
    if (field_0x5dc != 0) {
        field_0x5dc += 7;
    }

    u32 y = ((field_0x90 >> 16) & 0x3F) * 8;
    u32 x = ((field_0x90 >> 24) & 0x3F) * 8;
    field_0x5e0 = (f32)x + 8.0f;
    field_0x5e4 = (f32)y + 8.0f;
    field_0x5e8 = (field_0x90 >> 10) & 0x3F;
    field_0x5e9 = (field_0x90 >> 4) & 0x3F;

    fn_801E4790(false);
    fn_801E4758();

    field_0xb0 = 0;
    field_0x98 = 2;
    mFlags |= ENEMY_FLAG_NO_FLOOR | ENEMY_FLAG_NO_ATTACK | ENEMY_FLAG_KEEP_LIFE;
    field_0x114 = 2;
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
    field_0x464.fn_801EB710(field_0x4, &mPos);

    field_0x5d8 = field_0x4;
    field_0x5b8 = fn_801CD664(4);

    field_0x5bc = mPos.x;
    field_0x5c0 = mPos.y;
    field_0x5ec = field_0x5e0 + (f32)field_0x5e8 * 8.0f + 32.0f;
    field_0x5f0 = field_0x5e4;
    field_0x5f4 = field_0x5ec;
    field_0x5f8 = field_0x5e4 + -32.0f;

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

    mAnim.fn_801D2608(lbl_8053A138, 2);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1D60();

    getGameView_802D6A9C()->fn_802D7440(0);

    for (s32 i = 0; i < 4; i++) {
        fn_80304C10(i, "gb_ch_obj_tr_sch5.szs", 0x6017000, 1, 0);
    }

    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "gb_ch_obj_tr_sch5.szs");
    gGameRoot2_8053AB10->field_0x20->fn_802F9714(res, 0, 0, 0x7000, 0);
}

void VaatiBoss_804A4958::vfunc_0x54() {
    gAudioMgr_8053AF80->fn_80458F9C(60);
}

// FIXME: 97.13%. regalloc-only + reloc-name. Register allocation of the random-velocity arithmetic.
void VaatiBoss_804A4958::fn_801D63D0() {
    for (s32 i = 0; i < 5; i++) {
        s32 serial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, 2);
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                JGeometry::TVec3f vel;
                vel.z = 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 3.0f;
                vel.y = 1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                vel.x = 1.2f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f);
                actor->field_0x3c = vel;
            }
        }
    }
}

void VaatiBoss_804A4958::vfunc_0x94() {
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
        mAnim.fn_801D1C18();
        break;
    }
}

// FIXME: 98.45%. missing/extra. The original reloads mFlags (0x230) before clearing 0x8000; ours reuses the loaded value.
void VaatiBoss_804A4958::vfunc_0x74() {
    if (mState == 5) {
        fn_801E4D74();
    }

    if (mFlags & 0x8000) {
        mFlags &= ~0x8000;
        if (fn_801CD2F8(field_0x4)) {
            fn_801D5D84(0x15);
        } else {
            fn_801D5D84(0x17);
        }
    } else if (mState == 11) {
        if (fn_801CD2F8(field_0x4)) {
            fn_801D5D84(0xF);
        } else {
            field_0x448.fn_80217CD8();
            fn_801D5D84(0xC);
        }
    } else if (fn_801CD2F8(field_0x4)) {
        switch (mState) {
        case 15:
        case 16:
        case 18:
        case 19:
        case 20:
            fn_801D5D84(10);
            break;
        }
    } else if (mLife > 0 && mState != 23) {
        field_0x448.fn_80217CD8();
    }
}

void VaatiBoss_804A4958::vfunc_0x24() {}

VaatiBoss_804A4958::~VaatiBoss_804A4958() {}
