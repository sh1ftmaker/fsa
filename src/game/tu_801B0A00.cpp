#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/ActorManager.h"
#include <math.h>

// Sprite frames of the third step, .data at 0x804A35A0.
static const s32 lbl_804A35A0[3] = {2, 1, 0};

// 0x801B2394.
// FIXME(shotgun): 99.89 %, reloc-name; behaviour clean.
void cls_804A35AC::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x250 = 0;
    field_0x254 = -1;
    gAudioMgr_8053AF80->fn_80458F9C(0x3c);
    field_0x98 = 2;
    fn_801B2138(field_0x250);
    field_0x264[0] = mPos.x;
    field_0x264[1] = mPos.y;
    field_0x264[2] = mPos.z;
    field_0x264[3] = mPos.x;
    field_0x264[4] = mPos.y;
    field_0x264[5] = mPos.z;
    field_0x264[6] = mPos.x;
    field_0x264[7] = mPos.y;
    field_0x264[8] = mPos.z;
    field_0x258 = 0;
    field_0x259 = 0;
    field_0x288[0].x = mPos.x;
    field_0x288[0].y = mPos.y;
    field_0x288[0].z = mPos.z;
    field_0x288[1].x = mPos.x;
    field_0x288[1].y = mPos.y;
    field_0x288[1].z = mPos.z;
    field_0x288[2].x = mPos.x;
    field_0x288[2].y = mPos.y;
    field_0x288[2].z = mPos.z;
    field_0x288[3].x = mPos.x;
    field_0x288[3].y = mPos.y;
    field_0x288[3].z = mPos.z;
    mEmitters[0] = NULL;
    mEmitters[1] = NULL;
    field_0x25c = 0.0f;
    field_0x260 = 0.0f;
    getGameView_802D6A9C()->fn_802D7440(false);
    s32 fade = 200;
    switch (field_0x90) {
    case 0:
        if (!getGameView_802D6A9C()->fn_802D800C(0x10)) {
            field_0x230 = 0;
        } else {
            field_0x90 = 2;
            getGameView_802D6A9C()->fn_802D8050(1, true, 0xFF);
            field_0x230 = 0x14;
        }
        break;
    case 1:
        field_0x230 = 15;
        fade = 100;
        break;
    }
    if (mAlive == true && field_0x90 != 2) {
        fn_80119DD0()->fn_80119998(fade, 0, 0, 0, 0, 0, 0);
    }
}

// Destructor at 0x801B22F4.
cls_804A35AC::~cls_804A35AC() {
    for (s32 i = 0; i < 2; i++) {
        releaseEmitter_801EE434(mEmitters[i]);
    }
    getGameView_802D6A9C()->fn_802D7440(true);
}

// 0x801B2138.
// FIXME(shotgun): 96.26 %, missing/extra + reloc-name; behaviour clean.
void cls_804A35AC::fn_801B2138(s32 arg) {
    if (field_0x254 != arg) {
        field_0x254 = arg;
        field_0x259 = 0;
        field_0x234 = 0;
        field_0x240 = 0;
    }
    if (field_0x234 != 0) {
        return;
    }
    switch (field_0x250) {
    case 0:
        field_0x24c = field_0x98 << 3;
    // fall through
    case 1:
        field_0x24c = (field_0x98 << 3) + field_0x240;
        field_0x234 = 5;
        field_0x240++;
        field_0x240 &= 7;
    // fall through
    case 2:
        field_0x24c = 2;
    // fall through
    case 3:
        field_0x24c = lbl_804A35A0[field_0x240];
        field_0x234 = 8;
        if (field_0x240 == 0) {
            field_0x234 = 40;
        }
        field_0x240++;
        if (field_0x240 <= 2) {
            return;
        }
        field_0x240 = 0;
        field_0x250 = 4;
        field_0x254 = field_0x250;
    // fall through
    case 4:
        switch (field_0x98) {
        case 0:
            field_0x98 = 2;
            break;
        case 1:
            field_0x98 = 3;
            break;
        case 2:
            field_0x98 = 1;
            break;
        case 3:
            field_0x98 = 0;
            break;
        }
        field_0x240++;
        field_0x234 = 5;
        if (field_0x240 >= 2) {
            field_0x240 = 0;
            field_0x259 = 1;
            field_0x250 = 0;
            field_0x254 = field_0x250;
        }
        field_0x24c = field_0x98 << 3;
        break;
    }
}

// 0x801B2000.
// FIXME: 82.28 %, missing/extra + reloc-name; behaviour drift.
// FIXME(shotgun): operands differ: fn_802303FC's original call passes 0xFF for its bool flag; register choice for mPos.y differs.
void cls_804A35AC::fn_801B2000(f32 yOffset, s32 mode) {
    f32 y = mPos.y - yOffset;
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023F264(i)) {
            if (field_0x4 == getPlayerLayer_8023DE58(i)) {
                const JGeometry::TVec3f* pos = getPlayerPos_8023E724(i);
                field_0x288[i].x = pos->x;
                field_0x288[i].y = pos->y;
                field_0x288[i].z = pos->z;
                if (mode == 0) {
                    if (field_0x258 == 1) {
                        field_0x288[i].y = y;
                    }
                    fn_80230498(i, &field_0x288[i], 0x3E, 0xFF, -128, -128, true);
                } else {
                    addCalc2_801F71A4(&field_0x288[i].y, y, 0.3f, 1.0f);
                    fn_80230BF4(i, 2);
                    fn_802303FC(i, &field_0x288[i], 0xFF);
                    if (fabs(getPlayerPos_8023E724(i)->y - y) < 1.0f) {
                        field_0x258 = 1;
                    }
                }
            }
        }
    }
}

// 0x801B0A00.
// FIXME(shotgun): 97.31 %, different + reloc-name; behaviour drift.
// field_0x90 compare is cmplwi (u32), original cmpwi
void cls_804A35AC::draw() {
    s32 frame = 0x9B;
    if (field_0x90 == 0) {
        if (field_0x250 == 2 || field_0x250 == 3) {
            frame = 0x274;
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", frame + field_0x24c, NULL, 7, 1.0f, 1.0f);
    }
}
