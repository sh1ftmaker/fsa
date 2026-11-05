#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_somaria_block.h"

// Offsets (x, y, z) that init walks mPos along; the index is fn_80230CFC ^ 1.
static const f32 lbl_80471368[4][3] = {
    {0.5f, 0.0f, 0.0f}, {-0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f},
};

CaneOfSomariaBlock_804AA9F0::~CaneOfSomariaBlock_804AA9F0() {}

void CaneOfSomariaBlock_804AA9F0::fn_8030EA00() {
    mState = 7;
}

bool CaneOfSomariaBlock_804AA9F0::fn_8030EA0C() const {
    return mState < 6;
}

bool CaneOfSomariaBlock_804AA9F0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    // FIXME: 57.54 %. The original tests the tile code as a compare chain (cmpwi /
    // beq to one shared return 0 for 29 values, in this order). A switch becomes a
    // range / jump table, a || chain merges the values into ranges, and separate
    // ifs each return on their own (183 instructions).
    if (field_0x245 != 0) {
        return false;
    }
    u16 tile = fn_8022461C(pos, layer);
    u16 code = fn_80226850(pos, layer);
    if ((tile >= 0x10 && tile <= 0x17) || (code >= 0x140 && code <= 0x15F)) {
        field_0x245 = 1;
        return false;
    }
    switch (tile) {
    case 0:
    case 4:
    case 5:
    case 6:
    case 7:
    case 2:
    case 3:
    case 0xE:
    case 0x50:
    case 0xC:
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2A:
    case 0x2B:
    case 0x2C:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x20:
    case 0xD:
    case 0x3B:
    case 0x39:
    case 0x3A:
    case 0x4C:
    case 0x4D:
    case 0x4E:
    case 0x4F:
    case 0x24:
        return false;
    case 0x25:
        return getGameView_802D6A9C()->field_0x171 == 0;
    case 0x26:
        return getGameView_802D6A9C()->field_0x171 == 1;
    case 0x22:
        return getGameView_802D6A9C()->field_0x170 == 1;
    default:
        return true;
    }
}

void CaneOfSomariaBlock_804AA9F0::draw() {
    // FIXME: 99.86 %. Only the literal names differ: the 1.0f scales and the
    // "enemy_both2_sob.bin" string are @457 / @458 here, lbl_ in the original.
    if (mState > 7) {
        return;
    }
    SpriteParams_801F5AE0 params;
    if (field_0xc4 != 0) {
        params.field_0x20 = 0;
        params.field_0x1d = fn_8023CB24(field_0xc8) + 1;
        params.field_0x1c = fn_8023CA88(field_0xc8);
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x24c + 0x2FF, &params, 0, 1.0f, 1.0f);
    fn_803075AC(mSerial, field_0x4, &mPos, 0x2FF, false, 2, -1, field_0xc4, 0, 0);
}

void CaneOfSomariaBlock_804AA9F0::init() {
    // FIXME: 99.59 %. Same instructions; the sdata2 literals are @486-@492 here
    // (lbl_ in the original, a literal-split issue).
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mBox.set(-6.0f, -6.0f, 6.0f, 6.0f);
    field_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x1a4 = 0;
    field_0xb8 = 2;
    mState = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x240 = -1;
    field_0x245 = 0;
    field_0x246 = 0;
    field_0x247 = 0;
    field_0x248 = 0;
    field_0x249 = 0;
    field_0x23c = -1;
    field_0x244 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;

    s32 count = 0;
    while (fn_801F2618(NULL, false)) {
        s32 dir = fn_80230CFC(field_0x110) ^ 1;
        count++;
        mPos.x += 2.0f * lbl_80471368[dir][0];
        mPos.y += 2.0f * lbl_80471368[dir][1];
        if (count > 8) {
            break;
        }
    }
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
}
