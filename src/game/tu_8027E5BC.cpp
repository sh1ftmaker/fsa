#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/BoxTest.h"
#include "game/Map.h"
#include "game/tu_8030AE84.h"
#include "game/Player.h"
#include "game/VecEpsilon.h"
#include "game/ViewLayer.h"
#include "game/cls_804A71A8.h"
#include "game/EmitterDelete.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_cannon.h"
#include "game/act/tu_801F9484.h"

// Directions by facing (right, left, down, up) and the pad bit of each facing; lbl_8046BDA8 and
// lbl_8046BE18 hold the same steps. fn_80284978 addresses lbl_8046BE08 / lbl_8046BE18 /
// lbl_8046BE48 off one base (MWCC shares it between the file's tables).
static const Vec lbl_8046BDA8[4] = {
    {10.0f, 0.0f, 0.0f},
    {-10.0f, 0.0f, 0.0f},
    {0.0f, 10.0f, 0.0f},
    {0.0f, -10.0f, 0.0f},
};
static const Vec lbl_8046BDD8[4] = {
    {12.0f, 0.0f, 0.0f},
    {-12.0f, 0.0f, 0.0f},
    {0.0f, 12.0f, 0.0f},
    {0.0f, -12.0f, 0.0f},
};
static const u32 lbl_8046BE08[4] = {2, 1, 4, 8};
static const Vec lbl_8046BE18[4] = {
    {10.0f, 0.0f, 0.0f},
    {-10.0f, 0.0f, 0.0f},
    {0.0f, 10.0f, 0.0f},
    {0.0f, -10.0f, 0.0f},
};
static const Vec lbl_8046BE48[4] = {
    {-10.0f, 0.0f, 0.0f},
    {10.0f, 0.0f, 0.0f},
    {0.0f, -10.0f, 0.0f},
    {0.0f, 10.0f, 0.0f},
};
// Speeds along the facing (right, left, down, up): with 3 up for fn_80281760, flat for fn_80281D50.
static const Vec lbl_8046BE78[4] = {
    {2.0f, 0.0f, 0.0f},
    {-2.0f, 0.0f, 0.0f},
    {0.0f, 2.0f, 0.0f},
    {0.0f, -2.0f, 0.0f},
};
static const Vec lbl_8046BEA8[4] = {
    {2.0f, 0.0f, 3.0f},
    {-2.0f, 0.0f, 3.0f},
    {0.0f, 2.0f, 3.0f},
    {0.0f, -2.0f, 3.0f},
};
// Unit directions by 0x3E8 (right, left, down, up, then the diagonals 4-7): fn_802804E0.
static const Vec lbl_8046BED8[8] = {
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.707f, 0.707f, 0.0f},
    {-0.707f, 0.707f, 0.0f},
    {0.707f, -0.707f, 0.0f},
    {-0.707f, -0.707f, 0.0f},
};
// The same directions for fn_80280074.
static const Vec lbl_8046BF38[8] = {
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
    {0.707f, 0.707f, 0.0f},
    {-0.707f, 0.707f, 0.0f},
    {0.707f, -0.707f, 0.0f},
    {-0.707f, -0.707f, 0.0f},
};
// Offsets by [player][this player's index] (x, y), for fn_802486A0() == 3 and 2.
static const f32 lbl_8046BF98[4][4][2] = {
    {{0.0f, 0.0f}, {-16.0f, 16.0f}, {16.0f, 16.0f}, {0.0f, 32.0f}},
    {{16.0f, -16.0f}, {0.0f, 0.0f}, {32.0f, 0.0f}, {16.0f, 16.0f}},
    {{-16.0f, -16.0f}, {-32.0f, 0.0f}, {0.0f, 0.0f}, {-16.0f, 16.0f}},
    {{0.0f, -32.0f}, {-16.0f, -16.0f}, {16.0f, -16.0f}, {0.0f, 0.0f}},
};
static const f32 lbl_8046C018[4][4][2] = {
    {{0.0f, 0.0f}, {16.0f, 0.0f}, {0.0f, 16.0f}, {16.0f, 16.0f}},
    {{-16.0f, 0.0f}, {0.0f, 0.0f}, {-16.0f, 16.0f}, {0.0f, 16.0f}},
    {{0.0f, -16.0f}, {16.0f, -16.0f}, {0.0f, 0.0f}, {16.0f, 0.0f}},
    {{-16.0f, -16.0f}, {0.0f, -16.0f}, {-16.0f, 0.0f}, {0.0f, 0.0f}},
};
// The frames 0x3F9 of state 0x0E by facing, per 8 steps of the cycle 0x3F8.
static const u8 lbl_8046C098[4][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 4, 5, 6},
    {0, 1, 2, 3, 0, 4, 5, 6},
};
// The facing of each player (by index) in the four-player formation: on view-flag layers / elsewhere.
static const s32 lbl_8046C0B8[4] = {2, 1, 0, 3};
static const s32 lbl_8046C0C8[4] = {3, 1, 0, 2};

// Whether p (x, y) is in box: fn_802844F8's test (its registers need the point by reference and
// the plain && return, not BoxTest.h's containsPoint).
static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

void Player_804A6F38::fn_8027E5BC(f32 speed) {
    // The divides by powers of two compile to the original's multiplies (operand order of fmuls/fmadds).
    if (!fn_8024BBB0()) {
        speed = speed * magnitude(field_0x3a4 - field_0x398) / 64.0f;
    }
    if (field_0x3a4.x > field_0x398.x) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
        mPos.x += speed;
        if (mPos.x > field_0x3a4.x) {
            mPos.x = field_0x3a4.x;
        }
    } else if (field_0x3a4.x < field_0x398.x) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
        mPos.x -= speed;
        if (mPos.x < field_0x3a4.x) {
            mPos.x = field_0x3a4.x;
        }
    }
    if (field_0x3a4.y > field_0x398.y) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
        if (fn_8024BBB0() && (field_0xcf4 == 0x15 || field_0xcf4 == 0x23) && viewLayerFlag(field_0x3b4)) {
            mPos.y += speed / 16.0f;
        } else {
            mPos.y += speed;
        }
        if (mPos.y > field_0x3a4.y) {
            mPos.y = field_0x3a4.y;
        }
    } else if (field_0x3a4.y < field_0x398.y) {
        field_0x3dc = 3;
        field_0x3e8 = 3;
        if (fn_8024BBB0() && (field_0xcf4 == 0x15 || field_0xcf4 == 0x23) && viewLayerFlag(field_0x3b4)) {
            mPos.y -= speed / 16.0f;
        } else {
            mPos.y -= speed;
        }
        if (mPos.y < field_0x3a4.y) {
            mPos.y = field_0x3a4.y;
        }
    }
}

// FIXME: 98.85. Same instructions; the original reads field_0x3b4 into r30 before loading field_0x11fc for the getPlayerLayer_8023DE58 call, ours loads it after the call.
void Player_804A6F38::fn_8027E7E0() {
    if (field_0x11fc >= 0) {
        if (fn_80235580(field_0x11fc) || field_0x3b4 != getPlayerLayer_8023DE58(field_0x11fc) ||
            magnitude(*getPlayerPos_8023E724(field_0x11fc) - mPos) > 60.0f) {
            field_0x11fc = -1;
        }
    }
    f32 nearest;
    if (field_0x11fc >= 0) {
        nearest = magnitude(*getPlayerPos_8023E724(field_0x11fc) - mPos);
    } else {
        nearest = 60.0f;
    }
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4) {
            continue;
        }
        if (fn_802486A0() == 4) {
            s32 other = fn_80248778();
            if (fn_80240888() != other) {
                continue;
            }
        }
        if (magnitude(*getPlayerPos_8023E724(i) - mPos) < nearest - 10.0f) {
            field_0x11fc = i;
            nearest = magnitude(*getPlayerPos_8023E724(i) - mPos);
        }
    }
    if (field_0x11fc < 0) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    } else {
        JGeometry::TVec3f diff = *getPlayerPos_8023E724(field_0x11fc) - mPos;
        s16 angle = JMath::atanTable_.atan2_(diff.y, diff.x) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
        if (angle < -0x6000) {
            field_0x3dc = 1;
            field_0x3e8 = 1;
        } else if (angle < -0x2000) {
            field_0x3dc = 3;
            field_0x3e8 = 3;
        } else if (angle < 0x2000) {
            field_0x3dc = 0;
            field_0x3e8 = 0;
        } else if (angle < 0x6000) {
            field_0x3dc = 2;
            field_0x3e8 = 2;
        } else {
            field_0x3dc = 1;
            field_0x3e8 = 1;
        }
    }
}

// The facing (0x3DC) and the walk direction (0x3E8): in a group from the formation or the leader,
// else from the pad directions (fn_80246B10: 1 left, 2 right, 4 down, 8 up; 4-7 are diagonals).
void Player_804A6F38::fn_8027EB04() {
    if (!fn_80241230() && !fn_802412A4() && !field_0x1214) {
        if (fn_802486A0() != 4 && fn_80240808() && fn_8022D75C(fn_80240644())) {
            return;
        }
        if (fn_802486A0() == 3 && fn_80248970() >= 2) {
            if (viewLayerFlag(field_0x3b4)) {
                s32 facing = lbl_8046C0B8[field_0x4];
                field_0x3dc = facing;
                field_0x3e8 = facing;
            } else {
                s32 facing = lbl_8046C0C8[field_0x4];
                field_0x3dc = facing;
                field_0x3e8 = facing;
            }
            return;
        }
        if (fn_80240808() && fn_802486A0() != 5 && fn_802486A0() != 4) {
            field_0x3dc = fn_80230CFC(fn_80240644());
            field_0x3e8 = fn_80230B70(fn_80240644());
            return;
        }
    }
    if (fn_80246B10(2)) {
        if (fn_80246B10(8)) {
            if (field_0x3dc != 0 && field_0x3dc != 3) {
                field_0x3dc = 0;
            }
            field_0x3e8 = 6;
        } else if (fn_80246B10(4)) {
            if (field_0x3dc != 0 && field_0x3dc != 2) {
                field_0x3dc = 0;
            }
            field_0x3e8 = 4;
        } else {
            field_0x3dc = 0;
            field_0x3e8 = 0;
        }
    } else if (fn_80246B10(1)) {
        if (fn_80246B10(8)) {
            if (field_0x3dc != 1 && field_0x3dc != 3) {
                field_0x3dc = 1;
            }
            field_0x3e8 = 7;
        } else if (fn_80246B10(4)) {
            if (field_0x3dc != 1 && field_0x3dc != 2) {
                field_0x3dc = 1;
            }
            field_0x3e8 = 5;
        } else {
            field_0x3dc = 1;
            field_0x3e8 = 1;
        }
    } else if (fn_80246B10(8)) {
        field_0x3dc = 3;
            field_0x3e8 = 3;
    } else if (fn_80246B10(4)) {
        field_0x3dc = 2;
            field_0x3e8 = 2;
    }
}

void Player_804A6F38::fn_8027EDDC() {
    if (field_0x3fc < 8) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 16) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 24) {
        field_0x3fd = 2;
    } else if (field_0x3fc < 30) {
        field_0x3fd = 3;
    } else if (field_0x3fc < 36) {
        field_0x3fd = 4;
    } else if (field_0x3fc < 42) {
        field_0x3fd = 5;
    } else if (field_0x3fc < 48) {
        field_0x3fd = 4;
    } else {
        field_0x3fd = 3;
    }
    field_0x3fc++;
}

void Player_804A6F38::fn_8027EE84() {
    field_0x3f8++;
    if (field_0x3f8 >= 16) {
        field_0x3f8 = 0;
    }
    field_0x3f9 = field_0x3f8 / 2;
}

// FIXME: 99.31. regalloc only: in the epsilon compare the original loads __float_epsilon into f0 before the
// 32.0f constant (f2) and keeps the difference in f0 / -epsilon in f3; ours uses f1-f4. In the 0.0f test the
// fcmpu operands are swapped (f1, f0 in the original).
void Player_804A6F38::fn_8027EEB4() {
    if ((!fn_80240808() || !fn_80234ECC(fn_80240644())) && fn_80250D30()) {
        fn_80251AEC(13);
    } else if (field_0x1240 != 3 && field_0xbf0->field_0xb7 != 0 && field_0x368.isZero() &&
               field_0xbf0->field_0xa0 == 0) {
        fn_80251AEC(0);
        field_0x3f8 = 0;
    } else if (field_0xbf0->field_0xb7 != 0 && fn_8022C6AC(field_0x4) == 0.0f && field_0xbf0->field_0xa0 == 0) {
        field_0x3f8 = 0;
    } else if (field_0xbf0->field_0xb7 == 0 &&
               epsilonEquals(mPos, field_0x14, JGeometry::TUtilf::epsilon()) &&
               field_0xbf0->field_0xa0 == 0 && !field_0x434) {
        fn_80251AEC(0);
        field_0x3f8 = 0;
    } else {
        if (field_0xbf0->field_0xb7 != 0) {
            if (field_0x3f8 % 3 == 0) {
                field_0x3f8 += 2;
            } else {
                field_0x3f8 += 1;
            }
        } else {
            field_0x3f8++;
        }
        if (field_0x3f8 >= 24) {
            field_0x3f8 = 0;
        }
    }
    field_0x3f9 = field_0x3f8 / 3;
}

void Player_804A6F38::fn_8027F16C() {
    if (!fn_80240808() && !fn_80245FEC() && field_0x1201 != 1) {
        fn_802416FC(0x180);
    }
}

void Player_804A6F38::fn_8027F1C8() {
    if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << field_0x4))) {
        return;
    }
    if (fn_802413E8()) {
        return;
    }
    if (!fn_802341D0(field_0x4) || fn_8024BBB0()) {
        deleteEmitter(&field_0x1388);
        return;
    }
    JGeometry::TVec3f pos;
    pos.set(mPos);
    pos.z = 0.0f;
    switch (fn_802486A0()) {
    case 0:
        pos.x += 8.0f * (fn_802487FC() - 1);
        break;
    case 1:
        pos.y += 8.0f * (fn_802487FC() - 1);
        break;
    case 2:
        pos.x += 8.0f;
        pos.y += 8.0f;
        break;
    case 3:
        if (fn_80241284()) {
            pos.set(field_0x11b4);
        } else {
            switch (field_0x4) {
            case 0:
                pos.y += 16.0f;
                break;
            case 1:
                pos.x += 16.0f;
                break;
            case 2:
                pos.x -= 16.0f;
                break;
            case 3:
                pos.y -= 16.0f;
                break;
            }
        }
        break;
    }
    if (!field_0x121d) {
        if (!gGameRoot_8053AAF8->field_0x7e) {
            if (field_0x1388 != NULL) {
                setEmitterPos_8013CB44(field_0x1388, &pos, 0, -1, true);
            } else {
                switch (fn_802486A0()) {
                case 0:
                    field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35e, 0, -1, NULL, NULL, true, 1.0f);
                    break;
                case 1:
                    field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x360, 0, -1, NULL, NULL, true, 1.0f);
                    break;
                case 2:
                    field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35f, 0, -1, NULL, NULL, true, 1.0f);
                    break;
                case 3:
                    field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x361, 0, -1, NULL, NULL, true, 1.0f);
                    break;
                }
            }
        } else {
            deleteEmitter(&field_0x1388);
        }
        return;
    }
    deleteEmitter(&field_0x1388);
    switch (fn_802486A0()) {
    case 0:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35e, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1ba, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1bb, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 1:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x360, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b8, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b9, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 2:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x35f, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b5, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b6, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b7, 0, -1, NULL, NULL, true, 1.0f);
        break;
    case 3:
        field_0x1388 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x361, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b2, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b3, 0, -1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1b4, 0, -1, NULL, NULL, true, 1.0f);
        break;
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i)) {
            fn_8022B860(i, 5);
        }
    }
    field_0x121d = 0;
    if (fn_802486A0() != 5 && fn_802486A0() != 4) {
        fn_8024EFF8(0x37);
    }
}

void Player_804A6F38::fn_8027F854(JGeometry::TVec3f* out, s32 player) {
    out->set(*getPlayerPos_8023E724(player));
    switch (fn_802486A0()) {
    case 3:
        out->x += lbl_8046BF98[player][field_0x4][0];
        out->y += lbl_8046BF98[player][field_0x4][1];
        break;
    case 2:
        out->x += lbl_8046C018[player][field_0x4][0];
        out->y += lbl_8046C018[player][field_0x4][1];
        break;
    case 0:
        out->x += 16.0f * (fn_80248778() - fn_8022E8D4(player));
        break;
    case 1:
        out->y += 16.0f * (fn_80248778() - fn_8022E8D4(player));
        break;
    case 5:
        break;
    }
}

// The state 3 movement: in formation 5 with cls_804A71A8 0xB4 the stick walk step at 1.5, else
// (outside the cooldown 0x122C) a step of 0.5 toward the follow target (fn_8027FA08), standing (0)
// or walking (2) on arrival. Dead-stripped: fn_8028538C inlines it. Its own copy created the pool's
// 1.5, 0.5 and 3.0 (TUtilf::inv_sqrt in normalize) in that order, which sit before fn_8027FA08's 4.0
// in the original .sdata2; inlined, its locals give fn_8028538C the original's stack slots.
void Player_804A6F38::followStep() {
    if (fn_802486A0() == 5 && field_0xbf0->field_0xb4) {
        fn_802808E8(1.5f);
    } else if (field_0x122c <= 0) {
        JGeometry::TVec3f to;
        fn_8027FA08(&to);
        JGeometry::TVec3f dir = to - mPos;
        dir.z = 0.0f;
        if (magnitude(dir) >= 0.5f) {
            dir.normalize();
            dir.scale(0.5f);
            mPos += dir;
        } else {
            mPos.set(to);
            if (fn_802486A0() != 5) {
                if (fn_8022C6AC(field_0x4) != 0.0f) {
                    fn_80251AEC(2);
                } else {
                    fn_80251AEC(0);
                }
            } else if (!fn_80240808()) {
                fn_80251AEC(0);
            }
        }
    }
}

// Where this player's follow target is (update B, fn_8028676C): its own history head (no leader,
// 0x1216), mPos, or the leader's position plus this player's place in the formation by fn_802486A0
// (0 a row along x, 1 a column along y, 2 a 2x2 square, 3 a diamond around the leader turned by
// 0x11C0 or by both indices, 4 mPos, 5 the leader's history 12 frames per place back).
void Player_804A6F38::fn_8027FA08(JGeometry::TVec3f* out) {
    if (!fn_80240808() && field_0x1216) {
        *out = field_0x38[0];
        return;
    }
    if (fn_80240808()) {
        if (fn_8022DCD8(fn_80240644())) {
            *out = mPos;
            return;
        }
        // Read before the call: the original loads the layer first (declaration order only steers scheduling).
        s32 layer = field_0x3b4;
        if (layer != getPlayerLayer_8023DE58(fn_80240644())) {
            *out = mPos;
            return;
        }
        if (field_0x1240 == 0x46) {
            *out = mPos;
            return;
        }
        *out = *getPlayerPos_8023E724(fn_80240644());
        if (fn_802486A0() == 5) {
            out->z = mPos.z;
        }
    } else {
        *out = mPos;
    }
    if (fn_802486A0() != 4 && fn_80240808() && fn_8023EA28(fn_80240644())) {
        return;
    }
    if (field_0x1214) {
        return;
    }
    switch (fn_802486A0()) {
    case 3:
        if (fn_80241284() && fn_80248970() >= 2) {
            JGeometry::TVec3f center = *fn_80252F48();
            out->x = center.x + 16.0f * JMath::sincosTable_.cosShort(field_0x11c0);
            if (viewLayerFlag(field_0x3b4)) {
                out->y = center.y + 4.0f * -JMath::sincosTable_.sinShort(field_0x11c0);
            } else {
                out->y = center.y + 16.0f * JMath::sincosTable_.sinShort(field_0x11c0);
            }
            out->z = center.z;
        } else if (fn_80240808()) {
            switch (fn_80240644()) {
            case 0:
                if (viewLayerFlag(field_0x3b4)) {
                    out->y -= 4.0f;
                } else {
                    out->y += 16.0f;
                }
                break;
            case 1:
                out->x += 16.0f;
                break;
            case 2:
                out->x -= 16.0f;
                break;
            case 3:
                if (viewLayerFlag(field_0x3b4)) {
                    out->y += 4.0f;
                } else {
                    out->y -= 16.0f;
                }
                break;
            }
            switch (field_0x4) {
            case 0:
                if (viewLayerFlag(field_0x3b4)) {
                    out->y += 4.0f;
                } else {
                    out->y -= 16.0f;
                }
                break;
            case 1:
                out->x -= 16.0f;
                break;
            case 2:
                out->x += 16.0f;
                break;
            case 3:
                if (viewLayerFlag(field_0x3b4)) {
                    out->y -= 4.0f;
                } else {
                    out->y += 16.0f;
                }
                break;
            }
        }
        break;
    case 2:
        out->x += 16.0f * (fn_80248778() & 1);
        if (viewLayerFlag(field_0x3b4)) {
            out->y -= 16.0f * (fn_80248778() & 2) / 2;
        } else {
            out->y += 16.0f * (fn_80248778() & 2) / 2;
        }
        break;
    case 0:
        out->x += 16.0f * fn_80248778();
        break;
    case 1:
        if (viewLayerFlag(field_0x3b4)) {
            out->y -= 16.0f * fn_80248778();
        } else {
            out->y += 16.0f * fn_80248778();
        }
        break;
    case 5:
        if (fn_80240808()) {
            out->set(*fn_8023E4E4(fn_80240644(), fn_80248778() * 12));
        } else {
            *out = *getPlayerPos_8023E724(field_0x4);
        }
        break;
    case 4:
        out->set(mPos);
        break;
    }
}

// Update helper of state 0x4B (fn_80282ACC): a follower (unless it is the acting player of
// formation 4) steps 4.5 a frame toward its follow target (fn_8027FA08); else, without pad 0x40, a
// run along 0x3E8 (lbl_8046BF38) whose speed 0x344 decays by 0.025 to 1.2 (fn_80244FFC held), 1
// (stick held) or 0, and the dash counter 0x3FF (started by fn_80245238, sound 0x20039) shapes
// it: kept to 9, 1 with 0x410 = -0.6 at 10, +0.25 to 15, then decays to 1.2, back to 0 after 33.
void Player_804A6F38::fn_80280074() {
    bool follow = fn_80240808();
    if (fn_802486A0() == 4 && fn_80240888() == fn_80248778()) {
        follow = false;
    }
    if (follow) {
        JGeometry::TVec3f to;
        fn_8027FA08(&to);
        JGeometry::TVec3f dir = to - mPos;
        dir.z = 0.0f;
        if (magnitude(dir) >= 4.5f) {
            dir.normalize();
            dir.scale(4.5f);
            mPos += dir;
        } else {
            mPos.set(to);
        }
        if (field_0x3ff == 0) {
            if (fn_80245238()) {
                field_0x3ff = 1;
                fn_8024F0E8(0x20039);
            }
        } else {
            field_0x3ff++;
            if (field_0x3ff >= 33) {
                field_0x3ff = 0;
            }
        }
    } else if (!fn_80246B10(0x40)) {
        if (field_0x3ff == 0) {
            if (fn_80245238()) {
                field_0x3ff = 1;
                fn_8024F0E8(0x20039);
            } else {
                f32 speed = magnitude(field_0x344);
                if (fn_80244FFC()) {
                    speed -= 0.025f;
                    if (speed < 1.2f) {
                        speed = 1.2f;
                    }
                } else if (fn_80246F28() > 0.0f) {
                    speed -= 0.025f;
                    if (speed < 1.0f) {
                        speed = 1.0f;
                    }
                } else {
                    speed -= 0.025f;
                    if (speed < 0.0f) {
                        speed = 0.0f;
                    }
                }
                const Vec& dir = lbl_8046BF38[field_0x3e8];
                field_0x344.set(dir.x, dir.y, dir.z);
                field_0x344.scale(speed);
                mPos += field_0x344;
            }
        } else {
            f32 speed;
            if (field_0x3ff < 10) {
                speed = magnitude(field_0x344);
            } else if (field_0x3ff == 10) {
                field_0x410 = -0.6f;
                speed = 1.0f;
            } else if (field_0x3ff < 16) {
                speed = magnitude(field_0x344);
                speed += 0.25f;
            } else {
                speed = magnitude(field_0x344);
                speed -= 0.025f;
                if (speed < 1.2f) {
                    speed = 1.2f;
                }
            }
            const Vec& dir = lbl_8046BF38[field_0x3e8];
            field_0x344.set(dir.x, dir.y, dir.z);
            field_0x344.scale(speed);
            mPos += field_0x344;
            if (field_0x3ff >= 33) {
                field_0x3ff = 0;
            } else {
                field_0x3ff++;
            }
        }
    }
}

// Update helper of state 0x4A (fn_80282FE0): fn_80280074's movement with lbl_8046BED8, a speed
// read once, 0.18 added on counts 11-15, and while 0x430 runs only the decay to 0.
void Player_804A6F38::fn_802804E0() {
    bool follow = fn_80240808();
    if (fn_802486A0() == 4 && fn_80240888() == fn_80248778()) {
        follow = false;
    }
    if (follow) {
        JGeometry::TVec3f to;
        fn_8027FA08(&to);
        JGeometry::TVec3f dir = to - mPos;
        dir.z = 0.0f;
        if (magnitude(dir) >= 4.5f) {
            dir.normalize();
            dir.scale(4.5f);
            mPos += dir;
        } else {
            mPos.set(to);
        }
        if (field_0x3ff == 0) {
            if (fn_80245238()) {
                field_0x3ff = 1;
                fn_8024F0E8(0x20039);
            }
        } else {
            field_0x3ff++;
            if (field_0x3ff >= 33) {
                field_0x3ff = 0;
            }
        }
    } else if (!fn_80246B10(0x40)) {
        f32 speed = magnitude(field_0x344);
        if (field_0x430 > 0) {
            speed -= 0.025f;
            if (speed < 0.0f) {
                speed = 0.0f;
            }
        } else if (field_0x3ff == 0) {
            if (fn_80245238()) {
                field_0x3ff = 1;
                fn_8024F0E8(0x20039);
            } else if (fn_80244FFC()) {
                speed -= 0.025f;
                if (speed < 1.2f) {
                    speed = 1.2f;
                }
            } else if (fn_80246F28() > 0.0f) {
                speed -= 0.025f;
                if (speed < 1.0f) {
                    speed = 1.0f;
                }
            } else {
                speed -= 0.025f;
                if (speed < 0.0f) {
                    speed = 0.0f;
                }
            }
        } else {
            if (field_0x3ff < 10) {
                // the speed is kept
            } else if (field_0x3ff == 10) {
                field_0x410 = -0.6f;
                speed = 1.0f;
            } else if (field_0x3ff < 16) {
                speed += 0.18f;
            } else {
                speed -= 0.025f;
                if (speed < 1.2f) {
                    speed = 1.2f;
                }
            }
            if (field_0x3ff >= 33) {
                field_0x3ff = 0;
            } else {
                field_0x3ff++;
            }
        }
        const Vec& dir = lbl_8046BED8[field_0x3e8];
        field_0x344.set(dir.x, dir.y, dir.z);
        field_0x344.scale(speed);
        mPos += field_0x344;
    }
}

// The stick walk step: speed (x and y, zero while knocked back 0x35C or blocked by 0x3C1 / 0x3C2,
// halved on tiles 0x2C-0x2F, x1.4 with fn_8022B244 and item 5) into 0x38C / 0x390, then mPos moves
// by the stick (fn_8022C82C, fn_8022C76C); upward on view-flag layers only while one of the eight
// probe points (fn_8024B298) stays off tile 2.
void Player_804A6F38::fn_802808E8(f32 speed) {
    f32 speedX = speed;
    f32 speedY = speed;
    if (field_0x1216) {
        return;
    }
    if (!field_0x35c.isZero()) {
        speedX = 0.0f;
        speedY = 0.0f;
    }
    if (field_0x3c1) {
        speedX = 0.0f;
    }
    if (field_0x3c2) {
        speedY = 0.0f;
    }
    u16 tile = fn_8022461C(&mPos, field_0x3b4);
    if (tile == 0x2c || tile == 0x2d || tile == 0x2e || tile == 0x2f) {
        speedY *= 0.5f;
    }
    if (fn_8022B244() && fn_802416C4() == 5) {
        speedX *= 1.4f;
        speedY *= 1.4f;
    }
    field_0x38c = speedX;
    field_0x390 = speedY;
    if (speedX * speedX + speedY * speedY <= JGeometry::TUtilf::epsilon()) {
        return;
    }
    if (field_0xbf0->field_0xb7) {
        return;
    }
    if (fn_80246B10(0x40)) {
        field_0x368.x = 0.0f;
        field_0x368.y = 0.0f;
        return;
    }
    mPos.x += speedX * fn_8022C82C(field_0x4);
    if (viewLayerFlag(field_0x3b4) && fn_8022C76C(field_0x4) > 0.0f) {
        for (s32 i = 0; i < 8; i++) {
            JGeometry::TVec3f probe;
            fn_8024B298(i, &probe);
            probe.y += speedY * -fn_8022C76C(field_0x4);
            if (fn_8022461C(&probe, field_0x3b4) != 2) {
                mPos.y += speedY * -fn_8022C76C(field_0x4);
                return;
            }
        }
        field_0x368.y = 0.0f;
    } else {
        mPos.y += speedY * -fn_8022C76C(field_0x4);
    }
}

// Ice sliding (called by fn_802870C8 after the state update): stopped while pushing (fn_80250D30) or with
// fn_80240B5C; on slippery ground (cls_804A71A8 0xB7) the stick (0x38C / 0x390) pushes the slide
// speed 0x368 by 0.05 on the floor (sound 0x20059 when it slows down), the speed loses 0.02 a frame
// and is capped at 1.5, and mPos moves by it in x / y; elsewhere the speed is cleared unless field_0x14 is on tile 3.
void Player_804A6F38::fn_80280B30() {
    if (fn_80250D30() || fn_80240B5C()) {
        field_0x368.x = 0.0f;
        field_0x368.y = 0.0f;
        field_0x368.z = 0.0f;
    } else if (fn_8024BBB0() || fn_802413E8() || fn_80270A8C()) {
        // left alone (no slide while these hold)
    } else if (field_0xbf0->field_0xb7) {
        if (mPos.z <= 0.0f) {
            if (!fn_80246B10(0x40)) {
                f32 before = magnitude(field_0x368);
                field_0x368.x += 0.05f * (field_0x38c * fn_8022C82C(field_0x4));
                field_0x368.y += 0.05f * (field_0x390 * -fn_8022C76C(field_0x4));
                if (magnitude(field_0x368) < before) {
                    fn_8024F0E8(0x20059);
                }
            }
        } else {
            field_0x368.x = 0.0f;
            field_0x368.y = 0.0f;
        }
        field_0x368.z = 0.0f;
        f32 speed = magnitude(field_0x368);
        speed -= 0.02f;
        if (speed < 0.0f) {
            speed = 0.0f;
        } else if (speed >= 1.5f) {
            speed = 1.5f;
        }
        field_0x368.normalize();
        field_0x368.scale(speed);
        mPos.x += field_0x368.x;
        mPos.y += field_0x368.y;
    } else if (fn_8022461C(&field_0x14, 0) != 3) {
        field_0x368.x = 0.0f;
        field_0x368.y = 0.0f;
        field_0x368.z = 0.0f;
    }
}

void Player_804A6F38::fn_80280DD8() {
    if (field_0x1350 != NULL) {
        field_0x1350->becomeInvalidEmitter();
        field_0x1350 = NULL;
    }
    field_0x484 = 0;
    mPos -= field_0x474;
    field_0x474.x = 0.0f;
    field_0x474.y = 0.0f;
    field_0x474.z = 0.0f;
    field_0x464 = 60;
}

// Update of state 0x51 (hit kind 0x18): with 0x484 it waits out 0x480 and stands (0); else the
// buttons (pad trigger 0xF) take 10 off the 300-frame timer 0x46C and jolt the player one unit
// by the pad direction for 5 frames (offset 0x474, 0x470), and at 0 it stands (0) with effects
// 0x28B / 0x28C and sound 0x2003C; the effect 0x1350 follows mPos.
void Player_804A6F38::fn_80280E44() {
    if (field_0x484) {
        field_0x46c = 0;
        if (field_0x480 > 0) {
            field_0x480--;
        }
        if (field_0x480 <= 0 && fn_80251AEC(0)) {
            fn_8024E3D8();
        }
    } else {
        if (field_0x470 > 0) {
            field_0x470--;
            if (field_0x470 == 0) {
                mPos -= field_0x474;
                field_0x474.x = 0.0f;
                field_0x474.y = 0.0f;
                field_0x474.z = 0.0f;
            }
        }
        if (fn_80246D1C(0xF)) {
            field_0x46c -= 10;
            if (field_0x46c < 0) {
                field_0x46c = 0;
            }
            field_0x470 = 5;
            mPos -= field_0x474;
            if (fn_80246D1C(2)) {
                field_0x474.set(1.0f, 0.0f, 0.0f);
            } else if (fn_80246D1C(1)) {
                field_0x474.set(-1.0f, 0.0f, 0.0f);
            } else if (fn_80246D1C(4)) {
                field_0x474.set(0.0f, 1.0f, 0.0f);
            } else {
                field_0x474.set(0.0f, -1.0f, 0.0f);
            }
            mPos += field_0x474;
        } else if (field_0x46c > 0) {
            field_0x46c--;
        }
        if (field_0x46c == 0 && fn_80251AEC(0)) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x28b, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x28c, 0, 1, NULL, NULL, true, 1.0f);
            fn_8024F0E8(0x2003C);
        }
    }
    fn_80268B3C();
    if (field_0x1350 != NULL) {
        setEmitterPos_8013CB44(field_0x1350, &mPos, 0, 1, true);
    }
}

void Player_804A6F38::fn_802810E4() {
    field_0x458 = 0;
    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x289, 0, 1, NULL, NULL, true, 1.0f);
    if (field_0x1350 == NULL) {
        field_0x1350 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x28a, 0, 1, NULL, NULL, true, 1.0f);
    }
    if (!fn_80240808()) {
        fn_8024E3D8();
    }
    field_0x46c = 300;
    field_0x484 = 0;
    field_0x470 = 0;
    field_0x474.x = 0.0f;
    field_0x474.y = 0.0f;
    field_0x474.z = 0.0f;
    fn_8024F0E8(0x2003B);
}

void Player_804A6F38::fn_802811C0() {
    if (field_0x132c != NULL) {
        field_0x132c->becomeInvalidEmitter();
        field_0x132c = NULL;
    }
    if (field_0x135c != NULL) {
        field_0x135c->becomeInvalidEmitter();
        field_0x135c = NULL;
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802BAA00();
        }
        field_0xb28 = -1;
    }
}

// FIXME: 99.92. Registers only: the original keeps the old facing in r29 and the 0xB28 serial in r30,
// ours the other way round.
// Update of state 0x50 (burning): on the floor it runs at 2 along the facing (fn_8027EB04 steers)
// until 300 frames (0x408) or tile 0x39, then stands (0); in the air it falls 0.25 a frame. The
// ACTOR_FIRE actor 0xB28 follows the box 0x4C0; the walk cycle runs at half speed; on a turn the
// fire effects 0x135C / 0x132C are respawned for the new facing; 0x135C follows mPos, and the
// particle callback 0x1330 gets mPos (3 lower facing down, 3 higher facing up) through 0x1344.
void Player_804A6F38::fn_8028126C() {
    s32 facing = field_0x3dc;
    if (mPos.z <= 0.0f && field_0x344.z <= 0.0f) {
        field_0x344.z = 0.0f;
        fn_8027EB04();
        mPos += field_0x344;
        switch (field_0x3dc) {
        case 0:
            field_0x344.set(2.0f, 0.0f, 0.0f);
            break;
        case 1:
            field_0x344.set(-2.0f, 0.0f, 0.0f);
            break;
        case 2:
            field_0x344.set(0.0f, 2.0f, 0.0f);
            break;
        case 3:
            field_0x344.set(0.0f, -2.0f, 0.0f);
            break;
        }
        field_0x408++;
        if (field_0x408 >= 300 || fn_8022461C(&mPos, field_0x3b4) == 0x39) {
            fn_80251AEC(0);
        }
    } else {
        field_0x344.z -= 0.25f;
        mPos.z += field_0x344.z;
        if (mPos.z <= 0.0f) {
            field_0x344.z = 0.0f;
        }
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        FireRodFire_804A89F0* fire =
            static_cast<FireRodFire_804A89F0*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (fire != NULL) {
            fire->fn_802BAA0C(&field_0x4c0);
        }
    }
    fn_80268B3C();
    field_0x3f8++;
    if (field_0x3f8 >= 16) {
        field_0x3f8 = 0;
    }
    field_0x3f9 = field_0x3f8 / 2;
    if (facing != field_0x3dc) {
        if (field_0x135c != NULL) {
            field_0x135c->becomeInvalidEmitter();
            field_0x135c = NULL;
        }
        if (field_0x135c == NULL) {
            switch (field_0x3dc) {
            case 0:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x248, 0, 0, NULL, NULL, true, 1.0f);
                break;
            case 1:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x247, 0, 0, NULL, NULL, true, 1.0f);
                break;
            case 2:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x246, 0, -1, NULL, NULL, true, 1.0f);
                break;
            case 3:
                field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x249, 0, 0, NULL, NULL, true, 1.0f);
                break;
            }
        }
        deleteEmitter(&field_0x132c);
        if (field_0x132c == NULL) {
            switch (field_0x3dc) {
            case 0:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x244, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            case 1:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x243, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            case 2:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x242, 0, -1, NULL, &field_0x1330, true, 1.0f);
                break;
            case 3:
                field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x245, 0, 0, NULL, &field_0x1330, true, 1.0f);
                break;
            }
        }
    }
    if (field_0x135c != NULL) {
        switch (field_0x3dc) {
        case 0:
        case 1:
        case 3:
            setEmitterPos_8013CB44(field_0x135c, &mPos, 0, 0, true);
            break;
        case 2:
            setEmitterPos_8013CB44(field_0x135c, &mPos, 0, -1, true);
            break;
        }
    }
    if (field_0x132c != NULL) {
        field_0x1344.x = mPos.x;
        field_0x1344.y = mPos.y;
        field_0x1344.z = mPos.z;
        if (field_0x3dc == 2) {
            field_0x1344.y -= 3.0f;
        } else if (field_0x3dc == 3) {
            field_0x1344.y += 3.0f;
        }
        field_0x1330.field_0x4.x = field_0x1344.x;
        field_0x1330.field_0x4.y = field_0x1344.y;
        field_0x1330.field_0x4.z = field_0x1344.z;
    }
}

// Enter of a hop on fire: speed lbl_8046BEA8 along the facing (3 up), the two fire effects by facing
// (0x135C: 0x248 / 0x247 / 0x246 / 0x249; 0x132C with the particle callback 0x1330: 0x244 / 0x243 /
// 0x242 / 0x245) unless they exist, an ACTOR_FIRE by the player unless 0xB28 is alive, sounds.
void Player_804A6F38::fn_80281760() {
    field_0x458 = 0;
    const Vec& speed = lbl_8046BEA8[field_0x3dc];
    field_0x344.set(speed.x, speed.y, speed.z);
    if (field_0x135c == NULL) {
        switch (field_0x3dc) {
        case 0:
            field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x248, 0, 0, NULL, NULL, true, 1.0f);
            break;
        case 1:
            field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x247, 0, 0, NULL, NULL, true, 1.0f);
            break;
        case 2:
            field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x246, 0, -1, NULL, NULL, true, 1.0f);
            break;
        case 3:
            field_0x135c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x249, 0, 0, NULL, NULL, true, 1.0f);
            break;
        }
    }
    if (field_0x132c == NULL) {
        switch (field_0x3dc) {
        case 0:
            field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x244, 0, 0, NULL, &field_0x1330, true, 1.0f);
            break;
        case 1:
            field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x243, 0, 0, NULL, &field_0x1330, true, 1.0f);
            break;
        case 2:
            field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x242, 0, -1, NULL, &field_0x1330, true, 1.0f);
            break;
        case 3:
            field_0x132c = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x245, 0, 0, NULL, &field_0x1330, true, 1.0f);
            break;
        }
    }
    if (field_0xb28 < 0) {
        s32 index = field_0x4;
        field_0xb28 = getActorManager_801F856C()->spawnActor_801F9484(
            ACTOR_FIRE, field_0x3b4, &mPos, ((field_0x3dc & 3) << 4) | 0x180 | (index & 3), -1, index);
    }
    field_0x408 = 0;
    fn_8024F0E8(0x10014);
    fn_8024F0E8(0x2004C);
}

void Player_804A6F38::fn_80281A18() {
    field_0x11d8 = 0;
    field_0x11d4 = 10;
    field_0x368.set(field_0x344.x, field_0x344.y, 0.0f);
    if (field_0x1354 != NULL) {
        field_0x1354->becomeInvalidEmitter();
        field_0x1354 = NULL;
    }
    if (field_0x1358 != NULL) {
        field_0x1358->becomeInvalidEmitter();
        field_0x1358 = NULL;
    }
}

void Player_804A6F38::fn_80281A90() {
    if (field_0x3fa >= 5 && fn_802466B4() && !fn_802DEBF8() && fn_80251AEC(9)) {
        field_0x11ac = 4;
    }
    mPos.x += field_0x344.x;
    mPos.x += fn_8022C82C(field_0x4);
    if (!viewLayerFlag(field_0x3b4) || fn_8022461C(&mPos, field_0x3b4) != 2) {
        mPos.y += field_0x344.y;
        mPos.y -= fn_8022C76C(field_0x4);
    }
    field_0x368.x = 0.0f;
    field_0x368.y = 0.0f;
    field_0x368.z = 0.0f;
    if (field_0x3fa < 4) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 11) {
        field_0x3fb = 2;
    } else if (field_0x3fa < 14) {
        field_0x3fb = 3;
    } else if (field_0x3fa < 17) {
        field_0x3fb = 4;
    } else if (field_0x3fa < 20) {
        field_0x3fb = 5;
    } else {
        field_0x3fb = 5;
        fn_80251AEC(0);
    }
    field_0x3fa++;
    fn_80268B3C();
    if (mPos.z <= 0.0f) {
        if (fn_8022461C(&mPos, field_0x3b4) == 0x39) {
            if (field_0x1358 == NULL) {
                field_0x1358 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3d4, 0, -1, &field_0x13a8, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(field_0x1358, &mPos, 0, -1, true);
            }
            if (field_0x1354 != NULL) {
                field_0x1354->becomeInvalidEmitter();
                field_0x1354 = NULL;
            }
        } else {
            if (field_0x1354 == NULL) {
                field_0x1354 = spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3d3, 0, -1, &field_0x1398, NULL, true, 1.0f);
            } else {
                setEmitterPos_8013CB44(field_0x1354, &mPos, 0, -1, true);
            }
            if (field_0x1358 != NULL) {
                field_0x1358->becomeInvalidEmitter();
                field_0x1358 = NULL;
            }
        }
    }
}

void Player_804A6F38::fn_80281D50() {
    field_0x458 = 0;
    if (fn_80240670()) {
        s32 facing = fn_80230CFC(fn_80240644());
        field_0x3dc = facing;
        field_0x3e8 = facing;
    } else if (fn_80246B10(2)) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (fn_80246B10(1)) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else if (fn_80246B10(8)) {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    } else if (fn_80246B10(4)) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    }
    const Vec& speed = lbl_8046BE78[field_0x3dc];
    field_0x344.set(speed.x, speed.y, speed.z);
    fn_8024F0E8(0x1000C);
    field_0x368.x = 0.0f;
    field_0x368.y = 0.0f;
    field_0x368.z = 0.0f;
    field_0x11d8 = 0;
}

void Player_804A6F38::fn_80281E84() {
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
}

// Update of state 0x4E: after the 0x3FC wait, flies with 0x344 (y falls 0.1 a frame to -1, x speeds
// up 0.1 a frame to +-5) and rebounds at (-1 / 1, 3) when x is past 5 or fn_802517F8 hits; from the
// second rebound on, landing (y <= 0), 0x3FD or fn_802518AC stops it (state 0 once 0xC8C is back
// to 1), else the squash 0xC84 = (1, 1, 0.5).
void Player_804A6F38::fn_80281E98() {
    if (field_0x3fc != 0) {
        field_0x3fc--;
    } else {
        field_0x344.y -= 0.1f;
        if (field_0x344.y <= -1.0f) {
            field_0x344.y = -1.0f;
        }
        if (field_0x344.x > 0.0f) {
            field_0x344.x += 0.1f;
            if (field_0x344.x > 5.0f || fn_802517F8(false)) {
                field_0x344.x = -1.0f;
                field_0x344.y = 3.0f;
                field_0x344.z = 0.0f;
                field_0x3fc = 10;
                field_0x3fa++;
            }
        } else {
            field_0x344.x -= 0.1f;
            if (field_0x344.x < -5.0f || fn_802517F8(false)) {
                field_0x344.x = 1.0f;
                field_0x344.y = 3.0f;
                field_0x344.z = 0.0f;
                field_0x3fc = 10;
                field_0x3fa++;
            }
        }
    }
    if (field_0x3fc == 0) {
        mPos += field_0x344;
    }
    if ((field_0x3fa >= 2 && field_0x344.y <= 0.0f) || field_0x3fd == 1 || fn_802518AC(false)) {
        field_0x3fd = 1;
        field_0x344.x = 0.0f;
        field_0x344.y = 0.0f;
        field_0x344.z = 0.0f;
        mPos.z = 0.0f;
        if (field_0xc84.z >= 1.0f) {
            fn_80251AEC(0);
        }
    } else {
        field_0xc84.x = 1.0f;
        field_0xc84.y = 1.0f;
        field_0xc84.z = 0.5f;
    }
    fn_802685D4();
}

// Enter of state 0x4E: speed (-1, 3, 0), a view shake (fn_802D79E8, 4 along x) and on the TV the
// effect of kind 1 at 0xCB0; 20 frames (0x3FC).
void Player_804A6F38::fn_80282094() {
    field_0x458 = 0;
    field_0x344.x = -1.0f;
    field_0x344.y = 3.0f;
    field_0x344.z = 0.0f;
    getGameView_802D6A9C()->fn_802D79E8(field_0x3b4, 4.0f, 0.0f, false);
    gGameRoot2_8053AB10->field_0x20->fn_802F601C(&field_0xcb0, 1, NULL, 1.0f, -2.0f);
    field_0x3fc = 20;
}

void Player_804A6F38::fn_8028211C() {
    if (field_0xce8 == true && (field_0xcf4 == 0x13 || field_0xcf4 == 0x16)) {
        field_0xce8 = false;
    }
    field_0xcea = 0;
    if (field_0x3b4 < 8) {
        fn_8024BAEC();
    }
}

// FIXME: 99.33. Scheduling only: before the ground mark (fn_802F601C) the original computes &mPos
// (addi r4) ahead of the layer compare, ours after loading gGameRoot2_8053AB10.
// Update of state 0x4D (fired from a cannon): sound 0x10003 at frame 72 (0x394); while the landing
// wait 0x3FC runs (and the GBA link fn_80301CD0 is free), a pending switch 0xCEA moves the player
// to layer 0xCE4 at 0xCD4 (keeping the return point 0xCB0 when leaving layers 0-7; speed y
// clamped to -7..-10 on view-flag layers, else y / z swapped and z clamped to 7..10), and at 0 the
// landing (fn_80290D58, ground mark fn_802F601C below layer 8, effect 0x17D). In flight mPos moves
// by 0x344 + 0x380; 0xCE8 starts the wait (60 below layer 8, else 20); on view-flag layers y
// gains 0.25 to 6 and it lands (0x12 if fn_8025F684, else 0) at a wall (fn_8025190C: bounce 0x4E)
// or out of the shrunk view box, elsewhere z loses 0.25 to -6 and it lands below 0; the stick
// steers 0x380, the pad (fn_8022C3AC) cancels to state 5. The body frame, and with fn_8025F684
// the carried actor's place 0x2F4 (18 up) and frame 0x3F9 (falling faster than 1).
void Player_804A6F38::fn_80282178() {
    field_0x394++;
    if (field_0x394 == 72) {
        fn_8024F0E8(0x10003);
    }
    if (field_0x3fc) {
        if (!fn_80301CD0(field_0x4)) {
            if (field_0xcea == 1) {
                if (field_0x3b4 < 8) {
                    field_0xcb0 = mPos;
                }
                field_0x3b4 = field_0xce4;
                mPos = field_0xcd4;
                mPos.z = 1.25f;
                field_0xbec->fn_8030BD3C(field_0x4);
                if (viewLayerFlag(field_0x3b4)) {
                    if (field_0x344.y > -7.0f) {
                        field_0x344.y = -7.0f;
                    } else if (field_0x344.y < -10.0f) {
                        field_0x344.y = -10.0f;
                    }
                } else {
                    field_0x344.set(field_0x344.x, -field_0x344.z, -field_0x344.y);
                    if (field_0x344.z < 7.0f) {
                        field_0x344.z = 7.0f;
                    } else if (field_0x344.z > 10.0f) {
                        field_0x344.z = 10.0f;
                    }
                }
                fn_80301D54(field_0x4);
                field_0xcea = 0;
            }
            field_0x3fc--;
            if (field_0x3fc == 0) {
                fn_80290D58();
                if (field_0x3b4 < 8) {
                    gGameRoot2_8053AB10->field_0x20->fn_802F601C(&mPos, 1, NULL, 1.0f, -2.0f);
                }
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x17d, 1, 0, NULL, NULL, true, 1.0f);
                field_0x124d = 0;
            }
        }
    } else {
        mPos += field_0x344;
        mPos += field_0x380;
        if (field_0xce8 == true) {
            if (field_0xce4 < 8) {
                field_0x3fc = 60;
            } else {
                field_0x3fc = 20;
            }
            field_0x124d = 1;
            field_0xcea = 1;
        }
        if (viewLayerFlag(field_0x3b4)) {
            field_0x344.y += 0.25f;
            if (field_0x344.y > 6.0f) {
                field_0x344.y = 6.0f;
            }
            JGeometry::TBox2f box;
            box = *layerBox(field_0x3b4);
            growBox(box, -16.0f);
            if (fn_8025190C(true)) {
                fn_80251AEC(0x4e);
                mPos.z = 0.0f;
            } else if (field_0x344.y >= 0.0f && (fn_802518AC(true) || !containsPoint(box, mPos))) {
                if (fn_8025F684()) {
                    fn_80251AEC(0x12);
                } else {
                    fn_80251AEC(0);
                }
                mPos.z = 0.0f;
            } else {
                mPos.z = 1.25f;
            }
        } else {
            field_0x344.z -= 0.25f;
            if (field_0x344.z < -6.0f) {
                field_0x344.z = -6.0f;
            }
            if (mPos.z < 0.0f) {
                if (fn_8025F684()) {
                    fn_80251AEC(0x12);
                } else {
                    fn_80251AEC(0);
                }
                mPos.z = 0.0f;
            }
        }
        if (field_0x380.x > 0.0f) {
            if (fn_8022C82C(field_0x4) > 0.0f) {
                field_0x380.x += 0.1f;
                if (field_0x380.x > 1.6f) {
                    field_0x380.x = 1.6f;
                }
            } else {
                field_0x380.x -= 0.1f;
                if (field_0x380.x < 0.1f) {
                    field_0x380.x = 0.1f;
                }
            }
        } else if (field_0x380.x < 0.0f) {
            if (fn_8022C82C(field_0x4) < 0.0f) {
                field_0x380.x -= 0.1f;
                if (field_0x380.x < -1.6f) {
                    field_0x380.x = -1.6f;
                }
            } else {
                field_0x380.x += 0.1f;
                if (field_0x380.x > -0.1f) {
                    field_0x380.x = -0.1f;
                }
            }
        } else {
            field_0x380.x = 0.8f * fn_8022C82C(field_0x4);
        }
        if (!viewLayerFlag(field_0x3b4)) {
            if (field_0x380.y > 0.0f) {
                if (-fn_8022C76C(field_0x4) > 0.0f) {
                    field_0x380.y += 0.1f;
                    if (field_0x380.y > 1.6f) {
                        field_0x380.y = 1.6f;
                    }
                } else {
                    field_0x380.y -= 0.1f;
                    if (field_0x380.y < 0.1f) {
                        field_0x380.y = 0.1f;
                    }
                }
            } else if (field_0x380.y < 0.0f) {
                if (-fn_8022C76C(field_0x4) < 0.0f) {
                    field_0x380.y -= 0.1f;
                    if (field_0x380.y < -1.6f) {
                        field_0x380.y = -1.6f;
                    }
                } else {
                    field_0x380.y += 0.1f;
                    if (field_0x380.y > -0.1f) {
                        field_0x380.y = -0.1f;
                    }
                }
            } else {
                field_0x380.y = 0.8f * -fn_8022C76C(field_0x4);
            }
        }
        if (!field_0xce8 && fn_8022C3AC(field_0x4)) {
            fn_80251AEC(5);
        }
    }
    fn_802685D4();
    if (fn_8025F684()) {
        field_0x2f4.set(0.0f, 0.0f, 18.0f);
        field_0x2f4 += mPos;
        if (field_0x344.z < -1.0f) {
            field_0x3f9 = 1;
        } else {
            field_0x3f9 = 0;
        }
    }
}

void Player_804A6F38::fn_8028286C() {
    field_0x344.x = field_0x374.x;
    field_0x344.y = field_0x374.y;
    field_0x344.z = field_0x374.z;
    field_0x380.x = 0.0f;
    field_0x380.y = 0.0f;
    field_0x380.z = 0.0f;
    field_0x458 = 0;
    field_0x394 = 0;
    field_0xcea = 0;
}

void Player_804A6F38::fn_802828A8() {
    Cannon_8049CCF0* cannon = static_cast<Cannon_8049CCF0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd8c));
    if (cannon != NULL) {
        if (viewLayerFlag(field_0x3b4)) {
            field_0x374.set(0.0f, -cannon->fn_801080B8(), 0.0f);
        } else {
            field_0x374.set(0.0f, 0.0f, cannon->fn_801080B8());
        }
    }
    field_0xd8c = -1;
    field_0xd90 = 0;
}

void Player_804A6F38::fn_80282958() {
    Cannon_8049CCF0* cannon = static_cast<Cannon_8049CCF0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd8c));
    if (cannon != NULL) {
        mPos.x = cannon->mPos.x;
        mPos.y = cannon->mPos.y;
        mPos.z = cannon->mPos.z;
        mPos.y -= 16.0f;
    }
    mPos.z = 0.0f;
    if (fn_80245DB0() || field_0xd90 == 1) {
        field_0x124d = 0;
        if (fn_80251AEC(0x4d)) {
            if (cannon != NULL) {
                cannon->fn_80107360();
            }
            fn_802685D4();
        }
    }
    fn_8024BAEC();
}

void Player_804A6F38::fn_80282A24() {
    field_0x124d = 1;
    field_0x458 = 0;
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd8c);
    if (actor != NULL) {
        mPos.x = actor->mPos.x;
        mPos.y = actor->mPos.y;
        mPos.z = actor->mPos.z;
        mPos.y -= 16.0f;
    }
    mPos.z = 0.0f;
    field_0xd90 = 0;
}

void Player_804A6F38::fn_80282AA8() {
    if (field_0x1244 == 0xf) {
        return;
    }
    if (field_0x1244 == 0x10) {
        return;
    }
    field_0xbe4 = 0;
    field_0xbe8 = 0;
}

// Update of state 0x4B: down (0x33) without life; a follower with 0x1215 takes its leader's layer
// and position (state 0x4A off view-flag layers); else a counter 0x402 grows by a random -2..7
// (moving faster than 1) or -4..5 a frame and every 50 spawns a 'PEFF' actor ahead of the player,
// the walk cycle 0x3F8 runs (double speed when fast; frames by the dash count 0x3FF), the movement
// fn_80280074 and fn_80242DAC, the facing (up with pad 0x200), and the bob 0x410 on y (+0.02 a
// frame to 0.3); it stands (0) once fn_80225BE4 fails.
void Player_804A6F38::fn_80282ACC() {
    if (fn_8024189C() <= 0 && fn_80251AEC(0x33)) {
        return;
    }
    if (fn_80240808() && field_0x1215) {
        field_0x3b4 = getPlayerLayer_8023DE58(fn_80240644());
        mPos = *getPlayerPos_8023E724(fn_80240644());
        if (!viewLayerFlag(field_0x3b4)) {
            fn_80251AEC(0x4a);
        }
        return;
    }
    s32 step;
    if (magnitude(field_0x344) > 1.0f) {
        step = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 2;
    } else {
        step = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 4;
    }
    if (field_0x402 + step >= 0) {
        field_0x402 += step;
    }
    if (field_0x402 >= 50) {
        field_0x402 = 0;
        JGeometry::TVec3f pos = mPos;
        if (field_0x3dc == 0) {
            pos.x += 6.0f;
        } else if (field_0x3dc == 1) {
            pos.x -= 6.0f;
        }
        s32 index = field_0x4;
        getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x3b4, &pos, (index & 3) | 0xC, -1, index);
    }
    if (magnitude(field_0x344) > 1.0f) {
        field_0x3f8 += 2;
    } else {
        field_0x3f8 += 1;
    }
    if (field_0x3f8 >= 24) {
        field_0x3f8 = 0;
    }
    if (field_0x3ff == 0) {
        field_0x3f9 = field_0x3f8 / 6;
    } else if (field_0x3ff < 7) {
        field_0x3f9 = 4;
    } else if (field_0x3ff < 13) {
        field_0x3f9 = 5;
    } else if (field_0x3ff < 19) {
        field_0x3f9 = 6;
    } else {
        field_0x3f9 = 7;
    }
    fn_80280074();
    fn_80242DAC();
    if (fn_80246B10(0x200)) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    } else if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        fn_8027E7E0();
    } else {
        fn_8027EB04();
    }
    mPos.y += field_0x410;
    field_0x410 += 0.02f;
    if (field_0x410 >= 0.3f) {
        field_0x410 = 0.3f;
    }
    if (!fn_80225BE4(&mPos, field_0x3b4)) {
        fn_80251AEC(0);
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80282E6C() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x414.x = 0.0f;
    field_0x414.y = 0.0f;
    field_0x414.z = 0.0f;
    field_0x3ff = 0;
    field_0x410 = 0.0f;
    field_0x402 = 0;
    mPos.z = 0.0f;
    if (field_0x1248 != 0x1b && field_0x1248 != 3 && field_0x1248 != 0x1d) {
        fn_8024F0E8(0x2003A);
    }
    if (fn_80240808()) {
        field_0x3dc = field_0x3e8 = fn_80230CFC(fn_80240644());
    }
    if (!field_0xbe8) {
        field_0xbe4 = 600;
        field_0xbe8 = 1;
    }
    if (!fn_80240670() && field_0x1248 != 0xf && field_0x1248 != 0x10) {
        if (field_0x1248 == 0x27) {
            field_0xd28 = 0;
        } else {
            field_0xd28 = field_0x3b4;
        }
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023F264(i) && fn_80243194(i) && fn_8022CAF8(i) != 4) {
                fn_8023E080(i, field_0xd28);
            }
        }
    }
}

void Player_804A6F38::fn_80282FC8() {
    field_0x42c = false;
    field_0x430 = 0;
    field_0x42d = 0;
    field_0x400 = 0;
}

// FIXME: 99.25. The distance moved: the original stores the TVec2f to the stack (0x8 / 0xC) and
// squares it unfused (fmuls, fmuls, fadds); ours keeps it in registers with one fmadds (TVec2f::length).
// Unfused square() with the same TVec2f keeps it in registers too.
// Update of state 0x4A (the 0x4B movement on view-flag-free layers): down (0x33) without life; a
// follower with 0x1215 takes its leader's layer and position (0x4B on view-flag layers). On the
// floor, the walk cycle 0x3F8; the mode 0x42C (timer 0xBE0 = 300, effects 0x15B / 0x15C, sounds
// 0x20040 on / 0x20041 off) follows the leader's (fn_8022B7C4) for a follower, else the pad
// (fn_802466B4) toggles it once the cooldown 0x430 is over; its end (0x42D, or 0xBE0 at 0) sets
// the cooldown 90, stops 0x344 and (own mode) ends the followers' (fn_80231928). Then the movement
// fn_802804E0, fn_80242DAC, the facing, back to 0 off the ground (fn_80225BE4) or with
// fn_8024B24C, the body frame, a dust effect 0x54E every 40 counts of 0x401 (by the distance
// moved since field_0x14), and the frames 0x3FB / 0x3F9 (cooldown, mode, dash count 0x3FF, walk).
void Player_804A6F38::fn_80282FE0() {
    if (fn_8024189C() <= 0 && fn_80251AEC(0x33)) {
        return;
    }
    if (fn_80240808() && field_0x1215) {
        field_0x3b4 = getPlayerLayer_8023DE58(fn_80240644());
        mPos = *getPlayerPos_8023E724(fn_80240644());
        if (viewLayerFlag(field_0x3b4)) {
            fn_80251AEC(0x4b);
        }
        return;
    }
    mPos.z = 0.0f;
    if (magnitude(field_0x344) > 1.0f) {
        field_0x3f8 += 2;
    } else {
        field_0x3f8 += 1;
    }
    if (field_0x3f8 >= 24) {
        field_0x3f8 = 0;
    }
    if (fn_80240670()) {
        if (field_0xbe0 > 0) {
            field_0xbe0--;
        }
        if (field_0x430 > 0 && !field_0x400) {
            field_0x430--;
        }
        if (field_0x42d == 1) {
            field_0x42c = false;
            field_0x42d = 0;
            field_0x430 = 90;
            field_0x344.x = 0.0f;
            field_0x344.y = 0.0f;
            field_0x344.z = 0.0f;
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
            fn_8024F0E8(0x20041);
            fn_8024F0E8(0x1000F);
        } else if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
            if (field_0x42c == true) {
                field_0x42c = false;
                field_0xbe0 = 0;
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
            }
        } else if (!fn_8022B7C4(fn_8022EA0C(field_0x4))) {
            if (field_0x42c == true) {
                field_0x42c = false;
                field_0xbe0 = 0;
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
                fn_8024F0E8(0x20041);
            }
        } else if (field_0x42c == false) {
            field_0x42c = true;
            field_0xbe0 = 300;
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
            fn_8024F0E8(0x20040);
        }
    } else {
        if (field_0x430 <= 0) {
            if (fn_802466B4()) {
                if (field_0x42c == true) {
                    field_0x42c = false;
                    field_0xbe0 = 0;
                    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
                    fn_8024F0E8(0x20041);
                } else {
                    field_0x42c = true;
                    field_0xbe0 = 300;
                    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
                    fn_8024F0E8(0x20040);
                }
            }
        } else if (!field_0x400) {
            field_0x430--;
        }
        if (field_0xbe0 > 0) {
            field_0xbe0--;
            if (field_0xbe0 == 0) {
                field_0x42c = false;
                field_0x430 = 90;
                field_0x344.x = 0.0f;
                field_0x344.y = 0.0f;
                field_0x344.z = 0.0f;
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
                fn_8024F0E8(0x20041);
                fn_8024F0E8(0x1000F);
                for (s32 i = 0; i < 4; i++) {
                    if (i != field_0x4 && fn_80243194(i)) {
                        fn_80231928(i);
                    }
                }
            }
        }
    }
    fn_802804E0();
    fn_80242DAC();
    if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        fn_8027E7E0();
    } else {
        fn_8027EB04();
    }
    if (!fn_80225BE4(&mPos, field_0x3b4) || fn_8024B24C()) {
        fn_80251AEC(0);
    }
    fn_80268B3C();
    JGeometry::TVec2f moved(mPos.x - field_0x14.x, mPos.y - field_0x14.y);
    f32 dist = moved.length();
    if (dist > 1.0f) {
        field_0x401 += 6;
    } else if (dist > 0.1f) {
        field_0x401 += 3;
    } else {
        field_0x401 += 1;
    }
    if (field_0x401 >= 40 && mPos.z == 0.0f) {
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x54e, -1, 0, NULL, NULL, true, 1.0f);
        field_0x401 = 0;
    }
    if (field_0x42c == true) {
        field_0x400++;
        if (field_0x400 > 8) {
            field_0x400 = 8;
        }
    } else if (field_0x400) {
        field_0x400--;
    }
    if (field_0x430 > 0) {
        field_0x3fb = 3;
        if (field_0x430 > 84) {
            field_0x3f9 = (90 - field_0x430) / 2;
        } else {
            field_0x3f9 = ((84 - field_0x430) / 6 & 3) + 3;
        }
    } else if (field_0x42c == true && field_0x400 >= 8) {
        field_0x3fb = 1;
        field_0x3f9 = field_0x3f8 / 6;
    } else if (field_0x3ff != 0 && field_0x3ff < 24) {
        field_0x3fb = 2;
        field_0x3f9 = field_0x3ff / 6;
    } else {
        field_0x3fb = 0;
        field_0x3f9 = field_0x3f8 / 6;
    }
}

void Player_804A6F38::fn_80283848() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x414.x = 0.0f;
    field_0x414.y = 0.0f;
    field_0x414.z = 0.0f;
    field_0x3ff = 0;
    field_0x400 = 0;
    field_0x401 = 0;
    field_0x410 = 0.0f;
    field_0x42c = false;
    field_0xbe0 = 0;
    field_0x42d = 0;
    field_0x430 = 0;
    if (field_0x44c == 0 && field_0x1248 != 0x1b && field_0x1248 != 3 && field_0x1248 != 0x1d) {
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15b, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
        fn_8024F0E8(0x2003A);
        field_0x44c = 5;
    }
}

void Player_804A6F38::fn_80283944() {}

// Update of state 0x49 (held down / struggling): a pending switch 0xCE8 moves the player to layer
// 0xCE4 at 0xCD4 (waiting while the layer changes for a non-follower and fn_80303750 holds) and
// lands it (fn_80290E90, fn_80290D58); down (0x33) without life. In mode 0x1178 2 a 36-frame cycle
// of poses 0x3FD; else the buttons (pad trigger 0xF) count 0x3FC up (in mode 1 free at 15, or 30
// with more than one player taking part: state 0), which drops one every 20 frames (0x3FA)
// without them; the frames 0x3F8 / 0x3F9 / 0x3FD run faster once 0x3FC reaches 4; then the facing.
void Player_804A6F38::fn_80283948() {
    if (field_0xce8 == true) {
        field_0x3b4 = field_0xce4;
        mPos = field_0xcd4;
        if (field_0xce4 == field_0xce0 || fn_80240808() || !fn_80303750(field_0x4)) {
            field_0xbec->fn_8030BD3C(field_0x4);
            fn_80290E90();
            fn_80290D58();
        } else {
            return;
        }
    }
    if (fn_8024189C() <= 0) {
        field_0x124d = 0;
        fn_80251AEC(0x33);
        return;
    }
    if (field_0x1178 == 2) {
        field_0x3fc++;
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
        return;
    }
    if (fn_80246D1C(0xF)) {
        field_0x3fc++;
        field_0x3fa = 0;
        field_0x3f8 = 0;
        field_0x3f9++;
        if (field_0x1178 == 1) {
            if (gGameRoot_8053AAF8->fn_80138A30() <= 1) {
                if (field_0x3fc >= 15) {
                    field_0x124d = 0;
                    fn_80251AEC(0);
                }
            } else if (field_0x3fc >= 30) {
                field_0x124d = 0;
                fn_80251AEC(0);
            }
        }
    } else {
        field_0x3fa++;
        if (field_0x3fa >= 20) {
            field_0x3fa = 0;
            if (field_0x3fc) {
                field_0x3fc--;
            }
        }
    }
    if (field_0x1178 == 0) {
        if (field_0x3fc < 4) {
            field_0x3f8++;
            if (field_0x3f8 >= 5) {
                field_0x3f8 = 0;
                field_0x3f9++;
            }
            field_0x3fd = 0;
        } else {
            field_0x3f8++;
            if (field_0x3f8 >= 6) {
                field_0x3f8 = 0;
                field_0x3f9++;
            }
            if ((field_0x3f9 & 3) == 1) {
                field_0x3fd = 1;
            } else if ((field_0x3f9 & 3) == 3) {
                field_0x3fd = 2;
            } else {
                field_0x3fd = 0;
            }
        }
    } else if (field_0x3fc < 4) {
        field_0x3f8++;
        if (field_0x3f8 >= 5) {
            field_0x3f8 = 0;
            field_0x3f9++;
        }
        field_0x3fd = field_0x3f9 & 1;
    } else {
        field_0x3f8++;
        if (field_0x3f8 >= 6) {
            field_0x3f8 = 0;
            field_0x3f9++;
        }
        if ((field_0x3f9 & 3) == 1) {
            field_0x3fd = 3;
        } else if ((field_0x3f9 & 3) == 3) {
            field_0x3fd = 4;
        } else {
            field_0x3fd = 2;
        }
    }
    fn_8027EB04();
}

void Player_804A6F38::fn_80283CEC() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80283D00() {
    fn_8027EB04();
}

void Player_804A6F38::fn_80283D20() {
    field_0x458 = 0;
}

// FIXME: 99.38. Registers only: in the follower loop the original keeps i in r30 and field_0x4 in
// r31 (cmpw r3, r31), ours the other way round.
// Exit of state 0x38: keeps mPos as the return point 0x2C4 (0x440); unless 0xCF4 is 0x24, clears
// 0xCE8 and either (no 0x43F) releases the followers of this player (fn_802384BC) and on tile 0xD
// spawns the effect 0x3D0, or keeps / clears the 0x43F carry state for state 0x1B.
void Player_804A6F38::fn_80283D2C() {
    if (field_0x440) {
        field_0x2c4.set(mPos);
    }
    if (field_0xcf4 != 0x24) {
        field_0xce8 = false;
        if (!field_0x43f) {
            if (fn_802486A0() != 4 && !fn_80240808()) {
                for (s32 i = 0; i < 4; i++) {
                    s32 index = field_0x4;
                    if (fn_8022F3DC(i) == index) {
                        fn_802384BC(i);
                    }
                }
            }
            if (fn_8022461C(&mPos, field_0x3b4) == 0xd) {
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x3d0, 0, -1, NULL, NULL, true, 1.2f);
            }
        } else if (field_0x1244 != 0x1b) {
            field_0x43f = false;
            field_0x440 = false;
            field_0x444 = 0.0f;
        } else {
            field_0x444 = field_0x344.z;
        }
    }
    field_0x441 = false;
    fn_8024BAEC();
}

// Update of state 0x38 (dropping down a ledge): moves by 0x344 while above the floor; falls 0.25 a
// frame, or 0.17 to -6 when carried (0x43F) or with 0x441; landing below the ledge's y 0x328
// (carried) lifts it there at height 8, else it stands (0, or 8 with 0xCAD) and, carried, spawns
// the landing effects 0x4F3-0x4F6 at y + 6 (half size without 0x440) and fn_80134DE4(1, 0);
// with 0x440 the falling sounds 0x1000D / 0x2003E.
void Player_804A6F38::fn_80283E70() {
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
            mPos.y += field_0x324.y - mPos.y;
            mPos.z = 8.0f;
        } else {
            mPos.z = 0.0f;
            s32 state = 0;
            if (field_0xcad == true) {
                state = 8;
            }
            field_0x124d = 0;
            if (fn_80251AEC(state) && field_0x43f) {
                JGeometry::TVec3f pos = mPos;
                pos.y += 6.0f;
                JPABaseEmitter* e0 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x4f3, -1, 0, NULL, NULL, true, 1.0f);
                JPABaseEmitter* e1 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x4f4, 0, 0, NULL, NULL, true, 1.0f);
                JPABaseEmitter* e2 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x4f5, 0, 0, NULL, NULL, true, 1.0f);
                JPABaseEmitter* e3 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x4f6, 0, 0, NULL, NULL, true, 1.0f);
                if (!field_0x440) {
                    e0->setGlobalScale(0.5f);
                    e1->setGlobalScale(0.5f);
                    e2->setGlobalScale(0.5f);
                    e3->setGlobalScale(0.5f);
                }
                gGameRoot_8053AAF8->fn_80134DE4(1, 0);
            }
        }
    }
    if (field_0x440 && field_0x344.z < 0.0f) {
        fn_8024F0E8(0x1000D);
        fn_8024F0E8(0x2003E);
    }
}

void Player_804A6F38::fn_8028414C() {
    field_0x458 = 0;
    field_0xce8 = true;
    if (!field_0x43f) {
        field_0x124d = 1;
    }
    field_0x318.x = mPos.x;
    field_0x318.y = mPos.y;
    field_0x318.z = mPos.z;
    f32 dx = field_0x324.x - mPos.x;
    f32 dy = field_0x324.y - mPos.y;
    if (field_0x43f || field_0x441) {
        mPos.z = 0.0f;
        mPos.y += dy;
        mPos.z += dy;
        field_0x344.x = 0.0f;
        field_0x344.y = 0.0f;
        if (field_0x43f && field_0x1248 == 0x1b) {
            field_0x344.z = field_0x444;
        } else {
            field_0x344.z = 2.0f;
        }
    } else {
        mPos.z = 0.0f;
        field_0x344.x = dx / 26.0f;
        field_0x344.y = dy / 26.0f;
        field_0x344.z = 3.25f;
    }
    if (field_0x1248 != 0x1b) {
        if (!field_0x440) {
            fn_8024F0E8(0x10002);
        }
        fn_8024F0E8(0x20008);
    }
}

void Player_804A6F38::fn_80284290() {}

void Player_804A6F38::fn_80284294() {
    if (field_0x3fc < 10) {
        field_0xc84.z = 0.5f + 0.05f * (10 - field_0x3fc);
    } else {
        field_0xc84.z = 0.5f;
        field_0x124d = 0;
        fn_80251AEC(0);
    }
    field_0x3fc++;
    fn_80268B3C();
}

void Player_804A6F38::fn_80284328() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
}

void Player_804A6F38::fn_8028434C() {
    mPos += field_0x344;
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -4.0f) {
        field_0x344.z = -4.0f;
    }
    if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << field_0x4)) && !fn_80240808()) {
            fn_80251AEC(0);
        } else {
            fn_80251AEC(0xc);
        }
    }
}

void Player_804A6F38::fn_80284430() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 4.0f;
    fn_8024F0E8(0x2002F);
}

void Player_804A6F38::fn_8028447C() {
    field_0x318 = mPos;
    field_0x2c.x = mPos.x;
    field_0x2c.y = mPos.y;
    field_0x2c.z = mPos.z;
    field_0x3bc = field_0x3b4;
    field_0x43e = 0;
    fn_80248604();
    field_0x3fe = 0;
    field_0x1220 = 5;
    fn_8024D134();
}

// Update of state 0x1B (walking onto the screen): steps toward the target (fn_8027E5BC, 3 for pose
// 0x3FD 3, else 2) while outside the visible box (39 in from the edges across the facing), and
// after 40 frames (0x3FE) regardless; once every present player in state 0x1B (fn_80233EDC) is inside it, carried
// (0x43F) it hands over (fn_80248A24), else it goes on by pose 0x3FD (0x12 carrying, 0x4A, 0x4B,
// 0xB, or 0), the acting player (lbl_8053A190) calling fn_80231368; the walk frames by pose, and
// with 0x440 the sounds 0x1000D / 0x2003E.
void Player_804A6F38::fn_802844F8() {
    bool allInside = true;
    field_0x3fe++;
    JGeometry::TBox2f box;
    box = getGameView_802D6A9C()->mVisibleBox;
    if (field_0x3dc == 3 || field_0x3dc == 2) {
        box.i.y += 39.0f;
        box.f.y -= 39.0f;
    } else {
        box.i.x += 39.0f;
        box.f.x -= 39.0f;
    }
    if (!contains(box, mPos) || field_0x3fe >= 40) {
        if (field_0x3fd == 3) {
            fn_8027E5BC(3.0f);
        } else {
            fn_8027E5BC(2.0f);
        }
    }
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i) && fn_80233EDC(i) && !contains(box, *getPlayerPos_8023E724(i))) {
            allInside = false;
            break;
        }
    }
    if (allInside) {
        field_0x124d = 0;
        if (field_0x43f) {
            fn_80248A24();
            if (lbl_8053A190 == field_0x4) {
                fn_80231368();
            }
        } else {
            if (fn_8025F684()) {
                fn_80251AEC(0x12);
            } else if (field_0x3fd == 1) {
                fn_80251AEC(0x4a);
            } else if (field_0x3fd == 2) {
                fn_80251AEC(0x4b);
            } else if (field_0x3fd == 3) {
                fn_80251AEC(0xb);
            } else {
                fn_80251AEC(0);
            }
            if (lbl_8053A190 == field_0x4) {
                fn_80231368();
            }
        }
    }
    if (!field_0x43f) {
        if (fn_8025F684()) {
            fn_8026152C(false);
        } else if (field_0x3fd == 0) {
            field_0x3f8++;
            if (field_0x3f8 >= 16) {
                field_0x3f8 = 0;
            }
            field_0x3f9 = field_0x3f8 / 2;
        } else if (field_0x3fd == 3) {
            field_0x3f8++;
            if (field_0x3f8 >= 19) {
                field_0x3f8 = 0;
            }
            if (field_0x3f8 < 7) {
                field_0x3f9 = 0;
            } else if (field_0x3f8 < 12) {
                field_0x3f9 = 1;
            } else {
                field_0x3f9 = 2;
            }
        } else {
            field_0x3f8++;
            if (field_0x3f8 >= 24) {
                field_0x3f8 = 0;
            }
            field_0x3f9 = field_0x3f8 / 6;
        }
    }
    if (field_0x440) {
        fn_8024F0E8(0x1000D);
        fn_8024F0E8(0x2003E);
    }
}

void Player_804A6F38::fn_802848F0() {
    field_0x124d = 1;
    field_0xce8 = false;
    field_0x458 = 0;
    if (!field_0x43f) {
        mPos.z = 0.0f;
    }
    if (field_0x1248 == 0x4a) {
        field_0x3fd = 1;
    } else if (field_0x1248 == 0x4b) {
        field_0x3fd = 2;
    } else if (field_0x1248 == 0xb) {
        field_0x3fd = 3;
    } else {
        field_0x3fd = 0;
    }
    field_0x3fe = 0;
}

void Player_804A6F38::fn_8028496C() {
    field_0xda4 = -1;
}

// Update of state 0x1A (pushing / pulling a block): standing (0) when the tile ahead (+10 along
// the facing) is free or tile 0xE / 0x39 / 0xD and neither fn_802684A4 nor fn_8024F63C holds, or
// when the tile behind is blocked; to 0xE while the grabbed actor (fn_8024FF84) moves; the pushed
// actor 0xDA4 (fn_8024FF6C) carries the player along while it moves (0.1 or more a frame), else
// letting go (fn_802456FC) stands and pressing the facing's pad bit (lbl_8046BE08) goes to 0xD;
// the push frames (sound 0x10012 at 16 of the 64-frame cycle) while fn_8024FEFC or carried.
void Player_804A6F38::fn_80284978() {
    const Vec& ahead = lbl_8046BE18[field_0x3dc];
    JGeometry::TVec3f front;
    front.x = ahead.x;
    front.y = ahead.y;
    front.z = ahead.z;
    front += mPos;
    const Vec& behind = lbl_8046BE48[field_0x3dc];
    JGeometry::TVec3f back;
    back.x = behind.x;
    back.y = behind.y;
    back.z = behind.z;
    back += mPos;
    u16 tile = fn_8022461C(&front, field_0x3b4);
    if ((!fn_802265DC(&front, field_0x3b4, true) || tile == 0xe || tile == 0x39 || tile == 0xd) &&
        !fn_802684A4(&front, field_0x3b4) && !fn_8024F63C()) {
        fn_80251AEC(0);
    } else if (fn_802265DC(&back, field_0x3b4, true) || fn_802684A4(&back, field_0x3b4)) {
        fn_80251AEC(0);
    } else {
        if (fn_8024FF84() >= 0) {
            ActorBase_804A5770* grabbed = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8024FF84());
            if (grabbed != NULL && magnitude(grabbed->mPos - grabbed->mPrevPos) >= 0.1f) {
                fn_80251AEC(0xe);
                return;
            }
        }
        bool carried = false;
        if (field_0xda4 >= 0) {
            ActorBase_804A5770* pushed = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8024FF6C());
            if (pushed != NULL) {
                if (magnitude(pushed->mPos - pushed->mPrevPos) >= 0.1f) {
                    carried = true;
                    mPos += pushed->field_0x3c;
                } else if (!fn_802456FC()) {
                    fn_80251AEC(0);
                } else if (fn_802456FC() && fn_80246D1C(lbl_8046BE08[field_0x3dc])) {
                    fn_80251AEC(0xd);
                }
            } else {
                field_0xda4 = -1;
                if (fn_802456FC() && fn_80246D1C(lbl_8046BE08[field_0x3dc])) {
                    fn_80251AEC(0xd);
                }
            }
        } else if (!fn_802456FC()) {
            fn_80251AEC(0);
        } else if (fn_802456FC() && fn_80246D1C(lbl_8046BE08[field_0x3dc])) {
            fn_80251AEC(0xd);
        }
        if (fn_8024FEFC() || carried) {
            field_0x3f8++;
            if (field_0x3f8 == 16) {
                fn_8024F0E8(0x10012);
            } else if (field_0x3f8 >= 64) {
                field_0x3f8 = 0;
            }
            if (field_0x3f8 >= 32) {
                field_0x3f9 = (field_0x3f8 - 32) / 8;
            } else {
                field_0x3f9 = field_0x3f8 / 8;
            }
        } else {
            field_0x3f8 = 0;
            field_0x3f9 = 0;
        }
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80284DD0() {
    field_0x458 = 0;
    field_0xda4 = -1;
}

void Player_804A6F38::fn_80284DE4() {
    field_0xd94 = -1;
    field_0xd9c = -1;
    field_0xd98 = 0;
    field_0x1170 = 60;
}

// Update of state 0x0E (moving with a pushed block): standing (0) when the tile 12 ahead is free
// or tile 0xE / 0x39 / 0xD and neither fn_802684A4 nor fn_80250090 holds; while the grabbed actor
// (fn_8024FF84) moves (0.1 or more a frame) the player moves with it (its 0x3C) through a
// 128-frame cycle 0x3F8 (frames lbl_8046C098 by facing, sound 0x10012 at 16); else back to 0x1A
// (still holding, fn_802456FC, without fn_80244630) or 0xD.
void Player_804A6F38::fn_80284E04() {
    const Vec& ahead = lbl_8046BDD8[field_0x3dc];
    JGeometry::TVec3f front;
    front.x = ahead.x;
    front.y = ahead.y;
    front.z = ahead.z;
    front += mPos;
    u16 tile = fn_8022461C(&front, field_0x3b4);
    if ((!fn_802265DC(&front, field_0x3b4, true) || tile == 0xe || tile == 0x39 || tile == 0xd) &&
        !fn_802684A4(&front, field_0x3b4) && !fn_80250090()) {
        fn_80251AEC(0);
    } else {
        ActorBase_804A5770* grabbed = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8024FF84());
        if (grabbed != NULL && magnitude(grabbed->mPos - grabbed->mPrevPos) >= 0.1f) {
            mPos += grabbed->field_0x3c;
            field_0x3f8++;
            if (field_0x3f8 >= 128) {
                field_0x3f8 = 0;
            }
            if (field_0x3f8 >= 64) {
                field_0x3f9 = lbl_8046C098[field_0x3dc][(field_0x3f8 - 64) / 8];
            } else {
                field_0x3f9 = lbl_8046C098[field_0x3dc][field_0x3f8 / 8];
            }
            if (field_0x3f8 == 16) {
                fn_8024F0E8(0x10012);
            }
        } else if (fn_802456FC() && !fn_80244630()) {
            fn_80251AEC(0x1a);
        } else {
            fn_80251AEC(0xd);
        }
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_80285068() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285074() {
    if (field_0x1244 == 0xe) {
        return;
    }
    field_0xd94 = -1;
    field_0xd9c = -1;
    field_0xd98 = 0;
}

// Update of state 0x0D (holding a block): standing (0) when the tile 10 ahead is free or tile
// 0xE / 0x39 / 0xD and neither fn_802684A4 nor fn_80250090 holds; to 0xE while the grabbed actor
// (fn_8024FF84) moves; else 0xD94 = -1, the stick moves the player at 1.5 along the facing's axis
// (not with pad 0x40), and a turn or the end of the push count (cls_804A71A8 0xA0) stands (0) and
// clears 0xD98 / 0xD9C and the counts 0xA0 / 0xA4; then pick-ups and the 128-frame cycle 0x3F8
// (frames lbl_8046C098 by facing, sound 0x10012 at 16).
void Player_804A6F38::fn_80285098() {
    const Vec& ahead = lbl_8046BDA8[field_0x3dc];
    JGeometry::TVec3f front;
    front.x = ahead.x;
    front.y = ahead.y;
    front.z = ahead.z;
    front += mPos;
    u16 tile = fn_8022461C(&front, field_0x3b4);
    if ((!fn_802265DC(&front, field_0x3b4, true) || tile == 0xe || tile == 0x39 || tile == 0xd) &&
        !fn_802684A4(&front, field_0x3b4) && !fn_80250090()) {
        fn_80251AEC(0);
    } else {
        ActorBase_804A5770* grabbed = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8024FF84());
        if (grabbed != NULL && magnitude(grabbed->mPos - grabbed->mPrevPos) >= 0.1f) {
            fn_80251AEC(0xe);
        } else {
            field_0xd94 = -1;
            fn_80275D9C();
            if (!fn_80246B10(0x40)) {
                if (field_0x3dc == 0 || field_0x3dc == 1) {
                    mPos.x += 1.5f * fn_8022C82C(field_0x4);
                } else {
                    mPos.y += 1.5f * -fn_8022C76C(field_0x4);
                }
            }
            s32 facing = field_0x3dc;
            fn_8027EB04();
            if (facing != field_0x3dc || field_0xbf0->field_0xa0 == 0) {
                fn_80251AEC(0);
                field_0xd9c = -1;
                field_0xd98 = 0;
                field_0xbf0->field_0xa0 = 0;
                field_0xbf0->field_0xa4 = 0;
            }
            fn_80261480();
            field_0x3f8++;
            if (field_0x3f8 >= 128) {
                field_0x3f8 = 0;
            }
            if (field_0x3f8 >= 64) {
                field_0x3f9 = lbl_8046C098[field_0x3dc][(field_0x3f8 - 64) / 8];
            } else {
                field_0x3f9 = lbl_8046C098[field_0x3dc][field_0x3f8 / 8];
            }
            if (field_0x3f8 == 16) {
                fn_8024F0E8(0x10012);
            }
        }
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_8028537C() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285388() {}

// Update of state 3 (running / following): a follower on its leader's layer just takes the leader's
// layer and position (0x1215); otherwise it moves (followStep), then the facing, the walk cycle,
// pick-ups and the body frame.
void Player_804A6F38::fn_8028538C() {
    JGeometry::TVec3f target;
    fn_8027FA08(&target);
    if (target.z == 0.0f && mPos.z != 0.0f && !field_0x434) {
        mPos.z = 0.0f;
    }
    if (fn_80240808() && field_0x1215) {
        field_0x3b4 = getPlayerLayer_8023DE58(fn_80240644());
        mPos = *getPlayerPos_8023E724(fn_80240644());
        return;
    }
    fn_80275D9C();
    followStep();
    fn_8027EB04();
    fn_8027EEB4();
    fn_80261480();
    fn_80268B3C();
}

void Player_804A6F38::fn_80285668() {
    if (field_0x1248 == 2) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

// Update of state 2 (walking): a follower runs (3) with the leader's pad 0x40, or while it or its
// leader moves; then the stick walk step at 1.5, the facing, the walk cycle, pick-ups, body frame.
// The follow target (fn_8027FA08) is computed and not used.
void Player_804A6F38::fn_80285690() {
    if (!fn_80275D9C() && fn_80240808()) {
        JGeometry::TVec3f target;
        fn_8027FA08(&target);
        if (fn_8022C5EC(fn_80240644(), 0x40)) {
            fn_80251AEC(3);
        } else if (fn_8022C52C(fn_80240644(), 0x40) && fn_8022C6AC(field_0x4) != 0.0f) {
            fn_80251AEC(3);
        } else if (fn_8022C6AC(fn_80240644()) == 0.0f && fn_8022CFE8(field_0x4) > 0.0f) {
            // Read before the call: the original loads the layer first (declaration order only steers scheduling).
            s32 layer = field_0x3b4;
            s32 leaderLayer = getPlayerLayer_8023DE58(fn_80240644());
            if (leaderLayer == layer) {
                fn_80251AEC(3);
            }
        }
    }
    fn_802808E8(1.5f);
    fn_8027EB04();
    fn_8027EEB4();
    fn_80261480();
    fn_80268B3C();
}

void Player_804A6F38::fn_802857C8() {
    if (field_0x1248 == 3) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

// Update of a standing state: drops to the floor height 0 unless 0x434; on tiles 3 / 0xE / 0x3A the
// effect 0x503 at the old and at the return point 0x2C4 (layer 0); faces the nearest player; a
// leaderless player whose box touches another present player that can pair up (fn_802300EC) follows
// it (0xD60) and spawns 'TSNO' (two-sword motion) when state 0 can be requested.
void Player_804A6F38::fn_802857F0() {
    if (mPos.z > 0.0f && !field_0x434) {
        mPos.z = 0.0f;
    }
    u16 tile = fn_8022461C(&mPos, field_0x3b4);
    if (tile == 3 || tile == 0xe || tile == 0x3a) {
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x503, 1, 1, NULL, NULL, true, 1.0f);
        field_0x3b4 = 0;
        mPos.set(field_0x2c4);
        spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x503, 1, 1, NULL, NULL, true, 1.0f);
    }
    fn_8027E7E0();
    fn_80268B3C();
    for (s32 i = 0; i < 4; i++) {
        if (i == field_0x4) {
            continue;
        }
        if (!(gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i))) {
            continue;
        }
        if (fn_8023EA28(i) || !fn_802300EC(i) || !fn_8023B0F4(i, &field_0x4c0, field_0x3b4, false, 1.0f)) {
            continue;
        }
        if (field_0xd60 >= 0) {
            continue;
        }
        field_0xd60 = i;
        if (fn_80251AEC(0)) {
            field_0x11ec = i;
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_TSNO, field_0x3b4, &mPos, i & 3, -1, field_0x4);
            return;
        }
        field_0xd60 = -1;
    }
}

void Player_804A6F38::fn_802859B8() {
    field_0x458 = 0;
}

// Update of state 0 (standing): walk (2) or run (3) when the stick or the leader moves, then the
// facing (toward the nearest player as player 4 of a group outside the leader's slot, else
// fn_8027EB04), the pick-up checks and the body frame.
void Player_804A6F38::fn_802859C4() {
    if (mPos.z != 0.0f && !field_0x434) {
        mPos.z = 0.0f;
    }
    if (!fn_80275D9C()) {
        if (fn_80240808()) {
            if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
                if (fn_80246F28() != 0.0f) {
                    fn_80251AEC(2);
                }
            } else if (fn_8022C5EC(fn_80240644(), 0x40)) {
                fn_80251AEC(3);
            } else if (fn_8022C6AC(field_0x4) != 0.0f) {
                fn_80251AEC(3);
            } else if (fn_8022C6AC(fn_80240644()) == 0.0f && fn_8022CFE8(field_0x4) > 0.0f) {
                // Read before the call: the original loads the layer first (declaration order only steers scheduling).
                s32 layer = field_0x3b4;
                s32 leaderLayer = getPlayerLayer_8023DE58(fn_80240644());
                if (leaderLayer == layer) {
                    fn_80251AEC(3);
                }
            }
        } else if (fn_802486A0() == 3) {
            if (fn_80246F28() != 0.0f) {
                if (fn_80246B10(0x40)) {
                    fn_80251AEC(3);
                } else {
                    fn_80251AEC(2);
                }
            } else if (fn_8022CFE8(field_0x4) != 0.0f) {
                fn_80251AEC(3);
            }
        } else if (fn_80246F28() != 0.0f) {
            fn_80251AEC(2);
        }
    }
    if (fn_802486A0() == 4 && fn_80240888() != fn_80248778()) {
        fn_8027E7E0();
    } else {
        fn_8027EB04();
        if (field_0x434) {
            fn_80251AEC(2);
        }
    }
    fn_80261480();
    fn_80268B3C();
}

void Player_804A6F38::fn_80285C28() {
    field_0x458 = 0;
}
