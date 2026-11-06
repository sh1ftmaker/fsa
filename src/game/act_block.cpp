#include "game/GameAudio.h"
#include "game/act/act_block.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"

static const f32 lbl_8046C120[4][3] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

static const f32 lbl_8046C150[4][3] = {
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
};

static const f32 lbl_8046C180[4][3] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

static const f32 lbl_8046C1B0[4][3] = {
    {20.0f, 0.0f, 0.0f},
    {-20.0f, 0.0f, 0.0f},
    {0.0f, 20.0f, 0.0f},
    {0.0f, -20.0f, 0.0f},
};

static const f32 lbl_8046C1E0[4][3] = {
    {16.0f, 0.0f, 0.0f},
    {-16.0f, 0.0f, 0.0f},
    {0.0f, 16.0f, 0.0f},
    {0.0f, -16.0f, 0.0f},
};

static f32 lbl_804A79E0[4][3] = {
    {12.0f, 0.0f, 0.0f},
    {-12.0f, 0.0f, 0.0f},
    {0.0f, 12.0f, 0.0f},
    {0.0f, -12.0f, 0.0f},
};

void Block_804A7A10::init() {
    field_0x244 = 0;
    s32 type = field_0x90 & 3;
    if (type == 0) {
        mPos.x = (s32)mPos.x / 16 * 16 + 8;
        mPos.y = (s32)mPos.y / 16 * 16 + 8;
        u8 dir = (field_0x90 >> 2) & 3;
        field_0x3c.set(lbl_8046C120[dir][0], lbl_8046C120[dir][1], lbl_8046C120[dir][2]);
        JGeometry::TVec3f checkPos = field_0x3c;
        checkPos.normalize();
        checkPos.scale(16.0f);
        checkPos += mPos;
        if (fn_8022461C(&checkPos, field_0x4) == 0x50) {
            field_0x3c.normalize();
            field_0x3c.scale(1.0f);
        }
    } else {
        if (!((field_0x90 >> 9) & 1)) {
            if (fn_8028D4DC(&mPos, type, field_0x16c)) {
                field_0x244 = 1;
                vfunc_0x30(0xFF, true);
            }
        }
        field_0x3c.set(0.0f, 0.0f, 0.0f);
    }

    field_0x230 = mPos;
    field_0x248 = 0;
    field_0x24c = 1;

    type = field_0x90 & 3;
    if (type == 0) {
        mBox.set(-7.5f, -7.5f, 7.5f, 7.5f);
        field_0x80.set(-7.5f, -7.5f, 7.5f, 7.5f);
        field_0x1a4 = 1;
        field_0x24c = 0;
    } else if (type == 1) {
        mBox.set(-15.5f, -15.5f, 15.5f, 15.5f);
        field_0x80.set(-15.5f, -15.5f, 15.5f, 15.5f);
        field_0x1a4 = 2;
    } else if (type == 2) {
        mBox.set(-23.5f, -23.5f, 23.5f, 23.5f);
        field_0x80.set(-23.5f, -23.5f, 23.5f, 23.5f);
        field_0x1a4 = 3;
    } else if (type == 3) {
        mBox.set(-31.5f, -31.5f, 31.5f, 31.5f);
        field_0x80.set(-31.5f, -31.5f, 31.5f, 31.5f);
        field_0x1a4 = 4;
    }

    mPrevPos = mPos;
    JGeometry::TVec3f saved3c = field_0x3c;
    if (!fn_8028C518((field_0x90 >> 2) & 3)) {
        mAlive = false;
    } else {
        mPrevPos = mPos;
        if ((field_0x90 & 3) == 0) {
            field_0x3c.x = saved3c.x;
            field_0x3c.y = saved3c.y;
            field_0x3c.z = saved3c.z;
            gBgRewrite_8053AC98->fn_8029E9EC(&mPos, field_0x4, ((field_0x90 >> 4) & 7) + 0x21, 0, (field_0x90 >> 2) & 3);
            startSound_801F0E34(0x30004, 0);
        }
        field_0x23c = 0;
        field_0x240 = 0;
        field_0xb8 = 2;
        field_0x24d = 0;
        field_0x24e = 0;
        if ((field_0x90 & 3) != 0) {
            field_0x24e = (field_0x90 >> 10) & 1;
        }
    }
}

u8 fn_8028D4DC(JGeometry::TVec3f* pos, s32 index, s32 layer) {
    u8 viewVal = getGameView_802D6A9C()->fn_802D7544(layer);
    if (viewVal == 0) {
        return 0;
    }
    u8 dir = viewVal - 1;
    JGeometry::TVec3f offset;
    offset.x = (index + 1) * lbl_8046C150[dir][0] * 16.0f;
    offset.y = (index + 1) * lbl_8046C150[dir][1] * 16.0f;
    offset.z = 0.0f;
    *pos += offset;
    return 1;
}

bool Block_804A7A10::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if ((s32)fn_8022461C(pos, layer) == 0xD) {
        return false;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

// FIXME: 98.28%. scheduling+regalloc: same instructions modulo registers, different order
bool Block_804A7A10::fn_8028C518(u8 dir) {
    bool result = false;
    s32 kind = field_0x90 & 3;
    JGeometry::TVec3f origPos = mPos;

    if (kind == 3) {
        return true;
    }

    if (kind == 0) {
        field_0x3c.set(lbl_8046C1E0[dir][0], lbl_8046C1E0[dir][1], lbl_8046C1E0[dir][2]);
        mPrevPos += field_0x3c;
    }

    field_0x3c.set(lbl_8046C1B0[dir][0], lbl_8046C1B0[dir][1], lbl_8046C1B0[dir][2]);
    mPos += field_0x3c;

    JGeometry::TVec3f v18(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&v18, false)) {
        switch (dir) {
        case 0:
            if (v18.x >= 0.0f) {
                result = true;
            }
            break;
        case 1:
            if (v18.x <= 0.0f) {
                result = true;
            }
            break;
        case 2:
            if (v18.y >= 0.0f) {
                result = true;
            }
            break;
        case 3:
            if (v18.y <= 0.0f) {
                result = true;
            }
            break;
        }
    } else {
        result = true;
    }

    if (!result) {
        result = true;
        JGeometry::TBox2f box(mBox);
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;

        s32 serial;
        for (int i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                if (fn_8022CAF8(i) == 5) {
                    if (fn_8022F478(i)) {
                        continue;
                    }
                }
                serial = mSerial;
                if (fn_80238CB4(i) == serial) {
                    continue;
                }
                result = false;
            }
        }
    }

    mPos = origPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    return result;
}

// FIXME: 99.40%. regalloc-only: target->ours r30->r31, r31->r30
bool Block_804A7A10::fn_8028C364(u8 dir) {
    f32 max;
    f32 min;
    f32 center;
    if (dir <= 1) {
        max = mBox.f.y + mPos.y;
        min = mBox.i.y + mPos.y;
        center = mPos.x;
    } else {
        max = mBox.f.x + mPos.x;
        min = mBox.i.x + mPos.x;
        center = mPos.y;
    }

    int i;
    s32 serial;
    for (i = 0; i < 4; i++) {
        serial = mSerial;
        s32 pushing = fn_80238CB4(i);
        if (pushing != serial) {
            continue;
        }
        if (dir <= 1) {
            f32 y = getPlayerPos_8023E724(i)->y;
            if (y < min && max < y) {
                return false;
            }
            if (dir == 0) {
                if (getPlayerPos_8023E724(i)->x > center) {
                    return false;
                }
            } else {
                if (getPlayerPos_8023E724(i)->x < center) {
                    return false;
                }
            }
        } else {
            f32 x = getPlayerPos_8023E724(i)->x;
            if (x < min && max < x) {
                return false;
            }
            if (dir == 2) {
                if (getPlayerPos_8023E724(i)->y > center) {
                    return false;
                }
            } else {
                if (getPlayerPos_8023E724(i)->y < center) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Block_804A7A10::draw() {
    s32 val = 0;
    SpriteParams_801F5AE0 params;
    s32 kind = field_0x90 & 3;
    if (kind != 0) {
        params.field_0x1d = -1;
        params.field_0x1c = 0;
        drawSprite_801F06F0("enemy_both_sob.bin", kind + 4, &params, 0, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, kind + 4, 0, 0, -1);
    } else {
        s32 v = (field_0x90 >> 4) & 7;
        if (v != 0) {
            val = v + 3;
        }
        drawSprite_801F06F0("enemy_both_sob.bin", val + 0x45, NULL, 0, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, val + 0x45, 0, 0, -1);
    }
}

void Block_804A7A10::vfunc_0x24() {
    mPos.x = (s32)((s32)mPos.x + 4) / 8 * 8;
    mPos.y = (s32)((s32)mPos.y + 4) / 8 * 8;
    if ((field_0x90 & 3) == 0) {
        gBgRewrite_8053AC98->fn_8029E928(&mPos, field_0x4, 0x20);
    }
    mAlive = false;
}

Block_804A7A10::~Block_804A7A10() {}
