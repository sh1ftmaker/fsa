#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Unk_8045A080.h"
#include "game/act/tu_801F9484.h"
#include "game/Effect.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Player.h"
#include "game/StageFlag.h"
#include "game/GameState.h"
#include "JSystem/JParticle/JPAEmitter.h"

// The 'GOL2' offset tables in rodata (per mode and sprite; only the part init reads is
// described, the rest of the 0x680 bytes belongs to update).
struct GoalTables_8046EA10 {
    f32 x1[6][4];
    f32 y1[6][4];
    f32 x2[6][4];
    f32 y2[6][4];
    u16 id[6][4];
    JGeometry::TBox2f hurtDefault[4][4];  // 0x1B0: [mode][sprite] (modes 0, 2, 3)
    JGeometry::TBox2f attackDefault[4][4]; // 0x2B0
    JGeometry::TBox2f hurtMode1[4][3];    // 0x3B0: [sprite][part]
    JGeometry::TBox2f attackMode1[4][3];  // 0x470
    JGeometry::TBox2f hurtMode4[3][3];    // 0x530
    JGeometry::TBox2f attackMode4[3][3];  // 0x5C0
    JGeometry::TVec2f focus[6];           // 0x650: [mode] offset of the camera focus point
};
extern const GoalTables_8046EA10 lbl_8046EA10;

// 0x802E3B2C.
// FIXME(shotgun): 84.30 %, missing/extra; behaviour drift.
void cls_804A9C20::init() {
    field_0x238 = field_0x90 & 0x1F;
    field_0x240 = (field_0x90 >> 12) & 0x1F;
    field_0x244 = (field_0x90 >> 7) & 1;
    field_0x23c = gGameRoot_8053AAF8->fn_80138A30();
    for (s32 i = 0; i < 4; i++) {
        mEmitters2c8[i] = NULL;
        mEmitters2d8[i] = NULL;
    }
    mEmitter2e8 = NULL;
    mEmitter2ec = NULL;
    field_0x2f4 = 0xFF;
    if (!getGameView_802D6A9C()->fn_802D800C(field_0x240)) {
        for (s32 i = 0; i < 4; i++) {
            field_0x258[i].x = mPos.x + lbl_8046EA10.x1[field_0x238][i];
            field_0x258[i].y = mPos.y + lbl_8046EA10.y1[field_0x238][i];
            field_0x258[i].z = 0.0f;
            field_0x298[i] = 0;
            field_0x2a8[i] = 0;
            field_0x288[i] = 1;
            mEmitters2c8[i] = spawnEmitter_8013CC50(field_0x4, &field_0x258[i], 0x10A, 0, 0, NULL, NULL,
                                                    true, 1.0f);
        }
        if (field_0x238 != 5) {
            for (s32 i = 0; i < 4; i++) {
                field_0x2b8[i] = lbl_8046EA10.id[field_0x238][i];
                if (lbl_8046EA10.id[field_0x238][i] != 0) {
                    JGeometry::TVec3f pos(field_0x258[i].x + lbl_8046EA10.x2[field_0x238][i],
                                          field_0x258[i].y + lbl_8046EA10.y2[field_0x238][i],
                                          field_0x258[i].z);
                    mEmitters2d8[i] = spawnEmitter_8013CC50(field_0x4, &pos, lbl_8046EA10.id[field_0x238][i],
                                                            1, 1, NULL, NULL, true, 1.0f);
                }
            }
        }
        field_0x230 = 0;
        field_0x234 = 0;
        field_0x24c[0] = mPos.x;
        field_0x24c[1] = mPos.y;
        field_0x24c[2] = mPos.z;
    } else {
        field_0x24c[0] = mPos.x;
        field_0x24c[1] = mPos.y;
        field_0x24c[2] = mPos.z;
        field_0x230 = 5;
    }
    if (field_0x238 == 5) {
        JGeometry::TVec3f pos(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        mEmitter2e8 = spawnEmitter_8013CC50(field_0x4, &pos, 0xC6, 0, 0, NULL, NULL, true, 1.0f);
        JGeometry::TVec3f pos2(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        mEmitter2ec = spawnEmitter_8013CC50(field_0x4, &pos2, 0xC7, 0, 0, NULL, NULL, true, 1.0f);
    }
    field_0x248 = 0;
    if (field_0x238 != 5) {
        gAudioMgr_8053AF80->fn_80457E54(true);
    }
}

// 0x802E3A48.
cls_804A9C20::~cls_804A9C20() {
    for (s32 i = 0; i < 4; i++) {
        if (mEmitters2c8[i] != NULL) {
            releaseEmitter_801EE434(mEmitters2c8[i]);
        }
        if (mEmitters2d8[i] != NULL) {
            releaseEmitter_801EE434(mEmitters2d8[i]);
        }
    }
    if (mEmitter2e8 != NULL) {
        releaseEmitter_801EE434(mEmitter2e8);
    }
    if (mEmitter2ec != NULL) {
        releaseEmitter_801EE434(mEmitter2ec);
    }
    if (field_0x238 != 5) {
        gAudioMgr_8053AF80->fn_80457E54(false);
    }
}

// The hit box `box` (relative to mPos) the actor shifts to mPos and hands to fn_80239914.
static inline void shiftBox(JGeometry::TBox2f* out, const JGeometry::TBox2f& box, const JGeometry::TVec3f& pos) {
    out->i.x = box.i.x + pos.x;
    out->i.y = box.i.y + pos.y;
    out->f.x = box.f.x + pos.x;
    out->f.y = box.f.y + pos.y;
}

// 0x802E2C28.
// FIXME(shotgun): 49.01 %, missing/extra + reloc-name; behaviour drift.
void cls_804A9C20::update() {
    if (field_0x230 < 1) {
        field_0x248++;
    }
    mPos.x = field_0x24c[0];
    mPos.y = field_0x24c[1];
    mPos.z = field_0x24c[2];
    JGeometry::TVec3f savedPos = mPos;
    switch (field_0x230) {
    case 1:
        field_0x234--;
        if (field_0x234 == 0) {
            field_0x230 = 4;
            field_0x234 = 100;
        }
        break;
    case 4:
        field_0x234--;
        if (field_0x234 == 0) {
            if (field_0x238 != 5) {
                mPos = savedPos;
                s32 kind = 0;
                if (field_0x248 < 0x258) {
                    if (gGameRoot_8053AAF8->field_0x7e == 1) {
                        kind = 2;
                    } else {
                        kind = 1;
                    }
                }
                if (!getGameView_802D6A9C()->fn_802D75BC(field_0x16c)) {
                    fn_801F34DC();
                } else {
                    kind = 0;
                }
                if (!(getGameView_802D6A9C()->field_0x4 == 7 && getGameView_802D6A9C()->field_0x8 == 3)) {
                    getActorManager_801F856C()->spawnActor_801F9484(
                        'GOAL', field_0x4, &mPos, (kind << 17) + ((field_0x238 + 1) << 12) + 2, -1, -1);
                }
                mAlive = false;
            } else {
                field_0x230 = 5;
            }
        } else if (field_0x234 == 0x3C) {
            fn_801F0D20(3);
        }
        break;
    case 3:
    case 5:
        break;
    case 2:
        field_0x2c4--;
        if (field_0x2c4 == 0) {
            lbl_8053AC00 = 1;
            field_0x230 = 0;
        }
        // fallthrough
    case 0:
        {
            s32 downCount = 0;
            for (s32 i = 0; i < 4; i++) {
                if (field_0x288[i] == 4) {
                    downCount++;
                }
            }
            for (s32 i = 0; i < 4; i++) {
                mPos = field_0x258[i];
                mBox.set(-10.0f, -10.0f, 10.0f, 10.0f);
                JGeometry::TBox2f area;
                shiftBox(&area, mBox, mPos);
                JGeometry::TVec2f xy;
                xy.set(mPos.x, mPos.y);
                s32 player = fn_8023B8E4(&area, field_0x4, &xy, true);
                if (field_0x288[i] == 1) {
                    if (player >= 0 && fn_80237060(player) == 2) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x113, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x114, 1, 1, NULL, NULL, true, 1.0f);
                        field_0x288[i] = 2;
                        field_0x298[i] = 20;
                        field_0x2a8[i] = 0;
                        field_0x2c0[i] = 0xFF;
                        startSound_801F0E34(0x3003A, 0);
                        gGameRoot_8053AAF8->fn_80134DE4(1, 0);
                        if (downCount >= 3) {
                            field_0x288[i] = 3;
                            field_0x2c0[i] = 0;
                            mEmitters2c8[i]->setGlobalAlpha(field_0x2c0[i]);
                        }
                    } else if (player >= 0) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x7B, 0, 0, NULL, NULL, true, 1.0f);
                        JGeometry::TVec2f hit;
                        hit.set(mPos.x, mPos.y);
                        fn_80238548(player, 0, &hit, 10);
                        startSound_801F0E34(0x3008B, 0);
                        field_0x230 = 2;
                        field_0x2c4 = 0x2D;
                    }
                } else if (field_0x288[i] == 2) {
                    field_0x2c0[i] -= 12;
                    field_0x298[i]--;
                    if (field_0x298[i] == 0) {
                        field_0x2c0[i] = 0;
                        field_0x288[i] = 3;
                    }
                    mEmitters2c8[i]->setGlobalAlpha(field_0x2c0[i]);
                } else if (field_0x288[i] == 3) {
                    if (mEmitters2c8[i] != NULL) {
                        releaseEmitter_801EE434(mEmitters2c8[i]);
                        mEmitters2c8[i] = NULL;
                    }
                    field_0x288[i] = 4;
                    if (downCount >= 3) {
                        field_0x298[i] = 1;
                    }
                } else if (field_0x288[i] == 4) {
                    if (field_0x2a8[i] != 0) {
                        field_0x2a8[i]--;
                    }
                    if (field_0x2a8[i] == 0 && player >= 0) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x113, 1, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x114, 1, 1, NULL, NULL, true, 1.0f);
                        field_0x2a8[i] = 20;
                    }
                }
                field_0x80.set(-10.0f, -10.0f, 10.0f, 10.0f);
                shiftBox(&area, field_0x80, mPos);
                fn_80239914(&area, field_0x4, mSerial);
                if (field_0x2b8[i] != 0) {
                    switch (field_0x238) {
                    case 4:
                        if (i < 3) {
                            if (i != 1) {
                                for (s32 j = 0; j < 3; j++) {
                                    mBox = lbl_8046EA10.hurtMode4[i][j];
                                    shiftBox(&area, mBox, mPos);
                                    if (!fn_801F2B7C(field_0x114, -1, 4)) {
                                        field_0x80 = lbl_8046EA10.attackMode4[i][j];
                                        shiftBox(&area, field_0x80, mPos);
                                        fn_80239914(&area, field_0x4, mSerial);
                                    }
                                }
                            } else {
                                mBox.set(0.0f, -8.0f, 64.0f, 8.0f);
                                shiftBox(&area, mBox, mPos);
                                if (!fn_801F2B7C(field_0x114, -1, 4)) {
                                    field_0x80.set(0.0f, -7.0f, 64.0f, 7.0f);
                                    shiftBox(&area, field_0x80, mPos);
                                    fn_80239914(&area, field_0x4, mSerial);
                                }
                            }
                        }
                        break;
                    case 1:
                        for (s32 j = 0; j < 3; j++) {
                            mBox = lbl_8046EA10.hurtMode1[i][j];
                            shiftBox(&area, mBox, mPos);
                            if (!fn_801F2B7C(field_0x114, -1, 0x1A)) {
                                field_0x80 = lbl_8046EA10.attackMode1[i][j];
                                shiftBox(&area, field_0x80, mPos);
                                fn_80239914(&area, field_0x4, mSerial);
                            }
                        }
                        break;
                    case 5:
                        break;
                    default:
                        mBox = lbl_8046EA10.hurtDefault[field_0x238][i];
                        shiftBox(&area, mBox, mPos);
                        if (!fn_801F2B7C(field_0x114, -1, 0x1A)) {
                            field_0x80 = lbl_8046EA10.attackDefault[field_0x238][i];
                            shiftBox(&area, field_0x80, mPos);
                            fn_80239914(&area, field_0x4, mSerial);
                        }
                        break;
                    }
                }
            }
            bool anyUp = false;
            for (s32 i = 0; i < 4; i++) {
                if (field_0x288[i] != 4) {
                    anyUp = true;
                }
            }
            if (!anyUp) {
                for (s32 i = 0; i < 4; i++) {
                    mPos = field_0x258[i];
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x7B, 0, 0, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x7C, 0, 0, NULL, NULL, true, 1.0f);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x7D, 0, 0, NULL, NULL, true, 1.0f);
                    if (mEmitters2d8[i] != NULL) {
                        mEmitters2d8[i]->setStatus(1);
                        // FIXME(shotgun): the original also stores 1 in the emitter word 0x24.
                        mEmitters2d8[i] = NULL;
                    }
                }
                if (field_0x238 == 5) {
                    vfunc_0x30(0xFF, true);
                } else if (field_0x244 != 0) {
                    vfunc_0x30(0xFF, true);
                }
                if (fn_802DE7CC()) {
                    if (!isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c)) {
                        lbl_8053AED0->fn_8043D67C(0x341, 0, 0);
                    }
                }
                startSound_801F0E34(0x30039, 0);
                field_0x230 = 1;
                field_0x234 = 20;
            }
        }
        break;
    default:
        break;
    }
    if (field_0x230 < 4) {
        startSound_801F0E34(0x30038, 0);
    }
    mPos = savedPos;
    JGeometry::TVec3f focus(mPos.x + lbl_8046EA10.focus[field_0x238].x,
                            mPos.y + lbl_8046EA10.focus[field_0x238].y, mPos.z);
    gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    if (mEmitter2e8 != NULL) {
        mEmitter2e8->setGlobalAlpha(field_0x2f4);
    }
    if (mEmitter2ec != NULL) {
        mEmitter2ec->setGlobalAlpha(field_0x2f4);
    }
}

// 0x802E2998.
// FIXME: 99.82 %, reloc-name; behaviour clean.
// FIXME(shotgun): reloc-name only (string literal placement)
void cls_804A9C20::draw() {
    if (field_0x230 <= 3) {
        f32 savedX = mPos.x;
        f32 savedY = mPos.y;
        f32 savedZ = mPos.z;
        for (s32 i = 0; i < 4; i++) {
            mPos = field_0x258[i];
            if (field_0x288[i] == 1) {
                drawSprite_801F06F0("enemy_gc_sob.bin", 0x25A, NULL, 7, 1.0f, 1.0f);
            } else {
                drawSprite_801F06F0("enemy_gc_sob.bin", 0x25B, NULL, 7, 1.0f, 1.0f);
            }
        }
        mPos.x = savedX;
        mPos.y = savedY;
        mPos.z = savedZ;
    }
    if (field_0x238 == 5) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -2;
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "shikimono.bti");
        JGeometry::TVec3f pos(mPos.x + 80.0f, mPos.y + 48.0f, mPos.z);
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F745C(&pos, res, &params);
        }
    }
}

// 0x802E2990.
void cls_804A9C20::fn_802E2990(u8 value) {
    field_0x2f4 = value;
}
