#include "game/GameAudio.h"
#include "game/act/tu_802B7C78.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "JSystem/JMath/JMath.h"

// Read as data by fn_802B7D1C (lbl_804A8858 in the original).
s32 lbl_804A8858[5] = {0, 1, 2, 3, 3};

// Frame offsets into the "SOB " bank, picked by field_0x24c.
const s32 lbl_8046CB60[4] = {0, 1, 0, 2};

static inline void normalize(JGeometry::TVec2f& v) {
    f32 sq = v.x * v.x + v.y * v.y;
    if (sq <= 32.0f * FLT_EPSILON) {
        return;
    }
    f32 norm = JGeometry::TUtil<f32>::inv_sqrt(sq);
    v.x *= norm;
    v.y *= norm;
}

// FIXME: 99.64 %. Only reloc-names: the -8.0f, 8.0f and 2.0f literals are
// @603-@605 here, lbl_80540ADC/AE0/AE4 in the original; they sit at the right
// offsets once fn_802B8078 and update() (which use 16.0f and 48.0f) are written.
void ReaperShinigami_804A886C::init() {
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    field_0x244 = 180;
    field_0x248 = 20;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x260[0] = 1.0f;
    field_0x268[0] = 2.0f;
    field_0x260[1] = 0.75f;
    field_0x268[1] = 1.5f;
    field_0x234 = mPos;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x298 = 4;
    field_0x280 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2a0[i] = getActorManager_801F856C()->fn_801F6058(i);
    }
    field_0x27c = 0;
    field_0x196 = 1;
    field_0x230 = 0;
    startSound_801F0E34(0x4003F, 0);
    lbl_8053AED0->fn_8043D67C(0x197, 0, 0);
}

// The player at field_0x2a0[field_0x27c] is in the game and present.
static inline bool playerInGame(s32 player) {
    return (gGameRoot_8053AAF8->mPlayerMask & (1 << player)) != 0;
}

// FIXME(shotgun): 89.05 %, missing/extra + reloc-name
void ReaperShinigami_804A886C::update() {
    if (gGameRoot_8053AAF8->field_0xfe == 0) {
        vfunc_0x24();
        return;
    }
    switch (field_0x230) {
    case 0:
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x54E, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x172, 1, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 1, 0, NULL, NULL, true, 1.0f);
        field_0x230 = 1;
        break;
    case 1:
    case 8:
        if (field_0x280 == 0) {
            field_0x244--;
            if (field_0x244 == 0) {
                if (gGameRoot_8053AAF8->field_0x7e == 1) {
                    if (gGameRoot_8053AAF8->field_0x7f == 1) {
                        while (true) {
                            field_0x298 = field_0x2a0[field_0x27c];
                            if (playerInGame(field_0x298) == 1 && fn_8023F264(field_0x298) == 1) {
                                break;
                            }
                            field_0x27c++;
                            if (field_0x27c >= 4) {
                                field_0x244 = 0x78;
                                field_0x27c = 0;
                            }
                        }
                        fn_802B8078();
                        field_0x27c++;
                        if (field_0x27c >= 4) {
                            field_0x27c = 0;
                        }
                    } else {
                        bool found = false;
                        while (field_0x27c < 4) {
                            field_0x298 = field_0x2a0[field_0x27c];
                            if (playerInGame(field_0x298) == 1 && fn_8023F264(field_0x298) == 1 &&
                                fn_80237774(field_0x298) > 1) {
                                found = true;
                                break;
                            }
                            field_0x27c++;
                        }
                        if (!found) {
                            mAlive = false;
                            startSound_801F0E34(0x40041, 0);
                        }
                        if (mAlive) {
                            fn_802B8078();
                            field_0x27c++;
                        }
                    }
                } else {
                    field_0x298--;
                    if (field_0x298 < 0) {
                        field_0x298 = 3;
                    }
                    fn_802B8078();
                    field_0x27c++;
                }
            } else {
                field_0x274 += 0xC0;
                mPos.x = field_0x234.x + 16.0f * JMASSin((s16)field_0x274);
                mPos.y = field_0x234.y + 16.0f * JMASCos((s16)field_0x274);
            }
        } else {
            JGeometry::TVec3f playerPos(*getPlayerPos_8023E724(field_0x298));
            field_0x284.x = playerPos.x - mPos.x;
            field_0x284.y = (playerPos.y - 48.0f) - mPos.y;
            normalize(field_0x284);
            field_0x254 = field_0x284.x * field_0x268[1];
            field_0x258 = field_0x284.y * field_0x268[1];
            addCalc2_801F71A4(&field_0x3c.x, field_0x254, 0.3f, field_0x260[1]);
            addCalc2_801F71A4(&field_0x3c.y, field_0x258, 0.3f, field_0x260[1]);
            addCalc2_801F71A4(&field_0x260[1], 1.0f, 0.1f, 0.1f);
            mPos += field_0x3c;
            field_0x280--;
            if (field_0x280 == 0) {
                field_0x234 = mPos;
                field_0x274 = 0x8000;
            }
        }
        break;
    case 2:
        fn_802B7D1C(0, 1);
        break;
    case 3:
        field_0x274 += 0xC0;
        mPos.x = field_0x234.x + 16.0f * JMASSin((s16)field_0x274);
        mPos.y = field_0x234.y + 16.0f * JMASCos((s16)field_0x274);
        field_0x244--;
        if (field_0x244 == 0) {
            fn_802B8078();
            startSound_801F0E34(0x40041, 0);
        }
        break;
    case 4:
        field_0x244--;
        if (field_0x244 == 0) {
            field_0x230 = 2;
            field_0x244 = 300;
            startSound_801F0E34(0x4003F, 0);
        }
        break;
    case 5:
        field_0x244--;
        if (field_0x244 == 0) {
            field_0x230 = 7;
            field_0x244 = 300;
            startSound_801F0E34(0x4003F, 0);
        }
        break;
    case 7:
        fn_802B7D1C(1, 8);
        break;
    }
    if (field_0x270 < 255) {
        field_0x270++;
    }
    if (field_0x250 == 0) {
        field_0x248--;
        if (field_0x248 == 0) {
            field_0x24c++;
            if (field_0x24c >= 4) {
                field_0x24c = 0;
            }
            field_0x248 = 20;
        }
    } else {
        field_0x24c = field_0x250 - 1;
    }
}

void ReaperShinigami_804A886C::draw() {
    if (field_0x230 == 0) {
        return;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    params.field_0x10.a = field_0x270;
    params.field_0x30 = 8;
    drawSprite_801F06F0("enemy_gc_sob.bin", lbl_8046CB60[field_0x24c] + 0x466, &params, 7, 1.0f, 1.0f);
}

void ReaperShinigami_804A886C::vfunc_0x24() {
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x173, 1, 0, NULL, NULL, true, 1.0f);
    mAlive = false;
    startSound_801F0E34(0x40041, 0);
}

// The reappearance fn_802B8078 does twice (inlined): back to where it left, on the player's layer.
static inline void reappear(ReaperShinigami_804A886C* self) {
    self->mPos = self->field_0x28c;
    self->field_0x4 = self->field_0x29c;
    JGeometry::TVec3f emitterPos;
    emitterPos.set(self->mPos);
    emitterPos.y += 32.0f;
    emitterPos.z += 16.0f;
    spawnEmitter_8013CC50(self->field_0x4, &emitterPos, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
    gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(self->field_0x4, &self->mPos, 3);
    self->field_0x244 = 50;
    self->field_0x24c = 0;
    self->field_0x230 = 4;
}

// 0x802B8078 is a plain `fn_802B8078` symbol in the original; ours is the mangled member.
// FIXME(shotgun): 77.86 %, missing/extra + reloc-name
void ReaperShinigami_804A886C::fn_802B8078() {
    field_0x29c = getPlayerLayer_8023DE58(field_0x298);
    if (field_0x29c < 8) {
        if (field_0x4 >= 8) {
            reappear(this);
        } else {
            field_0x230 = 2;
            field_0x244 = 300;
        }
    } else if (field_0x4 >= 8) {
        if (field_0x29c == field_0x4) {
            field_0x230 = 7;
            field_0x244 = 300;
        } else {
            reappear(this);
        }
    } else {
        JGeometry::TVec3f emitterPos;
        emitterPos.set(mPos);
        emitterPos.y += 32.0f;
        emitterPos.z += 16.0f;
        spawnEmitter_8013CC50(field_0x4, &emitterPos, 0x17D, 1, 0, NULL, NULL, true, 1.0f);
        gGameRoot2_8053AB10->field_0x30->fn_8015F8FC(field_0x4, &mPos, 3);
        field_0x28c = mPos;
        mPos = *getPlayerPos_8023E724(field_0x298);
        field_0x4 = field_0x29c;
        field_0x244 = 180;
        field_0x24c = 0;
        field_0x230 = 5;
        JGeometry::TVec3f at(mPos.x - 16.0f, mPos.y - 16.0f, mPos.z);
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &at, 0x81, -1, -1);
        at.set(mPos.x + 16.0f, mPos.y - 16.0f, mPos.z);
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &at, 0x81, -1, -1);
        at.set(mPos.x - 16.0f, mPos.y + 16.0f, mPos.z);
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &at, 0x81, -1, -1);
        at.set(mPos.x + 16.0f, mPos.y + 16.0f, mPos.z);
        getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &at, 0x81, -1, -1);
    }
}

// FIXME: 89.45 %. Its frame is 0x40 with the playerPos copy at 0x10 (ours
// 0x30, playerPos at 0x8). The direction temp's dead stack stores are not
// reproduced, and the idx table walk takes other registers (r28 / r29). (The
// layer is read through getField_0x4(), before the call, as the original does.)
void ReaperShinigami_804A886C::fn_802B7D1C(s32 idx, s32 arg) {
    JGeometry::TVec3f playerPos(*getPlayerPos_8023E724(field_0x298));
    s32 dist = (s32)FABS(mPos.x - playerPos.x);
    if (dist < 40) {
        field_0x250 = 1;
    } else if (mPos.x < playerPos.x) {
        field_0x250 = 2;
    } else {
        field_0x250 = 4;
    }

    startSound_801F0E34(0x40040, 0);
    if (getPlayerLayer_8023DE58(field_0x298) == getField_0x4()) {
        JGeometry::TVec3f* p = getPlayerPos_8023E724(field_0x298);
        JGeometry::TVec2f dir(p->x, p->y);
        dir.x -= mPos.x;
        dir.y -= mPos.y;
        field_0x284 = dir;
        normalize(field_0x284);

        field_0x254 = field_0x284.x * field_0x268[idx];
        field_0x258 = field_0x284.y * field_0x268[idx];
        addCalc2_801F71A4(&field_0x3c.x, field_0x254, 0.3f, field_0x260[idx]);
        addCalc2_801F71A4(&field_0x3c.y, field_0x258, 0.3f, field_0x260[idx]);
        addCalc2_801F71A4(&field_0x260[idx], 1.0f, 0.1f, 0.1f);
        mPos += field_0x3c;

        s32 damage = field_0x114;
        s32 arg3 = 0x12;
        if (gGameRoot_8053AAF8->field_0x7f == 0) {
            s32 n = fn_80237774(field_0x298);
            damage = 0;
            if (n > 1) {
                damage = n - 1;
            }
            arg3 = 4;
        }
        if (fn_8023F264(field_0x298) == true && !fn_80236B50(field_0x298)) {
            u8 hit = fn_801F2B7C(damage, -1, arg3);
            if (hit) {
                startSound_801F0E34(0x40041, 0);
                field_0x244 = 140;
                field_0x234 = mPos;
                if (field_0x298 == lbl_804A8858[hit >> 1]) {
                    field_0x230 = arg;
                    field_0x250 = 0;
                }
                field_0x280 = 100;
                if (gGameRoot_8053AAF8->field_0x7e == 0) {
                    mAlive = false;
                    startSound_801F0E34(0x40041, 0);
                }
            }
        } else {
            field_0x244 = 180;
            field_0x230 = arg;
            field_0x250 = 0;
        }
    } else {
        field_0x244 = 180;
        field_0x234 = mPos;
        field_0x230 = 3;
        field_0x250 = 0;
    }
}

void fn_802B7CD8(ActorBase_804A5770* actor) {
    getGameView_802D6A9C()->fn_802D9FBC();
    actor->vfunc_0x24();
}

ReaperShinigami_804A886C::~ReaperShinigami_804A886C() {}
