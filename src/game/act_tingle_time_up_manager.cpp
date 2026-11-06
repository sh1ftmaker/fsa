#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/tu_80182914.h"
#include "game/act/act_tingle_time_up_manager.h"

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

void TingleTimeUpManager_804ACBF0::draw() {}

TingleTimeUpManager_804ACBF0::~TingleTimeUpManager_804ACBF0() {}
