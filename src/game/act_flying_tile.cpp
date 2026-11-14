#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/GameGlobals.h"
#include "game/act/act_flying_tile.h"

// FIXME(shotgun): 99 %, scheduling + reloc-name
void FlyingTile_804AC100::init() {
    field_0x244 = field_0x90 & 0x7FF;
    field_0x250 = (field_0x90 >> 11) & 0x1F;
    field_0x24c = (field_0x90 >> 16) & 0xF;
    field_0x254 = (field_0x90 >> 20) & 0xF;
    field_0x264 = ((field_0x90 >> 24) & 7) * 5 + 15;
    mLife = 0;
    field_0x27c = 0.0f;
    field_0x280 = 0.0f;
    field_0x284 = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x234 = 0;
    field_0x260 = 0;
    field_0x288 = NULL;
    field_0x238 = 0;
    field_0x23c = 5;
    field_0x25a = 0;
    field_0x240 = 0x8000;
    field_0x258 = 0;
    field_0x114 = 1;
    field_0x268 = 1;
    field_0x270 = field_0x244 * 60 + 60;
    field_0x248 = field_0x244;
    field_0x278 = 0;
    field_0x26c = 0xFFF;
    field_0x274 = 0;
    if (field_0x250 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 4;
    }
    switch (field_0x24c) {
    case 0:
    case 5:
    case 8:
        mPos += JGeometry::TVec3f(8.0f, 8.0f, 0.0f);
        field_0x80.set(-3.5f, -3.5f, 3.5f, 3.5f);
        mBox.set(-3.5f, -3.5f, 3.5f, 3.5f);
        field_0x25c = 0x1000;
        field_0x114 = 2;
        break;
    case 1:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x1000;
        field_0x114 = 2;
        break;
    case 2:
        mPos += JGeometry::TVec3f(0.0f, 16.0f, 0.0f);
        field_0x80.set(-30.0f, -30.0f, 30.0f, 30.0f);
        mBox.set(-30.0f, -30.0f, 30.0f, 30.0f);
        field_0x25c = 0x800;
        field_0x288 = getGameView_802D6A9C()->fn_802D9C4C();
        field_0x114 = 2;
        break;
    case 3:
        field_0x80.set(-40.0f, -40.0f, 40.0f, 40.0f);
        mBox.set(-40.0f, -40.0f, 40.0f, 40.0f);
        field_0x25c = 0x400;
        field_0x288 = getGameView_802D6A9C()->fn_802D9C4C();
        field_0x114 = 4;
        field_0x268 = 2;
        break;
    case 4:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x800;
        field_0x114 = 4;
        break;
    case 6:
    case 9:
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-14.0f, -14.0f, 14.0f, 14.0f);
        field_0x25c = 0x800;
        field_0x114 = 4;
        break;
    case 7:
        mPos += JGeometry::TVec3f(8.0f, 8.0f, 0.0f);
        field_0x80.set(-7.0f, -7.0f, 7.0f, 7.0f);
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
        field_0x25c = 0x800;
        field_0x114 = 2;
        break;
    }
}

// FIXME(shotgun): 100 %, matches
void FlyingTile_804AC100::vfunc_0x10() {
    s32 serial = -1;
    s32 best = 0;
    for (;;) {
        serial = getActorManager_801F856C()->fn_801F78F4(ACTOR_FLYO, serial);
        if (serial < 0) {
            break;
        }
        FlyingTile_804AC100* tile =
            static_cast<FlyingTile_804AC100*>(getActorManager_801F856C()->findActorBySerial_801F666C(serial));
        if (tile == NULL) {
            continue;
        }
        s32 value;
        if (tile->field_0x230 == 0 || tile->field_0x230 == 4) {
            value = tile->field_0x248;
        } else {
            value = 0xFFF;
        }
        if (value < field_0x26c) {
            field_0x26c = value;
        }
        if (best < value && value < field_0x244) {
            best = value;
        }
    }
    if (field_0x26c == field_0x244) {
        field_0x278 = 1;
    }
    if (best != 0) {
        field_0x274 = field_0x244 - best;
    }
}

// FIXME(shotgun): 95 %, missing/extra
bool FlyingTile_804AC100::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL) == true) {
        return true;
    }
    s32 code = fn_8022461C(pos, layer);
    if (code == 0 || code == 2 || code == 0x50) {
        return false;
    }
    if (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0) {
        return false;
    }
    if ((u16)code == 0x39) {
        return false;
    }
    if ((u16)code == 0x25 && getGameView_802D6A9C()->field_0x171 == 0) {
        return false;
    }
    if ((u16)code == 4) {
        if (((s32)pos->y & 0xF) >= 8) {
            return false;
        }
    } else if ((u16)code == 5) {
        if (((s32)pos->y & 0xF) < 8) {
            return false;
        }
    } else if ((u16)code == 6) {
        if (((s32)pos->x & 0xF) >= 8) {
            return false;
        }
    } else if ((u16)code == 7) {
        if (((s32)pos->x & 0xF) < 8) {
            return false;
        }
    } else if ((u16)code == 3 || (u16)code == 0xE || (u16)code == 0xD) {
        return false;
    }
    return true;
}

void FlyingTile_804AC100::update() {
    u16 angle = 0;
    u8 hitWall = 0;
    s32 player;
    s32 flag;
    addCalc2_801F71A4(&field_0x3c.x, field_0x27c, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x280, 0.3f, 1.0f);
    mPos.add(field_0x3c);
    if (field_0x230 == 2 && field_0x238 >= field_0x264) {
        hitWall = fn_801F2618(NULL, false);
    }
    switch (field_0x230) {
    case 0:
        if (field_0x238 >= field_0x270) {
            field_0x230 = 1;
            switch (field_0x24c) {
            case 0:
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x276);
                break;
            case 8:
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0xD);
                break;
            case 1:
                if (fn_8022461C(&mPos, field_0x4) != 0x21) {
                    mAlive = false;
                } else {
                    field_0x260 = fn_80226850(&mPos, field_0x4);
                    gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x79);
                }
                break;
            case 2: {
                JGeometry::TVec3f pos(mPos.x - 16.0f, mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_8029D8F0(&pos, field_0x4, 1, 0);
                break;
            }
            case 3:
                for (s32 i = 0; i < 6; i++) {
                    for (s32 j = 0; j < 6; j++) {
                        JGeometry::TVec3f pos(mPos.x - 48.0f + 16.0f * i, mPos.y - 48.0f + 16.0f * j, 0.0f);
                        gBgRewrite_8053AC98->fn_802A381C(&pos, field_0x4, 0x1B0);
                        JGeometry::TVec3f pos2(mPos.x - 48.0f + 16.0f * i, mPos.y - 48.0f + 16.0f * j, 0.0f);
                        gBgRewrite_8053AC98->fn_802A381C(&pos2, field_0x4 + 1, 0);
                    }
                }
                break;
            case 4: {
                JGeometry::TVec3f pos0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos0, field_0x4, 0x272);
                JGeometry::TVec3f pos1(8.0f + mPos.x, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos1, field_0x4, 0x27D);
                JGeometry::TVec3f pos2(mPos.x - 8.0f, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos2, field_0x4, 0x275);
                JGeometry::TVec3f pos3(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos3, field_0x4, 0x27A);
                break;
            }
            case 6: {
                JGeometry::TVec3f pos0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos0, field_0x4, 0x273);
                JGeometry::TVec3f pos1(8.0f + mPos.x, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos1, field_0x4, 0x27C);
                JGeometry::TVec3f pos2(mPos.x - 8.0f, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos2, field_0x4, 0x274);
                JGeometry::TVec3f pos3(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos3, field_0x4, 0x27B);
                break;
            }
            case 9: {
                JGeometry::TVec3f pos0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos0, field_0x4, 0x361);
                JGeometry::TVec3f pos1(8.0f + mPos.x, mPos.y - 8.0f, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos1, field_0x4, 0x361);
                JGeometry::TVec3f pos2(mPos.x - 8.0f, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos2, field_0x4, 0x361);
                JGeometry::TVec3f pos3(8.0f + mPos.x, 8.0f + mPos.y, 0.0f);
                gBgRewrite_8053AC98->fn_802A381C(&pos3, field_0x4, 0x361);
                break;
            }
            case 5:
                field_0x260 = fn_80226850(&mPos, field_0x4);
                if (field_0x260 < 0x40 || field_0x260 > 0x49) {
                    mAlive = false;
                } else {
                    gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x11);
                }
                break;
            case 7: {
                s32 serial = fn_801F2718(ACTOR_TKRA);
                if (serial >= 0) {
                    ActorBase_804A5770* chest = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (chest != NULL) {
                        chest->mAlive = false;
                    }
                }
                if (fn_80226850(&mPos, field_0x4) == 0x10) {
                    field_0x258 = 1;
                }
                gBgRewrite_8053AC98->fn_802A381C(&mPos, field_0x4, 0x9E);
                break;
            }
            }
            field_0x238 = 0;
            field_0x3c.z = 1.0f;
        } else {
            switch (field_0x24c) {
            case 1:
                if (fn_8022461C(&mPos, field_0x4) != 0x21) {
                    if (vfunc_0x3C() > 0) {
                        vfunc_0x30(0xFF, true);
                    }
                    mAlive = false;
                    return;
                }
                break;
            case 5:
                field_0x260 = fn_80226850(&mPos, field_0x4);
                if (field_0x260 < 0x40 || field_0x260 > 0x49) {
                    if (vfunc_0x3C() > 0) {
                        vfunc_0x30(0xFF, true);
                    }
                    mAlive = false;
                    return;
                }
                break;
            }
            field_0x248 = field_0x244;
            if (field_0x278 == 0) {
                s32 first = 0xFFF;
                s32 end = getActorManager_801F856C()->mActorEnd;
                for (s32 i = 0; i < end; i++) {
                    ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                    if (actor != NULL && actor->mActorId == ACTOR_FLYO) {
                        FlyingTile_804AC100* tile = static_cast<FlyingTile_804AC100*>(actor);
                        if (tile != NULL && tile->getOrder() <= first) {
                            first = tile->getOrder();
                        }
                    }
                }
                if (first == field_0x244) {
                    field_0x244 = field_0x26c + field_0x274;
                    field_0x270 = field_0x244 * 60 + 60;
                    field_0x278 = 1;
                }
                field_0x26c = first;
            }
        }
        break;
    case 1:
        if (field_0x238 >= 90) {
            startSound_801F0E34(0x30040, 0);
            field_0x230 = 2;
            field_0x25a += field_0x25c;
            field_0x238 = 0;
            fn_801EEC98(mSerial, &angle, NULL, NULL, &player, 0, 0);
            if (JMath::sincosTable_.cosShort(angle) > 0.0f) {
                field_0x264 += 10;
            }
        } else if (field_0x238 >= 30) {
            field_0x25a += field_0x25c;
            field_0x3c.z = 0.0f;
        } else {
            field_0x25a += field_0x25c * field_0x238 / 30;
            field_0x80.i.y -= field_0x3c.z;
            field_0x80.f.y -= field_0x3c.z;
        }
        break;
    case 2:
        field_0x25a += field_0x25c;
        switch (field_0x24c) {
        case 0:
        case 1:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            if (field_0x238 <= 2) {
                mPos.y -= mPos.z;
                fn_801EEC98(mSerial, &angle, NULL, NULL, &player, 0, 0);
                mPos.y += mPos.z;
                field_0x27c = 4.0f * JMath::sincosTable_.sinShort(angle);
                field_0x280 = 4.0f * JMath::sincosTable_.cosShort(angle);
            }
            break;
        case 2:
        case 3:
            if (field_0x238 % 20 == 1 && field_0x238 <= 61) {
                mPos.y -= mPos.z;
                fn_801EEC98(mSerial, &angle, NULL, NULL, &player, 0, 0);
                mPos.y += mPos.z;
                field_0x27c = 2.0f * JMath::sincosTable_.sinShort(angle);
                field_0x280 = 2.0f * JMath::sincosTable_.cosShort(angle);
            }
            break;
        }
        break;
    case 3:
        if (vfunc_0x3C() > 0) {
            vfunc_0x30(0xFF, true);
        }
        switch (field_0x24c) {
        case 0:
        case 8:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x7E, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x7F, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30018, 0);
            break;
        case 1:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x357, 0, 0, NULL, NULL, true, 0.15f);
            startSound_801F0E34(0x30002, 0);
            fn_80339820();
            break;
        case 2:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAD, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAF, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xB0, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30003, 0);
            break;
        case 3:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x82, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x83, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x84, 1, 0, NULL, NULL, true, 1.0f);
            if (field_0x288[0] == 'r' || field_0x288[0] == 's') {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x80, 1, 0, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x81, 1, 0, NULL, NULL, true, 1.0f);
            }
            startSound_801F0E34(0x30003, 0);
            break;
        case 4:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xA7, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xA8, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30003, 0);
            break;
        case 5:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAB, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0xAC, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x3000B, 0);
            fn_80339820();
            break;
        case 6:
        case 9:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x99, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x9A, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30003, 0);
            break;
        case 7:
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x78, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &mPos, 0x79, 1, 0, NULL, NULL, true, 1.0f);
            startSound_801F0E34(0x30018, 0);
            break;
        }
        if (field_0x254 == 1) {
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &mPos, 4);
            if (serial >= 0) {
                ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (gem != NULL) {
                    gem->field_0x3c = JGeometry::TVec3f(0.0f, 0.0f, 3.0f);
                }
            }
        }
        mAlive = false;
        break;
    case 4:
        if (getGameView_802D6A9C()->fn_802D800C(field_0x250)) {
            field_0x230 = 0;
            field_0x238 = 0;
        }
        break;
    }
    if (field_0x230 != 0 && field_0x230 != 4) {
        JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
        field_0x240 += field_0x25c;
        if (field_0x240 >= 0x8000) {
            field_0x240 -= 0x8000;
            field_0x23c--;
            if (field_0x23c <= 0) {
                field_0x23c = 1;
                startSound_801F0E34(0x3003F, 0);
            }
        }
    }
    if (field_0x230 == 1 || field_0x230 == 2) {
        if (fn_801F6758(&mPos, field_0x4, 16.0f)) {
            mAlive = false;
        } else if (hitWall || fn_801F2B7C(field_0x114, -1, field_0x268)) {
            field_0x230 = 3;
        } else {
            for (s32 i = 0; i < 4; i++) {
                if (i == fn_801F285C(NULL, &flag, true, -1)) {
                    if (flag == 1) {
                        fn_802359A4(fn_8022F340(i), fn_802357E0(fn_8022F340(i)));
                        getActorManager_801F856C()->fn_801F6718(fn_8022F340(i), field_0x4, &mPos,
                                                                fn_802357E0(fn_8022F340(i)), 0);
                    }
                    fn_801F37AC(fn_8023B6CC(i), getPlayerPos_8023E724(i), 0, flag, i);
                    field_0x230 = 3;
                    break;
                }
            }
        }
    }
    if (field_0x230 != 4) {
        field_0x238++;
    }
}

// Draws the 'SMP ' sprite `name` at pos with params, below layer 8.
static inline void drawSmp(FlyingTile_804AC100* self, const JGeometry::TVec3f* pos, const char* name,
                           SpriteParams_801F5AE0* params) {
    if (self->field_0x4 < 8) {
        void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', name);
        gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(pos, res, params);
    }
}

// Turns params into the tile's shadow: dark, fading with the height.
static inline void setShadow(FlyingTile_804AC100* self, SpriteParams_801F5AE0* params) {
    params->field_0x1d = 0;
    params->field_0x14.r = 0;
    params->field_0x10.r = 0x28;
    params->field_0x10.g = 0x28;
    params->field_0x10.b = 0x28;
    f32 alpha = 223.0f - 0.5f * self->mPos.z;
    params->field_0x10.a = 159.0f >= alpha ? 159.0f : alpha;
}

// Sets mtx to a rotation by angle about the view axis.
static inline void rotZ(Mtx mtx, s16 angle) {
    f32 s = JMath::sincosTable_.sinShort(angle);
    f32 c = JMath::sincosTable_.cosShort(angle);
    mtx[0][0] = c;
    mtx[0][1] = -s;
    mtx[1][0] = s;
    mtx[1][1] = c;
    mtx[2][2] = 1.0f;
    mtx[2][1] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[0][2] = 0.0f;
}

// Sets mtx to a rotation by angle about the vertical axis.
static inline void rotY(Mtx mtx, s16 angle) {
    f32 s = JMath::sincosTable_.sinShort(angle);
    f32 c = JMath::sincosTable_.cosShort(angle);
    mtx[0][0] = c;
    mtx[1][1] = 1.0f;
    mtx[0][2] = s;
    mtx[2][0] = -s;
    mtx[2][2] = c;
    mtx[2][1] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[0][1] = 0.0f;
}

void FlyingTile_804AC100::draw() {
    SpriteParams_801F5AE0 params;
    Mtx rot;
    Mtx trans;
    PSMTXIdentity(rot);
    PSMTXIdentity(trans);
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    if (field_0x230 != 0 && field_0x230 != 4) {
        if (field_0x254 == 1) {
            params.field_0x14.r = 0x96;
        }
        switch (field_0x24c) {
        case 0:
        case 8: {
            PSMTXTrans(trans, -8.0f, -8.0f, 0.0f);
            rotZ(rot, field_0x25a);
            PSMTXConcat(rot, trans, rot);
            params.mMtx = rot;
            JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
            drawSmp(this, &pos, "floor1_smp.bin", &params);
            setShadow(this, &params);
            JGeometry::TVec3f flat(mPos.x, mPos.y, 0.0f);
            drawSmp(this, &flat, "floor1_smp.bin", &params);
            break;
        }
        case 1:
            rotY(rot, field_0x25a);
            params.mMtx = rot;
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x27, &params, 7, 1.0f, 1.0f);
            break;
        case 2:
            rotY(rot, field_0x25a);
            params.mMtx = rot;
            if (field_0x288[0] == 'r' || field_0x288[0] == 's' || field_0x288[0] == 'y') {
                fn_801EFD34(gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_ura.bti"), &params, 7, 0.0f,
                            -16.0f, 0.0f);
            } else if (field_0x288[0] == 'l' || field_0x288[0] == 'x' || field_0x288[0] == 'p') {
                fn_801EFD34(gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_yasi.bti"), &params, 7, 0.0f,
                            -16.0f, 0.0f);
            } else {
                fn_801EFD34(gGameRoot_8053AAF8->field_0xa4->getResource('TIMG', "wood_omote.bti"), &params, 7, 0.0f,
                            -16.0f, 0.0f);
            }
            break;
        case 3: {
            PSMTXTrans(trans, -48.0f, -48.0f, 0.0f);
            rotZ(rot, field_0x25a);
            PSMTXConcat(rot, trans, rot);
            params.mMtx = rot;
            JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
            drawSmp(this, &pos, "house_smp.bin", &params);
            setShadow(this, &params);
            JGeometry::TVec3f flat(mPos.x, mPos.y, 0.0f);
            drawSmp(this, &flat, "house_smp.bin", &params);
            break;
        }
        case 4:
            rotY(rot, field_0x25a);
            params.mMtx = rot;
            drawSprite_801F06F0("enemy_gc_sob.bin", 0x16D, &params, 7, 1.0f, 1.0f);
            break;
        case 5:
            rotY(rot, field_0x25a);
            params.mMtx = rot;
            drawSprite_801F06F0("enemy_both2_sob.bin", 0x21, &params, 7, 1.0f, 1.0f);
            break;
        case 6:
        case 9: {
            PSMTXTrans(trans, -16.0f, -16.0f, 0.0f);
            rotZ(rot, field_0x25a);
            PSMTXConcat(rot, trans, rot);
            params.mMtx = rot;
            JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
            drawSmp(this, &pos, "statue_smp.bin", &params);
            setShadow(this, &params);
            JGeometry::TVec3f flat(mPos.x, mPos.y, 0.0f);
            drawSmp(this, &flat, "statue_smp.bin", &params);
            break;
        }
        case 7: {
            PSMTXTrans(trans, -8.0f, -8.0f, 0.0f);
            rotZ(rot, field_0x25a);
            PSMTXConcat(rot, trans, rot);
            params.mMtx = rot;
            if (field_0x258 != 0) {
                JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
                drawSmp(this, &pos, "empty_smp.bin", &params);
            } else {
                JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
                drawSmp(this, &pos, "treasure_smp.bin", &params);
            }
            setShadow(this, &params);
            if (field_0x258 != 0) {
                JGeometry::TVec3f flat(mPos.x, mPos.y, 0.0f);
                drawSmp(this, &flat, "empty_smp.bin", &params);
            } else {
                JGeometry::TVec3f flat(mPos.x, mPos.y, 0.0f);
                drawSmp(this, &flat, "treasure_smp.bin", &params);
            }
            break;
        }
        }
    }
}

// FIXME(shotgun): 86 %, missing/extra
void FlyingTile_804AC100::fn_80339820() {
    JGeometry::TVec3f pos = mPos;
    pos.y -= pos.z;
    pos.z = 0.0f;
    if (field_0x24c == 5) {
        if (field_0x260 >= 0x45) {
            field_0x260 -= 0x45;
        } else {
            field_0x260 -= 0x40;
        }
    } else if (field_0x24c == 1) {
        switch (field_0x260) {
        case 0x75:
            field_0x260 = 2;
            break;
        case 0x6B:
            field_0x260 = 4;
            break;
        default:
            field_0x260 = 1;
            break;
        }
    }
    if (field_0x260 <= 1) {
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
            if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.8f) {
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &pos, 0x261400, -1, -1);
            } else {
                bool skip = false;
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    skip = true;
                }
                if (!skip) {
                if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FARY, field_0x4, &pos, 0, -1, -1);
                } else {
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
                }
                }
            }
        }
    } else if (field_0x260 == 2) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HART, field_0x4, &pos, 0, -1, -1);
    } else if (field_0x260 != 3 && field_0x260 == 4) {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FARY, field_0x4, &pos, 0, -1, -1);
    }
}

FlyingTile_804AC100::~FlyingTile_804AC100() {}
