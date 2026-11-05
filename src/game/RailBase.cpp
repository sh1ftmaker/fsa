#include "game/GameAudio.h"
#include "game/RailBase.h"

cls_8049C770::~cls_8049C770() {}

void cls_8049C770::draw() {}

void cls_8049C770::update() {}
void cls_8049C770::init() {
    mPos.x += 8.0f;
    mPos.y += 8.0f;
}

bool cls_800FE3EC::fn_800FD548(u8 index) {
    if (index == 0xFF) {
        index = field_0x3;
    }
    if (field_0x1 != 0) {
        return (field_0x4[index]->field_0x90 & 0x8) != 0;
    }
    return (field_0x4[index]->field_0x90 & 0x10) != 0;
}

bool cls_800FE3EC::fn_800FD594(u8 index) {
    // Both arms feed one shared test; the original merges them the same way.
    u32 flag;
    if (index == 0xFF) {
        flag = field_0x4[field_0x3]->field_0x90 & 0x4;
    } else {
        flag = field_0x4[index]->field_0x90 & 0x4;
    }
    if (flag != 0) {
        return false;
    }
    return true;
}

void cls_800FE3EC::fn_800FDE38() {
    field_0x1 ^= 1;
    if (field_0x1 == 0) {
        field_0x3++;
    } else {
        field_0x3--;
    }
    if (field_0x3 == 0xFF) {
        field_0x3 = field_0x2 - 1;
        return;
    }
    if (field_0x3 == field_0x2) {
        field_0x3 = 0;
    }
}

u8 cls_800FE3EC::fn_800FE21C(u8 index) {
    if (index == 0xFF) {
        return field_0x4[field_0x3]->field_0x90 >> 24;
    }
    return field_0x4[index]->field_0x90 >> 24;
}

s32 cls_800FE3EC::fn_800FE25C(u8 index) {
    if (index == 0xFF) {
        return field_0x4[field_0x3]->field_0x4;
    }
    return field_0x4[index]->field_0x4;
}

void fn_800FE294(JGeometry::TVec3f* out, cls_800FE3EC* buffer, u8 index) {
    if (index == 0xFF) {
        *out = buffer->field_0x4[buffer->field_0x3]->mPos;
    } else {
        *out = buffer->field_0x4[index]->mPos;
    }
}
