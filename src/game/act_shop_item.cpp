#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_shop_item.h"

// Prices by item kind (kind 0-7), then a zero terminator.
static const u16 lbl_80464028[10] = {300, 100, 16, 300, 200, 300, 600, 900, 10000, 0};
// Message ids: [0] offered when a player stands by the item, [2] when the
// player can't pay, [3] when the message in progress reports a result.
static const u32 lbl_8046403C[4] = {0x257, 0x25C, 0x25A, 0x25B};

// The eight animations: cel 0-7 for 60 frames, then the terminator.
static const s16 lbl_8053C328[4] = {0, 60, -1, -1};
static const s16 lbl_8053C330[4] = {1, 60, -1, -1};
static const s16 lbl_8053C338[4] = {2, 60, -1, -1};
static const s16 lbl_8053C340[4] = {3, 60, -1, -1};
static const s16 lbl_8053C348[4] = {4, 60, -1, -1};
static const s16 lbl_8053C350[4] = {5, 60, -1, -1};
static const s16 lbl_8053C358[4] = {6, 60, -1, -1};
static const s16 lbl_8053C360[4] = {7, 60, -1, -1};
static const s16* lbl_8049CDA0[8] = {
    lbl_8053C328, lbl_8053C330, lbl_8053C338, lbl_8053C340,
    lbl_8053C348, lbl_8053C350, lbl_8053C358, lbl_8053C360,
};

// fn_8038AD24 takes the position by value. The original loads the message
// arguments before it copies mMsgPos and the flags after, so the copy is made
// inside this helper.
static inline bool showMessage(s32 player, u32 msg, s32 serial, s32 layer,
                               const JGeometry::TVec3f& pos) {
    return fn_8038AD24(player, msg, serial, layer, pos, 0x28);
}

ShopItem_8049CDC0::~ShopItem_8049CDC0() {}

void ShopItem_8049CDC0::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    fn_801F02BC("enemy_both2_sob.bin", mAnim.mCel + 0x17B, &params, 7, 0, false, 1.0f, 1.0f);
}

void ShopItem_8049CDC0::update() {
    ActorBase_804A5770::update();
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    switch (mState) {
    case 0: {
        fn_80239914(&box, field_0x4, mSerial);
        fn_8022FEB8(&box, field_0x4, mSerial, false, false);
        s32 player = fn_8022FD8C(mSerial);
        if (player < 0) {
            break;
        }
        if (!fn_802300EC(player)) {
            break;
        }
        s32 slot = player;
        if (fn_8022F514(player)) {
            slot = fn_8022F3DC(player);
        }
        if (lbl_8053AD30->fn_80309D6C(fn_8030AAF4(slot), 0x120) == 0) {
            break;
        }
        if (showMessage(player, lbl_8046403C[0], mSerial, field_0x4, mMsgPos)) {
            mPlayer = player;
            mState = 1;
        }
        break;
    }
    case 1: {
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (!fn_803865B8(mPlayer)) {
            break;
        }
        switch (fn_8037EFB4(mPlayer)) {
        case 0: {
            bool canAfford = false;
            switch (mKind) {
            case 2:
                if (fn_80237774(mPlayer) > lbl_80464028[mKind]) {
                    canAfford = true;
                }
                break;
            case 0:
            case 1:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                if (fn_80236228(mPlayer) >= lbl_80464028[mKind]) {
                    canAfford = true;
                }
                break;
            }
            if (canAfford) {
                fn_80388CAC(mPlayer, mSerial);
                fn_802308E0(mPlayer);
                mState = 2;
            } else {
                fn_8038470C(mPlayer, lbl_8046403C[2]);
                mState = 5;
            }
            break;
        }
        case 1:
        default:
            fn_8038470C(mPlayer, lbl_8046403C[3]);
            mState = 4;
            break;
        }
        break;
    }
    case 2: {
        s32 player = mPlayer;
        if (player >= 0) {
            mDrawEnabled = false;
            switch (mKind) {
            case 0:
                fn_802362C4(player, -lbl_80464028[0]);
                break;
            case 1:
                fn_802362C4(player, -lbl_80464028[1]);
                break;
            case 2:
                fn_8023764C(player, fn_80237774(player) - lbl_80464028[2]);
                break;
            case 3:
                fn_802362C4(player, -lbl_80464028[3]);
                break;
            case 4:
                fn_802362C4(player, -lbl_80464028[4]);
                fn_80235FE8(player);
                fn_802372F8(player, 9, true);
                break;
            case 5:
                fn_802362C4(player, -lbl_80464028[5]);
                fn_80235FE8(player);
                fn_802372F8(player, 4, true);
                break;
            case 6:
                fn_802362C4(player, -lbl_80464028[6]);
                fn_80235FE8(player);
                fn_802372F8(player, 3, true);
                break;
            case 7:
                fn_802362C4(player, -lbl_80464028[7]);
                fn_80235FE8(player);
                fn_802372F8(player, 6, true);
                break;
            }
        }
        mState = 3;
        u32 flags = 0;
        switch (mKind) {
        case 2:
            flags |= 0x4;
            break;
        case 0:
            flags |= 0x40020;
            break;
        case 1:
            flags |= 0x50000;
            break;
        case 3:
            flags |= 0x40014;
            break;
        case 4:
            flags |= 0x10008;
            break;
        case 5:
            flags |= 0x10003;
            break;
        case 6:
            flags |= 0x10002;
            break;
        case 7:
            flags |= 0x10005;
            break;
        }
        flags |= mPlayer << 24;
        flags |= mPlayer << 26;
        JGeometry::TVec3f* pos = fn_8023E724(mPlayer);
        fn_801F856C()->fn_801F9484('IMUP', field_0x4, pos, flags, -1, -1);
        break;
    }
    case 3:
        if (!fn_80230188(mPlayer)) {
            fn_8023061C(mPlayer, false);
            mState = 6;
        }
        break;
    case 4:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mState = 0;
            mPlayer = -1;
        }
        break;
    case 5:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(mPlayer, mSerial)) {
            mState = 0;
            mPlayer = -1;
        }
        break;
    case 6:
        mPlayer = -1;
        fn_801F3668();
        break;
    }
    mAnim.fn_801D1C18();
}

void ShopItem_8049CDC0::init() {
    ActorBase_804A5770::init();
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mLife = 0;
    mKind = field_0x90 & 0xFF;
    mMsgPos.x = mPos.x;
    mMsgPos.y = mPos.y;
    mMsgPos.z = mPos.z;
    mMsgPos.y -= 32.0f;
    mAnim.fn_801D2608(lbl_8049CDA0, 8);
    mAnim.fn_801D22DC(mKind, 2, false);
    mAnim.fn_801D1F14(0.0f);
}

ShopItem_8049CDC0::ShopItem_8049CDC0() {
    mKind = 8;
    mState = 0;
    mPlayer = -1;
    field_0x274 = 0;
}
