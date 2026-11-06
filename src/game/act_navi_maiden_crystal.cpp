#include "game/GameAudio.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_navi_maiden_crystal.h"

void Navi54MaidenCrystalCutscene_804ADFE8::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x268 != 0) {
        field_0x268--;
    }
    if (field_0x268 == 0) {
        field_0x268 = 1;
        field_0x26c++;
        if (field_0x26c >= 6) {
            field_0x26c = 0;
        }
    }
    drawSprite_801F06F0("enemy_both2_sob.bin", field_0x264 + field_0x26c, &params, 7, 1.0f, 1.0f);
}

void Navi54MaidenCrystalCutscene_804ADFE8::fn_803676CC() {
    if (mEmitter270 != NULL) {
        mEmitter270->setStatus(JPAEMIT_ForceDelete);
        mEmitter270 = NULL;
    }
    if (mEmitter274 != NULL) {
        mEmitter274->setStatus(JPAEMIT_ForceDelete);
        mEmitter274 = NULL;
    }
}

Navi54MaidenCrystalCutscene_804ADFE8::~Navi54MaidenCrystalCutscene_804ADFE8() {}
