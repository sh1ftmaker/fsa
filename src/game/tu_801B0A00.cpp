#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Message.h"
#include "game/ViewLayer.h"
#include "game/Unk_8045A080.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include <math.h>

// Sprite frames of the third step, .data at 0x804A35A0.
static const s32 lbl_804A35A0[3] = {2, 1, 0};

// The scripted camera move towards pos (mode 4).
static inline void setCameraTarget(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 4;
        camera->field_0x7fc.set(pos);
        camera->field_0x7f6 = 0;
    }
}

// The scripted camera move to box (mode 5).
static inline void setCameraBox(const JGeometry::TBox2f& box) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x7f5 == 0) {
        camera->field_0x7f4 = 1;
        camera->field_0x7f8 = 5;
        camera->field_0x808.set(box);
        camera->field_0x7f6 = 0;
    }
}

static inline void setCameraFocus(const JGeometry::TVec3f& pos) {
    Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
    if (camera->field_0x840 == 0) {
        camera->field_0x840 = 1;
        camera->field_0x844.i.x = camera->field_0x844.f.x = pos.x;
        camera->field_0x844.i.y = camera->field_0x844.f.y = pos.y;
        camera->field_0x854 = 1.0f;
        camera->field_0x1d4 = -1.0f;
        camera->field_0x1d8 = -1.0f;
    }
}

static inline void requestSpot() {
    cls_804A03A8* obj = gGameRoot2_8053AB10->field_0x30;
    if (obj->field_0x284 == 0) {
        obj->field_0x284 = 1;
        obj->field_0x28c = 0.05f;
    }
}

// 0x801B2394.
void cls_804A35AC::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x250 = 0;
    field_0x254 = -1;
    gAudioMgr_8053AF80->fn_80458F9C(0x3c);
    field_0x98 = 2;
    fn_801B2138(field_0x250);
    field_0x264.x = mPos.x;
    field_0x264.y = mPos.y;
    field_0x264.z = mPos.z;
    field_0x270.x = mPos.x;
    field_0x270.y = mPos.y;
    field_0x270.z = mPos.z;
    field_0x27c.x = mPos.x;
    field_0x27c.y = mPos.y;
    field_0x27c.z = mPos.z;
    field_0x258 = 0;
    field_0x259 = 0;
    field_0x288[0].x = mPos.x;
    field_0x288[0].y = mPos.y;
    field_0x288[0].z = mPos.z;
    field_0x288[1].x = mPos.x;
    field_0x288[1].y = mPos.y;
    field_0x288[1].z = mPos.z;
    field_0x288[2].x = mPos.x;
    field_0x288[2].y = mPos.y;
    field_0x288[2].z = mPos.z;
    field_0x288[3].x = mPos.x;
    field_0x288[3].y = mPos.y;
    field_0x288[3].z = mPos.z;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    field_0x25c = 0.0f;
    field_0x260 = 0.0f;
    getGameView_802D6A9C()->fn_802D7440(false);
    s32 fade = 200;
    switch (field_0x90) {
    case 0:
        if (!getGameView_802D6A9C()->fn_802D800C(0x10)) {
            field_0x230 = 0;
        } else {
            field_0x90 = 2;
            getGameView_802D6A9C()->fn_802D8050(1, true, 0xFF);
            field_0x230 = 0x14;
        }
        break;
    case 1:
        field_0x230 = 15;
        fade = 100;
        break;
    }
    if (mAlive == true && field_0x90 != 2) {
        fn_80119DD0()->fn_80119998(fade, 0, 0, 0, 0, 0, 0);
    }
}

// Destructor at 0x801B22F4.
cls_804A35AC::~cls_804A35AC() {
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    getGameView_802D6A9C()->fn_802D7440(true);
}

// 0x801B2138.
// FIXME(shotgun): 96.26 %, missing/extra + reloc-name; behaviour clean.
void cls_804A35AC::fn_801B2138(s32 arg) {
    if (field_0x254 != arg) {
        field_0x254 = arg;
        field_0x259 = 0;
        field_0x234 = 0;
        field_0x240 = 0;
    }
    if (field_0x234 != 0) {
        return;
    }
    switch (field_0x250) {
    case 0:
        field_0x24c = field_0x98 << 3;
    // fall through
    case 1:
        field_0x24c = (field_0x98 << 3) + field_0x240;
        field_0x234 = 5;
        field_0x240++;
        field_0x240 &= 7;
    // fall through
    case 2:
        field_0x24c = 2;
    // fall through
    case 3:
        field_0x24c = lbl_804A35A0[field_0x240];
        field_0x234 = 8;
        if (field_0x240 == 0) {
            field_0x234 = 40;
        }
        field_0x240++;
        if (field_0x240 <= 2) {
            return;
        }
        field_0x240 = 0;
        field_0x250 = 4;
        field_0x254 = field_0x250;
    // fall through
    case 4:
        switch (field_0x98) {
        case 0:
            field_0x98 = 2;
            break;
        case 1:
            field_0x98 = 3;
            break;
        case 2:
            field_0x98 = 1;
            break;
        case 3:
            field_0x98 = 0;
            break;
        }
        field_0x240++;
        field_0x234 = 5;
        if (field_0x240 >= 2) {
            field_0x240 = 0;
            field_0x259 = 1;
            field_0x250 = 0;
            field_0x254 = field_0x250;
        }
        field_0x24c = field_0x98 << 3;
        break;
    }
}

// Not auto-inlined: update calls fn_801B2000 out of line in the original (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// 0x801B2000.
// FIXME: 84.40 %, missing/extra; behaviour near.
// FIXME: operands differ: fn_802303FC's original call passes 0xFF for its bool flag (we pass true); loop pointer and layer registers differ.
void cls_804A35AC::fn_801B2000(f32 yOffset, s32 mode) {
    f32 y = mPos.y - yOffset;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                const JGeometry::TVec3f* pos = getPlayerPos_8023E724(i);
                field_0x288[i].x = pos->x;
                field_0x288[i].y = pos->y;
                field_0x288[i].z = pos->z;
                if (mode == 0) {
                    if (field_0x258 == 1) {
                        field_0x288[i].y = y;
                    }
                    fn_80230498(i, &field_0x288[i], 0x3E, 0xFF, -128, -128, true);
                } else {
                    addCalc2_801F71A4(&field_0x288[i].y, y, 0.3f, 1.0f);
                    fn_80230BF4(i, 2);
                    fn_802303FC(i, &field_0x288[i], 0xFF);
                    if (FABS(getPlayerPos_8023E724(i)->y - y) < 1.0f) {
                        field_0x258 = 1;
                    }
                }
            }
        }
    }
}
#pragma auto_inline reset

// 0x801B0A90.
// FIXME: 98.70 %, scheduling+regalloc + reloc-name; behaviour near.
void cls_804A35AC::update() {
    JGeometry::TBox2f box;
    JGeometry::TVec3f pos;
    box = *layerBox(field_0x4);
    if (field_0x230 != 0 && field_0x230 != 1 && field_0x230 != 2 && field_0x230 != 3 && field_0x230 != 4 &&
        field_0x230 != 5) {
        getGameView_802D6A9C()->fn_802D79E8(field_0x4, 1.0f, 0.0f, false);
        pos.set(mPos);
        mPos.zero();
        mPos.set(pos);
    }
    if (field_0x230 == 8 || field_0x230 == 9 || field_0x230 == 10) {
        pos.set(mPos);
        pos.y -= 8.0f;
        setCameraTarget(pos);
        requestSpot();
    }
    switch (field_0x230) {
    case 0:
        if (getActorManager_801F856C()->fn_801F68E0()) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023F264(i)) {
                    if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                        pos.set(*getPlayerPos_8023E724(i));
                        pos.x = 16.0f * i + 4840.0f;
                        fn_80230498(i, &pos, 0x3E, 0xFF, -128, -128, true);
                        field_0x264.y = getPlayerPos_8023E724(i)->y;
                        fn_80230BF4(i, 2);
                    }
                }
            }
            field_0x250 = 2;
            field_0x244 = 0;
            field_0x230 = 1;
        }
        break;
    case 1:
        fn_801B2000(32.0f, 0);
        field_0x244++;
        if (field_0x244 < 60) {
            break;
        }
        field_0x244 = 61;
        setCameraBox(box);
        fn_801B2000(128.0f, 1);
        if (!field_0x258) {
            break;
        }
        field_0x258 = 0;
        field_0x244 = 0;
        field_0x230 = 2;
    case 2:
        fn_801B2000(32.0f, 1);
        field_0x244++;
        if (field_0x244 == 30) {
            pos.set(mPos);
            pos.y -= 16.0f;
            startSound_801F0E34(0x400B5, 0);
            spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
            gAudioMgr_8053AF80->fn_80458FF0(0x100000F, false, 0);
            field_0x250 = 3;
        }
        pos.set(mPos);
        pos.y -= 8.0f;
        setCameraTarget(pos);
        requestSpot();
        if (!field_0x258) {
            break;
        }
        fn_801B2000(32.0f, 0);
        if (field_0x244 < 30) {
            break;
        }
        field_0x244 = 0;
        field_0x230 = 3;
    case 3:
        pos.set(mPos);
        pos.y -= 8.0f;
        setCameraFocus(pos);
        requestSpot();
        if (!field_0x259) {
            break;
        }
        field_0x244++;
        if (field_0x244 < 60) {
            break;
        }
        field_0x244 = 60;
        pos.zero();
        pos.y = -10.0f;
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (fn_8038AD24(0, 0x2D6, mSerial, field_0x4, pos, 0x4102)) {
            field_0x230 = 4;
        }
        break;
    case 4:
        pos.set(mPos);
        pos.y -= 8.0f;
        setCameraFocus(pos);
        requestSpot();
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (!fn_80388CAC(0, mSerial)) {
            break;
        }
        field_0x244 = 0;
        field_0x230 = 5;
    case 5:
        field_0x244++;
        if (field_0x244 < 60) {
            pos.set(mPos);
            pos.y -= 8.0f;
            setCameraFocus(pos);
            requestSpot();
            break;
        }
        field_0x244 = 0;
        startSound_801F0E34(0x400B5, 0);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x33B, 0, 1, NULL, NULL, true, 1.0f);
        getGameView_802D6A9C()->fn_802D8050(1, true, 0xFF);
        field_0x230 = 6;
    case 6:
        field_0x244++;
        if (field_0x244 < 120) {
            pos.set(mPos);
            pos.y -= 8.0f;
            setCameraFocus(pos);
            requestSpot();
            break;
        }
        gAudioMgr_8053AF80->fn_80458FF0(0x1000022, false, 0);
        field_0x244 = 0;
        field_0x230 = 7;
    case 7:
        pos.set(field_0x264);
        setCameraTarget(pos);
        requestSpot();
        field_0x244++;
        if (field_0x244 < 300) {
            break;
        }
        field_0x244 = 0;
        field_0x230 = 8;
    case 8:
        field_0x244++;
        if (field_0x244 < 120) {
            break;
        }
        field_0x244 = 120;
        pos.zero();
        pos.y = -10.0f;
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (fn_8038AD24(0, 0x2D7, mSerial, field_0x4, pos, 0x4102)) {
            field_0x244 = 0;
            field_0x230 = 9;
        }
        break;
    case 9:
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (!fn_80388CAC(0, mSerial)) {
            break;
        }
        field_0x230 = 10;
    case 10:
        field_0x244++;
        if (field_0x244 < 60) {
            break;
        }
        field_0x244 = 0;
        field_0x270.set(mPos);
        field_0x250 = 4;
        field_0x230 = 11;
        break;
    case 11:
        pos.set(field_0x270);
        pos.y -= 8.0f;
        setCameraTarget(pos);
        requestSpot();
        if (!field_0x259) {
            break;
        }
        field_0x250 = 1;
        field_0x244 = 0;
        field_0x258 = 0;
        field_0x230 = 12;
    case 12:
        fn_801B2000(32.0f, 1);
        addCalc2_801F71A4(&mPos.y, 16.0f + mPos.y, 0.3f, 1.0f);
        field_0x244++;
        if (field_0x244 < 240) {
            pos.set(field_0x270);
            pos.y -= 8.0f;
            setCameraTarget(pos);
            requestSpot();
            break;
        }
        field_0x244 = 0;
        field_0x230 = 13;
    case 13:
        pos.set(field_0x264);
        setCameraTarget(pos);
        requestSpot();
        field_0x244++;
        if (field_0x244 < 120) {
            break;
        }
        fn_80119DD0()->fn_80119998(100, 0, 1, 0, 0, 101, 0);
        getGameView_802D6A9C()->fn_802D8050(0x10, true, 0xFF);
        field_0x244 = 100;
        field_0x230 = 14;
    case 14:
        pos.set(field_0x264);
        setCameraTarget(pos);
        requestSpot();
        field_0x244--;
        if (field_0x244 < 0) {
            getGameView_802D6A9C()->fn_802D9FF4(8, 0);
            getGameView_802D6A9C()->fn_802DBD60();
            mAlive = false;
        }
        break;
    case 15:
        if (!getActorManager_801F856C()->fn_801F68E0()) {
            break;
        }
        field_0x230 = 16;
    case 16:
        pos.set(field_0x264);
        pos.y += 100.0f;
        setCameraFocus(pos);
        requestSpot();
        field_0x234 = 200;
        field_0x230 = 17;
    case 17:
        pos.set(field_0x264);
        pos.y += 100.0f;
        setCameraFocus(pos);
        requestSpot();
        if (field_0x234 != 0) {
            break;
        }
        pos.zero();
        pos.y = -10.0f;
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (fn_8038AD24(0, 0x2E1, mSerial, field_0x4, pos, 0x4102)) {
            field_0x230 = 18;
        }
        break;
    case 18:
        pos.set(field_0x264);
        pos.y += 100.0f;
        setCameraFocus(pos);
        requestSpot();
        fn_8022FE80(0, field_0x4, mSerial, false, false);
        if (!fn_80388CAC(0, mSerial)) {
            break;
        }
        fn_80119DD0()->fn_80119998(50, 0, 1, 0, 0, 51, 0);
        field_0x234 = 100;
        field_0x230 = 19;
    case 19:
        pos.set(field_0x264);
        pos.y += 100.0f;
        setCameraFocus(pos);
        requestSpot();
        if (field_0x234 != 0) {
            break;
        }
        getGameView_802D6A9C()->fn_802D9FF4(9, 0);
        getGameView_802D6A9C()->fn_802DBD60();
        mAlive = false;
        break;
    case 20:
        if (!getActorManager_801F856C()->fn_801F68E0()) {
            break;
        }
        field_0x264.y = getPlayerPos_8023E724(0)->y;
        field_0x234 = 150;
        field_0x230 = 21;
    case 21:
        setCameraFocus(field_0x264);
        requestSpot();
        if (field_0x234 != 0) {
            break;
        }
        field_0x27c.set(field_0x264);
        field_0x27c.y += 24.0f;
        field_0x25c = 1.0f;
        if (mEmitters[0] == NULL) {
            mEmitters[0] = spawnEmitter_8013CC50(field_0x4, &field_0x27c, 0x224, 0, 0, NULL, NULL, true, 0.1f);
        }
        if (mEmitters[1] == NULL) {
            mEmitters[1] = spawnEmitter_8013CC50(field_0x4, &field_0x27c, 0x225, 0, 0, NULL, NULL, true, 0.1f);
        }
        if (mEmitters[0] == NULL || mEmitters[1] == NULL) {
            break;
        }
        pos.set(mPos);
        mPos.set(field_0x27c);
        startSound_801F0E34(0x400BA, 0);
        mPos.set(pos);
        field_0x230 = 22;
    case 22:
        setCameraFocus(field_0x264);
        requestSpot();
        field_0x244++;
        pos.set(mPos);
        mPos.set(field_0x27c);
        startSound_801F0E34(0x30058, 0);
        mPos.set(pos);
        for (s32 i = 0; i < 2; i++) {
            if (mEmitters[i]) {
                setEmitterPos_8013CB44(mEmitters[i], &field_0x27c, 0, 0, true);
                mEmitters[i]->setScale(field_0x25c);
            }
        }
        field_0x244++;
        f32 targetY = 500.0f + field_0x264.y;
        f32 targetX = field_0x264.x - 500.0f;
        addCalc2_801F71A4(&field_0x27c.x, targetX, 0.1f, 0.7f);
        addCalc2_801F71A4(&field_0x27c.y, targetY, 0.1f, 0.7f);
        if (field_0x244 < 700) {
            break;
        }
        fn_80119DD0()->fn_80119998(100, 0, 1, 0, 0, 101, 0);
        field_0x238 = 200;
        field_0x244 = 0;
        field_0x230 = 23;
    case 23:
        if (field_0x238 == 0) {
            getActorManager_801F856C()->fn_801F6884();
            gAudioMgr_8053AF80->fn_80458FF0(0x1000022, false, 0);
            getGameView_802D6A9C()->fn_802D9FF4(9, 2);
            getGameView_802D6A9C()->fn_802DBD60();
            mAlive = false;
        } else {
            setCameraFocus(field_0x264);
            requestSpot();
        }
        break;
    }
    fn_801B2138(field_0x250);
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

// 0x801B0A00.
// FIXME(shotgun): 97.31 %, different + reloc-name; behaviour drift.
// field_0x90 compare is cmplwi (u32), original cmpwi
void cls_804A35AC::draw() {
    s32 frame = 0x9B;
    if (field_0x90 == 0) {
        if (field_0x250 == 2 || field_0x250 == 3) {
            frame = 0x274;
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", frame + field_0x24c, NULL, 7, 1.0f, 1.0f);
    }
}
