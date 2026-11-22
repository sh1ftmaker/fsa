#include "game/GameAudio.h"
#include "game/act/act_princess_zelda_demo.h"
#include "game/ActorId.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/cls_8049D740.h"
#include "game/Camera.h"
#include "game/EnemyBase.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/GameRootSub20.h"
#include "game/SpriteParams.h"
#include "game/RotMatrix.h"
#include "JSystem/JMath/JMath.h"

// FIXME: 99.81 %, reloc-name (float literal pool order); behaviour clean.
PrincessZeldaDemo_8049D208::PrincessZeldaDemo_8049D208()
    : field_0x230(0),
      field_0x231(0),
      field_0x234(4),
      field_0x238(1),
      field_0x23c(-1),
      field_0x240(0),
      field_0x244(0),
      field_0x248(0),
      field_0x24c(0),
      field_0x250(0),
      field_0x254(0),
      field_0x258(0),
      field_0x25c(-1),
      field_0x260(0),
      field_0x264(0),
      field_0x268(0.0f),
      field_0x26c(0.0f),
      field_0x270(0.37f),
      field_0x274(96.0f),
      field_0x278(0.0f) {
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

// The animation key tables (lbl_8049D1A0, 26 of them; lbl_80539EF8, 2) with their key lists (frame, duration pairs, -1 ends).
static const s16 lbl_80464360[] = { 0, 3, 1, 3, 2, 3, 3, 3, 4, 3, 5, 3, 6, 3, 7, 3, -1, -1 };
static const s16 lbl_80464384[] = { 8, 3, 9, 3, 10, 3, 11, 3, 12, 3, 13, 3, 14, 3, 15, 3, -1, -1 };
static const s16 lbl_804643A8[] = { 16, 3, 17, 3, 18, 3, 19, 3, 20, 3, 21, 3, 22, 3, 23, 3, -1, -1 };
static const s16 lbl_804643CC[] = { 24, 3, 25, 3, 26, 3, 27, 3, 28, 3, 29, 3, 30, 3, 31, 3, -1, -1 };
static const s16 lbl_804643F0[] = { 32, 3, 33, 5, 34, 6, 35, 7, 36, 30, -1, -1 };
static const s16 lbl_80464408[] = { 36, 30, 35, 7, 34, 6, 33, 5, 32, 3, -1, -1 };
static const s16 lbl_80464420[] = { 24, 1, 0, 2, 16, 10, -1, -1 };
static const s16 lbl_80464430[] = { 16, 1, 0, 2, 24, 10, -1, -1 };
static const s16 lbl_80464440[] = { 24, 1, 8, 2, 16, 10, -1, -1 };
static const s16 lbl_80464450[] = { 16, 1, 8, 2, 24, 10, -1, -1 };
static const s16 lbl_80464460[] = { 16, 8, 8, 2, 24, 2, -1, -1 };
static const s16 lbl_80464470[] = { 0, 8, 1, 8, 2, 40, -1, -1 };
static const s16 lbl_80464480[] = { 3, 5, 4, 5, 5, 5, -1, -1 };
static const s16 lbl_80464490[] = { 6, 5, 7, 5, 8, 5, -1, -1 };
static const s16 lbl_804644A0[] = { 0, 9, 1, 9, 2, 9, -1, -1 };
static const s16 lbl_804644B0[] = { 3, 9, 4, 9, 5, 9, -1, -1 };
static const s16 lbl_804644C0[] = { 7, 1, 8, 9, 0, 10, -1, -1 };
static const s16 lbl_804644D0[] = { 10, 1, 6, 10, -1, -1 };
static const s16 lbl_804644DC[] = { 0, 1, 8, 9, 7, 9, -1, -1 };
static const s16 lbl_804644EC[] = { 7, 8, 6, 8, -1, -1 };
static const s16 lbl_804644F8[] = { 5, 5, 9, 9, 10, 10, -1, -1 };
static const s16 lbl_80464508[] = { 24, 6, 25, 6, 26, 6, 27, 6, 28, 6, 29, 6, 30, 6, 31, 6, -1, -1 };
static const s16 lbl_8046452C[] = { 16, 6, 17, 6, 18, 6, 19, 6, 20, 6, 21, 6, 22, 6, 23, 6, -1, -1 };
static const s16 lbl_8053C520[] = { 0, 3, -1, -1 };
static const s16 lbl_8053C528[] = { 8, 3, -1, -1 };
static const s16 lbl_8053C530[] = { 16, 3, -1, -1 };
static const s16 lbl_8053C538[] = { 24, 3, -1, -1 };
static const s16 lbl_8053C540[] = { 33, 5, -1, -1 };

static const s16* lbl_8049D1A0[26] = {
    lbl_80464360, lbl_80464384, lbl_804643A8, lbl_804643CC,
    lbl_804643F0, lbl_80464408, lbl_8053C520, lbl_8053C528,
    lbl_8053C530, lbl_8053C538, lbl_80464420, lbl_80464430,
    lbl_80464440, lbl_80464450, lbl_80464460, lbl_80464470,
    lbl_80464480, lbl_80464490, lbl_8053C540, lbl_804644A0,
    lbl_804644B0, lbl_804644C0, lbl_804644D0, lbl_804644DC,
    lbl_804644F8, lbl_804644EC,
};
static const s16* lbl_80539EF8[2] = {
    lbl_80464508, lbl_8046452C,
};
// The scenario keys of the demo (rodata 0x80464550).
static const s32 lbl_80464550[] = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0078, 0x012C, 0x014A, 0x0168,
    0x01A4, 0x01E0, 0x01FE, 0x021C, 0x023A, 0x025D, 0x0000, 0x0064,
    0x00A0, 0x015E, 0x0190, 0x0212, 0x024E, 0x02B2, 0x047E, 0x076C,
    0x07C6, 0x080C, 0x082A, 0x0834, 0x083E, 0x087A, 0x0974, 0x09D8,
    0x09F6, 0x0AF0, 0x0C9E, 0x0DAC, 0x0DB6, 0x0E24, 0x0000, 0x007D,
    0x00F5, 0x015E, 0x01C2, 0x01E0, 0x0230, 0x030C, 0x05FA, 0x062C,
    0x0000, 0x0000, 0x003C, 0x0078, 0x0316, 0x037A, 0x042E, 0x046A,
    0x0500, 0x0532, 0x076C, 0x07A8, 0x082A, 0x0848, 0x0000,
};
static const s32 lbl_8053C548[] = { 0x232, 0x4B0 };

// FIXME: 99.87 %, reloc-name (float literal pool order); behaviour clean.
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
            switch (field_0x234) {
            case 0:
            case 1:
                fn_8010E7E8(0x3d);
                break;
            case 2:
                fn_8010E2D4(0x3d);
                break;
            case 3:
                break;
            }
            field_0x238 = 0x3d;
        } else {
            mDrawEnabled = false;
        }
    }
}

void PrincessZeldaDemo_8049D208::vfunc_0x10() {
    cls_801192AC* list = fn_8011921C();
    ActorBase_804A5770** goal = list->fn_8011871C(list->mActors.mBegin, ACTOR_DMGL);
    if (goal != fn_8011921C()->mActors.mEnd) {
        field_0x27c = (*goal)->mPos;
    }

    ActorBase_804A5770** node = fn_8011921C()->mActors.mBegin;
    do {
        node = fn_8011921C()->fn_8011871C(node, ACTOR_DMMD);
        if (node == fn_8011921C()->mActors.mEnd) {
            break;
        }
        cls_8049D740* maiden = static_cast<cls_8049D740*>(*node);
        if (maiden->mMode < 6) {
            field_0x288[maiden->mMode] = maiden->mPos;
            node++;
        }
    } while (node != fn_8011921C()->mActors.mEnd);

    field_0x288[6] = mPos;
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

// The two camera requests the demo states make (inlined in the original): a focus range on the
// view (field_0x840) and a movement box (field_0x7f4), each taken only while free.
static inline void camRange(f32 x0, f32 y0, f32 x1, f32 y1) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x840 == 0) {
        cam->field_0x840 = 1;
        cam->field_0x844.set(x0, y0, x1, y1);
        cam->field_0x854 = 1.0f;
        cam->field_0x1d4 = -1.0f;
        cam->field_0x1d8 = -1.0f;
    }
}

static inline void camBox(f32 x0, f32 y0, f32 x1, f32 y1) {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->field_0x7f5 == 0) {
        cam->field_0x7f4 = 1;
        cam->field_0x7f8 = 5;
        cam->field_0x808.set(x0, y0, x1, y1);
        cam->field_0x7f6 = 0;
    }
}

// The demo states of the first two scene sets (field_0x234 0 and 1): keeps the maiden's box
// registered, steers the camera (fn_80110454) and runs the state's step.
// FIXME: 97.79 %, missing/extra + reloc-name; behaviour drift.
void PrincessZeldaDemo_8049D208::fn_80110E9C() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x238 == 1 || field_0x238 == 2) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    fn_80110454();
    switch (field_0x238) {
    case 1: {
        gAudioMgr_8053AF80->fn_80458FF0(0x100000E, false, 0);
        if (field_0x234 == 1) {
            fn_8011921C()->fn_801186B0(1);
            break;
        }
        if (field_0x234 == 0) {
            fn_8011921C()->fn_801186B0(0);
            break;
        }
        bool talkFlag = false;
        if (field_0x234 != 1) {
            u16 angle;
            f32 dist;
            s32 other;
            s32 dir;
            fn_801EEC98(mSerial, &angle, &dir, &dist, &other, 0, 2);
            if (dir >= 0 && !(dist > 80.0f)) {
                faceDir(dir);
            }
        } else {
            talkFlag = true;
        }
        u32 msg = lbl_8053C548[field_0x234];
        s32 player = startTalk(msg, talkFlag);
        if (player >= 0) {
            switch (field_0x234) {
            case 0:
            case 1:
                fn_8010E7E8(2);
                break;
            case 2:
                fn_8010E2D4(2);
                break;
            }
            field_0x238 = 2;
        }
        break;
    }
    case 2:
        fn_8022FF8C(field_0x23c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x23c, mSerial)) {
            field_0x23c = -1;
            switch (field_0x234) {
            case 1:
                break;
            case 0:
                switch (field_0x234) {
                case 0:
                case 1:
                    fn_8010E7E8(0xD);
                    break;
                case 2:
                    fn_8010E2D4(0xD);
                    break;
                }
                field_0x238 = 0xD;
                switch (field_0x234) {
                case 0:
                case 1:
                    fn_8010E7E8(0x3E);
                    break;
                case 2:
                    fn_8010E2D4(0x3E);
                    break;
                }
                field_0x238 = 0x3E;
                break;
            default:
                switch (field_0x234) {
                case 0:
                case 1:
                    fn_8010E7E8(1);
                    break;
                case 2:
                    fn_8010E2D4(1);
                    break;
                }
                field_0x238 = 1;
                break;
            }
        }
        break;
    case 4:
        mAnim0.mFrameCtrl.setRate(1.0f);
        if (fn_801CD950(&mPos.y, getPlayerPos_8023E724(0)->y - 24.0f, 0.7f) == 0) {
            mAnim0.startAnim_801D22DC(2, 2, false);
            mAnim0.fn_801D1F14(0.0f);
            mAnim0.mFrameCtrl.setRate(1.0f);
        }
        break;
    case 5:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 7:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 9:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 11:
        fn_801CD950(&mPos.y, getPlayerPos_8023E724(0)->y - 300.0f, 0.7f);
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.y <= field_0x2dc.y) {
                pos.y = field_0x2dc.y;
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            } else {
                pos += field_0x2e8;
                fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
            }
        }
        break;
    case 12:
        fn_801CD950(&mPos.y, getPlayerPos_8023E724(0)->y - 300.0f, 0.7f);
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.y <= field_0x2dc.y) {
                pos.y = field_0x2dc.y;
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            } else {
                pos += field_0x2e8;
                fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
            }
        }
        break;
    case 14:
        if (fn_8011921C()->field_0x8 >= 30) {
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
                if (pos.y <= field_0x2dc.y) {
                    pos.y = field_0x2dc.y;
                    fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
                } else {
                    pos += field_0x2e8;
                    fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
                }
            }
            mPos += field_0x2e8;
        }
        break;
    case 15:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.y <= field_0x2dc.y) {
                pos.y = field_0x2dc.y;
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            } else {
                pos += field_0x2e8;
                fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
            }
        }
        mPos += field_0x2e8;
        break;
    case 16:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.y <= field_0x2dc.y) {
                pos.y = field_0x2dc.y;
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            } else {
                pos += field_0x2e8;
                fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
            }
        }
        mPos += field_0x2e8;
        if (mPos.y <= mSpawnPos.y) {
            mPos.y = mSpawnPos.y;
            mAnim0.startAnim_801D22DC(2, 2, false);
            mAnim0.fn_801D1F14(0.0f);
        }
        break;
    case 17:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 18:
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.y >= field_0x2dc.y) {
                pos.y = field_0x2dc.y;
                fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
            } else {
                pos += field_0x2e8;
                fn_80230498(i, &pos, mAnim1.mCel, 0xFF, -0x80, -0x80, true);
            }
        }
        break;
    case 21:
        if (mAnim0.fn_801D208C()) {
            mAnim0.startAnim_801D22DC(0x13, 2, false);
            mAnim0.fn_801D1F14(0.0f);
        }
        break;
    case 26:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
            gAudioMgr_8053AF80->fn_80458FF0(0x100001F, true, 0);
        }
        break;
    case 31:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 32:
        switch (field_0x260) {
        case 0:
        case 90:
        case 110:
        case 130:
            switch (mAnim0.mAnim) {
            case 9:
            case 11:
            case 13:
                mAnim0.startAnim_801D22DC(10, 0, false);
                break;
            default:
                mAnim0.startAnim_801D22DC(11, 0, false);
                break;
            }
            mAnim0.fn_801D1F14(0.0f);
            break;
        }
        field_0x260++;
        break;
    case 33: {
        f32 dz = 2.0f - 0.2f * (fn_8011921C()->field_0x8 - lbl_80464550[33]);
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            pos.z += dz;
            if (pos.z <= 0.0f) {
                pos.z = 0.0f;
            } else {
                pos.y += 2.0f;
            }
            fn_80230498(i, &pos, 0x3F, 0xFF, -0x80, -0x80, true);
        }
        break;
    }
    case 35: {
        field_0x268 += 0.04f;
        fn_801CD950(&field_0x26c, 2.0f, 0.05f);
        s16 angle = (s32)(JMath::atanTable_.atan2_(field_0x27c.x - mPos.x, field_0x27c.y - mPos.y) *
                          10430.378f);
        field_0x3c.x = field_0x26c * JMath::sincosTable_.sinShort(angle);
        field_0x3c.y = field_0x26c * JMath::sincosTable_.cosShort(angle);
        s32 doneX = fn_801CD950(&mPos.x, field_0x27c.x, field_0x3c.x);
        s32 doneY = fn_801CD950(&mPos.y, field_0x27c.y, field_0x3c.y);
        fn_801CD950(&field_0x270, 0.05f, -0.005f);
        bool arrived = doneX == 0 && doneY == 0;
        if (arrived) {
            switch (field_0x234) {
            case 0:
            case 1:
                fn_8010E7E8(0x24);
                break;
            case 2:
                fn_8010E2D4(0x24);
                break;
            }
            field_0x238 = 0x24;
        }
        break;
    }
    case 36:
        fn_801CD950(&field_0x270, 0.0f, -0.02f);
        break;
    case 62:
        mDrawEnabled = false;
        break;
    }
}

// The idle states of the demo: keeps the maiden's box registered, steers the camera
// (fn_80110210) and runs the state's step; a player standing at the maiden starts the talk (state 2).
// FIXME: 98.40 %, scheduling + reloc-name; behaviour drift.
// Left: in startTalk the original stores the message position's z after loading mSerial and field_0x4.
void PrincessZeldaDemo_8049D208::fn_8011093C() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x231 != 0) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    fn_80110210();
    switch (field_0x238) {
    case 2:
        fn_8022FF8C(field_0x23c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x23c, mSerial)) {
            field_0x23c = -1;
            switch (field_0x234) {
            case 0:
            case 1:
                fn_8010E7E8(0x3D);
                break;
            case 2:
                fn_8010E2D4(0x3D);
                break;
            case 3:
                break;
            }
            field_0x238 = 0x3D;
        }
        break;
    case 0x2A:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 0x2C:
        fn_801CD950(&mPos.z, 24.0f, 0.15f);
        fn_801CD950(&field_0x274, 162.0f, 0.2f);
        if (mAnim0.fn_801D208C()) {
            mAnim0.startAnim_801D22DC(0x13, 2, false);
            mAnim0.fn_801D1F14(0.0f);
        }
        break;
    case 0x2E:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 0x32:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 0x35:
        if (fn_801CD950(&mPos.z, mSpawnPos.z, 0.3f) == 0 && mAnim0.mAnim == 0x13) {
            mAnim0.startAnim_801D22DC(0x17, 0, false);
            mAnim0.fn_801D1F14(0.0f);
        }
        if (mAnim0.fn_801D208C() && mAnim0.mAnim == 0x17) {
            mAnim0.startAnim_801D22DC(0x19, 0, false);
            mAnim0.fn_801D1F14(0.0f);
        }
        break;
    case 0x36:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            fn_8011921C()->fn_801183BC();
        }
        break;
    case 0x37:
        fn_801CD950(&field_0x278, -80.0f, -3.0f);
        fn_801CD950(&field_0x274, 130.0f, 2.0f);
        break;
    case 0x3B:
        fn_801CD950(&mPos.x, 64.0f + mSpawnPos.x, 0.5f);
        break;
    case 0x3D: {
        u16 angle;
        f32 dist;
        s32 other;
        s32 dir;
        fn_801EEC98(mSerial, &angle, &dir, &dist, &other, 0, 2);
        if (dir >= 0 && !(dist > 80.0f)) {
            faceDir(dir);
        }
        s32 player = startTalk(0x2D2, false);
        if (player >= 0) {
            switch (field_0x234) {
            case 0:
            case 1:
                fn_8010E7E8(2);
                break;
            case 2:
                fn_8010E2D4(2);
                break;
            case 3:
                break;
            }
            field_0x238 = 2;
        }
        break;
    }
    case 0x3E:
        break;
    }
}

// The camera for the demo states 3-28: which part of the stage stays in view (player, spawn point
// or the maiden), per state.
// FIXME: 99.59 %, reloc-name (float literal pool order); behaviour clean.
void PrincessZeldaDemo_8049D208::fn_80110454() {
    switch (field_0x238) {
    case 3:
    case 4:
    case 5: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = *getPlayerPos_8023E724(0);
        p.y += -8.0f;
        camRange(p.x, p.y - 96.0f, p.x, 96.0f + p.y);
        break;
    }
    case 6:
    case 7: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = *getPlayerPos_8023E724(0);
        p.y = p.y + field_0x278;
        camBox(p.x, p.y - 96.0f, p.x, 96.0f + p.y);
        break;
    }
    case 8:
    case 9:
    case 10: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = *getPlayerPos_8023E724(0);
        fn_801CD950(&field_0x278, -8.0f, 1.0f);
        p.y = p.y + field_0x278;
        camBox(p.x, p.y - 96.0f, p.x, 96.0f + p.y);
        break;
    }
    case 11:
    case 12:
    case 13:
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
        break;
    case 14:
    case 15: {
        gGameRoot2_8053AB10->field_0x24->field_0x17c = 1;
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = mSpawnPos;
        p.y += -40.0f;
        camRange(p.x, p.y - 180.0f, p.x, 180.0f + p.y);
        break;
    }
    case 16:
    case 17:
    case 18:
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        camRange(mSpawnPos.x, mSpawnPos.y - 80.0f, mSpawnPos.x, 80.0f + mSpawnPos.y);
        break;
    case 19:
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        camRange(field_0x288[6].x, field_0x288[6].y - 80.0f, field_0x288[6].x, 80.0f + field_0x288[6].y);
        break;
    case 20: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = mPos;
        p.y += -75.0f;
        camRange(p.x, p.y - 130.0f, p.x, 130.0f + p.y);
        break;
    }
    case 21:
    case 22:
    case 23: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = mPos;
        p.y += -75.0f;
        camBox(p.x, p.y - 130.0f, p.x, 130.0f + p.y);
        break;
    }
    case 24:
    case 25:
    case 26:
    case 27:
    case 28: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x179 == 0) {
            cam->field_0x17a = 1;
            cam->field_0x179 = 0;
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        Camera_804AA9E4* cam2 = gGameRoot2_8053AB10->field_0x24;
        if (cam2->field_0x840 == 0) {
            cam2->field_0x840 = 1;
            cam2->field_0x844.i.x = cam2->field_0x844.f.x = mPos.x;
            cam2->field_0x844.i.y = cam2->field_0x844.f.y = mPos.y;
            cam2->field_0x854 = 1.0f;
            cam2->field_0x1d4 = -1.0f;
            cam2->field_0x1d8 = -1.0f;
        }
        break;
    }
    }
}

// The camera for the demo states 0x26-0x3C (the second set of scenes).
// FIXME(shotgun): 99.62 %, regalloc-only + reloc-name; behaviour drift.
void PrincessZeldaDemo_8049D208::fn_80110210() {
    switch (field_0x238) {
    case 0x26:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        break;
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        camBox(mPos.x, mPos.y + field_0x278 - field_0x274, mPos.x, field_0x274 + (mPos.y + field_0x278));
        break;
    }
    case 0x2C:
    case 0x2D:
    case 0x2E: {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        camBox(mPos.x, mPos.y + field_0x278 - field_0x274, mPos.x, field_0x274 + (mPos.y + field_0x278));
        break;
    }
    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f4 == 0) {
            cam->field_0x7f5 = 1;
        }
        break;
    }
    case 0x34:
    case 0x35:
    case 0x36:
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        camRange(mPos.x, mPos.y - field_0x274, mPos.x, mPos.y + field_0x274);
        break;
    case 0x37:
    case 0x38:
    case 0x39:
    case 0x3A:
    case 0x3B:
    case 0x3C: {
        gGameRoot2_8053AB10->field_0x24->field_0x17c = 1;
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        JGeometry::TVec3f p = mSpawnPos;
        p.y = p.y + field_0x278;
        camBox(p.x, p.y - field_0x274, p.x, p.y + field_0x274);
        break;
    }
    case 0x2F:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        break;
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

// Starts the demo scene for the GameView scenario `key` (fn_8011921C()'s field_0x8): maps it to the
// scene's first state, per demo variant (field_0x234) and chapter (field_0x4).
void PrincessZeldaDemo_8049D208::vfunc_0x54() {
    s32 key = fn_8011921C()->field_0x8;
    switch (field_0x234) {
    case 0:
        if (fn_8011921C()->field_0x4 == 0) {
            if (key == lbl_80464550[3]) {
                fn_8010F478(0x3);
            } else if (key == lbl_80464550[4]) {
                fn_8010F478(0x4);
            } else if (key == lbl_80464550[5]) {
                fn_8010F478(0x5);
            } else if (key == lbl_80464550[6]) {
                fn_8010F478(0x6);
            } else if (key == lbl_80464550[7]) {
                fn_8010F478(0x7);
            } else if (key == lbl_80464550[8]) {
                fn_8010F478(0x8);
            } else if (key == lbl_80464550[9]) {
                fn_8010F478(0x9);
            } else if (key == lbl_80464550[10]) {
                fn_8010F478(0xA);
            } else if (key == lbl_80464550[11]) {
                fn_8010F478(0xB);
            } else if (key == lbl_80464550[12]) {
                fn_8010F478(0xC);
            } else if (key == lbl_80464550[13]) {
                fn_8010F478(0xD);
                fn_8010F478(0x25);
            }
        }
        break;
    case 1:
        if (fn_8011921C()->field_0x4 == 1) {
            if (key == lbl_80464550[14]) {
                fn_8010F478(0xE);
            } else if (key == lbl_80464550[15]) {
                fn_8010F478(0xF);
            } else if (key == lbl_80464550[16]) {
                fn_8010F478(0x10);
            } else if (key == lbl_80464550[17]) {
                fn_8010F478(0x11);
            } else if (key == lbl_80464550[18]) {
                fn_8010F478(0x12);
            } else if (key == lbl_80464550[19]) {
                fn_8010F478(0x13);
            } else if (key == lbl_80464550[20]) {
                fn_8010F478(0x14);
            } else if (key == lbl_80464550[21]) {
                fn_8010F478(0x15);
            } else if (key == lbl_80464550[22]) {
                fn_8010F478(0x16);
            } else if (key == lbl_80464550[23]) {
                fn_8010F478(0x17);
            } else if (key == lbl_80464550[24]) {
                fn_8010F478(0x18);
            } else if (key == lbl_80464550[25]) {
                fn_8010F478(0x19);
            } else if (key == lbl_80464550[26]) {
                fn_8010F478(0x1A);
            } else if (key == lbl_80464550[27]) {
                fn_8010F478(0x1B);
            } else if (key == lbl_80464550[28]) {
                fn_8010F478(0x1C);
            } else if (key == lbl_80464550[29]) {
                fn_8010F478(0x1D);
            } else if (key == lbl_80464550[30]) {
                fn_8010F478(0x1E);
            } else if (key == lbl_80464550[31]) {
                fn_8010F478(0x1F);
            } else if (key == lbl_80464550[32]) {
                fn_8010F478(0x20);
            } else if (key == lbl_80464550[33]) {
                fn_8010F478(0x21);
            } else if (key == lbl_80464550[34]) {
                fn_8010F478(0x22);
            } else if (key == lbl_80464550[35]) {
                fn_8010F478(0x23);
            } else if (key == lbl_80464550[37]) {
                fn_8010F478(0x3E);
            }
        }
        break;
    case 2:
        switch (fn_8011921C()->field_0x4) {
        case 0xF:
            if (key == lbl_80464550[38]) {
                fn_8010F478(0x26);
            } else if (key == lbl_80464550[39]) {
                fn_8010F478(0x27);
            } else if (key == lbl_80464550[40]) {
                fn_8010F478(0x28);
            } else if (key == lbl_80464550[41]) {
                fn_8010F478(0x29);
            } else if (key == lbl_80464550[42]) {
                fn_8010F478(0x2A);
            } else if (key == lbl_80464550[43]) {
                fn_8010F478(0x2B);
            } else if (key == lbl_80464550[44]) {
                fn_8010F478(0x2C);
            } else if (key == lbl_80464550[45]) {
                fn_8010F478(0x2D);
            } else if (key == lbl_80464550[46]) {
                fn_8010F478(0x2E);
            } else if (key == lbl_80464550[47]) {
                fn_8010F478(0x2F);
            }
            break;
        case 0xD:
            if (key == lbl_80464550[48]) {
                fn_8010F478(0x30);
            } else if (key == lbl_80464550[50]) {
                fn_8010F478(0x32);
            } else if (key == lbl_80464550[51]) {
                fn_8010F478(0x33);
            } else if (key == lbl_80464550[52]) {
                fn_8010F478(0x34);
            } else if (key == lbl_80464550[53]) {
                fn_8010F478(0x35);
            } else if (key == lbl_80464550[54]) {
                fn_8010F478(0x36);
            } else if (key == lbl_80464550[55]) {
                fn_8010F478(0x37);
            } else if (key == lbl_80464550[56]) {
                fn_8010F478(0x38);
            } else if (key == lbl_80464550[57]) {
                fn_8010F478(0x39);
            } else if (key == lbl_80464550[58]) {
                fn_8010F478(0x3A);
            } else if (key == lbl_80464550[59]) {
                fn_8010F478(0x3B);
            } else if (key == lbl_80464550[60]) {
                fn_8010F478(0x3C);
            } else if (key == lbl_80464550[61]) {
                fn_8010F478(0x3D);
            }
            break;
        }
        break;
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
// FIXME: 99.81 %, reloc-name (float literal pool order); behaviour clean.
void PrincessZeldaDemo_8049D208::fn_8010F7EC(s32 arg) {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    params.field_0x1d = field_0x24c;
    if (field_0x238 == 0x2F) {
        field_0x264 += 0x258;
        if (field_0x264 >= 0x10000) {
            field_0x264 -= 0x10000;
        }
        f32 z = 2.0f * JMath::sincosTable_.sinShort(field_0x264);
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = z;
        mtx[0][0] = 1.0f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.0f;
        params.mMtx = mtx;
    }
    if (arg == 0x21B) {
        fn_801F02BC("enemy_gc_sob.bin", arg + mAnim0.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
    } else {
        fn_801F02BC("enemy_both2_sob.bin", arg + mAnim0.mCel, &params, 0x407, 0, false, 1.0f, 1.0f);
    }
}

// The rotation matrix about `axis` by `rad` (the 3x3 part), with its own normalisation.
// FIXME: 98.56 %, regalloc-only + reloc-name; behaviour clean.
// FIXME(shotgun): register allocation and literal-pool names still differ.
void fn_8010F668(Mtx m, const JGeometry::TVec3f* axis, f32 rad) {
    JGeometry::TVec3f n;
    n.normalize(*axis);
    f32 s = sin(rad);
    f32 c = cos(rad);
    f32 t = 1.0f - c;
    f32 x = n.x;
    f32 y = n.y;
    f32 z = n.z;
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
// FIXME: 99.65 %, reloc-name (float literal pool order); behaviour clean.
void PrincessZeldaDemo_8049D208::fn_8010F4DC() {
    field_0x268 += 0.01f;
    Mtx mtx;
    PSMTXTrans(mtx, mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f axisX(1.0f, 0.0f, 0.0f);
    fn_8010F668(mtx, &axisX, 0.08726646f);
    Mtx rot;
    rot[0][0] = rot[1][0] = rot[2][0] = rot[0][1] = rot[1][1] = rot[2][1] = rot[0][2] = rot[1][2] =
        rot[2][2] = rot[0][3] = rot[1][3] = rot[2][3] = 0.0f;
    JGeometry::TVec3f axisZ(0.0f, 0.0f, 1.0f);
    fn_8010F668(rot, &axisZ, field_0x268);
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
        JMath::gekko_ps_copy12(crystal.mMtx, mtx);
        crystal.field_0x48 = -1.0f;
        crystal.field_0x4c = frame;
        crystal.field_0x54 = 0;
        crystal.field_0x58 = 1;
    }
}

#pragma auto_inline reset

// Not auto-inlined: vfunc_0x54 calls fn_8010F478 out of line in the original.
#pragma auto_inline off
void PrincessZeldaDemo_8049D208::fn_8010F478(s32 arg) {
    switch (field_0x234) {
    case 0:
    case 1:
        fn_8010E7E8(arg);
        break;
    case 2:
        fn_8010E2D4(arg);
        break;
    case 3:
        break;
    }
    field_0x238 = arg;
}
#pragma auto_inline reset

// The centre of the camera's visible box, at z 0.
static inline void setCenter(JGeometry::TVec3f& pos, JGeometry::TBox2f box) {
    pos.x = box.f.x + 0.5f * (box.i.x - box.f.x);
    pos.y = box.f.y + 0.5f * (box.i.y - box.f.y);
    pos.z = 0.0f;
}

// Enters demo state `arg` (3-0x25) of the second scene set.
// FIXME: 99.45 %, operands + reloc-name; behaviour drift.
void PrincessZeldaDemo_8049D208::fn_8010E7E8(s32 arg) {
    switch (arg) {
    case 3:
        fn_8011921C()->setField_0x2(1);
        lbl_8053AED0->fn_8043A450(0, 3);
        break;
    case 4:
        mAnim0.startAnim_801D22DC(2, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 5:
        mAnim0.startAnim_801D22DC(8, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, lbl_8053C548[field_0x234], field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 6:
        field_0x278 = -40.0f;
        break;
    case 7:
        mAnim0.startAnim_801D22DC(0xE, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x233, field_0x4, JGeometry::TVec3f(0.0f, -226.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 8:
        mAnim0.startAnim_801D22DC(0xC, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        field_0x278 = -25.0f;
        break;
    case 9:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x234, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 10:
        mAnim0.startAnim_801D22DC(0xD, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        break;
    case 11:
        mAnim0.startAnim_801D22DC(3, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        field_0x2e8.set(0.0f, -0.7f, 0.0f);
        field_0x2dc.set(*getPlayerPos_8023E724(0));
        field_0x2dc.y -= 300.0f;
        break;
    case 12:
        fn_80119DD0()->fn_80119998(0x5A, 0, 2, 0, 0, 0x3C, 0);
        break;
    case 13:
        getGameView_802D6A9C()->fn_802D9FF4(field_0x244, field_0x248);
        getGameView_802D6A9C()->fn_802DA8DC();
        break;
    case 14: {
        fn_8011921C()->setField_0x2(1);
        JGeometry::TVec3f start = mSpawnPos;
        JGeometry::TVec3f target = *getPlayerPos_8023E724(0);
        target.y += 32.0f;
        start.y += 16.0f;
        for (int i = 0; i < 4; i++) {
            fn_80230498(i, &target, fn_8023CBC0(i), 0xFF, -0x80, -0x80, true);
        }
        JGeometry::TVec3f pos = target;
        pos.y -= 16.0f;
        mPos.set(pos);
        mAnim0.startAnim_801D22DC(3, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        field_0x2dc = start;
        JGeometry::TVec3f d = start - target;
        field_0x2e8 = JGeometry::TVec3f(d.x / 240.0f, d.y / 240.0f, d.z / 240.0f);
        mAnim1.startAnim_801D22DC(0, 2, false);
        mAnim1.fn_801D1F14(0.0f);
        break;
    }
    case 16: {
        JGeometry::TVec3f pos = *fn_8023E604(0);
        pos.y -= 32.0f;
        mPos.set(pos);
        mAnim0.startAnim_801D22DC(3, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        JGeometry::TVec3f start = mSpawnPos;
        JGeometry::TVec3f target = *fn_8023E604(0);
        start.y += 16.0f;
        target.y -= 16.0f;
        for (int i = 0; i < 4; i++) {
            fn_80230498(i, &target, fn_8023CBC0(i), 0xFF, -0x80, -0x80, true);
        }
        field_0x2dc = start;
        JGeometry::TVec3f d = start - target;
        field_0x2e8 = JGeometry::TVec3f(d.x / 110.0f, d.y / 110.0f, d.z / 110.0f);
        mAnim1.startAnim_801D22DC(0, 2, false);
        mAnim1.fn_801D1F14(0.0f);
        break;
    }
    case 17:
        mAnim0.startAnim_801D22DC(8, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(1.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x4B0, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 18: {
        JGeometry::TVec3f start = mPos;
        start.y += 48.0f;
        JGeometry::TVec3f* target = getPlayerPos_8023E724(0);
        field_0x2dc = start;
        JGeometry::TVec3f d = start - *target;
        field_0x2e8 = JGeometry::TVec3f(d.x / 60.0f, d.y / 60.0f, d.z / 60.0f);
        mAnim1.startAnim_801D22DC(0, 2, false);
        mAnim1.fn_801D1F14(0.0f);
        break;
    }
    case 19:
        fn_8011921C()->fn_801183BC();
        mAnim0.startAnim_801D22DC(0xF, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_8011921C()->setField_0x2(1);
        gAudioMgr_8053AF80->fn_80458FF0(0x100001B, false, 0);
        break;
    case 21:
        field_0x364 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x309, 0, 0, NULL, NULL, true, 1.0f);
        field_0x368 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x30A, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x2E1, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x2E2, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x2E3, 0, 0, NULL, NULL, true, 1.0f);
        field_0x36c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x2EA, 0, 0, NULL, NULL, true, 1.0f);
        mAnim0.startAnim_801D22DC(0x15, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 22:
        mAnim0.startAnim_801D22DC(0x14, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 23:
        if (field_0x364 != NULL) {
            JPABaseEmitter* emitter = field_0x364;
            emitter->stopCreateParticle();
            emitter->mMaxFrame = 1;
            field_0x364 = NULL;
        }
        if (field_0x368 != NULL) {
            JPABaseEmitter* emitter = field_0x368;
            emitter->stopCreateParticle();
            emitter->mMaxFrame = 1;
            field_0x368 = NULL;
        }
        if (field_0x36c != NULL) {
            JPABaseEmitter* emitter = field_0x36c;
            emitter->stopCreateParticle();
            emitter->mMaxFrame = 1;
            field_0x36c = NULL;
        }
        mAnim0.startAnim_801D22DC(0x18, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 25:
        mAnim0.startAnim_801D22DC(0x16, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 26:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x239, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 28: {
        JGeometry::TVec3f pos = mPos;
        pos.y += -16.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400B5, 0);
        mAnim0.startAnim_801D22DC(0xD, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    }
    case 30: {
        JGeometry::TVec3f pos = mPos;
        pos.y += -16.0f;
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x400B5, 0);
        break;
    }
    case 31:
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x4B1, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4022);
        fn_8011921C()->fn_80118454();
        break;
    case 33: {
        gAudioMgr_8053AF80->fn_80458FF0(0x100002B, true, 0);
        startSound_801F0E34(0x40071, 0);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xC1, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xC2, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xC3, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0xC4, 0, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f pos;
        setCenter(pos, getGameView_802D6A9C()->mVisibleBox);
        spawnEmitter_8013CC50(field_0x4, &pos, 0xC5, 0, 0, NULL, NULL, true, 1.0f);
        break;
    }
    case 34:
        field_0x230 = 1;
        break;
    case 0x3E: // the first scene set's last state
        break;
    }
}

// Enters demo state `arg` (0x26-0x3D) of the first scene set: animation, camera objects, emitters
// and the talk prompts of the states that need them.
// FIXME: 99.54 %, reloc-name (float literal pool order); behaviour clean.
void PrincessZeldaDemo_8049D208::fn_8010E2D4(s32 arg) {
    switch (arg) {
    case 0x26:
        fn_801C0D70()->fn_801C11A4();
        break;
    case 0x27:
        fn_8011921C()->setField_0x2(1);
        field_0x274 = 130.0f;
        field_0x278 = 24.0f;
        break;
    case 0x28:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D3, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D4, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D5, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x4D6, 0, 0, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30079, 0);
        break;
    case 0x29:
        mDrawEnabled = true;
        fn_801C0D70()->field_0x0 |= 0x800;
        break;
    case 0x2A: {
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x2D4, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4002)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 0x2B:
        mAnim0.startAnim_801D22DC(0xF, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 0x2C:
        mAnim0.startAnim_801D22DC(0x15, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 0x2D:
        mAnim0.startAnim_801D22DC(0x14, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_801C0D70()->fn_801C115C();
        break;
    case 0x2E: {
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x2D3, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4002)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 0x2F:
        mAnim0.startAnim_801D22DC(0x13, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_801C0D70()->field_0x0 &= ~0x400;
        fn_8011921C()->fn_80118384();
        field_0x264 = 0;
        break;
    case 0x30:
        field_0x274 = 96.0f;
        getGameView_802D6A9C()->fn_802D8050(field_0x254, true, 0xFF);
        fn_8011921C()->setField_0x2(1);
        fn_8011921C()->fn_80118454();
        break;
    case 0x31:
        mAnim0.startAnim_801D22DC(0x14, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_8011921C()->fn_801183BC();
        break;
    case 0x32: {
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        fn_8022FD8C(mSerial);
        fn_8038BD08(0, 0x2D1, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 0x22);
        fn_8011921C()->fn_80118454();
        break;
    }
    case 0x33:
        mAnim0.startAnim_801D22DC(0x13, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        fn_801C0D70()->field_0x0 &= ~0x1000;
        break;
    case 0x36: {
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x2D5, field_0x4, JGeometry::TVec3f(0.0f, -10.0f, 0.0f), 0x4002)) {
            fn_8011921C()->fn_80118454();
        }
        break;
    }
    case 0x37:
        field_0x24c = 0;
        field_0x274 = 96.0f;
        field_0x278 = -16.0f;
        mAnim0.startAnim_801D22DC(0xD, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 0x38:
        mAnim0.startAnim_801D22DC(0x12, 0, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 0x39:
        vfunc_0x30(0xFF, true);
        break;
    case 0x3A:
        mAnim0.startAnim_801D22DC(8, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        break;
    case 0x3B:
        mAnim0.startAnim_801D22DC(0, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(0.5f);
        break;
    case 0x3C:
        mAnim0.startAnim_801D22DC(8, 2, false);
        mAnim0.fn_801D1F14(0.0f);
        mAnim0.mFrameCtrl.setRate(1.0f);
        break;
    case 0x3D:
        field_0x231 = 1;
        getGameView_802D6A9C()->fn_802D8050(field_0x250, true, 0xFF);
        mPos.x = 64.0f + mSpawnPos.x;
        break;
    case 0x00:
    case 0x3E:
    default:
        break;
    }
}

// Not in the binary: the plain functions below are inlined by fn_80110E9C and fn_8010E7E8, and the
// pool has their literals ahead of fn_8010E2D4's, so they stand at the bottom of the file.

// Turns the maiden to a direction (0..3) and starts that direction's standing animation.
void PrincessZeldaDemo_8049D208::faceDir(s32 dir) {
    if (dir >= 0) {
        field_0x98 = dir;
        if (dir + 6 != mAnim0.mAnim) {
            mAnim0.startAnim_801D22DC(dir + 6, 2, false);
            mAnim0.fn_801D1F14(0.0f);
        }
    }
}

// Offers the talk to the player standing at the maiden's box; the player it started with, or -1.
inline s32 PrincessZeldaDemo_8049D208::startTalk(u32 msg, bool talkFlag) {
    JGeometry::TBox2f box2 = mBox;
    box2.i.x += mPos.x;
    box2.i.y += mPos.y;
    box2.f.x += mPos.x;
    box2.f.y += mPos.y;
    fn_8022FEB8(&box2, field_0x4, mSerial, true, talkFlag);
    s32 player = fn_8022FD8C(mSerial);
    if (player < 0 || player >= 4) {
        player = -1;
    } else if (!fn_802300EC(player)) {
        player = -1;
    } else if (!fn_8023F264(player)) {
        player = -1;
    } else {
        JGeometry::TVec3f at = mPos;
        at.y += -10.0f;
        if (!fn_8038AD24(player, msg, mSerial, field_0x4, at, 0x20)) {
            player = -1;
        } else {
            field_0x23c = player;
            s32 anim = fn_801CD354(fn_80230CFC(field_0x23c)) + 6;
            mAnim0.startAnim_801D22DC(anim, 2, false);
            mAnim0.fn_801D1F14(0.0f);
            for (s32 i = 0; i < 4; i++) {
                fn_80230C6C(i);
            }
        }
    }
    return player;
}
