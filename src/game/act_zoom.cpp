#include "game/GameAudio.h"
#include "game/act/act_zoom.h"
#include "game/Camera.h"
#include "game/GameGlobals.h"
#include "game/Player.h"

static const u32 lbl_8046F260[4] = {0x3E, 0x3E, 0x3E, 0x3E};

Zoom_804A9DF8::~Zoom_804A9DF8() {}

void Zoom_804A9DF8::draw() {}

// Takes the scripted-camera slot (Camera_804AA9E4::field_0x7f4/0x7f5) when it is free.
static inline bool takeCamera(Camera_804AA9E4* cam) {
    bool got = false;
    if (cam->field_0x7f4 == 0) {
        got = true;
        cam->field_0x7f5 = 1;
    }
    return got;
}

// FIXME: 92%. Scheduling/regalloc only, same shape as act_attack_hitbox_when_player_is_thrown:
// the original loads mPos.y then mPos.x (f6, f2) before copying mBox to the stack and then adds
// into f4/f3/f2/f0; ours loads mBox first and mPos.x/y after the raw copy.
void Zoom_804A9DF8::update() {
    switch (mState) {
    case 0: {
        JGeometry::TBox2f box(mBox);
        box.addPos(mPos.x, mPos.y);
        if (fn_8023AF14(&box, field_0x4, false, 0.0f) >= 0) {
            if (takeCamera(lbl_8053AB10->field_0x24) == 1) {
                if (field_0x90 >> 27) {
                    vfunc_0x30(0xFF, true);
                }
                mState = 1;
                mTimer = mDuration;
                if (field_0x234 != 0) {
                    fn_801F0D20(lbl_8046F260[field_0x234]);
                }
            }
        }
        break;
    }
    case 1:
        if (field_0x238 == 0) {
            if (fn_8023077C() == 1) {
                mState = 2;
                fn_801F0D20(0x3C);
            }
        } else {
            mState = 2;
            fn_801F0D20(0x3C);
        }
        break;
    case 2:
        takeCamera(lbl_8053AB10->field_0x24);
        if (mTimer == 0) {
            mState = 3;
        } else {
            mTimer--;
        }
        break;
    case 3:
        takeCamera(lbl_8053AB10->field_0x24);
        if (field_0x238 == 0) {
            if (fn_80230534(false) == 1) {
                fn_801F3668();
            }
        } else {
            fn_801F3668();
        }
        break;
    }
}

void Zoom_804A9DF8::init() {
    field_0x234 = (field_0x90 >> 23) & 3;
    field_0x238 = (field_0x90 >> 22) & 1;
    mDuration = field_0x90 & 0xFFF;
    mHalfWidth = ((field_0x90 >> 17) & 0x1F) + 1;
    mHalfHeight = ((field_0x90 >> 12) & 0x1F) + 1;
    mTimer = 0;
    mPos.x += 8.0f;
    mPos.y += 8.0f;
    f32 h = 16.0f * mHalfHeight;
    f32 w = 16.0f * mHalfWidth;
    mBox.i.x = -w;
    mBox.i.y = -h;
    mBox.f.x = w;
    mBox.f.y = h;
    mState = 0;
}
