#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/Unk_8045A080.h"
#include "game/tu_804465B8.h"
#include "game/act/tu_800FF060.h"

extern const u8 lbl_80539EA8[2];
extern s32 lbl_8049C848[4];
s32 lbl_8049C848[4] = { 500, 300, 100, 0 };

// FIXME(shotgun): 99.8 %, reloc-name
void HammerShopMinigame_8049C858::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x235 = 2;
    field_0x23c = 0;
    field_0x234 = 0;
    field_0x240 = -1;
    field_0x236 = 0;
    field_0x239 = (field_0x90 >> 22) & 0x1F;
    field_0x230 = 0;
    field_0x248 = 0;
    field_0x260 = 600;
    if (vfunc_0x34()) {
        field_0x230 = 8;
        field_0x248 = 0;
    } else {
        field_0x230 = 0;
        field_0x248 = 0;
    }
}

// FIXME(shotgun): 91.3 %, missing/extra
void HammerShopMinigame_8049C858::fn_801003F4() {
    s32 rank = 0;
    u8 used[4] = {0, 0, 0, 0};
    s32 count = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < count; i++) {
        if (field_0x250[i] == 0) {
            getGameView_802D6A9C()->fn_802D7844(1, i);
        } else {
            getGameView_802D6A9C()->fn_802D7844(0, i);
        }
    }
    for (rank = 0; rank != count; rank++) {
        s32 best = -1;
        s32 bestScore = 1000000;
        for (s32 i = 0; i < count; i++) {
            if (bestScore > field_0x250[i] && used[i] == 0) {
                bestScore = field_0x250[i];
                best = i;
            }
        }
        field_0x264[rank] = best;
        used[best] = 1;
    }
    field_0x274 = 0;
    if (field_0x250[field_0x264[0]] == field_0x250[field_0x264[1]]) {
        field_0x278 = 0;
    } else {
        field_0x278 = 1;
    }
    if (count != 2) {
        if (field_0x250[field_0x264[1]] == field_0x250[field_0x264[2]]) {
            field_0x27c = field_0x278;
        } else {
            field_0x27c = 2;
        }
        if (count != 3) {
            if (field_0x250[field_0x264[2]] == field_0x250[field_0x264[3]]) {
                field_0x280 = field_0x27c;
            } else {
                field_0x280 = 3;
            }
        }
    }
}

// FIXME(shotgun): 86.37 %, missing/extra
void HammerShopMinigame_8049C858::update() {
    JGeometry::TBox2f box = mBox;
    JGeometry::TVec3f msgPos = mPos;
    msgPos.y -= 50.0f;
    JGeometry::TVec3f focus = mPos;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    field_0x234++;
    if (field_0x234 == lbl_80539EA8[field_0x23c]) {
        field_0x23c++;
        if (field_0x23c == 2) {
            field_0x23c = 0;
        }
        field_0x234 = 0;
    }
    switch (field_0x230) {
    case 0:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x240 = fn_8022FD8C(mSerial);
        if (field_0x240 >= 0 && fn_802300EC(field_0x240)) {
            if (fn_8038AD24(field_0x240, 0x26B, mSerial, field_0x4, msgPos, 0)) {
                for (s32 i = 0; i < 4; i++) {
                    fn_802308E0(i);
                }
                field_0x230 = 1;
                JGeometry::TVec3f dir;
                f32 dist;
                fn_801EE558(&dir, &dist, field_0x240 + 1);
                if (dist < 100.0f) {
                    u32 angle = (u16)(s32)(JMath::atanTable_.atan2_(-dir.x, -dir.y) * 10430.378f);
                    if (angle < 0x2000 || angle >= 0xE000) {
                        field_0x235 = 3;
                    } else if (angle < 0x6000) {
                        field_0x235 = 1;
                    } else if (angle <= 0xA000) {
                        field_0x235 = 2;
                    } else {
                        field_0x235 = 0;
                    }
                }
            }
        }
        break;
    case 1: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = focus;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x240)) {
            switch (fn_8037EFB4(field_0x240)) {
            case 0:
                if (gGameRoot_8053AAF8->fn_80138A30() == 1) {
                    fn_8038470C(field_0x240, 0x285);
                    field_0x230 = 2;
                    field_0x248 = 0;
                } else {
                    field_0x284 = (s32)((f32)gGameRoot_8053AAF8->fn_80138A30() * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    if (field_0x284 == 4) {
                        field_0x284 = 3;
                    }
                    fn_803894BC(field_0x240, field_0x284 + 1);
                    fn_8038938C(field_0x240, field_0x260);
                    fn_8038470C(field_0x240, 0x26C);
                    s32 chosen = field_0x284;
                    for (s32 i = 0; i < 4; i++) {
                        field_0x28c[i] = fn_802373EC(i);
                        field_0x29c[i] = fn_8023725C(i);
                        fn_802372F8(i, 0, 1);
                    }
                    fn_802372F8(chosen, 5, 1);
                    field_0x230 = 3;
                    field_0x248 = 0;
                }
                break;
            case 1:
                fn_8038470C(field_0x240, 0x26D);
                field_0x230 = 2;
                field_0x248 = 0;
                break;
            }
        }
        break;
    }
    case 2: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = focus;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x240, mSerial)) {
            field_0x230 = 0;
            field_0x248 = 0;
            fn_80230534(false);
        }
        break;
    }
    case 3: {
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x7f5 == 0) {
            cam->field_0x7f4 = 1;
            cam->field_0x7f8 = 4;
            cam->field_0x7fc = focus;
            cam->field_0x7f6 = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x240, mSerial)) {
            field_0x240 = field_0x284;
            field_0x230 = 4;
            field_0x248 = 0;
            field_0x244 = field_0x260 * 60;
            field_0x24c = field_0x260 * 60;
            field_0x237 = -2;
            field_0x238 = 0;
            field_0x250[0] = 0;
            field_0x250[1] = 0;
            field_0x250[2] = 0;
            field_0x250[3] = 0;
            field_0x260 = 0;
            fn_80230534(false);
            fn_802308E0(field_0x240);
            fn_8022B278();
            fn_801F0D20(0x3A);
            field_0x288 = gAudioMgr_8053AF80->field_0x58;
            gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
            if (field_0x239 != 0) {
                getGameView_802D6A9C()->fn_802D8050(field_0x239, true, 0xFF);
            }
        }
        break;
    }
    case 4:
        if (field_0x244 == field_0x24c - 180) {
            fn_8023061C(field_0x240, false);
            field_0x237 = -1;
        }
        if (field_0x237 >= 0) {
            field_0x238++;
            if (field_0x238 == 15) {
                fn_801F0D20(0x2A);
                fn_802372F8(field_0x240, 0, 1);
                field_0x240 = field_0x237;
                fn_802372F8(field_0x240, 5, 1);
                fn_802308E0(field_0x240);
                field_0x24c = field_0x244;
                field_0x237 = -2;
                field_0x238 = 0;
            }
        } else if (field_0x237 == -2) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(field_0x240);
            if ((field_0x244 & 7) == 0) {
                field_0x238++;
            }
            fn_80230498(field_0x240, &pos, (field_0x238 & 3) + 0xBD, 0xFF, -0x80, -0x80, true);
            if (field_0x244 % 60 == 59) {
                fn_801F0D20(0x3F);
            }
        } else if (field_0x237 == -1) {
            for (s32 i = 0; i < 4; i++) {
                if (!fn_8022F514(i) && fn_8023A3DC(i) && field_0x240 != i) {
                    field_0x237 = i;
                    field_0x238 = 0;
                }
            }
        }
        field_0x244--;
        if (field_0x237 == -1) {
            field_0x250[field_0x240]++;
            if (field_0x250[field_0x240] >= 3000) {
                gAudioMgr_8053AF80->fn_80458508(1);
            } else {
                gAudioMgr_8053AF80->fn_80458508(0);
            }
        }
        if (field_0x250[0] == 3600) {
            field_0x236 = 1;
        }
        if (field_0x250[1] == 3600) {
            field_0x236 = 1;
        }
        if (field_0x250[2] == 3600) {
            field_0x236 = 1;
        }
        if (field_0x250[3] == 3600) {
            field_0x236 = 1;
        }
        if (field_0x244 == 0 || field_0x236 != 0) {
            getActorManager_801F856C()->fn_801F68E0();
            field_0x230 = 5;
            field_0x248 = 0;
            fn_801F0D20(0x3B);
            field_0x235 = 2;
            gAudioMgr_8053AF80->fn_80458508(0);
        }
        break;
    case 5:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x248++;
        if (field_0x248 > 50 & getActorManager_801F856C()->fn_801F68E0() == 1) {
            field_0x240 = fn_8022EA0C(field_0x240);
            fn_8022FE80(field_0x240, getPlayerLayer_8023DE58(field_0x240), mSerial, false, false);
            if (fn_8022FD8C(mSerial) >= 0) {
                fn_801003F4();
                fn_803894BC(field_0x240, 1);
                fn_8038AD24(field_0x240, 0x26E, mSerial, field_0x4, msgPos, 2);
                field_0x230 = 6;
                field_0x248 = 0;
                fn_8022B260();
                gGameRoot_8053AAF8->fn_80138B7C();
                field_0x248 = 0;
            }
        }
        break;
    case 6:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(field_0x240, getPlayerLayer_8023DE58(field_0x240), mSerial, false, false);
        if (fn_803865B8(field_0x240)) {
            if (field_0x248 > 0) {
                s32 prize = lbl_8049C848[(&field_0x274)[field_0x248 - 1]];
                if (prize != 0) {
                    lbl_8053AED0->fn_8043D874(field_0x264[field_0x248 - 1], prize, 0);
                }
            }
            if (field_0x248 < gGameRoot_8053AAF8->fn_80138A30()) {
                fn_803894BC(field_0x240, field_0x264[field_0x248] + 1);
                u32 msg;
                switch ((&field_0x274)[field_0x248]) {
                case 0:
                    msg = 0x36B;
                    break;
                case 1:
                    msg = 0x36C;
                    break;
                case 2:
                    msg = 0x36D;
                    break;
                case 3:
                    msg = 0x36E;
                    break;
                }
                fn_8038470C(field_0x240, msg);
            } else {
                fn_8038470C(field_0x240, 0x36F);
                field_0x230 = 7;
                field_0x248 = 0;
            }
            field_0x248++;
        }
        break;
    case 7:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(field_0x240, getPlayerLayer_8023DE58(field_0x240), mSerial, false, false);
        if (fn_803865B8(field_0x240) && fn_80384C58(field_0x240, mSerial)) {
            vfunc_0x30(0xFF, true);
            field_0x230 = 8;
            field_0x248 = 0;
            getActorManager_801F856C()->fn_801F6884();
            for (s32 i = 0; i < 4; i++) {
                if (!fn_8022F514(i)) {
                    fn_802372F8(i, field_0x28c[i], field_0x29c[i]);
                }
            }
        }
        break;
    case 8:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x240 = fn_8022FD8C(mSerial);
        if (field_0x240 >= 0 && fn_8038AD24(field_0x240, 0x370, mSerial, field_0x4, msgPos, 0)) {
            field_0x230 = 9;
            field_0x248 = 0;
            JGeometry::TVec3f dir;
            f32 dist;
            fn_801EE558(&dir, &dist, field_0x240 + 1);
            if (dist < 100.0f) {
                u32 angle = (u16)(s32)(JMath::atanTable_.atan2_(-dir.x, -dir.y) * 10430.378f);
                if (angle < 0x2000 || angle >= 0xE000) {
                    field_0x235 = 3;
                } else if (angle < 0x6000) {
                    field_0x235 = 1;
                } else if (angle <= 0xA000) {
                    field_0x235 = 2;
                } else {
                    field_0x235 = 0;
                }
            }
        }
        break;
    case 9:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x240, mSerial)) {
            field_0x230 = 8;
            field_0x248 = 0;
        }
        break;
    }
}

// FIXME(shotgun): 55.5 %, missing/extra (54 more instrs; loop unroll differs)
void HammerShopMinigame_8049C858::draw() {
    if (field_0x230 == 4 || field_0x230 == 5 || field_0x230 == 6 || field_0x230 == 7) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        for (s32 i = 0; i < count; i++) {
            lbl_8053AF00->field_0x30 |= (u8)(1 << i);
            lbl_8053AF00->field_0x2c[i] = field_0x250[i] / 60;
            lbl_8053AF00->field_0x28[i] = (f32)(field_0x250[i] % 60) / 0.6f;
        }
        lbl_8053AF00->field_0x30 |= (u8)(1 << (field_0x240 + 4));
        if (field_0x230 == 4) {
            JGeometry::TVec3f savedPos = mPos;
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(field_0x240);
            mPos.x = pos->x;
            mPos.y = pos->y;
            mPos.z = pos->z;
            mPos.y -= 20.0f;
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x314, NULL, -1, 1.0f, 1.0f);
            if (getPlayerLayer_8023DE58(field_0x240) != 0) {
                fn_803075AC(mSerial, getPlayerLayer_8023DE58(field_0x240), &mPos, 0x314, 0, 2, -1, false, 0, 0);
            }
            mPos.x = savedPos.x;
            mPos.y = savedPos.y;
            mPos.z = savedPos.z;
        }
    }
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x235 * 2 + field_0x23c + 0x311, NULL, 0, 1.0f, 1.0f);
}

HammerShopMinigame_8049C858::~HammerShopMinigame_8049C858() {}
