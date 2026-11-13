#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_vaati_boss.h"
#include "game/act/tu_801D7AFC.h"
#include "JSystem/JMath/JMath.h"

void cls_801D6A88::fn_801D789C() {
    field_0x0 = 0;
    field_0x4 = 0;
    field_0x8 = 0;
    field_0xc.fn_8021BD18(2);
}

void cls_801D6A58::fn_801D7874() {
    field_0x0 = -1;
    field_0x4 = 0;
    field_0x5 = 0;
    field_0x8 = 0.0f;
    field_0xc = 0.0f;
    field_0x10 = 0.0f;
}

void cls_801D6A58::fn_801D7830() {
    field_0x8 *= 0.97f;
    field_0xc *= 0.97f;
    field_0x10 += -0.2f;
    if (field_0x10 < -4.0f) {
        field_0x10 = -4.0f;
    }
}

// FIXME(shotgun): 96.13 %, regalloc-only
void cls_801D7730::fn_801D77B4() {
    field_0x0[0x0] = -1;
    field_0x0[0x1] = -1;
    field_0x0[0x2] = -1;
    field_0x0[0x3] = -1;
    field_0x0[0x4] = -1;
    field_0x0[0x5] = -1;
    field_0x0[0x6] = -1;
    field_0x0[0x7] = -1;
    field_0x0[0x8] = -1;
    field_0x0[0x9] = -1;
    field_0x0[0xA] = -1;
    field_0x0[0xB] = -1;
    field_0x0[0xC] = -1;
    field_0x0[0xD] = -1;
    field_0x0[0xE] = -1;
    field_0x0[0xF] = -1;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0.0f;
    field_0x4c = 0.0f;
    field_0x50 = 0.0f;
    field_0x54 = 0;
    field_0x56 = 0;
    field_0x58 = 0;
    field_0x5c = 0.0f;
    field_0x60 = 0.0f;
    field_0x64 = 0;
}

// FIXME(shotgun): 98.94 %, regalloc-only
cls_801D7730::~cls_801D7730() {
    fn_801D7638();
}

cls_801D7730::cls_801D7730() {
    field_0x0[0x0] = -1;
    field_0x0[0x1] = -1;
    field_0x0[0x2] = -1;
    field_0x0[0x3] = -1;
    field_0x0[0x4] = -1;
    field_0x0[0x5] = -1;
    field_0x0[0x6] = -1;
    field_0x0[0x7] = -1;
    field_0x0[0x8] = -1;
    field_0x0[0x9] = -1;
    field_0x0[0xA] = -1;
    field_0x0[0xB] = -1;
    field_0x0[0xC] = -1;
    field_0x0[0xD] = -1;
    field_0x0[0xE] = -1;
    field_0x0[0xF] = -1;
    field_0x40 = 0;
    field_0x44 = 0;
    field_0x48 = 0.0f;
    field_0x4c = 0.0f;
    field_0x50 = 0.0f;
    field_0x54 = 0;
    field_0x56 = 0;
    field_0x58 = 0;
    field_0x5c = 0.0f;
    field_0x60 = 0.0f;
    field_0x64 = 0;
}

// Clears mAlive of each actor named by one of the 16 serials (serials below 0
// are empty slots). Vaati calls it on field_0x4c4 right before reconstructing it.
void cls_801D7730::fn_801D7638() {
    s32* p = field_0x0;
    s32 serial;
    for (s32 i = 0; i < 0x10; i++, p++) {
        serial = *p;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
}

// FIXME(shotgun): 93.58 %, scheduling+regalloc
void cls_801D7730::fn_801D74E0(const JGeometry::TVec3f* pos, s32 layer) {
    fn_801D77B4();
    field_0x40 = layer;
    field_0x48 = pos->x;
    field_0x4c = -22.0f + pos->y;
    field_0x50 = pos->z;
    if (fn_801CD2F8(field_0x40)) {
        field_0x4c = -25.0f + pos->y;
    }
    s32 count;
    if (fn_801CD2F8(field_0x40)) {
        field_0x44 = 1;
        count = 1;
    } else {
        field_0x44 = 2;
        count = 16;
    }
    for (s32 i = 0; i < count; i++) {
        field_0x0[i] = getActorManager_801F856C()->spawnActor_801F9484('BGBL', field_0x40, (const JGeometry::TVec3f*)&field_0x48, 0, -1, -1);
    }
}

// The ring's slot i is on a circle of radius field_0x5c around (field_0x48, field_0x4c).
// FIXME(shotgun): 50.24 %, missing/extra + reloc-name
void cls_801D7730::fn_801D72C0(const JGeometry::TVec3f* pos) {
    field_0x48 = pos->x;
    field_0x4c = -22.0f + pos->y;
    field_0x50 = pos->z;
    if (fn_801CD2F8(field_0x40)) {
        field_0x4c = -25.0f + pos->y;
    }
    for (s32 i = 0; i < 16; i++) {
        s32 serial = field_0x0[i];
        if (serial >= 0) {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (a != NULL) {
                s16 ang = -(s16)(field_0x54 + i * 0xFFF);
                f32 s = JMath::sincosTable_.sinShort(ang);
                f32 c = JMath::sincosTable_.cosShort(ang);
                JGeometry::TVec2f d(field_0x48 + (0.0f * c - field_0x5c * s) - field_0x48,
                                    field_0x4c + (0.0f * s + field_0x5c * c) - field_0x4c);
                d.normalize();
                d.scale(10.0f);
                a->field_0x3c.set(d.x, d.y, 0.0f);
            }
        }
    }
    field_0x44 = 4;
}

// FIXME(shotgun): 90.12 %, missing/extra + reloc-name
void cls_801D7730::fn_801D70BC(const JGeometry::TVec3f* pos, s32 player) {
    field_0x48 = pos->x;
    field_0x4c = -22.0f + pos->y;
    field_0x50 = pos->z;
    if (fn_801CD2F8(field_0x40)) {
        field_0x4c = -25.0f + pos->y;
    }
    s32 serial = field_0x0[0];
    if (serial >= 0) {
        ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (a != NULL) {
            f32 vx;
            f32 vy;
            if (player >= 0 && player < 4) {
                f32 dx = getPlayerPos_8023E724(player)->x - a->mPos.x;
                f32 dy = getPlayerPos_8023E724(player)->y - a->mPos.y;
                f32 sq = dx * dx + dy * dy;
                if (sq <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
                    dx = 1.0f;
                    dy = 0.0f;
                } else {
                    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
                    dx *= norm;
                    dy *= norm;
                }
                s16 ang = -(s16)(fn_801CD664(0xE39) - 0x71C);
                f32 s = JMath::sincosTable_.sinShort(ang);
                f32 c = JMath::sincosTable_.cosShort(ang);
                vx = dx * c - dy * s;
                vy = dx * s + dy * c;
            } else {
                s16 ang = fn_801CD664(0x10000);
                vx = JMath::sincosTable_.sinShort(ang);
                vy = JMath::sincosTable_.cosShort(ang);
            }
            a->field_0x3c = JGeometry::TVec3f(5.0f * vx, 5.0f * vy, 0.0f);
        }
    }
    field_0x44 = 4;
}

// FIXME(shotgun): 80.58 %, missing/extra
void cls_801D7730::fn_801D6E20(const JGeometry::TVec3f* pos) {
    field_0x48 = pos->x;
    field_0x4c = -22.0f + pos->y;
    field_0x50 = pos->z;
    if (fn_801CD2F8(field_0x40)) {
        field_0x4c = -25.0f + pos->y;
    }
    switch (field_0x44) {
    case 1: {
        s32 serial = field_0x0[0];
        if (serial >= 0) {
            ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (a != NULL) {
                a->mPos.x = field_0x48;
                a->mPos.y = field_0x4c;
                a->mPos.z = field_0x50;
            }
        }
        break;
    }
    case 2: {
        field_0x64++;
        if (field_0x64 >= 0x2D) {
            field_0x44 = 3;
        } else {
            f32 t = (f32)(0x2D - field_0x64);
            if (t < JGeometry::TUtil<f32>::epsilon()) {
                field_0x60 = 1.0f;
            } else {
                field_0x60 = (1.0f / t) * (1.0f - field_0x60) + field_0x60;
            }
            if (t < JGeometry::TUtil<f32>::epsilon()) {
                field_0x5c = 60.0f;
            } else {
                field_0x5c = (1.0f / t) * (60.0f - field_0x5c) + field_0x5c;
            }
        }
        break;
    }
    case 3:
        if (field_0x58 != 0) {
            fn_801CD930(&field_0x56, 0, 0x20);
        } else {
            fn_801CD930(&field_0x56, 0x100, 0x10);
        }
        field_0x54 += field_0x56;
        break;
    }
    if (field_0x44 >= 2 && field_0x44 < 4) {
        for (s32 i = 0; i < 16; i++) {
            s32 serial = field_0x0[i];
            if (serial >= 0) {
                ActorBase_804A5770* a = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (a != NULL) {
                    s16 ang = -(s16)(field_0x54 + i * 0xFFF);
                    f32 s = JMath::sincosTable_.sinShort(ang);
                    f32 c = JMath::sincosTable_.cosShort(ang);
                    a->mPos.x = field_0x48 + (0.0f * c - field_0x5c * s);
                    a->mPos.y = field_0x4c + (0.0f * s + field_0x5c * c);
                    a->mPos.z = field_0x50;
                    if (field_0x44 == 2) {
                        ((cls_804A4BB0*)a)->fn_801D7DB4(field_0x60);
                    }
                }
            }
        }
    }
}

// Remaining diff: load order and register choice around the normalize.
void cls_801D6CEC::fn_801D6D38(const JGeometry::TVec2f* dir) {
    JGeometry::TVec2f v(dir->x, dir->y);
    if (v.squared() <= 32.0f * JGeometry::TUtil<f32>::epsilon()) {
        v.x = 0.0f;
        v.y = -1.0f;
    } else {
        v.normalize();
    }
    v.x *= 20.0f;
    v.y *= 20.0f;
    fn_801E4AB0(&field_0x0, 0x3C, 0x2000, v.x);
    fn_801E4AB0(&field_0x14, 0x3C, 0x2000, v.y);
}

void cls_801D6CEC::fn_801D6D08() {
    fn_801E4AB0(&field_0x28, 0x1E, 0x400, -3.0f);
}

// FIXME(shotgun): 81.43 %, scheduling+regalloc
void cls_801D6CEC::fn_801D6CEC(JGeometry::TVec3f* out) {
    out->x = field_0x0.value;
    out->y = field_0x14.value;
    out->z = 0.0f;
}
