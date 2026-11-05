#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_merchant.h"

// The four animations' keys: cel 0, 1, 2, 1 for 20 frames each, then the terminator.
static const s16 lbl_80464000[10] = {0, 20, 1, 20, 2, 20, 1, 20, -1, -1};
// The table of animations (one), followed by a zero word in the original.
static const s16* lbl_80539ED8[2] = {lbl_80464000, NULL};

// True while a shop item ('SITM') actor exists.
static inline bool isShopItemAlive() {
    s32 serial = getActorManager_801F856C()->fn_801F7A08('SITM');
    if (serial < 0) {
        return false;
    }
    if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) == NULL) {
        return false;
    }
    return true;
}

Merchant_8049CD48::~Merchant_8049CD48() {}

void Merchant_8049CD48::draw() {
    ActorBase_804A5770::draw();
    s32 frame = mAnim.mCel + 0x1AB;
    const JGeometry::TVec3f* pos = &mPos;
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(pos, res, frame, NULL, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, mAnim.mCel + 0x1AB, false, 1, -1, 0, 0, 0);
}

// FIXME: 92.13%, scheduling only. In the fn_8038AD24 call the original loads mSerial, the
// mPos copy address, field_0x4 and the player before the psq_l copy of mPos and the
// clrlwi of msg; ours copies mPos (psq_l/lfs) first and loads the serial and layer last.
void Merchant_8049CD48::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    switch (mState) {
    case 0:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player >= 0) {
            if (fn_802300EC(player)) {
                u16 msg = 0x259;
                bool found = isShopItemAlive();
                if (!found) {
                    msg = 0x2AA;
                }
                if (fn_8038AD24(player, msg, mSerial, field_0x4, mPos, 0x28)) {
                    mPlayer = player;
                    mState = 1;
                }
            }
        }
        break;
    case 1:
        fn_8022FF8C(mPlayer, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mPlayer = -1;
            mState = 0;
        }
        break;
    case 2:
        break; // nothing sets 2
    case 3:
        if (fn_80388EEC(mPlayer)) {
            mPlayer = -1;
            mState = 0;
        }
        break;
    }
    mAnim.fn_801D1C18();
}

void Merchant_8049CD48::init() {
    ActorBase_804A5770::init();
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    mAnim.fn_801D2608(lbl_80539ED8, 1);
    mAnim.startAnim_801D22DC(0, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

Merchant_8049CD48::Merchant_8049CD48() : mState(0), mPlayer(-1) {}
