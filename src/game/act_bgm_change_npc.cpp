#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/act/act_bgm_change_npc.h"

void BGMChangeNPC_804A9DA0::init() {
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = ((field_0x90 >> 22) & 0x1F) + 1;
    field_0x23c = ((field_0x90 >> 17) & 0x1F) + 1;
    field_0x240 = (field_0x90 >> 12) & 0x1F;
    field_0x250 = (field_0x90 >> 7) & 1;
    GameView_804A9950* view = getGameView_802D6A9C();
    field_0x248 = view->field_0x34;
    f32 halfH = 16.0f * field_0x23c;
    f32 halfW = 16.0f * field_0x238;
    mBox.set(-halfW, -halfH, halfW, halfH);
    field_0x94 = 0xFFFF;
    field_0x230 = 0;
    field_0x24c = -1;
}

void BGMChangeNPC_804A9DA0::draw() {}

BGMChangeNPC_804A9DA0::~BGMChangeNPC_804A9DA0() {}
