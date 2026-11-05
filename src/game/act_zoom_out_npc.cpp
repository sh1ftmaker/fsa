#include "game/GameAudio.h"
#include "game/act/act_zoom_out_npc.h"

ZoomOutNPC_804AA228::~ZoomOutNPC_804AA228() {}

void ZoomOutNPC_804AA228::draw() {}

// Takes the scripted-camera slot (Camera_804AA9E4::field_0x7f4/0x7f5) when it is free.
static inline bool takeCamera(Camera_804AA9E4* cam) {
    bool got = false;
    if (cam->field_0x7f4 == 0) {
        got = true;
        cam->field_0x7f5 = 1;
    }
    return got;
}

void ZoomOutNPC_804AA228::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34()) {
            // The original makes the request twice (0x802EC7E8, 0x802EC804); only the second result is used.
            takeCamera(lbl_8053AB10->field_0x24);
            if (takeCamera(lbl_8053AB10->field_0x24) == 1) {
                mState = 1;
                mTimer = mDuration;
            }
        }
        break;
    case 1:
        takeCamera(lbl_8053AB10->field_0x24);
        mTimer--;
        if (mTimer == 0) {
            mAlive = false;
        }
        break;
    }
}

void ZoomOutNPC_804AA228::init() {
    mDuration = field_0x90 & 0xFFF;
    mTimer = 0;
    mState = 0;
}
