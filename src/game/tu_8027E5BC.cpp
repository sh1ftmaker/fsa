#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/cls_804A71A8.h"
#include "game/EmitterDelete.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_cannon.h"

// Componentwise TUtil::epsilonEquals of two vectors.
static inline bool epsilonEqualsXY(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (JGeometry::TUtilf::epsilonEquals(a.x, b.x, eps) && JGeometry::TUtilf::epsilonEquals(a.y, b.y, eps)) {
        equal = true;
    }
    return equal;
}

static inline bool epsilonEquals(const JGeometry::TVec3f& a, const JGeometry::TVec3f& b, f32 eps) {
    bool equal = false;
    if (epsilonEqualsXY(a, b, eps) && JGeometry::TUtilf::epsilonEquals(a.z, b.z, eps)) {
        equal = true;
    }
    return equal;
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
    if (!(lbl_8053AAF8->field_0x82 & (u8)(1 << field_0x4))) {
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
        if (!lbl_8053AAF8->field_0x7e) {
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

void Player_804A6F38::fn_80281E84() {
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
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

void Player_804A6F38::fn_80282FC8() {
    field_0x42c = false;
    field_0x430 = 0;
    field_0x42d = 0;
    field_0x400 = 0;
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
        if (!(lbl_8053AAF8->field_0x82 & (u8)(1 << field_0x4)) && !fn_80240808()) {
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

void Player_804A6F38::fn_8028537C() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285388() {}

void Player_804A6F38::fn_80285668() {
    if (field_0x1248 == 2) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

void Player_804A6F38::fn_802857C8() {
    if (field_0x1248 == 3) {
        field_0x3f8 = field_0x437;
        field_0x3f9 = field_0x438;
    }
    field_0x458 = 0;
}

void Player_804A6F38::fn_802859B8() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80285C28() {
    field_0x458 = 0;
}
