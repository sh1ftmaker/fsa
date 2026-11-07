#include "game/GameAudio.h"
#include "game/act/act_thwomp_launch_npc_2.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/Message.h"
#include "JSystem/JMath/JMath.h"

extern s32 lbl_804ABB40[];
extern s32 lbl_804ABB50[];

s32 lbl_804ACDC8[4][4] = {
    { 0, 1 },
    { 2, 3 },
    { 4, 5 },
    { 6, 7 }
};

s32 lbl_804ACE08[4][4] = {
    { 8, 9 },
    { 10, 11 }
};

s32 lbl_804ACE48[4][4] = {
    { 8, 9, 8, 10 },
    { 11, 12, 11, 13 }
};

s32 lbl_804ACE88[4] = { 24, 24 };
s32 lbl_804ACE98[4] = { 20, 20, 20, 20 };
s32 lbl_804ACEA8[4] = { 24, 24 };
s32 lbl_804ACEB8[4] = { 24, 24 };
s32 lbl_804ACEC8[4] = { 16, 16 };
s32 lbl_804ACED8[4] = { 12, 12, 12, 12 };

s32 lbl_804ACEE8[4][4] = {
    { 3, 10 },
    { 2, 11 },
    { 0, 1 },
    { 4, 5 }
};

s32 lbl_804ACF28[4][4] = {
    { 0, 6, 0, 7 },
    { 4, 8, 4, 9 }
};

s32 lbl_804ACF68[4] = { 20, 20 };
s32 lbl_804ACF78[4] = { 20, 20, 20, 20 };

s32 lbl_804ACF88[4][4] = { { 0, 1 } };
s32 lbl_804ACFC8[4][4] = { { 0, 2, 0, 3 } };

s32 lbl_804AD008[4] = { 16, 16 };
s32 lbl_804AD018[4] = { 24, 16, 16, 16 };

s32 lbl_804AD028[4][4] = { { 0, 4 } };
s32 lbl_804AD068[4][4] = { { 0, 1, 2, 3 } };

s32 lbl_804AD0A8[4] = { 10, 10 };
s32 lbl_804AD0B8[4] = { 10, 10, 10, 10 };

s32 lbl_804AD0C8[4][4] = { { 2, 5 } };
s32 lbl_804AD108[4][4] = { { 2, 3, 2, 4 } };

s32 lbl_804AD148[4][4] = { { 0, 1 } };
s32 lbl_804AD188[4][4] = { { 0, 6 } };

struct NPCLaunchAnimInfo {
    const s32* animDelays0;
    const s32* animDelays1;
    const s32 (*spriteFrames0)[4];
    const s32 (*spriteFrames1)[4];
};

struct NPCLaunchSpriteInfo {
    s32 sobType;
    s32 baseSpriteId;
};

struct NPCLaunchTable {
    NPCLaunchAnimInfo animInfo[11];
    NPCLaunchSpriteInfo spriteInfo[11];
    char enemyBothSob[20];
    char enemyGcSob[20];
};

static const NPCLaunchTable lbl_80472390 = {
    {
        { lbl_804ACE88, lbl_804ACE98, lbl_804ACDC8, lbl_804ACE48 },
        { lbl_804ABB40, lbl_804ABB50, lbl_804ACDC8, lbl_804ACE48 },
        { lbl_804ABB40, lbl_804ABB50, lbl_804ACDC8, lbl_804ACE48 },
        { lbl_804ACEB8, lbl_804ACEB8, lbl_804ACDC8, lbl_804ACE08 },
        { lbl_804ACEA8, lbl_804ACEB8, lbl_804ACDC8, lbl_804ACE08 },
        { lbl_804ACEC8, lbl_804ACED8, lbl_804ACEE8, lbl_804ACF28 },
        { lbl_804ABB40, lbl_804ABB50, lbl_804ACF88, lbl_804ACFC8 },
        { lbl_804ACF68, lbl_804ACF78, lbl_804ACF88, lbl_804ACFC8 },
        { lbl_804AD008, lbl_804AD018, lbl_804AD028, lbl_804AD068 },
        { lbl_804AD0A8, lbl_804AD0B8, lbl_804AD0C8, lbl_804AD108 },
        { lbl_804ACEA8, lbl_804ACEB8, lbl_804AD148, lbl_804AD188 },
    },
    {
        { 0, 0x516 },
        { 0, 0x4E2 },
        { 0, 0x4F0 },
        { 0, 0x4FE },
        { 0, 0x50A },
        { 1, 0x13A },
        { 0, 0x540 },
        { 0, 0x537 },
        { 0, 0x53B },
        { 1, 0x14C },
        { 0, 0x530 },
    },
    "enemy_both_sob.bin",
    "enemy_gc_sob.bin"
};

// FIXME: 94.43% scheduling+regalloc + reloc-name. Multiplier 0x19660d hoisted into r4 vs r6, and literal relocations.
void NPCLaunchedByThwomp2_804AD1C8::init() {
    field_0x230 = 0;
    field_0xb0 = 0;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mLife = 10000;
    field_0x23c = 0;
    field_0x26c = mSpawnPos;

    mPos.x += (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 7.0f) - 3;
    mPos.y += (s32)(gGameRoot_8053AAF8->mRandom.get_ufloat_1() * 7.0f) - 3;
    mSpawnPos = mPos;

    field_0x238 = 0;
    field_0x234 = 0;
    field_0x244 = 0;
    field_0x248 = 3;

    field_0x258 = (field_0x90 >> 22) & 7;
    if ((field_0x90 >> 25) & 1) {
        field_0x258 = -field_0x258;
    }
    field_0x278.x = mSpawnPos.x + field_0x258 * 16.0f;

    field_0x25c = (field_0x90 >> 18) & 7;
    if ((field_0x90 >> 21) & 1) {
        field_0x25c = -field_0x25c;
    }
    field_0x278.y = mSpawnPos.y + field_0x25c * 16.0f;
    field_0x278.z = 0.0f;

    field_0x24c = -1;
    field_0x264 = (field_0x90 & 0xFFF) + 0x12b;
    field_0x250 = (field_0x90 >> 12) & 0xF;
    field_0x240 = 0;
    field_0x260 = 0.0f;
    field_0x254 = 1;

    field_0x268 = (field_0x90 >> 26) & 1;
    if (field_0x268 == 1) {
        field_0x230 = 2;
        field_0x254 = 0;
        if (!fn_801F6758(&field_0x278, field_0x4, 0.0f)) {
            field_0x230 = 1;
            mPos = field_0x278;
        }
    }

    fn_8035680C();

    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);

    field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays0[0];
    field_0x194 = 1;
}

// FIXME: 91.07% missing/extra. Stack frame size (0x80 vs 0x70) and float temporary layout in inlined launch(), plus switch dispatch ordering in updateAnim().
void NPCLaunchedByThwomp2_804AD1C8::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x -= 2.0f;
    box.f.x += 2.0f;
    if (field_0x254 == 1 && (field_0x230 == 0 || field_0x230 == 4 || field_0x230 == 1)) {
        box.addPos(field_0x26c.x, field_0x26c.y);
    } else {
        box.addPos(mPos.x, mPos.y);
    }

    if (field_0x230 != 2) {
        fn_80239914(&box, field_0x4, mSerial);
    }

    mPos += field_0x3c;

    switch (field_0x230) {
    case 0:
        if (field_0x268 == 0 && fn_8023A30C()) {
            launch();
            break;
        }
        fn_8022FEB8(&box, field_0x4, mSerial, 1, 0);
        field_0x24c = fn_8022FD8C(mSerial);
        if (field_0x24c >= 0) {
            if (getPlayerPos_8023E724(field_0x24c)->z == 0.0f) {
                if (fn_802300EC(field_0x24c)) {
                    if (fn_8038C678(field_0x24c, field_0x264, mSerial, field_0x4, mPos, 0)) {
                        field_0x230 = 4;
                    }
                }
            }
        }
        break;

    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, 1, 0);
        field_0x24c = fn_8022FD8C(mSerial);
        if (field_0x24c >= 0) {
            if (getPlayerPos_8023E724(field_0x24c)->z != 0.0f) {
                break;
            }
            if (!fn_802300EC(field_0x24c)) {
                break;
            }
            if (fn_8038C678(field_0x24c, field_0x264, mSerial, field_0x4, mPos, 0)) {
                field_0x230 = 4;
                break;
            }
        }
        // Fallthrough to case 2

    case 2:
        if (vfunc_0x34() == 1 && !getGameView_802D6A9C()->fn_802DCD0C()) {
            field_0x254 = 1;
            field_0x23c = ((field_0x90 >> 16) & 3) * 70 + 30;
            mPos = field_0x278;
            field_0x3c = JGeometry::TVec3f(
                (mSpawnPos.x - mPos.x) / (f32)field_0x23c,
                (mSpawnPos.y - mPos.y) / (f32)field_0x23c,
                0.0f
            );
            field_0x240 = field_0x23c;
            field_0x230 = 5;
            field_0x238 = 0;
            field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays1[0];
        }
        break;

    case 3:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= field_0x260;
        } else if (field_0x3c.z < 0.0f) {
            mPos.z = 0.0f;
            field_0x23c = ((field_0x90 >> 16) & 3) * 70 + 30;
            mPos = field_0x278;
            field_0x3c = JGeometry::TVec3f(
                (mSpawnPos.x - mPos.x) / (f32)field_0x23c,
                (mSpawnPos.y - mPos.y) / (f32)field_0x23c,
                0.0f
            );
            field_0x240 = field_0x23c;
            field_0x230 = 5;
            field_0x238 = 0;
            field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays1[0];
        }
        break;

    case 5:
        if (field_0x268 == 0 && fn_8023A30C()) {
            launch();
            field_0x240 = 0;
            field_0x238 = 0;
            field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays1[0];
        } else if (field_0x240 <= 0 || (mPos.x == mSpawnPos.x && mPos.y == mSpawnPos.y)) {
            mPos = mSpawnPos;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            field_0x230 = 0;
            field_0x238 = 0;
            field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays0[0];
        }
        break;

    case 4:
        fn_8022FEB8(&box, field_0x4, mSerial, 1, 0);
        if (fn_80388CAC(field_0x24c, mSerial)) {
            if (field_0x254 == 1) {
                field_0x230 = 0;
            } else {
                field_0x230 = 1;
            }
        }
        break;
    }

    fn_8035680C();

    updateAnim();

    if (field_0x240 > 0) {
        field_0x240--;
    }
}

// Plain function whose out-of-line copy the linker dropped (inlined into update).
// Compiled between draw (unwritten, see below) and update, it pools 40.0f / 4.0f / 0.2f before update's 2.0f
// as in the original (docs/RULES_CARD.md (3)).
void NPCLaunchedByThwomp2_804AD1C8::launch() {
    f32 dy = field_0x278.y - mPos.y;
    f32 dx = field_0x278.x - mPos.x;
    field_0x3c.x = dx / 40.0f;
    field_0x3c.y = dy / 40.0f;
    field_0x3c.z = 4.0f;
    field_0x260 = 0.2f;
    field_0x230 = 3;
}

// draw (0x80356A24) is unwritten: best attempt was 79.24 % missing/extra (try.py STOP), below the merge bar.
void NPCLaunchedByThwomp2_804AD1C8::fn_8035680C() {
    switch (field_0x250) {
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        field_0x248 = 0;
        return;
    case 1:
    case 3:
        if (field_0x268 == 1 && getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 1) {
            field_0x248 = 0;
            return;
        }
        break;
    }

    switch (field_0x230) {
    case 0:
    case 1:
    case 4: {
        if (field_0x244 != 0) {
            field_0x244--;
            return;
        }

        field_0x244 = 20;
        JGeometry::TVec3f diff;
        f32 dist;
        fn_801EE558(&diff, &dist, 0);

        if (dist < 100.0f) {
            u16 angle = (s32)(JMath::atanTable_.atan2_(-diff.x, -diff.y) * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
            if (angle < 0x2000 || angle >= 0xE000) {
                field_0x248 = 3;
            } else if (angle >= 0x2000 && angle < 0x6000) {
                field_0x248 = 1;
            } else if (angle >= 0x6000 && angle <= 0xA000) {
                field_0x248 = 2;
            } else {
                field_0x248 = 0;
            }
        } else {
            field_0x248 = 3;
        }
        break;
    }

    case 3:
    case 5:
        if (field_0x250 == 5) {
            if (field_0x25c > 0) {
                field_0x248 = 0;
            } else {
                field_0x248 = 1;
            }
        } else {
            if (field_0x258 > 0) {
                field_0x248 = 0;
            } else {
                field_0x248 = 1;
            }
        }
        if (field_0x230 == 5) {
            field_0x248 ^= 1;
        }
        break;
    }
}

inline void NPCLaunchedByThwomp2_804AD1C8::updateAnim() {
    field_0x234--;
    if (field_0x234 <= 0) {
        field_0x238++;
        if (field_0x250 >= 0 && field_0x250 < 3) {
            if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
                field_0x238 &= 1;
                field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays0[field_0x238];
            } else if (field_0x230 == 5) {
                field_0x238 &= 3;
                field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays1[field_0x238];
            }
        } else if ((field_0x250 >= 3 && field_0x250 < 5) || field_0x250 == 10) {
            if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
                field_0x238 &= 1;
                field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays0[field_0x238];
            } else if (field_0x230 == 5) {
                field_0x238 &= 1;
                field_0x234 = lbl_80472390.animInfo[field_0x250].animDelays1[field_0x238];
            }
        }

        if (field_0x238 == 0) {
            addRandomDelay();
        }
    }
}

NPCLaunchedByThwomp2_804AD1C8::~NPCLaunchedByThwomp2_804AD1C8() {}

// Plain function whose out-of-line copy the linker dropped (inlined into updateAnim/update).
// Compiled first (the bottom of the file), it gives the .sdata2 pool its leading 3.0f and 1.0f
// ahead of fn_8035680C's 100.0f / 10430.378f, as in the original (docs/RULES_CARD.md (3)).
void NPCLaunchedByThwomp2_804AD1C8::addRandomDelay() {
    field_0x234 += (s32)(3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
}
