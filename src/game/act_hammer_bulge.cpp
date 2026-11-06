#include "game/GameAudio.h"
#include "game/act/act_hammer_bulge.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/GameMath.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/Effect.h"
#include "JSystem/JMath/JMath.h"

void HammerBulge_804A88E8::init() {
    mBox.set(-50.0f, -50.0f, 50.0f, 50.0f);
    field_0x234 = field_0x90 & 0xF;
    field_0x238 = (field_0x90 >> 4) & 0xFFFF;
    field_0x24d = (field_0x90 >> 20) & 1;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x24c = 0;
    field_0x230 = 0;

    if (vfunc_0x34()) {
        if (field_0x234 > 0) {
            if (getGameView_802D6A9C()->field_0x4 == 2 && getGameView_802D6A9C()->field_0x8 == 1) {
                u32 msg = 0x388;
                if (field_0x234 != 5) {
                    msg = 0x389;
                }
                getActorManager_801F856C()->spawnActor_801F9484('JIJI', field_0x4, &mPos, ((msg - 0x12b) | 0x00110000), -1, -1);
            }
        }
        mAlive = false;
    }
}

// FIXME: scheduling+regalloc + reloc-name (94.76%). Our frame saves r29-r31 where the original saves r28-r31 (the original keeps one more local live across the loop, so every register from this/loop counter on is shifted by one); the fn_802F601C argument loads and the TBox2f scale stores are also scheduled differently.
void HammerBulge_804A88E8::update() {
    switch (field_0x230) {
    case 0: {
        int j;
        if (field_0x244 > 0.0f || field_0x248 > 0.0f) {
            if (field_0x4 < 8) {
                gGameRoot2_8053AB10->field_0x20->fn_802F601C(&mPos, 5, NULL, field_0x244, field_0x248);
            }
        }

        if (field_0x24c == 1) {
            if (field_0x244 < 1.0f) {
                field_0x244 += 0.1f;
            }
            if (field_0x248 < 1.0f) {
                field_0x248 += 0.1f;
            }
            if (field_0x244 >= 1.0f && field_0x248 >= 1.0f) {
                field_0x24c = 0;
            }
        } else {
            if (field_0x248 > 0.0f) {
                field_0x248 -= 0.0025f;
                if (field_0x248 <= 0.0f) {
                    field_0x248 = 0.0f;
                }
            }
            if (field_0x248 <= 0.7f) {
                if (field_0x244 > 0.0f) {
                    field_0x244 -= 0.0025f;
                    if (field_0x244 <= 0.0f) {
                        field_0x244 = 0.0f;
                    }
                }
            }
        }

        if (field_0x240 <= 0) {
            for (j = 0; j < 4; j++) {
                f32 scale = fn_8023725C(j);
                JGeometry::TBox2f box;
                box.f.y = mBox.f.y * scale;
                box.i.y = mBox.i.y * scale;
                box.i.x = mBox.i.x * scale;
                box.f.x = mBox.f.x * scale;
                box.addPos(mPos.x, mPos.y);

                JGeometry::TVec2f out;
                if (fn_8023AAF8(j, &box, field_0x4, &out) == 1) {
                    field_0x240 = 5;
                    f32 dx = out.x - mPos.x;
                    f32 dy = out.y - mPos.y;
                    f32 dist = JGeometry::TUtil<f32>::sqrt(square(dx) + square(dy));

                    if ((dist < 38.0f && (field_0x244 > 0.0f || field_0x248 > 0.0f)) ||
                        (fn_8023A5B8(j) == 1 && dist < 50.0f * scale)) {
                        if (field_0x234 > 0 && field_0x234 <= 8) {
                            JGeometry::TVec3f pos = mPos;
                            if (getGameView_802D6A9C()->field_0x4 == 2 && getGameView_802D6A9C()->field_0x8 == 1) {
                                pos.x -= 8.0f;
                            }

                            s32 kind = field_0x234;
                            s32 serial;
                            if (kind < 5 || kind == 8) {
                                u32 arg = 3;
                                if (kind < 5) {
                                    arg = kind + 3;
                                }
                                serial = getActorManager_801F856C()->fn_801F8544('RUPY', field_0x4, &pos, arg);
                            } else if (kind == 5 || kind == 6) {
                                serial = getActorManager_801F856C()->spawnActor_801F9484('KEY0', field_0x4, &pos, ((kind - 5) * 6) | 0x300, -1, -1);
                            } else {
                                serial = getActorManager_801F856C()->spawnActor_801F9484('BHRT', field_0x4, &pos, 0, -1, -1);
                            }

                            if (serial >= 0) {
                                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                                if (actor != NULL) {
                                    if (actor->mActorId == 'KEY0') {
                                        actor->field_0x3c = JGeometry::TVec3f(
                                            0.35f - 0.7f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                            0.35f - 0.7f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                            5.0f + 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                                        );
                                    } else if (actor->mActorId == 'BHRT') {
                                        s16 angle = 65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                                        f32 speed = 0.15f + 0.35f * gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                                        actor->field_0x3c = JGeometry::TVec3f(
                                            speed * JMath::sincosTable_.sinShort(angle),
                                            speed * JMath::sincosTable_.cosShort(angle),
                                            5.0f + 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                                        );
                                    } else {
                                        actor->field_0x3c = JGeometry::TVec3f(
                                            0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                            0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                            5.0f + 2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()
                                        );
                                    }
                                }
                            }

                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x291, 0, 0, NULL, NULL, true, 0.4f);
                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x5c, 0, 0, NULL, NULL, true, 0.5f);

                            if (getGameView_802D6A9C()->field_0x4 == 2 && getGameView_802D6A9C()->field_0x8 == 1) {
                                pos.x += 16.0f;
                                s32 msg = 0x388;
                                if (field_0x234 != 5) {
                                    msg = 0x389;
                                }
                                getActorManager_801F856C()->spawnActor_801F9484('JIJI', field_0x4, &pos, ((msg - 0x12b) | 0x02110000), -1, -1);
                            }
                        } else {
                            s32 tkta;
                            if (field_0x234 == 0) {
                                tkta = 4;
                            } else if (field_0x234 == 9) {
                                tkta = 5;
                            }
                            getActorManager_801F856C()->spawnActor_801F9484('TKTA', field_0x4, &mPos, tkta, -1, -1);
                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x240, 0, 0, NULL, NULL, true, 1.0f);
                        }

                        if (field_0x24d == 1) {
                            fn_801F3668();
                        } else {
                            field_0x230 = 1;
                            vfunc_0x30(0xFF, true);
                        }
                        return;
                    } else if (dist < 50.0f * scale) {
                        field_0x23c = 0;
                        field_0x24c = 1;
                    }
                }
            }
        }

        for (int i = 0; i < 4; i++) {
            if (fn_8023A51C(i) == true && fn_8023A5B8(i) == true) {
                JGeometry::TVec3f* playerPos = fn_8023A478(i);
                f32 dx = playerPos->x - mPos.x;
                f32 dy = playerPos->y - mPos.y;
                f32 dist = JGeometry::TUtil<f32>::sqrt(square(dx) + square(dy));
                field_0x23c = (s32)dist >> 3;
            }
        }

        if (field_0x240 > 0) {
            field_0x240--;
            if (field_0x240 <= 0) {
                field_0x240 = 0;
            }
        }

        if (field_0x23c > 0) {
            field_0x23c--;
            if (field_0x23c <= 0) {
                field_0x23c = 0;
                field_0x24c = 1;
            }
        }
        break;
    }

    case 1:
        break;
    }
}

void HammerBulge_804A88E8::draw() {}

HammerBulge_804A88E8::~HammerBulge_804A88E8() {}
