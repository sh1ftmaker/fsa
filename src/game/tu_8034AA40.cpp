#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ActorManager.h"
#include "game/BoxTest.h"
#include "game/GameState.h"
#include "game/Unk_8045A080.h"
#include "game/act/act_cucco.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/tu_8034AA40.h"

// .rodata 0x804720E8: the start timers (state kinds 0, 1 use words 0 and the .sdata2 pair
// {20, 15}), the frame indices by facing and sub-state (16 words), and the frames of kind 1 (8).
static const s32 lbl_804720E8[28] = {10, 7, 10, 7, 1, 0, 2, 0, 4, 3, 5, 3, 7, 6, 8, 6, 10, 9, 11, 9, 0, 1, 2, 3, 4, 5, 6, 7};

// 0x8034C464.
// FIXME(shotgun): 96.25 %, missing/extra + reloc-name
void cls_804AC8C0::init() {
    field_0x230 = 0;
    field_0xb0 = 0x1000;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    mLife = 10000;
    field_0x238 = 0;
    field_0x234 = 0;
    field_0x2c0 = 0;
    field_0x244 = field_0x90 & 0xF;
    field_0x248 = 0;
    field_0x24c = (field_0x90 >> 4) & 0xFF;
    field_0x270 = 0;
    field_0x250 = 60;
    field_0x254 = -1;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x28f = 0;
    field_0x28c[0] = 0;
    field_0x28c[1] = 0;
    field_0x28c[2] = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x274[i] = 0;
        field_0x284[i] = 0;
        field_0x260[i] = -1;
        field_0x288[i] = 0;
        field_0x290[i] = *getPlayerPos_8023E724(i);
    }
    mPos.x = 16.0f * (f32)((s32)mPos.x / 16) + 8.0f;
    mPos.y = 16.0f * (f32)((s32)mPos.y / 16) + 8.0f;
    JGeometry::TVec3f dir;
    f32 dist;
    fn_801EE558(&dir, &dist, 0);
    if (dist < 100.0f) {
        u16 angle = (s16)(JMath::atanTable_.atan2_(-dir.x, -dir.y) * 10430.378f);
        if (angle < 0x2000 || angle >= 0xE000) {
            field_0x23c = 3;
        } else if (angle < 0x6000) {
            field_0x23c = 1;
        } else if (angle <= 0xA000) {
            field_0x23c = 2;
        } else {
            field_0x23c = 0;
        }
    } else if (field_0x230 == 0xD) {
        field_0x23c = 1;
    } else {
        field_0x23c = 2;
    }
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    switch (field_0x244) {
    case 0:
        field_0x234 = lbl_804720E8[0];
        field_0x2c0 = 0x423;
        break;
    case 1:
        field_0x234 = 20;
        field_0x2c0 = 0x427;
        break;
    }
    if (vfunc_0x34() == true) {
        field_0x230 = 0xB;
    }
}

// FIXME(shotgun): 82.66 %, missing/extra + reloc-name
void cls_804AC8C0::vfunc_0x10() {
    s32 serials[30];
    s32 count = 0;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4('NIWA', serial)) >= 0) {
        if (count < 30) {
            serials[count] = serial;
            count++;
        }
    }
    f32 roll = gGameRoot_8053AAF8->mRandom.get_ufloat_1();
    Cucco_804AB8C0* cucco = static_cast<Cucco_804AB8C0*>(
        getActorManager_801F856C()->findActorBySerial_801F666C(serials[(s32)((f32)count * roll)]));
    if (cucco != NULL) {
        cucco->field_0x2a3 = 1;
    }
}

// 0x8034AFE8.
// FIXME(shotgun): 82.03 %, missing/extra + reloc-name
void cls_804AC8C0::draw() {
    const char* name;
    s32 frame;
    switch (field_0x244) {
    case 0:
        name = "enemy_both2_sob.bin";
        frame = lbl_804720E8[4 + field_0x23c * 4 + field_0x238] + 0x218;
        break;
    case 1:
        name = "enemy_gc_sob.bin";
        frame = lbl_804720E8[20 + field_0x23c * 2 + field_0x238] + 0x2E6;
        break;
    }
    drawSprite_801F06F0(name, frame, NULL, 0, 1.0f, 1.0f);
}

// The counts per stage (.rodata 0x80472158, whose strings follow).
static const s32 lbl_80472158[3] = {10, 5, 1};

// FIXME(shotgun): 77.78 %, missing/extra + reloc-name
void cls_804AC8C0::fn_8034ACF4() {
    JGeometry::TBox2f box(9.0f, -7.0f, 103.0f, 87.0f);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023EA28(i) == 1) {
            field_0x288[i] = 1;
            field_0x290[i] = *getPlayerPos_8023E724(i);
        } else if (field_0x288[i] == 1) {
            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
            if (pos.x > field_0x290[i].x) {
                field_0x260[i] = 0;
            } else if (pos.x < field_0x290[i].x) {
                field_0x260[i] = 1;
            } else if (pos.y > field_0x290[i].y) {
                field_0x260[i] = 2;
            } else {
                field_0x260[i] = 3;
            }
            field_0x288[i] = 0;
        }
        JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
        if (pos.z <= 1.0f && containsPoint(box, pos) == true) {
            lbl_8053AED0->fn_8043D67C(0x410, 0, 0);
            JGeometry::TVec3f push(0.0f, 0.0f, 0.0f);
            switch (field_0x260[i]) {
            case 0:
                push.set(-2.0f, 0.0f, 5.0f);
                break;
            case 1:
                push.set(2.0f, 0.0f, 5.0f);
                break;
            case 2:
                push.set(0.0f, -2.0f, 5.0f);
                break;
            case 3:
                push.set(0.0f, 2.0f, 5.0f);
                break;
            }
            fn_802314D8(i, push);
        }
    }
}

// FIXME(shotgun): 68.97 %, missing/extra + reloc-name
s32 cls_804AC8C0::fn_8034AAA0() {
    JGeometry::TBox2f box(24.0f, 8.0f, 88.0f, 72.0f);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    s32 count = 0;
    field_0x274[0] = 0;
    field_0x274[1] = 0;
    field_0x274[2] = 0;
    field_0x274[3] = 0;
    s32 serial = -1;
    while ((serial = getActorManager_801F856C()->fn_801F78F4('NIWA', serial)) >= 0) {
        Cucco_804AB8C0* cucco = static_cast<Cucco_804AB8C0*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (cucco != NULL && field_0x4 == cucco->field_0x4 && cucco->mPos.z < 8.0f &&
            containsPoint(box, cucco->mPos) == true && !cucco->field_0xc4) {
            cucco->field_0x2a8 = 1;
            count++;
            field_0x274[cucco->field_0x25c]++;
        }
    }
    if (lbl_80472158[field_0x25c] >= field_0x24c - count && field_0x28c[field_0x25c] == 0) {
        lbl_8053AED0->fn_8043D67C(0x42A, 0, 0);
        field_0x28c[field_0x25c] = 1;
        if (field_0x25c == 2) {
            gAudioMgr_8053AF80->fn_80458508(1);
        }
        if (field_0x25c < 2) {
            field_0x25c++;
        }
    }
    return count;
}

// Destructor at 0x8034AA40.
cls_804AC8C0::~cls_804AC8C0() {}
