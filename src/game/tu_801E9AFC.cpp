#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/act/tu_801E6D48.h"
#include <stdlib.h>
#include "game/tu_801E9AFC.h"

// The texture of each sprite (0-2); the fourth entry ends the table.
static const char* lbl_804A5610[4] = {"sen.bti", "koge.bti", "sen_top.bti", NULL};

// qsort comparator of fn_801EA594 (defined below).
int fn_801EA8C0(const void* a, const void* b);

// The actor ids fn_801E9B58 looks for, ended by 'NULL'.
static const u32 lbl_80469C78[4] = {'FIRE', 'BLZE', 'BMTY', 'NULL'};

inline void cls_801E9B54::drawOutlines() {
    for (s32 i = 0; i < field_0x60; i++) {
        if (field_0x0[i].actor != NULL) {
            field_0x0[i].actor->fn_801E8080();
        }
    }
}

inline void cls_801E9B54::drawSprites() {
    for (s32 i = 0; i < field_0x60; i++) {
        if (field_0x0[i].actor != NULL) {
            field_0x0[i].actor->fn_801E801C();
        }
    }
    for (s32 i = 0; i < field_0x74; i++) {
        s32 serial = field_0x64[i];
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                ((cls_804A5418*)actor)->fn_801E6DE8();
            }
        }
    }
}

// qsort comparator of fn_801EA594: orders the slots by their byte at 0x4.
int fn_801EA8C0(const void* a, const void* b) {
    const Slot_801E9B54* slotA = (const Slot_801E9B54*)a;
    const Slot_801E9B54* slotB = (const Slot_801E9B54*)b;
    int result = 0;
    if (slotA->id < slotB->id) {
        result = -1;
    } else if (slotA->id > slotB->id) {
        result = 1;
    }
    return result;
}

void cls_801E9B54::fn_801EA75C(u8 id, u8 flag, u8 index, s32 layer, const JGeometry::TVec2f* pos) {
    s32 i = 0;
    for (s32 n = field_0x60; i < n; i++) {
        if (id == field_0x0[i].id) {
            break;
        }
    }
    if (i < field_0x60) {
        if (field_0x0[i].actor != NULL) {
            if (flag != 0) {
                field_0x0[i].actor->field_0x94 = flag;
            }
            field_0x0[i].actor->fn_801E8EB4(index, layer, pos);
        }
    } else {
        field_0x0[field_0x60].id = id;
        JGeometry::TVec3f spawnPos(pos->x, pos->y, 0.0f);
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FSOB, layer, &spawnPos, id, -1, -1);
        if (serial >= 0) {
            field_0x0[field_0x60].actor = (cls_804A5568*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (field_0x0[field_0x60].actor != NULL) {
                if (flag != 0) {
                    field_0x0[i].actor->field_0x94 = flag;
                }
                field_0x0[field_0x60].actor->fn_801E8EB4(index, layer, pos);
                field_0x60++;
            }
        }
    }
}

s32 cls_801E9B54::fn_801EA6D8(u8 arg0, s32 arg1) {
    s32 result = -1;
    if (arg0 == 0) {
        field_0x64[field_0x74] = arg1;
        field_0x74++;
    } else {
        for (s32 i = 0; i < field_0x60; i++) {
            if (arg0 == field_0x0[i].id) {
                field_0x0[i].value = arg1;
                if (field_0x0[i].actor != NULL) {
                    result = field_0x0[i].actor->mSerial;
                }
                break;
            }
        }
    }
    return result;
}

// FIXME: 89.9. The slot shift-down loop: the original recomputes the source
// slot from the index each pass (addi, mulli, add; 81 instructions), ours
// walks a second pointer (80).
void cls_801E9B54::fn_801EA594() {
    if (field_0x60 > 0) {
        bool removed;
        do {
            removed = false;
            for (s32 i = 0; i < field_0x60; i++) {
                if (field_0x0[i].value == -1) {
                    if (field_0x0[i].actor != NULL) {
                        field_0x0[i].actor->mAlive = false;
                        field_0x0[i].actor->mDrawEnabled = false;
                    }
                    Slot_801E9B54* dst = &field_0x0[i];
                    for (; i < field_0x60 - 1; i++) {
                        *dst = field_0x0[i + 1];
                        dst++;
                    }
                    field_0x60 = field_0x60 - 1;
                    removed = true;
                    break;
                }
            }
        } while (removed);
        if (field_0x60 > 1) {
            qsort(field_0x0, field_0x60, sizeof(Slot_801E9B54), fn_801EA8C0);
        }
        for (s32 i = 0; i < field_0x60; i++) {
            if (field_0x0[i].actor != NULL) {
                field_0x0[i].actor->fn_801E8C74(field_0x0[i].value);
            }
        }
    }
}

// FIXME: 99.2. Regalloc only: the texture loop's index is r31 in the original
// (r27 here) and the table, texture, palette and ratio pointers sit one
// register lower.
void cls_801E9AFC::fn_801EA384(bool flag) {
    if (flag) {
        for (s32 i = 0; i < 3; i++) {
            field_0x178[i] = fn_801CB270(lbl_804A5610[i], &field_0xf4[i], &field_0x154[i]);
        }
        field_0xf0 = 0;
        field_0x0[0].clear();
        field_0x0[1].clear();
    } else {
        field_0xf0 ^= 1;
        field_0x0[field_0xf0].clear();
    }
}

void cls_801E9AFC::fn_801EA344() {
    GXLoadTlut(&field_0x154[0], GX_TLUT0);
    GXLoadTexObj(&field_0xf4[0], GX_TEXMAP0);
}

void cls_801E9AFC::fn_801EA304() {
    GXLoadTlut(&field_0x154[1], GX_TLUT0);
    GXLoadTexObj(&field_0xf4[1], GX_TEXMAP0);
}

// 0x801EA108.
// FIXME(shotgun): 46.2 %, missing/extra; the original reloads the sin/cos table for every vertex.
void cls_801E9AFC::fn_801EA108(const JGeometry::TVec2f* pos, s16 angle, f32 z) {
    f32 s0 = 0.0f;
    f32 s1 = 1.0f;
    f32 centerX = 0.0f;
    f32 centerY = 2.0f;
    if (angle < 0) {
        centerX = -0.0f;
        s0 = s1;
        s1 = 0.0f;
        centerY = -2.0f;
    }
    s16 rot = -angle;
    f32 sn = JMath::sincosTable_.sinShort(rot);
    f32 cs = JMath::sincosTable_.cosShort(rot);
    f32 x = pos->x + (centerY * cs - centerX * sn);
    f32 y = pos->y + (centerY * sn + centerX * cs);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(x + (-4.0f * cs - -4.0f * sn), y + (-4.0f * sn + -4.0f * cs) + z, z);
    GXTexCoord2f32(s0, 0.0f);
    GXPosition3f32(x + (4.0f * cs - -4.0f * sn), y + (4.0f * sn + -4.0f * cs) + z, z);
    GXTexCoord2f32(s1, 0.0f);
    GXPosition3f32(x + (4.0f * cs - 4.0f * sn), y + (4.0f * sn + 4.0f * cs) + z, z);
    GXTexCoord2f32(s1, 1.0f);
    GXPosition3f32(x + (-4.0f * cs - 4.0f * sn), y + (-4.0f * sn + 4.0f * cs) + z, z);
    GXTexCoord2f32(s0, 1.0f);
}

// FIXME: 91.0. Three fewer-instruction spots: the original converts the
// "outlines" / "sprites" tests to bool through a byte (clrlwi, neg, or, srwi)
// where ours keeps the compare result; the rest is the same code, registers
// shifted.
void cls_801E9AFC::fn_801E9C74() {
    u8 both = getGameView_802D6A9C()->fn_802DCD0C();
    bool outlines;
    bool sprites;
    if (both) {
        outlines = field_0x0[0].hasSlots() || field_0x0[1].hasSlots();
        sprites = outlines || field_0x0[0].hasSerials() || field_0x0[1].hasSerials();
    } else {
        cls_801E9B54* element = &field_0x0[field_0xf0];
        outlines = element->hasSlots();
        sprites = outlines || element->hasSerials();
    }
    if (outlines || sprites) {
        gGameRoot2_8053AB10->fn_8013B438();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetNumTexGens(1);
        GXSetNumChans(0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetCullMode(GX_CULL_BACK);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_EQUAL, 0xFF, GX_AOP_AND, GX_ALWAYS, 0);
        if (outlines) {
            if (both) {
                field_0x0[0].drawOutlines();
                field_0x0[1].drawOutlines();
            } else {
                field_0x0[field_0xf0].drawOutlines();
            }
        }
        if (sprites) {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            GXSetZCompLoc(GX_FALSE);
            GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
            GXLoadTlut(&field_0x154[2], GX_TLUT0);
            GXLoadTexObj(&field_0xf4[2], GX_TEXMAP0);
            if (both) {
                field_0x0[0].drawSprites();
                field_0x0[1].drawSprites();
            } else {
                field_0x0[field_0xf0].drawSprites();
            }
        }
    }
}

void fn_801E9C00(s16 angle, JGeometry::TVec2f* vec) {
    vec->x = 0.0f;
    vec->y = 4.0f;
    s16 turn = angle + (angle < 0 ? -0x2000 : 0x2000);
    f32 x = vec->x;
    vec->x = x * JMath::sincosTable_.cosShort(-turn) - vec->y * JMath::sincosTable_.sinShort(-turn);
    vec->y = x * JMath::sincosTable_.sinShort(-turn) + vec->y * JMath::sincosTable_.cosShort(-turn);
}

// FIXME: 97.3. The original copies findActorBySerial_801F666C's result through r0 (mr r0, r3; mr. r30, r0); ours tests it in place.
s32 ActorBase_804A5770::fn_801E9B58() {
    s32 serial = getActorManager_801F856C()->fn_801F7F58(this, lbl_80469C78, mSerial, 32.0f);
    if (serial >= 0 && serial != mSerial) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            if (fn_801CC8E4(actor)) {
                serial = -1;
            } else if (actor->mActorId == ACTOR_FIRE) {
                actor->fn_802BAA00();
            }
        }
    }
    return serial;
}

cls_801E9B54::cls_801E9B54() {}

cls_801E9AFC* fn_801E9AFC() {
    static cls_801E9AFC lbl_804CA358;
    return &lbl_804CA358;
}
