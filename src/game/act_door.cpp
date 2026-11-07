#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_door.h"

// FIXME(shotgun): not written (size 0x22E0): update (0x802C59D8)

// FIXME(shotgun): 86.0 %, missing/extra + reloc-name
void Door_804A8D88::init() {
    field_0x258 = mPos.x;
    field_0x25c = mPos.y;
    field_0x260 = mPos.z;
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = (field_0x90 >> 7) & 0x1F;
    field_0x23c = (field_0x90 >> 12) & 0x1F;
    field_0x240 = (field_0x90 >> 17) & 1;
    field_0x24c = (field_0x90 >> 18) & 1;
    field_0x250 = 0;
    field_0x248 = -1;
    if (field_0x234 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 1;
    }
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    switch (field_0x234) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 8:
        switch (field_0x23c) {
        case 0:
            switch (field_0x238) {
            case 0:
                mPos.x += 12.0f;
                mPos.y += 4.0f;
                break;
            case 1:
            case 2:
                mPos.x += 12.0f;
                mPos.y += 12.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 12.0f;
                break;
            }
            break;
        case 1:
            switch (field_0x238) {
            case 0:
                mPos.x += 28.0f;
                mPos.y += 4.0f;
                break;
            case 1:
                mPos.x += 28.0f;
                mPos.y += 12.0f;
                break;
            case 2:
                mPos.x += 12.0f;
                mPos.y += 28.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 28.0f;
                break;
            }
            break;
        case 2:
            switch (field_0x238) {
            case 0:
                mPos.x += 44.0f;
                mPos.y += 4.0f;
                break;
            case 1:
                mPos.x += 44.0f;
                mPos.y += 12.0f;
                break;
            case 2:
                mPos.x += 12.0f;
                mPos.y += 44.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 44.0f;
                break;
            }
            break;
        }
        break;
    case 5:
        mBox.i.x = -9.0f;
        mBox.i.y = -9.0f;
        mBox.f.x = 9.0f;
        mBox.f.y = 9.0f;
        mPos.x += 8.0f;
        mPos.y += 8.0f;
        field_0x230 = 2;
        break;
    case 6:
        mBox.i.x = -12.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 12.0f;
        mBox.f.y = 12.0f;
        mPos.x += 16.0f;
        mPos.y += 16.0f;
        field_0x230 = 2;
        break;
    case 7:
        mBox.i.x = -9.0f;
        mBox.i.y = -9.0f;
        mBox.f.x = 9.0f;
        mBox.f.y = 9.0f;
        mPos.x += 16.0f;
        mPos.y += 8.0f;
        field_0x230 = 2;
        break;
    }
    field_0x264.x = mPos.x;
    field_0x264.y = mPos.y;
    field_0x26c.i.x = mBox.i.x;
    field_0x26c.i.y = mBox.i.y;
    field_0x26c.f.x = mBox.f.x;
    field_0x26c.f.y = mBox.f.y;
    field_0x26c.i.x += field_0x264.x;
    field_0x26c.i.y += field_0x264.y;
    field_0x26c.f.x += field_0x264.x;
    field_0x26c.f.y += field_0x264.y;
    if (field_0x23c == 0 || (field_0x23c == 2 && field_0x234 == 0)) {
        if (field_0x23c == 0 && field_0x238 < 2) {
            JGeometry::TVec2f v(field_0x258 + 16.0f, field_0x25c);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        } else if (field_0x23c == 2 && field_0x234 == 0) {
            JGeometry::TVec2f v(field_0x258, field_0x25c);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        } else {
            JGeometry::TVec2f v(field_0x258, field_0x25c + 16.0f);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        }
    } else {
        JGeometry::TVec2f v(field_0x258, field_0x25c + 16.0f);
        field_0x244 = fn_80226BC4(&v, field_0x4);
    }
    if ((field_0x234 >= 1 && field_0x234 <= 4) || field_0x234 == 8) {
        s32 layer = field_0x4;
        bool hit;
        if (layer < 8) {
            hit = getGameView_802D6A9C()->fn_802D49A8(layer, (u16)field_0x244);
        } else {
            hit = getGameView_802D6A9C()->fn_802D4ADC(layer, (u16)field_0x244);
        }
        if (hit == true) {
            field_0x230 = 3;
            update();
            field_0x230 = 5;
        }
    } else if (field_0x234 >= 5) {
        if (getGameView_802D6A9C()->fn_802D4ADC(field_0x4, (u16)field_0x244) == true) {
            field_0x230 = 5;
        }
    } else {
        update();
    }
}


void Door_804A8D88::fn_802C59B0(const JGeometry::TVec3f* delta) {
    field_0x258 += delta->x;
    field_0x25c += delta->y;
}

// Plain TVec3 setter that update calls everywhere.
void fn_802C7CB8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

void Door_804A8D88::draw() {}

Door_804A8D88::~Door_804A8D88() {}
