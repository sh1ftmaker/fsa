#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/act/act_spotlight_se.h"

// Four 5-bit fields of field_0x90, one per player.
static const u32 sFieldMask[4] = {0x07C00000, 0x003E0000, 0x0001F000, 0x00000F80};
static const u32 sFieldShift[4] = {22, 17, 12, 7};

void SpotlightSE_804A85E0::init() {}

void SpotlightSE_804A85E0::update() {
    u32 bits;
    s32 count = 0;
    for (s32 i = 0; i < 4; i++) {
        bits = field_0x90 & sFieldMask[i];
        if (bits != 0 && getGameView_802D6A9C()->fn_802D800C(bits >> sFieldShift[i])) {
            count++;
        }
    }
    if (count != 0) {
        startSound_801F0E34(0x30024, 0);
    }
}

void SpotlightSE_804A85E0::draw() {}

SpotlightSE_804A85E0::~SpotlightSE_804A85E0() {}
