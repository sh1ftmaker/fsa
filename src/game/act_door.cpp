#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_door.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/cls_804AE9B0.h"
#include "game/act/act_small_key.h"
#include "game/act/act_warp.h"

// FIXME(shotgun): 86.0 %, missing/extra + reloc-name
void Door_804A8D88::init() {
    field_0x258.x = mPos.x;
    field_0x258.y = mPos.y;
    field_0x258.z = mPos.z;
    field_0x234 = field_0x90 & 0x7F;
    field_0x238 = (field_0x90 >> 7) & 0x1F;
    field_0x23c = (field_0x90 >> 12) & 0x1F;
    field_0x240 = (field_0x90 >> 17) & 1;
    field_0x24c = (field_0x90 >> 18) & 1;
    field_0x250 = 0;
    field_0x248 = -1;
    if (field_0x234 == 0) {
        field_0x230 = 0;
    } else {
        field_0x230 = 1;
    }
    mBox.i.x = -8.0f;
    mBox.i.y = -8.0f;
    mBox.f.x = 8.0f;
    mBox.f.y = 8.0f;
    switch (field_0x234) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 8:
        switch (field_0x23c) {
        case 0:
            switch (field_0x238) {
            case 0:
                mPos.x += 12.0f;
                mPos.y += 4.0f;
                break;
            case 1:
            case 2:
                mPos.x += 12.0f;
                mPos.y += 12.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 12.0f;
                break;
            }
            break;
        case 1:
            switch (field_0x238) {
            case 0:
                mPos.x += 28.0f;
                mPos.y += 4.0f;
                break;
            case 1:
                mPos.x += 28.0f;
                mPos.y += 12.0f;
                break;
            case 2:
                mPos.x += 12.0f;
                mPos.y += 28.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 28.0f;
                break;
            }
            break;
        case 2:
            switch (field_0x238) {
            case 0:
                mPos.x += 44.0f;
                mPos.y += 4.0f;
                break;
            case 1:
                mPos.x += 44.0f;
                mPos.y += 12.0f;
                break;
            case 2:
                mPos.x += 12.0f;
                mPos.y += 44.0f;
                break;
            case 3:
                mPos.x += 4.0f;
                mPos.y += 44.0f;
                break;
            }
            break;
        }
        break;
    case 5:
        mBox.i.x = -9.0f;
        mBox.i.y = -9.0f;
        mBox.f.x = 9.0f;
        mBox.f.y = 9.0f;
        mPos.x += 8.0f;
        mPos.y += 8.0f;
        field_0x230 = 2;
        break;
    case 6:
        mBox.i.x = -12.0f;
        mBox.i.y = -12.0f;
        mBox.f.x = 12.0f;
        mBox.f.y = 12.0f;
        mPos.x += 16.0f;
        mPos.y += 16.0f;
        field_0x230 = 2;
        break;
    case 7:
        mBox.i.x = -9.0f;
        mBox.i.y = -9.0f;
        mBox.f.x = 9.0f;
        mBox.f.y = 9.0f;
        mPos.x += 16.0f;
        mPos.y += 8.0f;
        field_0x230 = 2;
        break;
    }
    field_0x264.x = mPos.x;
    field_0x264.y = mPos.y;
    field_0x26c.i.x = mBox.i.x;
    field_0x26c.i.y = mBox.i.y;
    field_0x26c.f.x = mBox.f.x;
    field_0x26c.f.y = mBox.f.y;
    field_0x26c.i.x += field_0x264.x;
    field_0x26c.i.y += field_0x264.y;
    field_0x26c.f.x += field_0x264.x;
    field_0x26c.f.y += field_0x264.y;
    if (field_0x23c == 0 || (field_0x23c == 2 && field_0x234 == 0)) {
        if (field_0x23c == 0 && field_0x238 < 2) {
            JGeometry::TVec2f v(field_0x258.x + 16.0f, field_0x258.y);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        } else if (field_0x23c == 2 && field_0x234 == 0) {
            JGeometry::TVec2f v(field_0x258.x, field_0x258.y);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        } else {
            JGeometry::TVec2f v(field_0x258.x, field_0x258.y + 16.0f);
            field_0x244 = fn_80226BC4(&v, field_0x4);
        }
    } else {
        JGeometry::TVec2f v(field_0x258.x, field_0x258.y + 16.0f);
        field_0x244 = fn_80226BC4(&v, field_0x4);
    }
    if ((field_0x234 >= 1 && field_0x234 <= 4) || field_0x234 == 8) {
        s32 layer = field_0x4;
        bool hit;
        if (layer < 8) {
            hit = getGameView_802D6A9C()->fn_802D49A8(layer, (u16)field_0x244);
        } else {
            hit = getGameView_802D6A9C()->fn_802D4ADC(layer, (u16)field_0x244);
        }
        if (hit == true) {
            field_0x230 = 3;
            update();
            field_0x230 = 5;
        }
    } else if (field_0x234 >= 5) {
        if (getGameView_802D6A9C()->fn_802D4ADC(field_0x4, (u16)field_0x244) == true) {
            field_0x230 = 5;
        }
    } else {
        update();
    }
}


// Plain TVec3 setter that update calls everywhere.
void fn_802C7CB8(JGeometry::TVec3f* v, f32 x, f32 y, f32 z) {
    v->set(x, y, z);
}

// The unit's .rodata (0x8046D598, 0x270 bytes): BG tile pairs per door type, direction and width,
// and the cell offsets of the second layer. FIXME(shotgun): table contents not written as source
// (the original's separate arrays are addressed off the section base).
struct DoorTiles_8046D598 {
    /* 0x000 */ u16 mWidth1[9][4][2];  // [type][dir]
    /* 0x090 */ u16 mWidth2[3][4][4];  // [type][dir]
    /* 0x0F0 */ u16 mWidth3[3][4][6];  // [type][dir]
    /* 0x180 */ u16 mLayer2Type2[4][2]; // [dir], width 1, type 2
    /* 0x190 */ u16 mLayer2Type4[4][2]; // [dir], width 1, type 4
    /* 0x1A0 */ u16 mLayer2Width2[4][4];
    /* 0x1C0 */ u16 mLayer2Width3[4][6];
    /* 0x1F0 */ u16 mOpenWidth1[4][2];
    /* 0x200 */ u16 mOpenWidth2[4][4];
    /* 0x220 */ u16 mOpenWidth3[4][6];
    /* 0x250 */ s32 mLayer2DX[4];
    /* 0x260 */ s32 mLayer2DY[4];
};
extern const DoorTiles_8046D598 lbl_8046D598;

// FIXME(shotgun): 82.59 %, missing/extra + reloc-name. The TVec3f temporaries make MWCC emit the weak
// template ctor __ct<f>__Q29JGeometry8TVec3<f>Ffff right after update: that is the original's
// fn_802C7CB8 (16 bytes at 0x802C7CB8). Follow-up: drop the free fn_802C7CB8 below and rename its
// symbols.txt entry to the ctor (measured: update 82.71 %, ctor matches).
void Door_804A8D88::update() {
    const DoorTiles_8046D598& tiles = lbl_8046D598;
    JGeometry::TVec2f pos(field_0x258.x, field_0x258.y);
    s32 cell = fn_80226BC4(&pos, field_0x4);
    switch (field_0x230) {
    case 0:
    case 3: {
        if (field_0x234 == 5 || field_0x234 == 6 || field_0x234 == 7) {
            if (field_0x230 != 3) {
                break;
            }
            if (field_0x234 == 5) {
                gBgRewrite_8053AC98->fn_802A3948(&field_0x258, 0, field_0x4, 0x9E);
                if (field_0x4 >= 8) {
                    JGeometry::TVec3f p = field_0x258;
                    p.z = 0.0f;
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &p, 0, -1, -1);
                } else {
                    spawnEmitter_8013CC50(field_0x4, &field_0x258, 0x564, 0, 0, NULL, NULL, true, 0.5f);
                    spawnEmitter_8013CC50(field_0x4, &field_0x258, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                }
                startSound_801F0E34(0x30075, 0);
                field_0x230 = 5;
            } else if (field_0x234 == 6) {
                gBgRewrite_8053AC98->fn_8029DF80(cell, field_0x4, 999, 1000, 1001, 1002);
                if (field_0x4 >= 8) {
                    JGeometry::TVec3f p = field_0x258;
                    p.z = 0.0f;
                    getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &p, 0, -1, -1);
                } else {
                    spawnEmitter_8013CC50(field_0x4, &field_0x258, 0x564, 0, 0, NULL, NULL, true, 0.5f);
                    spawnEmitter_8013CC50(field_0x4, &field_0x258, 0x503, 1, 0, NULL, NULL, true, 1.0f);
                }
                startSound_801F0E34(0x30075, 0);
                field_0x230 = 5;
            }
            break;
        }
        if (field_0x230 == 3) {
            s32 bit = getGameView_802D6A9C()->fn_802D4C10(field_0x4, field_0x244);
            if (bit != -1) {
                getGameView_802D6A9C()->fn_802D7D00(field_0x4, bit, true);
            }
        }
        u8 upper = 8;
        if (field_0x4 < 8) {
            upper = 1;
        }
        switch (field_0x23c) {
        case 0: {
            const u16* t = tiles.mWidth1[field_0x234][field_0x238];
            gBgRewrite_8053AC98->fn_8029DEBC(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1]);
            if (field_0x234 == 0 && field_0x230 == 3) {
                if (field_0x238 == 0) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x443, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 1) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y, 0.0f), 0x446, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 2) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x444, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 3) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x445, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
            if (field_0x234 == 2 || field_0x234 == 4) {
                JGeometry::TVec2f pos2(field_0x258.x + tiles.mLayer2DX[field_0x238], field_0x258.y + tiles.mLayer2DY[field_0x238]);
                s32 cell2 = fn_80226BC4(&pos2, field_0x4);
                if (field_0x234 == 2) {
                    const u16* t2 = tiles.mLayer2Type2[field_0x238];
                    gBgRewrite_8053AC98->fn_8029DEBC(field_0x4 + upper, cell2, (field_0x238 >> 1) & 1, t2[0], t2[1]);
                } else {
                    const u16* t2 = tiles.mLayer2Type4[field_0x238];
                    gBgRewrite_8053AC98->fn_8029DEBC(field_0x4 + upper, cell2, (field_0x238 >> 1) & 1, t2[0], t2[1]);
                }
            }
            break;
        }
        case 1: {
            const u16* t = tiles.mWidth2[field_0x234][field_0x238];
            gBgRewrite_8053AC98->fn_8029DD50(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1], t[2], t[3]);
            if (field_0x234 == 0 && field_0x230 == 3) {
                if (field_0x238 == 0) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(32.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x43F, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 1) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(32.0f + field_0x258.x, field_0x258.y, 0.0f), 0x442, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 2) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 32.0f + field_0x258.y, 0.0f), 0x440, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 3) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 32.0f + field_0x258.y, 0.0f), 0x441, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
            if (field_0x234 == 2) {
                JGeometry::TVec2f pos2(field_0x258.x + tiles.mLayer2DX[field_0x238], field_0x258.y + tiles.mLayer2DY[field_0x238]);
                s32 cell2 = fn_80226BC4(&pos2, field_0x4);
                const u16* t2 = tiles.mLayer2Width2[field_0x238];
                gBgRewrite_8053AC98->fn_8029DD50(field_0x4 + upper, cell2, (field_0x238 >> 1) & 1, t2[0], t2[1], t2[2], t2[3]);
            }
            break;
        }
        case 2: {
            const u16* t = tiles.mWidth3[field_0x234][field_0x238];
            gBgRewrite_8053AC98->fn_8029DB2C(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1], t[2], t[3], t[4], t[5]);
            if (field_0x234 == 0 && field_0x230 == 3) {
                if (field_0x238 == 0) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(48.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x43B, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 1) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(48.0f + field_0x258.x, field_0x258.y, 0.0f), 0x43E, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 2) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 48.0f + field_0x258.y, 0.0f), 0x43C, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 3) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 48.0f + field_0x258.y, 0.0f), 0x43D, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
            if (field_0x234 == 2) {
                JGeometry::TVec2f pos2(field_0x258.x + tiles.mLayer2DX[field_0x238], field_0x258.y + tiles.mLayer2DY[field_0x238]);
                cell = fn_80226BC4(&pos2, field_0x4);
                const u16* t2 = tiles.mLayer2Width3[field_0x238];
                gBgRewrite_8053AC98->fn_8029DB2C(field_0x4, cell, (field_0x238 >> 1) & 1, t2[0], t2[1], t2[2], t2[3], t2[4], t2[5]);
                t2 = tiles.mLayer2Width3[field_0x238];
                gBgRewrite_8053AC98->fn_8029DB2C(field_0x4 + upper, cell, (field_0x238 >> 1) & 1, t2[0], t2[1], t2[2], t2[3], t2[4], t2[5]);
            }
            break;
        }
        }
        if (field_0x234 == 8) {
            startSound_801F0E34(0x30006, 0);
        } else {
            startSound_801F0E34(0x30007, 0);
        }
        if ((field_0x230 == 3 && field_0x234 == 2) || field_0x234 == 4) {
            if (field_0x4 >= 8) {
                JGeometry::TVec3f p = field_0x258;
                getActorManager_801F856C()->spawnActor_801F9484('KMRI', field_0x4, &JGeometry::TVec3f(8.0f + p.x, 10.0f + p.y, 0.0f), 0, -1, -1);
            }
        } else if (field_0x234 == 8) {
            fn_801F0D20(3);
        }
        if (field_0x230 == 3 && field_0x234 == 0 && field_0x240 == 0) {
            getActorManager_801F856C()->spawnActor_801F9484('SEN2', field_0x4, &mPos, 0x2800, -1, -1);
        }
        if (field_0x230 == 0) {
            field_0x230 = 1;
        } else if (field_0x234 == 0 && field_0x240 != 0) {
            field_0x230 = 6;
        } else if (field_0x234 == 0 && field_0x4 < 8 && field_0x24c != 0) {
            field_0x230 = 9;
            field_0x250 = 120;
        } else {
            field_0x230 = 5;
        }
        break;
    }
    case 1:
        if (!getGameView_802D6A9C()->fn_802DCD0C() && field_0x234 == 0) {
            field_0x230 = 4;
        } else {
            field_0x230 = 2;
        }
        break;
    case 2:
        switch (field_0x234) {
        case 0:
            if (!vfunc_0x34()) {
                break;
            }
            if (field_0x4 < 8 && field_0x24c != 0 && field_0x240 == 0) {
                field_0x230 = 8;
            } else {
                field_0x230 = 3;
            }
            break;
        case 1:
        case 3:
        case 5:
        case 6:
            field_0x248 = fn_8023AF14(&field_0x26c, field_0x4, false, 0.0f);
            if (field_0x248 < 0) {
                break;
            }
            if (fn_8022CAF8(field_0x248) == 4) {
                if (fn_8022F478(field_0x248) || fn_8023EEB4(field_0x248) != 'KEY0') {
                    break;
                }
                SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(field_0x248)));
                if (key == NULL || (key->field_0x90 & 0x1F)) {
                    break;
                }
                fn_8023EE28(field_0x248);
                key->fn_802C1D90();
                field_0x230 = 0xA;
                break;
            }
            for (s32 i = 0; i < 4; i++) {
                if (i == field_0x248 || (fn_8022F514(i) == true && field_0x248 == fn_8022F3DC(i))) {
                    if (fn_8023EEB4(i) != 'KEY0') {
                        continue;
                    }
                    SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(i)));
                    if (key == NULL || (key->field_0x90 & 0x1F)) {
                        continue;
                    }
                    fn_8023EE28(i);
                    key->fn_802C1D90();
                    field_0x230 = 0xA;
                    break;
                }
            }
            break;
        case 2:
        case 4:
            if (field_0x240 == 0) {
                s32 serial = fn_801F2718('HBMB');
                if (serial < 0) {
                    break;
                }
                ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (bomb->field_0x94 == 0) {
                    break;
                }
                field_0x230 = 3;
                getActorManager_801F856C()->spawnActor_801F9484('SEN2', field_0x4, &mPos, 0x3C00, -1, -1);
                if (field_0x238 == 0) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y, 0.0f), 0x5D, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 1) {
                    if (field_0x23c == 0) {
                        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x60, 0, 0, NULL, NULL, true, 1.0f);
                    } else if (field_0x23c == 2) {
                        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(20.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x60, 0, 0, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(36.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x60, 0, 0, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(52.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x60, 0, 0, NULL, NULL, true, 1.0f);
                        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(68.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x60, 0, 0, NULL, NULL, true, 1.0f);
                    }
                } else if (field_0x238 == 2) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x5E, 0, 0, NULL, NULL, true, 1.0f);
                } else if (field_0x238 == 3) {
                    spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x5F, 0, 0, NULL, NULL, true, 1.0f);
                }
                if (bomb->field_0x90 == 12) {
                    break;
                }
                s32 warpSerial = fn_801F2718('WARP');
                if (warpSerial < 0) {
                    break;
                }
                Warp_804A6DC8* warp = static_cast<Warp_804A6DC8*>(getActorManager_801F856C()->findActorBySerial_801F666C(warpSerial));
                if (warp == NULL) {
                    break;
                }
                const JGeometry::TVec3f* spawnPos = &warp->field_0x238;
                s32 raw = (warp->field_0x90 >> 24) & 7;
                s32 layer = raw + 7;
                if (raw == 0) {
                    layer = raw;
                }
                getActorManager_801F856C()->spawnActor_801F9484('HBMB', layer, spawnPos, 0xC, -1, -1);
            } else if (vfunc_0x34()) {
                field_0x230 = 3;
            }
            break;
        case 7: {
            field_0x248 = fn_8023AF14(&field_0x26c, field_0x4, false, 0.0f);
            if (field_0x248 < 0 || fn_8023EEB4(field_0x248) != 'KEY0') {
                break;
            }
            SmallKey_804A8CD8* key = static_cast<SmallKey_804A8CD8*>(getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(field_0x248)));
            if (key == NULL || (key->field_0x90 & 0xF) != 1) {
                break;
            }
            fn_8023EE28(field_0x248);
            key->fn_802C1DFC();
            bool talk = false;
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                if (fn_802308E0(field_0x248) == true) {
                    talk = true;
                }
            } else if (fn_8023077C() == true) {
                lbl_8053AD70->fn_8037AE30();
                talk = true;
            }
            if (talk == true) {
                field_0x230 = 0xC;
            }
            break;
        }
        case 8: {
            JGeometry::TVec2f p(field_0x258.x, field_0x258.y);
            field_0x248 = fn_8023B8E4(&field_0x26c, field_0x4, &p, true);
            if (field_0x248 >= 0) {
                field_0x230 = 3;
            }
            break;
        }
        }
        break;
    case 4: {
        if (field_0x234 == 0 && vfunc_0x34()) {
            field_0x230 = 6;
            break;
        }
        s32 bit = getGameView_802D6A9C()->fn_802D4C10(field_0x4, field_0x244);
        if (bit != -1) {
            getGameView_802D6A9C()->fn_802D7D00(field_0x4, bit, false);
        }
        switch (field_0x23c) {
        case 0: {
            const u16* t = tiles.mOpenWidth1[field_0x238];
            gBgRewrite_8053AC98->fn_8029DEBC(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1]);
            if (field_0x234 != 0) {
                break;
            }
            if (field_0x238 == 0) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x437, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 1) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y, 0.0f), 0x43A, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 2) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x438, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 3) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x439, 0, 0, NULL, NULL, true, 1.0f);
            }
            break;
        }
        case 1: {
            const u16* t = tiles.mOpenWidth2[field_0x238];
            gBgRewrite_8053AC98->fn_8029DD50(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1], t[2], t[3]);
            if (field_0x234 != 0) {
                break;
            }
            if (field_0x238 == 0) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(32.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x433, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 1) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(32.0f + field_0x258.x, field_0x258.y, 0.0f), 0x436, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 2) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 32.0f + field_0x258.y, 0.0f), 0x434, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 3) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 32.0f + field_0x258.y, 0.0f), 0x435, 0, 0, NULL, NULL, true, 1.0f);
            }
            break;
        }
        case 2: {
            const u16* t = tiles.mOpenWidth3[field_0x238];
            gBgRewrite_8053AC98->fn_8029DB2C(field_0x4, cell, (field_0x238 >> 1) & 1, t[0], t[1], t[2], t[3], t[4], t[5]);
            if (field_0x234 != 0) {
                break;
            }
            if (field_0x238 == 0) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(48.0f + field_0x258.x, 16.0f + field_0x258.y, 0.0f), 0x42F, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 1) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(48.0f + field_0x258.x, field_0x258.y, 0.0f), 0x432, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 2) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, 48.0f + field_0x258.y, 0.0f), 0x430, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 3) {
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 48.0f + field_0x258.y, 0.0f), 0x431, 0, 0, NULL, NULL, true, 1.0f);
            }
            break;
        }
        }
        startSound_801F0E34(0x30008, 0);
        field_0x230 = 2;
        break;
    }
    case 5:
        mAlive = false;
        break;
    case 6:
        switch (field_0x234) {
        case 0:
            if (!vfunc_0x34()) {
                field_0x230 = 4;
            }
            break;
        }
        break;
    case 7: {
        if (field_0x248 < 0) {
            field_0x230 = 3;
            break;
        }
        if (fn_802308E0(field_0x248) != true) {
            break;
        }
        s32 leader = field_0x248;
        if (fn_8022F514(field_0x248)) {
            leader = fn_8022F3DC(field_0x248);
        }
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f player = *getPlayerPos_8023E724(i);
            JGeometry::TVec3f p = player;
            p.x = 1.0f + player.x;
            p.y = player.y - 12.0f;
            p.z = player.z;
            if (fn_8022F514(i) == true && leader == fn_8022F3DC(i)) {
                if (i == field_0x248) {
                    getActorManager_801F856C()->spawnActor_801F9484('IMUP', field_0x4, &p, 0x800004 + (i << 24) + (field_0x248 << 26), -1, -1);
                }
            } else if (i == leader && i == field_0x248) {
                getActorManager_801F856C()->spawnActor_801F9484('IMUP', field_0x4, &p, 0x800004 + (i << 24) + (field_0x248 << 26), -1, -1);
            }
        }
        field_0x230 = 3;
        break;
    }
    case 8:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (gGameRoot2_8053AB10->field_0x24->fn_8030C2A8(&mPos) == true) {
            field_0x230 = 3;
        }
        break;
    case 9:
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        if (field_0x250 == 0) {
            field_0x230 = 5;
        } else {
            field_0x250--;
        }
        break;
    case 0xA:
        if (field_0x234 == 5) {
            spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(8.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B3, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(8.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B4, 0, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(8.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B5, 0, 0, NULL, NULL, true, 1.0f);
        } else {
            f32 offset = 0.0f;
            if (field_0x238 == 0) {
                if (field_0x23c == 0) {
                    offset = 16.0f;
                } else if (field_0x23c == 1) {
                    offset = 32.0f;
                } else if (field_0x23c == 2) {
                    offset = 48.0f;
                }
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, 16.0f + field_0x258.y, 0.0f), 0x4AA, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, 16.0f + field_0x258.y, 0.0f), 0x4AB, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, 16.0f + field_0x258.y, 0.0f), 0x4AC, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 1) {
                if (field_0x23c == 0) {
                    offset = 16.0f;
                } else if (field_0x23c == 1) {
                    offset = 32.0f;
                } else if (field_0x23c == 2) {
                    offset = 48.0f;
                }
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, field_0x258.y, 0.0f), 0x4B3, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, field_0x258.y, 0.0f), 0x4B4, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x + offset, field_0x258.y, 0.0f), 0x4B5, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 2) {
                if (field_0x23c == 0) {
                    offset = 16.0f;
                } else if (field_0x23c == 1) {
                    offset = 32.0f;
                } else if (field_0x23c == 2) {
                    offset = 48.0f;
                }
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, field_0x258.y + offset, 0.0f), 0x4AD, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, field_0x258.y + offset, 0.0f), 0x4AE, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(field_0x258.x, field_0x258.y + offset, 0.0f), 0x4AF, 0, 0, NULL, NULL, true, 1.0f);
            } else if (field_0x238 == 3) {
                if (field_0x23c == 0) {
                    offset = 16.0f;
                } else if (field_0x23c == 1) {
                    offset = 32.0f;
                } else if (field_0x23c == 2) {
                    offset = 48.0f;
                }
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y + offset, 0.0f), 0x4B0, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y + offset, 0.0f), 0x4B1, 0, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y + offset, 0.0f), 0x4B2, 0, 0, NULL, NULL, true, 1.0f);
            }
        }
        field_0x254 = 17;
        field_0x230 = 0xB;
        break;
    case 0xB:
        field_0x254--;
        if (field_0x254 != 0) {
            break;
        }
        field_0x230 = 3;
        startSound_801F0E34(0x30009, 0);
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            field_0x230 = 7;
        }
        break;
    case 0xC:
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B3, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B4, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 4.0f + field_0x258.y, 0.0f), 0x4B5, 0, 0, NULL, NULL, true, 1.0f);
        field_0x254 = 17;
        field_0x230 = 0xD;
        break;
    case 0xD:
        field_0x254--;
        if (field_0x254 != 0) {
            break;
        }
        gBgRewrite_8053AC98->fn_802A3948(&field_0x258, 0, field_0x4, 0xBC);
        gBgRewrite_8053AC98->fn_802A3948(&JGeometry::TVec3f(16.0f + field_0x258.x, field_0x258.y, field_0x258.z), 0, field_0x4, 0xBD);
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 8.0f + field_0x258.y, 0.0f), 0xCD, 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 8.0f + field_0x258.y, 0.0f), 0xCE, 0, 1, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(16.0f + field_0x258.x, 8.0f + field_0x258.y, 0.0f), 0xCF, 0, 1, NULL, NULL, true, 1.0f);
        startSound_801F0E34(0x30076, 0);
        field_0x254 = 15;
        field_0x230 = 0xE;
        break;
    case 0xE:
        if (field_0x254 == 0) {
            bool done = false;
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                if (fn_8023061C(field_0x248, false) == true) {
                    done = true;
                }
            } else if (fn_80230534(false) == true) {
                done = true;
            }
            if (done == true) {
                getActorManager_801F856C()->spawnActor_801F9484('TKRA', field_0x4, &mPos, (field_0x248 << 12) + 0x208, -1, -1);
                field_0x230 = 5;
            }
        } else {
            field_0x254--;
        }
        break;
    }
}

void Door_804A8D88::draw() {}

void Door_804A8D88::fn_802C59B0(const JGeometry::TVec3f* delta) {
    field_0x258.x += delta->x;
    field_0x258.y += delta->y;
}

Door_804A8D88::~Door_804A8D88() {}
