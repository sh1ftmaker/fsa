#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/act/act_zora_escort.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_80127E20.h"
#include "game/tu_80131D98.h"

// The escort's animation keys: (cel, frames) s16 pairs, ended by a cel of -1.
static const s16 lbl_80464C58[10] = {2, 6, 1, 4, 3, 6, 1, 4, -1, -1};
static const s16 lbl_80464C6C[10] = {6, 6, 4, 4, 5, 6, 4, 4, -1, -1};
static const s16 lbl_80464C80[10] = {8, 6, 7, 4, 9, 6, 7, 4, -1, -1};
static const s16 lbl_80464C94[10] = {11, 6, 10, 4, 12, 6, 10, 4, -1, -1};
static const s16 lbl_80464CA8[12] = {1, 20, 4, 20, 1, 20, 4, 20, 0, 3, -1, -1};
static const s16 lbl_8053C930[2] = {-1, -1};
static const s16 lbl_8053C934[4] = {1, 10, -1, -1};
static const s16 lbl_8053C93C[4] = {4, 10, -1, -1};
static const s16 lbl_8053C944[4] = {0, 10, -1, -1};
static const s16 lbl_8053C94C[4] = {10, 10, -1, -1};
static const s16 lbl_8053C954[2] = {-1, -1};
static const s16 lbl_8053C958[2] = {-1, -1};
static const s16 lbl_8053C95C[2] = {-1, -1};
static const s16 lbl_8053C960[2] = {-1, -1};

// The key tables of mAnim, in address order.
static const s16* lbl_8049DF40[14] = {
    lbl_80464C58, lbl_80464C6C, lbl_80464C80, lbl_80464C94, lbl_8053C930, lbl_8053C934, lbl_8053C93C,
    lbl_8053C944, lbl_8053C94C, lbl_80464CA8, lbl_8053C954, lbl_8053C958, lbl_8053C95C, lbl_8053C960,
};

ZoraEscort_8049DF78::ZoraEscort_8049DF78() : EscortBase_8049E0C8("enemy_both2_sob.bin", 0x242, 0) {
    field_0x3c4 = 0;
    field_0x3c8 = 0;
    field_0x3cc = -1;
    field_0x3d0 = 0;
}

ZoraEscort_8049DF78::~ZoraEscort_8049DF78() {}

void ZoraEscort_8049DF78::init() {
    fn_80124448();
    if ((field_0x90 >> 2) & 1) {
        field_0x344 = (field_0x90 >> 3) & 0xF;
        fn_80131F0C()->fn_80131EC0(this);
        vfunc_0x6C(7);
    }
}

void ZoraEscort_8049DF78::update() {
    if (field_0x33c != 0xF) {
        EscortBase_8049E0C8::update();
    }
}

bool ZoraEscort_8049DF78::vfunc_0x54() {
    switch (field_0x33c) {
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
        return true;
    }
    return false;
}

bool ZoraEscort_8049DF78::vfunc_0x58() {
    switch (field_0x33c) {
    case 7:
    case 11:
    case 12:
        return fn_80121DC8(fn_8012285C()) <= 32.0f;
    case 13: // nothing sets 13 or 14 here
    case 14:
        break;
    }
    return false;
}

s32 ZoraEscort_8049DF78::vfunc_0x5C(const s32*) {
    if (field_0x33c == 0 || field_0x33c == 0xF) {
        return 3;
    }
    bool flag = vfunc_0x58();
    s32 result = 1;
    if (flag) {
        result = 2;
    }
    return result;
}

void ZoraEscort_8049DF78::vfunc_0x64() {
    if (field_0x90 & 1) {
        field_0x3c4 = 1;
    }
    field_0x35c = (field_0x90 >> 8) & 0xFF;
    field_0x3d0 = (field_0x90 >> 16) & 0xFF;
}

// FIXME: 99.57% (try.py: scheduling+regalloc). atan2_ argument load order in
// stepToPlayer and r29<->r30 swap in stepToPlayer and hasNearbyBari.
void ZoraEscort_8049DF78::vfunc_0x68() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    switch (field_0x33c) {
    case 0: {
        vfunc_0x98(0, 1, 64.0f);
        s32 nearPlayer = findNearbyPlayer();
        if (nearPlayer >= 0 && nearPlayer != field_0x3cc) {
            if (!fn_8023F264(nearPlayer)) {
                return;
            }
            if (!fn_802300EC(nearPlayer)) {
                return;
            }
            if (fn_802308E0(nearPlayer)) {
                field_0x3cc = nearPlayer;
                if (fn_8022F514(nearPlayer)) {
                    fn_8022F3DC(nearPlayer);
                }
                field_0x344 = nearPlayer;
                vfunc_0x6C(2);
                break;
            }
        }
        u16 msg = 0x2B7;
        if (fn_80131F0C()->mCount == 1) {
            msg = 0x2B4;
        }
        s32 player = fn_80122898(msg);
        if (player >= 0) {
            if (fn_8022F514(player)) {
                player = fn_8022F3DC(player);
            }
            field_0x344 = player;
            vfunc_0x6C(1);
        }
        break;
    }

    case 1:
        fn_8022FF8C(field_0x34c, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x34c, mSerial)) {
            vfunc_0x6C(8);
        }
        break;

    case 2:
        field_0x3c8++;
        if (field_0x3c8 >= 60) {
            if (fn_80131F0C()->mCount == 1) {
                vfunc_0x6C(4);
            } else {
                fn_8022FE80(field_0x3cc, field_0x4, mSerial, false, false);
                if (field_0x3cc == fn_8022FD8C(mSerial)) {
                    if (fn_8038AD24(field_0x3cc, 0x2B3, mSerial, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 2)) {
                        vfunc_0x6C(3);
                        field_0x3c8 = 0;
                    }
                }
            }
        }
        break;

    case 3:
        fn_8022FE80(field_0x3cc, field_0x4, mSerial, false, false);
        if (fn_80388CAC(field_0x3cc, mSerial)) {
            vfunc_0x6C(4);
        }
        break;

    case 4:
        if (!stepToPlayer()) {
            fn_80122730(field_0x98, false, false);
        } else {
            fn_8022FE80(field_0x3cc, field_0x4, mSerial, false, false);
            if (field_0x3cc == fn_8022FD8C(mSerial)) {
                u16 msg = 0x2B7;
                if (fn_80131F0C()->mCount == 1) {
                    msg = 0x2B4;
                }
                if (fn_8038AD24(field_0x3cc, msg, mSerial, field_0x4, JGeometry::TVec3f(0.0f, 0.0f, 0.0f), 2)) {
                    vfunc_0x6C(5);
                }
            }
        }
        break;

    case 5:
        fn_8022FE80(field_0x3cc, field_0x4, mSerial, false, false);
        if (fn_80388CAC(field_0x3cc, mSerial)) {
            fn_8023061C(field_0x3cc, false);
            vfunc_0x6C(8);
        }
        break;

    case 6:
    case 8:
        break;

    case 7:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
            break;
        }
        if (hasNearbyBari() && !getGameView_802D6A9C()->fn_802DCD0C() && fn_8011921C()->fn_801186B0(0xE)) {
            cls_801192AC* seq = fn_8011921C();
            seq->field_0x2 = 1;
            fn_8011921C()->fn_80118454();
            lbl_8053AD70->fn_8037AE00();
            for (s32 i = 0; i < fn_80131F0C()->mCount; i++) {
                EscortBase_8049E0C8* escort = static_cast<EscortBase_8049E0C8*>(fn_80131F0C()->fn_80131E30(i));
                if (escort != NULL) {
                    escort->vfunc_0x6C(13);
                }
            }
        } else {
            if (gGameRoot_8053AAF8->fn_80138A30() > 1) {
                vfunc_0x9C(fn_8012285C(), 1.2f, 0.6f, 2.0f, 0.2f, 16.0f);
            } else {
                vfunc_0x9C(fn_8012285C(), 1.2f, 0.6f, 2.0f, 0.2f, 8.0f);
            }
        }
        break;

    case 9:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
        } else if (!fn_8022D75C(fn_8012285C()) && field_0x4 == getPlayerLayer_8023DE58(fn_8012285C())) {
            vfunc_0x6C(10);
        }
        break;

    case 10:
        if (fn_8012285C() < 0) {
            vfunc_0x6C(0);
        }
        if (fn_80122804()) {
            vfunc_0x6C(7);
        }
        break;

    case 11:
        fn_80122548();
        if (!getGameView_802D6A9C()->fn_802DCD0C()) {
            vfunc_0x6C(7);
        }
        break;

    case 12:
        field_0x4 = getPlayerLayer_8023DE58(fn_8012285C());
        if (!fn_8022D75C(fn_8012285C()) && fn_801216D0(fn_8012285C()) < 0) {
            vfunc_0x6C(7);
        } else {
            fn_80122148();
        }
        break;

    case 13:
        field_0x3c8++;
        if (field_0x3c8 >= 60) {
            vfunc_0x6C(14);
        }
        break;

    case 14:
        if (vfunc_0x3C() == 0) {
            vfunc_0x6C(7);
        } else {
            field_0x380 = 0.7f;
            field_0x37c = 1.0f;
            if (vfunc_0xC8(JGeometry::TVec3f(0.0f, 0.0f, 0.0f), field_0x380)) {
                vfunc_0x6C(15);
            }
        }
        break;

    case 15:
        break;
    }
}

bool ZoraEscort_8049DF78::hasNearbyBari() {
    if (fn_80131F0C()->mCount <= 1) {
        return false;
    }
    f32 distSq = 300.0f;
    s32 serial = getActorManager_801F856C()->fn_801F778C(ACTOR_BARI, &mPos, &distSq);
    if (serial < 0) {
        return false;
    }
    if (distSq > 90000.0f) {
        return false;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return false;
    }
    if (!actor->mAlive) {
        return false;
    }
    return true;
}

s32 ZoraEscort_8049DF78::findNearbyPlayer() {
    if (field_0x33c != 0) {
        return -1;
    }
    u16 angle = 0;
    s32 dir = 0;
    s32 player = 0;
    f32 dist = 0.0f;
    fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
    if (player < 0 || player >= 4) {
        return -1;
    }
    if (dist >= 48.0f) {
        return -1;
    }
    return player;
}

// FIXME: 99.75% (try.py: regalloc-only). f3<->f5 swap in the second
// JGeometry::TUtilf::sqrt in case 13.
s32 ZoraEscort_8049DF78::vfunc_0x6C(s32 state) {
    field_0x330 = 0;
    switch (state) {
    case 0:
        field_0x330 = 1;
        mAnim.startAnim_801D22DC(7, 2, false);
        break;
    case 2:
        field_0x3c8 = 0;
        field_0x330 = 1;
        fn_801220C4();
        break;
    case 3:
        field_0x330 = 1;
        break;
    case 4:
        field_0x331 = 0;
        break;
    case 5:
        field_0x330 = 1;
        if (viewLayerFlag(field_0x4)) {
            mAnim.startAnim_801D22DC(6, 2, false);
        } else {
            mAnim.startAnim_801D22DC(7, 2, false);
        }
        break;
    case 7:
        field_0x11e = 1;
        break;
    case 8: {
        fn_801F3668();
        u32 arg = field_0x90;
        arg |= 4;
        arg |= fn_8012285C() << 3;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FAZR, field_0x4, &mPos, arg, -1, -1);
        break;
    }
    case 9:
        fn_8012202C();
        break;
    case 10:
        fn_80121FA8();
        mAnim.startAnim_801D22DC(2, 2, false);
        break;
    case 11:
        mAnim.startAnim_801D22DC(fn_80230CFC(fn_8012285C()), 2, false);
        break;
    case 12:
        mAnim.startAnim_801D22DC(fn_80230CFC(fn_8012285C()), 2, false);
        break;
    case 13: {
        JGeometry::TVec3f pos2;
        JGeometry::TVec3f pos1(mPos);
        pos2 = mPos;
        for (s32 serial = getActorManager_801F856C()->fn_801F7A08(ACTOR_RAIL); serial != -1;
             serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_RAIL, serial)) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (!actor) {
                break;
            }
            s32 railId = (actor->field_0x90 >> 8) & 0xFF;
            if (railId == field_0x35c) {
                pos1 = actor->mPos;
            } else if (railId == field_0x3d0) {
                pos2 = actor->mPos;
            }
        }
        f32 dist1 = JGeometry::TUtilf::sqrt(square(mPos.x - pos1.x) + square(mPos.y - pos1.y));
        f32 dist2 = JGeometry::TUtilf::sqrt(square(mPos.x - pos2.x) + square(mPos.y - pos2.y));
        if (dist1 > dist2) {
            if (field_0x3c4) {
                fn_801283E8()->fn_80127ED8(&pos2);
            } else {
                fn_801283E8()->fn_80127E20(&pos2);
            }
            field_0x35c = field_0x3d0;
        } else {
            if (field_0x3c4) {
                fn_801283E8()->fn_80127ED8(&pos1);
            } else {
                fn_801283E8()->fn_80127E20(&pos1);
            }
        }
        fn_80121D68();
        fn_801220C4();
        break;
    }
    case 14:
        field_0x3c8 = 0;
        break;
    case 15:
        getGameView_802D6A9C()->fn_802D8050(vfunc_0x3C(), true, 0xFF);
        field_0x11e = 0;
        mDrawEnabled = false;
        mAlive = false;
        lbl_80539F30 = -1;
        fn_80131F0C()->fn_80131EE4();
        break;
    }
    mAnim.fn_801D1F14(0.0f);
    return EscortBase_8049E0C8::vfunc_0x6C(state);
}

bool ZoraEscort_8049DF78::stepToPlayer() {
    JGeometry::TVec3f targetPos = *getPlayerPos_8023E724(field_0x3cc);
    targetPos.z = 0.0f;
    if (viewLayerFlag(field_0x4)) {
        targetPos.x += 20.0f;
    } else {
        targetPos.y -= 20.0f;
    }
    u16 angle = (s32)(JMath::atanTable_.atan2_(targetPos.x - mPos.x, targetPos.y - mPos.y) *
                      (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
    field_0x98 = fn_801F681C(angle);
    f32 stepX = 0.7f * JMath::sincosTable_.sinShort(angle);
    f32 stepY = 0.7f * JMath::sincosTable_.cosShort(angle);
    bool arrived = true;
    if (fn_801CD950(&mPos.x, targetPos.x, stepX) != 0 && stepX != 0.0f) {
        arrived = false;
    }
    if (!viewLayerFlag(field_0x4)) {
        if (fn_801CD950(&mPos.y, targetPos.y, stepY) != 0 && stepY != 0.0f) {
            arrived = false;
        }
    }
    return arrived;
}

void ZoraEscort_8049DF78::vfunc_0x70() {
    mAnim.fn_801D2608(lbl_8049DF40, 14);
    mAnim.startAnim_801D22DC(7, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

void ZoraEscort_8049DF78::vfunc_0x74() {
    fn_80121DB0();
}

s32 ZoraEscort_8049DF78::vfunc_0x84() {
    return vfunc_0x6C(0xB);
}

s32 ZoraEscort_8049DF78::vfunc_0x88() {
    return vfunc_0x6C(9);
}

void ZoraEscort_8049DF78::vfunc_0x8C() {
    vfunc_0x6C(0xC);
}

void ZoraEscort_8049DF78::vfunc_0x9C(s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6) {
    EscortBase_8049E0C8::vfunc_0x9C(arg1, arg2, arg3, arg4, arg5, arg6);
}

s32 ZoraEscort_8049DF78::vfunc_0xC0() {
    return 1;
}

bool ZoraEscort_8049DF78::vfunc_0xC4() {
    if (fn_80131F0C()->mCount < 2) {
        return false;
    }
    ActorBase_804A5770* other = fn_80131F0C()->fn_80131E30(1);
    if (other == NULL) {
        return false;
    }
    return mSerial == other->mSerial;
}

void ZoraEscort_8049DF78::vfunc_0x94() {}

void ZoraEscort_8049DF78::vfunc_0x90() {}

s32 ZoraEscort_8049DF78::vfunc_0x78() const {
    return 0;
}

u8 ZoraEscort_8049DF78::vfunc_0x60() {
    return 0;
}
