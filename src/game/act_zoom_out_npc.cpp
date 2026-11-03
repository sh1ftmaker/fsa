#include "game/GameAudio.h"
#include "game/act/act_zoom_out_npc.h"

cls_804AA228::~cls_804AA228() {}

void cls_804AA228::init() {
    mDuration = field_0x90 & 0xFFF;
    mTimer = 0;
    mState = 0;
}

// Takes the scripted-camera slot (Camera_804AA9E4::field_0x7f4/0x7f5) when it is free.
static inline bool takeCamera(Camera_804AA9E4* cam) {
    bool got = false;
    if (cam->field_0x7f4 == 0) {
        got = true;
        cam->field_0x7f5 = 1;
    }
    return got;
}

void cls_804AA228::update() {
    switch (mState) {
    case 0:
        if (vfunc_0x34()) {
            takeCamera(lbl_8053AB10->field_0x24);
            if (takeCamera(lbl_8053AB10->field_0x24) == 1) {
                mState = 1;
                mTimer = mDuration;
            }
        }
        break;
    case 1:
        if (lbl_8053AB10->field_0x24->field_0x7f4 == 0) {
            lbl_8053AB10->field_0x24->field_0x7f5 = 1;
        }
        mTimer--;
        if (mTimer == 0) {
            mAlive = false;
        }
        break;
    }
}

void cls_804AA228::draw() {}
