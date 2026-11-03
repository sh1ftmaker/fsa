#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_magic_jar.h"

// GameView byte 0x131DF + layer. The layer is loaded before fn_802D6A9C is called.
static inline u8 viewLayerFlag(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    return view->field_0x131df[layer];
}

MagicJar_804AF018::~MagicJar_804AF018() {}

// A player picks it up: ends its lifetime and hands the player the refill.
inline void MagicJar_804AF018::collect(s32 player) {
    if (fn_802375B0(player) == 0x180) {
        fn_80230EB8(player, 0x12);
    }
    if (field_0x90 == 0) {
        fn_80237524(player, 0xC0);
    } else {
        fn_80237524(player, 0x30);
    }
    mAlive = false;
}

void MagicJar_804AF018::vfunc_0x40() {
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x230 = 2;
}

void MagicJar_804AF018::draw() {
    if (field_0x234 >= 0x100 || !(field_0x234 & 2)) {
        if (field_0x90 == 0) {
            fn_801F06F0("enemy_both2_sob.bin", 0x5C, NULL, 2, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x5C, false, 2, -1, 0, 0, 0);
        } else {
            fn_801F06F0("enemy_both2_sob.bin", 0x5D, NULL, 1, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x5D, false, 2, -1, 0, 0, 0);
        }
    }
}

void MagicJar_804AF018::update() {
    JGeometry::TVec2<f32> p(mPos.x, mPos.y);
    mPos += field_0x3c;
    field_0x234--;
    if (field_0x234 <= 0) {
        mAlive = false;
    } else {
        s32 player;
        JGeometry::TBox2f box;
        switch (field_0x230) {
        case 0:
            if (viewLayerFlag(field_0x4)) {
                field_0x3c.y += 0.1f;
                if (field_0x3c.y > 0.0f) {
                    field_0x230 = 1;
                }
            } else {
                field_0x3c.z -= 0.1f;
                if (field_0x3c.z < 0.0f) {
                    field_0x230 = 1;
                }
            }
            break;
        case 1:
            if (viewLayerFlag(field_0x4)) {
                field_0x3c.y += 0.1f;
                f32 dy = field_0x90 == 0 ? 8.0f : 4.0f;
                JGeometry::TVec3f probe(mPos.x, mPos.y + dy, mPos.z);
                if (vfunc_0x4C(field_0x4, &probe) == true) {
                    field_0x238--;
                    if (field_0x238 <= 0) {
                        field_0x3c.y = 0.0f;
                        field_0x230 = 2;
                    } else {
                        field_0x3c.y = -0.5f * field_0x238;
                        field_0x230 = 0;
                    }
                }
            } else {
                field_0x3c.z -= 0.1f;
                if (mPos.z <= 0.0f) {
                    field_0x238--;
                    mPos.z = 0.0f;
                    if (field_0x238 <= 0) {
                        field_0x3c.z = 0.0f;
                        field_0x230 = 2;
                    } else {
                        field_0x3c.z = 0.5f * field_0x238;
                        field_0x230 = 0;
                    }
                }
            }
            break;
        case 2: // resting; nothing to do (the original switch has an unreachable branch here)
            break;
        }
        box.set(field_0x80);
        box.addPos(p);
        s32 playerB = fn_8023B8E4(&box, field_0x4, &p, true);
        if (lbl_8053AAF8->field_0x7f) {
            player = fn_8023AF14(&box, field_0x4, false, 0.0f);
            if (player >= 0 && fn_80236B50(player)) {
                player = -1;
            }
            if (playerB >= 0 && fn_80236B50(playerB)) {
                playerB = -1;
            }
        } else {
            player = fn_8023AF14(&box, field_0x4, true, 0.0f);
        }
        if (player >= 0) {
            collect(player);
        } else if (playerB >= 0) {
            collect(playerB);
        }
        if (field_0x4 < 8) {
            JGeometry::TVec3f camPos(mPos.x, mPos.y - mPos.z, 0.0f);
            lbl_8053AB10->field_0x24->fn_8030C210(&camPos);
        }
    }
}

void MagicJar_804AF018::init() {
    mAlive = false;
}
