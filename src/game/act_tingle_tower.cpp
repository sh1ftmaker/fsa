#include "JSystem/JMath/JMath.h"
#include "game/ActorBase.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/Unk_8045A080.h"
#include "game/cls_8031709C.h"
#include "game/tu_803B8C74.h"
#include "game/act/tu_801F9484.h"

extern u8 lbl_8053ABF8;

static const u32 lbl_804714D8[8] = {
    0x474, 0x475, 0x476, 0x477, 0x478, 0x479, 0x47A, 0x47B,
};

static const s32 lbl_804714F8[8] = {
    5, 6, 7, 8, 9, 10, 8, 8,
};

static const s32 lbl_80471518[4] = {
    19, 20, 21, 22,
};

static void (cls_804AAD60::*lbl_804AACDC[11])() = {
    &cls_804AAD60::fn_80315EE8,
    &cls_804AAD60::fn_80315C84,
    &cls_804AAD60::fn_80315C80,
    &cls_804AAD60::fn_80313C20,
    &cls_804AAD60::fn_80315864,
    &cls_804AAD60::fn_80315460,
    &cls_804AAD60::fn_80315080,
    &cls_804AAD60::fn_80314B1C,
    &cls_804AAD60::fn_803146DC,
    &cls_804AAD60::fn_803142E4,
    &cls_804AAD60::fn_80313D7C,
};

inline void cls_804AAD60::updateFacing() {
    if (field_0x274 <= 0) {
        field_0x274 = 20;
        JGeometry::TVec3f dir;
        f32 dist;
        if (field_0x254 >= 0 && getPlayerLayer_8023DE58(field_0x254) == getField_0x4()) {
            fn_801EE558(&dir, &dist, field_0x254 + 1);
        } else {
            fn_801EE558(&dir, &dist, 0);
        }
        if (dist < 80.0f) {
            u32 angle = (u16)(JMath::atanTable_.atan2_(-dir.x, -dir.y) *
                              (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
            if (angle < 0x2000 || angle >= 0xE000) {
                field_0x270 = 3;
            } else if (angle >= 0x2000 && angle < 0x6000) {
                field_0x270 = 1;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                field_0x270 = 2;
            } else {
                field_0x270 = 0;
            }
        } else {
            field_0x270 = 2;
        }
    } else {
        field_0x274--;
    }
}

static inline u8 getTowerViewByte(s32 i) {
    GameView_804A9950* view = getGameView_802D6A9C();
    return (i >= 0 && i < 4) ? view->field_0x131d8[i] : 0;
}

cls_804AAD60::cls_804AAD60() {
    field_0x278 = new cls_8031709C;
}

cls_804AAD60::~cls_804AAD60() {
    if (field_0x278 != NULL) {
        delete field_0x278;
        field_0x278 = NULL;
    }
}

void cls_804AAD60::init() {
    field_0x230 = field_0x90 & 3;
    field_0x234 = 0;
    field_0x240 = 0;
    field_0x23c = (field_0x90 >> 4) & 7;
    if (field_0x230 == 0) {
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x258.i.x = -24.0f;
        field_0x258.i.y = 128.0f;
        field_0x258.f.x = 24.0f;
        field_0x258.f.y = 160.0f;
        for (s32 i = 0; i < 4; i++) {
            getGameView_802D6A9C()->fn_802D7844(0, i);
        }
    } else if (field_0x230 == 3) {
        mBox.i.x = 0.0f;
        mBox.i.y = 0.0f;
        mBox.f.x = 16.0f;
        mBox.f.y = 16.0f;
        field_0x258.i.x = 0.0f;
        field_0x258.i.y = 0.0f;
        field_0x258.f.x = 16.0f;
        field_0x258.f.y = 16.0f;
    } else {
        field_0x230 = lbl_804714F8[field_0x23c];
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x258.i.x = -24.0f;
        field_0x258.i.y = -24.0f;
        field_0x258.f.x = 24.0f;
        field_0x258.f.y = 24.0f;
    }

    if ((((1 << field_0x23c) & 0xFF) & lbl_8050EC80.getCurrentFile().field_0x5a[0]) != 0) {
        field_0x268 = lbl_804714D8[field_0x23c];
    } else {
        field_0x268 = 0x473;
    }

    field_0x244[0] = 0;
    field_0x244[1] = 1;
    field_0x244[2] = 2;
    field_0x244[3] = 3;
    field_0x254 = -1;
    field_0x238 = 0;
    field_0x26c = 0;
    field_0x270 = 2;
    field_0x274 = 0;
    field_0x278->fn_80316E50(field_0x4, JGeometry::TVec3f(mPos));
}

void cls_804AAD60::update() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    (this->*lbl_804AACDC[field_0x230])();
}

void cls_804AAD60::draw() {
    if (field_0x230 != 3) {
        s32 cel;
        if (field_0x234 == 1 && field_0x254 >= 0) {
            u32 soundId = field_0x278->fn_80316E74(field_0x254);
            if (field_0x278->fn_80316E34() >= 0) {
                cel = field_0x278->fn_80316E34();
                if (soundId != 0xFFFFFFFF) {
                    startSound_801F0E34(soundId, 0);
                }
            } else {
                cel = lbl_80471518[field_0x270];
            }
        } else {
            cel = lbl_80471518[field_0x270];
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", cel + 0xC0, NULL, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, cel + 0xC0, 0, 2, -1, 0, 0, 0);
    }
}

void cls_804AAD60::fn_80315EE8() {
    updateFacing();
    if (field_0x234 == 0) {
        s32 found = -1;
        JGeometry::TBox2f box(field_0x258);
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        for (s32 i = 0; i < 4; i++) {
            if (!fn_8023EA28(i) && !fn_8022F478(i) && fn_802300EC(i) && fn_80230CFC(i) == 2) {
                const JGeometry::TVec3f* pPos = getPlayerPos_8023E724(i);
                bool inBox = false;
                if (pPos->x >= box.i.x && pPos->y >= box.i.y && pPos->x < box.f.x && pPos->y < box.f.y) {
                    inBox = true;
                }
                if (inBox) {
                    found = i;
                    break;
                }
            }
        }
        if (found >= 0) {
            fn_8022FF8C(found, &box, field_0x4, mSerial, 0, 0);
            bool started;
            if (fn_8038AD24(found, 0x47E, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                field_0x254 = found;
                field_0x268 = 0x47E;
                field_0x234 = 1;
                started = true;
            } else {
                started = false;
            }
            if (started) {
                JGeometry::TVec3f targetPos = *getPlayerPos_8023E724(found);
                targetPos.y = box.i.y - 1.0f;
                fn_80230BF4(found, 3);
                fn_80230498(found, &targetPos, 0x3E, 0xFF, -0x80, -0x80, true);
                startSound_801F0E34(0x50057, 0);
            }
        } else {
            JGeometry::TBox2f box2(mBox);
            box2.i.x += mPos.x;
            box2.i.y += mPos.y;
            box2.f.x += mPos.x;
            box2.f.y += mPos.y;
            fn_8022FEB8(&box2, field_0x4, mSerial, true, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (gGameRoot_8053AAF8->field_0x7e != 0) {
                    if (fn_8038AD24(p, 0x47C, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x120)) {
                        field_0x254 = p;
                        field_0x268 = 0x47C;
                        field_0x234 = 1;
                    }
                } else {
                    if (fn_8038AD24(p, 0x47D, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x10)) {
                        field_0x254 = p;
                        field_0x268 = 0x47D;
                        field_0x234 = 1;
                    }
                }
            }
        }
    } else if (field_0x234 == 1) {
        switch (field_0x268) {
        case 0x47D: {
            JGeometry::TBox2f box(mBox);
            box.i.x += mPos.x;
            box.i.y += mPos.y;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_80388CAC(field_0x254, mSerial)) {
                field_0x254 = -1;
                field_0x234 = 0;
            }
            break;
        }
        case 0x47E: {
            JGeometry::TBox2f box(field_0x258);
            box.i.x += mPos.x;
            box.i.y += mPos.y;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            fn_8022FE80(field_0x254, field_0x4, mSerial, true, false);
            if (fn_803865B8(field_0x254)) {
                if (fn_8037EFB4(field_0x254) == 0) {
                    fn_80384C58(field_0x254, mSerial);
                    if (gGameRoot_8053AAF8->field_0x7e != 0 && lbl_80529DEC.field_0x24e != 0) {
                        lbl_8053ABF8 = 1;
                        lbl_80529DEC.field_0x24e = 0;
                        gGameRoot_8053AAF8->fn_8013670C(12);
                        field_0x234 = 2;
                    } else {
                        lbl_8053ABF8 = 1;
                        gGameRoot_8053AAF8->fn_8013670C(6);
                        field_0x234 = 2;
                    }
                } else {
                    if (fn_8038470C(field_0x254, 0x47F)) {
                        field_0x268 = 0x47F;
                    }
                }
            }
            break;
        }
        case 0x47F: {
            JGeometry::TBox2f box(field_0x258);
            box.i.x += mPos.x;
            box.i.y += mPos.y;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            fn_8022FE80(field_0x254, field_0x4, mSerial, true, false);
            if (fn_80388CAC(field_0x254, mSerial)) {
                field_0x254 = -1;
                field_0x234 = 0;
            }
            break;
        }
        case 0x47C:
        default: {
            JGeometry::TBox2f box(mBox);
            box.i.x += mPos.x;
            box.i.y += mPos.y;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_80388CAC(field_0x254, mSerial)) {
                field_0x254 = -1;
                field_0x234 = 0;
            }
            break;
        }
        }
    }
}

void cls_804AAD60::fn_80315C84() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        fn_8022FEB8(&box, field_0x4, mSerial, false, false);
        s32 p = fn_8022FD8C(mSerial);
        if (p >= 0 && fn_802300EC(p)) {
            if (fn_8038AD24(p, 0x480, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                field_0x254 = p;
                field_0x268 = 0x480;
                field_0x234 = 1;
            }
        }
    } else {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_80388CAC(field_0x254, mSerial)) {
            gGameRoot_8053AAF8->fn_8013665C(10, 2);
            field_0x234 = 2;
        }
    }
}

void cls_804AAD60::fn_80315C80() {}

void cls_804AAD60::fn_80315864() {
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
    if (field_0x234 == 0) {
        switch (field_0x238) {
        case 0: {
            bool ok;
            if (fn_8038470C(field_0x254, 0x486)) {
                field_0x268 = 0x486;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok) {
                for (s32 n = gGameRoot_8053AAF8->fn_80138A30() - 1; n > 0; n--) {
                    for (s32 i = 0; i < n; i++) {
                        s32 vNext = fn_80236228(field_0x244[i + 1]);
                        if (fn_80236228(field_0x244[i]) < vNext) {
                            s32 tmp = field_0x244[i];
                            field_0x244[i] = field_0x244[i + 1];
                            field_0x244[i + 1] = tmp;
                        }
                    }
                }
            }
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4: {
            s32 p = field_0x244[4 - field_0x238];
            s32 score = fn_80236228(p);
            s32 rank = 0;
            for (s32 i = 0; i < 4; i++) {
                if (i != p && (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i))) {
                    if (score < fn_80236228(i)) {
                        rank++;
                    }
                }
            }
            fn_803894BC(field_0x254, rank + 1);
            fn_8038938C(field_0x254, fn_80236228(field_0x244[4 - field_0x238]));
            if (rank != 0) {
                u32 msg = field_0x244[4 - field_0x238] + 0x48B;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                }
            } else {
                u32 msg = field_0x244[4 - field_0x238] + 0x48F;
                bool ok;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                    ok = true;
                } else {
                    ok = false;
                }
                if (ok) {
                    gAudioMgr_8053AF80->fn_80458F04(0x1000027);
                }
            }
            break;
        }
        case 5:
        default: {
            s32 count = gGameRoot_8053AAF8->fn_80138A30();
            s32 total = 0;
            for (s32 i = 0; i < count; i++) {
                total += fn_80236228(i);
            }
            fn_80389254(field_0x254, total);
            s32 bonus = total / 1000;
            if (bonus != 0) {
                for (s32 i = 0; i < bonus; i++) {
                    if (lbl_80529DEC.field_0x24f < 99) {
                        lbl_80529DEC.field_0x24f++;
                    } else {
                        lbl_80529DEC.field_0x24f = 99;
                    }
                }
                fn_8038911C(field_0x254, bonus);
                if (fn_8038470C(field_0x254, 0x493)) {
                    field_0x268 = 0x493;
                    field_0x234 = 1;
                }
            } else {
                if (fn_8038470C(field_0x254, 0x495)) {
                    field_0x268 = 0x495;
                    field_0x234 = 1;
                }
            }
            break;
        }
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 5) {
                if (field_0x238 == 0) {
                    field_0x238 = 5 - gGameRoot_8053AAF8->fn_80138A30();
                } else {
                    field_0x238++;
                }
                field_0x234 = 0;
            } else {
                fn_80384C58(field_0x254, mSerial);
                fn_803B8C74(lbl_80529DEC.field_0x24f);
                gGameRoot_8053AAF8->fn_8013665C(10, 2);
                field_0x234 = 2;
            }
        }
    }
}

void cls_804AAD60::fn_80315460() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x4CE, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x4CE;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4CF)) {
                field_0x268 = 0x4CF;
                field_0x234 = 1;
            }
            break;
        case 2: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            s32 i = 0;
            for (; i < count; i++) {
                if (getTowerViewByte(i) != 0) {
                    break;
                }
            }
            if (i < count) {
                field_0x268 = 0x4D0;
                fn_803894BC(field_0x254, i + 1);
            } else {
                field_0x268 = 0x4D1;
            }
            u32 msg = field_0x268;
            bool ok;
            if (fn_8038470C(field_0x254, msg)) {
                field_0x268 = msg;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok && field_0x268 == 0x4D0) {
                lbl_8053AED0->fn_8043D874(i, 500, 0);
            }
            break;
        }
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 2) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_80315080() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x4CE, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x4CE;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4CF)) {
                field_0x268 = 0x4CF;
                field_0x234 = 1;
            }
            break;
        case 2: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            s32 i = 0;
            for (; i < count; i++) {
                if (getTowerViewByte(i) != 0) {
                    break;
                }
            }
            s32 idx = i & 3;
            fn_803894BC(field_0x254, idx + 1);
            bool ok;
            if (fn_8038470C(field_0x254, 0x4D2)) {
                field_0x268 = 0x4D2;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok) {
                lbl_8053AED0->fn_8043D874(idx, 300, 0);
            }
            break;
        }
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 2) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_80314B1C() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x4CE, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x4CE;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4CF)) {
                field_0x268 = 0x4CF;
                field_0x234 = 1;
            }
            break;
        case 2: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4D3)) {
                field_0x268 = 0x4D3;
                field_0x234 = 1;
            }
            s32 i = 0;
            for (; i < count; i++) {
                if (getTowerViewByte(i) != 0) {
                    break;
                }
            }
            if (i >= count) {
                field_0x238 = 7;
            }
            break;
        }
        case 3:
        case 4:
        case 5:
        case 6: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            while (field_0x238 < 7) {
                if (getTowerViewByte(field_0x238 - 3) != 0) {
                    break;
                }
                field_0x238++;
            }
            if (field_0x238 < count + 3) {
                u32 msg = field_0x238 + 0x4D1;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                }
                break;
            }
            // fallthrough to case 7
        }
        case 7: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            bool ok;
            if (fn_8038470C(field_0x254, 0x4D8)) {
                field_0x268 = 0x4D8;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok) {
                for (s32 i = 0; i < 4; i++) {
                    if (getTowerViewByte(i) != 0) {
                        lbl_8053AED0->fn_8043D874(i, 300, 0);
                    }
                }
                field_0x238 = 8;
            }
            break;
        }
        case 8:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4D9)) {
                field_0x268 = 0x4D9;
                field_0x234 = 1;
            }
            break;
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 8) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_803146DC() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x481, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x481;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
        case 2:
        case 3:
        case 4: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            s32 count = fn_80235ACC(field_0x238 - 1);
            fn_8038938C(field_0x254, count);
            if (count != 0) {
                if (field_0x23c == 3) {
                    fn_80389254(field_0x254, count * 5);
                } else if (field_0x23c == 6) {
                    fn_80389254(field_0x254, count * 10);
                } else {
                    fn_80389254(field_0x254, count * 20);
                }
                u32 msg = field_0x238 + 0x481;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                }
            } else {
                u32 msg = field_0x238 + 0x486;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                }
            }
            break;
        }
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 >= 1 && field_0x238 <= 4) {
                s32 count = fn_80235ACC(field_0x238 - 1);
                if (count != 0) {
                    if (field_0x23c == 3) {
                        lbl_8053AED0->fn_8043D874(field_0x238 - 1, count * 5, 0);
                    } else if (field_0x23c == 6) {
                        lbl_8053AED0->fn_8043D874(field_0x238 - 1, count * 10, 0);
                    } else {
                        lbl_8053AED0->fn_8043D874(field_0x238 - 1, count * 20, 0);
                    }
                }
            }
            if (field_0x238 >= gGameRoot_8053AAF8->fn_80138A30()) {
                field_0x238 = 4;
            }
            if (field_0x238 < 4) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_803142E4() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x4CE, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x4CE;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4CF)) {
                field_0x268 = 0x4CF;
                field_0x234 = 1;
            }
            break;
        case 2: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            f32 maxP = gGameRoot_8053AAF8->fn_80138A30();
            s32 rnd = (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * maxP) & 3;
            fn_803894BC(field_0x254, rnd + 1);
            bool ok;
            if (fn_8038470C(field_0x254, 0x4DA)) {
                field_0x268 = 0x4DA;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok) {
                lbl_8053AED0->fn_8043D874(rnd, 300, 0);
            }
            break;
        }
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 2) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_80313D7C() {
    updateFacing();
    JGeometry::TBox2f box(field_0x258);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 0) {
        s32 count = gGameRoot_8053AAF8->fn_80138A30();
        switch (field_0x238) {
        case 0: {
            fn_8022FEB8(&box, field_0x4, mSerial, false, false);
            s32 p = fn_8022FD8C(mSerial);
            if (p >= 0 && fn_802300EC(p)) {
                if (fn_8038AD24(p, 0x4DB, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x110)) {
                    field_0x254 = p;
                    field_0x268 = 0x4DB;
                    field_0x234 = 1;
                }
            }
            break;
        }
        case 1:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4CF)) {
                field_0x268 = 0x4CF;
                field_0x234 = 1;
            }
            break;
        case 2: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4DC)) {
                field_0x268 = 0x4DC;
                field_0x234 = 1;
            }
            s32 i = 0;
            for (; i < count; i++) {
                if (getTowerViewByte(i) != 0) {
                    break;
                }
            }
            if (i >= count) {
                field_0x238 = 7;
            }
            break;
        }
        case 3:
        case 4:
        case 5:
        case 6: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            while (field_0x238 < 7) {
                if (getTowerViewByte(field_0x238 - 3) != 0) {
                    break;
                }
                field_0x238++;
            }
            if (field_0x238 < count + 3) {
                u32 msg = field_0x238 + 0x4D1;
                if (fn_8038470C(field_0x254, msg)) {
                    field_0x268 = msg;
                    field_0x234 = 1;
                }
                break;
            }
            // fallthrough to case 7
        }
        case 7: {
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            bool ok;
            if (fn_8038470C(field_0x254, 0x4DD)) {
                field_0x268 = 0x4DD;
                field_0x234 = 1;
                ok = true;
            } else {
                ok = false;
            }
            if (ok) {
                for (s32 i = 0; i < count; i++) {
                    if (getTowerViewByte(i) != 0) {
                        lbl_8053AED0->fn_8043D874(i, -300, 0);
                    }
                }
                field_0x238 = 8;
            }
            break;
        }
        case 8:
            fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
            if (fn_8038470C(field_0x254, 0x4DE)) {
                field_0x268 = 0x4DE;
                field_0x234 = 1;
            }
            break;
        default:
            field_0x230 = 4;
            field_0x238 = 5;
            field_0x254 = 0;
            field_0x234 = 0;
            break;
        }
    } else if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_803865B8(field_0x254)) {
            if (field_0x238 < 8) {
                field_0x238++;
                field_0x234 = 0;
            } else {
                field_0x230 = 4;
                field_0x238 = 0;
                field_0x234 = 0;
            }
        }
    }
}

void cls_804AAD60::fn_80313C20() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    if (field_0x234 == 1) {
        fn_8022FF8C(field_0x254, &box, field_0x4, mSerial, 1, 0);
        if (fn_80388CAC(field_0x254, mSerial)) {
            field_0x254 = -1;
            field_0x234 = 0;
        }
    } else {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        s32 p = fn_8022FD8C(mSerial);
        if (p >= 0 && fn_802300EC(p) && fn_80230CFC(p) == 3) {
            u32 msg = field_0x268;
            if (fn_8038AD24(p, msg, mSerial, field_0x4, JGeometry::TVec3f(mPos), 0x220)) {
                field_0x254 = p;
                field_0x268 = msg;
                field_0x234 = 1;
            }
        }
    }
}
