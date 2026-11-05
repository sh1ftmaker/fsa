#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/PlayerBit.h"
#include "game/ActorId.h"

// File-scope tables in address order (.rodata). The per-pose tables are flat Vec[4 * n] indexed
// [pose + facing * n]: a 2-D Vec[4][n] indexed [facing][pose] changes the codegen (tried). Positions are offsets from mPos per
// facing (0x3DC: right, left, down, up) and animation pose (0x3FD).
// The held thing's position per facing and pose in the lift (state 0x11, fn_8026441C).
static const Vec lbl_8046B250[4 * 4] = {
    {16.0f, 0.0f, 0.0f}, {12.0f, 0.0f, 8.0f}, {0.0f, 0.0f, 16.0f}, {16.0f, 0.0f, -1.0f},
    {-16.0f, 0.0f, 0.0f}, {-12.0f, 0.0f, 8.0f}, {0.0f, 0.0f, 16.0f}, {-16.0f, 0.0f, -1.0f},
    {0.0f, 16.0f, 0.0f}, {0.0f, 12.0f, 8.0f}, {0.0f, 0.0f, 16.0f}, {0.0f, 16.0f, -1.0f},
    {0.0f, -16.0f, 0.0f}, {0.0f, -12.0f, 8.0f}, {0.0f, 0.0f, 16.0f}, {0.0f, -16.0f, -1.0f},
};

// ... in state 0x15 (fn_80263860).
static const Vec lbl_8046B310[4 * 5] = {
    {-6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {16.0f, 0.0f, 8.0f}, {16.0f, 0.0f, 2.0f}, {16.0f, 0.0f, 4.0f},
    {6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {-16.0f, 0.0f, 8.0f}, {-16.0f, 0.0f, 2.0f}, {-16.0f, 0.0f, 4.0f},
    {0.0f, -6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 16.0f, 8.0f}, {0.0f, 16.0f, 2.0f}, {0.0f, 16.0f, 4.0f},
    {0.0f, 6.0f, 11.0f}, {0.0f, 0.0f, 11.0f}, {0.0f, -16.0f, 8.0f}, {0.0f, -16.0f, 2.0f}, {0.0f, -16.0f, 6.0f},
};

// ... in the throw wind-up, state 0x16 (fn_802636D4): poses 0-2.
static const Vec lbl_8046B400[4 * 3] = {
    {-6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 0.0f, 20.0f},
    {6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 0.0f, 20.0f},
    {0.0f, -6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 0.0f, 20.0f},
    {0.0f, 6.0f, 11.0f}, {0.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 20.0f},
};

// ... in the throw, state 0x17 (fn_802635D8): poses 2-4.
static const Vec lbl_8046B490[4 * 5] = {
    {-6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {16.0f, 0.0f, 8.0f}, {16.0f, 0.0f, 2.0f}, {16.0f, 0.0f, 4.0f},
    {6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {-16.0f, 0.0f, 8.0f}, {-16.0f, 0.0f, 2.0f}, {-16.0f, 0.0f, 4.0f},
    {0.0f, -6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 16.0f, 8.0f}, {0.0f, 16.0f, 2.0f}, {0.0f, 16.0f, 4.0f},
    {0.0f, 6.0f, 11.0f}, {0.0f, 0.0f, 11.0f}, {0.0f, -16.0f, 8.0f}, {0.0f, -16.0f, 2.0f}, {0.0f, -16.0f, 6.0f},
};

// ... in state 0x18 (fn_802632A8).
static const Vec lbl_8046B580[4 * 5] = {
    {-6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {7.0f, 0.0f, 12.0f}, {12.0f, 0.0f, 0.0f}, {16.0f, 0.0f, 0.0f},
    {6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {-7.0f, 0.0f, 12.0f}, {-12.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f},
    {0.0f, -6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 7.0f, 12.0f}, {0.0f, 12.0f, 0.0f}, {0.0f, 16.0f, 0.0f},
    {0.0f, 6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, -7.0f, 12.0f}, {0.0f, -12.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
};

// ... in state 0x14 (fn_80262254).
static const Vec lbl_8046B670[4 * 5] = {
    {-6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {16.0f, 0.0f, 8.0f}, {16.0f, 0.0f, 2.0f}, {16.0f, 0.0f, 4.0f},
    {6.0f, 0.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {-16.0f, 0.0f, 8.0f}, {-16.0f, 0.0f, 2.0f}, {-16.0f, 0.0f, 4.0f},
    {0.0f, -6.0f, 15.0f}, {0.0f, 0.0f, 17.0f}, {0.0f, 16.0f, 8.0f}, {0.0f, 16.0f, 2.0f}, {0.0f, 16.0f, 4.0f},
    {0.0f, 6.0f, 11.0f}, {0.0f, 0.0f, 11.0f}, {0.0f, -16.0f, 8.0f}, {0.0f, -16.0f, 2.0f}, {0.0f, -16.0f, 6.0f},
};

// 4 along the facing (fn_80261750, state 0x19).
static const Vec lbl_8046B760[4] = {
    {4.0f, 0.0f, 0.0f}, {-4.0f, 0.0f, 0.0f}, {0.0f, 4.0f, 0.0f}, {0.0f, -4.0f, 0.0f},
};

// Carry walk pose per facing and step (fn_8026152C, 0x80262BAC, 0x80263C64).
static const u8 lbl_8046B790[4][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 4, 5, 6},
    {0, 1, 2, 3, 0, 4, 5, 6},
};

// The held thing's offset per carry pose; only the first three floats of a row are read.
static const f32 lbl_8046B7B0[7][5] = {
    {0.0f, 0.0f, 18.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 20.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 19.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 17.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 20.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 19.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 17.0f, 0.0f, 0.0f},
};

// 15.999 along the facing (fn_80260C08).
static const Vec lbl_8046B83C[4] = {
    {15.999f, 0.0f, 0.0f}, {-15.999f, 0.0f, 0.0f}, {0.0f, 15.999f, 0.0f}, {0.0f, -15.999f, 0.0f},
};

// 15.999 along the facing (fn_802604CC).
static const Vec lbl_8046B86C[4] = {
    {15.999f, 0.0f, 0.0f}, {-15.999f, 0.0f, 0.0f}, {0.0f, 15.999f, 0.0f}, {0.0f, -15.999f, 0.0f},
};

// 9.999 along the facing, and 10 against it (fn_8026022C).
static const Vec lbl_8046B89C[4] = {
    {9.999f, 0.0f, 0.0f}, {-9.999f, 0.0f, 0.0f}, {0.0f, 9.999f, 0.0f}, {0.0f, -9.999f, 0.0f},
};

static const Vec lbl_8046B8CC[4] = {
    {-10.0f, 0.0f, 0.0f}, {10.0f, 0.0f, 0.0f}, {0.0f, -10.0f, 0.0f}, {0.0f, 10.0f, 0.0f},
};

// Unit vectors along the facing (fn_8025F698).
static const Vec lbl_8046B8FC[4] = {
    {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
};

static const Vec lbl_8046B92C[4] = {
    {1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
};

// field_0xd64 == 1 as a u8. The original's callers below expand this inline (they test
// it again with cmplwi 1) and fn_8025F684 is its out-of-line copy; a member inline in
// Player.h was measured: fn_8025F684 is then not emitted and fn_8023EFEC (inlines it) drops.
static inline u8 isField0xd64One(const Player_804A6F38& p) {
    return p.field_0xd64 == 1;
}

// fn_8025F684's body (lifting something: field_0xd64 == 1, as a bool). fn_8025EE48,
// fn_8025EFFC and fn_8025F2CC, before fn_8025F684, have it inlined.
static inline bool isLifting_8025F684(const Player_804A6F38& p) {
    return isField0xd64One(p);
}

// fn_8025F590's body: field_0xd64 == 1 (holding) with a player index in field_0xd70.
// fn_8025EB18, before fn_8025F590, has it inlined, so it is a file inline and
// fn_8025F590 its out-of-line copy (as playerFlags_802413F0 / fn_802413F0).
static inline bool holdsPlayer(const Player_804A6F38& p) {
    if (isField0xd64One(p) == 1 && p.field_0xd70 >= 0 && p.field_0xd70 < 4) {
        return true;
    }
    return false;
}

// fn_8025F448's body: holding a player (field_0xd70) or an actor (field_0xd68) without
// lifting it (field_0xd64 == 0), i.e. grabbing it. fn_8025EE48 and fn_8025EFFC, before
// fn_8025F448, have it inlined; fn_8025F448 is its out-of-line copy.
static inline bool isGrabbing_8025F448(const Player_804A6F38& p) {
    if ((p.field_0xd70 >= 0 && p.field_0xd70 < 4 || p.field_0xd68 >= 0) && p.field_0xd64 == 0) {
        return true;
    }
    return false;
}

// fn_8025F2CC's body: whether the player may pick something up (not down, holding,
// grabbing, busy (0xCE8 / 0x454), in the air, or a follower in mode 5 without a
// pending 0xD88). fn_8025EFFC, before fn_8025F2CC, has it inlined; fn_8025F2CC is its
// out-of-line copy.
static inline bool canLift_8025F2CC(Player_804A6F38& p) {
    if (p.fn_80270A8C()) {
        return false;
    }
    if (isLifting_8025F684(p)) {
        return false;
    }
    if (isGrabbing_8025F448(p)) {
        return false;
    }
    if (p.fn_8024BBB0()) {
        return false;
    }
    if (p.fn_80241230()) {
        return false;
    }
    if (p.fn_802412A4()) {
        return false;
    }
    if (p.fn_802413E8()) {
        return false;
    }
    if (p.mPos.z > 0.0f) {
        return false;
    }
    if (p.field_0xd88 < 0 && p.fn_80240808() && p.fn_802486A0() == 5) {
        return false;
    }
    return true;
}

// fn_8025FCC8's body: whether the requested state makes the player let go of what it
// holds (true unless the state is one of the lift / carry / throw states 0x11-0x18,
// 0x1B-0x1D, a screen change 0x20-0x25 while holding something other than a player,
// 0x1E / 0x1F holding a small key, or 0x4C / 0x4D while holding an actor).
// fn_8025F698, before fn_8025FCC8, has it inlined; fn_8025FCC8 is its out-of-line copy.
static inline bool letsGo_8025FCC8(const Player_804A6F38& p) {
    s32 state = p.field_0x1244;
    if (state == 0x11 || state == 0x12 || state == 0x13 || state == 0x14 || state == 0x15 || state == 0x16
        || state == 0x18 || state == 0x1B || state == 0x1C || state == 0x1D
        || (isField0xd64One(p) && !holdsPlayer(p)
            && (state == 0x20 || state == 0x21 || state == 0x22 || state == 0x23 || state == 0x24 || state == 0x25))
        || (p.field_0xd6c == ACTOR_KEY0 && (state == 0x1E || state == 0x1F))
        // plain == 1 here: via isField0xd64One it scores slightly lower (87.38 vs 87.39)
        || (p.field_0xd64 == 1 && !holdsPlayer(p) && (state == 0x4C || state == 0x4D))) {
        return false;
    }
    return true;
}

// The view's box for `layer`: mVisibleBox below layer 8, else field_0xdc
// (as in tu_802684A4 / tu_8026FC20).
static inline JGeometry::TBox2f* layerBox(s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    return layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
}

// FIXME: 99.71. Registers only: the four float temporaries of the view-corner offset
// (f0-f3) are assigned differently.
// Whether the held thing can be put down at field_0x2f4: a player held across from
// a GBA layer (8) is checked on its own layer, with the point moved between the two
// view corners; else the tile there, and for an actor its vfunc_0x4C. On success the
// drop point takes the player's x / y.
bool Player_804A6F38::fn_8025EB18() {
    if (holdsPlayer(*this) && field_0x3b4 == 8 && fn_8023DE58(field_0xd70) < 8) {
        JGeometry::TVec3f pos(field_0x2f4);
        JGeometry::TBox2f* box = layerBox(fn_8023DE58(field_0xd70));
        f32 x = box->i.x;
        f32 y = box->i.y;
        box = layerBox(field_0x3b4);
        JGeometry::TVec2f offset(x - box->i.x, y - box->i.y);
        pos.x += offset.x;
        pos.y += offset.y;
        u16 tile = fn_8022461C(&pos, fn_8023DE58(field_0xd70));
        if ((fn_802265DC(&pos, fn_8023DE58(field_0xd70), true) && tile != 0xE && tile != 0x39 && tile != 0xD)
            || fn_802684A4(&pos, fn_8023DE58(field_0xd70))) {
            field_0x2f4.x = mPos.x;
            field_0x2f4.y = mPos.y;
            return true;
        }
    } else {
        u16 tile = fn_8022461C(&field_0x2f4, field_0x3b4);
        if (holdsPlayer(*this)) {
            if ((fn_802265DC(&field_0x2f4, field_0x3b4, true) && tile != 0xE && tile != 0x39 && tile != 0xD)
                || fn_802684A4(&field_0x2f4, field_0x3b4)) {
                field_0x2f4.x = mPos.x;
                field_0x2f4.y = mPos.y;
                return true;
            }
        } else {
            ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0 && actor->vfunc_0x4C(field_0x3b4, &field_0x2f4)) {
                field_0x2f4.x = mPos.x;
                field_0x2f4.y = mPos.y;
                return true;
            }
        }
    }
    return false;
}

bool Player_804A6F38::fn_8025EDD4(s32 a, const JGeometry::TVec3f* pos) {
    if (field_0xd68 == a) {
        field_0xd68 = -1;
        field_0xd78 = *pos;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8025EE0C(s32 a, const JGeometry::TVec3f* b) {
    if (field_0xd68 == a) {
        field_0x300.set(b->x, b->y, b->z);
    }
}

bool Player_804A6F38::fn_8025EE34(s32 arg) {
    return field_0xd68 == arg;
}

// FIXME: 96.32. The lifting test: the original materialises field_0xd64 == 1 (subfic /
// cntlzw / extrwi.) and branches on it; ours folds it to cmplwi 1 (with `== true`, 98.47 %,
// one cmplwi too many). Same in fn_8025EFFC and fn_8025F2CC.
// Grab the actor `serial` (state 0x19).
bool Player_804A6F38::fn_8025EE48(s32 serial) {
    if (fn_80270A8C()) {
        return false;
    }
    if (isLifting_8025F684(*this)) {
        return false;
    }
    if (isGrabbing_8025F448(*this)) {
        return false;
    }
    if (fn_8024BBB0()) {
        return false;
    }
    if (fn_80241230()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (field_0xd68 < 0 && fn_80251AEC(0x19)) {
        field_0xd68 = serial;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025EF68(s32 arg) {
    if (field_0xd88 == arg) {
        return true;
    }
    if (field_0xd88 >= 0) {
        return false;
    }
    ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(arg);
    if (actor != 0) {
        if (actor->fn_801F36D4(field_0x4)) {
            field_0xd88 = arg;
            return true;
        }
        return false;
    }
    return false;
}

// FIXME: 95.94. The materialised field_0xd64 == 1 test twice, as in fn_8025EE48.
// Lift the actor `serial` (state 0x11, or 0x12 with lift false). With grabOnly, a
// player already holding or grabbing only claims it through fn_8025EF68.
bool Player_804A6F38::fn_8025EFFC(s32 serial, bool lift, bool grabOnly) {
    if (grabOnly) {
        if (isLifting_8025F684(*this) || isGrabbing_8025F448(*this) || fn_80241230() || fn_802412A4()) {
            fn_8025EF68(serial);
            return true;
        }
    } else if (!canLift_8025F2CC(*this)) {
        return false;
    }
    ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
    if (actor != 0) {
        s32 state = 0x12;
        if (lift) {
            state = 0x11;
        }
        if (fn_80251AEC(state)) {
            field_0xd88 = -1;
            field_0xd6c = actor->mActorId;
            field_0x2f4 = actor->mPos;
            field_0xd64 = 1;
            field_0xd68 = serial;
            actor->fn_801F3744(field_0x4);
            field_0xd74 = 3;
            field_0x1174 = 1;
            fn_8024F0E8(0x20022);
            return true;
        }
    }
    return false;
}

// FIXME: 96.81. The materialised field_0xd64 == 1 test, as in fn_8025EE48.
bool Player_804A6F38::fn_8025F2CC() {
    return canLift_8025F2CC(*this);
}

bool Player_804A6F38::fn_8025F418() {
    if (field_0xd70 >= 0 && field_0xd70 < 4 && field_0xd64 == 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025F448() {
    return isGrabbing_8025F448(*this);
}

bool Player_804A6F38::fn_8025F484() const {
    if (isField0xd64One(*this) == 1) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && actor->field_0x1a4 > 0) {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool Player_804A6F38::fn_8025F500() {
    if (isField0xd64One(*this) == 1 && field_0xd68 >= 0) {
        return true;
    }
    return false;
}

s32 Player_804A6F38::fn_8025F534() {
    if (field_0xd64 == 1 || field_0xd70 < 0 || field_0xd70 >= 4) {
        return -1;
    }
    return field_0xd70;
}

s32 Player_804A6F38::fn_8025F55C() {
    if (isField0xd64One(*this) == 1 && field_0xd70 >= 0 && field_0xd70 < 4) {
        return field_0xd70;
    }
    return -1;
}

bool Player_804A6F38::fn_8025F590() {
    return holdsPlayer(*this);
}

void Player_804A6F38::fn_8025F5CC() {
    if (field_0xd64 != 0) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0) {
                actor->mAlive = false;
                field_0xd6c = 'NULL';
                field_0xd64 = 0;
                field_0xd68 = -1;
            }
        }
    }
}

s32 Player_804A6F38::fn_8025F648() {
    if (field_0xd64 == 1) {
        return field_0xd6c;
    }
    return 'NULL';
}

s32 Player_804A6F38::fn_8025F668() {
    if (field_0xd64 == 1) {
        return field_0xd68;
    }
    return -1;
}

bool Player_804A6F38::fn_8025F684() {
    return isLifting_8025F684(*this);
}

// FIXME: 95.94. The six missing instructions are the inlined fn_8025FCC8's (see there: state
// 0x11 tested on its own, the field_0xd64 == 1 byte recomputed in each inlined
// fn_8025F590); besides, the facing table's three loads for `step` are scheduled z first.
// Let go of what the player holds (from the commit of every state change, with
// checkState, and from fn_8026000C): a held player is thrown 16 a step along the facing
// (snapped to the tile centre across when two of three steps are walls, else at 3.3
// along the facing; nothing when the state is 0x1E / 0x32 or the player has no life),
// an actor gets a speed along the facing by kind (crystal ball / Bombchu 2.5, grass 4,
// dead 6, else 4); then the thrown player's group partners holding no living actor
// leave their states too.
void Player_804A6F38::fn_8025F698(bool checkState) {
    if (field_0xd64 != 0 && (!checkState || letsGo_8025FCC8(*this))) {
        bool held = true;
        if (field_0xd6c == 'NULL') {
            held = false;
        }
        if (holdsPlayer(*this)) {
            JGeometry::TVec3f speed;
            JGeometry::TVec3f step;
            const Vec& dir = lbl_8046B8FC[field_0x3dc];
            step.set(dir.x, dir.y, dir.z);
            step.scale(16.0f);
            if (field_0x1244 == 0x1E || field_0x1244 == 0x32 || fn_8024189C() <= 0) {
                speed.set(0.0f, 0.0f, 0.0f);
            } else {
                JGeometry::TVec3f pos(*fn_8023E724(field_0xd70));
                int walls = 0;
                for (int i = 0; i < 3; i++) {
                    pos += step;
                    u16 tile = fn_8022461C(&pos, fn_8023DE58(field_0xd70));
                    if (tile == 3 || tile == 0xE || tile == 0x3A) {
                        walls++;
                    }
                }
                const Vec& unit = lbl_8046B8FC[field_0x3dc];
                speed.set(unit.x, unit.y, unit.z);
                if (walls >= 2) {
                    if (field_0x3dc == 0 || field_0x3dc == 1) {
                        pos.x = (s32)pos.x / 16 * 16 + 8;
                    } else {
                        pos.y = (s32)pos.y / 16 * 16 + 8;
                    }
                    JGeometry::TVec3f diff = pos - *fn_8023E724(field_0xd70);
                    speed.set(diff);
                    speed.scale(1.0f / 14.0f);
                } else {
                    speed.scale(3.3f);
                }
            }
            fn_80230D80(field_0xd70, &speed);
            field_0xd70 = -1;
            fn_8024F0E8(0x20014);
        } else {
            ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0) {
                JGeometry::TVec3f speed;
                const Vec& dir = lbl_8046B92C[field_0x3dc];
                speed.set(dir.x, dir.y, dir.z);
                if (actor->mActorId == ACTOR_CRTL || actor->mActorId == ACTOR_BMTY) {
                    speed.scale(2.5f);
                } else if (actor->mActorId == ACTOR_GRAS) {
                    speed.scale(4.0f);
                } else if (actor->field_0x1a4 == 0) {
                    speed.scale(6.0f);
                } else {
                    speed.scale(4.0f);
                }
                actor->fn_801F3708(&speed);
                fn_8024F0E8(0x20014);
            }
            field_0xd68 = -1;
        }
        field_0xd64 = 0;
        field_0xd6c = 'NULL';
        field_0xd74 = 3;
        if (held == true && fn_802486A0() != 4 && isPlayerBitSet(lbl_8053AAF8, field_0x4)) {
            for (int i = 0; i < 4; i++) {
                if (fn_8022E438(field_0x4, i) && i != field_0x4) {
                    s32 serial = fn_8023EF50(i);
                    ActorBase_804A5770* other = fn_801F856C()->findActorBySerial_801F666C(serial);
                    if (other != 0 && other->field_0x1a4 <= 0) {
                        fn_80230C6C(i);
                    }
                }
            }
        }
    }
}

// FIXME: 87.39. Two differences: the original tests state 0x11 on its own before the folded
// range 0x12-0x16 (ours folds 0x11-0x16), and it recomputes the field_0xd64 == 1 byte
// (subfic / cntlzw / extrwi) inside each inlined fn_8025F590 where ours reuses it.
bool Player_804A6F38::fn_8025FCC8() {
    return letsGo_8025FCC8(*this);
}

// FIXME: 99.10. Registers only: the float temporaries of the view-corner offset.
// Let go of the held player: take its position, moved from its layer's view corner
// to this player's, and remember pos as the drop point.
bool Player_804A6F38::fn_8025FDD0(const JGeometry::TVec3f* pos) {
    mPos = *fn_8023E7B0(field_0xd70);
    JGeometry::TBox2f* box = layerBox(fn_8023DE58(field_0xd70));
    f32 x = box->i.x;
    f32 y = box->i.y;
    box = layerBox(field_0x3b4);
    JGeometry::TVec2f offset(box->i.x - x, box->i.y - y);
    mPos.x += offset.x;
    mPos.y += offset.y;
    field_0xd70 = -1;
    field_0xd58 = 5;
    field_0xd78 = *pos;
    return true;
}

bool Player_804A6F38::fn_8025FEDC() {
    if (!fn_8025F448()) {
        return false;
    }
    fn_80251AEC(0);
    field_0xd70 = -1;
    field_0xd68 = -1;
    mPos.z = 0.0f;
    return true;
}

bool Player_804A6F38::fn_8025FF70() {
    if (!holdsPlayer(*this)) {
        return false;
    }
    fn_80251AEC(0);
    field_0xd64 = 0;
    field_0xd70 = -1;
    field_0xd6c = 'NULL';
    return true;
}

bool Player_804A6F38::fn_8026000C() {
    fn_8025F698(false);
    return fn_80251AEC(0);
}

bool Player_804A6F38::fn_80260048(s32 arg) {
    if (!fn_80270A8C()) {
        return false;
    }
    field_0xd70 = arg;
    fn_80248FF0();
    return true;
}

// Grab another player: lift it (state 0x19) when it can't take part in a pair
// ('TSNO', "two-sword motion", docs/names/actors.tsv), else pair up with it.
bool Player_804A6F38::fn_802600A0(s32 player) {
    if (fn_80270A8C()) {
        return false;
    }
    if (!isPlayerBitSet(lbl_8053AAF8, field_0x4) && !fn_8022E438(field_0x4, player)
        && (lbl_8053AAF8->field_0x7e == 0 || fn_802486A0() != 4)
        && (fn_8023DE58(player) != 8 || field_0x3b4 >= 8)) {
        field_0xd60 = fn_8022F340(player);
        field_0x11ec = fn_8022F340(player);
        fn_801F856C()->fn_801F9484(ACTOR_TSNO, field_0x3b4, &mPos, (player & 3) | (field_0x4 << 2), -1, player);
        if (player > field_0x4) {
            fn_80233F78(fn_80240644());
        }
    } else if (fn_80251AEC(0x19)) {
        field_0xd70 = player;
        if (fn_802486A0() != 4) {
            for (int i = 0; i < 4; i++) {
                if (fn_80243194(i) && i != field_0x4) {
                    fn_80230C6C(i);
                }
            }
        }
    } else {
        return false;
    }
    fn_80248FF0();
    return true;
}

void Player_804A6F38::fn_80261468() {
    if (field_0xd74 > 0) {
        field_0xd74--;
    }
}

void Player_804A6F38::fn_80261480() {
    if (field_0xc9c <= 0 && field_0xd74 == 0 && !(fn_80240808() && fn_802486A0() == 5)) {
        if (fn_80260C08()) {
            field_0xd74 = 3;
            field_0x1174 = 1;
        } else if (fn_802604CC()) {
            field_0xd74 = 3;
            field_0x1174 = 1;
        } else {
            fn_8026022C();
        }
    }
}

void Player_804A6F38::fn_8026173C() {
    field_0x121c = 0;
    field_0xd84 = 20;
}

void Player_804A6F38::fn_80262188() {
    field_0x458 = 0;
    if (field_0x1248 != 0x33 && field_0xcfe == 0) {
        fn_8024F0E8(0x10007);
    }
    if (field_0xd70 >= 0 && lbl_8053AAF8->field_0x7e != 0 && !isPlayerBitSet(lbl_8053AAF8, field_0x4)
        && field_0xd60 < 0) {
        field_0x11ec = fn_8022F340(field_0xd70);
        field_0xd60 = fn_8022F340(field_0xd70);
    }
}

void Player_804A6F38::fn_80262234() {
    fn_80258BBC();
}

void Player_804A6F38::fn_80262B6C() {
    fn_80259660();
}

void Player_804A6F38::fn_80262B8C() {
    fn_80283D2C();
}

void Player_804A6F38::fn_802632A4() {}

void Player_804A6F38::fn_802635C8() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_802635D4() {}

// Update of state 0x17 (the throw, provisional): poses 2 / 3 / 4 over 14 frames, then
// state 0; the held position follows lbl_8046B490.
void Player_804A6F38::fn_802635D8() {
    field_0x3fc++;
    if (field_0x3fc < 2) {
        field_0x3fd = 2;
    } else if (field_0x3fc < 10) {
        field_0x3fd = 3;
    } else if (field_0x3fc < 14) {
        field_0x3fd = 4;
    } else {
        field_0x3fd = 2;
        fn_80251AEC(0);
    }
    const Vec& offset = lbl_8046B490[field_0x3fd + field_0x3dc * 5];
    field_0x2f4.set(offset.x, offset.y, offset.z);
    field_0x2f4 += mPos;
    fn_8025EB18();
    fn_80268B3C();
}

void Player_804A6F38::fn_802636C4() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_802636D0() {}

// Update of state 0x16 (the throw wind-up, provisional): poses 0 / 1 over 10 frames, then
// state 0x17; a thrown player gets a 'THCL' ("attack hitbox when player is thrown",
// docs/names/actors.tsv) at its position.
void Player_804A6F38::fn_802636D4() {
    field_0x3fc++;
    if (field_0x3fc < 8) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 10) {
        field_0x3fd = 1;
    } else {
        field_0x3fd = 2;
        if (fn_80251AEC(0x17) && holdsPlayer(*this)) {
            JGeometry::TVec3f* pos; // declared first: the original's register order (r30)
            u16 thrower = field_0x4;
            s32 thrown = field_0xd70;
            pos = fn_8023E724(thrown);
            fn_801F856C()->fn_801F9484(ACTOR_THCL, field_0x3b4, pos, thrown, -1, thrower);
        }
    }
    const Vec& offset = lbl_8046B400[field_0x3fd + field_0x3dc * 3];
    field_0x2f4.set(offset.x, offset.y, offset.z);
    field_0x2f4 += mPos;
    fn_8025EB18();
    fn_80268B3C();
}

void Player_804A6F38::fn_80263848() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80263854() {
    field_0x459 = 0;
}

void Player_804A6F38::fn_80263BA0() {
    field_0x458 = 0;
    field_0x459 = 0;
    if (holdsPlayer(*this)) {
        fn_8027221C(4);
    } else {
        ActorBase_804A5770* actor = fn_801F856C()->findActorBySerial_801F666C(field_0xd68);
        if (actor != 0) {
            fn_8027221C(actor->field_0x118);
        } else {
            fn_8027221C(0);
        }
    }
    fn_8024F0E8(0x1000E);
}

void Player_804A6F38::fn_80264410() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80264768() {
    field_0x458 = 0;
}
