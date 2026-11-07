#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/SpriteParams.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "std/math.h"

// 0x80171AF8.
// FIXME(shotgun): 90.12 %, missing/extra + reloc-name
void cls_804A0810::init() {
    field_0x94 = field_0x90 & 0xFF;
    field_0x270 = (field_0x90 >> 8) & 1;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    for (s32 i = 0; i < 10; i++) {
        field_0x29c[i] = mPos;
    }
    field_0x290[2] = 0.0f;
    field_0x290[1] = 0.0f;
    field_0x290[0] = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x280 = 0.3f;
    field_0x278 = 0.0f;
    field_0x27c = 1.5f;
    field_0xb0 = 0xA002204;
    field_0xbc = 3;
    field_0x28c = NULL;
    field_0x264 = 0;
    if (fn_802DE814() == true) {
        mLife = 0x10;
    }
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3) {
        mLife = 0x18;
    }
    field_0x268 = mLife;
    field_0x258 = 0;
    field_0x284 = 0.0f;
    field_0x288 = 70.0f;
    field_0x118 = 0x10;
    field_0x32c[0] = mPos.x;
    field_0x32c[1] = mPos.y;
    field_0x32c[2] = mPos.z;
    if (lbl_80529DEC.field_0x118 == 0 && lbl_80529DEC.field_0x11c == 3 && getGameView_802D6A9C()->field_0x18 == 3 &&
        getGameView_802D6A9C()->field_0x14 == 1) {
        field_0x244 = 100;
    }
    field_0x260 = 0;
    field_0x25c = 0;
    field_0x98 = 2;
    field_0x250 = 0x47E;
    field_0x23c = (s32)(100.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 100;
    field_0x274 = 8;
    field_0x26c = 0;
    field_0x230 = 0;
    switch (field_0x94) {
    case 0:
        field_0x250 = 0x190;
        break;
    case 1:
        field_0x274 = 4;
        field_0x27c = 1.0f;
        field_0x288 = 10.0f;
        field_0x230 = 1;
        break;
    case 2:
        field_0x274 = 2;
        break;
    case 3:
        field_0x274 = 6;
        field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
        field_0x250 = 100;
        field_0x27c = 1.0f;
        field_0x288 = 10.0f;
        field_0x230 = 2;
        break;
    case 4:
        field_0x23c = 50;
        field_0x274 = 3;
        field_0x230 = 3;
        break;
    case 5:
        field_0x114 = 4;
        mLife = 10000;
        field_0x1a8 = false;
        break;
    }
    if ((field_0x90 >> 27) != 0) {
        field_0xb0 = 0;
    } else if (field_0x94 != 5) {
        field_0x26c = getActorManager_801F856C()->spawnActor_801F9484('SLR3', field_0x4, &mPos, 5, -1, -1);
        if (gGameRoot_8053AAF8->field_0x7f == 1) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
        }
    }
}

// 0x80171A28.
cls_804A0810::~cls_804A0810() {
    if (field_0x94 != 5) {
        if (gGameRoot_8053AAF8->field_0x4c != 9 && gGameRoot_8053AAF8->field_0x4c != 0xB && mLife != 0) {
            gGameRoot_8053AAF8->fn_80138B7C();
        }
        s32 serial = field_0x26c;
        if (serial != 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
    releaseEmitter_801EE434(field_0x28c);
}

// 0x801719C0.
void cls_804A0810::vfunc_0x44() {
    if (mLife <= 0) {
        s32 serial = field_0x26c;
        if (serial != 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
                field_0x26c = 0;
            }
        }
    }
}

static s32 lbl_804A07D8_frames[4] = { 0, 1, 2, 1 };
static s32 lbl_804A07D8_timers[4] = { 10, 2, 10, 10 };
static f32 lbl_804A07D8_offsets[3][2] = {
    { -8.0f, 1.0f },
    { -9.0f, 2.0f },
    { -9.0f, 4.0f },
};

// 0x80170450.
// FIXME(shotgun): 94.96 %, missing/extra
void cls_804A0810::update() {

    u16 dirAngle = 0;
    f32 targetDist = 0.0f;
    s32 targetPlayer = 0;
    s32 moveResult = 0;
    JGeometry::TVec3f savedPos = mPos;
    f32 origY = mPos.y;
    f32 origX = mPos.x;
    f32 boxX0 = mBox.i.x;
    f32 boxY0 = mBox.i.y;
    f32 boxX1 = mBox.f.x;
    f32 boxY1 = mBox.f.y;

    if (field_0x94 == 5) {
        mLife = 10000;
        fn_801F2B20(0);
        fn_801F2B7C(2, -1, 3);
        return;
    }

    if ((field_0x90 >> 27) != 0) {
        if (!vfunc_0x34()) {
            return;
        }
        if (field_0x26c == 0) {
            if (gGameRoot_8053AAF8->field_0x7f == 1) {
                gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
            }
            field_0x26c = getActorManager_801F856C()->spawnActor_801F9484('SLR3', field_0x4, &mPos, 5, -1, -1);
        }
        if (field_0xb0 == 0) {
            startSound_801F0E34(0x40087, 0);
        }
        field_0xb0 = 0xA002204;
    }

    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    field_0x278 += 0.2f;
    f32 offX = lbl_804A07D8_offsets[field_0x258][0];
    f32 offY = lbl_804A07D8_offsets[field_0x258][1];

    if (field_0x260 == 0) {
        field_0x314 = mPos;
        field_0x314.x += field_0x284 * JMath::sincosTable_.sinShort(field_0x24c) + offX;
        field_0x314.y += field_0x284 * JMath::sincosTable_.cosShort(field_0x24c) + offY;
        field_0x314.z = 16.0f;
        field_0x320.x = field_0x314.x;
        field_0x320.y = field_0x314.y;
        field_0x320.z = field_0x314.z;
        field_0x24c += field_0x250;
        if ((field_0x24c & 0xF000) == 0) {
            if (field_0x254 == 0) {
                startSound_801F0E34(0x50044, 0);
                field_0x254 = 1;
            }
        } else {
            field_0x254 = 0;
        }
    }

    if (field_0x244 == 0) {
        addCalc2_801F71A4(&field_0x284, field_0x288, 0.7f, 3.0f);
    }

    f32 stepX = ((mPos.x + offX) - field_0x314.x) / 10.0f;
    f32 stepY = ((mPos.y + offY) - field_0x314.y) / 10.0f;
    for (s32 i = 0; i < 10; i++) {
        field_0x29c[i] = mPos;
        field_0x29c[i].x -= stepX * (f32)i;
        field_0x29c[i].y -= stepY * (f32)i;
        field_0x29c[i].x += offX;
        field_0x29c[i].y += offY;
        field_0x29c[i].z = 16.0f;
    }

    mPos = field_0x314;
    if (field_0x26c != 0) {
        ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
        if (helper != NULL) {
            helper->mPos = field_0x314;
        }
    }

    if (mLife > 0) {
        if (field_0x240 == 1) {
            startSound_801F0E34(0x40087, 0);
        }
        for (s32 i = 0; i < 4; i++) {
            targetPlayer = fn_801F285C(NULL, NULL, true, i);
            if (targetPlayer >= 0) {
                boxX0 += origX;
                boxX1 += origX;
                boxY0 += origY;
                boxY1 += origY;
                JGeometry::TVec2f center(boxX1 + 0.5f * (boxX0 - boxX1), boxY1 + 0.5f * (boxY0 - boxY1));
                if (fn_80237948(targetPlayer, &center)) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x117, 0, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x118, 0, 1, NULL, NULL, true, 1.0f);
                    startSound_801F0E34(0x400D1, 0);
                }
            }
        }
    }

    mPos.x = savedPos.x;
    mPos.y = savedPos.y;
    mPos.z = savedPos.z;

    if (fn_801F3D94(1)) {
        if (field_0x260 != 0) {
            field_0x320.x = mPos.x;
            field_0x320.y = mPos.y;
            field_0x320.z = mPos.z;
            field_0x320.x += field_0x284 * JMath::sincosTable_.sinShort(field_0x24c) + offX;
            field_0x320.y += field_0x284 * JMath::sincosTable_.cosShort(field_0x24c) + offY;
            f32 dx = fabs(field_0x314.x - field_0x320.x);
            f32 dy = fabs(field_0x314.y - field_0x320.y);
            addCalc2_801F71A4(&field_0x314.x, field_0x320.x, 0.1f, 0.1f * dx);
            addCalc2_801F71A4(&field_0x314.y, field_0x320.y, 0.1f, 0.1f * dy);
            if (field_0x26c != 0) {
                ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                if (helper != NULL) {
                    helper->mPos = field_0x314;
                }
            }
        }
        fn_801F2618(NULL, false);
        if (mLife <= 0) {
            if (field_0x26c != 0) {
                ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                if (helper != NULL) {
                    helper->mAlive = false;
                    field_0x26c = 0;
                }
            }
            return;
        }
    }

    fn_801EEC98(mSerial, &dirAngle, &moveResult, &targetDist, &targetPlayer, 0, 0);
    if (targetPlayer >= 0) {
        JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(targetPlayer);
        field_0x32c[0] = playerPos->x;
        field_0x32c[1] = playerPos->y;
        field_0x32c[2] = playerPos->z;
    }

    if (getGameView_802D6A9C()->field_0x131df[field_0x4] != 0) {
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        field_0x3c.z = 0.0f;
        field_0x290[1] = 3.0f;
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x290[0], 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x290[1], 0.3f, 1.0f);
    mPos += field_0x3c;

    if (field_0x230 != 4) {
        bool hit = false;
        if (field_0x270 == 0 && moveResult == -1) {
            hit = true;
        }
        if (fn_801F2FAC()) {
            hit = true;
        }
        if (hit) {
            field_0x290[2] = 0.0f;
            field_0x290[1] = 0.0f;
            field_0x290[0] = 0.0f;
            field_0x288 = 10.0f;
            field_0x240 = 30;
            switch (field_0x94) {
            case 1:
                field_0x23c = 90;
                field_0x25c = 1;
                field_0x27c = 1.0f;
                break;
            case 2:
                break;
            case 3:
                field_0x250 = 100;
                field_0x25c = 0;
                field_0x27c = 1.0f;
                break;
            }
            if (mLife <= 0 && field_0x26c != 0) {
                ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                if (helper != NULL) {
                    helper->mAlive = false;
                    field_0x26c = 0;
                }
            }
            if (field_0x94 == 0 && field_0x268 != mLife) {
                if (field_0x264 < 2) {
                    field_0x250 = (s32)(1.9f * (f32)field_0x250);
                    field_0x264++;
                }
                field_0x268 = mLife;
            }
            field_0x230 = 4;
        }

        if (gGameRoot_8053AAF8->field_0x7f == 0) {
            s32 prevLife = mLife;
            f32 prevVelX = field_0x3c.x;
            f32 prevVelY = field_0x3c.y;
            f32 prevVelZ = field_0x3c.z;
            if (fn_801F2DB4('HBMB', 'NULL', NULL) && prevLife != mLife) {
                field_0x3c.x = prevVelX;
                field_0x3c.y = prevVelY;
                field_0x3c.z = prevVelZ;
                field_0xd8 = 0;
                field_0xdc = 0;
            }
        }
    }

    fn_801F2B7C(-100, -1, 1);
    fn_801F2618(NULL, false);

    switch (field_0x230) {
    case 0:
        if (field_0x94 == 2) {
            field_0x284 = targetDist;
            if (field_0x284 > 100.0f) {
                field_0x284 = 100.0f;
            }
        } else if (field_0x240 == 0) {
            field_0x288 = 70.0f;
        }
        break;
    case 1:
        switch (field_0x25c) {
        case 0:
            if (field_0x23c == 0) {
                startSound_801F0E34(0x40087, 0);
                field_0x23c = 90;
                field_0x250 = 3500;
                field_0x290[2] = 0.0f;
                field_0x290[1] = 0.0f;
                field_0x290[0] = 0.0f;
                field_0x25c++;
            }
            break;
        case 1:
            if (field_0x23c == 0) {
                field_0x23c = 60;
                field_0x250 = 1500;
                field_0x25c++;
            }
            break;
        case 2:
            addCalc2_801F71A4(&field_0x288, 100.0f, 0.3f, 3.0f);
            addCalc2_801F71A4(&field_0x27c, 2.0f, 0.2f, 0.2f);
            if (field_0x23c == 0) {
                field_0x25c++;
            }
            break;
        case 3:
            addCalc2_801F71A4(&field_0x288, 10.0f, 0.3f, 3.0f);
            addCalc2_801F71A4(&field_0x27c, 1.0f, 0.2f, 0.2f);
            if (fabs(field_0x288 - 10.0f) < 0.1f) {
                field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x288 = 10.0f;
                field_0x27c = 1.0f;
                field_0x25c = 0;
            }
            break;
        }
        break;
    case 2:
        switch (field_0x25c) {
        case 0:
            field_0x250 += 100;
            if (field_0x250 > 3000) {
                field_0x250 = 3000;
            }
            if (field_0x23c == 0) {
                Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
                boxX0 += origX;
                boxY0 += origY;
                boxX1 += origX;
                boxY1 += origY;
                JGeometry::TVec3f p0(boxX0, boxY0, 0.0f);
                bool inView = false;
                if (cam->fn_8030C2A8(&p0)) {
                    JGeometry::TVec3f p1(boxX1, boxY1, 0.0f);
                    if (cam->fn_8030C2A8(&p1)) {
                        inView = true;
                    }
                }
                if (inView) {
                    if (targetPlayer >= 0) {
                        field_0x320 = *getPlayerPos_8023E724(targetPlayer);
                        if (field_0x26c != 0) {
                            ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                            if (helper != NULL) {
                                helper->mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
                            }
                        }
                    }
                    field_0x290[2] = 0.0f;
                    field_0x290[1] = 0.0f;
                    field_0x290[0] = 0.0f;
                    field_0x260 = 1;
                    field_0x25c++;
                    startSound_801F0E34(0x40087, 0);
                }
            }
            break;
        case 1: {
            f32 dx = fabs(field_0x314.x - field_0x320.x);
            f32 dy = fabs(field_0x314.y - field_0x320.y);
            addCalc2_801F71A4(&field_0x314.x, field_0x320.x, 0.1f, 0.1f * dx);
            addCalc2_801F71A4(&field_0x314.y, field_0x320.y, 0.1f, 0.1f * dy);
            gBgRewrite_8053AC98->fn_802A1688(&field_0x314, field_0x4, 'NULL');
            f32 diffX = field_0x314.x - field_0x320.x;
            f32 diffY = field_0x314.y - field_0x320.y;
            addCalc2_801F71A4(&field_0x27c, 2.0f, 0.1f, 0.1f);
            if (std::sqrtf(diffX * diffX + diffY * diffY) < 2.0f) {
                field_0x27c = 2.0f;
                field_0x320 = mPos;
                field_0x320.x += field_0x284 * JMath::sincosTable_.sinShort(field_0x24c) + offX;
                field_0x320.y += field_0x284 * JMath::sincosTable_.cosShort(field_0x24c) + offY;
                field_0x23c = 20;
                field_0x25c++;
            }
            break;
        }
        case 2:
            if (field_0x23c == 0) {
                f32 dx = fabs(field_0x314.x - field_0x320.x);
                f32 dy = fabs(field_0x314.y - field_0x320.y);
                addCalc2_801F71A4(&field_0x314.x, field_0x320.x, 0.1f, 0.1f * dx);
                addCalc2_801F71A4(&field_0x314.y, field_0x320.y, 0.1f, 0.1f * dy);
                addCalc2_801F71A4(&field_0x27c, 1.0f, 0.1f, 0.1f);
                f32 diffY = field_0x314.y - field_0x320.y;
                f32 diffX = field_0x314.x - field_0x320.x;
                if (std::sqrtf(diffX * diffX + diffY * diffY) < 2.0f) {
                    field_0x27c = 1.0f;
                    field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                    field_0x250 = 100;
                    field_0x260 = 0;
                    field_0x25c = 0;
                    if (field_0x26c != 0) {
                        ActorBase_804A5770* helper = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x26c);
                        if (helper != NULL) {
                            helper->mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
                        }
                    }
                }
            }
            break;
        }
        break;
    case 3:
        if (field_0x240 == 0) {
            field_0x288 = 70.0f;
            if (field_0x23c == 0) {
                if (field_0x28c == NULL) {
                    field_0x28c = spawnEmitter_8013CC50(field_0x4, &field_0x314, 0x19F, 0, 2, NULL, NULL, true, 0.3f);
                } else {
                    field_0x28c->setGlobalTranslation(field_0x314.x, field_0x314.y + 16.0f, field_0x314.z);
                }
                getActorManager_801F856C()->spawnActor_801F9484('BLZE', field_0x4, &field_0x314, 0x80C0, -1, -1);
                field_0x23c = 2;
            }
        }
        break;
    case 4:
        if (moveResult != -1) {
            field_0x230 = 0;
            if (field_0x94 == 1) {
                field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x230 = 1;
            }
            if (field_0x94 == 3) {
                field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x260 = 0;
                field_0x250 = 100;
                field_0x27c = 1.0f;
                field_0x288 = 10.0f;
                field_0x230 = 2;
            }
            if (field_0x94 == 4) {
                field_0x23c = 0;
                field_0x230 = 3;
            }
        }
        break;
    }

    if (field_0x230 != 4 && field_0x260 == 0) {
        if (moveResult != -1) {
            if (field_0x230 != 1 || field_0x25c != 1) {
                field_0x290[0] = field_0x280 * JMath::sincosTable_.sinShort(dirAngle);
                field_0x290[1] = field_0x280 * JMath::sincosTable_.cosShort(dirAngle);
            }
        } else if (field_0x270 == 1) {
            f32 dx = field_0x32c[0] - mPos.x;
            f32 dy = field_0x32c[1] - mPos.y;
            field_0x290[0] = field_0x280 * JMath::sincosTable_.sinShort((s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
            field_0x290[1] = field_0x280 * JMath::sincosTable_.cosShort((s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
        }
        if (field_0x238 == 0) {
            field_0x248++;
            field_0x248 &= 3;
            field_0x238 = lbl_804A07D8_timers[field_0x248];
            field_0x258 = lbl_804A07D8_frames[field_0x248];
        }
    }

    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
}

// Draws the body (frame 0x7F + field_0x258), the ten trail segments (frame 0x83, at the stored
// positions) and, rotated by field_0x278 and scaled by field_0x27c, the shell (frame 0x82 or 0x11).
// FIXME(shotgun): 92 %, missing/extra
void cls_804A0810::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    f32 x = mPos.x;
    f32 y = mPos.y;
    f32 z = mPos.z;
    if (field_0x94 != 5) {
        if ((field_0x90 >> 27) == 0 || vfunc_0x34()) {
            params.field_0x0[8] = field_0x274;
            drawSprite_801F06F0("enemy_gc_sob.bin", field_0x258 + 0x7F, &params, 7, 1.0f, 1.0f);
            if (field_0x244 == 0) {
                params.field_0x1d = 1;
            } else {
                params.field_0x20 = 0;
            }
            params.field_0x0[6] = 6;
            for (s32 i = 0; i < 10; i++) {
                mtx[0][0] = 0.7f;
                mtx[0][1] = 0.0f;
                mtx[0][2] = 0.0f;
                mtx[1][0] = 0.0f;
                mtx[1][1] = 0.7f;
                mtx[1][2] = 0.0f;
                mtx[2][0] = 0.0f;
                mtx[2][1] = 0.0f;
                mtx[2][2] = 0.7f;
                params.mMtx = mtx;
                mPos = field_0x29c[i];
                drawSprite_801F06F0("enemy_gc_sob.bin", 0x83, &params, 7, 1.0f, 1.0f);
            }
            mPos.x = x;
            mPos.y = y;
            mPos.z = z;
            if (field_0x244 == 0) {
                params.field_0x1d = 2;
            } else {
                params.field_0x20 = 0;
            }
            f32 angle = field_0x278;
            f32 scale = field_0x27c;
            f32 sa = sin(0.0f);
            f32 sb = sin(angle);
            f32 sc = sin(0.0f);
            f32 ca = cos(0.0f);
            f32 cb = cos(angle);
            f32 cc = cos(0.0f);
            params.field_0x30 = 0x11;
            params.mMtx = mtx;
            mtx[0][0] = scale * (cb * cc);
            mtx[1][0] = scale * (cb * sc);
            mtx[2][0] = scale * -sb;
            mtx[0][1] = scale * ((sa * sb) * cc - ca * sc);
            mtx[1][1] = scale * ((sa * sb) * sc + ca * cc);
            mtx[2][1] = scale * (sa * cb);
            mtx[0][2] = scale * ((ca * cc) * sb + sa * sc);
            mtx[1][2] = scale * ((ca * sc) * sb - sa * cc);
            mtx[2][2] = scale * (ca * cb);
            if (field_0x94 == 4) {
                params.field_0x0[6] = field_0x274;
                params.field_0x1d = 0;
            }
            mPos = field_0x314;
            drawSprite_801F06F0("enemy_gc_sob.bin", 0x82, &params, 7, 1.0f, 1.0f);
            mPos.x = x;
            mPos.y = y;
            mPos.z = z;
        }
    }
}
