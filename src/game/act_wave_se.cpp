// Actor WAVE ("Wave SE"): plays two wave sounds from its own sound object,
// the second repeating every 60 frames.

#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/act/act_wave_se.h"

void WaveSe_804A9EA8::init() {
    field_0x234 = 60;
    mSound = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

WaveSe_804A9EA8::~WaveSe_804A9EA8() {
    if (mSound != NULL) {
        delete mSound;
    }
}

// Plays sound 0x3003C every frame and, whenever the counter is 0, resets it
// to 60 and plays 0x3003D.
void WaveSe_804A9EA8::update() {
    mSound->startSound(0x3003C);
    if (field_0x234 == 0) {
        field_0x234 = 60;
        mSound->startSound(0x3003D);
    }
}

void WaveSe_804A9EA8::draw() {}
