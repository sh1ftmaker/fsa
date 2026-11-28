#include "game/GameAudio.h"
#include "game/Camera.h"
#include "game/cls_804AE9B0.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/tu_801F9484.h"

static void (TowerAppearanceDemo_8049E770::*lbl_8049E6EC[11])() = {
    &TowerAppearanceDemo_8049E770::fn_8012C334,
    &TowerAppearanceDemo_8049E770::fn_8012C170,
    &TowerAppearanceDemo_8049E770::fn_8012C030,
    &TowerAppearanceDemo_8049E770::fn_8012BE2C,
    &TowerAppearanceDemo_8049E770::fn_8012BC40,
    &TowerAppearanceDemo_8049E770::fn_8012B218,
    &TowerAppearanceDemo_8049E770::fn_8012B0D8,
    &TowerAppearanceDemo_8049E770::fn_8012B02C,
    &TowerAppearanceDemo_8049E770::fn_8012AA84,
    &TowerAppearanceDemo_8049E770::fn_8012A9A0,
    &TowerAppearanceDemo_8049E770::fn_8012A8C0,
};

static const s32 lbl_804650B0[6] = {0x1B3, 0x1CD, 0x17F, 0x201, 0x1E7, 0x199};
static const u32 lbl_804650C8[3] = {0x840, 0x841, 0x842};
static const s16 lbl_804650D4[6] = {-0x4000, -0x1C00, 0x1C00, 0x4000, 0x6400, -0x6400};

inline bool TowerAppearanceDemo_8049E770::talk(s32 layer, const JGeometry::TVec3f& pos) {
    return fn_8038BD08(0, lbl_804650C8[field_0x254], layer, pos, 0x22);
}

// FIXME(shotgun): 99.54 %, scheduling+regalloc; behaviour clean.
void TowerAppearanceDemo_8049E770::init() {
    lbl_8053AD70->fn_8037AE00();
    field_0x3f4 = (s32)(mPos.y / 384.0f) * 384.0f;
    field_0x3f8 = (s32)(mPos.x / 512.0f) * 512.0f + 256.0f;
    mPos.x = field_0x3f8;
    mPos.y = field_0x3f4 + 250.0f;
    field_0x230 = 0;
    field_0x234 = 0;
    field_0x248 = 130.0f;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x238.set(field_0x3f8, (field_0x3f4 + 384.0f) - 102.0f, 0.0f);
    field_0x400 = 0.0f;
    field_0x3fc = 0.0f;
    for (s32 i = 0; i < 6; i++) {
        s16 angle = lbl_804650D4[i];
        field_0x25c[i] = mPos + JGeometry::TVec3f(JMACosShort(angle) * 55.0f, JMASinShort(angle) * 35.0f, 0.0f);
        field_0x2a4[i] = lbl_804650B0[i];
    }
    field_0x2bc = 0;
    field_0x2c0 = 8;
    field_0x2c4 = 0;
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    if (gAudioMgr_8053AF80->field_0x58 != 0x100001C) {
        gAudioMgr_8053AF80->stopBgm_80458F9C(60);
        field_0x258 = 60;
    }
    field_0x404 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x388[0][i] = NULL;
        field_0x388[1][i] = NULL;
        field_0x388[2][i] = NULL;
    }
    fn_80119DD0()->startFade_80119998(150, 0, 0, 0, 0, 110, 0);
    field_0x250 = 60;
    f32 posX = field_0x238.x;
    f32 posY = field_0x238.y;
    f32 halfH = field_0x248;
    f32 y0 = posY - halfH;
    f32 y1 = posY + halfH;
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->mBlendBoxOn_0x840 == 0) {
        cam->mBlendBoxOn_0x840 = 1;
        cam->mBlendBox_0x844.i.x = posX;
        cam->mBlendBox_0x844.i.y = y0;
        cam->mBlendBox_0x844.f.x = posX;
        cam->mBlendBox_0x844.f.y = y1;
        cam->mBlendWeight_0x854 = 1.0f;
        cam->mMargin_0x1d4 = -1.0f;
        cam->mMinHalfWidth_0x1d8 = -1.0f;
    }
}

void TowerAppearanceDemo_8049E770::vfunc_0x10() {}

void TowerAppearanceDemo_8049E770::update() {
    if (field_0x258 != 0) {
        field_0x258--;
    } else if (gAudioMgr_8053AF80->field_0x58 != 0x100001C) {
        gAudioMgr_8053AF80->startBgm_80458FF0(0x100001C, false, 0);
    }
    if (field_0x3f0 != 0) {
        field_0x3f0--;
    }
    if (field_0x2bc != 0) {
        field_0x2bc--;
    }
    if (field_0x250 != 0) {
        field_0x250--;
    }
    cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
    if (obj->field_0x284 == 0) {
        obj->field_0x284 = 1;
        obj->field_0x28c = 0.05f;
    }
    gGameRoot2_8053AB10->field_0x24->mWidenBounds_0x17c = 1;
    if (isStageFlagSet(lbl_80529DEC.mStage_0x118, lbl_80529DEC.mLevel_0x11c) &&
        field_0x230 >= 3 && field_0x230 <= 9) {
        gGameRoot_8053AAF8->field_0x85 = 2;
        if (lbl_8053AD30->fn_80309C4C(0x1000, -1) != 0) {
            field_0x404 = 1;
            fn_80387494(0);
            fn_80119DD0()->fn_80119958();
            field_0x230 = 9;
            field_0x250 = 0;
        }
    }
    (this->*lbl_8049E6EC[field_0x230])();
}

void TowerAppearanceDemo_8049E770::draw() {
    JGeometry::TVec3f savedPos(mPos);
    for (s32 i = 0; i < 6; i++) {
        mPos = field_0x25c[i];
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x2a4[i] + field_0x2c0, NULL, 7, 1.0f, 1.0f);
    }
    if (field_0x230 > 0) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_80230188(i)) {
                fn_80230498(i, &field_0x3b8[i], field_0x3e8 + field_0x3ec, 0xFF, -0x80, -0x80, true);
            }
        }
    }
    mPos = savedPos;
}

// An inline of its own: its loop registers are handed out before the destructor's
// (docs/REGALLOC.md, rule 4).
static inline void deleteEmitters(TowerAppearanceDemo_8049E770* self) {
    for (s32 i = 0; i < 4; i++) {
        if (self->field_0x388[0][i] != NULL) {
            self->field_0x388[0][i]->setStatus(JPAEMIT_ForceDelete);
            self->field_0x388[0][i] = NULL;
        }
        if (self->field_0x388[1][i] != NULL) {
            self->field_0x388[1][i]->setStatus(JPAEMIT_ForceDelete);
            self->field_0x388[1][i] = NULL;
        }
        if (self->field_0x388[2][i] != NULL) {
            self->field_0x388[2][i]->setStatus(JPAEMIT_ForceDelete);
            self->field_0x388[2][i] = NULL;
        }
    }
}

TowerAppearanceDemo_8049E770::~TowerAppearanceDemo_8049E770() {
    deleteEmitters(this);
}

void TowerAppearanceDemo_8049E770::fn_8012C334() {
    s32 i;
    for (i = 0; i < 4; i++) {
        if (fn_80230188(i)) {
            return;
        }
        if (!fn_802300EC(i)) {
            return;
        }
    }
    static JGeometry::TVec3f lbl_804BD790[4] = {
        JGeometry::TVec3f(field_0x3f8 - 48.0f, field_0x3f4 + 384.0f + 48.0f, 0.0f),
        JGeometry::TVec3f(field_0x3f8 - 16.0f, field_0x3f4 + 384.0f + 48.0f, 0.0f),
        JGeometry::TVec3f(field_0x3f8 + 16.0f, field_0x3f4 + 384.0f + 48.0f, 0.0f),
        JGeometry::TVec3f(field_0x3f8 + 48.0f, field_0x3f4 + 384.0f + 48.0f, 0.0f),
    };
    for (i = 0; i < 4; i++) {
        field_0x3b8[i] = lbl_804BD790[i];
        fn_8023EE28(i);
    }
    fn_8023077C();
    field_0x3e8 = 24;
    field_0x3ec = 0;
    field_0x3f0 = 6;
    for (i = 0; i < 4; i++) {
        fn_80230498(i, &field_0x3b8[i], field_0x3e8 + field_0x3ec, 0xFF, -0x80, -0x80, true);
    }
    field_0x230++;
    fn_8012A5EC();
}

void TowerAppearanceDemo_8049E770::fn_8012C170() {
    if (field_0x250 == 1) {
        fn_80119DD0()->fn_80119468(0, 0);
    }
    if (field_0x3b8[0].y >= field_0x3f4 + 325.0f) {
        field_0x3b8[0].y -= 0.5f;
        field_0x3b8[1].y -= 0.5f;
        field_0x3b8[2].y -= 0.5f;
        field_0x3b8[3].y -= 0.5f;
        if (field_0x3f0 == 0) {
            field_0x3f0 = 6;
            field_0x3ec++;
            if (field_0x3ec >= 8) {
                field_0x3ec = 0;
            }
        }
        fn_8012A5EC();
    } else {
        field_0x3b8[0].y = field_0x3f4 + 325.0f;
        field_0x3ec = 0;
        fn_8012A5EC();
        const _GXRenderModeObj* rm = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f msgPos;
        msgPos.x = (s32)rm->fbWidth / 2.0f;
        msgPos.y = (s32)rm->efbHeight / 2.0f;
        msgPos.z = 0.0f;
        msgPos.y += -16.0f;
        fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
        if (talk(mLayer_0x4, msgPos)) {
            field_0x254++;
            fn_8011921C()->fn_80118454();
            field_0x230 += 2;
        }
    }
}

void TowerAppearanceDemo_8049E770::fn_8012C030() {
    bool done = fn_8012A5EC();
    if (field_0x238.y == field_0x3f4 + 260.0f && field_0x248 == 85.0f && done == true) {
        const _GXRenderModeObj* rm = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f msgPos;
        msgPos.x = (s32)rm->fbWidth / 2.0f;
        msgPos.y = (s32)rm->efbHeight / 2.0f;
        msgPos.z = 0.0f;
        msgPos.y += -16.0f;
        fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
        if (talk(mLayer_0x4, msgPos)) {
            field_0x254++;
            fn_8011921C()->fn_80118454();
            field_0x230++;
        }
    }
}

void TowerAppearanceDemo_8049E770::fn_8012BE2C() {
    fn_8012A5EC();
    fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
    if (stepMessageWindow_803865B8(0)) {
        fn_80388EEC(0);
        fn_8011921C()->fn_801183BC();
        field_0x230 += 2;
        static const u8 order[4] = {2, 0, 3, 1};
        for (s32 i = 0; i < 4; i++) {
            field_0x2c8[i] = field_0x3b8[order[i]];
            field_0x2f8[i].x = 0.0f;
            field_0x2f8[i].y = 0.0f;
            field_0x2f8[i].z = 0.5f;
        }
        spawnEmitters();
        field_0x234 = 0;
        startSound_801F0E34(0x50062, 0);
    }
}

void TowerAppearanceDemo_8049E770::fn_8012BC40() {
    bool done = fn_8012A5EC();
    if (field_0x238.y == field_0x3f4 + 279.0f && field_0x248 == 105.0f && done == true) {
        static const u8 order[4] = {2, 0, 3, 1};
        for (s32 i = 0; i < 4; i++) {
            field_0x2c8[i] = field_0x3b8[order[i]];
            field_0x2f8[i].x = 0.0f;
            field_0x2f8[i].y = 0.0f;
            field_0x2f8[i].z = 0.5f;
        }
        spawnEmitters();
        field_0x230++;
        field_0x234 = 0;
    }
}

inline void TowerAppearanceDemo_8049E770::spawnEmitters() {
    for (s32 i = 0; i < 4; i++) {
        field_0x328[0][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3C0 + i * 3, 0, 0, NULL, NULL, true, 1.0f);
        field_0x328[1][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3C1 + i * 3, 0, 0, NULL, NULL, true, 1.0f);
        field_0x328[2][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3C2 + i * 3, 0, 0, NULL, NULL, true, 1.0f);
    }
}

// FIXME(shotgun): 98.89 %, operands; behaviour drift.
void TowerAppearanceDemo_8049E770::fn_8012B218() {
    static JGeometry::TVec3f lbl_804BD7C0[4] = {
        JGeometry::TVec3f(-113.0f, 382.0f, 50.0f),
        JGeometry::TVec3f(109.0f, 300.0f, 50.0f),
        JGeometry::TVec3f(126.0f, 382.0f, 50.0f),
        JGeometry::TVec3f(-99.0f, 300.0f, 50.0f),
    };
    fn_8012A5EC();
    switch (field_0x234) {
    case 0:
        for (s32 i = 0; i < 4; i++) {
            field_0x2c8[i] += field_0x2f8[i];
        }
        if (field_0x2c8[0].z >= 50.0f) {
            field_0x234++;
            field_0x250 = 60;
        }
        break;
    case 1:
        if (field_0x250 == 0) {
            field_0x234++;
            for (s32 i = 0; i < 4; i++) {
                field_0x2f8[i] = JGeometry::TVec3f(
                    (field_0x3f8 + lbl_804BD7C0[i].x - field_0x2c8[i].x) / 40.0f,
                    (field_0x3f4 + lbl_804BD7C0[i].y - field_0x2c8[i].y) / 40.0f,
                    0.0f);
            }
            field_0x250 = 40;
        }
        break;
    case 2:
        for (s32 i = 0; i < 4; i++) {
            field_0x2c8[i] += field_0x2f8[i];
        }
        if (field_0x250 == 0) {
            for (s32 i = 0; i < 4; i++) {
                field_0x2f8[i].zero();
                field_0x2f8[i].z = -0.2f;
                field_0x2c8[i] = lbl_804BD7C0[i] + JGeometry::TVec3f(field_0x3f8, field_0x3f4, 0.0f);
            }
            field_0x234++;
            field_0x250 = 60;
        }
        break;
    case 3:
        if (field_0x250 == 0) {
            for (s32 i = 0; i < 4; i++) {
                field_0x2c8[i] += field_0x2f8[i];
            }
            if (field_0x2c8[0].z <= 32.0f) {
                field_0x234++;
                field_0x250 = 390;
                for (s32 i = 0; i < 4; i++) {
                    if (field_0x328[0][i] != NULL) {
                        field_0x328[0][i]->setStatus(0x100);
                        field_0x328[0][i] = NULL;
                    }
                    if (field_0x328[1][i] != NULL) {
                        field_0x328[1][i]->setStatus(0x100);
                        field_0x328[1][i] = NULL;
                    }
                    if (field_0x328[2][i] != NULL) {
                        field_0x328[2][i]->setStatus(0x100);
                        field_0x328[2][i] = NULL;
                    }
                }
                for (s32 i = 0; i < 4; i++) {
                    field_0x358[0][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3B4 + i * 3, 0, 0, NULL, NULL, true, 1.0f);
                    field_0x358[1][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3B4 + i * 3 + 1, 0, 0, NULL, NULL, true, 1.0f);
                    field_0x358[2][i] = spawnEmitter_8013CC50(mLayer_0x4, &field_0x2c8[i], 0x3B4 + i * 3 + 2, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
        }
        break;
    case 4:
        if (field_0x250 == 360) {
            fn_80119DD0()->startFade_80119998(150, 0, 2, 0, 1, 150, 0);
        } else if (field_0x250 == 210) {
            for (s32 i = 0; i < 4; i++) {
                if (field_0x358[0][i] != NULL) {
                    field_0x358[0][i]->setStatus(0x100);
                    field_0x358[0][i] = NULL;
                }
                if (field_0x358[1][i] != NULL) {
                    field_0x358[1][i]->setStatus(0x100);
                    field_0x358[1][i] = NULL;
                }
                if (field_0x358[2][i] != NULL) {
                    field_0x358[2][i]->setStatus(0x100);
                    field_0x358[2][i] = NULL;
                }
            }
            for (s32 i = 0; i < 4; i++) {
                field_0x388[0][i] = spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x461 + i * 3, 1, 2, NULL, NULL, true, 1.0f);
                field_0x388[1][i] = spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x461 + i * 3 + 1, 1, 2, NULL, NULL, true, 1.0f);
                field_0x388[2][i] = spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x461 + i * 3 + 2, 1, 2, NULL, NULL, true, 1.0f);
            }
        } else if (field_0x250 == 30) {
            field_0x2c0 = 10;
        } else if (field_0x250 == 0) {
            field_0x2c0 = 10;
            field_0x234 = 0;
            const _GXRenderModeObj* rm = JUTVideo::getManager()->getRenderMode();
            JGeometry::TVec3f msgPos;
            msgPos.z = 0.0f;
            msgPos.x = (s32)rm->fbWidth * 0.5f;
            msgPos.y = (s32)rm->efbHeight * 0.5f;
            msgPos.y += -16.0f;
            fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
            if (!talk(mLayer_0x4, msgPos)) {
                return;
            }
            field_0x254++;
            fn_8011921C()->fn_80118454();
            field_0x230 += 2;
        }
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        if (field_0x328[0][i] != NULL) {
            setEmitterPos_8013CB44(field_0x328[0][i], &field_0x2c8[i], 0, 0, true);
        }
        if (field_0x328[1][i] != NULL) {
            setEmitterPos_8013CB44(field_0x328[1][i], &field_0x2c8[i], 0, 0, true);
        }
        if (field_0x328[2][i] != NULL) {
            setEmitterPos_8013CB44(field_0x328[2][i], &field_0x2c8[i], 0, 0, true);
        }
    }
}

void TowerAppearanceDemo_8049E770::fn_8012B0D8() {
    bool done = fn_8012A5EC();
    if (field_0x238.y == field_0x3f4 + 260.0f && field_0x248 == 85.0f && done == true) {
        const _GXRenderModeObj* rm = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f msgPos;
        msgPos.x = (s32)rm->fbWidth / 2.0f;
        msgPos.y = (s32)rm->efbHeight / 2.0f;
        msgPos.z = 0.0f;
        msgPos.y += -16.0f;
        fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
        if (talk(mLayer_0x4, msgPos)) {
            field_0x254++;
            fn_8011921C()->fn_80118454();
            field_0x230++;
        }
    }
}

void TowerAppearanceDemo_8049E770::fn_8012B02C() {
    fn_8012A5EC();
    fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
    if (stepMessageWindow_803865B8(0)) {
        fn_80388EEC(0);
        fn_8011921C()->fn_801183BC();
        field_0x250 = 1540;
        field_0x230++;
        field_0x244 = (field_0x238.y - (field_0x3f4 + 154.0f)) / 270.0f;
        field_0x24c = (field_0x248 - 186.0f) / 270.0f;
    }
}

// FIXME(shotgun): 99.81 %, operands; behaviour clean.
void TowerAppearanceDemo_8049E770::fn_8012AA84() {
    if (field_0x250 >= 1300) {
        if (field_0x250 == 1300) {
            spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x460, 0, -1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x46F, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x470, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &JGeometry::TVec3f(field_0x3f8, field_0x3f4 + 192.0f, 0.0f), 0x46E, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &field_0x238, 0x471, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(mLayer_0x4, &field_0x238, 0x472, 0, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x50063, 0);
            field_0x2c0 = 0;
        } else if (field_0x250 == 1309) {
            field_0x2c0 = 11;
        }
        fn_8012A5EC();
        return;
    }
    if (field_0x250 > 400) {
        if (field_0x250 >= 730) {
            if (field_0x2bc == 0) {
                field_0x2c0++;
                if (field_0x2c0 >= 6) {
                    field_0x2c0 = 0;
                }
                field_0x2bc = 9;
            }
            if (field_0x250 == 730) {
                field_0x2c0 = 14;
                field_0x2bc = 9;
                startSound_801F0E34(0x50064, 0);
            }
        } else {
            if (field_0x2bc == 0) {
                field_0x2c0++;
                if (field_0x2c0 >= 20) {
                    field_0x2c0 = 14;
                }
                field_0x2bc = 9;
            }
            f64 range = 20.0 - (field_0x250 - 400) / 16.5;
            field_0x3fc = (f32)randInt_801CD664((s32)range);
            field_0x400 = (f32)randInt_801CD664((s32)range);
            gGameRoot_8053AAF8->startRumble_80134DE4(3, 0);
        }
        fn_8012A5EC();
        return;
    }
    if (field_0x250 > 132) {
        if (field_0x2bc == 0) {
            field_0x2c0++;
            if (field_0x2c0 >= 20) {
                field_0x2c0 = 14;
            }
            field_0x2bc = 9;
        }
    } else if (field_0x250 > 123) {
        field_0x2c0 = 20;
        field_0x2bc = 0;
    } else if (field_0x250 > 63) {
        field_0x2c0 = 24;
        field_0x2bc = 0;
    } else if (field_0x250 > 60) {
        field_0x2c0 = 25;
        field_0x2bc = 0;
    } else {
        field_0x2c0 = 8;
        field_0x2bc = 0;
    }
    if (field_0x250 == 400) {
        field_0x400 = 0.0f;
        field_0x3fc = 0.0f;
        getGameView_802D6A9C()->setSwitchBit_802D8050(1, true, 0xFF);
        field_0x2c0 = 19;
        field_0x2bc = 4;
    } else if (field_0x250 == 30) {
        field_0x248 = 130.0f;
        field_0x238.y = (field_0x3f4 + 384.0f) - 102.0f;
    } else if (field_0x250 == 0) {
        const _GXRenderModeObj* rm = JUTVideo::getManager()->getRenderMode();
        JGeometry::TVec3f msgPos;
        msgPos.x = (s32)rm->fbWidth / 2.0f;
        msgPos.y = (s32)rm->efbHeight / 2.0f;
        msgPos.z = 0.0f;
        msgPos.y += -16.0f;
        fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
        if (!talk(mLayer_0x4, msgPos)) {
            return;
        }
        field_0x254++;
        fn_8011921C()->fn_80118454();
        field_0x230++;
    }
    f32 posY = field_0x238.y;
    f32 halfH = field_0x248;
    f32 posX = field_0x238.x;
    f32 y1 = posY + halfH;
    f32 y0 = posY - halfH;
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    if (cam->mBlendBoxOn_0x840 == 0) {
        cam->mBlendBoxOn_0x840 = 1;
        cam->mBlendBox_0x844.i.x = posX;
        cam->mBlendBox_0x844.i.y = y0;
        cam->mBlendBox_0x844.f.x = posX;
        cam->mBlendBox_0x844.f.y = y1;
        cam->mBlendWeight_0x854 = 1.0f;
        cam->mMargin_0x1d4 = -1.0f;
        cam->mMinHalfWidth_0x1d8 = -1.0f;
    }
}

void TowerAppearanceDemo_8049E770::fn_8012A9A0() {
    fn_8012A5EC();
    if (field_0x404 == 0) {
        fn_8022FE80(0, mLayer_0x4, mSerial, true, false);
        if (stepMessageWindow_803865B8(0)) {
            fn_80388EEC(0);
            field_0x250 = 40;
            field_0x404 = 1;
        }
    } else if (field_0x250 == 0) {
        fn_8011921C()->fn_801183BC();
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        cam->field_0x17a = 1;
        cam->field_0x179 = 0;
        getGameView_802D6A9C()->fn_802D9FF4(0, 0);
        gGameRoot_8053AAF8->fn_80136798(13, 0);
        fn_80119DD0()->fn_80119468(0, 1);
        field_0x230++;
    }
}

void TowerAppearanceDemo_8049E770::fn_8012A8C0() {
    JGeometry::TBox2f box(mBox);
    JGeometry::TBox2f baseBox(mBox);
    for (s32 i = 0; i < 6; i++) {
        box.i.x += field_0x25c[i].x;
        box.i.y += field_0x25c[i].y;
        box.f.x += field_0x25c[i].x;
        box.f.y += field_0x25c[i].y;
        fn_80239914(&box, mLayer_0x4, mSerial);
        box = baseBox;
    }
}

// FIXME(shotgun): 99.61 %, regalloc-only; behaviour drift.
bool TowerAppearanceDemo_8049E770::fn_8012A5EC() {
    f32 posY;
    f32 halfH;
    switch (field_0x230) {
    case 0:
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
        field_0x238.y = (field_0x3f4 + 384.0f) - 102.0f;
        field_0x248 = 130.0f;
        break;
    case 2: {
        f32 targetY = field_0x3f4 + 260.0f;
        if (field_0x238.y > targetY) {
            field_0x238.y = field_0x238.y - field_0x244;
        } else {
            field_0x238.y = targetY;
        }
        if (field_0x248 > 85.0f) {
            field_0x248 -= field_0x24c;
        } else {
            field_0x248 = 85.0f;
        }
        break;
    }
    case 4: {
        f32 targetY = field_0x3f4 + 279.0f;
        if (field_0x238.y < targetY) {
            field_0x238.y = field_0x238.y - field_0x244;
        } else {
            field_0x238.y = targetY;
        }
        if (field_0x248 < 105.0f) {
            field_0x248 -= field_0x24c;
        } else {
            field_0x248 = 105.0f;
        }
        break;
    }
    case 6: {
        f32 targetY = field_0x3f4 + 260.0f;
        if (field_0x238.y > targetY) {
            field_0x238.y = field_0x238.y - field_0x244;
        } else {
            field_0x238.y = targetY;
        }
        if (field_0x248 > 85.0f) {
            field_0x248 -= field_0x24c;
        } else {
            field_0x248 = 85.0f;
        }
        break;
    }
    case 8: {
        f32 targetY = field_0x3f4 + 154.0f;
        if (field_0x238.y > targetY) {
            field_0x238.y = field_0x238.y - field_0x244;
        } else {
            field_0x238.y = targetY;
        }
        if (field_0x248 < 186.0f) {
            field_0x248 -= field_0x24c;
        } else {
            field_0x248 = 186.0f;
        }
        break;
    }
    }
    f32 posX = field_0x238.x + field_0x3fc;
    posY = field_0x238.y + field_0x400;
    halfH = field_0x248;
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    f32 y0 = posY - halfH;
    f32 y1 = halfH + posY;
    if (field_0x3fc != 0.0f || field_0x400 != 0.0f || field_0x230 == 0 || field_0x230 == 1) {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->mBlendBoxOn_0x840 == 0) {
            cam->mBlendBoxOn_0x840 = 1;
            cam->mBlendBox_0x844.i.x = posX;
            cam->mBlendBox_0x844.i.y = y0;
            cam->mBlendBox_0x844.f.x = posX;
            cam->mBlendBox_0x844.f.y = y1;
            cam->mBlendWeight_0x854 = 1.0f;
            cam->mMargin_0x1d4 = -1.0f;
            cam->mMinHalfWidth_0x1d8 = -1.0f;
        }
    } else {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->mShowWholeArea_0x7f5 == 0) {
            cam->mFocusOverride_0x7f4 = 1;
            cam->mFocusMode_0x7f8 = 5;
            cam->mFocusBox_0x808.i.x = posX;
            cam->mFocusBox_0x808.i.y = y0;
            cam->mFocusBox_0x808.f.x = posX;
            cam->mFocusBox_0x808.f.y = y1;
            cam->mOverrideBlend_0x7f6 = 0;
        }
    }
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    Vec camPos;
    camPos.x = -cam->mViewMtx_0x4[0][3];
    camPos.y = -cam->mViewMtx_0x4[1][3];
    camPos.z = 0.0f;
    return PSVECDistance(&field_0x238, &camPos) < 0.1f;
}
