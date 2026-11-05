#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameState.h"
#include "game/cls_804A71A8.h"
#include "game/Map.h"
#include "game/ViewLayer.h"

// One step of 16 along each facing (0: +x, 1: -x, 2: +y, 3: -y).
static const Vec lbl_8046B998[4] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
};

// The scan step of fn_8026E68C for ledge tiles 0x10-0x17: 8 units toward
// -y, +y, -x, +x, then the diagonals.
static const f32 lbl_8046B9C8[8][2] = {
    {0.0f, -8.0f}, {0.0f, 8.0f}, {-8.0f, 0.0f}, {8.0f, 0.0f},
    {-8.0f, -8.0f}, {8.0f, -8.0f}, {-8.0f, 8.0f}, {8.0f, 8.0f},
};

// fn_80269574's push out of a blocked direction (the index of the
// per-direction flags): 0.5 away from each side, 0.3 per axis away from the
// diagonals.
static const f32 lbl_8046BA08[8][3] = {
    {-0.5f, 0.0f, 0.0f}, {0.5f, 0.0f, 0.0f}, {0.0f, -0.5f, 0.0f}, {0.0f, 0.5f, 0.0f},
    {-0.3f, -0.3f, 0.0f}, {0.3f, -0.3f, 0.0f}, {-0.3f, 0.3f, 0.0f}, {0.3f, 0.3f, 0.0f},
};

// Four (x, y) corner offsets of +-4.
static const f32 lbl_8046BA68[4][2] = {
    {4.0f, 4.0f}, {-4.0f, 4.0f}, {4.0f, -4.0f}, {-4.0f, -4.0f},
};

// Eight (x, y, z) offsets: +-6.999 along each axis, then the four
// diagonals at +-5 (fn_8026942C).
static const f32 lbl_8046BA88[8][3] = {
    {6.999f, 0.0f, 0.0f}, {-6.999f, 0.0f, 0.0f}, {0.0f, 6.999f, 0.0f}, {0.0f, -6.999f, 0.0f},
    {5.0f, 5.0f, 0.0f}, {-5.0f, 5.0f, 0.0f}, {5.0f, -5.0f, 0.0f}, {-5.0f, -5.0f, 0.0f},
};

// The same eight directions for fn_80268BC4's probes: entry i is direction
// i of the per-direction flag arrays (+x, -x, +y, -y, then the diagonals).
static const f32 lbl_8046BAE8[8][3] = {
    {6.999f, 0.0f, 0.0f}, {-6.999f, 0.0f, 0.0f}, {0.0f, 6.999f, 0.0f}, {0.0f, -6.999f, 0.0f},
    {5.0f, 5.0f, 0.0f}, {-5.0f, 5.0f, 0.0f}, {5.0f, -5.0f, 0.0f}, {-5.0f, -5.0f, 0.0f},
};

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

bool Player_804A6F38::fn_802684A4(const JGeometry::TVec3f* pos, s32 layer) {
    for (s32 i = 0; i < 0x50; i++) {
        if (field_0xdb0[i] >= 0 && layer == field_0x1030[i] && contains(field_0x5fc[i], *pos)) {
            field_0xef0[i] = field_0xdb0[i];
            return true;
        }
    }
    return false;
}

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

// The start of each frame (fn_80268BC4, fn_8026F710): last frame's
// field_0x48 moves to field_0x40, the owner's field_0xef0 entries are
// cleared and field_0xb4 moves to field_0xb5.
static inline void beginFrame(cls_804A71A8* p) {
    for (s32 i = 0; i < 8; i++) {
        p->field_0x40[i] = p->field_0x48[i];
        p->field_0x48[i] = 0;
        p->field_0x30[i] = 0;
    }
    // int, not s32: an s32 counter adds a bound check the original lacks.
    for (int i = 0; i < 0x50; i++) {
        p->field_0x4->field_0xef0[i] = -1;
    }
    p->field_0xb5 = p->field_0xb4;
    p->field_0xb4 = 0;
    p->field_0xb6 = 0;
    p->field_0xb7 = 0;
    p->field_0xb8 = 0;
    p->field_0xb9 = 0;
    p->field_0xba = 0;
    p->field_0xac = 0;
}

// The start of each probe pass (fn_80268BC4, fn_8026F710).
static inline void beginPass(cls_804A71A8* p) {
    p->field_0x9a = 0;
    p->field_0x1c = 0;
    for (s32 i = 0; i < 8; i++) {
        p->field_0x20[i] = 0;
        p->field_0x28[i] = 0;
        p->field_0x38[i] = 0;
        p->field_0x50[i] = 0;
        p->field_0x58[i] = 0;
        p->field_0x60[i] = 0;
        p->field_0x68[i] = 0;
        p->field_0x70[i] = 0;
        p->field_0x78[i] = 0;
        p->field_0x80[i] = 0;
        p->field_0x88[i] = 0;
    }
}

// FIXME: 99.13. The end's TVec2f `moved` lives in registers; the original
// also stores its x, y before and after the subtraction to a stack slot at
// 0x8 (4 dead stores), as in fn_802685D4.
void cls_804A71A8::fn_80268BC4() {
    beginFrame(this);
    f32 startX = field_0x4->mPos.x;
    f32 startY = field_0x4->mPos.y;
    if (!field_0x4->fn_80240B84() && !field_0x4->fn_8025F448()) {
        field_0xb0 = 0;
        do {
            beginPass(this);
            field_0xb0++;
            for (s32 i = 0; i < 8; i++) {
                field_0x8.set(lbl_8046BAE8[i][0], lbl_8046BAE8[i][1], lbl_8046BAE8[i][2]);
                field_0x1c = i;
                field_0x8 += field_0x4->mPos;
                u16 tile = fn_8022461C(&field_0x8, field_0x4->field_0x3b4);
                if (field_0x4->fn_8025F590() && field_0x4->field_0x3b4 == 8 &&
                    getPlayerLayer_8023DE58(field_0x4->field_0xd70) < 8)
                {
                    // Probe the same spot on the layer of the player getPlayerLayer_8023DE58
                    // names, moved from this layer's view box to that one's.
                    JGeometry::TVec3f saved = field_0x8;
                    field_0x8.x -= layerBox(field_0x4->field_0x3b4)->i.x;
                    field_0x8.y -= layerBox(field_0x4->field_0x3b4)->i.y;
                    field_0x8.x += layerBox(getPlayerLayer_8023DE58(field_0x4->field_0xd70))->i.x;
                    field_0x8.y += layerBox(getPlayerLayer_8023DE58(field_0x4->field_0xd70))->i.y;
                    if (field_0x4->fn_802684A4(&field_0x8, getPlayerLayer_8023DE58(field_0x4->field_0xd70))) {
                        field_0x28[field_0x1c] = 1;
                    } else {
                        fn_8026B81C(fn_8022461C(&field_0x8, getPlayerLayer_8023DE58(field_0x4->field_0xd70)));
                    }
                    field_0x8 = saved;
                }
                if (field_0x4->fn_802684A4(&field_0x8, field_0x4->field_0x3b4)) {
                    field_0x28[field_0x1c] = 1;
                } else if (field_0x4->fn_8026FC20() <= 0 && !getGameView_802D6A9C()->fn_802DCD0C() &&
                           !contains(*layerBox(field_0x4->field_0x3b4), field_0x8))
                {
                    field_0x38[field_0x1c] = 1;
                } else {
                    fn_8026C350(tile);
                }
            }
            fn_80269574();
            if (field_0xb0 <= 1) {
                fn_8026E1E0();
            }
        } while (field_0x9a == 1 && field_0xb0 < 100);

        if (!isField98Or99Set()) {
            field_0x4->field_0xc90.set(1.0f, 1.0f, 1.0f);
        } else if (gGameRoot_8053AAF8->field_0x96 == 0) {
            field_0x99 = field_0x98;
            field_0x98 = 0;
        }
        JGeometry::TVec2f moved(field_0x4->mPos.x, field_0x4->mPos.y);
        moved.set(moved.x - startX, moved.y - startY);
        field_0x14.x = moved.x;
        field_0x14.y = moved.y;
    }
}

void cls_804A71A8::fn_80269420() {
    field_0x98 = 1;
}

void cls_804A71A8::fn_8026942C(s32 index, JGeometry::TVec3f* out) {
    out->set(lbl_8046BA88[index][0], lbl_8046BA88[index][1], lbl_8046BA88[index][2]);
    *out += field_0x4->mPos;
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

// Whether flags[i] != 0 for some direction i.
static inline bool anySet(const u8* flags) {
    for (s32 i = 0; i < 8; i++) {
        if (flags[i] != 0) {
            return true;
        }
    }
    return false;
}

// Whether flags[i] != 0 for every direction i.
static inline bool allSet(const u8* flags) {
    for (s32 i = 0; i < 8; i++) {
        if (flags[i] == 0) {
            return false;
        }
    }
    return true;
}

// fn_80269574's sections, each inlined once.

// The directions of field_0x50 become blocked unless one already is or all
// of them are marked.
static inline void blockMarked50(cls_804A71A8* p) {
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x38[i] == 1) {
            return;
        }
    }
    s32 n;
    for (n = 0; n < 8; n++) {
        if (p->field_0x50[n] == 0) {
            break;
        }
    }
    if (n == 8) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x50[i] == 1) {
            p->field_0x38[i] = 1;
        }
    }
}

// A follower (fn_80240808, not in group mode 4) closed in on every side
// (field_0x68, 0x88 or 0x38) asks fn_8022BC38 to move to its leader.
static inline void followLeader(cls_804A71A8* p) {
    if (p->field_0x4->fn_80240F28() && p->field_0x4->fn_802415CC() >= 2) {
        return;
    }
    if (viewLayerFlag(p->field_0x4->field_0x3b4) || p->isField98Or99Set() == true) {
        return;
    }
    if (p->field_0x4->field_0x1216 == 1 || p->field_0x4->field_0x1228 > 0) {
        return;
    }
    if (p->field_0x4->fn_802486A0() == 5 && p->field_0x4->fn_80240670() &&
        fn_80234950(fn_8022F340(p->field_0x4->field_0x4)))
    {
        return;
    }
    if (!p->field_0x4->fn_80240808() || p->field_0x4->fn_802486A0() == 4) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x68[i] == 0 && p->field_0x88[i] == 0 && p->field_0x38[i] == 0) {
            return;
        }
    }
    if (fn_8022BC38(p->field_0x4->fn_80240644(), p->field_0x4->field_0x3b4, &p->field_0x4->mPos)) {
        fn_802381B8(p->field_0x4->fn_80240644());
        fn_802348C4(p->field_0x4->fn_80240644(), p->field_0x4->field_0x3b4, &p->field_0x4->mPos);
    }
}

// field_0xb7: the floor slows the pushes (tile 0x3EB / 0x34 just above on
// a flagged layer, else tile 0x50 under the owner).
static inline void checkSlowFloor(cls_804A71A8* p) {
    if (viewLayerFlag(p->field_0x4->field_0x3b4)) {
        JGeometry::TVec3f above(lbl_8046BA88[2][0], lbl_8046BA88[2][1], lbl_8046BA88[2][2]);
        above += p->field_0x4->mPos;
        above.z += 2.0f;
        u16 tile = fn_80226850(&above, p->field_0x4->field_0x3b4);
        if (tile == 0x3EB || tile == 0x34) {
            p->field_0xb7 = 1;
        }
    } else if (fn_8022461C(&p->field_0x4->mPos, p->field_0x4->field_0x3b4) == 0x50) {
        p->field_0xb7 = 1;
    }
}

// Closed in on all sides by field_0x78 / 0x38: request state 0x4B (flagged
// layer) or 0x4A.
static inline void request4A(cls_804A71A8* p) {
    if (p->field_0x4->fn_80240F28() && p->field_0x4->fn_802415CC() >= 2) {
        return;
    }
    if (p->field_0x4->fn_802486A0() == 5 && p->field_0x4->fn_80240670() &&
        fn_80234950(fn_8022F340(p->field_0x4->field_0x4)))
    {
        return;
    }
    if (p->countClosed() >= 6 || p->field_0x4->fn_80240B00()) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x78[i] == 0 && p->field_0x38[i] == 0) {
            return;
        }
        if (p->isField98Or99Set() == true) {
            return;
        }
    }
    Player_804A6F38* owner = p->field_0x4;
    if (viewLayerFlag(owner->field_0x3b4)) {
        owner->fn_80251AEC(0x4B);
    } else {
        owner->fn_80251AEC(0x4A);
    }
}

// Closed in on all sides by field_0x88 / 0x38: screen change 0xD
// (fn_802DEB58) or 0xC to field_0xd1c.
static inline void leaveBy88(cls_804A71A8* p) {
    if (p->field_0x4->fn_80240F28() && p->field_0x4->fn_802415CC() >= 2) {
        return;
    }
    if (p->field_0x4->fn_802486A0() == 5 && p->field_0x4->fn_80240670() &&
        fn_80234950(fn_8022F340(p->field_0x4->field_0x4)))
    {
        return;
    }
    if (p->countClosed() >= 6) {
        return;
    }
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x88[i] == 0 && p->field_0x38[i] == 0) {
            return;
        }
        if (p->isField98Or99Set() == true) {
            return;
        }
    }
    if (fn_802DEB58()) {
        p->field_0x4->fn_80290070(p->field_0x4->field_0x3b4, &p->field_0x4->mPos, &p->field_0x4->field_0xd1c, 0xD);
    } else {
        p->field_0x4->fn_80290070(p->field_0x4->field_0x3b4, &p->field_0x4->mPos, &p->field_0x4->field_0xd1c, 0xC);
    }
}

// Advances the frames-shut-in counter; false when it was reset.
static inline bool advanceCounter(cls_804A71A8* p) {
    if (p->field_0xbc > 0) {
        p->field_0xbc++;
        return true;
    }
    if (p->field_0x4->fn_80240F28() && p->field_0x4->fn_802415CC() >= 2) {
        p->field_0xbc = 0;
        return false;
    }
    if (p->field_0x4->field_0x1216 == 1 || p->field_0x4->field_0x1228 > 0) {
        p->field_0xbc = 0;
        return false;
    }
    if (p->field_0x4->fn_802486A0() == 5 && p->field_0x4->fn_80240670() &&
        fn_80234950(fn_8022F340(p->field_0x4->field_0x4)))
    {
        p->field_0xbc = 0;
        return false;
    }
    if (viewLayerFlag(p->field_0x4->field_0x3b4)) {
        for (s32 i = 0; i < 8; i++) {
            if (p->field_0x68[i] == 1 && p->field_0x38[i] == 0) {
                p->field_0xbc = 3;
                return true;
            }
        }
        p->field_0xbc = 0;
        return false;
    }
    for (s32 i = 0; i < 8; i++) {
        if (p->field_0x68[i] == 0 && p->field_0x38[i] == 0) {
            p->field_0xbc = 0;
            return false;
        }
    }
    p->field_0xbc++;
    return true;
}

// FIXME: 96.27. Every call, field, constant and branch target is in place;
// ours is 40 instructions longer (2258 vs 2218). Where the original branches
// straight out of an early-exit test, this source materialises a value:
// the loop index (li n / cmpwi n, 8) after the break-out check loops
// (field_0x50, field_0x38 / 0x58 / 0x78, field_0x60 and the snap's
// field_0x68 / 0x88 / 0x38), and the bools advanceCounter returns and
// `nearWarp` (li 1 / clrlwi.), where the original jumps from each counter
// case to the WARP search and from the near-'WARP' case past the snap. The
// snap's mPos x test (r4 kept from the field_0xb0 test), the type-4 'WARP'
// turn test (field_0x4->field_0x4 loaded before fn_80248778), the 0xB7 store
// (one shared stb in the original) and the register numbers differ too.
// The state machine on field_0xbc (frames shut in), the 'WARP' actors,
// then the pushes and the owner's respawn point field_0xd1c.
void cls_804A71A8::fn_80269574() {
    blockMarked50(this);
    followLeader(this);

    bool nearWarp = false;
    if (isField98Or99Set() == true) {
        field_0xbc = 0;
    } else if (countClosed() < 6) {
        if (advanceCounter(this)) {
            if (field_0x4->fn_802486A0() == 4 && field_0x4->fn_80240888() != field_0x4->fn_80248778()) {
                field_0x4->fn_80290070(field_0x4->field_0x3b4, &field_0x4->mPos, &field_0x4->field_0xd1c, 9);
                field_0xbc = 0;
                return;
            }

            // A 'WARP' actor on the owner's layer: type 4 is near (< 7), type
            // 7 sends to a cell of another layer, type 8 to the same place on
            // another layer.
            s32 serial = -1;
            for (;;) {
                serial = getActorManager_801F856C()->fn_801F78F4('WARP', serial);
                if (serial < 0) {
                    break;
                }
                ActorBase_804A5770* warp = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (warp == NULL || field_0x4->field_0x3b4 != warp->field_0x4) {
                    continue;
                }
                u32 type = warp->field_0x90;
                if ((type & 0xFF) == 4) {
                    if (fn_8022F514(field_0x4->field_0x4)) {
                        continue;
                    }
                    if (!fn_8022F514(field_0x4->field_0x4) && fn_8022CAF8(field_0x4->field_0x4) == 4 &&
                        fn_80234004(field_0x4->field_0x4) != field_0x4->fn_80248778())
                    {
                        continue;
                    }
                    JGeometry::TVec2f d(field_0x4->mPos.x - warp->mPos.x, field_0x4->mPos.y - warp->mPos.y);
                    if (JGeometry::TUtil<f32>::sqrt(square(d.x) + square(d.y)) < 7.0f) {
                        field_0xbc = 0;
                        nearWarp = true;
                        break;
                    }
                } else if ((type & 0xFF) == 7) {
                    s32 layer = (type >> 24) & 7;
                    f32 x;
                    f32 y;
                    if (layer == 0) {
                        GameView_804A9950* view = getGameView_802D6A9C();
                        x = view->mVisibleBox.i.x;
                        y = view->mVisibleBox.i.y;
                    } else {
                        x = y = 0.0f;
                        layer += 7;
                    }
                    if (layer >= 8 && gGameRoot_8053AAF8->field_0x7e && !field_0x4->fn_80247534()) {
                        if (field_0x4->fn_80290070(0, &field_0x4->mPos, &field_0x4->field_0xd1c, 9)) {
                            field_0xbc = 0;
                            return;
                        }
                        continue;
                    }
                    JGeometry::TVec3f dest;
                    dest.x = 8.0f + (((type >> 16) & 0x3F) * 8 + x);
                    dest.y = 8.0f + (((type >> 8) & 0x3F) * 8 + y);
                    dest.z = 0.0f;
                    if (field_0x4->fn_80290070(layer, &field_0x4->mPos, &dest, 7)) {
                        field_0xbc = 0;
                        return;
                    }
                } else if ((type & 0xFF) == 8) {
                    JGeometry::TVec3f pos(field_0x4->mPos);
                    s32 layer = (type >> 24) & 7;
                    if (layer != 0) {
                        layer += 7;
                    }
                    pos.x -= layerBox(field_0x4->field_0x3b4)->i.x;
                    pos.y -= layerBox(field_0x4->field_0x3b4)->i.y;
                    pos.x += layerBox(layer)->i.x;
                    pos.y += layerBox(layer)->i.y;
                    if (layer >= 8 && gGameRoot_8053AAF8->field_0x7e && !field_0x4->fn_80247534()) {
                        if (field_0x4->fn_80290070(0, &field_0x4->mPos, &field_0x4->field_0xd1c, 9)) {
                            field_0xbc = 0;
                            return;
                        }
                        continue;
                    }
                    if (field_0x4->fn_80290070(layer, &field_0x4->mPos, &pos, 8)) {
                        field_0xbc = 0;
                        return;
                    }
                }
            }

            if (!nearWarp) {
                if (field_0xbc >= 2) {
                    field_0x4->field_0x457 = 1;
                }
                if (field_0xbc >= 3) {
                    field_0x4->fn_80290070(field_0x4->field_0x3b4, &field_0x4->mPos, &field_0x4->field_0xd1c, 9);
                    field_0xbc = 0;
                    return;
                }
            }
        }
    }

    if (!nearWarp && !field_0x4->fn_80240F28() && !viewLayerFlag(field_0x4->field_0x3b4) &&
        isField98Or99Set() != true && field_0x4->field_0x1216 != 1 && field_0x4->field_0x1228 <= 0 &&
        !(field_0x4->fn_802486A0() == 5 && field_0x4->fn_80240670() &&
          fn_80234950(fn_8022F340(field_0x4->field_0x4))))
    {
        if (field_0x4->fn_80240808() && field_0x4->fn_802486A0() != 4) {
            s32 n;
            for (n = 0; n < 8; n++) {
                if (field_0x68[n] == 0 && field_0x88[n] == 0 && field_0x38[n] == 0) {
                    break;
                }
            }
            if (n == 8) {
                fn_8022BC38(field_0x4->fn_80240644(), field_0x4->field_0x3b4, &field_0x4->mPos);
            }
        } else if (field_0xb0 <= 1) {
            if ((field_0x88[0] == 1 || field_0x68[0] == 1) && field_0x88[1] == 0 && field_0x68[1] == 0) {
                s32 x = field_0x4->mPos.x;
                if (!(x & 8)) {
                    field_0x4->mPos.x = x / 16 * 16 + 8;
                    field_0x4->field_0x3c1 = true;
                }
            }
            if ((field_0x88[1] == 1 || field_0x68[1] == 1) && field_0x88[0] == 0 && field_0x68[0] == 0) {
                s32 x = field_0x4->mPos.x;
                if (x & 8) {
                    field_0x4->mPos.x = x / 16 * 16 + 8;
                    field_0x4->field_0x3c1 = true;
                }
            }
            if ((field_0x88[2] == 1 || field_0x68[2] == 1) && field_0x88[3] == 0 && field_0x68[3] == 0) {
                s32 y = field_0x4->mPos.y;
                if (!(y & 8)) {
                    field_0x4->mPos.y = y / 16 * 16 + 8;
                    field_0x4->field_0x3c2 = true;
                }
            }
            if ((field_0x88[3] == 1 || field_0x68[3] == 1) && field_0x88[2] == 0 && field_0x68[2] == 0) {
                s32 y = field_0x4->mPos.y;
                if (y & 8) {
                    field_0x4->mPos.y = y / 16 * 16 + 8;
                    field_0x4->field_0x3c2 = true;
                }
            }
        }
    }
    checkSlowFloor(this);
    request4A(this);
    leaveBy88(this);
    if (viewLayerFlag(field_0x4->field_0x3b4) && !closedBy78()) {
        bool below;
        if (field_0x4->fn_80240808() || field_0x4->fn_80240D50()) {
            below = closedAt(2);
        } else {
            below = closedAt(2) || (closedAt(4) && !closedAt(0)) || (closedAt(5) && !closedAt(1));
        }
        if (below) {
            field_0xb4 = 0;
            field_0xb6 = 0;
        } else {
            s32 n;
            for (n = 0; n < 8; n++) {
                if (field_0x38[n] == 0 && field_0x58[n] == 0 && field_0x78[n] == 0) {
                    break;
                }
            }
            if (n == 8 &&
                (field_0x4->fn_802486A0() == 4 || !field_0x4->fn_80240808() || field_0x4->fn_80270A8C()))
            {
                field_0xb4 = 1;
                field_0xb6 = 1;
                if (!(field_0x4->fn_80240F28() && field_0x4->fn_802415CC() >= 2 &&
                      (field_0x4->field_0x3dc == 0 || field_0x4->field_0x3dc == 1)) &&
                    !field_0x4->fn_80240B34() && !field_0x4->fn_80240D64() && !field_0x4->fn_80240860() &&
                    !(field_0x4->fn_80270A8C() && field_0x4->field_0xd08 >= 0) && !field_0x4->fn_8024BBB0())
                {
                    field_0x4->mPos.y += 4.0f;
                }
                field_0x9a = 1;
            }
        }
    }
    if (field_0xba == 1) {
        field_0x4->fn_80290070(field_0x4->field_0x3b4, &field_0x4->mPos, &field_0x4->field_0xcbc, 0xD);
    }
    s32 n60;
    for (n60 = 0; n60 < 8; n60++) {
        if (field_0x60[n60] == 1) {
            break;
        }
    }
    if (n60 < 8) {
        if (!(field_0x4->fn_80240F28() && field_0x4->fn_802415CC() >= 2) && !field_0x4->fn_80240B34() &&
            !field_0x4->fn_80240D64() && !field_0x4->fn_80240860())
        {
            field_0x4->mPos.y += 0.125f;
            field_0x4->fn_8024F0E8(0x20045);
        }
        field_0xb9 = 1;
        field_0x9a = 1;
    }
    if (field_0x4->fn_8026FC20() <= 0) {
        // Unless its four sides are all closed, the owner's boxes
        // (field_0x28) block their directions.
        if (!((field_0x28[0] != 0 || field_0x38[0] != 0) && (field_0x28[1] != 0 || field_0x38[1] != 0) &&
              (field_0x28[2] != 0 || field_0x38[2] != 0) && (field_0x28[3] != 0 || field_0x38[3] != 0)))
        {
            for (s32 i = 0; i < 8; i++) {
                if (field_0x28[i] == 1) {
                    field_0x30[i] = 1;
                    field_0x38[i] = 1;
                }
            }
        }
        if (field_0xb0 <= 1 && countClosed() >= 6) {
            if (field_0xa8 < 60) {
                field_0xa8++;
            }
        } else {
            field_0xa8 = 0;
        }
        if ((field_0x4->fn_80241230() && countClosed() >= 7) ||
            (!field_0x4->fn_80241230() && allClosed()) || field_0xa8 >= 60)
        {
            field_0x4->fn_80290070(field_0x4->field_0x3b4, &field_0x4->mPos, &field_0x4->field_0xd1c, 0x22);
        } else {
            for (s32 i = 0; i < 8; i++) {
                if (field_0x38[i] != 0) {
                    field_0x48[i] = 1;
                    field_0x9a = 1;
                    JGeometry::TVec3f push(lbl_8046BA08[i][0], lbl_8046BA08[i][1], lbl_8046BA08[i][2]);
                    if (field_0xb7) {
                        push.scale(0.25f);
                    }
                    if (i >= 4) {
                        if (i == 4) {
                            if (viewLayerFlag(field_0x4->field_0x3b4) && field_0x38[2] == 1) {
                                push.x = 0.0f;
                            } else if (field_0x38[0] == 1 || field_0x38[2] == 1) {
                                push.scale(0.25f);
                            }
                        } else if (i == 5) {
                            if (viewLayerFlag(field_0x4->field_0x3b4) && field_0x38[2] == 1) {
                                push.x = 0.0f;
                            } else if (field_0x38[1] == 1 || field_0x38[2] == 1) {
                                push.scale(0.25f);
                            }
                        } else if (i == 6) {
                            if (field_0x38[0] == 1 || field_0x38[3] == 1) {
                                push.scale(0.25f);
                            }
                        } else if (i == 7) {
                            if (field_0x38[1] == 1 || field_0x38[3] == 1) {
                                push.scale(0.25f);
                            }
                        }
                    }
                    field_0x4->mPos += push;
                }
            }
        }
    }

    if (!field_0x4->fn_80241230() || field_0x4->mPos.z <= 0.0f) {
        if (allSet(field_0x20) && !fn_802265DC(&field_0x4->mPos, field_0x4->field_0x3b4, true)) {
            field_0x4->field_0xd1c.set(field_0x4->mPos);
            field_0x4->field_0xd1c.z = 0.0f;
        } else if (!viewLayerFlag(field_0x4->field_0x3b4) && field_0x48[2] == 1 && field_0x48[3] != 1 &&
                   !pushedAlongX() && !anySet(field_0x30))
        {
            // Pushed toward -y only, with passable tiles 8 below: remember the spot.
            JGeometry::TVec2f below;
            if (probeTileXY(below, field_0x4->mPos.x, 8.0f + field_0x4->mPos.y, field_0x4->field_0x3b4) == 1) {
                JGeometry::TVec2f left;
                if (probeTileXY(left, field_0x4->mPos.x - 7.0f, 8.0f + field_0x4->mPos.y, field_0x4->field_0x3b4) == 1) {
                    JGeometry::TVec2f right;
                    if (probeTileXY(right, 7.0f + field_0x4->mPos.x, 8.0f + field_0x4->mPos.y,
                                    field_0x4->field_0x3b4) == 1)
                    {
                        field_0x4->field_0xd1c.set(field_0x4->mPos);
                        field_0x4->field_0xd1c.z = 0.0f;
                    }
                }
            }
        }
    }
}

// Counts one more frame of pushing into a blocked side (at most 20).
static inline void countPush(cls_804A71A8* p) {
    p->field_0xa0++;
    if (p->field_0xa0 > 20) {
        p->field_0xa0 = 20;
    }
    p->field_0xa4 = 0;
}

void cls_804A71A8::fn_8026E1E0() {
    if (field_0x4->fn_80240670() && (field_0x4->fn_802412A4() || field_0x4->field_0x1218 > 0)) {
        field_0xa0 = 0;
        field_0xa4 = 0;
    } else if (fn_8023EAC4(field_0x4->field_0x4)) {
        field_0xa0 = 0;
        field_0xa4 = 0;
    } else if (field_0x4->mPos.z > 0.0f) {
        field_0xa0 = 0;
        field_0xa4 = 0;
    } else if (field_0x38[0] == 1 && field_0x4->field_0x3dc == 0 && field_0x4->fn_80247330() > 0.0f) {
        countPush(this);
    } else if (field_0x38[1] == 1 && field_0x4->field_0x3dc == 1 && field_0x4->fn_80247330() < 0.0f) {
        countPush(this);
    } else if (field_0x38[2] == 1 && field_0x4->field_0x3dc == 2 && field_0x4->fn_8024712C() < 0.0f) {
        countPush(this);
    } else if (field_0x38[3] == 1 && field_0x4->field_0x3dc == 3 && field_0x4->fn_8024712C() > 0.0f) {
        countPush(this);
    } else if (field_0x4->fn_80250090()) {
        field_0xa0 = 20;
        field_0xa4 = 0;
    } else if (!field_0x4->fn_80241478()) {
        field_0xa4++;
        if (field_0xa4 >= 5) {
            field_0xa4 = 5;
            field_0xa0 = 0;
            field_0x4->field_0xd94 = -1;
            field_0x4->field_0xd9c = -1;
            field_0x4->field_0xd98 = 0;
        }
    }
}

void cls_804A71A8::fn_8026E464() {
    beginFrame(this);
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

// Whether the owner pushes toward the probed direction (fn_80250D30, the
// probe is its facing, fn_8024FF84() < 0) with a free tile (0, 2 or 0x50)
// one step of 16 beyond the probe.
static inline bool canPush(cls_804A71A8* p) {
    if (p->field_0x4->fn_80250D30() && p->field_0x1c == p->field_0x4->field_0x3dc &&
        p->field_0x4->fn_8024FF84() < 0)
    {
        JGeometry::TVec3f ahead = p->field_0x8;
        ahead += JGeometry::TVec3f(lbl_8046B998[p->field_0x4->field_0x3dc]);
        u16 tile = fn_8022461C(&ahead, p->field_0x4->field_0x3b4);
        if (tile == 0 || tile == 2 || tile == 0x50) {
            return true;
        }
        return false;
    }
    return false;
}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec2f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

// FIXME: 99.97. Code identical; the literals 16.0f (lbl_80540280) and the
// int-to-float constant (lbl_80540270) stay @NNN until the unit's literal
// pool is complete (fn_80269574 and fn_8026C350 come first).
void cls_804A71A8::fn_8026E68C(u16 tile) {
    fn_802381B8(field_0x4->field_0x4);
    if (field_0x4->fn_80241230()) {
        field_0x38[field_0x1c] = 1;
    } else if (field_0x4->fn_80240F28() && field_0x4->fn_802415CC() >= 2) {
        field_0x38[field_0x1c] = 1;
    } else if (!field_0x4->fn_80240670() &&
               (fn_8026EB1C(tile) || field_0x4->fn_8024F5E4() || field_0x4->fn_80240F28() ||
                (gGameRoot_8053AAF8->field_0x7f && lbl_80529DEC.field_0x118 == 2 && lbl_80529DEC.field_0x11c == 1)))
    {
        JGeometry::TVec2f pos;
        pos.x = field_0x8.x;
        pos.y = field_0x8.y;
        if (tile == 0x12 || tile == 0x13) {
            pos.y += 16.0f;
        }
        bool found = false;
        do {
            pos.x += lbl_8046B9C8[tile - 0x10][0];
            pos.y += lbl_8046B9C8[tile - 0x10][1];
            if (!contains(*layerBox(field_0x4->field_0x3b4), pos)) {
                break;
            }
            u16 under = probeTile(pos, field_0x4->field_0x3b4);
            if (under == 0 || under == 3 || under == 0xD || under == 0x39 || under == 0x3A) {
                found = true;
            }
        } while (!found);
        if (!found) {
            // Tiles 0x12 / 0x13 scan again from the probe itself.
            if (tile == 0x12 || tile == 0x13) {
                pos.x = field_0x8.x;
                pos.y = field_0x8.y;
                do {
                    pos.x += lbl_8046B9C8[tile - 0x10][0];
                    pos.y += lbl_8046B9C8[tile - 0x10][1];
                    if (!contains(*layerBox(field_0x4->field_0x3b4), pos)) {
                        return;
                    }
                    u16 under = probeTile(pos, field_0x4->field_0x3b4);
                    if (under == 0 || under == 3 || under == 0xD || under == 0x39 || under == 0x3A) {
                        found = true;
                    }
                } while (!found);
            } else {
                return;
            }
        }
        // The centre of the 16-unit cell.
        pos.x = ((s32)pos.x & 0xFFFFFFF0) + 8.0f;
        pos.y = ((s32)pos.y & 0xFFFFFFF0) + 8.0f;
        if (fn_80226998((const JGeometry::TVec3f*)&pos, field_0x4->field_0x3b4) == 0x17E) {
            pos.x += 8.0f;
        } else if (fn_80226998((const JGeometry::TVec3f*)&pos, field_0x4->field_0x3b4) == 0x17F) {
            pos.x -= 8.0f;
        }
        if (field_0x4->fn_8026E5F8(&pos) && tile == 0x11) {
            field_0x4->field_0x441 = true;
        }
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// Whether the tile allows a push toward the owner's facing.
u8 cls_804A71A8::fn_8026EB1C(u16 tile) {
    if (!field_0x4->fn_80250D30()) {
        return false;
    }
    switch (tile) {
    case 0x10:
        return field_0x4->field_0x3dc == 3;
    case 0x11:
        return field_0x4->field_0x3dc == 2;
    case 0x12:
        return field_0x4->field_0x3dc == 1;
    case 0x13:
        return field_0x4->field_0x3dc == 0;
    case 0x14:
        if (field_0x4->field_0x3dc == 3) {
            return true;
        }
        return field_0x4->field_0x3dc == 1;
    case 0x15:
        if (field_0x4->field_0x3dc == 3) {
            return true;
        }
        return field_0x4->field_0x3dc == 0;
    case 0x16:
        if (field_0x4->field_0x3dc == 2) {
            return true;
        }
        return field_0x4->field_0x3dc == 1;
    case 0x17:
        if (field_0x4->field_0x3dc == 2) {
            return true;
        }
        return field_0x4->field_0x3dc == 0;
    }
    return false;
}

// FIXME: 86.81. As fn_8026EE60; the spawn also reads `player + 1` after
// getActorManager_801F856C like the original, but the owner and facing are reloaded.
void cls_804A71A8::fn_8026EC88(s32 player) {
    field_0xac = 1;
    if (player != field_0x4->fn_80240868()) {
        field_0x38[field_0x1c] = 1;
    } else if (canPush(this)) {
        JGeometry::TVec3f pos = field_0x8;
        pos.z = 0.0f;
        Player_804A6F38* owner = field_0x4;
        s32 dir = owner->field_0x3dc;
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLCK, owner->field_0x3b4, &pos,
                                                ((player + 1) & 7) << 4 | (dir & 3) << 2, -1, -1);
        if (fn_8022461C(&pos, field_0x4->field_0x3b4) != 0x50 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
            field_0x4->field_0xd94 = serial;
        }
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// FIXME: 86.41. canPush's step vector is added with its x, y, z loaded in
// reverse and its z half first (the original copies x, y, z in order to the
// stack, then adds both halves); the spawn reads the owner's facing after
// getActorManager_801F856C (the original keeps the owner and the facing in r29 / r30
// across it).
void cls_804A71A8::fn_8026EE60() {
    field_0xac = 1;
    if (canPush(this)) {
        JGeometry::TVec3f pos = field_0x8;
        pos.z = 0.0f;
        Player_804A6F38* owner = field_0x4;
        s32 dir = owner->field_0x3dc;
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLCK, owner->field_0x3b4, &pos, (dir & 3) << 2, -1, -1);
        if (fn_8022461C(&pos, field_0x4->field_0x3b4) != 0x50 && getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
            field_0x4->field_0xd94 = serial;
        }
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// FIXME: 90.04. fn_8026EE60 inlined, with its differences.
void cls_804A71A8::fn_8026F000() {
    if (field_0x4->field_0x3dc == 1) {
        fn_8026EE60();
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// FIXME: 90.04. fn_8026EE60 inlined, with its differences.
void cls_804A71A8::fn_8026F1C4() {
    if (field_0x4->field_0x3dc == 0) {
        fn_8026EE60();
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// FIXME: 90.04. fn_8026EE60 inlined, with its differences.
void cls_804A71A8::fn_8026F388() {
    if (field_0x4->field_0x3dc == 2) {
        fn_8026EE60();
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

// FIXME: 90.04. fn_8026EE60 inlined, with its differences.
void cls_804A71A8::fn_8026F54C() {
    if (field_0x4->field_0x3dc == 3) {
        fn_8026EE60();
    } else {
        field_0x38[field_0x1c] = 1;
    }
}

void cls_804A71A8::fn_8026F710() {
    for (s32 i = 0; i < 8; i++) {
        field_0x48[i] = 0;
        field_0x30[i] = 0;
    }
    field_0xb4 = 0;
    field_0xb6 = 0;
    beginFrame(this);
    beginPass(this);
    field_0x98 = 0;
    field_0x99 = 0;
    field_0xa0 = 0;
    field_0xa4 = 0;
    field_0xa8 = 0;
    field_0xbc = 0;
    field_0xb0 = 0;
    field_0xac = 0;
    field_0x14.x = 0.0f;
    field_0x14.y = 0.0f;
}

cls_804A71A8::~cls_804A71A8() {
    field_0x4 = NULL;
}

cls_804A71A8::cls_804A71A8(Player_804A6F38* player) {
    field_0x4 = player;
}
