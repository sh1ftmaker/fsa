#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/tu_8030AE84.h"
#include "game/ViewLayer.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Map.h"
#include "JSystem/JParticle/JPAMath.h"

// The emitters' y rotation per facing (0x3DC: right, left, down, up), read
// by fn_80290FC0.
static const s32 lbl_8046C228[4] = {0x4000, 0xC000, 0x0000, 0x8000};

// The spin while rising or dropping (updates of 0x2A, 0x2B, 0x22, 0x1F:
// fn_80291B8C, fn_80291910, fn_80293398, fn_80293900): the facing 0x3DC/0x3E8
// turns a quarter every four counts of 0x3FC (down, left, up, right).
inline void Player_804A6F38::spinFacing() {
    s32 turn = (field_0x3fc >> 2) & 3;
    if (turn == 0) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    } else if (turn == 1) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else if (turn == 2) {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    } else {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    }
}

// The walk cycle (fn_80292198, fn_802924AC, stepTransitionAnim): 0x3F8
// counts 0-23, the frame 0x3F9 is a quarter of it.
inline void Player_804A6F38::stepWalkCycle() {
    field_0x3f8++;
    if (field_0x3f8 >= 24) {
        field_0x3f8 = 0;
    }
    field_0x3f9 = field_0x3f8 / 6;
}

// Shared by the updates of states 0x2A and 0x22 (fn_80291B8C, fn_80293398):
// the player rises 4 a frame; above 128 the position on the TV is kept
// (0xCB0), and once `state` is granted it lands at 0xCD4 on layer 0xCE4.
inline void Player_804A6F38::riseToTarget(s32 state) {
    mPos.z += 4.0f;
    if (mPos.z > 128.0f) {
        if (field_0x3b4 < 8) {
            field_0xcb0 = mPos;
        }
        field_0x124d = 0;
        if (fn_80251AEC(state)) {
            fn_80241454(0);
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            mPos.z = 0.0f;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    }
}

// The camera follows a point 150 above the player while it is on the TV
// (fn_802929FC, fn_80292ECC, fn_80293398, fn_802935A4).
inline void Player_804A6F38::followCamera() {
    if (field_0x3b4 < 8) {
        JGeometry::TVec3f pos = mPos;
        pos.y -= 150.0f;
        lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&pos);
    }
}

// The state after an arrival while carrying (fn_80292820, fn_802935A4): 0x33
// without life, 0x3E with 0xCF0, 0x12 while holding an actor, else 0.
inline bool Player_804A6F38::requestArrival() {
    if (fn_8024189C() <= 0) {
        return fn_80251AEC(0x33);
    }
    if (field_0xcf0 == 1) {
        return fn_80251AEC(0x3E);
    }
    if (fn_8025F684()) {
        return fn_80251AEC(0x12);
    }
    return fn_80251AEC(0);
}

// Effects 0x15B and 0x15C at the player (fn_80291E44, fn_802925E4).
inline void Player_804A6F38::spawnEffect15B() {
    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15B, 0, 1, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x15C, -1, 0, NULL, NULL, true, 1.0f);
}

// The life that decides between landing and state 0x33: the leader's
// (fn_80237774 of fn_80240644) when fn_80240808 holds, else the player's own.
// fn_80293900 inlines it, and the unwritten fn_80290FC0 twice.
inline s32 Player_804A6F38::life() {
    if (fn_80240808()) {
        return fn_80237774(fn_80240644());
    }
    return fn_8024189C();
}

// Gravity for the drops onto the floor below (fn_802953A0, fn_8029513C):
// the fall speed 0x34C grows by 0.25 up to 4, then the player moves by 0x344.
inline void Player_804A6F38::fallStep() {
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -4.0f) {
        field_0x344.z = -4.0f;
    }
    mPos += field_0x344;
}

// Effect 0x17D 32 above and 16 over the player (fn_80291910, fn_802935A4,
// fn_80293774).
inline void Player_804A6F38::spawnEffect17D() {
    JGeometry::TVec3f pos;
    pos.set(mPos);
    pos.y += 32.0f;
    pos.z += 16.0f;
    spawnEmitter_8013CC50(field_0x3b4, &pos, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
}

// The animation while walking in or out through an entrance (fn_80293FE4,
// fn_802948C0): the walk cycle with 0xCEB, a 19-count cycle of frames 0-2
// with 0xCEC, the carry step (fn_8026152C) while holding an actor, else
// fn_8027EE84.
inline void Player_804A6F38::stepTransitionAnim() {
    if (field_0xceb == 1) {
        stepWalkCycle();
    } else if (field_0xcec == 1) {
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
    } else if (fn_8025F684()) {
        fn_8026152C(false);
    } else {
        fn_8027EE84();
    }
}

// The held actor's position: mPos raised 16 (raiseHeldActor and the updates
// fn_80292820, fn_802929FC, fn_80292ECC, fn_80293398, fn_80293B30; enter fn_80293E8C).
inline void Player_804A6F38::setHeldPos() {
    field_0x2f4.set(mPos);
    field_0x2f4.z += 16.0f;
}

// Shared by the enters of states 0x25 and 0x23 (fn_80292944, fn_802932BC),
// which inline the same block: 0x2F4 = mPos raised 16, and while the player
// holds an actor (fn_8025F684, serial field_0xd68) that actor is warped there
// on the player's layer. The GameView fetch's result is unused in both.
inline void Player_804A6F38::raiseHeldActor() {
    setHeldPos();
    if (fn_8025F684()) {
        s32 serial = field_0xd68;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            getGameView_802D6A9C();
            if (actor != NULL) {
                actor->fn_801F349C(field_0x3b4, &field_0x2f4);
            }
        }
    }
}

bool Player_804A6F38::fn_8028FBA0() {
    JGeometry::TVec3f pos;
    pos.x = field_0xcb0.x;
    pos.y = field_0xcb0.y;
    pos.z = field_0xcb0.z;
    if (field_0xcf2 == 0) {
        pos.y = field_0xcb0.y - 16.0f;
    }
    if (fn_80290070(0, &mPos, &pos, 5)) {
        field_0xbdc = 0;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8028FC2C() {
    if (field_0xd28 < 8) {
        return fn_80290070(0, &mPos, &field_0xcb0, 0x27);
    }
    s32 stage = getGameView_802D6A9C()->field_0x4;
    s32 level = getGameView_802D6A9C()->field_0x8;
    s32 col = getGameView_802D6A9C()->field_0x14;
    GameView_804A9950* view = getGameView_802D6A9C();
    s32 screen = getGameView_802D6A9C()->field_0x60[view->field_0x18][col];
    s32 floor = field_0x3b4 - 7;
    if (stage == 0 && level == 0) {
        JGeometry::TVec3f pos(1960.0f, 1000.0f, 0.0f);
        return fn_80290070(0, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 0 && screen == 3) {
        JGeometry::TVec3f pos(1750.0f, 1080.0f, 0.0f);
        return fn_80290070(0, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 0 && screen == 5 && floor == 5) {
        JGeometry::TVec3f pos(56.0f, 326.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 0 && screen == 5 && floor == 6) {
        JGeometry::TVec3f pos(120.0f, 88.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 0 && screen == 5 && floor == 7) {
        JGeometry::TVec3f pos(40.0f, 284.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 0 && screen == 6) {
        JGeometry::TVec3f pos(184.0f, 214.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 1 && level == 3) {
        JGeometry::TVec3f pos(218.0f, 72.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 2 && level == 0) {
        JGeometry::TVec3f pos(234.0f, 72.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 3 && level == 0) {
        JGeometry::TVec3f pos(202.0f, 88.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 5 && level == 0) {
        return fn_80290070(0, &mPos, &field_0xcb0, 0x27);
    }
    if (stage == 6 && level == 0 && screen == 3) {
        JGeometry::TVec3f pos(256.0f, 20.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 6 && level == 0 && screen == 4) {
        JGeometry::TVec3f pos(256.0f, 20.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    if (stage == 6 && level == 0 && screen == 5) {
        JGeometry::TVec3f pos(472.0f, 44.0f, 0.0f);
        return fn_80290070(field_0xd28, &mPos, &pos, 0x27);
    }
    return false;
}

void Player_804A6F38::fn_80290D58() {
    if (!fn_80241E28(field_0xce0, &field_0xcc8)) {
        field_0xd1c = field_0xcbc = mPos;
    }
    if (field_0xcf4 == 0x14 && field_0x3b4 >= 8) {
        field_0xbd8 = 600;
        field_0xbdc = 1;
    }
    if (fn_80240D64()) {
        field_0x11f0 = 5;
    }
    field_0xce8 = false;
    field_0xcf8 = field_0xcf4;
    field_0xcf4 = 0x29;
    field_0x468 = 60;
    field_0xced = 0;
    fn_80241454(1);
    field_0x1216 = false;
    fn_8024BAEC();
    field_0xbec->fn_8030BD3C(field_0x4);
    if (fn_802486A0() != 4 && fn_802486A0() != 5) {
        fn_80248004(5, true);
    }
    fn_80248604();
    field_0xceb = 0;
    field_0x400 = 0;
    field_0xcec = 0;
}

void Player_804A6F38::fn_80290E90() {
    if (!fn_80240670()) {
        fn_80301D54(field_0x4);
    }
    field_0x400 = 0;
    if (lbl_8053AAF8->field_0x7e == 0 && !fn_80240670()) {
        if (field_0xce4 < 8) {
            gAudioMgr_8053AF80->fn_8045815C(false);
        } else if (field_0xce4 == 8) {
            gAudioMgr_8053AF80->fn_8045815C(true);
        }
    }
}

void Player_804A6F38::fn_80290F20() {
    if (field_0x132c != NULL) {
        field_0x132c->becomeInvalidEmitter();
        field_0x132c = NULL;
    }
    if (field_0x135c != NULL) {
        field_0x135c->becomeInvalidEmitter();
        field_0x135c = NULL;
    }
    if (fn_802486A0() != 4 && fn_80240808()) {
        fn_80238244(field_0x4);
    }
}

// FIXME: 99.62 %. Scheduling only: the original's lwz r4, 0x135c is scheduled at
// +0x7B0 (we emit it at +0x7B8), and the li r5, 0 / li r3, 0 pair at +0x840 is in the opposite order.
void Player_804A6F38::fn_80290FC0() {
    if (field_0x3fc == 40) {
        if (field_0xcf4 == 0xC && fn_802DE9A0()) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x2AA, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x2A9, -1, 0, NULL, NULL, true, 1.0f);
        } else if (field_0xcf4 == 0xD) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x45F, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x45E, -1, 0, NULL, NULL, true, 1.0f);
            switch (field_0x4) {
            case 0:
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16B, 0, 1, NULL, NULL, true, 1.0f);
                break;
            case 1:
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16D, 0, 1, NULL, NULL, true, 1.0f);
                break;
            case 2:
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16A, 0, 1, NULL, NULL, true, 1.0f);
                break;
            case 3:
                spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16C, 0, 1, NULL, NULL, true, 1.0f);
                break;
            }
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16E, 0, 1, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x16F, 0, 1, NULL, NULL, true, 1.0f);
        }
    }
    if (field_0xcf4 == 0xD) {
        if (field_0x3fc < 130) {
            field_0x3fc++;
            return;
        }
        if (!fn_80240670()) {
            fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
        }
        s32 hp = life();
        s32 state = 0;
        if (hp <= 0) {
            state = 0x33;
        }
        field_0x124d = 0;
        if (fn_80251AEC(state)) {
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            mPos.z = 0.0f;
        }
        return;
    }
    if (field_0x3fc >= 58) {
        if (field_0x3fb == 0) {
            fn_8024F0E8(0x1000A);
            if (field_0xcf4 == 0xC && fn_802DE9A0()) {
                fn_8024F0E8(0x20031);
            }
            field_0x3fb = 1;
            if (field_0xcf4 == 0xC && fn_802DE9A0() && field_0x135c == NULL) {
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
            if (field_0xcf4 == 0xC && fn_802DE9A0() && field_0x132c == NULL) {
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
            JGeometry::TVec3f velocity = (field_0xcd4 - mPos) * (1.0f / 64.0f);
            field_0x344.set(velocity.x, velocity.y, 8.0f);
        }
        if (field_0x3fc < 62) {
            field_0x3fd = 0;
        } else if (field_0x3fc < 66) {
            field_0x3fd = 1;
        } else if (field_0x3fc < 70) {
            field_0x3fd = 2;
        } else if (field_0x3fc < 74) {
            field_0x3fd = 3;
        }
        field_0x3fc++;
        mPos += field_0x344;
        field_0x344.z -= 0.25f;
        if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
            mPos.z = 0.0f;
            if (!fn_80240670() && (field_0xcf4 == 0xC || field_0xcf4 == 0xD || field_0xcf4 == 0x26)) {
                fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
            } else {
                fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x11);
            }
            s32 hp = life();
        s32 state = 0;
        if (hp <= 0) {
            state = 0x33;
        }
            field_0x124d = 0;
            if (fn_80251AEC(state)) {
                field_0x3b4 = field_0xce4;
                mPos = field_0xcd4;
                mPos.z = 0.0f;
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
            JPAGetXYZRotateMtx(0, lbl_8046C228[field_0x3dc], 0, field_0x135c->mGlobalRot);
        }
        if (field_0x132c != NULL) {
            field_0x1344.set(mPos);
            if (field_0x3dc == 2) {
                field_0x1344.y -= 3.0f;
            } else if (field_0x3dc == 3) {
                field_0x1344.y += 3.0f;
            }
            field_0x1330.field_0x4.set(field_0x1344);
            JPAGetXYZRotateMtx(0, lbl_8046C228[field_0x3dc], 0, field_0x132c->mGlobalRot);
        }
    } else {
        fn_8027EDDC();
    }
}

void Player_804A6F38::fn_8029185C() {
    field_0x124d = 1;
    field_0x458 = 0;
    fn_8024F0E8(0x10003);
    if (field_0xcf4 == 0xD) {
        fn_8024F0E8(0x20067);
        field_0x3fc = 40;
    } else if (field_0xcf4 == 0x26) {
        field_0x3fc = 58;
    }
    if (fn_802486A0() != 4 && fn_802486A0() != 5) {
        fn_80248004(5, true);
    }
    fn_8024BAEC();
}

void Player_804A6F38::fn_80291910() {
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc < 60) {
        return;
    }
    if (field_0x3fc == 60) {
        if (field_0xce0 == 8) {
            spawnEffect17D();
            fn_8024F0E8(0x20020);
        }
        fn_8024F0E8(0x2002F);
        fn_8024F0E8(0x20021);
        fn_80241454(1);
    }
    mPos += field_0x344;
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -4.0f) {
        field_0x344.z = -4.0f;
    }
    if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        bool requested;
        if (fn_8024189C() <= 0) {
            requested = fn_80251AEC(0x33);
        } else if (field_0xcf0 == 1) {
            requested = fn_80251AEC(0x3E);
        } else {
            requested = fn_80251AEC(0);
        }
        if (requested == true) {
            field_0x3dc = 2;
            field_0x3e8 = 2;
        }
    } else {
        field_0x3fc++;
        spinFacing();
    }
}

void Player_804A6F38::fn_80291B64() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 4.0f;
}

void Player_804A6F38::fn_80291B8C() {
    field_0x3fc++;
    if (field_0x3fc >= 60) {
        riseToTarget(0x2B);
    }
    spinFacing();
}

void Player_804A6F38::fn_80291CB8() {
    field_0x124d = 1;
    mPos.z = 0.0f;
    field_0x458 = 0;
    fn_8024F0E8(0x2002E);
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(false);
    }
}

void Player_804A6F38::fn_80291D28() {
    if (fn_80301CD0(field_0x4)) {
        return;
    }
    if (field_0xce4 < 8) {
        if (field_0x430 > 0) {
            field_0x430--;
        }
        field_0x3fb = 3;
        if (field_0x430 <= 90) {
            if (field_0x430 == 90) {
                fn_80241454(1);
            } else if (field_0x430 > 84) {
                field_0x3f9 = (90 - field_0x430) / 2;
            } else {
                field_0x3f9 = ((84 - field_0x430) / 6 & 3) + 3;
            }
        }
        if (field_0x430 == 0) {
            field_0x124d = 0;
            field_0x44c = 5;
            fn_80251AEC(0x4A);
        }
    } else if (field_0x3f8 < 60) {
        field_0x3f8++;
    } else {
        field_0x124d = 0;
        fn_80251AEC(0);
    }
}

void Player_804A6F38::fn_80291E44() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3f9 = 0;
    field_0x3dc = 2;
    field_0x3e8 = 2;
    if (field_0xce4 < 8) {
        spawnEffect15B();
        fn_8024F0E8(0x20041);
        fn_8024F0E8(0x1000F);
        field_0x430 = 0x5C;
    }
}

void Player_804A6F38::fn_80291F10() {
    field_0x3f8++;
    if (field_0x3f8 == 4 || field_0x3f8 == 8) {
        s32 index = field_0x4;
        getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x3b4, &mPos, (index & 3) | 0xC, -1, index);
    }
    if (field_0x3f8 <= 4) {
        field_0x3f9 = 0;
    } else if (field_0x3f8 < 19) {
        field_0x3f9 = 1;
    } else if (field_0x3f8 < 23) {
        field_0x3f9 = 2;
    } else if (field_0x3f8 < 27) {
        field_0x3f9 = 3;
    } else if (field_0x3f8 < 31) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 35) {
        field_0x3f9 = 5;
    } else if (field_0x3f8 < 39) {
        field_0x3f9 = 2;
    } else if (field_0x3f8 < 43) {
        field_0x3f9 = 3;
    } else if (field_0x3f8 < 47) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 51) {
        field_0x3f9 = 5;
    } else if (field_0x3f8 < 55) {
        field_0x3f9 = 2;
    } else if (field_0x3f8 < 59) {
        field_0x3f9 = 3;
    } else if (field_0x3f8 < 63) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 67) {
        field_0x3f9 = 5;
    }
    if (field_0x3f8 < 67) {
        mPos.y -= 0.1;
    } else {
        field_0x3f8 = 67;
        if (!fn_80301CD0(field_0x4)) {
            field_0x124d = 0;
            if (fn_80251AEC(0x31)) {
                field_0x3b4 = field_0xce4;
                mPos = field_0xcd4;
                field_0xbec->fn_8030BD3C(field_0x4);
            }
        }
    }
}

void Player_804A6F38::fn_80292124() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 3;
    field_0x3e8 = 3;
    if (!fn_80240670() && field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(0);
    }
}

void Player_804A6F38::fn_80292198() {
    stepWalkCycle();
    if (field_0x3fc == 2) {
        fn_80241454(1);
    }
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc < 60) {
        return;
    }
    if (field_0x3fb == 0) {
        if (field_0x400 != 0) {
            field_0x400--;
        } else {
            field_0x124d = 0;
            if (fn_80251AEC(0x4A) && field_0xce4 >= 8) {
                fn_803037D4(field_0x4, 0xD);
            }
        }
    } else {
        field_0x3fb = 0;
        field_0x400 = 8;
    }
}

void Player_804A6F38::fn_802922D0() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fb = 1;
    field_0x3f9 = 0;
}

void Player_804A6F38::fn_802922EC() {
    field_0x3f8++;
    if (field_0x3f8 < 7) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 13) {
        field_0x3f9 = 5;
    } else if (field_0x3f8 < 19) {
        field_0x3f9 = 6;
    } else if (field_0x3f8 < 34) {
        field_0x3f9 = 7;
    } else if (field_0x3f8 < 40) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 46) {
        field_0x3f9 = 5;
    } else if (field_0x3f8 < 52) {
        field_0x3f9 = 6;
    } else if (field_0x3f8 < 67) {
        field_0x3f9 = 7;
    }
    if (field_0x3f8 < 67) {
        mPos.y -= 0.5;
    } else {
        field_0x124d = 0;
        if (fn_80251AEC(0x29)) {
            fn_80241454(0);
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    }
}

void Player_804A6F38::fn_80292454() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 3;
    field_0x3e8 = 3;
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(0);
    }
}

void Player_804A6F38::fn_802924AC() {
    stepWalkCycle();
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc < 60) {
        return;
    }
    if (field_0x3fc >= 92) {
        field_0x124d = 0;
        if (fn_80251AEC(0x4B) && field_0xce4 >= 8) {
            fn_803037D4(field_0x4, 0xD);
        }
    } else if (field_0x3fc < 92) { // the original tests it again (a dead bge)
        mPos.y += 1.0;
        field_0x3fc++;
    }
}

void Player_804A6F38::fn_802925C4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}

void Player_804A6F38::fn_802925E4() {
    if (field_0x3f8 < 8) {
        field_0x3f9 = 0;
    } else if (field_0x3f8 < 14) {
        field_0x3f9 = 1;
    } else if (field_0x3f8 < 20) {
        field_0x3f9 = 2;
    } else if (field_0x3f8 < 26) {
        field_0x3f9 = 3;
    } else if (field_0x3f8 < 30) {
        field_0x3f9 = 4;
    } else if (field_0x3f8 < 34) {
        field_0x3f9 = 5;
    } else if (field_0x3f8 < 38) {
        field_0x3f9 = 6;
    } else if (field_0x3f8 < 42) {
        field_0x3f9 = 7;
    } else if (field_0x3f8 < 46) {
        field_0x3f9 = 8;
    } else if (field_0x3f8 < 48) {
        field_0x3f9 = 9;
    } else if (field_0x3f8 < 50) {
        field_0x3f9 = 10;
    } else {
        field_0x3f9 = 11;
        field_0x124d = 0;
        if (fn_80251AEC(0x27)) {
            spawnEffect15B();
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    }
    field_0x3f8++;
}

void Player_804A6F38::fn_802927B0() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fb = 0;
    fn_8024F0E8(0x20060);
    spawnEmitter_8013CC50(field_0x3b4, &mPos, 0x547, -1, 0, NULL, NULL, true, 1.0f);
}

void Player_804A6F38::fn_80292820() {
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 == field_0xce0 || fn_80240670() || !fn_80303750(field_0x4)) {
        if (field_0x3fc >= 60) {
            field_0x124d = 0;
            if (requestArrival()) {
                fn_803037D4(field_0x4, 0xD);
            }
        }
    }
    setHeldPos();
}

void Player_804A6F38::fn_80292944() {
    fn_80241454(1);
    field_0x124d = 1;
    field_0x458 = 0;
    raiseHeldActor();
}

void Player_804A6F38::fn_802929FC() {
    followCamera();
    if (field_0x3fc == 60) {
        if (field_0xcf4 != 0x1E && field_0xcf4 != 0x1F) {
            spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x546, 1, 1, NULL, NULL, true, 1.0f);
            switch (field_0x4) {
            case 0:
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14E, 1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14F, 1, 1, NULL, NULL, true, 1.0f);
                if (fn_80241544() >= 2) {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x151, 1, 1, NULL, NULL, true, 1.0f);
                } else {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x150, 1, 1, NULL, NULL, true, 1.0f);
                }
                break;
            case 1:
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x157, 1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x158, 1, 1, NULL, NULL, true, 1.0f);
                if (fn_80241544() >= 2) {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x15A, 1, 1, NULL, NULL, true, 1.0f);
                } else {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x159, 1, 1, NULL, NULL, true, 1.0f);
                }
                break;
            case 2:
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14A, 1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14B, 1, 1, NULL, NULL, true, 1.0f);
                if (fn_80241544() >= 2) {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14D, 1, 1, NULL, NULL, true, 1.0f);
                } else {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x14C, 1, 1, NULL, NULL, true, 1.0f);
                }
                break;
            case 3:
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x153, 1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x154, 1, 1, NULL, NULL, true, 1.0f);
                if (fn_80241544() >= 2) {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x156, 1, 1, NULL, NULL, true, 1.0f);
                } else {
                    spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x155, 1, 1, NULL, NULL, true, 1.0f);
                }
                break;
            }
            spawnEmitter_8013CC50(field_0xce0, &field_0xcc8, 0x152, 1, 1, NULL, NULL, true, 1.0f);
        }
        field_0x3fd = 0;
        fn_80241454(0);
    } else if (field_0x3fc >= 118) {
        field_0x124d = 0;
        if (fn_80251AEC(0x25)) {
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    }
    field_0x3fc++;
    setHeldPos();
}

void Player_804A6F38::fn_80292E90() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    fn_8024F0E8(0x2000C);
}

void Player_804A6F38::fn_80292ECC() {
    followCamera();
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 == field_0xce0 || fn_80240670() || !fn_80303750(field_0x4)) {
        if (field_0x3fc >= 60) {
            if (field_0x3fc == 60) {
                if (field_0xce0 >= 8) {
                    spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x545, 1, 1, NULL, NULL, true, 1.0f);
                    switch (field_0x4) {
                    case 0:
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x140, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x141, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x142, 1, 1, NULL, NULL, true, 1.0f);
                        break;
                    case 1:
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x147, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x148, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x149, 1, 1, NULL, NULL, true, 1.0f);
                        break;
                    case 2:
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x13D, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x13E, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x13F, 1, 1, NULL, NULL, true, 1.0f);
                        break;
                    case 3:
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x144, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x145, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x146, 1, 1, NULL, NULL, true, 1.0f);
                        break;
                    }
                    spawnEmitter_8013CC50(field_0xce4, &field_0xcd4, 0x143, 1, 1, NULL, NULL, true, 1.0f);
                    fn_8024F0E8(0x20020);
                }
                fn_8024F0E8(0x2002F);
                fn_8024F0E8(0x20021);
            }
            field_0x3fc++;
            if (field_0x3fc >= 171) {
                mPos.z = 0.0f;
                field_0x124d = 0;
                if (fn_8025F684()) {
                    fn_80251AEC(0x12);
                } else {
                    fn_80251AEC(0);
                }
                field_0x3dc = 2;
                field_0x3e8 = 2;
                fn_80241454(1);
            }
        }
    }
    setHeldPos();
}

void Player_804A6F38::fn_802932BC() {
    field_0x124d = 1;
    mPos.z = 0.0f;
    field_0x458 = 0;
    field_0x344.set(0.0f, -0.5f, 4.0f);
    raiseHeldActor();
    if (field_0xcf4 == 0x1F) {
        field_0x3fc = 50;
    }
}

void Player_804A6F38::fn_80293398() {
    followCamera();
    field_0x3fc++;
    if (field_0x3fc >= 60 || field_0xcf4 == 0x1F) {
        riseToTarget(0x23);
    }
    spinFacing();
    setHeldPos();
}

void Player_804A6F38::fn_80293534() {
    field_0x124d = 1;
    mPos.z = 0.0f;
    field_0x458 = 0;
    fn_8024F0E8(0x2002E);
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(false);
    }
}

void Player_804A6F38::fn_802935A4() {
    followCamera();
    if (field_0x3fc < 60) {
        field_0x3fc++;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc < 60) {
        return;
    }
    field_0x124d = 0;
    if (requestArrival()) {
        if (field_0x3b4 < 8) {
            spawnEffect17D();
            fn_8024F0E8(0x20020);
            fn_8024F0E8(0x20021);
        } else {
            fn_803037D4(field_0x4, 0xD);
        }
    }
}

void Player_804A6F38::fn_80293760() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80293774() {
    if (field_0x3fc == 60) {
        if (field_0xcf4 != 0x1D) {
            if (field_0x3b4 < 8) {
                spawnEffect17D();
            } else if (field_0x3b4 >= 8 && field_0xce4 < 8) {
                fn_8024EFA0(false);
            }
        }
        field_0x3fd = 0;
    } else if (field_0x3fc >= 118) {
        if (field_0x3b4 < 8) {
            field_0xcb0 = mPos;
        }
        field_0x124d = 0;
        if (fn_80251AEC(0x21)) {
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    }
    field_0x3fc++;
}

void Player_804A6F38::fn_802938B8() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    if (field_0xcf4 != 0x1D) {
        fn_8024F0E8(0x2000C);
    }
}

void Player_804A6F38::fn_80293900() {
    if (field_0x3fc < 60) {
        if (fn_80240670()) {
            field_0x3fc = 60;
        } else {
            field_0x3fc++;
        }
    }
    if (!fn_80240670()) {
        if (field_0x3b4 != 0 && field_0xce4 != field_0xce0 && fn_80303750(field_0x4)) {
            return;
        }
        if (field_0x3fc < 60) {
            return;
        }
    }
    if (field_0x3fc == 60) {
        if (field_0x3b4 < 8) {
            fn_8024F0E8(0x20021);
        } else {
            fn_803037D4(field_0x4, 0xD);
        }
    }
    field_0x3fc++;
    fn_80241454(1);
    mPos.z -= 4.0f;
    if (mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        if (life() <= 0) {
            fn_80251AEC(0x33);
        } else if (fn_8025F684()) {
            if (fn_80251AEC(0x12) && field_0xcf4 == 9) {
                field_0x464 = 180;
            }
        } else if (fn_80251AEC(0) && field_0xcf4 == 9) {
            field_0x464 = 180;
        }
    }
    spinFacing();
}

void Player_804A6F38::fn_80293B1C() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_80293B30() {
    if (field_0x3fc >= 58) {
        field_0x3fc = 58;
        field_0x3fd = 0;
        field_0x3fb = 1;
        fn_80241454(0);
        if (!fn_80301CD0(field_0x4)) {
            if (fn_80240670()) {
                if (fn_8022D75C(fn_8024064C())) {
                    return;
                }
                if (field_0xcf4 == 0x12) {
                    field_0xcd4 = *getPlayerPos_8023E724(fn_8024064C());
                    field_0xce4 = getPlayerLayer_8023DE58(fn_8024064C());
                }
            }
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x124d = 0;
            if (fn_80251AEC(0x1F)) {
                if (fn_80241E28(field_0xce4, &field_0xcd4)) {
                    if (fn_802486A0() == 5 && fn_80240808()) {
                        field_0xcd4.set(*getPlayerPos_8023E724(fn_80240644()));
                    }
                    u16 tile = fn_8022461C(&field_0xcd4, field_0xce4);
                    if (fn_802265DC(&field_0xcd4, field_0xce4, true) || tile == 3 || tile == 0xE ||
                        fn_802684A4(&field_0xcd4, field_0xce4))
                    {
                        if (field_0xce4 < 8) {
                            field_0xcd4.set(field_0x2c4);
                        } else {
                            field_0xcd4.set(field_0xcbc);
                        }
                    }
                }
                field_0x3b4 = field_0xce4;
                mPos = field_0xcd4;
                // fn_80272160 reads only x and y of the position.
                if (fn_802DEABC() && field_0x3b4 <= 8 && (field_0xcf4 == 9 || field_0xcf4 == 0x12)) {
                    if (!fn_80240670()) {
                        fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
                    }
                } else if (field_0x3b4 > 8 && (field_0xcf4 == 9 || field_0xcf4 == 0x12) && !fn_80240670()) {
                    fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
                }
                if (field_0xcf4 == 9) {
                    mPos.z = 0.0f;
                } else if (field_0xcf4 == 0x24) {
                    mPos.z = 128.0f;
                } else if (viewLayerFlag(field_0x3b4)) {
                    mPos.z = 0.0f;
                } else if (field_0xce4 >= 8) {
                    mPos.z = 128.0f;
                } else {
                    mPos.z = 0.0f;
                }
                field_0xbec->fn_8030BD3C(field_0x4);
            }
        }
    }
    fn_8027EDDC();
    setHeldPos();
}

void Player_804A6F38::fn_80293E8C() {
    field_0x124d = 1;
    mPos.z = 0.0f;
    field_0x458 = 0;
    fn_8024F0E8(0x10003);
    fn_8024F0E8(0x2002D);
    s32 layer = field_0x3b4;
    if (layer > 8 && field_0xce4 > 8 && viewLayerFlag(layer) && viewLayerFlag(field_0xce4) &&
        field_0xce4 != field_0x3b4)
    {
        field_0x3fc = 58;
    }
    setHeldPos();
    if (field_0xcf4 == 0x24 && field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(false);
    }
    if (fn_802486A0() != 4) {
        if (fn_802486A0() != 5) {
            fn_80248004(5, true);
        }
        fn_8024BAEC();
    }
}

void Player_804A6F38::fn_80293FE4() {
    if (field_0x3fc < 30) {
        field_0x3fc++;
        return;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc == 30) {
        if (field_0x3a4.y > field_0x398.y) {
            if (field_0x3b4 < 8) {
                if (field_0xceb == 0) {
                    fn_8024F0E8(0x2001B);
                }
                fn_8024F0E8(0x20021);
            } else {
                fn_803037D4(field_0x4, 0xD);
            }
        } else {
            if (field_0x3b4 < 8) {
                if (field_0xceb == 0) {
                    fn_8024F0E8(0x20019);
                }
                fn_8024F0E8(0x20021);
            } else {
                fn_803037D4(field_0x4, 0xD);
            }
        }
    }
    if (field_0xcf4 == 3) {
        fn_80265D8C();
    }
    field_0x3fc++;
    fn_80241454(1);
    fn_8027E5BC(0.5f);
    if ((field_0x3a4 - mPos).isZero() || field_0x3fc >= 80) {
        field_0x124d = 0;
        if (fn_8024189C() <= 0) {
            fn_80251AEC(0x33);
        } else if (field_0xceb == 1) {
            if (viewLayerFlag(field_0x3b4)) {
                fn_80251AEC(0x4B);
            } else {
                fn_80251AEC(0x4A);
            }
        } else if (field_0xcec == 1) {
            fn_80251AEC(0xB);
        } else if (fn_8025F684()) {
            fn_80251AEC(0x12);
        } else {
            fn_80251AEC(0);
        }
    }
    if (!((field_0xcf4 == 0x15 || field_0xcf4 == 0x23) && viewLayerFlag(field_0x3b4))) {
        fn_80268B3C();
    }
    stepTransitionAnim();
}

void Player_804A6F38::fn_80294374() {
    field_0x398.set(mPos);
    field_0x398.z = 0.0f;
    field_0x3a4.set(mPos);
    field_0x3a4.z = 0.0f;
    field_0x124d = 1;
    if (field_0xcf4 == 0x15) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x3b4 < 8) {
                field_0x398 += JGeometry::TVec3f(0.0f, -1.25f, 0.0f);
                mPos.y -= 1.25f;
                field_0x3dc = 2;
                field_0x3e8 = 2;
            } else {
                field_0x398 += JGeometry::TVec3f(0.0f, 1.25f, 0.0f);
                mPos.y += 1.25f;
                field_0x3dc = 3;
                field_0x3e8 = 3;
            }
        } else if (field_0x3b4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
        } else if (field_0xce0 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
        } else {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
        }
    } else if (field_0xcf4 == 0x23) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x3b4 < 8) {
                field_0x398 += JGeometry::TVec3f(0.0f, 1.25f, 0.0f);
                mPos.y += 1.25f;
                field_0x3dc = 3;
                field_0x3e8 = 3;
            } else {
                field_0x398 += JGeometry::TVec3f(0.0f, -1.25f, 0.0f);
                mPos.y -= 1.25f;
                field_0x3dc = 2;
                field_0x3e8 = 2;
            }
        } else if (field_0x3b4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
        } else if (field_0xce0 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
        } else {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
        }
    } else if (field_0xcf4 == 0 || field_0xcf4 == 3) {
        field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
        field_0x3dc = 3;
        field_0x3e8 = 3;
    } else if (field_0xcf4 == 0x17) {
        field_0x3a4 += JGeometry::TVec3f(20.0f, 0.0f, 0.0f);
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (field_0xcf4 == 0x18) {
        field_0x3a4 += JGeometry::TVec3f(-20.0f, 0.0f, 0.0f);
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else {
        field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
        field_0x3dc = 2;
        field_0x3e8 = 2;
    }
    field_0x458 = 0;
    if (field_0xcf4 == 1) {
        gBgRewrite_8053AC98->fn_8029E508(&mPos, field_0x4, field_0x3b4);
    }
    if (field_0xcf4 == 3) {
        field_0xca0 = 0;
    } else {
        field_0xca0 = 0xFF;
    }
}

void Player_804A6F38::fn_802948C0() {
    fn_8027E5BC(0.5f);
    field_0x3fc++;
    if ((field_0x3a4 - mPos).isZero() || field_0x3fc >= 50) {
        if (field_0x3b4 < 8) {
            field_0xcb0 = mPos;
        }
        field_0x124d = 0;
        if (fn_80251AEC(0x1D)) {
            fn_80241454(0);
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    } else if (field_0xcf4 == 2) {
        fn_80265D68();
    }
    if (field_0xcf4 == 2) {
        fn_80268B3C();
    }
    stepTransitionAnim();
}

void Player_804A6F38::fn_80294ADC() {
    if (field_0x1248 == 0x4A || field_0x1248 == 0x4B) {
        field_0xceb = 1;
        field_0xcec = 0;
    } else if (field_0x1248 == 0xB) {
        field_0xceb = 0;
        field_0xcec = 1;
    } else {
        field_0xceb = 0;
        field_0xcec = 0;
    }
    field_0x124d = 1;
    field_0x458 = 0;
    mPos.z = 0.0f;
    field_0x398.set(mPos);
    field_0x398.z = 0.0f;
    field_0x3a4.set(mPos);
    field_0x3a4.z = 0.0f;
    if (field_0xcf4 == 0x15) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x3b4 < 8) {
                field_0x3a4 += JGeometry::TVec3f(0.0f, -1.25f, 0.0f);
                field_0x3dc = 3;
                field_0x3e8 = 3;
                fn_8024F0E8(0x20018);
            } else {
                field_0x3a4 += JGeometry::TVec3f(0.0f, 1.25f, 0.0f);
                field_0x3dc = 2;
                field_0x3e8 = 2;
                fn_8024F0E8(0x2001A);
            }
        } else if (field_0x3b4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
            fn_8024F0E8(0x20018);
        } else if (field_0xce4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
            fn_8024F0E8(0x2001A);
        } else {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
            fn_8024F0E8(0x20018);
        }
    } else if (field_0xcf4 == 0x23) {
        if (viewLayerFlag(field_0x3b4)) {
            if (field_0x3b4 < 8) {
                field_0x3a4 += JGeometry::TVec3f(0.0f, 1.25f, 0.0f);
                field_0x3dc = 2;
                field_0x3e8 = 2;
                fn_8024F0E8(0x2001A);
            } else {
                field_0x3a4 += JGeometry::TVec3f(0.0f, -1.25f, 0.0f);
                field_0x3dc = 3;
                field_0x3e8 = 3;
                fn_8024F0E8(0x20018);
            }
        } else if (field_0x3b4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
            fn_8024F0E8(0x2001A);
        } else if (field_0xce4 < 8) {
            field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
            field_0x3dc = 3;
            field_0x3e8 = 3;
            fn_8024F0E8(0x20018);
        } else {
            field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
            field_0x3dc = 2;
            field_0x3e8 = 2;
            fn_8024F0E8(0x2001A);
        }
    } else if (field_0xcf4 == 0x17) {
        field_0x3a4 += JGeometry::TVec3f(20.0f, 0.0f, 0.0f);
        field_0x3dc = 0;
        field_0x3e8 = 0;
        if (field_0xceb == 0) {
            fn_8024F0E8(0x2001A);
        }
    } else if (field_0xcf4 == 0x18) {
        field_0x3a4 += JGeometry::TVec3f(-20.0f, 0.0f, 0.0f);
        field_0x3dc = 1;
        field_0x3e8 = 1;
        if (field_0xceb == 0) {
            fn_8024F0E8(0x2001A);
        }
    } else if (field_0xcf4 == 1 || field_0xcf4 == 2) {
        field_0x3a4 += JGeometry::TVec3f(0.0f, 20.0f, 0.0f);
        field_0x3dc = 2;
        field_0x3e8 = 2;
        if (field_0xceb == 0) {
            fn_8024F0E8(0x2001A);
        }
    } else {
        field_0x3a4 += JGeometry::TVec3f(0.0f, -20.0f, 0.0f);
        field_0x3dc = 3;
        field_0x3e8 = 3;
        if (field_0xceb == 0) {
            fn_8024F0E8(0x20018);
        }
    }
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(false);
    }
    field_0xca0 = 0xFF;
}

void Player_804A6F38::fn_8029513C() {
    if (field_0x3fc < 60) {
        field_0x3fc++;
        return;
    }
    fn_80241454(1);
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    fallStep();
    if (mPos.z < 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        if (fn_8024189C() <= 0) {
            fn_80251AEC(0x33);
        } else {
            fn_80251AEC(0);
        }
    }
}

void Player_804A6F38::fn_80295240() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = -0.5f;
    field_0x344.z = 0.0f;
}

void Player_804A6F38::fn_80295268() {
    field_0x3fc++;
    if (field_0x3fc >= 10) {
        mPos.z += 4.0f;
        if (mPos.z > 128.0f) {
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x124d = 0;
            if (fn_80251AEC(0x2F)) {
                fn_80241454(0);
                field_0x3b4 = field_0xce4;
                mPos = field_0xcd4;
                field_0xbec->fn_8030BD3C(field_0x4);
            }
        }
    }
}

void Player_804A6F38::fn_80295334() {
    field_0x124d = 1;
    field_0x458 = 0;
    if (field_0x3b4 >= 8 && field_0xce4 < 8) {
        fn_8024EFA0(false);
    }
    field_0x3dc = 2;
    field_0x3e8 = 2;
    mPos.z = 0.0f;
}

void Player_804A6F38::fn_802953A0() {
    if (field_0x3fc < 60) {
        field_0x3fc++;
        return;
    }
    if (field_0xce4 != field_0xce0 && !fn_80240670() && fn_80303750(field_0x4)) {
        return;
    }
    if (field_0x3fc < 60) {
        return;
    }
    fallStep();
    if (mPos.z < 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        bool requested;
        if (fn_8024189C() <= 0) {
            requested = fn_80251AEC(0x33);
        } else {
            requested = fn_80251AEC(0);
        }
        if (requested) {
            fn_803037D4(field_0x4, 0xD);
        }
    }
}

void Player_804A6F38::fn_802954B8() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
}

void Player_804A6F38::fn_802954DC() {
    if (field_0x3fc >= 60) {
        field_0x124d = 0;
        if (fn_80251AEC(0x2D)) {
            if (field_0x3b4 < 8) {
                field_0xcb0 = mPos;
            }
            field_0x3b4 = field_0xce4;
            mPos = field_0xcd4;
            mPos.z = 128.0f;
            field_0xbec->fn_8030BD3C(field_0x4);
        }
    } else {
        field_0x3fc++;
    }
}

void Player_804A6F38::fn_80295584() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x3fd = 1;
    field_0x3dc = 2;
    field_0x3e8 = 2;
}
