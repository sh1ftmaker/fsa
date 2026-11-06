#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_four_person_pull_switch_pswb.h"

static const f32 lbl_80474F98[5] = {
    0.0f, 0.0f, 5.0f, 8.0f, 11.0f,
};

static const char* lbl_804AE560[5] = {
    "4hippari_switch_0.bti",
    "4hippari_switch_1.bti",
    "4hippari_switch_2.bti",
    "4hippari_switch_3.bti",
    "4hippari_switch_4.bti",
};

static const s32 lbl_80475024[15] = {
    30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1,
};

void FourPersonPullSwitchBTIVersion_804AE574::init() {
    mBox.set(-32.0f, -8.0f, 32.0f, 8.0f);
    mBox2.set(-32.0f, -8.0f, 32.0f, 5.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0x23c = 0;
    field_0x240 = field_0x254 = vfunc_0x34();
    if (field_0x240) {
        field_0x244 = 14.0f;
    } else {
        field_0x244 = 0.0f;
    }
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x248 = 0;
    field_0x24c = -1;
    field_0x250 = 0;
}

void FourPersonPullSwitchBTIVersion_804AE574::update() {
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    if (field_0x248 > 0) {
        field_0x248--;
    }
    fn_80373198();

    JGeometry::TBox2f box(-32.0f, -8.0f, 32.0f, 8.0f);
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;

    s32 playerCount = 0;
    if (fn_80239470(&box, field_0x4, mSerial) >= 0) {
        playerCount = fn_80238A68(mSerial);
        if (playerCount > 4) {
            playerCount = 4;
        }
    }

    if (field_0x244 < 3.0f) {
        box = mBox;
    } else {
        box = mBox2;
    }
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y + lbl_80474F98[field_0x23c];
    box.f.y += mPos.y + lbl_80474F98[field_0x23c];

    if (playerCount == 4) {
        fn_80239914(&box, field_0x4, mSerial);
        if (field_0x240) {
            field_0x244 = 14.0f;
        } else if (field_0x244 >= 11.0f) {
            field_0x244 += 0.05f;
            if (field_0x244 >= 14.0f) {
                field_0x244 = 14.0f;
                if (!field_0x254) {
                    field_0x240 = 1;
                    startSound_801F0E34(0x3004B, 0);
                    field_0x254 = 1;
                    if (fn_8023077C()) {
                        field_0x248 = 50;
                        field_0x24c = 4;
                        field_0x250 = 0;
                        for (s32 i = 0; i < 4; i++) {
                            JGeometry::TVec3f offset(0.0f, 7.0f, 0.0f);
                            JGeometry::TVec3f pos = *getPlayerPos_8023E724(i) + offset;
                            fn_80230498(i, &pos, 0x2E2, 0xFF, -0x80, -0x80, true);
                        }
                    } else {
                        field_0x24c = 0;
                    }
                }
            }
        } else {
            if (field_0x244 <= 0.0f) {
                field_0x230 = mPos;
                field_0x230.y += 16.0f;
            }
            if (field_0x244 < 3.0f) {
                field_0x244 += 0.05f;
            } else {
                field_0x244 += 0.125f;
            }
        }
    } else if (field_0x240) {
        field_0x244 = 14.0f;
    } else {
        field_0x244 -= 0.25f;
        if (field_0x244 < 0.0f) {
            field_0x244 = 0.0f;
        }
    }

    if (field_0x244 < 3.0f) {
        field_0x23c = 0;
    } else if (field_0x244 < 8.0f) {
        field_0x23c = 1;
    } else if (field_0x244 < 11.0f) {
        field_0x23c = 2;
    } else if (field_0x244 < 14.0f) {
        field_0x23c = 3;
    } else {
        field_0x23c = 4;
    }
}

void FourPersonPullSwitchBTIVersion_804AE574::draw() {
    void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', lbl_804AE560[field_0x23c]);
    if (field_0x4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F745C(&mPos, res, NULL);
    }
}

void FourPersonPullSwitchBTIVersion_804AE574::fn_80373198() {
    if (field_0x24c < 0) {
        return;
    }
    if (field_0x24c < 4) {
        if (fn_8023077C()) {
            field_0x24c = 4;
            field_0x248 = 50;
            field_0x250 = 0;
            for (s32 i = 0; i < 4; i++) {
                JGeometry::TVec3f offset(0.0f, 7.0f, 0.0f);
                JGeometry::TVec3f pos = *getPlayerPos_8023E724(i) + offset;
                fn_80230498(i, &pos, 0x2E2, 0xFF, -0x80, -0x80, true);
            }
        }
    } else if (field_0x250 == 0 && field_0x248 > 30) {
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, getPlayerPos_8023E724(i), 0x2E2, 0xFF, -0x80, -0x80, true);
        }
    } else if (field_0x250 < 13) {
        if (field_0x248 == 0) {
            field_0x250++;
            field_0x248 = lbl_80475024[field_0x250];
            for (s32 i = 0; i < 4; i++) {
                fn_80230498(i, getPlayerPos_8023E724(i), field_0x250 + 0xDB, 0xFF, -0x80, -0x80, true);
                if (field_0x250 == 1) {
                    fn_80230FD0(i, 0x10001);
                }
            }
            if (field_0x250 == 13) {
                field_0x248 = 25;
            }
        }
    } else if (field_0x248 == 0) {
        if (fn_80230534(false)) {
            field_0x24c = -1;
        }
    } else if (field_0x248 == 15) {
        vfunc_0x30(0xFF, true);
    }
}

FourPersonPullSwitchBTIVersion_804AE574::~FourPersonPullSwitchBTIVersion_804AE574() {}
