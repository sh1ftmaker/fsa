// Actor GMTB ("Gambling shop chest" in docs/names/actors.tsv): the spawner
// allocates 0x258 bytes for it and calls fn_80200904, which stores the
// vtable 0x804AAAE8. Its tile is rewritten through lbl_8053AC98.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_gambling_chest.h"
#include "game/act/act_gambling_shop.h"

// Indexed by field_0x238 (0-4). Passed to fn_802362C4 (the player's word).
static const s32 lbl_80471438[5] = {1, 100, 150, 300, 1000};
// Sprite ids (the chest's sprite frame and fn_803075AC's id).
static const s32 lbl_8047144C[5] = {1, 0x11, 0x15, 0x1D, 0x45A};

GamblingShopChest_804AAAE8::~GamblingShopChest_804AAAE8() {}

void GamblingShopChest_804AAAE8::fn_80310E48() {
    lbl_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x10);
    field_0x234 = -1;
    mState = 0;
}

void GamblingShopChest_804AAAE8::fn_80310E98() {
    if (mState == 1) {
        lbl_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x10);
        startSound_801F0E34(0x30076, 0);
        field_0x248.x = mPos.x;
        field_0x248.y = mPos.y;
        field_0x248.z = mPos.z;
        field_0x254 = false;
        field_0x240 = 0x5A;
        mState = 2;
    } else if (mState == 3) {
        fn_8023061C(field_0x234, false);
        field_0x234 = -1;
        mState = 0;
    }
}

void GamblingShopChest_804AAAE8::fn_80310F50(s32 a, s32 b, s32 serial) {
    lbl_8053AC98->fn_802A3884(&mPos, 0, field_0x4, 0x30);
    field_0x234 = a;
    field_0x238 = b;
    field_0x23c = serial;
    mState = 1;
}

void GamblingShopChest_804AAAE8::draw() {
    if (mState >= 2) {
        JGeometry::TVec3f savedPos = mPos;
        mPos = field_0x248;
        SpriteParams_801F5AE0 params;
        params.field_0x1d = 1;
        params.field_0x1c = 1;
        params.field_0x1f = 0;
        if (field_0x238 == 4) {
            Mtx mtx;
            PSMTXScale(mtx, 2.0f, 2.0f, 2.0f);
            params.mMtx = mtx;
            drawSprite_801F06F0("enemy_gc_sob.bin", 0x45A, &params, -1, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x11, true, 2, -1, 1, 0, 0);
        } else {
            drawSprite_801F06F0("enemy_both2_sob.bin", lbl_8047144C[field_0x238], &params, -1, 1.0f,
                        1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, lbl_8047144C[field_0x238], false, 2, -1, 1, 0,
                        0);
        }
        mPos = savedPos;
    }
}

void GamblingShopChest_804AAAE8::update() {
    switch (mState) {
    case 1: {
        JGeometry::TBox2f box(mBox);
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
        s32 player = fn_8022EA0C(field_0x234);
        if (player == fn_8023AF14(&box, field_0x4, false, 0.0f)) {
            if (fn_80230CFC(player) == 3 && !fn_8023EFEC(player)) {
                if (lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(fn_8022F340(player)), 0x100) != 0 &&
                    fn_802300EC(player) == true) {
                    field_0x234 = player;
                    fn_802308E0(field_0x234);
                    lbl_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x10);
                    fn_80230FD0(field_0x234, 0x20013);
                    ActorBase_804A5770* shop = fn_801F856C()->findActorBySerial_801F666C(field_0x23c);
                    if (shop != NULL && shop->mActorId == ACTOR_GMBL) {
                        static_cast<GamblingShop_804AAA70*>(shop)->fn_8030FDC0(field_0x238);
                    }
                    JGeometry::TVec3f playerPos = *fn_8023E724(field_0x234);
                    field_0x248.set(playerPos.x, playerPos.y - 12.0f, playerPos.z);
                    field_0x244 = 0;
                    field_0x254 = true;
                    fn_80230498(field_0x234, &playerPos, field_0x244 + 0xED, 0xFF, -0x80, -0x80,
                                true);
                    fn_802362C4(field_0x234, lbl_80471438[field_0x238]);
                    field_0x240 = 0x5A;
                    mState = 2;
                }
            }
        }
        break;
    }
    case 2:
        switch (field_0x240) {
        case 0:
            if (field_0x254) {
                mState = 3;
            } else {
                field_0x234 = -1;
                mState = 0;
            }
            return;
        case 0x48:
            if (field_0x254) {
                fn_80230FD0(field_0x234, 0x10008);
            }
            // fall through
        case 0x4C:
        case 0x50:
            field_0x248.y -= 4.0f;
            if (field_0x254) {
                field_0x244++;
                fn_80230498(field_0x234, fn_8023E724(field_0x234), field_0x244 + 0xED, 0xFF, -0x80,
                            -0x80, true);
            }
            break;
        }
        field_0x240--;
        break;
    case 0:
    case 3: // nothing sets 3 here (fn_80310E98 handles it); the original's dispatch has the branch
        break;
    }
}

void GamblingShopChest_804AAAE8::init() {
    mPos.x += 8.0f;
    mPos.y += 12.0f;
    mBox.set(-6.0f, 0.0f, 6.0f, 6.0f);
    mState = 0;
    field_0x234 = -1;
    field_0x238 = 0;
    field_0x23c = -1;
    field_0x240 = 0;
    field_0x248 = mPos;
    field_0x254 = false;
    field_0x244 = 0;
}
