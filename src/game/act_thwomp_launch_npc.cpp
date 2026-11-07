#include "JSystem/JMath/JMath.h"
#include "game/ActorManager.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/act_thwomp_launch_npc.h"

s32 lbl_804ABB40[4] = {15, 15};
s32 lbl_804ABB50[4] = {10, 7, 10, 7};
s32 lbl_804ABB60[4][4] = {
    {0, 1},
    {4, 5},
    {8, 9},
    {12, 13},
};
s32 lbl_804ABBA0[4][4] = {
    {2, 1, 3, 1},
    {6, 5, 7, 5},
    {10, 9, 11, 9},
    {14, 13, 15, 13},
};
s32 lbl_804ABBE0[4] = {20, 20};
s32 lbl_804ABBF0[4] = {10, 8, 10, 8};
s32 lbl_804ABC00[4][4] = {
    {0, 1},
    {4, 5},
    {8, 9},
    {12, 13},
};
s32 lbl_804ABC40[4][4] = {
    {2, 1, 3, 1},
    {6, 5, 7, 5},
    {10, 9, 11, 9},
    {14, 13, 15, 13},
};
s32 lbl_804ABC80[4] = {12, 10, 12, 10};
s32 lbl_804ABC90[4] = {12, 10, 12, 10};
s32 lbl_804ABCA0[4][4] = {
    {0, 1, 2},
    {6, 7, 8, 6},
    {12, 13, 14, 12},
    {17, 19, 18, 17},
};
s32 lbl_804ABCE0[4][4] = {
    {3, 4, 5, 4},
    {9, 10, 11, 10},
    {15, 14, 16, 14},
    {20, 18, 21, 18},
};
s32 lbl_804ABD20[4] = {20, 20};
s32 lbl_804ABD30[4] = {20, 10, 20, 10};
s32 lbl_804ABD40[4][4] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7},
};
s32 lbl_804ABD80[4][4] = {
    {8, 9},
    {10, 11},
    {12, 13, 14, 13},
    {15, 16, 17, 16},
};
s32 lbl_804ABDC0[4] = {10, 8, 10, 8};
s32 lbl_804ABDD0[4][4] = {
    {0, 1, 0, 2},
    {3, 4, 3, 5},
    {6, 7, 6, 8},
    {9, 10, 9, 11},
};
s32 lbl_804ABE10[4] = {16, 16};
s32 lbl_804ABE20[4] = {10, 10, 10, 10};
s32 lbl_804ABE30[4][4] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7},
};
s32 lbl_804ABE70[4][4] = {
    {8, 9, 8, 10},
    {11, 12, 11, 13},
};
s32 lbl_804ABEB0[4] = {16, 16};
s32 lbl_804ABEC0[4] = {10, 10};
s32 lbl_804ABED0[4][4] = {
    {0, 1},
    {2, 3},
    {4, 5},
    {6, 7},
};
s32 lbl_804ABF10[4][4] = {
    {8, 9},
    {10, 11},
};
s32 lbl_804ABF50[4] = {24, 24};
s32 lbl_804ABF60[4] = {26, 26};
s32 lbl_804ABF70[4] = {20, 20};
s32 lbl_804ABF80[4] = {20, 20, 20, 20};
s32 lbl_804ABF90[4][4] = {
    {0, 1},
};
s32 lbl_804ABFD0[4][4] = {
    {0, 2, 0, 3},
};

struct ThwompLaunchNPCAnim_80471BB8 {
    s32* field_0x0;
    s32* field_0x4;
    s32 (*field_0x8)[4];
    s32 (*field_0xc)[4];
};

static const ThwompLaunchNPCAnim_80471BB8 lbl_80471BB8[16] = {
    {lbl_804ABB40, lbl_804ABB50, lbl_804ABB60, lbl_804ABBA0},
    {lbl_804ABBE0, lbl_804ABBF0, lbl_804ABC00, lbl_804ABC40},
    {lbl_804ABC80, lbl_804ABC90, lbl_804ABCA0, lbl_804ABCE0},
    {lbl_804ABDC0, lbl_804ABDC0, lbl_804ABDD0, lbl_804ABDD0},
    {lbl_804ABD20, lbl_804ABD30, lbl_804ABD40, lbl_804ABD80},
    {lbl_804ABF70, lbl_804ABF80, lbl_804ABF90, lbl_804ABFD0},
    {lbl_804ABF70, lbl_804ABF80, lbl_804ABF90, lbl_804ABFD0},
    {lbl_804ABE10, lbl_804ABE20, lbl_804ABE30, lbl_804ABE70},
    {lbl_804ABEB0, lbl_804ABEC0, lbl_804ABED0, lbl_804ABF10},
    {lbl_804ABEB0, lbl_804ABEC0, lbl_804ABED0, lbl_804ABF10},
    {lbl_804ABEB0, lbl_804ABEC0, lbl_804ABED0, lbl_804ABE70},
    {lbl_804ABEB0, lbl_804ABEC0, lbl_804ABED0, lbl_804ABE70},
    {lbl_804ABEB0, lbl_804ABEC0, lbl_804ABED0, lbl_804ABE70},
    {lbl_804ABEB0, lbl_804ABE20, lbl_804ABED0, lbl_804ABE70},
    {lbl_804ABF70, lbl_804ABF80, lbl_804ABF90, lbl_804ABFD0},
    {lbl_804ABF50, lbl_804ABF60, lbl_804ABED0, lbl_804ABF10},
};

struct ThwompLaunchNPCSprite_80471CB8 {
    s32 field_0x0;
    s32 field_0x4;
};

static const ThwompLaunchNPCSprite_80471CB8 lbl_80471CB8[16] = {
    {2, 0x3A4}, {2, 0x394}, {2, 0x3B4}, {2, 0x388},
    {0, 0x432}, {0, 0x478}, {0, 0x47C}, {0, 0x4A6},
    {0, 0x480}, {0, 0x48C}, {0, 0x498}, {0, 0x4B8},
    {0, 0x4C6}, {0, 0x4D4}, {0, 0x4B4}, {0, 0x524},
};

// FIXME: 95.03%, scheduling+regalloc + reloc-name: in the prologue the original
// schedules the mRandom LCG constant (0x19660d) and the 0.0f/1.0f/7.0f loads before
// the int-to-float 0x4330 setup, placing 0 in r6 and 3 in r0 instead of r0 and r3.
void NPCLaunchedByThwomp_804AC010::init() {
    field_0x230 = 0;
    field_0xb0 = 0;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mLife = 10000;
    field_0x23c = 0;
    field_0x26c = mSpawnPos;
    mPos.x += (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 3;
    mPos.y += (s32)(7.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 3;
    mSpawnPos = mPos;
    field_0x238 = 0;
    field_0x234 = 0;
    field_0x244 = 0;
    field_0x248 = 3;
    field_0x258 = (field_0x90 >> 22) & 7;
    if ((field_0x90 >> 25) & 1) {
        field_0x258 = -field_0x258;
    }
    field_0x278.x = 16.0f * field_0x258 + mSpawnPos.x;
    field_0x25c = (field_0x90 >> 18) & 7;
    if ((field_0x90 >> 21) & 1) {
        field_0x25c = -field_0x25c;
    }
    field_0x278.y = 16.0f * field_0x25c + mSpawnPos.y;
    field_0x278.z = 0.0f;
    field_0x24c = -1;
    field_0x264 = (field_0x90 & 0xFFF) + 0x12B;
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
    fn_80337A08();
    switch (field_0x250) {
    case 2:
        field_0x80.set(-16.0f, -16.0f, 16.0f, 8.0f);
        mBox.set(-16.0f, -16.0f, 16.0f, 8.0f);
        field_0x234 = *lbl_80471BB8[field_0x250].field_0x0;
        break;
    default:
        field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
        field_0x234 = *lbl_80471BB8[field_0x250].field_0x0;
        break;
    }
    field_0x194 = 1;
}

// FIXME: 99.88%, operands + reloc-name: the temporary TVec3f stack slots for
// field_0x3c and the by-value mPos argument to fn_8038C678 are swapped (0x44/0x38/0x2c/0x20
// vs 0x14/0x2c/0x20/0x8 and 0x14/0x8 vs 0x44/0x38), and .sdata2 literals use @NNN symbols.
void NPCLaunchedByThwomp_804AC010::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x -= 2.0f;
    box.f.x += 2.0f;
    if (field_0x254 == 1 && (field_0x230 == 0 || field_0x230 == 4 || field_0x230 == 1)) {
        box.i.x += field_0x26c.x;
        box.i.y += field_0x26c.y;
        box.f.x += field_0x26c.x;
        box.f.y += field_0x26c.y;
    } else {
        box.i.x += mPos.x;
        box.i.y += mPos.y;
        box.f.x += mPos.x;
        box.f.y += mPos.y;
    }
    if (field_0x230 != 2) {
        fn_80239914(&box, field_0x4, mSerial);
    }
    mPos += field_0x3c;
    switch (field_0x230) {
    case 0:
        if (field_0x268 == 0 && fn_8023A30C()) {
            field_0x3c = JGeometry::TVec3f((field_0x278.x - mPos.x) / 40.0f,
                                           (field_0x278.y - mPos.y) / 40.0f, 4.0f);
            field_0x260 = 0.2f;
            field_0x230 = 3;
        } else {
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            field_0x24c = fn_8022FD8C(mSerial);
            if (field_0x24c >= 0 && getPlayerPos_8023E724(field_0x24c)->z == 0.0f &&
                fn_802300EC(field_0x24c) && talk(field_0x4, mSerial, mPos, 0))
            {
                field_0x230 = 4;
            }
        }
        break;
    case 1:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        field_0x24c = fn_8022FD8C(mSerial);
        if (field_0x24c >= 0) {
            if (getPlayerPos_8023E724(field_0x24c)->z != 0.0f || !fn_802300EC(field_0x24c)) {
                break;
            }
            if (talk(field_0x4, mSerial, mPos, 0)) {
                field_0x230 = 4;
                break;
            }
        }
        /* fallthrough */
    case 2:
        if (vfunc_0x34() == 1 && !getGameView_802D6A9C()->fn_802DCD0C()) {
            field_0x254 = 1;
            field_0x23c = ((field_0x90 >> 16) & 3) * 0x46 + 0x1E;
            mPos = field_0x278;
            field_0x3c = JGeometry::TVec3f((mSpawnPos.x - mPos.x) / field_0x23c,
                                           (mSpawnPos.y - mPos.y) / field_0x23c, 0.0f);
            field_0x240 = field_0x23c;
            field_0x230 = 5;
            field_0x238 = 0;
            switch (field_0x250) {
            case 4:
                if (field_0x248 >= 2) {
                    field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
                } else {
                    field_0x234 = *lbl_80471BB8[field_0x250].field_0x0;
                }
                break;
            default:
                field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
                break;
            }
        }
        break;
    case 3:
        if (mPos.z > 0.0f) {
            field_0x3c.z -= field_0x260;
        } else if (mPos.z <= 0.0f && field_0x3c.z < 0.0f) {
            mPos.z = 0.0f;
            field_0x23c = ((field_0x90 >> 16) & 3) * 0x46 + 0x1E;
            mPos = field_0x278;
            field_0x3c = JGeometry::TVec3f((mSpawnPos.x - mPos.x) / field_0x23c,
                                           (mSpawnPos.y - mPos.y) / field_0x23c, 0.0f);
            field_0x240 = field_0x23c;
            field_0x230 = 5;
            field_0x238 = 0;
            switch (field_0x250) {
            case 2:
                getGameView_802D6A9C()->fn_802D79E8(field_0x4, 4.0f, 0.0f, false);
                field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
                break;
            case 4:
                if (field_0x248 >= 2) {
                    field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
                } else {
                    field_0x234 = *lbl_80471BB8[field_0x250].field_0x0;
                }
                break;
            default:
                field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
                break;
            }
        }
        break;
    case 5:
        if (field_0x268 == 0 && fn_8023A30C()) {
            field_0x3c = JGeometry::TVec3f((field_0x278.x - mPos.x) / 40.0f,
                                           (field_0x278.y - mPos.y) / 40.0f, 4.0f);
            field_0x260 = 0.2f;
            field_0x240 = 0;
            field_0x230 = 3;
            field_0x238 = 0;
            field_0x234 = *lbl_80471BB8[field_0x250].field_0x4;
        } else if (field_0x240 <= 0 || (mPos.x == mSpawnPos.x && mPos.y == mSpawnPos.y)) {
            mPos = mSpawnPos;
            field_0x3c.z = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.x = 0.0f;
            field_0x230 = 0;
            field_0x238 = 0;
            field_0x234 = *lbl_80471BB8[field_0x250].field_0x0;
        }
        break;
    case 4:
        fn_8022FEB8(&box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x24c, mSerial)) {
            if (field_0x254 == 1) {
                field_0x230 = 0;
            } else {
                field_0x230 = 1;
            }
        }
        break;
    }
    fn_80337A08();
    fn_803376B0();
    if (field_0x240 > 0) {
        field_0x240--;
    }
}

// FIXME: 95.06%, missing/extra: the original keeps field_0x250 in r0 across the
// frame calculation instead of reloading it before the field_0x250 == 2 check (1 extra lwz).
void NPCLaunchedByThwomp_804AC010::draw() {
    if (field_0x230 == 2) {
        return;
    }
    s32 frame;
    const char* name;
    s32 arg5 = 0;
    if (lbl_80471CB8[field_0x250].field_0x0 == 2) {
        name = "enemy_both2_sob.bin";
    } else {
        name = "enemy_gc_sob.bin";
    }
    if (field_0x230 == 0 || field_0x230 == 3 || field_0x230 == 4 || field_0x230 == 1) {
        frame = lbl_80471CB8[field_0x250].field_0x4 +
                lbl_80471BB8[field_0x250].field_0x8[field_0x248][field_0x238];
    } else {
        frame = lbl_80471CB8[field_0x250].field_0x4 +
                lbl_80471BB8[field_0x250].field_0xc[field_0x248][field_0x238];
    }
    if (field_0x250 == 2) {
        arg5 = 3;
    }
    drawSprite_801F06F0(name, frame, NULL, arg5, 1.0f, 1.0f);
    switch (field_0x250) {
    case 0:
    case 1:
    case 2:
    case 3:
        fn_803075AC(mSerial, field_0x4, &mPos, frame, 0, 2, -1, false, 0, 0);
        break;
    }
}

// init and update call fn_80337A08 out of line (bl 0x803387FC, bl 0x8033853C).
// FIXME: 99.62%, regalloc-only: field_0x250 and field_0x25c use r3/r4 instead of r4/r3;
// dx and dy are kept in locals so MWCC does not reload field_0x258/field_0x25c after
// squaring them.
#pragma auto_inline off
void NPCLaunchedByThwomp_804AC010::fn_80337A08() {
    s32 prevFacing = field_0x248;
    switch (field_0x250) {
    case 5:
    case 6:
    case 14:
        field_0x248 = 0;
        return;
    }
    switch (field_0x230) {
    case 0:
    case 1:
    case 4:
        if (field_0x244 != 0) {
            field_0x244--;
            return;
        }
        field_0x244 = 20;
        JGeometry::TVec3f diff;
        f32 dist;
        fn_801EE558(&diff, &dist, 0);
        if (dist < 100.0f) {
            u16 angle = (s32)(JMath::atanTable_.atan2_(-diff.x, -diff.y) *
                              (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
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
    case 3:
    case 5: {
        s32 dx = field_0x258;
        s32 dy = field_0x25c;
        s32 dx2 = dx * dx;
        s32 dy2 = dy * dy;
        switch (field_0x250) {
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 15:
            if (dx > 0) {
                field_0x248 = 0;
            } else {
                field_0x248 = 1;
            }
            break;
        default:
            if (dx2 == 0 && dy2 == 0) {
                field_0x248 = 2;
            } else if (dx2 > dy2) {
                if (dx > 0) {
                    field_0x248 = 0;
                } else {
                    field_0x248 = 1;
                }
            } else {
                if (dy > 0) {
                    field_0x248 = 2;
                } else {
                    field_0x248 = 3;
                }
            }
            break;
        }
        if (field_0x230 == 5) {
            field_0x248 ^= 1;
        }
        break;
    }
    }
    if (field_0x250 == 4) {
        if ((field_0x248 >= 2 && prevFacing < 2) || (field_0x248 < 2 && prevFacing >= 2)) {
            field_0x238 = 0;
            field_0x234 = 20;
        }
    }
}

// update calls fn_803376B0 out of line (bl 0x80338544).
// field_0x238 is cast to u32 for the modulo so MWCC emits a single clrlwi
// while keeping the post-switch comparison (field_0x238 == 0) signed (cmpwi).
void NPCLaunchedByThwomp_804AC010::fn_803376B0() {
    field_0x234--;
    if (field_0x234 > 0) {
        return;
    }
    field_0x238++;
    switch (field_0x250) {
    case 0:
    case 1:
    case 5:
    case 6:
    case 7:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
            field_0x238 = (u32)field_0x238 % 2;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x0[field_0x238];
        } else if (field_0x230 == 5) {
            field_0x238 = (u32)field_0x238 % 4;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x4[field_0x238];
        }
        break;
    case 2:
    case 3:
        if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
            field_0x238 = (u32)field_0x238 % 4;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x0[field_0x238];
        } else if (field_0x230 == 5) {
            field_0x238 = (u32)field_0x238 % 4;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x4[field_0x238];
        }
        break;
    case 4:
        if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
            field_0x238 = (u32)field_0x238 % 2;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x0[field_0x238];
        } else if (field_0x230 == 5) {
            if (field_0x248 < 2) {
                field_0x238 = (u32)field_0x238 % 2;
                field_0x234 = lbl_80471BB8[field_0x250].field_0x0[field_0x238];
            } else {
                field_0x238 = (u32)field_0x238 % 4;
                field_0x234 = lbl_80471BB8[field_0x250].field_0x4[field_0x238];
            }
        }
        break;
    case 8:
    case 9:
    case 15:
        if (field_0x230 == 0 || field_0x230 == 1 || field_0x230 == 3 || field_0x230 == 4) {
            field_0x238 = (u32)field_0x238 % 2;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x0[field_0x238];
        } else if (field_0x230 == 5) {
            field_0x238 = (u32)field_0x238 % 2;
            field_0x234 = lbl_80471BB8[field_0x250].field_0x4[field_0x238];
        }
        break;
    }
    if (field_0x238 == 0) {
        field_0x234 += (s32)(3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    }
}
#pragma auto_inline reset

NPCLaunchedByThwomp_804AC010::~NPCLaunchedByThwomp_804AC010() {}
