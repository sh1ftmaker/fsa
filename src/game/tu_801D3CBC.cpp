#include "game/GameAudio.h"

#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "game/act/act_shadow_link_2.h"


// FIXME: 98.83 %. Register allocation only.
void ShadowLink2_804A43E8::fn_801D41B8() const {
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    s32 id;
    u8 alpha;
    bool onTile = false;
    if (fn_801CD2F8(field_0x56c)) {
        id = 0x3E;
        alpha = (u8)(255.0f * mStateTimer * (1.0f / lbl_8053EE00));
        pos.set(mPos);
    } else {
        id = 0x3F;
        alpha = (u8)(255.0f - 255.0f * mStateTimer * (1.0f / lbl_8053EE00));
        pos.set(field_0x570);
    }
    if (fn_8022461C(&pos, 0) == 1) {
        switch (fn_80226850(&pos, 0)) {
        case 0x9B:
        case 0x9C:
            onTile = true;
            break;
        }
    }
    if (onTile && alpha != 0) {
        SpriteParams_801F5AE0 params;
        params.field_0x30 = 0xE;
        params.field_0x2e = 0;
        params.field_0x10.a = alpha;
        s32 layer = field_0x4;
        if (viewLayerFlag(layer)) {
            params.field_0x1f = 0;
        }
        if (field_0x560 != 4) {
            params.field_0x0[7] = field_0x560 + 0xC;
        }
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
        gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
    }
}

// FIXME: 99.61 %. One instruction differs (see try.py).
void ShadowLink2_804A43E8::fn_801D3DAC(s32 frame) const {
    SpriteParams_801F5AE0 params;
    params.field_0x30 = 0xE;
    params.field_0x2e = 0x40;
    if (viewLayerFlag(field_0x4)) {
        params.field_0x1f = 0;
    }
    if (field_0x560 != 4 && (mState == 8 || mState == 9)) {
        if (mState == 8) {
            params.field_0x10.a = 0xFF;
        } else {
            params.field_0x10.a = (u8)(255.0f - 255.0f * mStateTimer / lbl_8053EDF8);
        }
        if (params.field_0x10.a != 0) {
            drawSprite_801F06F0("player_sob.bin", frame, &params, 7, 1.0f, 1.0f);
        }
        params.field_0x0[7] = field_0x560 + 0xC;
        if (mState == 8) {
            params.field_0x10.a = (u8)(255.0f * mStateTimer / lbl_8053EDFC);
        } else {
            params.field_0x10.a = 0xFF;
        }
        if (params.field_0x10.a != 0) {
            drawSprite_801F06F0("player_sob.bin", frame, &params, 7, 1.0f, 1.0f);
        }
    } else {
        if (field_0x560 != 4 && mState > 8 && mState != 0x29) {
            params.field_0x0[7] = field_0x560 + 0xC;
        }
        if (mState == 0x10) {
            if (mSpriteParams.mMtx == NULL) {
                fn_801EF830(field_0x4, &mPos, &params, 7, "player_sob.bin", frame, NULL);
            }
            if (field_0x4 < 8) {
                void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "player_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
            }
            if (field_0x4 == 8) {
                JGeometry::TVec3f pos;
                fn_801CD0D0(&mPos, &pos);
                fn_801EF574("player_sob.bin", frame, &pos, &params);
            }
        } else {
            drawSprite_801F06F0("player_sob.bin", frame, &params, 7, 1.0f, 1.0f);
        }
    }
    u8 player = field_0x560;
    if (mState == 0x29) {
        player = 4;
    }
    fn_80307830(mSerial, field_0x4, &mPos, frame, false, 1, 0, player);
}

void ShadowLink2_804A43E8::fn_801D3CBC() {
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 0xD:
        return;
    case 0x17:
        if (field_0x524 & 0x200) {
            fn_801D41B8();
        }
        break;
    default:
        s32 frame = field_0x340.mFrame;
        switch (mState) {
        case 0x18:
        case 0x19:
        case 0x1A:
            frame = field_0x98 + 0x3C;
            break;
        }
        fn_801D3DAC(frame);
        if (mFlags & 0x10000) {
            if (!fn_801CD2F8(field_0x4)) {
                JGeometry::TVec3f pos;
                fn_801CD150(&mPos, &pos);
                fn_803075AC(mSerial, 8, &pos, 0x176, false, 2, -1, 0, 0, 0);
            }
        }
        break;
    }
}
