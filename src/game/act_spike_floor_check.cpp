#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_spike_floor_check.h"

SpikeFloorCheck_804A8170::~SpikeFloorCheck_804A8170() {}

void SpikeFloorCheck_804A8170::draw() {}

void SpikeFloorCheck_804A8170::update() {
    switch (mState) {
    case 0:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0, 0, 0x40, 0xC0);
            mState = 1;
            startSound_801F0E34(0x30015, 0);
        }
        // fall through
    case 1:
        switch (mType) {
        case 0:
            if (vfunc_0x34()) {
                fn_801F0D20(3);
                mState = 2;
                mTimer = 8;
            }
            break;
        case 1:
        default:
            if (getActorManager_801F856C()->field_0x305c == 1) {
                fn_801F0D20(3);
                mTimer = 8;
                mState = 2;
            }
            break;
        }
        break;
    case 2:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0xC0, 0, 0x40, 0xC0);
            mState = 3;
            startSound_801F0E34(0x30016, 0);
            mTimer = 2;
        }
        break;
    case 3:
        mTimer--;
        if (mTimer == 0) {
            lbl_8053AB10->field_0x20->fn_802FA160(
                lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x180, 0, 0x40, 0xC0);
            getGameView_802D6A9C()->fn_802D7920(0);
            mAlive = false;
        }
        break;
    default:
        mAlive = false;
        break;
    }
}

void SpikeFloorCheck_804A8170::init() {
    mType = field_0x90 & 0xFF;
    if ((mType == 0 && vfunc_0x34()) || (mType != 0 && getActorManager_801F856C()->field_0x305c == 1)) {
        lbl_8053AB10->field_0x20->fn_802FA160(
            lbl_8053AAF8->field_0xa4->getResource('SCH ', "bg_tenso_sch.bin"), 0x180, 0, 0x40, 0xC0);
        getGameView_802D6A9C()->fn_802D7920(0);
        mAlive = false;
    } else {
        getGameView_802D6A9C()->fn_802D7920(1);
        mState = 0;
        mTimer = 0xF;
    }
}
