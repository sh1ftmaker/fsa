#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/RotMatrix.h"
#include "game/act/act_lotus.h"

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

// Does one of player `i`'s 8 outline points lie inside the lotus's box? The first one inside is reported (fn_8022DF48).
static inline bool outlineInBox(Lotus_804AC330* self, int i) {
    for (int j = 0; j < 8; j++) {
        JGeometry::TVec3f point;
        fn_8022E088(i, j, &point);
        point -= self->mPos;
        point.z = 0.0f;
        f32 rad = JMath::atanTable_.atan2_(point.y, point.x);
        s16 angle = rad * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        f32 dist = JGeometry::TUtil<f32>::sqrt(point.x * point.x + point.y * point.y);
        point.z = 0.0f;
        point.y = dist * JMath::sincosTable_.sinShort(angle);
        point.x = dist * JMath::sincosTable_.cosShort(angle);
        if (contains(self->mBox, point)) {
            fn_8022DF48(i);
            return true;
        }
    }
    return false;
}

void Lotus_804AC330::init() {
    for (int i = 0; i < 4; i++) {
        field_0x25e[i] = 0;
    }
    field_0x244 = field_0x90 & 0xF;
    field_0x248 = (field_0x90 >> 4) & 0xF;
    field_0x250 = (field_0x90 >> 8) & 0xF;
    field_0x254 = field_0x250;
    field_0x258 = (field_0x90 >> 12) & 0xFF;
    field_0x24c = 0;
    field_0x25c = 0;
    field_0x262 = 0;
    field_0x234.z = 0.0f;
    field_0x234.y = 0.0f;
    field_0x234.x = 0.0f;
    field_0x240 = 1.0f;
    field_0x230 = 0;
    if (vfunc_0x3C() > 0) {
        field_0x230 = 2;
        field_0x240 = 0.0f;
    }
    switch (field_0x244) {
    case 0:
        mBox.set(-8.4f, -8.4f, 8.4f, 8.4f);
        break;
    case 1:
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        break;
    case 2:
        mBox.set(-18.0f, -18.0f, 18.0f, 18.0f);
        break;
    }
}

// FIXME: 97.17 % (missing/extra: 422 vs 421 instrs). Register allocation in the player loop copies i into r26 (1 extra mr), and contains() loads mBox.i.y/f.x/f.y per branch instead of upfront.
void Lotus_804AC330::update() {
    if (field_0x230 == 1) {
        mPos += field_0x3c;
    }
    if (vfunc_0x3C() > 0 && !vfunc_0x34()) {
        field_0x230 = 2;
    }
    switch (field_0x230) {
    case 0:
        if (field_0x248 != 0 && field_0x24c <= 0) {
            switch (field_0x254) {
            case 0:
                field_0x3c.set(0.5f * (f32)field_0x248, 0.0f, 0.0f);
                break;
            case 1:
                field_0x3c.set(0.5f * -(f32)field_0x248, 0.0f, 0.0f);
                break;
            case 2:
                field_0x3c.set(0.0f, 0.5f * (f32)field_0x248, 0.0f);
                break;
            case 3:
                field_0x3c.set(0.0f, 0.5f * -(f32)field_0x248, 0.0f);
                break;
            }
            field_0x24c = 1;
            field_0x230 = 1;
        }
        break;
    case 1:
        if ((field_0x254 == field_0x250 && PSVECDistance(&mPos, &(mSpawnPos + field_0x234)) >= 16.0f * (f32)field_0x258) ||
            (field_0x254 != field_0x250 && PSVECDistance(&mPos, &(mSpawnPos + field_0x234)) <= 0.0f)) {
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x254 ^= 1;
            field_0x24c = 60;
            field_0x230 = 0;
        }
        if (field_0x24c == 0) {
            field_0x24c = 16;
        }
        break;
    case 2:
        if (vfunc_0x34() == true) {
            if (field_0x240 < 1.0f) {
                field_0x240 += 0.02f;
                if (field_0x240 >= 1.0f) {
                    field_0x240 = 1.0f;
                    field_0x230 = 0;
                }
            }
        } else {
            if (field_0x240 > 0.0f) {
                field_0x240 -= 0.02f;
                if (field_0x240 <= 0.0f) {
                    field_0x240 = 0.0f;
                }
            }
        }
        return;
    }
    if (field_0x262 != 0) {
        mPos.y -= field_0x234.y;
        field_0x25c += 0x800;
        field_0x234.y = JMath::sincosTable_.sinShort(field_0x25c);
        mPos.y += field_0x234.y;
        if (field_0x25c == 0) {
            field_0x234.y = 0.0f;
            field_0x262 = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        bool touching;
        if (getPlayerLayer_8023DE58(i) != getField_0x4()) {
            touching = false;
        } else {
            touching = outlineInBox(this, i);
        }
        if (touching) {
            if (getPlayerPos_8023E724(i)->z <= 0.0f) {
                fn_8023DEDC(i, &(mPos - mPrevPos));
                for (int j = 0; j < 3; j++) {
                    fn_8022CB94(i);
                }
            }
        }
        if (getPlayerPos_8023E724(i)->z > 0.0f) {
            touching = false;
        }
        if (touching != field_0x25e[i]) {
            field_0x262 = 1;
        }
        field_0x25e[i] = touching;
    }
    if (field_0x24c > 0) {
        field_0x24c--;
    }
}

// FIXME: 98.22 % (regalloc-only). Float register allocation in setRotScaleMtx (f26/f28/f29/f31).
void Lotus_804AC330::draw() {
    if (field_0x240 <= 0.0f) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    PSMTXIdentity(mtx);
    if (field_0x244 == 0) {
        JGeometry::TVec3f rot(0.0f, 2.0f * (3.14f * (1.0f - field_0x240)), 0.0f);
        setRotScaleMtx(mtx, rot, 1.4f * field_0x240);
    } else {
        JGeometry::TVec3f rot(0.0f, 2.0f * (3.14f * (1.0f - field_0x240)), 0.0f);
        setRotScaleMtx(mtx, rot, field_0x240);
    }
    params.mMtx = mtx;
    params.field_0x20 = 0;
    params.field_0x21 = 1;
    params.field_0x1c = 0;
    params.field_0x1d = -3;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x244 + 0x172, &params, -1, 1.0f, 1.0f);
}

Lotus_804AC330::~Lotus_804AC330() {}
