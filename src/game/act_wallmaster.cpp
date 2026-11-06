#include "game/GameAudio.h"
#include "game/Player.h"
#include "game/act/act_wallmaster.h"

void WallmasterParam::fn_801BCD70(s32 arg) {
    mType = (arg >> 5) & 7;
    switch (mType) {
    case 0:
        mOrder[0] = 3;
        mOrder[1] = 0;
        mOrder[2] = 2;
        break;
    case 1:
        mOrder[0] = 3;
        mOrder[1] = 1;
        mOrder[2] = 2;
        break;
    case 2:
        mOrder[0] = 1;
        mOrder[1] = 3;
        mOrder[2] = 0;
        break;
    case 3:
        mOrder[0] = 1;
        mOrder[1] = 2;
        mOrder[2] = 0;
        break;
    case 4:
        mOrder[0] = 2;
        mOrder[1] = 0;
        mOrder[2] = 3;
        break;
    case 5:
        mOrder[0] = 2;
        mOrder[1] = 1;
        mOrder[2] = 3;
        break;
    case 6:
        mOrder[0] = 0;
        mOrder[1] = 3;
        mOrder[2] = 1;
        break;
    case 7:
        mOrder[0] = 0;
        mOrder[1] = 2;
        mOrder[2] = 1;
        break;
    }

    switch (mType) {
    case 0:
    case 1:
    case 4:
    case 5:
        field_0x14 = 480;
        break;
    default:
        field_0x14 = 240;
        break;
    }

    field_0x18 = 40;
    field_0x10 = 40;
    mIndex = 0;
    mTarget = mOrder[mIndex];
    field_0x24 = 0;
}

cls_804A3E44::cls_804A3E44() {
    field_0x2a8 = 0;
    field_0x2ac = 0;
    field_0x2b0.set(0.0f, 0.0f);
    field_0x2b8 = -1;
    field_0x2bc = 0;
    field_0x2c0 = 0;
    field_0x2c4 = -1;
    mParam.mType = 8;
    mParam.mIndex = 0;
    mParam.mTarget = 2;
    mParam.field_0x24 = 0;
}

cls_804A3E44::~cls_804A3E44() {
    if (mFlags & 0x8000) {
        if (field_0x2c4 >= 0) {
            fn_8022DD74(field_0x2c4);
        }
    }
}
