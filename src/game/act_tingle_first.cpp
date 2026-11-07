#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/Unk_8045A080.h"
#include "game/cls_8031709C.h"
#include "game/act/act_tingle_first.h"

struct Step_804AADC8 {
    /* 0x0 */ s16 cel;
    /* 0x2 */ s16 frames;
};

static const Step_804AADC8 lbl_804715B8[] = {
    {29, 1}, {30, 4}, {21, 50}, {31, 4}, {32, 8}, {33, 4}, {34, 8}, {21, 0}, {-1, -1},
};

static const Step_804AADC8 lbl_804715DC[] = {
    {21, 4}, {26, 4}, {25, 4}, {24, 4}, {23, 4}, {6, 4}, {-1, -1},
};

static const Step_804AADC8 lbl_804715F8[] = {
    {0, 4}, {1, 4}, {2, 6}, {1, 6}, {-1, -1},
};

static const Step_804AADC8 lbl_8047160C[] = {
    {0, 10}, {1, 10}, {2, 10}, {1, 10}, {-1, -1},
};

static const Step_804AADC8* lbl_804AADC8[] = {
    lbl_804715B8,
    lbl_804715DC,
    lbl_804715F8,
    lbl_8047160C,
};

cls_804AADD8::cls_804AADD8() {
    field_0x24c = new cls_8031709C;
}

cls_804AADD8::~cls_804AADD8() {
    if (field_0x24c != NULL) {
        delete field_0x24c;
        field_0x24c = NULL;
    }
}

void cls_804AADD8::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -24.0f;
    mBox.f.x = 24.0f;
    mBox.f.y = 24.0f;
    field_0x230 = 0;
    field_0x238 = 50;
    field_0x234 = -1;
    field_0x23c = 29;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    s32 layer = field_0x4;
    JGeometry::TVec3f pos(mPos);
    field_0x24c->fn_80316E50(layer, pos);
    field_0x260.i.x = -20.0f;
    field_0x260.i.y = -20.0f;
    field_0x260.f.x = 20.0f;
    field_0x260.f.y = 20.0f;
    field_0x270 = 0;
}

// FIXME: 98.00%. missing/extra: the original uses a paired-single squared
// distance (psq_l/ps_sub/ps_mul/ps_madd/ps_sum0, 9 instrs vs 12 scalar) in
// selectNearestPlayer's loop, and swaps the stack slots of the case 5/7
// TVec3f temporaries.
void cls_804AADD8::update() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    if (field_0x230 < 9) {
        fn_80239914(&box, field_0x4, mSerial);
    }

    switch (field_0x230) {
    case 0:
        field_0x238--;
        if (field_0x238 <= 0) {
            for (int i = 0; i < 4; i++) {
                if (getPlayerLayer_8023DE58(i) == getField_0x4() && fn_8023EFEC(i)) {
                    fn_80230C6C(i);
                }
            }
            field_0x230 = 1;
        }
        if (field_0x270 == 0) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000025, false, 0);
            field_0x270 = 1;
        }
        break;

    case 1:
        if (getActorManager_801F856C()->fn_801F68E0() != 0) {
            for (int i = 0; i < 4; i++) {
                if (getPlayerLayer_8023DE58(i) == getField_0x4() && !fn_80233E40(i) &&
                    !fn_8023EA28(i) && !fn_80236B50(i)) {
                    s32 dir = (getPlayerPos_8023E724(i)->x > mPos.x) ? 1 : 0;
                    fn_80230498(i, getPlayerPos_8023E724(i), dir + 0x3C, 0xFF, -0x80, -0x80, true);
                    fn_80230BF4(i, dir);
                }
            }
            selectNearestPlayer();
        }
        break;

    case 3:
        field_0x238 = 120;
        field_0x230 = 4;
        break;

    case 5:
        fn_8022FE80(field_0x234, field_0x4, mSerial, false, false);
        {
            s32 player = fn_8022FD8C(mSerial);
            if (player >= 0) {
                s32 serial = mSerial;
                if (talk(player, 0x50D, serial, field_0x4, mPos, 0x102)) {
                    field_0x230 = 6;
                    s32 layer = field_0x4;
                    JGeometry::TVec3f effectPos(mPos);
                    field_0x24c->fn_80316E50(layer, effectPos);
                }
            }
        }
        break;

    case 6:
        fn_8022FE80(field_0x234, field_0x4, mSerial, true, false);
        if (fn_803865B8(field_0x234)) {
            field_0x230 = 7;
        }
        break;

    case 7:
        fn_8022FE80(field_0x234, field_0x4, mSerial, false, false);
        if (fn_8038470C(field_0x234, 0x50E)) {
            field_0x230 = 8;
            s32 layer = field_0x4;
            JGeometry::TVec3f pos(mPos);
            field_0x24c->fn_80316E50(layer, pos);
        }
        break;

    case 8:
        fn_8022FE80(field_0x234, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x234, mSerial)) {
            field_0x230 = 9;
            field_0x234 = -1;
            field_0x240 = 1;
            field_0x244 = 0;
            field_0x248 = 0;
        }
        break;

    case 9:
        if (field_0x240 >= 2) {
            field_0x3c.x = 0.4f;
            field_0x3c.z = 0.3f;
            field_0x230 = 10;
            getActorManager_801F856C()->fn_801F6884();
        }
        /* fallthrough */
    case 10:
        if (mPos.z >= 30.0f) {
            field_0x3c.z = 0.0f;
        }
        mPos += field_0x3c;
        box.set(field_0x260);
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        if (!fn_8022B978(field_0x4, &box, true)) {
            field_0x238 = 60;
            field_0x230 = 11;
        }
        break;

    case 11:
        mPos.x = 1364.0f;
        mPos.y = 920.0f;
        mPos.z = 0.0f;
        field_0x4 = 0;
        field_0x3c.x = 0.6f;
        field_0x3c.z = 0.3f;
        field_0x230 = 12;
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        gGameRoot_8053AAF8->fn_80138B7C();
        break;

    case 12:
        field_0x238--;
        if (field_0x238 <= 0) {
            field_0x230 = 13;
            startSound_801F0E34(0x5001B, 0);
        }
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        break;

    case 13:
        if (mPos.z >= 30.0f) {
            field_0x3c.z = 0.0f;
        }
        mPos += field_0x3c;
        box.set(field_0x260);
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        if (!fn_8022B978(field_0x4, &box, true)) {
            fn_801F35AC();
        } else {
            gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        }
        break;

    case 2:
    case 4:
        field_0x238--;
        if (field_0x238 <= 0) {
            field_0x230++;
        }
        break;
    }

    if (field_0x230 == 8 && field_0x234 >= 0) {
        s32 sound = field_0x24c->fn_80316E74(field_0x234);
        if (field_0x24c->fn_80316E34() >= 0) {
            field_0x23c = field_0x24c->fn_80316E34();
            if ((u32)(sound + 0x10000) != 0xFFFF) {
                startSound_801F0E34(sound, 0);
            }
        } else {
            field_0x23c = 20;
        }
    } else if (field_0x230 >= 3) {
        const Step_804AADC8* steps = lbl_804AADC8[field_0x240];
        field_0x23c = steps[field_0x244].cel;
        s32 frames = steps[field_0x244].frames;
        if (frames > 0) {
            field_0x248++;
            if (field_0x248 >= frames) {
                field_0x248 = 0;
                field_0x244++;
                if (steps[field_0x244].cel < 0) {
                    field_0x244 = 0;
                    if (field_0x240 < 3) {
                        field_0x240++;
                    }
                }
            }
        }
    }
}

void cls_804AADD8::draw() {
    if (field_0x230 == 0xC || field_0x230 == 0xB) {
        return;
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x23c + 0xC0, false, 2, -1, 0, 0, 0);
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x23c + 0xC0, NULL, 7, 1.0f, 1.0f);
}

// Dead-stripped in the original (update inlines it in case 1): its literal
// 65536.0f (lbl_805418C0) leads the unit's .sdata2 pool ahead of draw's 1.0f
// (lbl_805418C4), so the code using it sat in a plain function below draw
// (docs/RULES_CARD.md, literals before their first visible use (3)).
void cls_804AADD8::selectNearestPlayer() {
    s32 count = gGameRoot_8053AAF8->fn_80138A30();
    s32 bestPlayer = 0;
    f32 bestDist = 65536.0f;
    for (s32 i = 0; i < count; i++) {
        s32 player = fn_8022EA0C(i);
        if (getPlayerLayer_8023DE58(player) == getField_0x4()) {
            const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(player);
            f32 dx = mPos.x - pPos->x;
            f32 dy = mPos.y - pPos->y;
            f32 dz = mPos.z - pPos->z;
            f32 dist = dx * dx + dy * dy + dz * dz;
            if (dist < bestDist) {
                bestDist = dist;
                bestPlayer = player;
            }
        }
    }
    field_0x234 = bestPlayer;
    field_0x238 = 60;
    field_0x230 = 2;
    startSound_801F0E34(0x50050, 0);
}
