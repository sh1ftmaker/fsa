#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Map.h"
#include "game/act/act_chief_soldier.h"
#include "game/Effect.h"
#include "game/GameState.h"
#include "game/GameView.h"
#include "game/Player.h"
#include "game/act/act_keese.h"
#include "game/act/tu_801F9484.h"
#include "JSystem/JMath/JMath.h"

// Tables of the SLR4 actor (.data 0x804A08C0.., not written as source here): per heading
// (field_0x98) hit-box offsets and animation timings / frames (.data 0x804A08C0 pairs, 0x20 frames,
// 0x70 / 0x80 / 0x180 timings, 0x120 frames), boxes and life values.
extern const f32 lbl_804A08C0[][2];
extern const s32 lbl_804A08E0[][4];
extern const s32 lbl_804A0930[];
extern const s32 lbl_804A0940[];
extern const s32 lbl_804A09E0[][6];
extern const s32 lbl_804A0A40[];
extern const f32 lbl_804A0A58[][4];
extern const f32 lbl_804A0AD8[][2];
extern const s32 lbl_804A0AF8[];
extern const f32 lbl_804A0B10[][4];
extern const u8 lbl_80539F78[];


// Sets the 3x3 part of m to a uniform scale; the translation column is the caller's.
static inline void setScaleMtx(Mtx m, f32 s) {
    m[0][0] = s;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = s;
    m[1][2] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = s;
}

// FIXME(shotgun): 92.76 %, different + reloc-name; behaviour clean.
void ChiefSoldier_804A0B90::fn_801752D8() {
    if (field_0x4 >= 8 && field_0x23c == 1 && lbl_80529DEC.field_0x118 == 6) {
        s32 gone = 0;
        for (s32 i = 0; i < 4; i++) {
            s32 serial = field_0x2c0[i];
            if (serial < 0) {
                gone++;
                continue;
            }
            Keese_804A02B8* keese = static_cast<Keese_804A02B8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (keese == NULL) {
                gone++;
                field_0x2c0[i] = -1;
                continue;
            }
            JGeometry::TVec3f pos = mPos;
            keese->field_0x280.x = pos.x;
            keese->field_0x280.y = pos.y;
            keese->field_0x280.z = pos.z;
            field_0x260 = 900;
        }
        if (field_0x260 == 0 && gone == 4) {
            if (field_0x4 == getPlayerLayer_8023DE58(0)) {
                s32 spawned = 0;
                for (s32 i = 0; i < 4; i++) {
                    field_0x2c0[i] = getActorManager_801F856C()->spawnActor_801F9484('KETH', field_0x4, &mPos, 4, -1, -1);
                    s32 serial = field_0x2c0[i];
                    if (serial >= 0) {
                        Keese_804A02B8* keese = static_cast<Keese_804A02B8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
                        if (keese != NULL) {
                            spawned++;
                            keese->field_0x268 = i * 0x4000;
                            keese->field_0x270 = 48.0f;
                            if (spawned >= 4) {
                                field_0x260 = 900;
                            }
                        }
                    }
                }
            }
        }
    }
}

// FIXME(shotgun): 86.13 %, missing/extra + reloc-name; behaviour drift.
void ChiefSoldier_804A0B90::fn_80175024() {
    if (getType() == 6) {
        return;
    }
    s32 serial = field_0x238;
    if (serial < 0) {
        return;
    }
    cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
    if (partner == NULL) {
        s32 type = 7;
        s32 layer = field_0x4;
        if (layer >= 8) {
            type = 8;
            layer = 0;
        }
        field_0x238 = getActorManager_801F856C()->spawnActor_801F9484('SLR4', layer, &mPos, type, -1, -1);
        serial = field_0x238;
        if (serial >= 0) {
            partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (partner != NULL) {
                partner->field_0x114 = field_0x114;
                partner->field_0x238 = mSerial;
                partner->field_0x23c = field_0x23c;
            }
        }
        return;
    }
    switch (field_0x29c) {
    case 2:
        if (field_0x264 >= 4) {
            if (field_0x3c.x < 0.1f && field_0x3c.y < 0.1f) {
                partner->field_0x94 = 0;
                if (field_0x4 >= 8 && field_0x23c == 1) {
                    s32 dir = field_0x98;
                    mBox.set(lbl_804A0A58[dir][0], lbl_804A0A58[dir][1], lbl_804A0A58[dir][2], lbl_804A0A58[dir][3]);
                }
                fn_801F2B7C(-100, -1, 1);
            } else {
                fn_801F2B7C(4, -1, 3);
                partner->field_0x94 = 2;
            }
        } else {
            partner->field_0x94 = 1;
        }
        // fall through
    case 0:
    case 1:
    case 3:
        if (field_0x29c != 2) {
            partner->field_0x94 = 0;
        }
        if (field_0x29c == 3) {
            partner->field_0x94 = 1;
        }
        JGeometry::TVec3f pos;
        if (partner->field_0x90 == 7) {
            pos = mPos;
        } else {
            s32 layer = partner->field_0x4;
            GameView_804A9950* view = getGameView_802D6A9C();
            const JGeometry::TBox2f* boxp = layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
            JGeometry::TBox2f box = *boxp;
            pos.x = mPos.x + box.i.x;
            pos.y = mPos.y + box.i.y;
            pos.z = mPos.z;
        }
        partner->mPos = pos;
        partner->field_0x98 = field_0x98;
        break;
    }
    if (partner->field_0x90 == 8) {
        partner->field_0x280 = field_0x280;
    }
}

// FIXME(shotgun): 97.59 %, missing/extra + reloc-name; behaviour near.
void ChiefSoldier_804A0B90::init() {
    mBox.i.x = -12.0f;
    mBox.i.y = -20.0f;
    mBox.f.x = 12.0f;
    mBox.f.y = 4.0f;
    field_0x80.i.x = -10.0f;
    field_0x80.i.y = -10.0f;
    field_0x80.f.x = 10.0f;
    field_0x80.f.y = 10.0f;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x26c = 0;
    field_0x270 = 0;
    field_0x274 = 0;
    field_0x278 = 0;
    field_0x27c = 0;
    field_0x240 = 0;
    field_0x244 = 0;
    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x25c = 0;
    field_0x260 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x2c0[i] = -1;
    }
    field_0x278 = 3;
    field_0x298 = ((field_0x90 >> 8) & 0xF) * 20;
    field_0x294 = ((field_0x90 >> 12) & 0xF) * 20;
    field_0x2a5 = 0;
    field_0x29c = 0;
    field_0x2a0 = 0;
    field_0x23c = (field_0x90 >> 16) & 0xFF;
    if (field_0x23c != 0) {
        field_0x196 = 1;
        field_0x190 = 0;
    }
    field_0x2a4 = 0;
    field_0x98 = (s32)(3.99f * (gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f)) & 3;
    field_0x2b4.set(mPos);
    field_0x284 = 10000;
    field_0x238 = -1;
    if (getType() != 7 && getType() != 8 && getType() != 6) {
        if (getType() != 2 && getType() != 3 && field_0x23c == 0 && gGameRoot_8053AAF8->field_0x7f == 1) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, 0);
        }
        mLife = lbl_804A0AF8[field_0x90 & 0xFF] * 2;
        if (fn_802DE814() == 1) {
            mLife = lbl_804A0AF8[field_0x90 & 0xFF];
        }
        s32 type = 7;
        s32 layer = field_0x4;
        if (layer >= 8) {
            type = 8;
            layer = 0;
        }
        field_0x238 = getActorManager_801F856C()->spawnActor_801F9484('SLR4', layer, &mPos, type, -1, -1);
        s32 serial = field_0x238;
        if (serial >= 0) {
            cls_804A00C8* partner = static_cast<cls_804A00C8*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
            if (partner != NULL) {
                partner->field_0x114 = field_0x114;
                partner->field_0x238 = mSerial;
                partner->field_0x23c = field_0x23c;
            }
        }
    }
    if (getType() != 7 && getType() != 8 && getType() != 6) {
        if (field_0x4 >= 8) {
            mLife = 50;
            if (field_0x23c == 1) {
                mBox.i.x = -14.0f;
                mBox.i.y = -14.0f;
                mBox.f.x = 14.0f;
                mBox.f.y = 14.0f;
            } else {
                mLife = 25;
                mBox.i.x = -7.0f;
                mBox.i.y = -7.0f;
                mBox.f.x = 7.0f;
                mBox.f.y = 7.0f;
                field_0x80.i.x = -7.0f;
                field_0x80.i.y = -7.0f;
                field_0x80.f.x = 7.0f;
                field_0x80.f.y = 7.0f;
            }
        }
    }
    field_0x284 = mLife;
    if (getType() != 6 && field_0x4 >= 8 && field_0x23c == 1) {
        mLife = 10000;
    }
    field_0x280 = 0;
    field_0x288 = 0;
    field_0x28c = field_0x90 >> 24;
    if (field_0x28c == 0) {
        field_0x28c = -1;
    }
    field_0x90 &= 0xFF;
    field_0x234 = lbl_80539F78[field_0x90 >> 1];
    field_0x2b0 = 0.0f;
    field_0x2ac = 0.0f;
    field_0x2a8 = 0.0f;
    field_0x290 = 0;
    field_0x244 = 10;
    field_0x236 = 0;
    field_0xb0 = 0xA042209;
    field_0xb8 = 1;
    if (field_0x90 == 2 || field_0x90 == 3) {
        field_0x98 = 3;
    }
    field_0x230 = 0;
    fn_801746DC();
    field_0xc0 = 3;
    field_0xbc = 3;
    if (field_0x90 == 4 || field_0x90 == 5 || field_0x90 == 6) {
        field_0x230 = 2;
    }
    if (getType() == 7 || getType() == 8) {
        field_0x1a8 = false;
        field_0xb0 = 0xA040004;
        mLife = 10000;
        field_0xb8 = 0;
        mBox.i.x = -8.0f;
        mBox.i.y = -8.0f;
        mBox.f.x = 8.0f;
        mBox.f.y = 8.0f;
        field_0x80.i.x = 1.0f;
        field_0x80.i.y = 1.0f;
        field_0x80.f.x = -1.0f;
        field_0x80.f.y = -1.0f;
        field_0x230 = 10;
    }
}

// FIXME(shotgun): 81.63 %, different + reloc-name; behaviour drift.
void ChiefSoldier_804A0B90::fn_801746DC() {
    if (field_0x29c != field_0x2a0) {
        field_0x2a0 = field_0x29c;
        field_0x2a4 = 0;
        field_0x240 = 0;
        field_0x264 = 0;
        field_0x274 = 0;
        field_0x25c = 0;
        if (field_0x29c == 3) {
            field_0x264 = 5;
        }
    }
    switch (field_0x29c) {
    case 0:
        if (field_0x240 == 0) {
            field_0x240 = lbl_804A0930[field_0x264];
            field_0x280 = lbl_804A08E0[field_0x98][field_0x264];
            field_0x264++;
            field_0x264 &= 3;
        }
        break;
    case 1:
        if (field_0x240 == 0) {
            field_0x240 = lbl_804A0940[field_0x264];
            field_0x280 = lbl_804A0940[field_0x98 * 8 + field_0x264];
            field_0x264++;
            field_0x264 &= 7;
            if (field_0x264 == 0) {
                field_0x2a4 = 1;
            }
        }
        break;
    case 2:
        if (field_0x240 == 0) {
            if (field_0x264 >= 6) {
                field_0x2a4 = 1;
                break;
            }
            field_0x240 = lbl_804A0A40[field_0x264];
            if (field_0x4 >= 8 && field_0x23c == 1) {
                field_0x240 = (s32)(0.5f * field_0x240);
            }
            if (field_0x264 == 1) {
                field_0x240 += 10;
                JGeometry::TVec3f pos(mPos);
                pos.x = mPos.x + lbl_804A08C0[field_0x98][0];
                pos.y = mPos.y + lbl_804A08C0[field_0x98][1];
                spawnEmitter_8013CC50(field_0x4, &pos, 0xCB, 0, 3, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &pos, 0xCC, 0, 3, NULL, NULL, true, 1.0f);
            }
            field_0x280 = lbl_804A09E0[field_0x98][field_0x264];
            field_0x264++;
            if (field_0x264 == 6) {
                field_0x240 += 15;
            }
            if (field_0x264 == 4) {
                startSound_801F0E34(0x4008B, 0);
                switch (field_0x98) {
                case 0:
                    field_0x3c.set(13.0f, 0.0f, 0.0f);
                    break;
                case 1:
                    field_0x3c.set(-13.0f, 0.0f, 0.0f);
                    break;
                case 2:
                    field_0x3c.set(0.0f, 13.0f, 0.0f);
                    break;
                case 3:
                    field_0x3c.set(0.0f, -13.0f, 0.0f);
                    break;
                }
            }
        }
        if (field_0x25c == 0) {
            field_0x25c = 4;
            field_0x274++;
            if (field_0x274 > 5) {
                field_0x274 = 0;
            }
        }
        break;
    case 3:
        if (field_0x240 == 0) {
            if (field_0x264 <= 0) {
                field_0x2a4 = 1;
                break;
            }
            field_0x240 = lbl_804A0A40[field_0x264];
            field_0x280 = lbl_804A09E0[field_0x98][field_0x264];
            field_0x264--;
        }
        if (field_0x25c == 0) {
            field_0x25c = (6 - field_0x264) * 2;
            field_0x274++;
            if (field_0x274 > 5) {
                field_0x274 = 0;
            }
        }
        break;
    }
}

ChiefSoldier_804A0B90::~ChiefSoldier_804A0B90() {
    if (getType() != 7 && getType() != 8) {
        if (getType() != 6 && gGameRoot_8053AAF8->field_0x4c != 9 &&
            gGameRoot_8053AAF8->field_0x4c != 0xB && mLife != 0) {
            gGameRoot_8053AAF8->fn_80138B7C();
        }
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
    }
}

void ChiefSoldier_804A0B90::vfunc_0x44() {
    if (field_0x90 == 2 || field_0x90 == 3) {
        field_0x90 += 2;
        field_0x240 = 0;
        field_0x264 = 0;
        field_0x250 = 0x14;
        if (field_0x23c == 0 && gGameRoot_8053AAF8->field_0x7f == 1) {
            gAudioMgr_8053AF80->fn_80458FF0(0x1000021, false, false);
        }
        field_0x288 = 2;
        if (field_0x2a5 == 1) {
            startSound_801F0E34(0x5001F, 0);
        }
        field_0x230 = 2;
    }
    if (getType() != 7 && getType() != 8 && mLife <= 0) {
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->mAlive = false;
            }
        }
        field_0x238 = -1;
    }
}

// FIXME(shotgun): 91.66 %, missing/extra + reloc-name; behaviour near.
void ChiefSoldier_804A0B90::fn_80174220() {
    s32 dir = field_0x98;
    s32 index = dir;
    if (getType() == 8 && field_0x23c != 0) {
        index = dir + 4;
    }
    f32 x = mPos.x;
    f32 y = mPos.y;
    f32 z = mPos.z;
    if (index < 4) {
        mPos.x += lbl_804A0AD8[dir][0];
        mPos.y += lbl_804A0AD8[field_0x98][1];
    }
    mBox.i.x = lbl_804A0B10[index][0];
    mBox.i.y = lbl_804A0B10[index][1];
    mBox.f.x = lbl_804A0B10[index][2];
    mBox.f.y = lbl_804A0B10[index][3];
    if (field_0x94 == 0 || field_0x94 == 2) {
        s32 serial = field_0x238;
        if (serial >= 0) {
            ActorBase_804A5770* partner = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (partner != NULL && partner->field_0xd8 == 0) {
                if (field_0x94 == 0) {
                    fn_801F2B7C(field_0x114, -1, 1);
                } else {
                    fn_801F2B7C(4, -1, 3);
                }
            }
        }
        mLife = 10000;
        fn_801EF510();
        for (s32 i = 0; i < 4; i++) {
            s32 player = fn_801F285C(NULL, NULL, true, i);
            if (player < 0) {
                continue;
            }
            if (fn_80235448(player)) {
                continue;
            }
            f32 y0 = mBox.i.y + mPos.y;
            f32 y1 = mBox.f.y + mPos.y;
            f32 x0 = mBox.i.x + mPos.x;
            f32 x1 = mBox.f.x + mPos.x;
            JGeometry::TVec2f center(0.5f * (x0 - x1) + x1, 0.5f * (y0 - y1) + y1);
            if (fn_80237948(player, &center)) {
                fn_80230FD0(player, 0x20002);
                serial = field_0x238;
                if (serial >= 0) {
                    ActorBase_804A5770* partner = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (partner != NULL) {
                        partner->field_0x94 = player + 1;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x473, 0, 1, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x474, 0, 1, NULL, NULL, true, 1.0f);
                    }
                }
            }
        }
    }
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}

void ChiefSoldier_804A0B90::fn_80172BC8() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x90 != 7) {
        setScaleMtx(mtx, 2.0f);
        if (field_0x23c == 0) {
            setScaleMtx(mtx, 1.0f);
        }
        params.mMtx = mtx;
        s32 frame = field_0x280 + 0x3FD;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_gc_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f, 1.0f);
        }
    }
}

void ChiefSoldier_804A0B90::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (lbl_8053AB60 != 1) {
        if (getType() == 7 || getType() == 8) {
            fn_80172BC8();
        } else {
            s32 hasMtx = 0;
            params.field_0x0[1] = field_0x234;
            if (field_0x230 == 6 && field_0x264 >= 3) {
                params.field_0x30 = 0x11;
            }
            if (field_0x4 >= 8) {
                if (field_0x23c == 1) {
                    setScaleMtx(mtx, 2.0f);
                    hasMtx = 1;
                    params.mMtx = mtx;
                }
                JGeometry::TBox2f box;
                box = getGameView_802D6A9C()->mVisibleBox;
                JGeometry::TVec3f pos;
                pos.set(mPos);
                pos.x += box.i.x;
                pos.y += box.i.y;
                if (getType() != 6) {
                    fn_801EF574("enemy_gc_sob.bin", field_0x280 + 0x226, &pos, &params);
                }
                fn_803075AC(mSerial, field_0x4, &mPos, field_0x280 + 0x318, hasMtx != 0, 2, -1, 0, 0, 0);
            } else {
                fn_801F02BC("enemy_gc_sob.bin", field_0x280 + 0x226, &params, 7, 0, false, 1.0f,
                            1.0f);
            }
        }
    }
}

void ChiefSoldier_804A0B90::vfunc_0x24() {
    if (getType() == 6) {
        field_0x18c = 0;
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &mPos, 0, -1, -1);
    }
    ActorBase_804A5770::vfunc_0x24();
}

// FIXME(shotgun): update (fn_80172DC4, 0x1450 bytes) is not written.
