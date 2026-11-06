#include "game/GameAudio.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_four_person_pull_switch_pswk.h"

static const f32 lbl_80475060[4] = {
    0.0f, 2.0f, 4.0f, 8.0f,
};

static const s32 lbl_80475070[15] = {
    30, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 1, 1,
};

// result = a + b.
static inline void sum(JGeometry::TVec3f& result, const JGeometry::TVec3f& a,
                       const JGeometry::TVec3f& b) {
    JGeometry::TVec3f tmp = a;
    tmp.add(b);
    result = tmp;
}

// FIXME: 99.70%. scheduling: epilogue restores r0 and r31 in swapped order.
void FourPersonPullSwitchChainVersion_804AE5C8::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    mLife = 0;
    field_0x23c = 0;
    field_0x240 = field_0x254 = vfunc_0x34();
    field_0x244 = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x248 = 0;
    field_0x24c = -1;
    field_0x250 = 0;
}

void FourPersonPullSwitchChainVersion_804AE5C8::update() {
    f32 prev244 = field_0x244;
    f32 rem = field_0x244 - 10.0f * (s32)(field_0x244 / 10.0f);
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    if (field_0x248 > 0) {
        field_0x248--;
    }
    fn_80373C38();

    JGeometry::TBox2f box = mBox;
    JGeometry::TVec3f playerPos;
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;

    s32 i;
    s32 playerCount = 0;
    s32 player = fn_80239470(&box, field_0x4, mSerial);
    if (player >= 0) {
        playerCount = fn_80238A68(mSerial);
    }

    if (playerCount >= 4) {
        f32 dx = mPos.x - getPlayerPos_8023E724(player)->x;
        if (__fabsf(dx) >= 2.0f) {
            JGeometry::TVec3f push(dx, 0.0f, 0.0f);
            fn_8023DFF4(player, &push);
        }
        fn_80239914(&box, field_0x4, mSerial);
        if (field_0x244 >= 36.0f) {
            field_0x244 += 0.5f;
            if (field_0x244 >= 40.0f) {
                field_0x244 = 40.0f;
                if (!field_0x254) {
                    field_0x240 = 1;
                    startSound_801F0E34(0x3004B, 0);
                    mPos.y = mSpawnPos.y + field_0x244;
                    field_0x254 = 1;
                    if (fn_8023077C()) {
                        field_0x248 = 50;
                        field_0x24c = 4;
                        field_0x250 = 0;
                        for (i = 0; i < 4; i++) {
                            playerPos = *getPlayerPos_8023E724(i);
                            JGeometry::TVec3f pos;
                            sum(pos, playerPos, JGeometry::TVec3f(0.0f, 7.0f, 0.0f));
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
            if (rem < 2.0f) {
                field_0x244 += 0.03f;
                if (rem >= 1.97f) {
                    startSound_801F0E34(0x5007A, 0);
                }
            } else {
                field_0x244 += 0.5f;
            }
        }
    } else if (field_0x240 && field_0x248 > 0) {
        field_0x244 = 40.0f;
    } else {
        field_0x244 -= 1.25f;
        if (field_0x244 < 0.0f) {
            field_0x244 = 0.0f;
        } else {
            startSound_801F0E34(0x5007B, 0);
        }
    }

    field_0x3c.set(0.0f, field_0x244 - prev244, 0.0f);
    mPos.y = mSpawnPos.y + field_0x244;

    if (field_0x244 < 10.0f) {
        field_0x23c = 0;
    } else if (field_0x244 < 20.0f) {
        field_0x23c = 4;
    } else if (field_0x244 < 30.0f) {
        field_0x23c = 5;
    } else if (field_0x244 < 40.0f) {
        field_0x23c = 6;
    } else {
        field_0x23c = 7;
    }
}

void FourPersonPullSwitchChainVersion_804AE5C8::draw() {
    JGeometry::TVec3f savedPos = mPos;
    f32 rem = field_0x244 - 10.0f * (s32)(field_0x244 / 10.0f);
    s32 step;
    if (rem < 1.0f) {
        step = 0;
    } else if (rem < 3.0f) {
        step = 1;
    } else if (rem < 6.0f) {
        step = 2;
    } else {
        step = 3;
    }

    mPos.y -= field_0x244;
    mPos.y += lbl_80475060[step];
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23c + 0x469, NULL, -1, 1.0f, 1.0f);

    if (step == 0) {
        mPos = savedPos;
    } else {
        mPos = savedPos - JGeometry::TVec3f(0.0f, field_0x244, 0.0f);
        drawSprite_801F06F0("enemy_gc_sob.bin", step + 0x469, NULL, -1, 1.0f, 1.0f);
        mPos = savedPos;
    }
}

void FourPersonPullSwitchChainVersion_804AE5C8::fn_80373C38() {
    s32 i;
    JGeometry::TVec3f playerPos;
    JGeometry::TVec3f pos2;
    JGeometry::TVec3f pos3;
    JGeometry::TVec3f pos1;
    if (field_0x24c < 0) {
        return;
    }
    if (field_0x24c < 4) {
        if (fn_8023077C()) {
            field_0x24c = 4;
            field_0x248 = 50;
            field_0x250 = 0;
            for (i = 0; i < 4; i++) {
                playerPos = *getPlayerPos_8023E724(i);
                sum(pos1, playerPos, JGeometry::TVec3f(0.0f, 7.0f, 0.0f));
                fn_80230498(i, &pos1, 0x2E2, 0xFF, -0x80, -0x80, true);
            }
        }
    } else if (field_0x250 == 0 && field_0x248 > 30) {
        for (i = 0; i < 4; i++) {
            pos2 = *getPlayerPos_8023E724(i);
            fn_80230498(i, &pos2, 0x2E2, 0xFF, -0x80, -0x80, true);
        }
    } else if (field_0x250 < 13) {
        if (field_0x248 == 0) {
            field_0x250++;
            field_0x248 = lbl_80475070[field_0x250];
            for (i = 0; i < 4; i++) {
                pos3 = *getPlayerPos_8023E724(i);
                fn_80230498(i, &pos3, field_0x250 + 0xDB, 0xFF, -0x80, -0x80, true);
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
            vfunc_0x30(0xFF, true);
        }
    }
}

FourPersonPullSwitchChainVersion_804AE5C8::~FourPersonPullSwitchChainVersion_804AE5C8() {}
