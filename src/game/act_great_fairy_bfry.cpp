#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/Player.h"
#include "game/Message.h"
#include "game/SpriteParams.h"
#include "game/act/act_great_fairy_bfry.h"

// Sprite frames of the fairy kinds (.rodata 0x804755E0 / 0x80475600, not written as source here).
extern const s32 lbl_80475600[];

GreatFairy_804AF338::~GreatFairy_804AF338() {
    releaseEmitter_801EE434(field_0x2a0[0]);
    releaseEmitter_801EE434(field_0x2a0[1]);
    if (field_0x90 != 3) {
        for (s32 i = 0; i < 4; i++) {
            if (field_0x24c[i] == 1) {
                fn_803037D4(i, 0x12);
                field_0x24c[i] = 0;
            }
        }
    }
}

// FIXME(shotgun): 92.11 %, different + reloc-name
void GreatFairy_804AF338::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x234 = -1;
    field_0x238 = -1;
    field_0x260 = 0;
    field_0x268 = 0;
    field_0x270 = -1;
    field_0x29c = 0;
    field_0x284.z = 0.0f;
    field_0x284.y = 0.0f;
    field_0x284.x = 0.0f;
    field_0x290.z = 0.0f;
    field_0x290.y = 0.0f;
    field_0x290.x = 0.0f;
    field_0x274[0] = field_0x90 >> 27;
    field_0x274[1] = (field_0x90 >> 22) & 0x1F;
    field_0x274[2] = (field_0x90 >> 17) & 0x1F;
    field_0x274[3] = (field_0x90 >> 12) & 0x1F;
    if ((field_0x90 & 0xF) == 3) {
        field_0x29c = (field_0x90 >> 4) & 0xF;
        field_0x234 = (field_0x90 >> 8) & 3;
        if (field_0x29c == 0xB) {
            JGeometry::TVec3f p = *getPlayerPos_8023E724(field_0x234);
            field_0x284.x = mPos.x;
            field_0x284.y = mPos.y;
            field_0x284.z = mPos.z;
            field_0x284.z += 32.0f;
            p.z = 8.0f;
            field_0x290.x = (p.x - field_0x284.x) / 120.0f;
            field_0x290.y = (p.y - field_0x284.y) / 120.0f;
            field_0x290.z = (p.z - field_0x284.z) / 120.0f;
            mState = 9;
        } else {
            field_0x264 = 0x78;
            mState = 7;
        }
        switch (field_0x29c) {
        case 1:
            field_0x260 = 0x260;
            break;
        case 2:
            field_0x260 = 0x261;
            break;
        case 3:
            field_0x260 = 0x262;
            break;
        case 4:
            field_0x260 = 0x263;
            break;
        case 5:
            field_0x260 = 0x264;
            break;
        case 6:
            field_0x260 = 0x268;
            break;
        case 7:
            field_0x260 = 0x26A;
            break;
        case 8:
            field_0x260 = 0x269;
            break;
        case 9:
            if (((gGameRoot_8053AAF8->field_0x83 >> (fn_8022F340(field_0x234) * 2)) & 3) == 2) {
                field_0x260 = 0x27F;
            } else {
                field_0x260 = 0x284;
            }
            break;
        case 10:
            field_0x260 = 0x265;
            break;
        case 11:
            field_0x260 = 0x266;
            break;
        case 12:
            field_0x260 = 0x267;
            break;
        }
    } else {
        mState = 0;
    }
    field_0x240 = 2;
    field_0x244 = 0;
    field_0x23c = 0;
    field_0x248 = 0;
    field_0x24c[0] = 0;
    field_0x24c[1] = 0;
    field_0x24c[2] = 0;
    field_0x24c[3] = 0;
    field_0x25c = 0x1E;
    field_0x264 = 0;
    field_0x26c = 0;
    field_0x2a0[0] = NULL;
    field_0x2a0[1] = NULL;
    field_0x90 = field_0x90 & 0xF;
}

// FIXME(shotgun): 92.20 %, missing/extra + reloc-name
void GreatFairy_804AF338::draw() {
    if (field_0x248 != 1 && (field_0x240 & 2) && field_0x90 != 3) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x61, NULL, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x61, false, 2, -1, 0, 0, 0);
    }
    if (mState == 8 || mState == 10 || mState == 11 || mState == 12) {
        if (field_0x26c == 0 && field_0x90 == 3) {
            SpriteParams_801F5AE0 params;
            JGeometry::TVec3f savedPos = mPos;
            s32 frame = lbl_80475600[field_0x29c - 1];
            params.field_0x1d = 0;
            params.field_0x1c = 0;
            mPos.set(field_0x284);
            if (mState == 10 || mState == 11 || mState == 12) {
                frame = lbl_80475600[field_0x29c + 10];
            }
            s32 flags = 7;
            if (mState == 11 || mState == 12) {
                params.field_0x1d = 1;
                params.field_0x1c = 1;
                flags = -1;
            }
            drawSprite_801F06F0("enemy_both2_sob.bin", frame, &params, flags, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, frame, false, 2, -1, 1, 0, 0);
            mPos.set(savedPos);
        }
    }
}

// FIXME(shotgun): 89.17 %, missing/extra
void GreatFairy_804AF338::fn_8038DE74() {
    bool done = false;
    if (field_0x274[fn_8022F340(field_0x234)] != 0) {
        s32 bit = field_0x274[fn_8022F340(field_0x234)];
        if (getGameView_802D6A9C()->fn_802D800C(bit) == 1) {
            field_0x29c = 0xC;
            done = true;
        }
    }
    if (!done) {
        switch (fn_802373EC(fn_8022F340(field_0x234))) {
        case 0:
            field_0x29c = 0xC;
            break;
        case 1:
            field_0x29c = 7;
            break;
        case 2:
            field_0x29c = 0xC;
            break;
        case 3:
            field_0x29c = 4;
            break;
        case 4:
            field_0x29c = 5;
            break;
        case 5:
            field_0x29c = 3;
            break;
        case 6:
            field_0x29c = 2;
            break;
        case 7:
            field_0x29c = 8;
            break;
        case 8:
            field_0x29c = 6;
            break;
        case 9:
            field_0x29c = 9;
            break;
        case 10:
            field_0x29c = 1;
            break;
        default:
            field_0x29c = 0xC;
            break;
        }
    }
    if (field_0x29c != 0xC && fn_8023725C(fn_8022F340(field_0x234)) >= 2) {
        field_0x29c = 0xC;
    }
    if (field_0x29c == 0xC) {
        field_0x260 = 0x267;
        fn_8038470C(field_0x234, field_0x260);
        mState = 2;
    } else if (fn_80388CAC(field_0x234, mSerial) == 1) {
        field_0x270 = getActorManager_801F856C()->spawnActor_801F9484(
            ACTOR_BFRY, field_0x4, &mPos, ((field_0x29c << 4) & 0xF0) | 3 | ((field_0x234 << 8) & 0x300), -1, -1);
        if (field_0x270 >= 0) {
            mState = 5;
            if (field_0x274[fn_8022F340(field_0x234)] != 0) {
                getGameView_802D6A9C()->fn_802D8050(field_0x274[fn_8022F340(field_0x234)], true, 0xFF);
            }
        } else {
            field_0x29c = 0;
        }
    }
}
