#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/act_player_start_position.h"

PlayerStartPosition_804A8338::~PlayerStartPosition_804A8338() {}

static const s32 lbl_8046C890[4] = {2, 3, 0, 1};

void PlayerStartPosition_804A8338::draw() {}

void PlayerStartPosition_804A8338::update() {}

void PlayerStartPosition_804A8338::init() {
    field_0x234 = (field_0x90 >> 8) & 0xF;
    if (fn_802D6A9C()->field_0x131f6 == 0 || fn_802D6A9C()->field_0x131f9 == 2) {
        field_0x234 = 0;
    }
    fn_8023F1D4(field_0x90 & 0xF, &mPos, lbl_8046C890[(field_0x90 >> 4) & 3], field_0x234,
                (field_0x90 >> 12) & 1);
    mAlive = false;
}
