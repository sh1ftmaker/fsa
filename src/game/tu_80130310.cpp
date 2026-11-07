#include "game/EnemyBase.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/cls_8049E350.h"
#include "game/act/tu_80130310.h"

// Per step: the message, which actor reacts (0: field_0x234, 1 and 2: field_0x238) and the value.
static struct {
    u16 msg;
    s32 mode;
    s32 value;
} lbl_8049EA58[4] = {
    {0x4BB, 1, 1},
    {0x4BC, 0, 0},
    {0x4BD, 1, 1},
    {0x4BE, 2, 2},
};

inline void cls_8049EA88::setState(s32 state) {
    field_0x230 = state;
    switch (field_0x230) {
    case 1:
        field_0x24c = 0;
        break;
    case 3:
        field_0x248 = 0;
        break;
    case 5:
        fn_80230534(false);
        fn_801F3668();
        break;
    }
}

// FIXME(shotgun): 99.7 %, reloc-name
void cls_8049EA88::init() {
    ActorBase_804A5770::init();
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mBox2.i.x = mBox.i.x;
    mBox2.i.y = mBox.i.y;
    mBox2.f.x = mBox.f.x;
    mBox2.f.y = mBox.f.y;
    field_0x80.i.x = mBox.i.x;
    field_0x80.i.y = mBox.i.y;
    field_0x80.f.x = mBox.f.x;
    field_0x80.f.y = mBox.f.y;
    field_0x23c = (field_0x90 >> 22) & 0x1F;
    field_0x240 = (field_0x90 >> 17) & 0x1F;
}

// FIXME(shotgun): 99.9 %, reloc-name
void cls_8049EA88::vfunc_0x10() {
    s32 serial = getActorManager_801F856C()->fn_801F7884('OPGA', field_0x4);
    while (serial != -1) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            continue;
        }
        if (static_cast<cls_8049E350*>(actor)->field_0x238 == 1) {
            field_0x238 = actor->mSerial;
        } else {
            field_0x234 = actor->mSerial;
        }
        serial = getActorManager_801F856C()->fn_801F78F4('OPGA', serial);
    }
    ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
    mPos = target->mPos;
    mPos.y += 24.0f;
}

// FIXME(shotgun): 89.7 %, missing/extra
void cls_8049EA88::update() {
    ActorBase_804A5770::update();
    if (field_0x230 >= 2 && field_0x230 <= 4) {
        gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x7f5 == 0) {
            camera->field_0x7f4 = 1;
            camera->field_0x7f8 = 5;
            camera->field_0x808.set(mPos.x, mPos.y - 96.0f, mPos.x, 96.0f + mPos.y);
            camera->field_0x7f6 = 0;
        }
    }
    switch (field_0x230) {
    case 0:
        if (vfunc_0x34()) {
            setState(1);
        }
        break;
    case 1: {
        field_0x24c++;
        if (field_0x24c >= 120) {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
            if (a != NULL && a->field_0x98 != 1) {
                a->field_0x98 = 1;
                static_cast<cls_8049E350*>(a)->fn_80125BF4();
            }
        }
        if (getGameView_802D6A9C()->fn_802D800C(field_0x23c)) {
            setState(2);
        }
        break;
    }
    case 2: {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        if (fn_80130370()) {
            for (int i = 0; i < 4; i++) {
                fn_80230BF4(i, 3);
                fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0xFF, -0x80, -0x80, true);
            }
            setState(3);
        }
        break;
    }
    case 3: {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        field_0x248++;
        if (field_0x248 >= 60) {
            if (field_0x244 >= 4) {
                setState(5);
            } else if (fn_8013056C(lbl_8049EA58[field_0x244].msg, lbl_8049EA58[field_0x244].mode, lbl_8049EA58[field_0x244].value) >= 0) {
                setState(4);
            }
        }
        break;
    }
    case 4: {
        cls_804A03A8* sub = gGameRoot2_8053AB10->field_0x30;
        if (sub->field_0x284 == 0) {
            sub->field_0x284 = 1;
            sub->field_0x28c = 0.05f;
        }
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        bool done = false;
        if (fn_803865B8(0)) {
            fn_80388EEC(0);
            s32 serial = 0;
            switch (lbl_8049EA58[field_0x244].mode) {
            case 0:
                serial = field_0x234;
                break;
            case 1:
                serial = field_0x238;
                break;
            case 2:
                serial = field_0x238;
                break;
            }
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (a != NULL) {
                static_cast<cls_8049E350*>(a)->fn_80125C58(false);
            }
            done = true;
        }
        if (done) {
            field_0x244++;
            setState(3);
        }
        break;
    }
    }
}

// Not auto-inlined: update calls fn_8013056C out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 94.5 %, scheduling
s32 cls_8049EA88::fn_8013056C(u16 msg, s32 mode, s32 value) {
    s32 serial = field_0x238;
    if (mode == 1) {
        serial = field_0x234;
    }
    fn_8022FE80(0, field_0x4, serial, true, false);
    if (fn_8038BD08(0, msg, field_0x4, mPos, 0x22)) {
        switch (mode) {
        case 0: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
            }
            break;
        }
        case 1: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
            }
            break;
        }
        case 2: {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
            if (a != NULL) {
                a->field_0x98 = value;
                static_cast<cls_8049E350*>(a)->fn_80125C58(true);
                ActorBase_804A5770* b = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x234);
                if (b != NULL) {
                    b->field_0x98 = value;
                }
            }
            break;
        }
        }
        return 0;
    }
    return -1;
}
#pragma auto_inline reset

// FIXME(shotgun): 98.5 %, operands + reloc-name
bool cls_8049EA88::fn_80130370() {
    ActorBase_804A5770* target = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x238);
    if (target == NULL) {
        return false;
    }
    JGeometry::TVec3f pos = target->mPos;
    const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
    pos.x += -56.0f;
    pos.y += 48.0f;
    bool done = true;
    for (int i = 0; i < 4; i++) {
        pos.x += 20.0f;
        JGeometry::TVec3f p = *getPlayerPos_8023E724(i);
        u16 angle = (s32)(JMath::atanTable_.atan2_(pos.x - p.x, pos.y - p.y) * 10430.378f);
        s32 dir = fn_801F681C(angle);
        bool xDone;
        bool yDone;
        f32 sn = sincos.sinShort(angle);
        f32 cs = sincos.cosShort(angle);
        if (fn_801CD950(&p.x, pos.x, sn)) {
            done = false;
            xDone = false;
        } else {
            xDone = true;
        }
        if (fn_801CD950(&p.y, pos.y, cs)) {
            done = false;
            yDone = false;
        } else {
            yDone = true;
        }
        fn_80230BF4(i, dir);
        if (xDone && yDone) {
            fn_80230BF4(i, 3);
            fn_80230498(i, getPlayerPos_8023E724(i), 0x3F, 0xFF, -0x80, -0x80, true);
        } else {
            fn_802303FC(i, &p, true);
        }
    }
    return done;
}


cls_8049EA88::~cls_8049EA88() {}
