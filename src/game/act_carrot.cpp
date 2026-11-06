#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/RotMatrix.h"
#include "game/ViewLayer.h"
#include "game/act/act_carrot.h"

static const Vec lbl_80472010[12] = {
    { 0.6f, 0.0f, 0.0f },
    { -0.6f, 0.0f, 0.0f },
    { 0.0f, 0.6f, 0.0f },
    { 0.0f, -0.6f, 0.0f },
    { 1.6f, 0.0f, 0.0f },
    { -1.6f, 0.0f, 0.0f },
    { 0.0f, 1.6f, 0.0f },
    { 0.0f, -1.6f, 0.0f },
    { 2.4f, 0.0f, 0.0f },
    { -2.4f, 0.0f, 0.0f },
    { 0.0f, 2.4f, 0.0f },
    { 0.0f, -2.4f, 0.0f },
};

void Carrot_804AC6B0::init() {
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;
    field_0x80.i.x = -4.0f;
    field_0x80.i.y = -4.0f;
    field_0x80.f.x = 4.0f;
    field_0x80.f.y = 4.0f;
    mLife = 8;
    field_0x264 = 0;
    field_0x26c = 1.0f;
    field_0x268 = 0.0f;
    field_0x234.z = 0.0f;
    field_0x234.y = 0.0f;
    field_0x234.x = 0.0f;
    field_0x270 = (field_0x90 >> 8) & 1;
    field_0x24c = 0x400;
    if (((field_0x90 >> 4) & 1) == 1) {
        field_0x230 = 1;
        field_0x264 = fn_80226850(&mPos, field_0x4);
        field_0xb0 = 0;
    } else {
        field_0x230 = 0;
        field_0xb0 = 0x20000;
    }
    field_0x254 = (field_0x90 >> 27) & 3;
    if (field_0x254 != 0) {
        field_0x258 = (field_0x90 >> 22) & 3;
        const Vec* v = &lbl_80472010[field_0x258 | ((field_0x254 - 1) << 2)];
        field_0x3c.x = v->x;
        field_0x3c.y = v->y;
        field_0x3c.z = v->z;
    }
    field_0x250 = -1;
    field_0x271 = field_0x90 & 1;
    field_0x25c = 0;
}

bool Carrot_804AC6B0::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 tileCode = fn_8022461C(pos, layer);
    if (tileCode == 0x4C || tileCode == 0x4D || tileCode == 0x4E || tileCode == 0x4F) {
        return false;
    }
    if (tileCode == 3) {
        if (mPos.z <= 0.0f) {
            field_0x234.x = 8.0f + ((u16)mPos.x & 0xFFF0);
            field_0x234.y = 8.0f + ((u16)mPos.y & 0xFFF0);
            field_0x24c = 0x400;
            field_0x230 = 3;
            field_0x3c.x = 0.0f;
            field_0x3c.y = 0.0f;
            field_0x3c.z = 0.0f;
        }
        return false;
    }
    if (tileCode == 0xD || tileCode == 0x3A) {
        if (mPos.z <= 0.0f) {
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x3D0, 0, 1, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30022, 0);
            mAlive = false;
        }
        return false;
    }
    if (tileCode == 0xE) {
        if (mPos.z <= 0.0f) {
            if (fn_802DE9A0()) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2A9, -1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x2AA, -1, 0, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x5005F, 0);
                mAlive = false;
            } else if (fn_802DEB58()) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x45E, -1, 1, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x45F, 0, 0, NULL, NULL, true, 1.0f);
                startSound_801F0E34(0x5003F, 0);
            } else {
                field_0x234.x = 8.0f + ((u16)mPos.x & 0xFFF0);
                field_0x234.y = 8.0f + ((u16)mPos.y & 0xFFF0);
                field_0x24c = 0x400;
                field_0x230 = 3;
                field_0x3c.x = 0.0f;
                field_0x3c.y = 0.0f;
                field_0x3c.z = 0.0f;
            }
        }
        return false;
    }
    if (tileCode == 0x39) {
        return false;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

void Carrot_804AC6B0::update() {
    switch (field_0x230) {
    case 0:
        if (viewLayerFlag(field_0x4)) {
            mPos.z = 0.0f;
            field_0x3c.z = 0.0f;
            field_0x3c.y += 0.2f;
            mPos += field_0x3c;
            if (fn_801F2618(NULL, false) == 1) {
                field_0x3c.x = -field_0x3c.x;
                if (field_0x3c.y <= 1.0f) {
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                } else {
                    field_0x3c.y = -(field_0x3c.y / 2.0f);
                }
            }
        } else {
            if (mPos.z != 0.0f || field_0x3c.z != 0.0f) {
                field_0x3c.z -= 0.2f;
                if (mPos.z <= 0.0f) {
                    mPos.z = 0.0f;
                    field_0x3c.z = 0.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.x = 0.0f;
                }
            }
            mPos += field_0x3c;
            if (fn_801F2618(NULL, false) == 1) {
                field_0x3c.x = -field_0x3c.x;
                field_0x3c.y = -field_0x3c.y;
            }
        }
        for (int i = 0; i < 4; i++) {
            JGeometry::TBox2f box(mBox);
            box.addPos(JGeometry::TVec2f(mPos.x, mPos.y));
            if (fn_80233DA4(i) == true) {
                box.f.y += 16.0f;
            }
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f) == true) {
                if (getPlayerPos_8023E724(i)->z <= 0.0f) {
                    field_0x250 = i;
                    break;
                }
            }
        }
        {
            bool ok = false;
            bool mode4 = false;
            if (field_0x250 >= 0) {
                if (fn_8022CAF8(field_0x250) == 4) {
                    if (!fn_8022F478(field_0x250)) {
                        ok = true;
                        mode4 = true;
                    }
                } else if (isPlayerBitSet(gGameRoot_8053AAF8, field_0x250) != 0 || fn_8022F514(field_0x250)) {
                    ok = true;
                }
                if (!ok) {
                    field_0x250 = -1;
                }
            }
            if (ok == true && !fn_80235580(field_0x250) && !fn_8023EA28(field_0x250)) {
                field_0x24c = 0x2D0;
                if (!mode4 && fn_8022F514(field_0x250)) {
                    field_0x250 = fn_8022F3DC(field_0x250);
                }
                if (fn_8023EBB8(field_0x250) == true) {
                    fn_80230C6C(field_0x250);
                }
                bool flag2 = false;
                if (fn_80233DA4(field_0x250) == true) {
                    flag2 = true;
                }
                if (fn_8023C830(field_0x250) == true) {
                    field_0xb0 = 0;
                    if (flag2 == true) {
                        fn_80230FD0(field_0x250, 0x20049);
                    }
                    if (field_0x254 != 0) {
                        field_0x230 = 4;
                        field_0x25c = 25;
                        field_0x260 = 60;
                        mBox.i.x = 0.0f;
                        mBox.i.y = 0.0f;
                        mBox.f.x = 0.0f;
                        mBox.f.y = 0.0f;
                        field_0x80.i.x = 0.0f;
                        field_0x80.i.y = 0.0f;
                        field_0x80.f.x = 0.0f;
                        field_0x80.f.y = 0.0f;
                        field_0x240 = mPos;
                        break;
                    } else if (field_0x270 == 0) {
                        fn_801F3668();
                        break;
                    } else {
                        mAlive = false;
                        break;
                    }
                }
            }
            if (field_0x271 == 1) {
                field_0x24c--;
            }
        }
        break;
    case 1:
        if (field_0x264 != fn_80226850(&mPos, field_0x4)) {
            field_0x230 = 0;
            field_0x3c.z = 2.0f;
        }
        break;
    case 2:
        break;
    case 3:
        addCalc2_801F71A4(&mPos.x, field_0x234.x, 0.1f, 1.0f);
        addCalc2_801F71A4(&mPos.y, field_0x234.y, 0.1f, 1.0f);
        field_0x268 += 0.1f;
        field_0x26c -= 0.02f;
        if (field_0x26c < 0.1f) {
            fn_801F3668();
        }
        break;
    case 4:
        if (fn_8023F264(field_0x250)) {
            mPos = *getPlayerPos_8023E724(field_0x250);
            mPos.y -= 38.0f;
        }
        if (field_0x25c != 0) {
            field_0x25c--;
            if (field_0x25c == 0) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_NNJN, field_0x4, &field_0x240, field_0x90, field_0x16c, -1);
            } else if (field_0x25c == 4) {
                if (field_0x4 >= 8) {
                    JGeometry::TVec3f pos(field_0x240);
                    pos.z = 0.0f;
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_KMRI, field_0x4, &pos, 3, -1, -1);
                } else {
                    spawnEmitter_8013CC50(field_0x4, &field_0x240, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                }
            }
        }
        if (field_0x260 != 0) {
            field_0x260--;
            if (field_0x260 == 0) {
                mAlive = false;
            }
        }
        break;
    }
    if (field_0x24c <= 0) {
        mAlive = false;
    }
}

// FIXME: 99.05 %. regalloc-only: target->ours f29->f31, f28->f26, f26->f28, f31->f29 (setRotScaleMtx saved-FPR assignment across the sin/cos calls).
void Carrot_804AC6B0::draw() {
    if (field_0x230 == 4 && field_0x260 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x2d = 1;
    Mtx mtx;
    s32 flags = 7;
    PSMTXIdentity(mtx);
    JGeometry::TVec3f rot(0.0f, field_0x268, 0.0f);
    setRotScaleMtx(mtx, rot, field_0x26c);
    params.mMtx = mtx;
    if (field_0x230 == 3) {
        flags = -1;
    }
    if (field_0x230 == 0 || field_0x230 == 3 || field_0x230 == 4) {
        if (field_0x24c >= 0x100 || (field_0x24c & 2) == 0) {
            if (field_0x230 == 4) {
                params.field_0x1d = 3;
            }
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x2AA, &params, flags, 1.0f, 1.0f);
            fn_803075AC(mSerial, field_0x4, &mPos, 0x2AA, 0, 2, -1, false, 0, 0);
        }
    }
}

Carrot_804AC6B0::~Carrot_804AC6B0() {}
