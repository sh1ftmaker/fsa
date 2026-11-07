#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_searchlight.h"
#include "std/math.h"
#include "JSystem/JUtility/TColor.h"

// Two dummy ids searchlights share with tu_8018A8D8 (both -1 until a searchlight is made).
s32 lbl_80539F80 = -1;
s32 lbl_80539F84 = -1;

// 0x80179F64.
// FIXME(shotgun): 99.75 %, reloc-name
void Searchlight_804A0FF8::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x328 = (field_0x90 >> 4) & 0xF;
    field_0x32c = (field_0x90 >> 8) & 0xF;
    field_0x330 = (field_0x90 >> 12) & 0xF;
    field_0x334 = (field_0x90 >> 16) & 0xFF;
    field_0x90 = field_0x90 & 0xF;
    field_0x338 = 0;
    field_0x33c = 0;
    lbl_804C0EA0[2] = 0.0f;
    lbl_804C0EA0[1] = 0.0f;
    lbl_804C0EA0[0] = 0.0f;
    lbl_8053AB68 = -1;
    lbl_80539F80 = -1;
    lbl_80539F84 = -1;
    field_0x30c = 0;
    field_0x310 = 0;
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 0;
    field_0x320 = 0;
    field_0x300 = 0;
    field_0x304 = 0;
    field_0x308 = 0;
    field_0x324 = 0;
    field_0x344 = 32.0f;
    field_0x370.z = 0.0f;
    field_0x370.y = 0.0f;
    field_0x370.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x34c.x = mPos.x;
    field_0x34c.y = mPos.y;
    field_0x34c.z = mPos.z;
    field_0x358.x = mPos.x;
    field_0x358.y = mPos.y;
    field_0x358.z = mPos.z;
    field_0x364.x = mPos.x;
    field_0x364.y = mPos.y;
    field_0x364.z = mPos.z;
    field_0x348 = 50.0f;
    field_0x30c = 100;
    switch (field_0x90) {
    case 0:
    case 4:
    case 8:
        field_0x98 = 3;
        break;
    case 1:
    case 5:
    case 9:
        field_0x98 = 2;
        break;
    case 2:
    case 6:
    case 10:
        field_0x98 = 1;
        break;
    case 3:
    case 7:
    case 11:
        field_0x98 = 0;
        break;
    }
    if (field_0x90 >= 4 && field_0x90 <= 7) {
        field_0x30c = -100;
    }
    field_0x340 = fn_801F6874(field_0x98);
    if (field_0x90 == 12) {
        field_0x2f8 = 2;
        field_0x2fc = field_0x2f8;
    } else if (field_0x90 == 13) {
        field_0x2f8 = 4;
        field_0x2fc = field_0x2f8;
    } else {
        if (field_0x90 >= 4 && field_0x90 <= 11) {
            mPos.x = field_0x348 * JMath::sincosTable_.sinShort(field_0x340) + field_0x34c.x;
            mPos.y = field_0x348 * JMath::sincosTable_.cosShort(field_0x340) + field_0x34c.y;
            field_0x2f8 = 1;
        } else {
            field_0x300 = 300;
            field_0x370.x = 0.5f * JMath::sincosTable_.sinShort(field_0x340);
            field_0x370.y = 0.5f * JMath::sincosTable_.cosShort(field_0x340);
            field_0x2f8 = 0;
        }
        field_0x2fc = field_0x2f8;
    }
}

// 0x80179388.
// FIXME(shotgun): 97.4 %, missing/extra (two extra clrlwi narrowing field_0x334 / fn_800FE21C's u8 result in case 2)
void Searchlight_804A0FF8::update() {
    f32 dist = 0.0f;
    s32 player = 0;
    if (field_0x2f8 != 3) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x370.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x370.y, 0.3f, 1.0f);
    }
    mPos += field_0x3c;
    switch (field_0x2f8) {
    case 0:
        if (field_0x300 == 0) {
            field_0x300 = 300;
            field_0x370.x *= -1.0f;
            field_0x370.y *= -1.0f;
        }
        break;
    case 1:
        field_0x340 += (u16)field_0x30c;
        mPos.x = field_0x348 * JMath::sincosTable_.sinShort(field_0x340) + field_0x34c.x;
        mPos.y = field_0x348 * JMath::sincosTable_.cosShort(field_0x340) + field_0x34c.y;
        break;
    case 2: {
        u8 points[0x20];
        if (field_0x230.fn_800FE3EC(field_0x334, mPos, field_0x4)) {
            field_0x338 = field_0x230.fn_800FE21C(0xFF);
            field_0x338 = field_0x338 * 60;
            if (field_0x338 == 0) {
                field_0x338 = 3;
            }
            field_0x33c = field_0x230.fn_800FD894(field_0x4, points, 0x20);
            field_0x314 = 0;
            field_0x318 = 0;
            field_0x2f8 = 3;
            field_0x2fc = field_0x2f8;
        }
    }
    case 3: {
        u8 points[0x20];
        field_0x3c = field_0x230.fn_800FD998(field_0x358, field_0x314, field_0x338, 0xFF);
        field_0x314++;
        if (field_0x314 > field_0x338) {
            field_0x314 = 0;
            field_0x318++;
            if (field_0x318 >= field_0x33c) {
                field_0x318 = 0;
            }
            field_0x230.fn_800FD894(field_0x4, points, 0x20);
            u8 point = points[field_0x318];
            if (point < field_0x230.field_0x2) {
                field_0x230.field_0x3 = point;
            }
            field_0x358.x = mPos.x;
            field_0x358.y = mPos.y;
            field_0x358.z = mPos.z;
        }
        break;
    }
    case 4:
        field_0x2f8 = 5;
    case 5:
        fn_801EEC98(mSerial, NULL, NULL, &dist, &player, 0, 0);
        break;
    case 6:
        if (field_0x328 != 0) {
            gAudioMgr_8053AF80->fn_80458880(0x36, -1);
        }
        if (lbl_80539F80 >= 0 && field_0x4 == getPlayerLayer_8023DE58(lbl_80539F80)) {
            f32 dx = getPlayerPos_8023E724(lbl_80539F80)->x - mPos.x;
            f32 dy = getPlayerPos_8023E724(lbl_80539F80)->y - mPos.y;
            if (std::sqrtf(dx * dx + dy * dy) > 5.0f) {
                field_0x370.x = 5.0f * JMath::sincosTable_.sinShort((s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
                field_0x370.y = 5.0f * JMath::sincosTable_.cosShort((s32)(JMath::atanTable_.atan2_(dx, dy) * 10430.378f));
            } else {
                JGeometry::TVec3f* target = getPlayerPos_8023E724(lbl_80539F80);
                mPos.x = target->x;
                mPos.y = target->y;
                mPos.z = target->z;
                field_0x370.z = 0.0f;
                field_0x370.y = 0.0f;
                field_0x370.x = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                if (field_0x300 == 0 && field_0x310 != 0) {
                    getActorManager_801F856C()->fn_801F6884();
                    if (field_0x328 == 0) {
                        for (s32 i = 0; i < 20; i++) {
                            JGeometry::TVec3f pos = mPos;
                            pos.x = 80.0f * JMath::sincosTable_.sinShort((i + 1) * 0xC30) + mPos.x;
                            pos.y = 80.0f * JMath::sincosTable_.cosShort((i + 1) * 0xC30) + mPos.y;
                            if (getActorManager_801F856C()->spawnActor_801F9484('POOH', field_0x4, &pos, 0, -1, -1) >= 0) {
                                spawnEmitter_8013CC50(field_0x4, &pos, 0x1FD, 1, 1, NULL, NULL, true, 1.0f);
                            }
                        }
                        startSound_801F0E34(0x50067, 0);
                    } else {
                        getGameView_802D6A9C()->fn_802D9FF4(field_0x32c, field_0x330);
                        gGameRoot_8053AAF8->fn_80136798(0xE, 0);
                    }
                    field_0x310 = 0;
                }
            }
        } else {
            getActorManager_801F856C()->fn_801F6884();
        }
        break;
    case 7:
        if (lbl_80539F84 >= 0 && lbl_8053AB68 == field_0x4) {
            f32 dx = mPos.x - lbl_804C0EA0[0];
            f32 dy = mPos.y - lbl_804C0EA0[1];
            s32 angle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
            field_0x370.x = 5.0f * JMath::sincosTable_.sinShort(angle);
            field_0x370.y = 5.0f * JMath::sincosTable_.cosShort(angle);
            if (std::sqrtf(dx * dx + dy * dy) < 10.0f) {
                field_0x370.z = 0.0f;
                field_0x370.y = 0.0f;
                field_0x370.x = 0.0f;
                field_0x3c.z = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.x = 0.0f;
                field_0x31c++;
            }
        }
        if (field_0x31c > 60 || (lbl_804C0EA0[0] == 0.0f && lbl_804C0EA0[1] == 0.0f)) {
            lbl_80539F84 = -1;
            lbl_804C0EA0[2] = 0.0f;
            lbl_804C0EA0[1] = 0.0f;
            lbl_804C0EA0[0] = 0.0f;
            field_0x31c = 0;
            field_0x358.x = mPos.x;
            field_0x358.y = mPos.y;
            field_0x358.z = mPos.z;
            field_0x2f8 = 8;
            if (field_0x2fc == 3) {
                field_0x2f8 = field_0x2fc;
            }
        }
        break;
    case 8: {
        f32 dx = mPos.x - field_0x364.x;
        f32 dy = mPos.y - field_0x364.y;
        s32 angle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
        field_0x370.x = 5.0f * JMath::sincosTable_.sinShort(angle);
        field_0x370.y = 5.0f * JMath::sincosTable_.cosShort(angle);
        if (std::sqrtf(dx * dx + dy * dy) < 10.0f) {
            field_0x370.z = 0.0f;
            field_0x370.y = 0.0f;
            field_0x370.x = 0.0f;
            field_0x2f8 = field_0x2fc;
        }
        break;
    }
    }
    if (field_0x2f8 == 0 || field_0x2f8 == 1 || field_0x2f8 == 3 || field_0x2f8 == 7 || field_0x2f8 == 8) {
        fn_801EEC98(mSerial, NULL, NULL, &dist, &player, 0, 0);
        if (field_0x320 != 0) {
            if (getActorManager_801F856C()->fn_801F68E0() == 0) {
                return;
            }
            field_0x310 = 1;
            field_0x320 = 0;
        }
        if ((lbl_80539F80 >= 0 && field_0x320 == 0) ||
            (player >= 0 && dist < field_0x344 - 28.0f && !fn_8022B7C4(player))) {
            if (lbl_80539F80 == -1) {
                lbl_80539F80 = player;
                if (getActorManager_801F856C()->fn_801F68E0() == 0) {
                    field_0x320 = 1;
                    return;
                }
                field_0x320 = 0;
                field_0x310 = 1;
            }
            if (field_0x328 != 0) {
                gAudioMgr_8053AF80->fn_80458F9C(0x3C);
                field_0x300 = 0x104;
            } else {
                field_0x300 = 100;
            }
            field_0x2f8 = 6;
        } else if (field_0x2f8 != 7 && field_0x2f8 != 8 && lbl_8053AB68 == field_0x4 &&
                   (lbl_80539F84 >= 0 || lbl_804C0EA0[0] != 0.0f || lbl_804C0EA0[1] != 0.0f)) {
            lbl_80539F84 = 1;
            field_0x364.x = mPos.x;
            field_0x364.y = mPos.y;
            field_0x364.z = mPos.z;
            field_0x31c = 0;
            field_0x2f8 = 7;
        }
    }
    if (field_0x300 > 0) {
        field_0x300--;
    }
    if (field_0x304 > 0) {
        field_0x304--;
    }
    if (field_0x308 > 0) {
        field_0x308--;
    }
}

// 0x801791B0.
// FIXME(shotgun): 87.37 %, different
void Searchlight_804A0FF8::draw() {
    if (field_0x90 == 0xD) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -1;
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x431, &params, -1, 1.0f, 1.0f);
        fn_801F02BC("enemy_gc_sob.bin", 0x431, &params, -1, 0, false, 1.0f, 1.0f);
    } else {
        gGameRoot2_8053AB10->field_0x2c->fn_80158624(&field_0x34c, &mPos, JUtility::TColor(0x3F3F1F7F), 0.7f * field_0x344);
        gGameRoot2_8053AB10->field_0x2c->fn_80158624(&field_0x34c, &mPos, JUtility::TColor(0x3F3F1F3F), field_0x344);
    }
}

// Destructor at 0x80179150.
Searchlight_804A0FF8::~Searchlight_804A0FF8() {}
