#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_gambling_chest.h"
#include "game/act/act_gambling_shop.h"

// Chest slot sets, picked by update's roll (bucket | field_0x264 << 2).
static const s32 lbl_804713C0[8][3] = {
    {0, 1, 2}, {0, 1, 3}, {0, 2, 3}, {1, 2, 3}, {0, 1, 3}, {0, 1, 4}, {0, 3, 4}, {1, 3, 4},
};

// fn_8038AD24 / fn_8038BD08 take the position by value; the original copies
// mPos after loading the serial and layer, and loads the flags after the copy.
static inline bool talkAt(s32 player, u32 msg, s32 serial, s32 layer,
                          const JGeometry::TVec3f& pos) {
    return fn_8038AD24(player, msg, serial, layer, pos, 0x28);
}

static inline bool talkAtFlags21(s32 player, u32 msg, s32 layer, const JGeometry::TVec3f& pos) {
    return fn_8038BD08(player, msg, layer, pos, 0x21);
}

GamblingShop_804AAA70::~GamblingShop_804AAA70() {}

void GamblingShop_804AAA70::fn_8030FDC0(s32 value) {
    field_0x25c = value;
    field_0x234 = 0x3C;
    mState = 6;
}

void GamblingShop_804AAA70::draw() {
    if (field_0x288) {
        fn_801F06F0("enemy_both_sob.bin", field_0x240 + 0x12C, NULL, 0, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x240 + 0x12C, 0, 0, -1);
    } else {
        // FIXME: 82.02 %. The true branch matches; here the frame index is
        // scheduled differently (the original loads field_0x240 before field_0x238 * 2).
        fn_801F06F0("enemy_both_sob.bin", field_0x238 * 2 + field_0x240 + 0xEC, NULL, 0, 1.0f,
                    1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, field_0x238 * 2 + field_0x240 + 0xEC, 0, 0, -1);
    }
}

void GamblingShop_804AAA70::update() {
    // FIXME: 97.87 %. The original reads field_0x4 before the fn_8023DE58 call
    // (into a register kept across it); here it is read after the call. The
    // player/layer and chest-slot loops also take their registers in a different
    // order (regalloc), and the jump table and one float literal are symbol and
    // data-order differences.
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);

    if (field_0x288) {
        field_0x244 += 1;
        if (field_0x244 >= 0x10) {
            field_0x244 = 0;
            field_0x240 ^= 1;
        }
    } else {
        if (field_0x23c <= 0) {
            field_0x23c = 0x14;
            s32 player = field_0x248;
            if (player >= 0) {
                player = fn_8022EA0C(player);
            }
            JGeometry::TVec3f dir;
            f32 dist;
            // The layer is on the left: that compare order is what the original emits.
            if (player >= 0 && field_0x4 == fn_8023DE58(player)) {
                fn_801EE558(&dir, &dist, player + 1);
            } else {
                fn_801EE558(&dir, &dist, 0);
            }
            if (dist >= 80.0f) {
                field_0x238 = 2;
            } else {
                u16 angle = JMath::atanTable_.atan2_(-dir.x, -dir.y) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                if (angle < 0x2000 || angle >= 0xE000) {
                    field_0x238 = 3;
                } else if (angle >= 0x2000 && angle < 0x6000) {
                    field_0x238 = 1;
                } else if (angle >= 0x6000 && angle <= 0xA000) {
                    field_0x238 = 2;
                } else {
                    field_0x238 = 0;
                }
            }
        } else {
            field_0x23c -= 1;
        }
        field_0x244 += 1;
        if (field_0x244 >= 0x14) {
            field_0x244 = 0;
            field_0x240 ^= 1;
        }
    }

    switch (mState) {
    case 0: {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player < 0) {
            break;
        }
        if (!fn_802300EC(player)) {
            break;
        }
        s32 slot = fn_8022F340(player);
        if (field_0x278[slot] < field_0x268) {
            u32 msg;
            if (field_0x288) {
                msg = 0x583;
            } else if (field_0x264 != 0) {
                msg = 0x6F5;
            } else {
                msg = 0x6FF;
            }
            if (talkAt(player, msg, mSerial, field_0x4, mPos)) {
                field_0x248 = player;
                mState = 1;
            }
        } else {
            u32 msg = field_0x288 ? 0x5AF : 0x6FB;
            if (talkAt(player, msg, mSerial, field_0x4, mPos)) {
                field_0x248 = player;
                mState = 8;
            }
        }
        break;
    }
    case 1: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (!fn_803865B8(field_0x248)) {
            break;
        }
        if (fn_8037EFB4(field_0x248) == 0) {
            if (fn_80236228(field_0x248) >= field_0x260) {
                u32 msg = field_0x288 ? 0x5AC : 0x700;
                if (fn_8038470C(field_0x248, msg)) {
                    fn_802362C4(field_0x248, -field_0x260);
                    mState = 3;
                }
            } else {
                u32 msg;
                if (field_0x288) {
                    msg = 0x5AE;
                } else if (field_0x264 != 0) {
                    msg = 0x6F6;
                } else {
                    msg = 0x702;
                }
                if (fn_8038470C(field_0x248, msg)) {
                    mState = 2;
                }
            }
        } else {
            u32 msg = field_0x288 ? 0x5AD : 0x701;
            if (fn_8038470C(field_0x248, msg)) {
                mState = 2;
            }
        }
        break;
    }
    case 2: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x248, mSerial)) {
            field_0x248 = -1;
            mState = 0;
        }
        break;
    }
    case 3: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (!fn_80388CAC(field_0x248, mSerial)) {
            break;
        }
        field_0x248 = fn_8022F340(field_0x248);
        s32 roll = (s32)(10.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
        s32 bucket;
        if ((u32)roll <= field_0x26c) {
            bucket = 0;
        } else if ((u32)roll <= field_0x270) {
            bucket = 1;
        } else if ((u32)roll <= field_0x274) {
            bucket = 2;
        } else {
            bucket = 3;
        }
        const s32* row = lbl_804713C0[bucket | (field_0x264 << 2)];
        for (s32 k = 0; k < field_0x24c; k++) {
            s32 actorSlot = field_0x250[k];
            ActorBase_804A5770* actor = fn_801F856C()->mActors[actorSlot];
            if (actor != NULL && actor->mActorId == 'GMTB') {
                static_cast<GamblingShopChest_804AAAE8*>(actor)->fn_80310F50(field_0x248, row[k],
                                                                              mSerial);
            }
        }
        field_0x278[field_0x248] += 1;
        mState = 4;
        break;
    }
    case 4: {
        s32 player = fn_8022EA0C(field_0x248);
        if (fn_8023DE58(player) != field_0x4) {
            for (s32 k = 0; k < field_0x24c; k++) {
                s32 actorSlot = field_0x250[k];
                ActorBase_804A5770* actor = fn_801F856C()->mActors[actorSlot];
                if (actor != NULL && actor->mActorId == 'GMTB') {
                    static_cast<GamblingShopChest_804AAAE8*>(actor)->fn_80310E48();
                }
            }
            field_0x248 = -1;
            mState = 0;
            break;
        }
        fn_8022FF8C(player, &box, field_0x4, mSerial, true, false);
        if (fn_8022FD8C(mSerial) != player) {
            break;
        }
        if (!fn_802300EC(player)) {
            break;
        }
        u32 msg = field_0x288 ? 0x5AC : 0x700;
        if (talkAt(player, msg, mSerial, field_0x4, mPos)) {
            field_0x248 = player;
            mState = 5;
        }
        break;
    }
    case 5: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (!fn_80388CAC(field_0x248, mSerial)) {
            break;
        }
        field_0x248 = fn_8022F340(field_0x248);
        mState = 4;
        break;
    }
    case 6: {
        if (field_0x234 > 0) {
            field_0x234 -= 1;
            break;
        }
        u32 msg;
        if (field_0x264 != 0) {
            switch (field_0x25c) {
            case 1:
                msg = 0x6F8;
                break;
            case 3:
                msg = 0x6F9;
                break;
            case 4:
                msg = 0x6FA;
                break;
            case 0:
            case 2:
            default:
                msg = 0x6F7;
                break;
            }
        } else {
            switch (field_0x25c) {
            case 1:
                msg = 0x704;
                break;
            case 2:
                msg = 0x705;
                break;
            case 3:
                msg = 0x706;
                break;
            case 0:
            default:
                msg = 0x703;
                break;
            }
        }
        s32 player = fn_8022EA0C(field_0x248);
        if (talkAtFlags21(player, msg, field_0x4, mPos)) {
            for (s32 k = 0; k < field_0x24c; k++) {
                s32 actorSlot = field_0x250[k];
                ActorBase_804A5770* actor = fn_801F856C()->mActors[actorSlot];
                if (actor != NULL && actor->mActorId == 'GMTB') {
                    static_cast<GamblingShopChest_804AAAE8*>(actor)->fn_80310E98();
                }
            }
            field_0x248 = player;
            field_0x234 = 0x3C;
            mState = 7;
        }
        break;
    }
    case 7: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (field_0x234 > 0) {
            field_0x234 -= 1;
            break;
        }
        if (!fn_80388CAC(field_0x248, mSerial)) {
            break;
        }
        for (s32 k = 0; k < field_0x24c; k++) {
            s32 actorSlot = field_0x250[k];
            ActorBase_804A5770* actor = fn_801F856C()->mActors[actorSlot];
            if (actor != NULL && actor->mActorId == 'GMTB') {
                static_cast<GamblingShopChest_804AAAE8*>(actor)->fn_80310E98();
            }
        }
        s32 roll = (s32)(3.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
        switch (roll) {
        case 0: {
            s32 tmp = field_0x250[0];
            field_0x250[0] = field_0x250[1];
            field_0x250[1] = tmp;
            break;
        }
        case 1: {
            s32 tmp = field_0x250[1];
            field_0x250[1] = field_0x250[2];
            field_0x250[2] = tmp;
            break;
        }
        default: {
            s32 tmp = field_0x250[0];
            field_0x250[0] = field_0x250[2];
            field_0x250[2] = tmp;
            break;
        }
        }
        field_0x248 = -1;
        mState = 0;
        break;
    }
    case 8: {
        fn_8022FF8C(field_0x248, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x248, mSerial)) {
            field_0x248 = -1;
            mState = 0;
        }
        break;
    }
    }
}

void GamblingShop_804AAA70::vfunc_0x10() {
    // FIXME: 99.46 %. Regalloc only: the original keeps the chest-slot pointer
    // (this, stored with 0x250(rN)) in r28 and the loop's byte offset in r29.
    s32 actorEnd = fn_801F856C()->mActorEnd;
    s32 count = 0;
    for (s32 i = 0; i < actorEnd; i++) {
        ActorBase_804A5770* actor = fn_801F856C()->mActors[i];
        if (actor != NULL && actor->mActorId == 'GMTB') {
            field_0x250[count] = i;
            count++;
            if (count >= 3) {
                break;
            }
        }
    }
    if (count == 0) {
        mAlive = false;
        return;
    }
    field_0x24c = count;
    s32 pick = (s32)(6.0f * lbl_8053AAF8->mRandom.get_ufloat_1());
    switch (pick) {
    case 0: {
        s32 tmp = field_0x250[0];
        field_0x250[0] = field_0x250[1];
        field_0x250[1] = tmp;
        break;
    }
    case 1: {
        s32 tmp = field_0x250[1];
        field_0x250[1] = field_0x250[2];
        field_0x250[2] = tmp;
        break;
    }
    case 2: {
        s32 tmp = field_0x250[0];
        field_0x250[0] = field_0x250[2];
        field_0x250[2] = tmp;
        break;
    }
    case 3: {
        s32 tmp = field_0x250[0];
        field_0x250[0] = field_0x250[1];
        field_0x250[1] = field_0x250[2];
        field_0x250[2] = tmp;
        break;
    }
    case 4: {
        s32 tmp = field_0x250[0];
        field_0x250[0] = field_0x250[2];
        field_0x250[2] = field_0x250[1];
        field_0x250[1] = tmp;
        break;
    }
    case 5:
        break;
    }
}

void GamblingShop_804AAA70::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    mState = 0;
    field_0x234 = 0;
    field_0x248 = -1;
    field_0x264 = field_0x90 >> 31;
    if (field_0x264 != 0) {
        field_0x260 = 0x12C;
    } else {
        field_0x260 = 0x64;
    }
    field_0x24c = 0;
    field_0x250[0] = -1;
    field_0x250[1] = -1;
    field_0x250[2] = -1;
    field_0x26c = field_0x90 & 0xF;
    field_0x270 = field_0x26c + ((field_0x90 >> 4) & 0xF);
    field_0x274 = field_0x270 + ((field_0x90 >> 8) & 0xF);
    field_0x25c = -1;
    field_0x268 = (field_0x90 >> 24) & 0xF;
    field_0x278[0] = 0;
    field_0x278[1] = 0;
    field_0x278[2] = 0;
    field_0x278[3] = 0;
    if (lbl_8053AAF8->field_0x7f && lbl_80529DEC.field_0x118 == 1 &&
        lbl_80529DEC.field_0x11c == 1) {
        field_0x288 = true;
    } else {
        field_0x288 = false;
    }
    field_0x238 = 2;
    field_0x23c = 0x14;
    field_0x240 = 0;
    field_0x244 = 0;
}
