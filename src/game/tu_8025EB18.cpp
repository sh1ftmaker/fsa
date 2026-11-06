#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/PlayerBit.h"
#include "game/ActorId.h"
#include "game/ViewLayer.h"
#include "game/VecEpsilon.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/cls_804A71A8.h"
#include "game/tu_8030AE84.h"
#include "game/act/act_thief.h"
#include "game/act/act_cucco.h"
#include "game/BoxTest.h"

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

// field_0xd64 == 1 as a u8. The original's callers expand this inline (they test it
// again with cmplwi 1); fn_8025F684 returns it as a bool. A member inline in Player.h
// was measured: fn_8025F684 is then not emitted and fn_8023EFEC (inlines it) drops.
static inline u8 isField0xd64One(const Player_804A6F38& p) {
    return p.field_0xd64 == 1;
}

// Point-in-box test (x / y only), as in tu_8026FC20.
static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

void Player_804A6F38::fn_80264768() {
    field_0x458 = 0;
}

// Update of state 0x11, the lift: the held thing goes through the four lift positions
// (lbl_8046B250, `pose`) over 24 frames, then state 0x12; a living actor (field_0x1a4)
// is first shaken for 48 frames (pose 3) and lifted over frames 49-80. Nothing to lift
// (field_0xd68 < 0): poses 0-1, state 0 at frame 22; a lost actor ends the lift at once.
void Player_804A6F38::fn_8026441C() {
    s32 pose = 0;
    field_0x3fc++;
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
    if (fn_8025F590()) {
        if (field_0x3fc < 8) {
            field_0x3fd = 0;
            pose = 0;
        } else if (field_0x3fc < 16) {
            field_0x3fd = 1;
            pose = 0;
        } else if (field_0x3fc < 24) {
            field_0x3fd = 2;
            pose = 1;
        } else {
            field_0x3fd = 3;
            pose = 2;
            fn_80251AEC(0x12);
        }
    } else if (field_0xd68 < 0) {
        pose = 0;
        if (field_0x3fc >= 22) {
            field_0x3fd = 1;
            fn_80251AEC(0);
        } else if (field_0x3fc <= 8) {
            field_0x3fd = 0;
        } else {
            field_0x3fd = 1;
        }
    } else if (actor == 0) {
        fn_80251AEC(0);
        field_0xd64 = 0;
        field_0xd6c = 'NULL';
        field_0xd68 = -1;
    } else if (actor->field_0x1a4 != 0) {
        if (field_0x3fc <= 8) {
            field_0x3fd = 0;
            pose = 3;
        } else if (field_0x3fc <= 24) {
            field_0x3fd = 1;
            pose = 3;
        } else if (field_0x3fc <= 32) {
            field_0x3fd = 0;
            pose = 3;
        } else if (field_0x3fc < 48) {
            field_0x3fd = 1;
            pose = 3;
        } else if (field_0x3fc == 48) {
            // Its own test in the original (bge, then bne on the same compare with 48).
            field_0x3fd = 0;
            pose = 0;
        } else if (field_0x3fc <= 56) {
            field_0x3fd = 0;
            pose = 0;
        } else if (field_0x3fc <= 64) {
            field_0x3fd = 1;
            pose = 0;
        } else if (field_0x3fc <= 72) {
            field_0x3fd = 2;
            pose = 1;
        } else if (field_0x3fc <= 80) {
            field_0x3fd = 3;
            pose = 2;
        } else {
            field_0x3fd = 3;
            pose = 2;
            fn_80251AEC(0x12);
        }
    } else {
        if (field_0x3fc < 8) {
            field_0x3fd = 0;
            pose = 0;
        } else if (field_0x3fc < 16) {
            field_0x3fd = 1;
            pose = 0;
        } else if (field_0x3fc < 24) {
            field_0x3fd = 2;
            pose = 1;
        } else {
            field_0x3fd = 3;
            pose = 2;
            fn_80251AEC(0x12);
        }
    }
    if (pose >= 0) {
        const Vec& offset = lbl_8046B250[pose + field_0x3dc * 4];
        field_0x2f4.set(offset.x, offset.y, offset.z);
        field_0x2f4 += mPos;
    }
    fn_8025EB18();
    fn_80268B3C();
}

void Player_804A6F38::fn_80264410() {
    field_0x458 = 0;
}

// Update of state 0x12, the carry (tu_80285E58's dispatch): the stick step (fn_8027FA08's z
// of 0 drops mPos.z to the floor unless 0x434); fn_8022C13C (unless fn_802416C4 is 7): a
// held player that fn_80236B50 rejects, a bomb ('HBMB') or a small key ('KEY0') is put
// down (0x18), anything else thrown (0x16, also when fn_8024B24C); fn_8022C0A0 throws
// (0x16); fn_802466B4 starts 0x15 unless the held actor's field_0x1a4 is 2 or more. Then
// it walks with the held thing (fn_802808E8 at 1.25 for a player, 1.5 for an actor with
// field_0x1a4 == 0, none otherwise) through the carry cycle fn_8026152C(false); a held
// player that stops being one (fn_8023EA28) or a lost actor ends the carry (state 0).
void Player_804A6F38::fn_80263C64() {
    JGeometry::TVec3f stick;
    fn_8027FA08(&stick);
    if (0.0f == stick.z && 0.0f != mPos.z && !field_0x434) {
        mPos.z = 0.0f;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
    if (fn_8022C13C(field_0x4)) {
        if (fn_802416C4() != 7) {
            if (!fn_8024B24C()
                && ((fn_8025F590() && !fn_80236B50(field_0xd70)) || fn_8025F648() == ACTOR_HBMB
                    || fn_8025F648() == ACTOR_KEY0)) {
                fn_80251AEC(0x18);
            } else {
                fn_80251AEC(0x16);
            }
        }
    } else if (fn_8022C0A0(field_0x4)) {
        fn_80251AEC(0x16);
    } else if (fn_802466B4() && (actor == 0 || actor->field_0x1a4 < 2)) {
        fn_80251AEC(0x15);
    }
    if (fn_8025F590()) {
        if (fn_8023EA28(field_0xd70)) {
            fn_802808E8(1.25f);
            fn_8027EB04();
            fn_8026152C(false);
        } else {
            fn_80251AEC(0);
            field_0xd70 = -1;
        }
    } else if (actor != 0) {
        if (actor->field_0x1a4 == 0) {
            fn_802808E8(1.5f);
            fn_8027EB04();
            fn_8026152C(false);
        } else {
            fn_8027EB04();
            fn_8026152C(false);
        }
    } else {
        fn_80251AEC(0);
        field_0xd64 = 0;
        field_0xd68 = -1;
        field_0xd6c = 'NULL';
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80263BA0() {
    field_0x458 = 0;
    field_0x459 = 0;
    if (fn_8025F590()) {
        fn_8027221C(4);
    } else {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
        if (actor != 0) {
            fn_8027221C(actor->field_0x118);
        } else {
            fn_8027221C(0);
        }
    }
    fn_8024F0E8(0x1000E);
}

// Update of state 0x15 (with the held thing): poses 0-4 over 22 frames, then state 0x12;
// at frame 11 a held thief ('TZOK') gets fn_80206168; at pose 2 the tile under the held
// thing (fn_802A3164) triggers its vfunc_0x48; when it can be put down (fn_8025EB18)
// the pose goes back to 1 and field_0x35c is turned against the facing as in 0x18.
void Player_804A6F38::fn_80263860() {
    field_0x3fc++;
    if (field_0x3fc < 8) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 9) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 10) {
        field_0x459 = 1;
        field_0x3fd = 2;
    } else if (field_0x3fc < 18) {
        field_0x3fd = 3;
        if (field_0x3fc == 11 && field_0xd6c == ACTOR_TZOK) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0) {
                static_cast<Thief_804A5A30*>(actor)->fn_80206168();
            }
        }
    } else if (field_0x3fc < 22) {
        field_0x3fd = 4;
    } else {
        field_0x3fd = 2;
        fn_80251AEC(0x12);
    }
    const Vec& offset = lbl_8046B310[field_0x3fd + field_0x3dc * 5];
    field_0x2f4.set(offset.x, offset.y, offset.z);
    field_0x2f4 += mPos;
    if (field_0x3fd == 2) {
        JGeometry::TVec3f pos(field_0x2f4);
        pos.z = 0.0f;
        if (gBgRewrite_8053AC98->fn_802A3164(&pos, field_0x4, field_0x3b4)) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0) {
                actor->vfunc_0x48();
            }
        }
    }
    if (fn_8025EB18()) {
        field_0x3fd = 1;
        field_0x459 = 0;
        const Vec& back = lbl_8046B310[field_0x3fd + field_0x3dc * 5];
        field_0x2f4.set(back.x, back.y, back.z);
        field_0x2f4 += mPos;
        if (field_0x35c.length() < 1.0f) {
            const Vec& dir = lbl_8046B310[field_0x3dc * 5 + 2];
            field_0x35c.set(dir.x, dir.y, dir.z);
            field_0x35c.z = 0.0f;
            field_0x35c.normalize();
            field_0x35c.scale(-1.0f);
        }
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80263854() {
    field_0x459 = 0;
}

void Player_804A6F38::fn_80263848() {
    field_0x458 = 0;
}

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
        if (fn_80251AEC(0x17) && fn_8025F590()) {
            JGeometry::TVec3f* pos; // declared first: the original's register order (r30)
            u16 thrower = field_0x4;
            s32 thrown = field_0xd70;
            pos = getPlayerPos_8023E724(thrown);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_THCL, field_0x3b4, pos, thrown, -1, thrower);
        }
    }
    const Vec& offset = lbl_8046B400[field_0x3fd + field_0x3dc * 3];
    field_0x2f4.set(offset.x, offset.y, offset.z);
    field_0x2f4 += mPos;
    fn_8025EB18();
    fn_80268B3C();
}

void Player_804A6F38::fn_802636D0() {}

void Player_804A6F38::fn_802636C4() {
    field_0x458 = 0;
}

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

void Player_804A6F38::fn_802635D4() {}

void Player_804A6F38::fn_802635C8() {
    field_0x458 = 0;
}

// Update of state 0x18, the put-down: poses 0-2 over 11 frames, then 3 (4 for a held
// player); at frame 12 state 0, letting go in place (an actor gets speed 0); the held
// thing follows lbl_8046B580, and when it can be put down (fn_8025EB18) and field_0x35c
// is shorter than 1 it becomes a unit vector against the facing (pose 3's offset, flat).
void Player_804A6F38::fn_802632A8() {
    field_0x3fc++;
    if (field_0x3fc < 8) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 10) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 11) {
        field_0x3fd = 2;
    } else if (field_0x3fc < 12) {
        if (fn_8025F590()) {
            field_0x3fd = 4;
        } else {
            field_0x3fd = 3;
        }
    } else {
        field_0x3fc = 12;
        if (fn_80251AEC(0)) {
            if (fn_8025F590()) {
                field_0x3fd = 4;
            } else {
                field_0x3fd = 3;
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
                if (actor != 0) {
                    JGeometry::TVec3f speed(0.0f, 0.0f, 0.0f);
                    actor->fn_801F3708(&speed);
                }
            }
            field_0xd70 = -1;
            field_0xd68 = -1;
            field_0xd64 = 0;
            field_0xd6c = 'NULL';
            field_0xd74 = 3;
        }
    }
    const Vec& offset = lbl_8046B580[field_0x3fd + field_0x3dc * 5];
    field_0x2f4.set(offset.x, offset.y, offset.z);
    field_0x2f4 += mPos;
    if (fn_8025EB18() && field_0x35c.length() < 1.0f) {
        const Vec& dir = lbl_8046B580[field_0x3dc * 5 + 3];
        field_0x35c.set(dir.x, dir.y, dir.z);
        field_0x35c.z = 0.0f;
        field_0x35c.normalize();
        field_0x35c.scale(-1.0f);
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_802632A4() {}

// FIXME: 99.78. Registers only: the view-box test keeps pos.x in f2 and pos.y in f1 (ours
// f1 / f2); with the byte compared directly (cmplwi, not the original's cmpwi) they match.
// Enter of state 0x13 (tu_80285E58's dispatch; update fn_80262BAC, the cucco glide): with
// a held cucco ('NIWA') whose field_0x2a1 is 1, probe 16-steps along field_0x344.x inside the view box for the second tile 0x10-0x17; found,
// field_0x324 becomes the point one step further on (fn_80262BAC glides toward its x);
// else the player is put at field_0x324.y. Faces along field_0x344.x.
void Player_804A6F38::fn_80263078() {
    fn_8028414C();
    if (field_0xd6c == ACTOR_NIWA && !field_0x441) {
        Cucco_804AB8C0* cucco = static_cast<Cucco_804AB8C0*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68));
        if (cucco != 0 && cucco->getField0x2a1() == 1) {
            JGeometry::TVec2f pos;
            pos.x = mPos.x;
            pos.y = mPos.y;
            int left = 2;
            do {
                if (field_0x344.x > 0.0f) {
                    pos.x += 16.0f;
                } else {
                    pos.x -= 16.0f;
                }
                if (!containsPoint(*layerBox(field_0x3b4), pos)) {
                    break;
                }
                u16 tile = fn_802265FC((const JGeometry::TVec3f*)&pos, field_0x3b4);
                if (tile >= 0x10 && tile <= 0x17) {
                    left--;
                }
            } while (left > 0);
            if (left == 0) {
                if (field_0x344.x > 0.0f) {
                    pos.x += 16.0f;
                } else {
                    pos.x -= 16.0f;
                }
                field_0x324.x = pos.x;
                field_0x324.y = pos.y;
                field_0x344.z = 2.0f;
            } else {
                field_0x344.z = 2.0f;
                mPos.z = field_0x324.y - mPos.y;
                mPos.y = field_0x324.y;
            }
        } else {
            field_0x344.z = 2.0f;
            mPos.z = field_0x324.y - mPos.y;
            mPos.y = field_0x324.y;
        }
        if (field_0x344.x > 0.0f) {
            field_0x3dc = 0;
            field_0x3e8 = 0;
        } else if (field_0x344.x < 0.0f) {
            field_0x3dc = 1;
            field_0x3e8 = 1;
        }
    }
}

// FIXME: 99.23. Two lfs missing: the original reloads mPos.x for the 0.5 steps after
// computing field_0x324.x - mPos.x; ours reuses the register.
// Update of state 0x13 (the fall while carrying, field_0x344 the speed): a cucco ('NIWA') is
// carried gliding: the player moves 0.5 a frame toward field_0x324.x, bobbing with the
// distance, else falls and lands back in 0x12; anything else falls at 0.25 (0.17 down to
// -6 while 0x43F / 0x441, floating at 8 below field_0x324.y), with the carry walk cycle
// at lbl_8046B790 / lbl_8046B7B0 (fn_8026152C's) and sounds 0x1000D / 0x2003E when 0x440.
void Player_804A6F38::fn_80262BAC() {
    if (field_0xd6c == ACTOR_NIWA && !field_0x441) {
        f32 dx = field_0x324.x - mPos.x;
        if (dx >= 0.5f) {
            mPos.x += 0.5f;
        } else if (dx <= -0.5f) {
            mPos.x -= 0.5f;
        } else if (0.0f != dx) {
            mPos.x = field_0x324.x;
        }
        if (0.0f != dx) {
            field_0x344.z -= 0.25f;
            if (field_0x344.z <= 0.0f) {
                field_0x344.z = 0.0f;
            }
            mPos.z += field_0x344.z;
            mPos.z += 0.1f * JMath::sincosTable_.sinShort((s16)(4096.0f * dx));
        } else {
            field_0x344.z -= 0.25f;
            if (field_0x344.z <= -4.0f) {
                field_0x344.z = -4.0f;
            }
            mPos.z += field_0x344.z;
            if (mPos.z <= 0.0f) {
                mPos.z = 0.0f;
                if (fn_80251AEC(0x12)) {
                    field_0xce8 = false;
                }
            }
        }
        field_0x2f4.set(0.0f, 0.0f, 18.0f);
        field_0x2f4 += mPos;
    } else {
        if (mPos.z >= 0.0f) {
            mPos += field_0x344;
        }
        if (field_0x43f || field_0x441) {
            if (field_0x344.z >= -6.0f) {
                field_0x344.z -= 0.17f;
                if (field_0x344.z < -6.0f) {
                    field_0x344.z = -6.0f;
                }
            }
        } else {
            field_0x344.z -= 0.25f;
        }
        if (mPos.z <= 0.0f) {
            if ((field_0x43f || field_0x441) && mPos.y < field_0x324.y) {
                mPos.z = 8.0f;
            } else {
                mPos.z = 0.0f;
                field_0x124d = 0;
                fn_80251AEC(0x12);
            }
        }
        fn_8026152C(false);
        if (field_0x440 && field_0x344.z < 0.0f) {
            fn_8024F0E8(0x1000D);
            fn_8024F0E8(0x2003E);
        }
    }
    if (field_0x344.z < -1.0f) {
        field_0x3f9 = 1;
    } else {
        field_0x3f9 = 0;
    }
}

void Player_804A6F38::fn_80262B8C() {
    fn_80283D2C();
}

void Player_804A6F38::fn_80262B6C() {
    fn_80259660();
}

// FIXME: 99.64. scheduling+regalloc, as fn_80258BE8 (tu_80256C74): the no-pad branch
// computes &field_0x344 (addi r3) before the x / y speed test in the original.
// Update of state 0x14, the jump while holding something (enter fn_80262234 runs the
// jump's exit fn_80258BBC, exit fn_80262B6C its enter fn_80259660): the steering, gravity
// and landing of the jump fn_80258BE8 (tu_80256C74; same structure), with the held
// thing at lbl_8046B670 once 0x3FB is set (fn_80245938), let go at frame 8. Landing
// (z <= 0 after 2 frames, or on a 2D layer over a tile other than 2 below) returns to
// 0x12 while lifting, else 0; the held thing sits 18 up while falling.
void Player_804A6F38::fn_80262254() {
    if (fn_8022C6AC(field_0x4) > 0.0f) {
        if (0.0f == field_0x344.x) {
            field_0x344.x = 0.8f * fn_8022C82C(field_0x4);
        } else if (field_0x344.x > 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.x += 0.3f * fn_8022C82C(field_0x4);
            } else {
                field_0x344.x += 0.2f * fn_8022C82C(field_0x4);
            }
            if (field_0x344.x <= 0.001f || field_0xbf0->field_0x48[0]) {
                field_0x344.x = 0.001f;
            }
        } else if (field_0x344.x < 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.x += 0.3f * fn_8022C82C(field_0x4);
            } else {
                field_0x344.x += 0.2f * fn_8022C82C(field_0x4);
            }
            if (field_0x344.x >= -0.001f || field_0xbf0->field_0x48[1]) {
                field_0x344.x = -0.001f;
            }
        }
        if (0.0f == field_0x344.y) {
            field_0x344.y = 0.8f * -fn_8022C76C(field_0x4);
        } else if (field_0x344.y > 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.y -= 0.3f * fn_8022C76C(field_0x4);
            } else {
                field_0x344.y -= 0.2f * fn_8022C76C(field_0x4);
            }
            if (field_0x344.y <= 0.001f || field_0xbf0->field_0x48[2]) {
                field_0x344.y = 0.001f;
            }
        } else if (field_0x344.y < 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.y -= 0.3f * fn_8022C76C(field_0x4);
            } else {
                field_0x344.y -= 0.2f * fn_8022C76C(field_0x4);
            }
            if (field_0x344.y >= -0.001f || field_0xbf0->field_0x48[3]) {
                field_0x344.y = -0.001f;
            }
        }
        f32 z = field_0x344.z;
        field_0x344.z = 0.0f;
        f32 length = field_0x344.length();
        f32 speed = length;
        if (length >= 1.6f) {
            speed = 1.6f;
        }
        field_0x344.normalize();
        field_0x344.scale(speed);
        field_0x344.z = z;
    } else if (!(field_0x344.x * field_0x344.x + field_0x344.y * field_0x344.y <= JGeometry::TUtilf::epsilon())) {
        f32 z = field_0x344.z;
        field_0x344.z = 0.0f;
        f32 speed = field_0x344.length();
        if (fn_8022C274(field_0x4)) {
            if (z >= 0.0f) {
                speed += 0.3f;
                if (speed >= 1.6f) {
                    speed = 1.6f;
                }
            } else {
                speed += 0.2f;
                if (speed >= 1.6f) {
                    speed = 1.6f;
                }
            }
        } else {
            speed -= 0.1f;
            if (speed <= 0.001f) {
                speed = 0.001f;
            }
        }
        field_0x344.normalize();
        field_0x344.scale(speed);
        field_0x344.z = z;
    }

    if (!field_0x344.isZero() || mPos.z > 0.0f) {
        if (field_0x3fa >= 1) {
            field_0x344.z -= 0.25f;
            if (field_0x344.z < -4.0f) {
                field_0x344.z = -4.0f;
            }
            if (viewLayerFlag(field_0x3b4)) {
                mPos.x += field_0x344.x;
                mPos.y -= field_0x344.z;
                mPos.z = 0.5f;
                if (field_0x344.z < 0.0f && field_0xbf0->field_0x48[2]) {
                    field_0x368.set(field_0x344.x, field_0x344.y, 0.0f);
                    field_0x344.set(0.0f, 0.0f, 0.0f);
                    mPos.z = 0.0f;
                }
            } else {
                mPos += field_0x344;
                if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
                    mPos.z = 0.0f;
                    field_0x368.set(field_0x344.x, field_0x344.y, 0.0f);
                    field_0x344.set(0.0f, 0.0f, 0.0f);
                }
            }
        }
    }

    if (fn_80245938()) {
        field_0x3fb = 1;
    }
    if (field_0x3fb == 1) {
        field_0x3fc++;
        if (field_0x3fc < 8) {
            field_0x3fd = 0;
        } else if (field_0x3fc < 9) {
            fn_8025F698(false);
            field_0x3fd = 1;
        } else if (field_0x3fc < 11) {
            field_0x3fd = 2;
        } else if (field_0x3fc < 21) {
            field_0x3fd = 3;
        } else if (field_0x3fc < 25) {
            field_0x3fd = 4;
        } else {
            field_0x3fd = 2;
        }
        const Vec& offset = lbl_8046B670[field_0x3fd + field_0x3dc * 5];
        field_0x2f4.set(offset.x, offset.y, offset.z);
        field_0x2f4 += mPos;
        fn_80268B3C();
    }

    if (!field_0xb79 && mPos.z > 0.0f && fn_802415CC() >= 2 && fn_8022C310(field_0x4)) {
        field_0x3fa = 0;
        fn_80259778();
        field_0xb79 = 1;
    }
    if (field_0x3fa >= 2 && mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        if (field_0xd64 == 1) {
            fn_80251AEC(0x12);
        } else {
            fn_80251AEC(0);
        }
    }
    if (field_0x3fa >= 8 && viewLayerFlag(field_0x3b4) && field_0x344.z < 0.0f && mPos.z <= 1.0f) {
        JGeometry::TVec3f below;
        fn_8024B298(2, &below);
        below.y += 2.0f;
        if (fn_8022461C(&below, field_0x3b4) != 2) {
            if (field_0xd64 == 1) {
                fn_80251AEC(0x12);
            } else {
                fn_80251AEC(0);
            }
        }
    }
    field_0x3fa++;
    fn_802685D4();
    field_0x2f4.set(0.0f, 0.0f, 18.0f);
    field_0x2f4 += mPos;
    if (field_0x344.z < -2.0f) {
        field_0x3f9 = 1;
    } else {
        field_0x3f9 = 0;
    }
}

void Player_804A6F38::fn_80262234() {
    fn_80258BBC();
}

void Player_804A6F38::fn_80262188() {
    field_0x458 = 0;
    if (field_0x1248 != 0x33 && field_0xcfe == 0) {
        fn_8024F0E8(0x10007);
    }
    if (field_0xd70 >= 0 && gGameRoot_8053AAF8->field_0x7e != 0 && !isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)
        && field_0xd60 < 0) {
        field_0x11ec = fn_8022F340(field_0xd70);
        field_0xd60 = fn_8022F340(field_0xd70);
    }
}

// FIXME: 99.65. One extra lwz: the original passes the field_0xd70 the inlined
// fn_8025F418 loaded straight to fn_8023EFEC; ours reloads it.
// Update of state 0x19. Grabbing a player: follow it (its position moved to this
// layer's view corner, raised 2; effect 0x28F once at height 10), let go when it
// stops lifting; grabbing an actor: stay at field_0x300. Otherwise the player flies
// along field_0xd78 (falling 0.25 a frame; held inside the view less 16) until it
// lands (state 0 / 1) or fn_802466B4 (state 7), bouncing off walls at half speed.
// The pose cycles over 36 frames.
void Player_804A6F38::fn_80261750() {
    if (fn_8025F448()) {
        if (fn_8025F418()) {
            if (!fn_8023EFEC(field_0xd70)) {
                if (mPos.z <= 4.0f) {
                    field_0xd78.set(0.0f, 0.0f, 0.0f);
                } else {
                    const Vec& speed = lbl_8046B760[field_0x3dc];
                    field_0xd78.set(speed.x, speed.y, speed.z);
                }
                fn_8024F0E8(0x20014);
                field_0xd70 = -1;
                field_0xd58 = 5;
            } else {
                if ((fn_8022D75C(field_0xd70) && !fn_80234ECC(field_0xd70)) || field_0x3b4 >= 8
                    || getPlayerLayer_8023DE58(field_0xd70) != 8) {
                    field_0x3b4 = getPlayerLayer_8023DE58(field_0xd70);
                }
                mPos = *fn_8023E7B0(field_0xd70);
                JGeometry::TBox2f* box = layerBox(getPlayerLayer_8023DE58(field_0xd70));
                f32 x = box->i.x;
                f32 y = box->i.y;
                box = layerBox(field_0x3b4);
                JGeometry::TVec2f offset(box->i.x - x, box->i.y - y);
                mPos.x += offset.x;
                mPos.y += offset.y;
                mPos.y += 2.0f;
                mPos.z += 2.0f;
                if (field_0x121c == 0 && !isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && !fn_80234BC0(field_0xd70)
                    && field_0xd60 < 0 && mPos.z >= 10.0f) {
                    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x28F, 1, 1, NULL, NULL, true, 1.0f);
                    field_0x121c = 1;
                }
                if (fn_8022D75C(field_0xd70) && !fn_80234ECC(field_0xd70)) {
                    if (!fn_80241E28(field_0x3b4, &mPos)) {
                        if (field_0x3b8 < 8) {
                            field_0xcb0 = field_0x14;
                        }
                        field_0xcc8 = field_0x14;
                        field_0xce0 = field_0x3b8;
                        field_0xcd4 = mPos;
                        field_0xce4 = field_0x3b4;
                        if (viewLayerFlag(field_0x3b4) && fn_802486A0() != 5 && fn_802486A0() != 4) {
                            fn_80248004(5, false);
                        }
                    }
                    if (!fn_8022BC38(field_0xd70, fn_8023E190(field_0xd70), fn_8023E2A0(field_0xd70))) {
                        field_0xcd4 = mPos;
                        field_0xcbc = field_0xcd4;
                        field_0xd1c = field_0xcbc;
                        field_0xcd4.z = 0.0f;
                        field_0xcbc.z = 0.0f;
                        field_0xd1c.z = 0.0f;
                    }
                    field_0x1216 = false;
                    fn_8024BAEC();
                    field_0xbec->fn_8030BD3C(field_0x4);
                }
                field_0x3e8 = field_0x3dc = fn_80230CFC(field_0xd70);
                if (!fn_8022D75C(field_0xd70)) {
                    if (!fn_80243194(field_0xd70) && fn_8022C5EC(field_0x4, 0xF) != 0) {
                        field_0x3fa++;
                        field_0x3fc += 6;
                    } else {
                        field_0x3fc++;
                    }
                    if (field_0x3fa > 16) {
                        fn_80231194(field_0xd70);
                    }
                }
            }
            if (field_0x3b4 < 8 && field_0xd70 >= 0 && getPlayerLayer_8023DE58(field_0xd70) > 8) {
                fn_80230C6C(field_0xd70);
            }
        } else {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0) {
                // Per-field copies: the original moves floats (lfs / stfs), not words.
                mPos.x = field_0x300.x;
                mPos.y = field_0x300.y;
                mPos.z = field_0x300.z;
                if (fn_8022C5EC(field_0x4, 0xF) != 0) {
                    field_0x3fc += 6;
                } else {
                    field_0x3fc++;
                }
            } else {
                field_0xd68 = -1;
                field_0xd78.set(0.0f, 0.0f, 0.0f);
            }
        }
        field_0xbf0->fn_8026E464();
        field_0xbf0->field_0xa0 = 0;
    } else {
        fn_8027221C(4);
        bool outside = false;
        mPos += field_0xd78;
        field_0xd78.z -= 0.25;
        JGeometry::TBox2f box;
        box = *layerBox(field_0x3b4);
        // The view box inset by 16 (the original subtracts the -16 from i, adds it to f).
        box.i.x -= -16.0f;
        box.i.y -= -16.0f;
        box.f.x += -16.0f;
        box.f.y += -16.0f;
        if (viewLayerFlag(field_0x3b4) && !contains(box, mPos)) {
            outside = true;
            mPos.z = 0.0f;
        }
        if (mPos.z <= 0.0f) {
            if (viewLayerFlag(field_0x3b4)) {
                if (outside || fn_802518AC(true)) {
                    mPos.z = 0.0f;
                } else {
                    mPos.y += 1.0f - mPos.z;
                    mPos.z = 1.0f;
                }
            }
            if (mPos.z <= 0.0f) {
                field_0x368.set(field_0xd78.x, field_0xd78.y, 0.0f);
                field_0xd78.set(0.0f, 0.0f, 0.0f);
                mPos.z = 0.0f;
                if (gGameRoot_8053AAF8->field_0x7e != 0 && !isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && field_0xd60 < 0) {
                    fn_80251AEC(1);
                } else {
                    fn_80251AEC(0);
                }
            }
        } else if (fn_802466B4() && fn_80251AEC(7)) {
            field_0x344.x = field_0xd78.x;
            field_0x344.y = field_0xd78.y;
            field_0x344.z = field_0xd78.z;
            field_0x368.set(field_0xd78.x, field_0xd78.y, 0.0f);
            field_0xd78.set(0.0f, 0.0f, 0.0f);
        }
        fn_802685D4();
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0xbf0->pushedAlongX()) {
                field_0xd78.x = -field_0xd78.x * 0.5f;
            }
        } else if (field_0xbf0->anyFlag48()) {
            field_0xd78.x = -field_0xd78.x * 0.5f;
            field_0xd78.y = -field_0xd78.y * 0.5f;
        }
    }
    if (field_0x3fc >= 36) {
        field_0x3fc = 0;
    }
    if (field_0x3fc < 6) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 12) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 18) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 24) {
        field_0x3fd = 2;
    } else if (field_0x3fc < 30) {
        field_0x3fd = 3;
    } else {
        field_0x3fd = 2;
    }
}

void Player_804A6F38::fn_8026173C() {
    field_0x121c = 0;
    field_0xd84 = 20;
}

// The carry walk cycle (field_0x3f8 over 24 frames, 48 when slow) and the held thing's
// position: pose lbl_8046B790[facing][step], offset lbl_8046B7B0[pose] from mPos.
void Player_804A6F38::fn_8026152C(bool slow) {
    if (epsilonEquals(mPos, field_0x14, JGeometry::TUtilf::epsilon()) && field_0x368.isZero()) {
        field_0x3f8 = 0;
    } else {
        field_0x3f8++;
        if (slow) {
            if (field_0x3f8 >= 48) {
                field_0x3f8 = 0;
            }
        } else if (field_0x3f8 >= 24) {
            field_0x3f8 = 0;
        }
    }
    if (slow) {
        field_0x3f9 = lbl_8046B790[field_0x3dc][field_0x3f8 / 6];
    } else {
        field_0x3f9 = lbl_8046B790[field_0x3dc][field_0x3f8 / 3];
    }
    const f32* offset = lbl_8046B7B0[field_0x3f9];
    field_0x2f4.set(offset[0], offset[1], offset[2]);
    field_0x2f4 += mPos;
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

void Player_804A6F38::fn_80261468() {
    if (field_0xd74 > 0) {
        field_0xd74--;
    }
}

// FIXME: 96.63. scheduling+regalloc: the facing-table vector loads (as in fn_8026022C) and
// the side probes' argument loads (player / layer before the x / y stores in the original).
// Pick up what lies on the tile in front (16 along the facing): a wall tile the
// background rewriter takes (fn_8029E888), else the tile's object, spawned and
// lifted at once (state 0x11): grass, stone, a disguised stone, sign, rock, pot or
// coconut by the tile kind fn_802A02A8 returns.
bool Player_804A6F38::fn_80260C08() {
    if (fn_8022B1F8()) {
        return false;
    }
    if (!fn_80251BFC(0x11)) {
        return false;
    }
    const Vec& d = lbl_8046B83C[field_0x3dc];
    JGeometry::TVec3f pos(d.x, d.y, d.z);
    pos += mPos;
    pos.z = 0.0f;
    if (fn_8022C1D8(field_0x4) || fn_8022C0A0(field_0x4)) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x3dc == 3 || field_0x3dc == 1 || field_0x3dc == 0) {
                if (gBgRewrite_8053AC98->fn_8029E888(&pos, field_0x4, field_0x3b4) == 1) {
                    return true;
                }
            }
        } else if (field_0x3dc == 3) {
            if (fn_80247330() < 0.0f) {
                JGeometry::TVec3f left(pos.x - 16.0f, pos.y, pos.z);
                if (gBgRewrite_8053AC98->fn_8029E888(&left, field_0x4, field_0x3b4) == 1) {
                    return true;
                }
            } else if (fn_80247330() > 0.0f) {
                JGeometry::TVec3f right(16.0f + pos.x, pos.y, pos.z);
                if (gBgRewrite_8053AC98->fn_8029E888(&right, field_0x4, field_0x3b4) == 1) {
                    return true;
                }
            }
            if (gBgRewrite_8053AC98->fn_8029E888(&pos, field_0x4, field_0x3b4) == 1) {
                return true;
            }
        } else if (field_0x3dc == 1 || field_0x3dc == 0) {
            if (fn_8024712C() > 0.0f) {
                JGeometry::TVec3f down(pos.x, pos.y - 16.0f, pos.z);
                if (gBgRewrite_8053AC98->fn_8029E888(&down, field_0x4, field_0x3b4) == 1) {
                    return true;
                }
            }
        }
    }
    if (fn_8022C1D8(field_0x4) && gBgRewrite_8053AC98->fn_802A01F0(&pos, field_0x4, field_0x3b4)) {
        return true;
    }
    if (fn_8022C0A0(field_0x4)) {
        s32 kind = gBgRewrite_8053AC98->fn_802A02A8(&pos, field_0x4, field_0x3b4);
        s32 cell = fn_80226AE0(&pos, field_0x3b4);
        switch (kind) {
        case 0:
        case 1:
        case 2:
        case 3: {
            s32 serial;
            if (kind == 3) {
                if (!getGameView_802D6A9C()->fn_802D6AC4(cell)) {
                    serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GRAS, field_0x3b4, &pos, 3, -1, field_0x4);
                    getGameView_802D6A9C()->fn_802D6B24(cell, true);
                } else {
                    serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GRAS, field_0x3b4, &pos, 4, -1, field_0x4);
                }
            } else {
                serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GRAS, field_0x3b4, &pos, kind, -1, field_0x4);
            }
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_GRAS;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        case 5: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_STON, field_0x3b4, &pos, 0, -1, field_0x4);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_STON;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        case 6: {
            s32 player = field_0x4;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_SSTN, field_0x3b4, &pos, player << 4, -1, player);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_SSTN;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        case 7: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BORD, field_0x3b4, &pos, 0, -1, field_0x4);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_BORD;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        case 8: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_ROCK, field_0x3b4, &pos, 0, -1, field_0x4);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_ROCK;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
                actor->mDrawEnabled = false;
            }
            return true;
        }
        case 12: {
            // A disguised stone, held as a rock.
            s32 player = field_0x4;
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_SSTN, field_0x3b4, &pos, (player << 4) + 1, -1, player);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_ROCK;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
                actor->mDrawEnabled = false;
            }
            return true;
        }
        case 39: {
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_YASI, field_0x3b4, &pos, 0, -1, field_0x4);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_YASI;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35: {
            s32 variant = 0;
            if (kind == 34) {
                variant = 1;
            } else if (kind == 35) {
                variant = 2;
            }
            s32 index = kind - 24;
            if (kind >= 29) {
                index = kind - 29;
            }
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TUBO, field_0x3b4, &pos, (variant << 7) | index, -1, field_0x4);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && fn_80251AEC(0x11)) {
                field_0xd6c = ACTOR_TUBO;
                field_0x2f4 = pos;
                field_0xd64 = 1;
                field_0xd68 = serial;
                actor->fn_801F3744(field_0x4);
            }
            return true;
        }
        }
    }
    return false;
}

// FIXME: 88.23. missing/extra (41 fewer): every call, constant and field is in place (the
// other rows are register numbers, one 0.5 reload and one beq/bne flip that follow the
// missing code). The original builds the layer-moved points in stack TVec2 temporaries
// (the my-layer offset at sp 0x18 / 0x50, its frsp'd copy at 0x38, the probe at 0x58;
// 0x10 / 0x40 / 0x08 / 0x48 / 0x30 / 0x28 for the midpoint): TVec2 operator+ / operator-
// style helpers that neither this tree nor the tp donor's JGeometry has; ours keeps them
// in registers. Kept for the .sdata2 order (first 0.5).
// Lift another player standing in front (16 along the facing, state 0x11, 'PLAY'): it
// must be present and free, and both its spot seen from this layer and the midpoint of
// the two (on both layers) must not be blocked (fn_802260DC / fn_802684A4 unless tile
// 0x20); in a group or across layers the other player's fn_80231230 / fn_802312CC.
bool Player_804A6F38::fn_802604CC() {
    if (fn_8022B1F8()) {
        return false;
    }
    if (fn_8022AF10()) {
        return false;
    }
    if (fn_802486A0() != 4 && fn_80240808()) {
        return false;
    }
    if (gGameRoot_8053AAF8->field_0x7e == 0 && fn_802486A0() != 4) {
        return false;
    }
    if (fn_8022C0A0(field_0x4)) {
        const Vec& a = lbl_8046B86C[field_0x3dc];
        JGeometry::TVec3f ahead(a.x, a.y, a.z);
        ahead += mPos;
        ahead.z = 0.0f;
        for (int i = 0; i < 4; i++) {
            if (i == field_0x4 || !fn_8023F264(i) || fn_8023EFEC(i) || fn_8023EA28(i) || fn_80233DA4(i)
                || fn_80230188(i) || fn_80236AB4(i) || fn_802354E4(i) || fn_8022D894(i)) {
                continue;
            }
            if (field_0x3b4 == 8 && getPlayerLayer_8023DE58(i) < 8 && fn_8022CAF8(i) != 4 && fn_8022F514(i)) {
                continue;
            }
            if (fn_802486A0() != 4 && fn_8022E438(field_0x4, i) && fn_80246F28() > 0.0f) {
                continue;
            }
            // This player's position on the other player's layer.
            JGeometry::TBox2f* box = layerBox(getPlayerLayer_8023DE58(i));
            JGeometry::TBox2f* myBox = layerBox(field_0x3b4);
            JGeometry::TVec2f pos(mPos.x - myBox->i.x + box->i.x, mPos.y - myBox->i.y + box->i.y);
            if ((fn_802260DC(&pos, getPlayerLayer_8023DE58(i), true)
                 || fn_802684A4((const JGeometry::TVec3f*)&pos, getPlayerLayer_8023DE58(i)))
                && fn_802265FC((const JGeometry::TVec3f*)&pos, getPlayerLayer_8023DE58(i)) != 0x20) {
                continue;
            }
            // The midpoint of the two players, relative to the view corners.
            box = layerBox(getPlayerLayer_8023DE58(i));
            JGeometry::TVec3f* other = getPlayerPos_8023E724(i);
            myBox = layerBox(field_0x3b4);
            JGeometry::TVec2f mid((mPos.x - myBox->i.x + other->x - box->i.x) * 0.5f,
                                  (mPos.y - myBox->i.y + other->y - box->i.y) * 0.5f);
            box = layerBox(getPlayerLayer_8023DE58(i));
            pos.set(mid.x + box->i.x, mid.y + box->i.y);
            if ((fn_802260DC(&pos, getPlayerLayer_8023DE58(i), true)
                 || fn_802684A4((const JGeometry::TVec3f*)&pos, getPlayerLayer_8023DE58(i)))
                && fn_802265FC((const JGeometry::TVec3f*)&pos, getPlayerLayer_8023DE58(i)) != 0x20) {
                continue;
            }
            if (field_0x3b4 != getPlayerLayer_8023DE58(i)) {
                myBox = layerBox(field_0x3b4);
                pos.set(mid.x + myBox->i.x, mid.y + myBox->i.y);
                if ((fn_802260DC(&pos, field_0x3b4, true) || fn_802684A4((const JGeometry::TVec3f*)&pos, field_0x3b4))
                    && fn_802265FC((const JGeometry::TVec3f*)&pos, getPlayerLayer_8023DE58(i)) != 0x20) {
                    continue;
                }
            }
            JGeometry::TBox2f at(ahead.x, ahead.y, ahead.x, ahead.y);
            if (!fn_80239C9C(i, &at, field_0x3b4)) {
                continue;
            }
            if (isPlayerBitSet(gGameRoot_8053AAF8, i) || fn_8022E438(field_0x4, i)
                || (gGameRoot_8053AAF8->field_0x7e != 0 && fn_8022CAF8(i) != 4)
                || (field_0x3b4 == 8 && getPlayerLayer_8023DE58(i) < 8)) {
                if ((fn_80236B50(i) || fn_80251BFC(0x19)) && fn_80251AEC(0x11)) {
                    if (fn_80236B50(i)) {
                        fn_80231230(i, field_0x4);
                    } else {
                        fn_802312CC(i, field_0x4);
                    }
                    field_0x2f4 = ahead;
                    field_0xd64 = 1;
                    field_0xd70 = i;
                    field_0xd6c = 'PLAY';
                    fn_8024F0E8(0x20022);
                    fn_802702A4();
                    return true;
                }
            } else {
                if (fn_80236B50(i)) {
                    if (fn_80251AEC(0x11)) {
                        fn_80231230(i, field_0x4);
                        field_0x2f4 = ahead;
                        field_0xd64 = 1;
                        field_0xd70 = i;
                        field_0xd6c = 'PLAY';
                        fn_8024F0E8(0x20022);
                        fn_802702A4();
                    }
                } else {
                    fn_802312CC(i, field_0x4);
                }
                return true;
            }
        }
    }
    return false;
}

// FIXME: 95.46. scheduling+regalloc: the two facing-table vectors are loaded x, y, z and
// stored interleaved with the mPos add in the original (ours loads z first); the argument
// loads of fn_80272B4C's copy differ in order.
// The lift / push start (from fn_80261480): with a wall behind (fn_802265DC / fn_802684A4
// at 10 against the facing) or fn_8024B24C, lift (0x11, nothing held yet) when
// fn_80246F28 is 0, else 0x4F; with the tile 9.999 ahead blocked, push (0x1A). In state
// 0x4F (fn_8022C004) push through fn_8025190C / fn_802517F8 or fn_8025197C.
void Player_804A6F38::fn_8026022C() {
    const Vec& a = lbl_8046B89C[field_0x3dc];
    JGeometry::TVec3f ahead(a.x, a.y, a.z);
    ahead += mPos;
    ahead.z = 0.0f;
    const Vec& b = lbl_8046B8CC[field_0x3dc];
    JGeometry::TVec3f behind(b.x, b.y, b.z);
    behind += mPos;
    if (field_0x1174 == 0 && field_0xd64 == 0 && field_0xd74 == 0) {
        if (fn_8022BF68(field_0x4)) {
            if (!fn_802265DC(&behind, field_0x3b4, true) && !fn_802684A4(&behind, field_0x3b4) && !fn_8024B24C()
                // fn_80272B4C gets a copy of ahead (the original copies it to sp 0x8 first).
            && (fn_802265DC(&ahead, field_0x3b4, true) || fn_80272B4C(&JGeometry::TVec3f(ahead)))) {
                fn_80251AEC(0x1A);
            } else if (fn_80246F28() == 0.0f) {
                if (!fn_8022B1F8() && fn_80251AEC(0x11)) {
                    field_0xd64 = 1;
                    field_0xd68 = -1;
                    field_0xd6c = 'NULL';
                }
            } else if (!fn_8022B244() || fn_802416C4() == 5) {
                fn_80251AEC(0x4F);
            }
        } else if (field_0x1240 == 0x4F && fn_8022C004(field_0x4) && !fn_8024B24C()) {
            if (viewLayerFlag(field_0x3b4)) {
                if (fn_8025190C(true) || fn_802517F8(false)) {
                    fn_80251AEC(0x1A);
                }
            } else if (fn_8025197C(true)) {
                fn_80251AEC(0x1A);
            }
        }
    }
}

// Grab another player: lift it (state 0x19) when it can't take part in a pair
// ('TSNO', "two-sword motion", docs/names/actors.tsv), else pair up with it.
bool Player_804A6F38::fn_802600A0(s32 player) {
    if (fn_80270A8C()) {
        return false;
    }
    if (!isPlayerBitSet(gGameRoot_8053AAF8, field_0x4) && !fn_8022E438(field_0x4, player)
        && (gGameRoot_8053AAF8->field_0x7e == 0 || fn_802486A0() != 4)
        && (getPlayerLayer_8023DE58(player) != 8 || field_0x3b4 >= 8)) {
        field_0xd60 = fn_8022F340(player);
        field_0x11ec = fn_8022F340(player);
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TSNO, field_0x3b4, &mPos, (player & 3) | (field_0x4 << 2), -1, player);
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

bool Player_804A6F38::fn_80260048(s32 arg) {
    if (!fn_80270A8C()) {
        return false;
    }
    field_0xd70 = arg;
    fn_80248FF0();
    return true;
}

bool Player_804A6F38::fn_8026000C() {
    fn_8025F698(false);
    return fn_80251AEC(0);
}

bool Player_804A6F38::fn_8025FF70() {
    if (!fn_8025F590()) {
        return false;
    }
    fn_80251AEC(0);
    field_0xd64 = 0;
    field_0xd70 = -1;
    field_0xd6c = 'NULL';
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

// FIXME: 99.10. Registers only: the float temporaries of the view-corner offset.
// Let go of the held player: take its position, moved from its layer's view corner
// to this player's, and remember pos as the drop point.
bool Player_804A6F38::fn_8025FDD0(const JGeometry::TVec3f* pos) {
    mPos = *fn_8023E7B0(field_0xd70);
    JGeometry::TBox2f* box = layerBox(getPlayerLayer_8023DE58(field_0xd70));
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

// FIXME: 87.02. Two differences: the original tests state 0x11 on its own before the folded
// range 0x12-0x16 (ours folds 0x11-0x16), and it recomputes the field_0xd64 == 1 byte
// (subfic / cntlzw / extrwi) inside each inlined fn_8025F590 where ours reuses it.
// Whether the requested state makes the player let go of what it holds (true unless the
// state is one of the lift / carry / throw states 0x11-0x18, 0x1B-0x1D, a screen change
// 0x20-0x25 while holding something other than a player, 0x1E / 0x1F holding a small
// key, or 0x4C / 0x4D while holding an actor). fn_8025F698 inlines it.
bool Player_804A6F38::fn_8025FCC8() {
    s32 state = field_0x1244;
    if (state == 0x11 || state == 0x12 || state == 0x13 || state == 0x14 || state == 0x15 || state == 0x16
        || state == 0x18 || state == 0x1B || state == 0x1C || state == 0x1D
        || (isField0xd64One(*this) && !fn_8025F590()
            && (state == 0x20 || state == 0x21 || state == 0x22 || state == 0x23 || state == 0x24 || state == 0x25))
        || (field_0xd6c == ACTOR_KEY0 && (state == 0x1E || state == 0x1F))
        || (field_0xd64 == 1 && !fn_8025F590() && (state == 0x4C || state == 0x4D))) {
        return false;
    }
    return true;
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
    if (field_0xd64 != 0 && (!checkState || fn_8025FCC8())) {
        bool held = true;
        if (field_0xd6c == 'NULL') {
            held = false;
        }
        if (fn_8025F590()) {
            JGeometry::TVec3f speed;
            JGeometry::TVec3f step;
            const Vec& dir = lbl_8046B8FC[field_0x3dc];
            step.set(dir.x, dir.y, dir.z);
            step.scale(16.0f);
            if (field_0x1244 == 0x1E || field_0x1244 == 0x32 || fn_8024189C() <= 0) {
                speed.set(0.0f, 0.0f, 0.0f);
            } else {
                JGeometry::TVec3f pos(*getPlayerPos_8023E724(field_0xd70));
                int walls = 0;
                for (int i = 0; i < 3; i++) {
                    pos += step;
                    u16 tile = fn_8022461C(&pos, getPlayerLayer_8023DE58(field_0xd70));
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
                    JGeometry::TVec3f diff = pos - *getPlayerPos_8023E724(field_0xd70);
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
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
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
        if (held == true && fn_802486A0() != 4 && isPlayerBitSet(gGameRoot_8053AAF8, field_0x4)) {
            for (int i = 0; i < 4; i++) {
                if (fn_8022E438(field_0x4, i) && i != field_0x4) {
                    s32 serial = fn_8023EF50(i);
                    ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (other != 0 && other->field_0x1a4 <= 0) {
                        fn_80230C6C(i);
                    }
                }
            }
        }
    }
}

// Lifting something (field_0xd64 == 1).
bool Player_804A6F38::fn_8025F684() {
    return isField0xd64One(*this);
}

s32 Player_804A6F38::fn_8025F668() {
    if (field_0xd64 == 1) {
        return field_0xd68;
    }
    return -1;
}

s32 Player_804A6F38::fn_8025F648() {
    if (field_0xd64 == 1) {
        return field_0xd6c;
    }
    return 'NULL';
}

void Player_804A6F38::fn_8025F5CC() {
    if (field_0xd64 != 0) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0) {
                actor->mAlive = false;
                field_0xd6c = 'NULL';
                field_0xd64 = 0;
                field_0xd68 = -1;
            }
        }
    }
}

// Holding a player: field_0xd64 == 1 with a player index in field_0xd70 (inlined by most
// functions of the unit).
bool Player_804A6F38::fn_8025F590() {
    if (isField0xd64One(*this) == 1 && field_0xd70 >= 0 && field_0xd70 < 4) {
        return true;
    }
    return false;
}

s32 Player_804A6F38::fn_8025F55C() {
    if (isField0xd64One(*this) == 1 && field_0xd70 >= 0 && field_0xd70 < 4) {
        return field_0xd70;
    }
    return -1;
}

s32 Player_804A6F38::fn_8025F534() {
    if (field_0xd64 == 1 || field_0xd70 < 0 || field_0xd70 >= 4) {
        return -1;
    }
    return field_0xd70;
}

bool Player_804A6F38::fn_8025F500() {
    if (isField0xd64One(*this) == 1 && field_0xd68 >= 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025F484() const {
    if (isField0xd64One(*this) == 1) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != 0 && actor->field_0x1a4 > 0) {
                return true;
            }
            return false;
        }
    }
    return false;
}

// Holding a player (field_0xd70) or an actor (field_0xd68) without lifting it
// (field_0xd64 == 0), i.e. grabbing it. fn_8025EE48, fn_8025EFFC and fn_80261750 inline it.
bool Player_804A6F38::fn_8025F448() {
    if ((field_0xd70 >= 0 && field_0xd70 < 4 || field_0xd68 >= 0) && field_0xd64 == 0) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025F418() {
    if (field_0xd70 >= 0 && field_0xd70 < 4 && field_0xd64 == 0) {
        return true;
    }
    return false;
}

// FIXME: 96.81. The materialised field_0xd64 == 1 test, as in fn_8025EE48.
// Whether the player may pick something up (not down, holding, grabbing, busy (0xCE8 /
// 0x454), in the air, or a follower in mode 5 without a pending 0xD88). fn_8025EFFC
// inlines it.
bool Player_804A6F38::fn_8025F2CC() {
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_8025F684()) {
        return false;
    }
    if (fn_8025F448()) {
        return false;
    }
    if (fn_8024BBB0()) {
        return false;
    }
    if (fn_80241230()) {
        return false;
    }
    if (fn_802412A4()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (field_0xd88 < 0 && fn_80240808() && fn_802486A0() == 5) {
        return false;
    }
    return true;
}

// FIXME: 95.94. The materialised field_0xd64 == 1 test twice, as in fn_8025EE48.
// Lift the actor `serial` (state 0x11, or 0x12 with lift false). With grabOnly, a
// player already holding or grabbing only claims it through fn_8025EF68.
bool Player_804A6F38::fn_8025EFFC(s32 serial, bool lift, bool grabOnly) {
    if (grabOnly) {
        if (fn_8025F684() || fn_8025F448() || fn_80241230() || fn_802412A4()) {
            fn_8025EF68(serial);
            return true;
        }
    } else if (!fn_8025F2CC()) {
        return false;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
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

bool Player_804A6F38::fn_8025EF68(s32 arg) {
    if (field_0xd88 == arg) {
        return true;
    }
    if (field_0xd88 >= 0) {
        return false;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(arg);
    if (actor != 0) {
        if (actor->fn_801F36D4(field_0x4)) {
            field_0xd88 = arg;
            return true;
        }
        return false;
    }
    return false;
}

// FIXME: 96.32. The lifting test: the original materialises field_0xd64 == 1 (subfic /
// cntlzw / extrwi.) and branches on it; ours folds it to cmplwi 1 (with `== true`, 98.47 %,
// one cmplwi too many). Same in fn_8025EFFC and fn_8025F2CC.
// Grab the actor `serial` (state 0x19).
bool Player_804A6F38::fn_8025EE48(s32 serial) {
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_8025F684()) {
        return false;
    }
    if (fn_8025F448()) {
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

bool Player_804A6F38::fn_8025EE34(s32 arg) {
    return field_0xd68 == arg;
}

void Player_804A6F38::fn_8025EE0C(s32 a, const JGeometry::TVec3f* b) {
    if (field_0xd68 == a) {
        field_0x300.set(b->x, b->y, b->z);
    }
}

bool Player_804A6F38::fn_8025EDD4(s32 a, const JGeometry::TVec3f* pos) {
    if (field_0xd68 == a) {
        field_0xd68 = -1;
        field_0xd78 = *pos;
        return true;
    }
    return false;
}

// FIXME: 99.71. Registers only: the four float temporaries of the view-corner offset
// (f0-f3) are assigned differently.
// Whether the held thing can be put down at field_0x2f4: a player held across from
// a GBA layer (8) is checked on its own layer, with the point moved between the two
// view corners; else the tile there, and for an actor its vfunc_0x4C. On success the
// drop point takes the player's x / y.
bool Player_804A6F38::fn_8025EB18() {
    if (fn_8025F590() && field_0x3b4 == 8 && getPlayerLayer_8023DE58(field_0xd70) < 8) {
        JGeometry::TVec3f pos(field_0x2f4);
        JGeometry::TBox2f* box = layerBox(getPlayerLayer_8023DE58(field_0xd70));
        f32 x = box->i.x;
        f32 y = box->i.y;
        box = layerBox(field_0x3b4);
        JGeometry::TVec2f offset(x - box->i.x, y - box->i.y);
        pos.x += offset.x;
        pos.y += offset.y;
        u16 tile = fn_8022461C(&pos, getPlayerLayer_8023DE58(field_0xd70));
        if ((fn_802265DC(&pos, getPlayerLayer_8023DE58(field_0xd70), true) && tile != 0xE && tile != 0x39 && tile != 0xD)
            || fn_802684A4(&pos, getPlayerLayer_8023DE58(field_0xd70))) {
            field_0x2f4.x = mPos.x;
            field_0x2f4.y = mPos.y;
            return true;
        }
    } else {
        u16 tile = fn_8022461C(&field_0x2f4, field_0x3b4);
        if (fn_8025F590()) {
            if ((fn_802265DC(&field_0x2f4, field_0x3b4, true) && tile != 0xE && tile != 0x39 && tile != 0xD)
                || fn_802684A4(&field_0x2f4, field_0x3b4)) {
                field_0x2f4.x = mPos.x;
                field_0x2f4.y = mPos.y;
                return true;
            }
        } else {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
            if (actor != 0 && actor->vfunc_0x4C(field_0x3b4, &field_0x2f4)) {
                field_0x2f4.x = mPos.x;
                field_0x2f4.y = mPos.y;
                return true;
            }
        }
    }
    return false;
}
