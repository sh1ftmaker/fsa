#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/ActorManager.h"
#include "game/act/tu_8017A44C.h"

// .rodata: the actor ids fn_8020D6AC skips, ended by 'NULL'.
static const u32 lbl_80466880[5] = {'TOMI', 'BMST', 'IPOT', 'IBBM', 'NULL'};

// .data: the 3-entry-per-direction frame tables update() reads (index
// field_0x98 * 3 + field_0x248).
s32 lbl_804A10A0[24] = {3, 4, 0, 8, 9, 5, 0xD, 0xE, 0xA, 0x12, 0x13, 0xF, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
s32 lbl_804A1100[24] = {0, 1, 2, 5, 6, 7, 0xA, 0xB, 0xC, 0xF, 0x10, 0x11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

cls_804A1160::~cls_804A1160() {}

void cls_804A1160::draw() {
    SpriteParams_801F5AE0 params;
    if (field_0x230 == 1 || field_0x230 == 2) {
        if (field_0x240 == 0) {
            return;
        }
        params.field_0x1d = 1;
        params.field_0x30 = 0x11;
    }
    s32 frame = field_0x254 + 0x12d;
    if (field_0x4 < 8) {
        void* res = lbl_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
        lbl_8053AB10->field_0x20->fn_802F86CC(&mPos, res, frame, &params, 1.0f,
                                              1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x254 + 0x12d, false, 2, -1, 0, 0, 0);
}

// The hit effect at mPos: spawnEmitter_8013CC50 on the low layers, else the HTMK actor.
static inline void spawnHitEffect(s32 layer, const JGeometry::TVec3f* pos) {
    if (layer < 8) {
        spawnEmitter_8013CC50(layer, pos, 0x339, 0, 1, NULL, NULL, true, 1.0f);
    } else {
        getActorManager_801F856C()->spawnActor_801F9484('HTMK', layer, pos, 0, -1, -1);
    }
}

void cls_804A1160::update() {
    mPos += field_0x3c;

    if (field_0x25c == 1) {
        s16 angle = fn_801F6874(field_0x98);
        angle += 0x2000;
        const JMath::TSinCosTable<11, f32>& sincos = JMath::sincosTable_;
        for (s32 i = 0; i < 2; i++) {
            JGeometry::TVec3f pos;
            pos.set(mPos);
            pos.x += 8.0f * sincos.sinShort(angle);
            pos.y += 8.0f * sincos.cosShort(angle);
            gBgRewrite_8053AC98->fn_802A20F0(&pos, field_0x258, field_0x4, 0, 0);
            angle -= 0x4000;
        }
    }

    bool moved = fn_801F2618(NULL, false);
    if (field_0x230 != 2 && (moved || fn_801F2B20(2))) {
        if (!moved) {
            mAlive = false;
            return;
        }
        spawnHitEffect(field_0x4, &mPos);
        field_0x240 = 0;
        field_0x244 = 0x24;
        field_0x230 = 2;
        return;
    }

    switch (field_0x230) {
    case 0:
        if (field_0x23c == 0) {
            field_0x23c = 2;
            field_0x248 = field_0x248 + 1;
            if (field_0x248 > 2) {
                field_0x23c = 1;
                field_0x248 = 0;
                field_0x240 = 100;
                field_0x244 = 0x88;
                switch (field_0x98) {
                case 0:
                    field_0x3c.x = 5.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 0.0f;
                    break;
                case 1:
                    field_0x3c.x = -5.0f;
                    field_0x3c.y = 0.0f;
                    field_0x3c.z = 0.0f;
                    break;
                case 2:
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = 5.0f;
                    field_0x3c.z = 0.0f;
                    break;
                case 3:
                    field_0x3c.x = 0.0f;
                    field_0x3c.y = -5.0f;
                    field_0x3c.z = 0.0f;
                    break;
                }
                field_0x230 = 1;
            }
        }
        break;
    case 1:
        if (fn_801F2A10(field_0x258, field_0x98, true, true)) {
            spawnHitEffect(field_0x4, &mPos);
            mAlive = false;
            return;
        }
        if (field_0x23c == 0) {
            field_0x23c = 2;
            field_0x248 = field_0x248 + 1;
            if (field_0x248 > 2) {
                field_0x248 = 0;
            }
        }
        // fall through
    case 2:
        if (field_0x240 == 0) {
            field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        }
        if (field_0x244 == 0) {
            mAlive = false;
        }
        if (field_0x4 < 8) {
            lbl_8053AB10->field_0x24->addFocusPoint_8030C210(&mPos);
        }
        break;
    }

    if (field_0x230 == 0) {
        field_0x254 = lbl_804A10A0[field_0x98 * 3 + field_0x248];
    } else {
        field_0x254 = lbl_804A1100[field_0x98 * 3 + field_0x248];
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
    if (field_0x244 > 0) {
        field_0x244--;
    }
}

// FIXME: 72.66 %. The tile codes are signed s32s as in the original, but
// MWCC merges the listed-code test into subi/cmplwi ranges (runs 2-7,
// 0x27-0x2F, 0x39-0x3B) where the original compares each code with its own
// beq, and the 0x10..0x17 test is two cmpwi here, subi + cmplwi 7 in the
// original. The fn_8020D6AC call also loads pos into r5 after serial's copy
// to r6, the reverse of the original.
bool cls_804A1160::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    s32 serial = mSerial;
    CollisionBoxes_8020D58C* boxes = fn_8020D58C();
    if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, lbl_80466880) == 1) {
        return true;
    }

    s32 code = fn_8022461C(pos, layer);
    s32 code2 = fn_80226850(pos, layer);
    if ((code >= 0x10 && code <= 0x17) || (code2 >= 0x140 && code2 <= 0x15F)) {
        field_0x238 = 0;
        field_0x234 = 1;
        return false;
    }
    if (code2 == 0x230 || code2 == 0x231 || code2 == 0x234) {
        field_0x234 = 0;
        return false;
    }

    // Tile codes that never block (and go straight to the counter below).
    if (!(code == 0 || code == 4 || code == 5 || code == 6 || code == 7 || code == 2 ||
          code == 3 || code == 0x50 || code == 0xE || code == 0xC || code == 0x27 ||
          code == 0x28 || code == 0x29 || code == 0x2A || code == 0x2B || code == 0x2C ||
          code == 0x2D || code == 0x2E || code == 0x2F || code == 0x20 || code == 0xD ||
          code == 0x39 || code == 0x3A || code == 0x3B || code == 0x4C || code == 0x4D ||
          code == 0x4E || code == 0x4F || code == 0x24)) {
        if (fn_80226850(pos, layer) == 0x3A || fn_80226850(pos, layer) == 0x3D) {
            field_0x234 = 0;
            return false;
        }
        if (code2 == 0x14) {
            field_0x234 = 0;
            return false;
        }
        if (code == 0x25) {
            if (field_0x234 == 0 && getGameView_802D6A9C()->field_0x171 == 0) {
                return true;
            }
            field_0x234 = 0;
            return false;
        }
        if (code == 0x26) {
            if (field_0x234 == 0 && getGameView_802D6A9C()->field_0x171 == 1) {
                return true;
            }
            field_0x234 = 0;
            return false;
        }
        if (field_0x234 == 0) {
            if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
        if (field_0x234 == 1) {
            return false;
        }
    }

    if (field_0x234 == 1) {
        field_0x238 = field_0x238 + 1;
        if (field_0x238 > 0xB) {
            field_0x238 = 0;
            field_0x234 = 0;
        }
    }
    return false;
}

void cls_804A1160::init() {
    field_0x80.i.x = -2.0f;
    field_0x80.i.y = -2.0f;
    field_0x80.f.x = 2.0f;
    field_0x80.f.y = 2.0f;
    mBox.i.x = -4.0f;
    mBox.i.y = -4.0f;
    mBox.f.x = 4.0f;
    mBox.f.y = 4.0f;

    // The 0x90 word of the spawner's parameters: bits 4-5 go to 0x98, bits 0-3
    // to the player (0x258) and bits 8-11 to 0x25C.
    field_0x98 = (field_0x90 >> 4) & 3;
    field_0x258 = field_0x90 & 0xF;
    field_0x25c = (field_0x90 >> 8) & 0xF;

    field_0x248 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0;

    if (fn_80237060(field_0x258) == 1) {
        mAlive = false;
        return;
    }

    field_0x234 = 0;
    field_0x238 = 0;
    field_0x254 = 0;
    field_0x118 = fn_8023B630(field_0x258);
    field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
    field_0x23c = 2;
    field_0x230 = 0;
    startSound_801F0E34(0x30026, 0);
}
