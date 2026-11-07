#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/GameState.h"
#include "game/SpriteParams.h"
#include "game/Unk_8045A080.h"
#include "game/cls_804B1D00.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/Message.h"
#include "game/tu_803B8C74.h"
#include "game/act/tu_801A0710.h"
#include "JSystem/JMath/JMath.h"
#include "std/math.h"

void fn_8022AF2C();

f32 lbl_8053A0F8[2] = {2.0f, 0.0f};
s32 lbl_8053A100 = -1;
s32 lbl_8053A104 = 0; // second word of the 8-byte object at 0x8053A100
s32 lbl_8053ABC0;
u8 lbl_8053ABC4;
u8 lbl_8053ABC5;

// The four TIMG pictures of the shop keeper.
const char* lbl_804A2BE4[4] = {"obachan_1.bti", "obachan_2.bti", "obachan_3.bti", "obachan_4.bti"};
const s32 lbl_804A2BD0[5] = {1, 5, 20, 50, 100};
const s32 lbl_804A2B90[16] = {0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 1, 2, 0};

// FIXME(shotgun): 93.8 %, missing/extra + reloc-name
void TennisShop_804A2BF4::init() {
    field_0x248 = 0;
    for (s32 i = 0; i < 6; i++) {
        field_0x24c[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x238[i] = 0;
    }
    for (s32 i = 0; i < 20; i++) {
        field_0x2c8[i] = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x28c[i] = 0;
        field_0x27c[i] = -1;
        field_0x29c[i] = 0;
        field_0x2ac[i] = 0;
        field_0x2bc[i] = 0;
        mEmitters[i] = NULL;
    }
    field_0x31c = 0;
    field_0x2c4 = -1;
    field_0x318 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x33c = 0.0f;
    for (s32 i = 0; i < 6; i++) {
        field_0x324[i] = 1.0f;
    }
    field_0x340 = 255.0f;
    field_0x278 = -1;
    field_0x2c0 = 0;
    field_0x320 = -1;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = field_0x90 & 0xF;
    switch (field_0x274) {
    case 0:
        if (field_0x238[0] == 0) {
            field_0x264 = lbl_804A2B90[field_0x98 * 4 + field_0x248];
            if (field_0x248 & 1) {
                field_0x238[0] = 10;
            } else {
                field_0x238[0] = 12;
            }
            field_0x248++;
            field_0x248 &= 3;
        }
        field_0xb0 = 0xA040004;
        mBox.i.x = -24.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 24.0f;
        mBox.f.y = 8.0f;
        mBox2.i.x = -28.0f;
        mBox2.i.y = -10.0f;
        mBox2.f.x = 28.0f;
        mBox2.f.y = 10.0f;
        field_0x98 = 2;
        lbl_8053ABC0 = 0;
        lbl_8053ABC5 = 0;
        lbl_8053ABC4 = 0;
        lbl_8053A0F8[0] = 2.0f;
        if (vfunc_0x34()) {
            field_0x230 = 0xB;
        } else {
            field_0x230 = 0;
        }
        break;
    case 1:
        mLife = 10000;
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x230 = 0xE;
        break;
    case 2:
        mLife = 10000;
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x80.i.x = -2.0f;
        field_0x80.i.y = -2.0f;
        field_0x80.f.x = 2.0f;
        field_0x80.f.y = 2.0f;
        field_0x230 = 0x12;
        break;
    }
}

TennisShop_804A2BF4::~TennisShop_804A2BF4() {
    if (field_0x274 == 0 && (field_0x230 == 5 || field_0x230 == 6)) {
        fn_8022AF2C();
        if (gGameRoot_8053AAF8->field_0x4c != 9) {
            gGameRoot_8053AAF8->fn_80138B7C();
            gAudioMgr_8053AF80->fn_80458508(0);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
}

// The scripted camera move towards pos (Camera.h, field_0x7f4-0x804).
static inline void setCameraScript(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.x = pos.x;
        camera->field_0x7fc.y = pos.y;
        camera->field_0x7fc.z = pos.z;
        camera->field_0x7f6 = 0;
    }
}

static inline bool talk(s32 player, u32 msg, s32 serial, s32 layer, const JGeometry::TVec3f& pos, u32 flags) {
    return fn_8038AD24(player, msg, serial, layer, pos, flags);
}

static inline void bumpCount999(s32* count) {
    (*count)++;
    if (*count >= 999) {
        *count = 999;
    }
}

// The keeper's swing pose, set when a ball is served.
static inline void startSwing(TennisShop_804A2BF4* self) {
    self->field_0x33c = 20.0f;
    self->field_0x24c[0] = 1;
    self->field_0x248 = 0;
    self->field_0x238[0] = 0;
    self->field_0x324[0] = 1.45f;
    self->field_0x324[1] = 2.2f;
    self->field_0x324[2] = 1.75f;
}

// FIXME(shotgun): 94.73 %, missing/extra + reloc-name
void TennisShop_804A2BF4::fn_801A17B8() {
    JGeometry::TVec3f spawnPos(mPos.x, mPos.y, mPos.z);
    JGeometry::TVec3f talkPos = mPos;
    addCalc2_801F71A4(&field_0x33c, 0.0f, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x324[0], field_0x324[3], 0.2f, 0.1f);
    addCalc2_801F71A4(&field_0x324[1], field_0x324[4], 0.2f, 0.1f);
    if (field_0x230 == 0xB || field_0x230 == 0xC) {
        for (s32 i = 0; i < 4; i++) {
            if (field_0x27c[i] < 0 || field_0x2ac[i] == 0) {
                continue;
            }
            if (field_0x29c[i] == 0) {
                if (fn_802308E0(field_0x27c[i]) != true) {
                    continue;
                }
                field_0x29c[i] = 1;
                for (s32 j = 0; j < 4; j++) {
                    if (i == fn_8022F3DC(j)) {
                        field_0x27c[j] = j;
                        field_0x2ac[j] = 1;
                        field_0x29c[j] = 1;
                    }
                }
            }
            s32 p = field_0x27c[i];
            if (p >= 0) {
                s32 id = 0x3C;
                if (mEmitters[p] == NULL) {
                    mEmitters[p] = spawnEmitter_8013CC50(field_0x4, getPlayerPos_8023E724(p), 0x565, 0, -1, NULL, NULL, true, 0.5f);
                    fn_80230FD0(p, 0x2002A);
                }
                field_0x2ac[p]++;
                field_0x2bc[p] = (field_0x2ac[p] / 6) % 4;
                if (field_0x2bc[p] == 0) {
                    id = 0x3E;
                } else if (field_0x2bc[p] == 1) {
                    id = 0x3D;
                } else if (field_0x2bc[p] == 2) {
                    id = 0x3F;
                }
                fn_80230498(p, getPlayerPos_8023E724(p), id, 0xFF, -0x80, -0x80, true);
            }
            if (fn_8022C5EC(field_0x27c[i], 0xF)) {
                field_0x2ac[i] += 5;
            }
            if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(field_0x27c[i]), 0x300)) {
                field_0x2ac[i] += 5;
            }
            if (field_0x2ac[i] >= 360) {
                for (s32 j = 0; j < 4; j++) {
                    if (i == fn_8022F3DC(j)) {
                        deleteEmitter(&mEmitters[j]);
                        field_0x27c[j] = -1;
                    }
                }
                deleteEmitter(&mEmitters[i]);
                fn_8023061C(field_0x27c[i], false);
                field_0x27c[i] = -1;
            }
        }
    }
    JGeometry::TVec3f camPos = mPos;
    JGeometry::TBox2f box(mBox);
    talkPos.y -= 64.0f;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    box = mBox2;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    switch (field_0x230) {
    case 0:
        if (field_0x238[3] != 0) {
            break;
        }
        field_0x2c0 = gGameRoot_8053AAF8->fn_80138A30();
        field_0x230 = 1;
    case 1: {
        s32 dir;
        f32 dist;
        fn_801EEC98(mSerial, NULL, &dir, &dist, NULL, 0, 0);
        if (dir < 0 || dist > 64.0f) {
            break;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x278 = fn_8022FD8C(mSerial);
        if (field_0x278 < 0 || !fn_802300EC(field_0x278)) {
            break;
        }
        u32 msg = 0x352;
        if (field_0x270 == 1) {
            msg = 0x371;
        }
        if (talk(field_0x278, msg, mSerial, field_0x4, talkPos, 0x100)) {
            field_0x230 = 2;
        }
        break;
    }
    case 2:
        setCameraScript(camPos);
        fn_8022FE80(field_0x278, field_0x4, mSerial, false, false);
        if (!fn_803865B8(field_0x278)) {
            break;
        }
        switch (fn_8037EFB4(field_0x278)) {
        case 0:
            fn_8038470C(field_0x278, 0x354);
            field_0x230 = 3;
            break;
        case 1:
            fn_8038470C(field_0x278, 0x353);
            field_0x230 = 0xD;
            break;
        }
        break;
    case 3:
        setCameraScript(camPos);
        fn_8022FE80(field_0x278, field_0x4, mSerial, false, false);
        if (fn_80388CAC(field_0x278, mSerial)) {
            fn_80230534(false);
            field_0x2c4 = 3600;
            fn_801F0D20(0x3A);
            gAudioMgr_8053AF80->fn_80458FF0(0x1000007, true, 0);
            fn_8022AF44();
            field_0x230 = 4;
        }
        break;
    case 4: {
        lbl_8053ABC0 = 0;
        lbl_8053ABC4 = 0;
        field_0x320 = getActorManager_801F856C()->spawnActor_801F9484('TENN', field_0x4, &spawnPos, 1, -1, -1);
        s32 serial = field_0x320;
        if (serial < 0) {
            break;
        }
        TennisShop_804A2BF4* ball = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (ball == NULL) {
            break;
        }
        ball->field_0x320 = mSerial;
        ball->field_0x264 = field_0x268;
        field_0x238[1] = 200;
        field_0x24c[1] = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        field_0x24c[2] = 0;
        field_0x24c[3] = 0;
        field_0x230 = 5;
        break;
    }
    case 5:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x2c8[field_0x318] = field_0x24c[2];
        if (field_0x2c8[field_0x318] > 999) {
            field_0x2c8[field_0x318] = 999;
        }
        field_0x2c4--;
        if (field_0x2c4 < 0) {
            field_0x2c4 = 0;
        }
        if (field_0x2c4 / 60 <= 11 && field_0x2c4 % 60 == 0 && field_0x2c4 > 60) {
            fn_801F0D20(0x3F);
            gAudioMgr_8053AF80->fn_80458508(1);
        }
        if (field_0x2c8[field_0x318] > field_0x31c) {
            field_0x31c = field_0x2c8[field_0x318];
        }
        if (field_0x2c4 == 0) {
            if (field_0x238[1] > 2 || lbl_8053ABC0 == 1) {
                fn_801F0D20(0x3B);
                gAudioMgr_8053AF80->fn_80458508(0);
                s32 serial = field_0x320;
                if (serial >= 0) {
                    ActorBase_804A5770* ball = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (ball != NULL) {
                        ball->mLife = 0;
                    }
                }
                field_0x24c[0] = 0;
                field_0x24c[1] = 50;
                if (lbl_8053ABC5 == 1) {
                    field_0x24c[1] = 120;
                }
                field_0x2c4 = -1;
                field_0x230 = 6;
                break;
            }
        } else if (lbl_8053ABC0 == 1) {
            field_0x318++;
            if (field_0x318 >= 20) {
                field_0x318 = 19;
            }
            field_0x230 = 4;
            break;
        }
        switch (field_0x238[1]) {
        case 1: {
            s32 serial = field_0x320;
            if (serial >= 0) {
                TennisShop_804A2BF4* ball = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (ball != NULL) {
                    JGeometry::TVec3f vel(0.0f, 0.0f, 0.0f);
                    if (ball->field_0x264 >= 4) {
                        ball->field_0x264 = field_0x268;
                    }
                    lbl_8053A0F8[0] = 0.25f * field_0x24c[2] + 2.0f;
                    if (lbl_8053A0F8[0] > 7.0f) {
                        lbl_8053A0F8[0] = 7.0f;
                    }
                    startSound_801F0E34(0x40091, 0);
                    u16 angle;
                    fn_801EEC98(mSerial, &angle, NULL, NULL, NULL, ball->field_0x264, 2);
                    vel.x = lbl_8053A0F8[0] * JMath::sincosTable_.sinShort(angle);
                    vel.y = lbl_8053A0F8[0] * JMath::sincosTable_.cosShort(angle);
                    ball->field_0x3c = vel;
                }
            }
            startSwing(this);
            if (field_0x24c[2] != 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x12C, 0, 1, NULL, NULL, true, field_0x324[0]);
            }
            break;
        }
        case 0:
            if (field_0x24c[3] == 0) {
                s32 serial = field_0x320;
                if (serial < 0) {
                    break;
                }
                ActorBase_804A5770* ball = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (ball == NULL) {
                    break;
                }
                // Both terms read the ball's own position (x - x, x - y), as in the original.
                f32 dx = ball->mPos.x - ball->mPos.x;
                f32 dy = ball->mPos.x - ball->mPos.y;
                if (std::sqrtf(dx * dx + dy * dy) > 32.0f) {
                    lbl_8053ABC4 = 1;
                }
                break;
            }
            lbl_8053ABC4 = 0;
            if (field_0x2c0 == 1) {
                bumpCount999(&field_0x24c[2]);
                bumpCount999(&field_0x26c);
            }
            if (field_0x24c[2] >= 999 || field_0x26c >= 999) {
                s32 serial = field_0x320;
                if (serial >= 0) {
                    ActorBase_804A5770* ball = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (ball != NULL) {
                        ball->mLife = 0;
                        field_0x2c4 = 0;
                    }
                }
            }
            field_0x24c[3] = 0;
            field_0x238[1] = 2;
            startSound_801F0E34(0x40080, 0);
            if (field_0x24c[2] < 10) {
                field_0x24c[1] ^= 1;
            } else if ((field_0x24c[2] & 7) == 0) {
                field_0x24c[1] = (field_0x24c[2] >> 1) & 1;
            } else {
                field_0x24c[1] = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            }
            break;
        }
        break;
    case 6:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        field_0x24c[1]--;
        if (field_0x24c[1] > 0) {
            break;
        }
        field_0x24c[1] = 0;
        if (getActorManager_801F856C()->fn_801F68E0()) {
            for (s32 i = 0; i < 4; i++) {
                fn_80230C6C(i);
            }
            field_0x230 = 7;
        }
        break;
    case 7:
        fn_8022FE80(0, getPlayerLayer_8023DE58(0), mSerial, false, false);
        if (fn_8022FD8C(mSerial) == 0) {
            talk(0, 0x356, mSerial, field_0x4, talkPos, 0x102);
            field_0x238[1] = 0;
            field_0x230 = 8;
        }
        break;
    case 8:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(0, getPlayerLayer_8023DE58(0), mSerial, false, false);
        if (!fn_803865B8(0)) {
            break;
        }
        field_0x24c[1] = 0;
        gGameRoot_8053AAF8->fn_80138B7C();
        fn_8022AF2C();
        if (field_0x26c == 0) {
            fn_8038470C(0, 0x35B);
        } else {
            fn_8038938C(0, field_0x26c);
            if (lbl_8050EC80.getCurrentFile().field_0x14 < (u16)field_0x26c) {
                lbl_8050EC80.getCurrentFile().field_0x14 = field_0x26c;
                fn_8038470C(0, 0x357);
            } else {
                fn_8038470C(0, 0x358);
            }
            if (lbl_8050EC80.getCurrentFile().field_0x16 < (u16)field_0x31c) {
                lbl_8050EC80.getCurrentFile().field_0x16 = field_0x31c;
            }
        }
        field_0x24c[3] = 0;
        field_0x24c[4] = 0;
        field_0x230 = 9;
        break;
    case 9: {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        fn_8022FE80(0, getPlayerLayer_8023DE58(0), mSerial, false, false);
        if (!fn_803865B8(0) || !fn_80384C58(0, mSerial)) {
            break;
        }
        if (field_0x26c == 0) {
            getActorManager_801F856C()->fn_801F6884();
            field_0x270 = 1;
            field_0x230 = 0;
            break;
        }
        field_0x24c[4] = field_0x31c * 10;
        if (field_0x24c[4] > 500) {
            field_0x24c[4] = 500;
        }
        s32 best = -1;
        s32 bestPlayer = -1;
        for (s32 i = 0; i < 4; i++) {
            if (field_0x28c[i] > 0) {
                if (best < 0 && field_0x28c[i] != 0) {
                    field_0x27c[i] = i;
                    best = field_0x28c[i];
                    bestPlayer = i;
                } else if (best < field_0x28c[i]) {
                    field_0x27c[i] = i;
                    field_0x27c[bestPlayer] = -1;
                    best = field_0x28c[i];
                    bestPlayer = i;
                }
            }
        }
        for (s32 i = 0; i < 4; i++) {
            if (field_0x28c[i] > 0 && bestPlayer != i && best == field_0x28c[i]) {
                field_0x27c[i] = i;
            }
        }
        if (bestPlayer < 0) {
            field_0x24c[3] = 1;
            startSound_801F0E34(0x3004D, 0);
            field_0x320 = getActorManager_801F856C()->spawnActor_801F9484('TENN', field_0x4, &spawnPos, 2, -1, -1);
            s32 serial = field_0x320;
            if (serial >= 0) {
                TennisShop_804A2BF4* ball = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (ball != NULL) {
                    startSwing(this);
                    ball->field_0x264 = 4;
                }
            }
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (field_0x27c[i] < 0) {
                    continue;
                }
                field_0x320 = getActorManager_801F856C()->spawnActor_801F9484('TENN', field_0x4, &spawnPos, 2, -1, -1);
                s32 serial = field_0x320;
                if (serial < 0) {
                    continue;
                }
                TennisShop_804A2BF4* ball = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (ball == NULL) {
                    continue;
                }
                startSwing(this);
                ball->field_0x264 = i;
                ball->field_0x320 = mSerial;
                ball->field_0x27c[i] = field_0x27c[i];
            }
        }
        field_0x230 = 0xA;
        break;
    }
    case 0xA: {
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (field_0x24c[3] == 0) {
            s32 left = 0;
            for (s32 i = 0; i < 4; i++) {
                if (field_0x27c[i] >= 0) {
                    left++;
                    if (field_0x2ac[i] != 0) {
                        left--;
                    }
                }
            }
            if (left != 0) {
                break;
            }
        } else {
            JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
            pos.y -= 16.0f;
            field_0x238[1] = 80;
            startSound_801F0E34(0x4000E, 0);
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &pos, 0x565, 0, 1, NULL, NULL, true, 0.7f);
        }
        getActorManager_801F856C()->fn_801F6884();
        s32 budget = field_0x26c * 40;
        if (budget > 9999) {
            budget = 9999;
        }
        for (s32 i = 0; budget > 0 && i < 200; i++) {
            s32 kind = (s32)(4.5f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
            if (budget < lbl_804A2BD0[kind]) {
                continue;
            }
            budget -= lbl_804A2BD0[kind];
            s32 serial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &mPos, kind | 0x220000);
            if (serial < 0) {
                continue;
            }
            ActorBase_804A5770* rupee = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (rupee == NULL) {
                continue;
            }
            JGeometry::TVec3f vel;
            vel.x = 2.0f - 3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            vel.y = 3.0f - 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
            vel.z = 5.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 5.0f;
            rupee->field_0x3c = vel;
        }
        vfunc_0x30(0xFF, true);
        field_0x230 = 0xB;
        break;
    }
    case 0xB:
        // FIXME(shotgun): the original indexes the emitter it deletes with a register left over from an
        // earlier case (slwi r0, r26, 2) while testing and clearing mEmitters[0].
        if (field_0x24c[3] != 0 && field_0x238[1] == 0 && mEmitters[0] != NULL) {
            deleteEmitter(&mEmitters[0]);
            field_0x24c[3] = 0;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x278 = fn_8022FD8C(mSerial);
        if (field_0x278 < 0 || !fn_802300EC(field_0x278)) {
            break;
        }
        if (talk(field_0x278, 0x35C, mSerial, field_0x4, talkPos, 0x100)) {
            field_0x230 = 0xC;
        }
        break;
    case 0xC:
        setCameraScript(camPos);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x278, mSerial)) {
            field_0x230 = 0xB;
        }
        break;
    case 0xD:
        setCameraScript(camPos);
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x278, mSerial)) {
            fn_80230534(false);
            field_0x230 = 0;
            field_0x238[3] = 100;
        }
        break;
    }
    if (field_0x238[0] == 0) {
        s32 idx = field_0x98 * 4 + field_0x248;
        switch (field_0x24c[0]) {
        case 0:
            field_0x264 = lbl_804A2B90[idx];
            break;
        case 1:
            field_0x264 = 3;
            field_0x24c[0] = 0;
            field_0x238[0] = 10;
            field_0x248 = 3;
            break;
        }
        if (field_0x248 & 1) {
            field_0x238[0] = 10;
        } else {
            field_0x238[0] = 12;
        }
        field_0x248++;
        field_0x248 &= 3;
    }
    if (field_0x238[0] > 0) {
        field_0x238[0]--;
    }
    if (field_0x238[1] > 0) {
        field_0x238[1]--;
    }
    if (field_0x238[2] > 0) {
        field_0x238[2]--;
    }
    if (field_0x238[3] > 0) {
        field_0x238[3]--;
    }
}

// Highest set bit of a player mask (the original computes it with a shift loop).
static inline s32 highestPlayer(s32 mask) {
    s32 p = 0;
    while (mask > 0) {
        mask >>= 1;
        p++;
    }
    p--;
    if (p < 0) {
        p = 0;
    }
    return p;
}

static inline void bumpCount(s32* count) {
    (*count)++;
    if (*count > 999) {
        *count = 999;
    }
}

// FIXME(shotgun): 92.0 %, missing/extra (rewritten against the asm: 0xE-0x11 follow the switch; the p < 0 test of the id-range check and the
// duplicated count clamp are not reproduced exactly)
void TennisShop_804A2BF4::fn_801A0F90() {
    bool dead = false;
    mPos.x += field_0x3c.x;
    mPos.y += field_0x3c.y;
    mPos.z += field_0x3c.z;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    switch (field_0x230) {
    case 0xE:
        field_0x230 = 0xF;
    case 0xF:
        if (field_0x3c.x == 0.0f && field_0x3c.y == 0.0f) {
            if (mPos.z <= 0.0f) {
                startSound_801F0E34(0x3004C, 0);
                mPos.z = 0.0f;
                field_0x3c.z = 5.0f;
            } else {
                field_0x3c.z -= 0.5f;
            }
            break;
        }
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x3004C, 0);
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        } else {
            field_0x3c.z -= 0.3f;
        }
        addCalc2_801F71A4(&mPos.z, 16.0f, 0.3f, 2.0f);
        if (field_0x238[1] != 0) {
            break;
        }
        mBox.i.x = -4.0f;
        mBox.i.y = -4.0f;
        mBox.f.x = 4.0f;
        mBox.f.y = 4.0f;
        if (u8 mask = fn_801F2740(-1)) {
            s32 p = highestPlayer(mask);
            if (field_0x264 == p || field_0x264 == fn_8022F3DC(p)) {
                if (fn_801F2B7C(0, -1, 1)) {
                    startSound_801F0E34(0x3004D, 0);
                    dead = true;
                    lbl_8053ABC5 = 1;
                    break;
                }
            }
        }
        mBox.i.x = -10.0f;
        mBox.i.y = -10.0f;
        mBox.f.x = 10.0f;
        mBox.f.y = 10.0f;
        field_0x278 = fn_801F285C(NULL, NULL, true, -1);
        if (field_0x278 < 0) {
            break;
        }
        if (field_0x264 == field_0x278 || field_0x264 == fn_8022F3DC(field_0x278)) {
            u16 id = fn_8023CBC0(field_0x278);
            if (id >= 0x43 && id < 0x6F) {
                break;
            }
            f32 speed = lbl_8053A0F8[0];
            if (fn_802353AC(field_0x264)) {
                speed *= 1.5f;
            }
            field_0x264++;
            if (field_0x264 >= gGameRoot_8053AAF8->fn_80138A30()) {
                field_0x264 = 0;
            }
            field_0x248++;
            fn_8023B6CC(field_0x278);
            fn_80230FD0(field_0x278, 0x2003F);
            if (field_0x248 < gGameRoot_8053AAF8->fn_80138A30()) {
                if (fn_8023F264(field_0x264) && field_0x4 == getPlayerLayer_8023DE58(field_0x264) && !fn_80230188(field_0x264)) {
                    u16 angle;
                    fn_801EEC98(mSerial, &angle, NULL, NULL, NULL, field_0x264, 2);
                    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                        angle -= 0x600;
                    } else {
                        angle += 0x600;
                    }
                    field_0x3c.x = speed * JMath::sincosTable_.sinShort(angle);
                    field_0x3c.y = speed * JMath::sincosTable_.cosShort(angle);
                    field_0x238[1] = 30;
                    if (field_0x320 >= 0) {
                        TennisShop_804A2BF4* shop = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x320));
                        if (shop != NULL) {
                            bumpCount(&shop->field_0x24c[2]);
                            bumpCount(&shop->field_0x26c);
                            if (shop->field_0x24c[2] > 999) {
                                shop->field_0x24c[2] = 999;
                            }
                        }
                    }
                } else {
                    dead = true;
                }
            } else if (field_0x320 >= 0) {
                TennisShop_804A2BF4* shop = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x320));
                if (shop != NULL) {
                    field_0x248 = 0;
                    field_0x264 = 4;
                    if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
                        bumpCount(&shop->field_0x24c[2]);
                        bumpCount(&shop->field_0x26c);
                    }
                    getPlayerPos_8023E724(0);
                    getPlayerPos_8023E724(0);
                    if (field_0x278 >= 0) {
                        u16 angle = (s32)(JMath::atanTable_.atan2_(shop->mPos.x - mPos.x, shop->mPos.y - mPos.y) * 10430.378f);
                        field_0x3c.x = lbl_8053A0F8[0] * JMath::sincosTable_.sinShort(angle);
                        field_0x3c.y = lbl_8053A0F8[0] * JMath::sincosTable_.cosShort(angle);
                        field_0x230 = 0x10;
                    } else {
                        dead = true;
                    }
                } else {
                    dead = true;
                }
            } else {
                dead = true;
            }
        }
        break;
    case 0x10: {
        s32 target = fn_801F2718(0x54454E4E);
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x3004C, 0);
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        } else {
            field_0x3c.z -= 0.3f;
        }
        if (lbl_8053ABC4 == 1 && target >= 0) {
            TennisShop_804A2BF4* shop = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(target));
            if (shop != NULL) {
                shop->field_0x24c[3] = 1;
                field_0x230 = 0xE;
            }
        }
        break;
    }
    case 0x11:
        addCalc2_801F71A4(&field_0x340, 0.0f, 0.5f, 5.0f);
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x3004C, 0);
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        } else {
            field_0x3c.z -= 0.5f;
        }
        if (field_0x238[1] == 0 || field_0x340 < 1.0f) {
            mAlive = false;
        }
        break;
    }
    if (field_0x230 != 0x11) {
        if (mLife == 0 || dead || fn_801F6758(&mPos, field_0x4, -16.0f)) {
            field_0x238[1] = 50;
            if (mLife != 0 && field_0x320 >= 0) {
                TennisShop_804A2BF4* shop = static_cast<TennisShop_804A2BF4*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x320));
                if (shop != NULL) {
                    shop->field_0x268 = field_0x264;
                    shop->field_0x28c[field_0x264]++;
                }
            }
            field_0x3c.x *= -0.5f;
            field_0x3c.y *= -0.5f;
            field_0x3c.z = 3.0f;
            startSound_801F0E34(0x3004D, 0);
            lbl_8053ABC0 = 1;
            field_0x230 = 0x11;
        }
    }
    if (field_0x238[0] > 0) {
        field_0x238[0]--;
    }
    if (field_0x238[1] > 0) {
        field_0x238[1]--;
    }
    if (field_0x238[2] > 0) {
        field_0x238[2]--;
    }
    if (field_0x238[3] > 0) {
        field_0x238[3]--;
    }
}

// FIXME(shotgun): 85.9 %, missing/extra
void TennisShop_804A2BF4::fn_801A0BE4() {
    mPos.x += field_0x3c.x;
    mPos.y += field_0x3c.y;
    mPos.z += field_0x3c.z;
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
    switch (field_0x230) {
    case 0x12: {
        s32 serial = field_0x320;
        if (serial < 0 || getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
            field_0x238[1] = 50;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 5.0f;
            field_0x3c.z = 3.0f;
            startSound_801F0E34(0x3004D, 0);
            field_0x230 = 0x13;
            break;
        }
        for (s32 i = 0; i < 4; i++) {
            if (field_0x27c[i] >= 0) {
                f32 dx = mPos.x - getPlayerPos_8023E724(i)->x;
                f32 dy = mPos.y - getPlayerPos_8023E724(i)->y;
                f32 dist = std::sqrtf(dx * dx + dy * dy);
                if (dist > 16.0f) {
                    s32 angle = (s32)(JMath::atanTable_.atan2_(-dx, -dy) * 10430.378f);
                    field_0x3c.x = 10.0f * JMath::sincosTable_.sinShort(angle);
                    field_0x3c.y = 10.0f * JMath::sincosTable_.cosShort(angle);
                } else {
                    field_0x2ac[i] = 1;
                    field_0x238[1] = 50;
                    field_0x3c.x *= -0.5f;
                    field_0x3c.y *= -0.5f;
                    field_0x3c.z = 3.0f;
                    startSound_801F0E34(0x3004D, 0);
                    field_0x230 = 0x13;
                }
            }
        }
        break;
    }
    case 0x13:
        addCalc2_801F71A4(&field_0x340, 0.0f, 0.5f, 5.0f);
        if (mPos.z <= 0.0f) {
            startSound_801F0E34(0x3004C, 0);
            mPos.z = 0.0f;
            field_0x3c.z = 3.0f;
        } else {
            field_0x3c.z -= 0.5f;
        }
        if (field_0x238[1] != 0 && field_0x340 < 1.0f) {
            mAlive = false;
        }
        break;
    }
}

void TennisShop_804A2BF4::update() {
    switch (field_0x274) {
    case 0:
        fn_801A17B8();
        break;
    case 1:
        fn_801A0F90();
        break;
    case 2:
        fn_801A0BE4();
        break;
    }
}

// Not auto-inlined: draw calls fn_801A0910 / fn_801A0754 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 87.3 %, missing/extra + reloc-name
void TennisShop_804A2BF4::fn_801A0910() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    f32 savedY = mPos.y;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = field_0x324[0];
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = field_0x324[1];
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = field_0x324[1];
    params.mMtx = mtx;
    mPos.y = savedY + (field_0x33c - 16.0f);
    fn_801EFD34(fn_804032E8('TIMG', lbl_804A2BE4[field_0x264]), &params, 7, 0.0f, -2.0f, 0.0f);
    mPos.y = savedY;
    if (field_0x2c4 >= 0) {
        lbl_8053AF28->field_0x10 = (u8)field_0x26c;
        lbl_8053AF28->field_0x14 |= 1;
        lbl_8053AF28->field_0x12 = field_0x2c4 / 60;
        lbl_8053AF28->field_0x13 = (s32)(100.0f * (f32)(field_0x2c4 % 60) / 60.0f);
        lbl_8053AF28->field_0x14 |= 2;
    }
}

// FIXME(shotgun): 99.8 %, reloc-name
void TennisShop_804A2BF4::fn_801A0754() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    mtx[0][0] = 1.2f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 1.2f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 1.2f;
    if (field_0x274 == 2) {
        mtx[0][1] = 0.0f;
        mtx[0][0] = 2.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 2.0f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 2.0f;
    }
    params.mMtx = mtx;
    params.field_0x30 = 0x11;
    params.field_0x10.a = (s32)field_0x340;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x264 + 0x2BA, &params, 0, 1.0f, 1.0f);
}

#pragma auto_inline reset

void TennisShop_804A2BF4::draw() {
    switch (field_0x274) {
    case 0:
        fn_801A0910();
        break;
    case 1:
    case 2:
        fn_801A0754();
        break;
    }
}
