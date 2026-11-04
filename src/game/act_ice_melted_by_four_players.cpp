#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/act/act_ice_melted_by_four_players.h"

// Timer values of the melting steps, indexed by field_0x24c (lbl_804751A8).
static const s32 lbl_804751A8[15] = {
    30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1,
};

// FIXME: 99.30 %. regalloc only: the original keeps the 0.0f in f0 and the
// scale (field_0x238) in f1 for the matrix stores; ours swaps them (the
// literals are @NNN vs lbl_, a data split).
void IceMeltedByFourPlayers_804AE888::draw() {
    if (field_0x251 == 1) {
        return;
    }
    // The frame lays later locals lower: params (0x38) above mtx (0x8).
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    if (!field_0x54.isZero()) {
        mtx[0][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[2][3] = 0.0f;
        mtx[0][0] = field_0x238;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = field_0x238;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = field_0x238;
        params.mMtx = mtx;
    }
    void* res = fn_804032E8('TIMG', "yukikabe.bti");
    if (field_0x4 < 8) {
        lbl_8053AB10->field_0x20->fn_802F745C(&mPos, res, &params);
    }
}

// FIXME: 97.99 %. Same instructions except the register of the stage loop
// counter (r30 here, r29 in the original, which holds the 0x10000 constant
// of the fn_80230FD0 call in r30) and the epilogue: the original returns 1
// through several copies of li r3, 1.
u8 IceMeltedByFourPlayers_804AE888::fn_8037845C() {
    if (field_0x244 > 0) {
        field_0x244--;
    }
    if (field_0x248 < 0) {
        return false;
    }
    if (field_0x248 < 4) {
        if (fn_8023077C()) {
            field_0x248 = 4;
            field_0x244 = 50;
            field_0x24c = 0;
            for (s32 i = 0; i < 4; i++) {
                fn_80230498(i, fn_8023E724(i), 0xE8, 0xFF, -0x80, -0x80, true);
            }
        }
    } else if (field_0x24c == 0 && field_0x244 > 30) {
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, fn_8023E724(i), 0xE8, 0xFF, -0x80, -0x80, true);
        }
    } else if (field_0x24c < 13) {
        if (field_0x244 == 0) {
            field_0x24c++;
            field_0x244 = lbl_804751A8[field_0x24c];
            for (s32 i = 0; i < 4; i++) {
                fn_80230498(i, fn_8023E724(i), field_0x24c + 0xDB, 0xFF, -0x80, -0x80, true);
                if (field_0x24c == 1) {
                    fn_80230FD0(i, 0x10001);
                }
            }
            if (field_0x24c == 13) {
                field_0x244 = 25;
            }
        }
    } else if (field_0x244 == 0 && fn_80230534(false)) {
        field_0x248 = -1;
        fn_801F0D20(3);
        vfunc_0x30(0xFF, true);
        fn_801F3668();
    }
    return true;
}

// FIXME: 99.80 %. regalloc only: the 0.0f compare is fcmpu with the operands
// in the other registers (f0 and f1 swapped); the literal is @NNN vs lbl_.
bool IceMeltedByFourPlayers_804AE888::fn_80378680() {
    JGeometry::TBox2f ours;
    ours.set(mBox);
    ours.addPos(mPos.x, mPos.y);
    s32 serial = -1;
    for (;;) {
        serial = fn_801F856C()->fn_801F78F4('HBMB', serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* other = fn_801F856C()->fn_801F666C(serial);
        if (other == NULL) {
            continue;
        }
        if (field_0x4 != other->field_0x4) {
            continue;
        }
        JGeometry::TBox2f theirs;
        theirs.set(other->mBox);
        theirs.addPos(other->mPos.x, other->mPos.y);
        if (!theirs.intersect(ours)) {
            continue;
        }
        JGeometry::TVec3f offset = other->field_0x3c;
        if (offset.x != 0.0f || offset.y != 0.0f) {
            other->mLife = 0;
            continue;
        }
        JGeometry::TVec3f pos = other->mPos;
        if (pos.y > mPos.y) {
            pos.y = mPos.y + 16.0f;
        } else {
            pos.y = mPos.y - 16.0f;
        }
        other->mPos = pos;
    }
    return false;
}

u8 IceMeltedByFourPlayers_804AE888::fn_80378880() {
    JGeometry::TBox2f ours;
    ours.set(mBox);
    ours.addPos(mPos.x, mPos.y);
    s32 serial = -1;
    u8 playerMask = 0;
    for (;;) {
        serial = fn_801F856C()->fn_801F78F4('FIRE', serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* other = fn_801F856C()->fn_801F666C(serial);
        if (other == NULL) {
            continue;
        }
        if (field_0x4 != other->field_0x4) {
            continue;
        }
        JGeometry::TBox2f theirs;
        theirs.set(other->mBox);
        theirs.addPos(other->mPos.x, other->mPos.y);
        if (!theirs.intersect(ours)) {
            continue;
        }
        if (fn_802BA9F8(other) != 1) {
            continue;
        }
        playerMask |= 1 << fn_802BA9F0(other);
    }
    return playerMask == 0xF;
}

// FIXME: 97.98 %. Same shape; the box copy's adds are scheduled differently
// (the original keeps f.x in f2 and reloads pos.y into it), case 1 stores the
// 0 of field_0x24c from a register the original holds for its loop, the
// request copy and the literals (@NNN vs lbl_) differ.
void IceMeltedByFourPlayers_804AE888::update() {
    if (fn_8037845C() == 1) {
        return;
    }
    JGeometry::TBox2f box;
    box.set(mBox);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    fn_8020D58C()->addBox(mSerial);
    fn_80378680();
    switch (field_0x234) {
    case 0:
        mBox.set(-96.0f, -16.0f, 96.0f, 16.0f);
        if (fn_80378880() == 1) {
            if (mEmitter230 == NULL) {
                mEmitter230 = fn_8013CC50(field_0x4, &mPos, 0x456, 0, 1, NULL, NULL, true, 1.0f);
            }
            field_0x238 -= 0.002f;
            if (field_0x238 < 0.5f) {
                field_0x234 = 1;
            }
            fn_801F0E34(0x50087, 0);
        } else if (mEmitter230 != NULL) {
            fn_801EE434(mEmitter230);
            mEmitter230 = NULL;
        }
        break;
    case 1:
        fn_801F0E34(0x50087, 0);
        field_0x23c -= 0.005;
        if (field_0x23c < 0.01f) {
            field_0x23c = 0.01f;
        }
        field_0x238 -= 0.002f;
        if (field_0x238 < 0.002f) {
            field_0x238 = 0.01f;
            if (fn_8023077C()) {
                field_0x244 = 50;
                field_0x248 = 4;
                field_0x24c = 0;
                for (s32 i = 0; i < 4; i++) {
                    fn_80230498(i, fn_8023E724(i), 0xE8, 0xFF, -0x80, -0x80, true);
                }
            }
            if (mEmitter230 != NULL) {
                fn_801EE434(mEmitter230);
                mEmitter230 = NULL;
            }
        }
        break;
    }
    field_0x54.set(field_0x23c, 1.0f, field_0x238);
    s32 percent = 100 - (s32)(100.0f * field_0x238);
    GameRootSub20_801902C0* root = fn_801902C0();
    if (root->mIceRequest.field_0x10 != NULL && root->mIceRequest.mActive == 0) {
        root->mIceRequest.mPos.set(mPos);
        root->mIceRequest.mPercent = percent;
        root->mIceRequest.mActive = 1;
    }
    field_0x251 = 1;
}

IceMeltedByFourPlayers_804AE888::~IceMeltedByFourPlayers_804AE888() {
    if (mEmitter230 != NULL) {
        fn_801EE434(mEmitter230);
        mEmitter230 = NULL;
    }
}

// FIXME: 99.58 %. The code matches; the four box literals (-92, -16, 92, 16)
// and 1.0f are numbered in another order (@NNN vs lbl_, a data split).
void IceMeltedByFourPlayers_804AE888::init() {
    mBox.set(-92.0f, -16.0f, 92.0f, 16.0f);
    mBox2.set(mBox);
    field_0x80.set(mBox);
    field_0xb8 = 1;
    field_0xf8 = 2;
    field_0xbc = 3;
    field_0x1a4 = 1;
    field_0xb0 = 0x0A040304;
    field_0x234 = 0;
    field_0x23c = 1.0f;
    field_0x238 = 1.0f;
    mEmitter230 = NULL;
    field_0x240 = 0;
    field_0x250 = 0;
    field_0x251 = 0;
    field_0x244 = 0;
    field_0x248 = -1;
    field_0x24c = 0;
}
