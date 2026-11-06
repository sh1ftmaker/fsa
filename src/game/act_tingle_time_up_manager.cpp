#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"
#include "game/tu_80182914.h"
#include "game/tu_802EF76C.h"
#include "game/act/act_force_gem.h"
#include "game/act/act_force_gem_buried.h"
#include "game/act/act_force_gem_fleeing.h"
#include "game/act/act_force_gem_winged.h"
#include "game/act/tu_803536C4.h"
#include "game/act/act_tingle_time_up_manager.h"

// FIXME: 99.8 %. Epilogue reloads r0 and r31 in the other order.
void TingleTimeUpManager_804ACBF0::init() {
    mTimer = 0;
    field_0x238 = 0;
    mViewBox = getGameView_802D6A9C()->mVisibleBox;
    mState = 0;
    field_0x23c = fn_8018330C().toUInt32();
    field_0x240 = field_0x23c;
    field_0x244 = (f32)(field_0x23c >> 24);
    field_0x248 = (f32)((field_0x23c >> 16) & 0xFF);
    field_0x24c = (f32)((field_0x23c >> 8) & 0xFF);
    field_0x250 = (f32)(field_0x23c & 0xFF);
    field_0x254 = -(0.4f * field_0x244) / 30.0f;
    field_0x258 = -(0.4f * field_0x248) / 30.0f;
    field_0x25c = -(0.4f * field_0x24c) / 30.0f;
    field_0x264 = 0.0f;
    field_0x268 = 0.0f;
    field_0x260 = 0.0f;
}

// FIXME: 91.2 % (missing/extra + reloc-name, 698 vs 699 instrs). `this` is kept
// in r30 instead of r28, state 3/4 evaluate the TVec3f constructor before
// __cvt_fp2unsigned, and state 2's loop counter setup is 1 instruction shorter.
void TingleTimeUpManager_804ACBF0::update() {
    if (mState != 0 && mState != 7) {
        cls_804A03A8* obj30 = gGameRoot2_8053AB10->field_0x30;
        if (obj30->field_0x284 == 0) {
            obj30->field_0x284 = 1;
            obj30->field_0x28c = 0.05f;
        }
        gGameRoot2_8053AB10->field_0x20->field_0x216 = 1;
        gGameRoot2_8053AB10->field_0x24->field_0x17d = 1;
    }

    switch (mState) {
    case 0:
        gGameRoot_8053AAF8->fn_80134FC4(1);
        lbl_8053AD70->fn_8037AE00();
        gGameRoot_8053AAF8->fn_80134FBC(1);
        gGameRoot2_8053AB10->field_0x24->fn_8030C380(&field_0x27c);
        field_0x264 = field_0x288.y - field_0x27c.y;
        field_0x268 = mViewBox.f.y - mViewBox.i.y;
        field_0x260 = field_0x264 / field_0x268;
        mTimer = 30;
        mState = 1;
        break;
    case 1:
        if (mTimer <= 0) {
            JGeometry::TVec3f pos(32.0f + field_0x288.x, (field_0x27c.y + field_0x288.y) * 0.5f + 32.0f, 32.0f);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUTL', 0, &pos, 0x40, -1, -1);
            if (serial >= 0) {
                cls_804ACC68* child = static_cast<cls_804ACC68*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (child != NULL) {
                    JGeometry::TVec3f vel(-2.0f, 0.0f, 0.0f);
                    child->field_0x3c = vel;
                    mTimer = 150;
                    child->field_0x254 = field_0x27c;
                    child->field_0x260 = field_0x288;
                    mState = 3;
                }
            }
            field_0x244 = 0.6f * (f32)(field_0x23c >> 24);
            field_0x248 = 0.6f * (f32)((field_0x23c >> 16) & 0xFF);
            field_0x24c = 0.6f * (f32)((field_0x23c >> 8) & 0xFF);
        } else {
            field_0x244 += field_0x254;
            field_0x248 += field_0x258;
            field_0x24c += field_0x25c;
        }
        break;
    case 3:
        if (mTimer <= 0) {
            u32 param = ((u32)(8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) << 4) | 1;
            JGeometry::TVec3f pos(32.0f + field_0x288.x, (field_0x27c.y + field_0x288.y) * 0.5f + 54.0f, 32.0f);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUTL', 0, &pos, param, -1, -1);
            if (serial >= 0) {
                cls_804ACC68* child = static_cast<cls_804ACC68*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (child != NULL) {
                    JGeometry::TVec3f vel(-2.2f, -0.2f, 0.0f);
                    child->field_0x3c = vel;
                    mTimer = 15;
                    child->field_0x254 = field_0x27c;
                    child->field_0x260 = field_0x288;
                    mState = 4;
                }
            }
        }
        break;
    case 4:
        if (mTimer <= 0) {
            u32 param = ((u32)(8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) << 4) | 2;
            JGeometry::TVec3f pos(32.0f + field_0x288.x, (field_0x27c.y + field_0x288.y) * 0.5f + 8.0f, 32.0f);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUTL', 0, &pos, param, -1, -1);
            if (serial >= 0) {
                cls_804ACC68* child = static_cast<cls_804ACC68*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (child != NULL) {
                    JGeometry::TVec3f vel(-2.3f, 0.3f, 0.0f);
                    child->field_0x3c = vel;
                    mTimer = 45;
                    child->field_0x254 = field_0x27c;
                    child->field_0x260 = field_0x288;
                    mState = 2;
                }
            }
        }
        break;
    case 2:
        if (mTimer <= 0) {
            mTimer = 150;
            mState = 5;
            s32 count = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < count; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL) {
                    u32 id = actor->mActorId;
                    if (id == ACTOR_RUPY) {
                        ForceGem_804AB6D8* gem = static_cast<ForceGem_804AB6D8*>(actor);
                        s32 state = gem->field_0x230;
                        // Range tests written as (u32)(state - N) <= K: the natural == / >= && <= forms compile to different code (tried: 90.9 % and 90.0 % vs 91.2 %).
                        if (state == 0 || (u32)(state - 15) <= 1 || (u32)(state - 1) <= 1 || state == 9 || state == 13) {
                            actor->fn_801F3668();
                        }
                    } else if (id == ACTOR_ESRY) {
                        FleeingForceGem_804AB170* gem = static_cast<FleeingForceGem_804AB170*>(actor);
                        s32 state = gem->field_0x230;
                        if (state == 0 || (u32)(state - 3) <= 1 || state == 10) {
                            actor->fn_801F3668();
                        }
                    } else if (id == ACTOR_HNRY) {
                        WingedForceGem_804AB098* gem = static_cast<WingedForceGem_804AB098*>(actor);
                        s32 state = gem->field_0x230;
                        if (state == 0 || state == 3 || state == 4) {
                            if (gem->field_0x281 == 0) {
                                actor->fn_801F3668();
                            }
                        }
                    } else if (id == ACTOR_WDRY) {
                        actor->fn_801F3668();
                    } else if (id == ACTOR_BRRY) {
                        BuriedForceGem_804AB258* gem = static_cast<BuriedForceGem_804AB258*>(actor);
                        if (gem->field_0x230 == 0) {
                            actor->fn_801F3668();
                        }
                    }
                }
            }
        }
        break;
    case 5:
        if (mTimer <= 0) {
            mTimer = 250;
            mState = 6;
        }
        {
            u32 param = 3;
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() > 0.8f) {
                param = ((u32)(8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) << 4) | 3;
            } else {
                param |= 0x80;
            }
            JGeometry::TVec3f pos(
                32.0f + field_0x288.x,
                (64.0f + mViewBox.i.y) + (f32)(s32)(((mViewBox.f.y - mViewBox.i.y) - 48.0f) * gGameRoot_8053AAF8->mRandom.get_ufloat_1()),
                32.0f
            );
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUTL', 0, &pos, param | 0x100, -1, -1);
            field_0x238++;
            if (serial >= 0) {
                cls_804ACC68* child = static_cast<cls_804ACC68*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                if (child != NULL) {
                    JGeometry::TVec3f vel(
                        -2.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                        0.5f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 0.25f,
                        0.0f
                    );
                    child->field_0x3c = vel;
                    child->field_0x254 = field_0x27c;
                    child->field_0x260 = field_0x288;
                }
            }
        }
        break;
    case 6:
        if (getActorManager_801F856C()->fn_801F7A08('TUTL') < 0) {
            startSound_801F0E34(0x5001B, 0);
            gGameRoot_8053AAF8->fn_80134FC4(0);
            lbl_8053AD70->fn_8037AE30();
            lbl_8053AED0->fn_8043D67C(0x148, 0, 0);
            for (s32 i = 0; i < 4; i++) {
                bool takesPart = gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i);
                if (takesPart == true && !fn_8022F514(i)) {
                    lbl_8053AED0->fn_8043D874(i, -300, 1);
                }
            }
            gGameRoot_8053AAF8->fn_80134FBC(0);
            mTimer = 60;
            mState = 7;
            field_0x254 = ((f32)(field_0x240 >> 24) - field_0x244) / 60.0f;
            field_0x258 = ((f32)((field_0x240 >> 16) & 0xFF) - field_0x248) / 60.0f;
            field_0x25c = ((f32)((field_0x240 >> 8) & 0xFF) - field_0x24c) / 60.0f;
        }
        break;
    case 7:
        if (mTimer <= 0) {
            if (getGameView_802D6A9C()->field_0x4 != 5 || getGameView_802D6A9C()->field_0x8 != 0) {
                u32 color = field_0x240;
                fn_8013A884(&color);
            }
            fn_801F3668();
            return;
        }
        field_0x244 += field_0x254;
        field_0x248 += field_0x258;
        field_0x24c += field_0x25c;
        break;
    }

    if (getGameView_802D6A9C()->field_0x4 != 5 || getGameView_802D6A9C()->field_0x8 != 0) {
        u32 color = ((s32)field_0x244 << 24) | ((s32)field_0x248 << 16) | ((s32)field_0x24c << 8) | (s32)field_0x250;
        fn_8013A884(&color);
    }

    if (mTimer > 0) {
        mTimer--;
    }
}

void TingleTimeUpManager_804ACBF0::draw() {}

TingleTimeUpManager_804ACBF0::~TingleTimeUpManager_804ACBF0() {}
