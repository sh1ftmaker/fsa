#include "game/GameAudio.h"
#include "game/cls_804A71A8.h"
#include "game/Map.h"
#include "game/ViewLayer.h"

// Four (x, y) corner offsets of +-4.
static const f32 lbl_8046BA68[4][2] = {
    {4.0f, 4.0f}, {-4.0f, 4.0f}, {4.0f, -4.0f}, {-4.0f, -4.0f},
};

// Scales v to unit length when it is longer than 32 * FLT_EPSILON.
static inline void normalize2D(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

// fn_802265FC reads only the x and y of its position, so the 8-byte
// TVec2f temporaries are passed as the TVec3f it declares.
static inline u16 probeTile(const JGeometry::TVec2f& p, s32 layer) {
    return fn_802265FC((const JGeometry::TVec3f*)&p, layer);
}

// probeTile at (x, y), through the caller's temporary `p`.
static inline u16 probeTileXY(JGeometry::TVec2f& p, f32 x, f32 y, s32 layer) {
    p.set(x, y);
    return probeTile(p, layer);
}

// FIXME: 92.27%. Stack slots differ: the second probe's corner (`other`) is at
// 0x10 where the original has 0x28, and the step temporaries lack the dead
// stores of mPos the original keeps (17 instructions fewer). The tile-decay
// tail is also laid out with different branches.
// Moves the player by its step, mPos - field_0x2c. A step of at least 5
// keeps its direction and is set to 5 long (the first two cases, both on
// passable tiles); a step of 6.5 to 20 keeps its direction at 6.5 unless the
// player is in the state fn_80240D64 tests (the third case). Then the
// field_0x420 offset is added: it is zeroed while mPos.z > 0 and otherwise
// decays under the tile code.
void Player_804A6F38::fn_802685D4() {
    field_0x3c1 = false;
    field_0x3c2 = false;
    if (!fn_8024BBB0() &&
        !(fn_8025F418() && field_0x1216 == 0 && field_0x1228 == 0 && fn_8022D75C(field_0xd70)) &&
        !fn_802413E8() && fn_80241E28(field_0x3bc, &field_0x2c))
    {
        JGeometry::TVec2f corner(mPos.x, field_0x2c.y);
        JGeometry::TVec2f other;
        if (probeTile(corner, field_0x3b4) == 1 &&
            probeTileXY(other, field_0x2c.x, mPos.y, field_0x3b4) == 1)
        {
            JGeometry::TVec2f step1(mPos.x, mPos.y);
            step1.set(mPos.x - field_0x2c.x, mPos.y - field_0x2c.y);
            if (step1.x * step1.x + step1.y * step1.y >= 25.0f) {
                normalize2D(step1);
                step1.x *= 5.0f;
                step1.y *= 5.0f;
            }
            mPos.x = field_0x2c.x + step1.x;
            mPos.y = field_0x2c.y + step1.y;
        } else {
            JGeometry::TVec2f here(mPos.x, mPos.y);
            if (probeTile(here, field_0x3b4) == 1) {
                JGeometry::TVec2f step2(mPos.x, mPos.y);
                step2.set(mPos.x - field_0x2c.x, mPos.y - field_0x2c.y);
                if (step2.x * step2.x + step2.y * step2.y >= 25.0f) {
                    normalize2D(step2);
                    step2.x *= 5.0f;
                    step2.y *= 5.0f;
                }
                mPos.x = field_0x2c.x + step2.x;
                mPos.y = field_0x2c.y + step2.y;
            } else {
                JGeometry::TVec2f step3(mPos.x, mPos.y);
                step3.set(mPos.x - field_0x2c.x, mPos.y - field_0x2c.y);
                f32 lenSq = step3.x * step3.x + step3.y * step3.y;
                if (lenSq >= 42.25f && lenSq <= 400.0f && !field_0xce8 && !fn_80240D64()) {
                    normalize2D(step3);
                    step3.x *= 6.5f;
                    step3.y *= 6.5f;
                    mPos.x = field_0x2c.x + step3.x;
                    mPos.y = field_0x2c.y + step3.y;
                }
            }
        }
    }

    field_0xbf0->fn_80268BC4();
    field_0x2c.set(mPos);
    field_0x3bc = field_0x3b4;
    field_0x3c0 = true;
    u16 tile = fn_80226850(&mPos, field_0x3b4);
    if (mPos.z > 0.0f) {
        field_0x420.x = 0.0f;
        field_0x420.y = 0.0f;
        field_0x420.z = 0.0f;
    } else {
        if (tile == 0xA4) {
            field_0x420.y -= 0.05f;
            if (field_0x420.y < -0.5f) {
                field_0x420.y = -0.5f;
            }
        } else if (tile == 0xA5) {
            field_0x420.y += 0.05f;
            if (field_0x420.y > 0.5f) {
                field_0x420.y = 0.5f;
            }
        } else {
            if (field_0x420.y > 0.025f) {
                field_0x420.y -= 0.025f;
            } else if (field_0x420.y < -0.025f) {
                field_0x420.y += 0.025f;
            }
        }
        if (tile == 0xA7) {
            field_0x420.x += 0.05f;
            if (field_0x420.x > 0.5f) {
                field_0x420.x = 0.5f;
            }
        } else if (tile == 0xA6) {
            field_0x420.x -= 0.05f;
            if (field_0x420.x < -0.5f) {
                field_0x420.x = -0.5f;
            }
        } else {
            if (field_0x420.x > 0.025f) {
                field_0x420.x -= 0.025f;
            } else if (field_0x420.x < -0.025f) {
                field_0x420.x += 0.025f;
            }
        }
    }
    mPos += field_0x420;
}

// Runs fn_802685D4 with mPos.z held at 0.0f unless the player is in
// state 5 and fn_80240808 is set; the z is kept only if fn_802685D4 left it.
void Player_804A6F38::fn_80268B3C() {
    if (fn_802486A0() == 5 && fn_80240808()) {
        fn_802685D4();
        return;
    }
    f32 saved = mPos.z;
    mPos.z = 0.0f;
    fn_802685D4();
    if (0.0f == mPos.z) {
        mPos.z = saved;
    }
}

void cls_804A71A8::fn_8026947C(JGeometry::TVec3f* pos) {
    *pos = field_0x4->mPos;
    if (field_0x4->fn_802486A0() == 2) {
        pos->x += lbl_8046BA68[field_0x4->fn_80248778()][0];
        if (viewLayerFlag(field_0x4->field_0x3b4) != 0) {
            pos->y -= lbl_8046BA68[field_0x4->fn_80248778()][1];
        } else {
            pos->y += lbl_8046BA68[field_0x4->fn_80248778()][1];
        }
    }
}

// Stores pos's x and y in field_0x324 when fn_80251AEC accepts it for this
// player: the arg is 0x38 on field_0xd64 == 0, else 0x13.
bool Player_804A6F38::fn_8026E5F8(const JGeometry::TVec2f* pos) {
    if (field_0xd64 == 0) {
        if (fn_80251AEC(0x38)) {
            field_0x324.x = pos->x;
            field_0x324.y = pos->y;
            return true;
        }
    } else if (fn_80251AEC(0x13)) {
        field_0x324.x = pos->x;
        field_0x324.y = pos->y;
        return true;
    }
    return false;
}

cls_804A71A8::~cls_804A71A8() {
    field_0x4 = NULL;
}
