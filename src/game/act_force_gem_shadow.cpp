#include "game/GameAudio.h"
#include "game/act/act_force_gem_shadow.h"

ForceGemShadowNPC_804ACD18::~ForceGemShadowNPC_804ACD18() {}

void ForceGemShadowNPC_804ACD18::draw() {
    fn_803075AC(mSerial, field_0x4, &mPos, 0x3CD, false, 2, -1, 0, 0, 0);
}

void ForceGemShadowNPC_804ACD18::update() {
    if (field_0x230 >= 50) {
        fn_801F3668();
    } else {
        field_0x230++;
    }
}

void ForceGemShadowNPC_804ACD18::init() {
    field_0x230 = 0;
    mPos.z = 0.0f;
}
