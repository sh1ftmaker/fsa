#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_post_warp_pillar_handling.h"

PostWarpPillarHandling_804AC1F8::~PostWarpPillarHandling_804AC1F8() {}

void PostWarpPillarHandling_804AC1F8::draw() {}

void PostWarpPillarHandling_804AC1F8::update() {
    field_0x3c.z -= 0.1;
    bool done = false;
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
        pos += field_0x3c;
        if (pos.z <= 0.0f) {
            pos.z = 0.0f;
            fn_80230BF4(i, 2);
            done = true;
        } else if (field_0x3c.z > 0.0f) {
            spawnEmitter_8013CC50(field_0x4, &pos, 0x345, 0, 0, NULL, NULL, true, 0.15f);
            fn_80230498(i, &pos, 0x3cf, field_0x230 * 255 / 40, -128, -128, true);
        } else {
            fn_80230498(i, &pos, 0x3e, 255, -128, -128, true);
        }
    }
    field_0x230++;
    if (field_0x230 > 40) {
        field_0x230 = 40;
    }
    if (done == true) {
        if (fn_80230534(false) == true) {
            fn_80231368();
            for (s32 i = 0; i < 4; i++) {
                fn_8022CA6C(i, 0, true);
            }
            mAlive = false;
        }
    }
}

void PostWarpPillarHandling_804AC1F8::init() {
    field_0x230 = 0;
    for (s32 i = 0; i < 4; i++) {
        if (!fn_80230188(i)) {
            mAlive = false;
        } else {
            field_0x3c.z = 4.0f;
        }
    }
    if (!(field_0x90 & 1)) {
        gGameRoot2_8053AB10->field_0x2c->field_0xc = 0xff;
    }
}
