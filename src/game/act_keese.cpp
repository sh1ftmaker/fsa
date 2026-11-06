#include "game/GameAudio.h"
#include "game/act/act_keese.h"
#include "game/act/act_bombchu.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/RotMatrix.h"
#include "game/ViewLayer.h"
#include <math.h>

// FIXME: 99.36 %. regalloc-only: target->ours r6->r5, r5->r6 (the seed constant 0x19660D and gGameRoot_8053AAF8 pointer swap r5/r6).
void Keese_804A02B8::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(0.0f, 0.0f, 0.0f, 0.0f);
    field_0x260 = (field_0x90 >> 4) & 1;
    if (field_0x260 != 0) {
        mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
        field_0xc0 = 2;
    }
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x25c = 0;
    field_0x25e = 0;
    if (field_0x90 & 0x100) {
        field_0x25e = 1;
        field_0x25c = 0x8000;
    }
    field_0x264 = field_0x90 & 0xF;
    if (field_0x264 == 3) {
        field_0x24c = (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            field_0x254 = -((s32)(512.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x200);
        } else {
            field_0x254 = (s32)(512.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x200;
        }
    }
    if (field_0x264 == 2) {
        mPos.x += (f32)((s32)(9.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 4);
        mPos.y += (f32)((s32)(9.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) - 4);
        mSpawnPos = mPos;
    }
    field_0x26c = 0.0f;
    field_0x270 = 0.0f;
    field_0xb0 = 0x31201;
    field_0x274 = 0.0f;
    field_0x278 = 0.0f;
    field_0x27c = 0.0f;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x258 = 2;
    field_0x268 = 0;
    field_0x26a = 0xFFFF;
    mPos.z = 1.0f;
    if (field_0x90 >> 27) {
        field_0xb0 = 0;
    }
    if (field_0x264 == 4) {
        mPos.z = 0.0f;
        field_0x230 = 3;
    } else {
        field_0x230 = 0;
    }
    field_0x280.x = mPos.x;
    field_0x280.y = mPos.y;
    field_0x280.z = mPos.z;
}

void Keese_804A02B8::update() {
    if (field_0x90 >> 27) {
        if (!vfunc_0x34()) {
            return;
        }
        field_0xb0 = 0x31201;
    }
    if (field_0x264 == 1) {
        if (field_0x26a == 0xFFFF) {
            if (getGameView_802D6A9C()->fn_802DCD0C()) {
                return;
            }
            // fn_80226998 reads only x and y of its position; the 8-byte stack TVec2f is passed as TVec3f.
            s32 layer = field_0x4;
            JGeometry::TVec2f pos(mPos.x, mPos.y);
            field_0x26a = fn_80226998((const JGeometry::TVec3f*)&pos, layer);
            return;
        }
        s32 layer = field_0x4;
        JGeometry::TVec2f pos(mPos.x, mPos.y);
        if (field_0x26a == fn_80226998((const JGeometry::TVec3f*)&pos, layer)) {
            return;
        }
        field_0x264 = 0;
    }
    if (fn_801F3D94(1) != 0) {
        return;
    }
    if (fn_801F2FAC()) {
        if (field_0x25e == 1 && gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
        }
        return;
    }
    if ((s32)fn_8022461C(&mPos, field_0x4) == 0x3B) {
        vfunc_0x24();
        return;
    }
    s32 bmtySerial = fn_801F2718(ACTOR_BMTY);
    if (bmtySerial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(bmtySerial);
        if (actor != NULL && actor->mActorId == ACTOR_BMTY) {
            // u8 local tested against 1 matches the original's subfic 3; cntlzw; extrwi; cmplwi 1.
            u8 isDead = static_cast<Bombchu_804AB464*>(actor)->field_0x230 == 3;
            if (isDead == 1) {
                bmtySerial = -1;
            }
        }
    }
    if (bmtySerial < 0 && fn_801F2DB4(ACTOR_NULL, ACTOR_NULL, NULL)) {
        if (field_0x25e == 1 && gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
        }
        return;
    }
    u16 angle = 0;
    f32 dist = 0.0f;
    s32 dir = 0;
    s32 player = -1;
    fn_801EEC98(mSerial, &angle, &dir, &dist, &player, 0, 0);
    addCalc2_801F71A4(&field_0x3c.x, field_0x274, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x278, 0.3f, 1.0f);
    mPos += field_0x3c;
    fn_801F2B7C(-100, -1, 1);
    switch (field_0x230) {
    case 0: {
        bool started = false;
        if (field_0x264 == 3) {
            if (field_0x24c == 0) {
                field_0x274 = 1.2f * JMath::sincosTable_.sinShort(field_0x250);
            } else {
                field_0x278 = 1.2f * JMath::sincosTable_.cosShort(field_0x250);
            }
            field_0x250 += field_0x254;
            if (field_0x234 <= 0) {
                field_0x258++;
                if (field_0x258 > 2) {
                    field_0x258 = 0;
                }
                field_0x234 = 6;
            }
        }
        if (!viewLayerFlag(field_0x4) && field_0x25e == 0) {
            if (field_0x238 == 0 && dir >= 0 && dist < 60.0f) {
                field_0x268 = angle + 0x8000;
                started = true;
                field_0x238 = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x230 = 1;
            }
        } else {
            addCalcAngle2_801F714C(&field_0x25c, 0x8000, 1, 0x3000);
            if (field_0x238 == 0 && dir >= 0 &&
                FABS(mPos.x - getPlayerPos_8023E724(player)->x) < 64.0f &&
                mPos.y < getPlayerPos_8023E724(player)->y &&
                FABS(mPos.y - getPlayerPos_8023E724(player)->y) < 128.0f) {
                started = true;
                field_0x238 = (s32)(10.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 10;
                if (mPos.x > getPlayerPos_8023E724(player)->x) {
                    field_0x244 = (s32)(4096.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 0x1000;
                    field_0x25c = 0x8000;
                } else {
                    field_0x244 = 0xF000 - (s32)(4096.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
                    field_0x25c = 0x8001;
                }
                field_0x26c = getPlayerPos_8023E724(player)->y - 32.0f;
                field_0x23c = (s32)(50.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 50;
                field_0x230 = 2;
            }
        }
        if (started == true) {
            startSound_801F0E34(0x4000F, 0);
            startSound_801F0E34(0x400A9, 0);
        }
        break;
    }
    case 1:
        field_0x274 = 0.7f * JMath::sincosTable_.sinShort(field_0x268);
        field_0x278 = 0.7f * JMath::sincosTable_.cosShort(field_0x268);
        if (dir != -1) {
            addCalcAngle2_801F714C(&field_0x268, angle, 1, 0x300);
        }
        if (field_0x234 <= 0) {
            field_0x258++;
            if (field_0x258 > 2) {
                field_0x258 = 0;
            }
            field_0x234 = 6;
            if (field_0x238 == 0 && field_0x258 == 2 && !vfunc_0x4C(field_0x4, &mPos)) {
                field_0x238 = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
                field_0x274 = 0.0f;
                field_0x278 = 0.0f;
                field_0x27c = 0.0f;
                field_0x230 = 0;
            }
        }
        break;
    case 2:
        if (field_0x238 == 0) {
            u16 target = field_0x244;
            if (field_0x248 == 0 && mPos.y > field_0x26c) {
                field_0x248 = 1;
            }
            if (field_0x248 != 0) {
                addCalcAngle2_801F714C(&field_0x25c, target, 1, 0x200);
                field_0x274 = JMath::sincosTable_.sinShort(field_0x25c - 0x8000);
                field_0x278 = JMath::sincosTable_.cosShort(field_0x25c - 0x8000);
            } else {
                field_0x274 = 1.5f * JMath::sincosTable_.sinShort(field_0x25c - 0x8000);
                field_0x278 = 1.5f * JMath::sincosTable_.cosShort(field_0x25c - 0x8000);
            }
        }
        if (field_0x238 == 0 && field_0x234 <= 0) {
            field_0x258++;
            if (field_0x258 > 2) {
                field_0x258 = 0;
            }
            field_0x234 = 6;
        }
        if (field_0x23c == 0 && vfunc_0x4C(field_0x4, &mPos) == true) {
            field_0x238 = (s32)(20.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + 20;
            field_0x274 = 0.0f;
            field_0x278 = 0.0f;
            field_0x27c = 0.0f;
            field_0x258 = 2;
            field_0x248 = 0;
            field_0x230 = 0;
        }
        break;
    case 3: {
        addCalc2_801F71A4(&field_0x26c, field_0x270, 0.3f, 0.5f);
        // Computed before the addition so MWCC emits fmuls + fadds instead of fmadds.
        f32 dx = field_0x26c * JMath::sincosTable_.sinShort(field_0x268 + field_0x244);
        f32 dy = field_0x26c * JMath::sincosTable_.cosShort(field_0x268 + field_0x244);
        mPos.x = field_0x280.x + dx;
        mPos.y = field_0x280.y + dy;
        field_0x244 += 0x200;
        if (field_0x234 <= 0) {
            field_0x258++;
            if (field_0x258 > 2) {
                field_0x258 = 0;
            }
            field_0x234 = 6;
        }
        break;
    }
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
}

// FIXME: 98.70 %. regalloc-only: target->ours f30->f31, f31->f27, f29->f27, f27->f31 (setRotScaleMtx saved-FPR assignment across the sin/cos calls).
void Keese_804A02B8::draw() {
    if ((field_0x90 >> 27) && !vfunc_0x34()) {
        return;
    }
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x264 == 1) {
        return;
    }
    params.field_0x30 = mSpriteParams.field_0x30;
    JGeometry::TVec3f rot(0.0f, (f32)field_0x25c / -10430.228f, 0.0f);
    setRotScaleMtx(mtx, rot, 1.0f);
    params.mMtx = mtx;
    s32 mode = 0;
    if (field_0x260 != 0) {
        mode = 1;
    }
    if (field_0x4 == 8 && field_0x260 != 0) {
        JGeometry::TVec3f rot2(0.0f, (f32)field_0x25c / -10430.228f, 0.0f);
        setRotScaleMtx(mtx, rot2, 2.0f);
        params.mMtx = mtx;
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x258 + 0x155, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x176, 0, 2, -1, false, 0, 0);
    } else {
        fn_801F02BC("enemy_both2_sob.bin", field_0x258 + 0x155, &params, -1, mode, false, 1.0f, 1.0f);
    }
}

bool Keese_804A02B8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return fn_802265DC(pos, layer, false);
}

Keese_804A02B8::~Keese_804A02B8() {}
