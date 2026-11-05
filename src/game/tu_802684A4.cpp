#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/cls_804A71A8.h"
#include "game/Map.h"
#include "game/ViewLayer.h"

// One step of 16 along each facing (0: +x, 1: -x, 2: +y, 3: -y).
static const Vec lbl_8046B998[4] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
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

// The view's box for `layer`: mVisibleBox below layer 8, else field_0xdc.
static inline JGeometry::TBox2f* layerBox(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    return layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
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
                    fn_8023DE58(field_0x4->field_0xd70) < 8)
                {
                    // Probe the same spot on the layer of the player fn_8023DE58
                    // names, moved from this layer's view box to that one's.
                    JGeometry::TVec3f saved = field_0x8;
                    field_0x8.x -= layerBox(field_0x4->field_0x3b4)->i.x;
                    field_0x8.y -= layerBox(field_0x4->field_0x3b4)->i.y;
                    field_0x8.x += layerBox(fn_8023DE58(field_0x4->field_0xd70))->i.x;
                    field_0x8.y += layerBox(fn_8023DE58(field_0x4->field_0xd70))->i.y;
                    if (field_0x4->fn_802684A4(&field_0x8, fn_8023DE58(field_0x4->field_0xd70))) {
                        field_0x28[field_0x1c] = 1;
                    } else {
                        fn_8026B81C(fn_8022461C(&field_0x8, fn_8023DE58(field_0x4->field_0xd70)));
                    }
                    field_0x8 = saved;
                }
                if (field_0x4->fn_802684A4(&field_0x8, field_0x4->field_0x3b4)) {
                    field_0x28[field_0x1c] = 1;
                } else if (field_0x4->fn_8026FC20() <= 0 && !fn_802D6A9C()->fn_802DCD0C() &&
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
        } else if (lbl_8053AAF8->field_0x96 == 0) {
            field_0x99 = field_0x98;
            field_0x98 = 0;
        }
        JGeometry::TVec2f moved(field_0x4->mPos.x, field_0x4->mPos.y);
        moved.set(moved.x - startX, moved.y - startY);
        field_0x14 = moved.x;
        field_0x18 = moved.y;
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

// The body of fn_8026F000 / fn_8026F1C4 / fn_8026F388 / fn_8026F54C for
// their facing.
// FIXME (all four, 88.25): canPush's step vector is built and added with
// its x, y, z loaded in reverse and the z half added first (the original
// loads x, y, z in order, then adds both halves); the spawn reads the
// owner's facing after fn_801F856C (the original keeps the owner and the
// facing in r29 / r30 across it).
static inline void pushBlock(cls_804A71A8* p, s32 facing) {
    if (p->field_0x4->field_0x3dc == facing) {
        p->field_0xac = 1;
        if (canPush(p)) {
            JGeometry::TVec3f pos = p->field_0x8;
            pos.z = 0.0f;
            Player_804A6F38* owner = p->field_0x4;
            s32 dir = owner->field_0x3dc;
            s32 serial = fn_801F856C()->fn_801F9484(ACTOR_BLCK, owner->field_0x3b4, &pos, (dir & 3) << 2, -1, -1);
            if (fn_8022461C(&pos, p->field_0x4->field_0x3b4) != 0x50 && fn_801F856C()->fn_801F666C(serial) != NULL) {
                p->field_0x4->field_0xd94 = serial;
            }
        } else {
            p->field_0x38[p->field_0x1c] = 1;
        }
    } else {
        p->field_0x38[p->field_0x1c] = 1;
    }
}

// FIXME: 88.25. See pushBlock.
void cls_804A71A8::fn_8026F000() {
    pushBlock(this, 1);
}

// FIXME: 88.25. See pushBlock.
void cls_804A71A8::fn_8026F1C4() {
    pushBlock(this, 0);
}

// FIXME: 88.25. See pushBlock.
void cls_804A71A8::fn_8026F388() {
    pushBlock(this, 2);
}

// FIXME: 88.25. See pushBlock.
void cls_804A71A8::fn_8026F54C() {
    pushBlock(this, 3);
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
    field_0x14 = 0.0f;
    field_0x18 = 0.0f;
}

cls_804A71A8::~cls_804A71A8() {
    field_0x4 = NULL;
}

cls_804A71A8::cls_804A71A8(Player_804A6F38* player) {
    field_0x4 = player;
}
