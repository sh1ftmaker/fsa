#include "game/GameAudio.h"
#include "game/act/act_warp.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "game/GameRootSub20.h"
#include "game/act/tu_801688AC.h"
#include <float.h>

// The layer the warp's spawn parameter selects (0 stays 0, else 7 + n).
static inline s32 warpLayer(u32 param) {
    s32 raw = (param >> 24) & 7;
    s32 layer = raw + 7;
    if (raw == 0) {
        layer = raw;
    }
    return layer;
}

// Whether two boxes overlap (edges touching counts), on a copy of `a`.
static inline bool boxesOverlap(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    return overlap.intersect(b);
}

// The box `ofs` (relative to pos) in top-down screen space: the top edge shifts by z, the
// bottom edge by z / 2 (read from fn_80227290's asm).
static inline void toScreenBox(JGeometry::TBox2f* out, JGeometry::TBox2f ofs, JGeometry::TVec3f pos) {
    out->set(ofs.i.x + pos.x, ofs.i.y + pos.y - pos.z, ofs.f.x + pos.x, ofs.f.y + pos.y - pos.z * 0.5f);
}

// The spawn parameter of the WARP that init() creates for the paired kinds 0x19-0x1C: this warp's
// tile position (in 8-unit cells, from the layer box origin) and layer in the bits the pair reads
// back, the kind in the low bits.
static inline u32 pairParam(u32 param, s32 layer, f32 dx, f32 dy, u32 kind) {
    u32 v = param & 0xF8000000;
    s32 enc = layer < 8 ? 0 : layer - 7;
    v = (v & ~0x07000000) | ((enc << 24) & 0x07000000);
    v = (v & ~0x00FF0000) | ((((s32)dx / 8) << 16) & 0x00FF0000);
    v = (v & ~0x0000FF00) | ((((s32)dy / 8) << 8) & 0x0000FF00);
    v = (v & ~0x80) | (param & 0x80);
    return v | kind;
}

// 0x80228B60. Decodes the spawn parameter (field_0x90: kind in the low 7 bits, layer, a 6-bit cell
// offset to the destination), sets the destination (field_0x238) and the trigger box, and for the
// paired kinds 0x19-0x1C spawns the other warp and turns this one into the matching kind.
// FIXME(shotgun): 66.65 %, missing/extra; behaviour drift.
void Warp_804A6DC8::init() {
    field_0x256 = 0;
    field_0x278 = 0;
    field_0x255 = 0;
    for (s32 i = 0; i < 4; i++) {
        field_0x27c[i] = NULL;
    }
    if ((field_0x90 & 0x7F) == 0x20) {
        field_0x254 = 1;
    } else {
        field_0x254 = 0;
    }
    s32 destLayer = (field_0x90 >> 24) & 7;
    destLayer += 7;
    if (((field_0x90 >> 24) & 7) == 0) {
        destLayer = 0;
    }
    JGeometry::TBox2f* destBox = layerBox(destLayer);
    f32 destX = destBox->i.x;
    f32 destY = destBox->i.y;
    s32 myLayer = field_0x4;
    JGeometry::TBox2f* myBox = layerBox(myLayer);
    u32 kind0 = field_0x90 & 0x7F;
    if ((kind0 == 6 || kind0 == 4 || kind0 == 0x20 || kind0 == 0x24) && ((field_0x90 >> 13) & 0x1F8) == 0 &&
        ((field_0x90 >> 5) & 0x1F8) == 0)
    {
        field_0x238.x = destX + (mPos.x - myBox->i.x);
        field_0x238.y = destY + (mPos.y - myBox->i.y);
        field_0x238.z = 0.0f;
    } else {
        field_0x238.x = (f32)((field_0x90 >> 13) & 0x1F8) + destX;
        field_0x238.y = (f32)((field_0x90 >> 5) & 0x1F8) + destY;
        field_0x238.z = 0.0f;
    }
    u32 kind = field_0x90 & 0x7F;
    if (kind == 0x19) {
        s32 spawnLayer = warpLayer(field_0x90);
        u32 param = pairParam(field_0x90, field_0x4, (s32)(mPos.x - myBox->i.x), (s32)(mPos.y - myBox->i.y), 1);
        getActorManager_801F856C()->spawnActor_801F9484('WARP', spawnLayer, &field_0x238, param, -1, -1);
        field_0x90 = field_0x90 & ~0xFF;
    } else if (kind == 0x1A) {
        s32 spawnLayer = warpLayer(field_0x90);
        u32 param = pairParam(field_0x90, field_0x4, (s32)(mPos.x - myBox->i.x), (s32)(mPos.y - myBox->i.y), 3);
        getActorManager_801F856C()->spawnActor_801F9484('WARP', spawnLayer, &field_0x238, param, -1, -1);
        field_0x90 = (field_0x90 & ~0xFF) | 2;
    } else if (kind == 0x1B) {
        s32 spawnLayer = warpLayer(field_0x90);
        u32 param = pairParam(field_0x90, field_0x4, (s32)(mPos.x - myBox->i.x), (s32)(mPos.y - myBox->i.y), 0x18);
        getActorManager_801F856C()->spawnActor_801F9484('WARP', spawnLayer, &field_0x238, param, -1, -1);
        field_0x90 = (field_0x90 & ~0xFF) | 0x17;
    } else if (kind == 0x1C) {
        s32 spawnLayer = warpLayer(field_0x90);
        u32 param = pairParam(field_0x90, field_0x4, (s32)(mPos.x - myBox->i.x), (s32)(mPos.y - myBox->i.y), 0x17);
        getActorManager_801F856C()->spawnActor_801F9484('WARP', spawnLayer, &field_0x238, param, -1, -1);
        field_0x90 = (field_0x90 & ~0xFF) | 0x18;
    }
    if (((field_0x90 >> 7) & 1) == 0 && (field_0x90 & 0x7F) != 0x20) {
        field_0x238.x += 8.0f;
        field_0x238.y += 8.0f;
    }
    kind = field_0x90 & 0x7F;
    if (((field_0x90 >> 7) & 1) == 0 && kind != 0xB && kind != 0x1E && kind != 0x1F && kind != 0x20) {
        mPos.x = mPos.x + (f32)(s32)(8.0f - (f32)((s32)mPos.x % 8));
        mPos.y = mPos.y + (f32)(s32)(8.0f - (f32)((s32)mPos.y % 8));
    }
    kind = field_0x90 & 0x7F;
    if (kind == 6) {
        mBox.set(-0.01f, -0.01f, 0.01f, 0.01f);
    } else if (kind == 0xB) {
        mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    } else if (kind == 0x20) {
        mBox.set(-10.0f, -10.0f, 10.0f, 10.0f);
    } else if (kind == 0) {
        mBox.set(-4.0f, -20.0f, 4.0f, 4.0f);
    } else if (kind == 1) {
        mBox.set(-4.0f, -4.0f, 4.0f, 20.0f);
    } else if (kind == 2) {
        mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    } else if (kind == 3) {
        mBox.set(-4.0f, -20.0f, 4.0f, 4.0f);
    } else if (kind == 0xF) {
        mBox.set(-24.0f, -8.0f, 24.0f, 8.0f);
    } else if (kind == 0x13) {
        mBox.set(-256.0f, -8.0f, 256.0f, 8.0f);
    } else if (kind == 0x16) {
        mBox.set(-24.0f, -8.0f, 24.0f, 8.0f);
    } else if (kind == 0x15) {
        if (viewLayerFlag(field_0x4)) {
            mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
        } else if (field_0x4 >= 8) {
            mBox.set(-4.0f, -4.0f, 4.0f, 20.0f);
        } else {
            mBox.set(-4.0f, -20.0f, 4.0f, 4.0f);
        }
    } else if (kind == 0x23) {
        if (viewLayerFlag(field_0x4)) {
            mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
        } else if (field_0x4 >= 8) {
            mBox.set(-4.0f, -20.0f, 4.0f, 4.0f);
        } else {
            mBox.set(-4.0f, -4.0f, 4.0f, 20.0f);
        }
    } else if (kind == 0x17) {
        mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    } else if (kind == 0x18) {
        mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    } else if (kind == 4) {
        mBox.set(-1.0f, -1.0f, 1.0f, 1.0f);
    } else if (kind == 0x28) {
        mBox.set(-7.0f, -7.0f, 7.0f, 7.0f);
    } else {
        mBox.set(-4.0f, -4.0f, 4.0f, 4.0f);
    }
    if ((field_0x90 & 0x7F) == 0x28) {
        field_0x90 = (field_0x90 & ~0x7F) | 4;
    }
    mLife = 0;
    field_0xb0 = 3;
    for (s32 i = 0; i < 4; i++) {
        field_0x234[i] = 0;
    }
    mEmitter24c = NULL;
    mEmitter250 = NULL;
    if ((field_0x90 & 0x7F) == 0xE) {
        mEmitter24c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x553, 0, -1, NULL, NULL, true, 1.0f);
        mEmitter250 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4EE, 0, 0, NULL, NULL, true, 1.0f);
    }
    if ((field_0x90 & 0x7F) == 0x25) {
        mEmitter24c = spawnEmitter_8013CC50(field_0x4, &mPos, 0x553, -1, -1, NULL, NULL, true, 1.0f);
        mEmitter250 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x4EE, 0, 0, NULL, NULL, true, 1.0f);
    }
    field_0x244 = getGameView_802D6A9C()->field_0x14;
    field_0x248 = getGameView_802D6A9C()->field_0x18;
    for (s32 i = 0; i < 4; i++) {
        field_0x258[i] = 0.0f;
        field_0x268[i] = 0.0f;
    }
}

Warp_804A6DC8::~Warp_804A6DC8() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
}

extern const u32 lbl_8046ABE8[28]; // 'NULL'-ended actor id lists: [0] pushed, [14] and [17] by kind

// Whether |d| is within 32 * FLT_EPSILON.
static inline bool nearZero(f32 d) {
    f32 eps = 32.0f * FLT_EPSILON;
    bool ok = false;
    if (-eps <= d && d <= eps) {
        ok = true;
    }
    return ok;
}

// 0x80227F50. Every frame: tracks whether a player is near (the sand shake), lets the kind-0x20 warp
// swallow a player with a drawn box, then pushes the actors lying on the pad (per kind) to the
// warp's destination (field_0x238) or nudges their velocity.
// FIXME(shotgun): 77.80 %, missing/extra + reloc-name; behaviour drift.
void Warp_804A6DC8::update() {
    for (s32 i = 0; i < 4; i++) {
        field_0x258[i] = fn_8022C76C(i);
        field_0x268[i] = field_0x258[i];
    }
    field_0x256 = 0;
    for (s32 i = 0; i < 4; i++) {
        JGeometry::TVec3f pos = *getPlayerPos_8023E724(i);
        if (__fabsf(mPos.x - pos.x) < 32.0f && __fabsf(mPos.y - pos.y) < 32.0f) {
            field_0x256 = 1;
            break;
        }
    }
    if (field_0x256 == 1) {
        if (field_0x278 < 0) {
            if (field_0x278 == -1) {
                field_0x278 = 0;
            } else {
                field_0x278 = field_0x278 + 1;
            }
        }
    } else {
        field_0x278 = -1;
    }
    if ((field_0x90 & 0x7F) == 0x20) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_802373EC(i) == 1 && fn_8023A51C(i)) {
                JGeometry::TVec3f pos = *fn_8023A478(i);
                if (__fabsf(mPos.x - pos.x) < 64.0f && __fabsf(mPos.y - pos.y) < 64.0f) {
                    JGeometry::TVec3f p0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
                    gBgRewrite_8053AC98->fn_802A3948(&p0, i, field_0x4, 0xC);
                    JGeometry::TVec3f p1(mPos.x + 8.0f, mPos.y - 8.0f, 0.0f);
                    gBgRewrite_8053AC98->fn_802A3948(&p1, i, field_0x4, 0xC);
                    JGeometry::TVec3f p2(mPos.x - 8.0f, mPos.y + 8.0f, 0.0f);
                    gBgRewrite_8053AC98->fn_802A3948(&p2, i, field_0x4, 0xC);
                    JGeometry::TVec3f p3(mPos.x + 8.0f, mPos.y + 8.0f, 0.0f);
                    gBgRewrite_8053AC98->fn_802A3948(&p3, i, field_0x4, 0xC);
                    if (field_0x255 == 0) {
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E3, 0, -2, NULL, NULL, true, 1.0f);
                        field_0x255 = 1;
                    }
                    break;
                }
            }
        }
    }
    if ((field_0x90 >> 27) != 0 && !vfunc_0x34()) {
        return;
    }
    JGeometry::TBox2f box(mBox);
    JGeometry::TVec2f at(mPos.x, mPos.y);
    u16 tile = fn_802265FC((const JGeometry::TVec3f*)&at, field_0x4);
    if (tile == 1 || tile == 0x21) {
        return;
    }
    if (field_0x255 == 1) {
        GameRootSub20_801902C0* sand = fn_801902C0();
        if (sand->field_0x79c < 16) {
            sand->mSandFalls[sand->field_0x79c].mPos = mPos;
            sand->mSandFalls[sand->field_0x79c].field_0xc[0] = 1;
            sand->field_0x79c++;
        }
    }
    box.addPos(at);
    if ((field_0x90 & 0x7F) == 0x20) {
        fn_80227530(&box);
    } else if ((field_0x90 & 0x7F) == 0x24) {
        for (s32 i = 0; i < 4; i++) {
            if (gGameRoot_8053AAF8->field_0x7e && !fn_8022C8EC(i)) {
                if (getPlayerLayer_8023DE58(i) != warpLayer(field_0x90)) {
                    continue;
                }
            }
            JGeometry::TBox2f playerBox = box;
            if (getPlayerPos_8023E724(i)->y > mPos.y) {
                f32 dy = getPlayerPos_8023E724(i)->y - mPos.y;
                playerBox.addPos(JGeometry::TVec2f(0.0f, dy));
            }
            if (fn_8023B0F4(i, &playerBox, field_0x4, false, 8.0f)) {
                fn_802317F0(i, warpLayer(field_0x90), &mPos, &field_0x238, field_0x90 & 0x7F);
            }
        }
    } else {
        fn_802278A8(&box);
    }
    u16 destTile = fn_80226850(&field_0x238, warpLayer(field_0x90));
    if (destTile == 0x19C || destTile == 0x19D) {
        return;
    }
    s32 kind = field_0x90 & 0x7F;
    s32 serial;
    if (kind == 4 || kind == 0x20 || kind == 0x24) {
        serial = fn_801F26CC(lbl_8046ABE8 + 17);
    } else if (kind == 6 || kind == 0xB || kind == 0x14) {
        serial = fn_801F26CC(lbl_8046ABE8 + 14);
    } else {
        serial = fn_80227290(32.0f, lbl_8046ABE8, -1);
    }
    if (serial < 0) {
        return;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    kind = field_0x90 & 0x7F;
    if (kind == 0 || kind == 3 || kind == 0x15 || kind == 0x23) {
        if (actor->field_0x3c.y >= 0.0f) {
            return;
        }
    }
    if (kind == 1 || kind == 2) {
        if (actor->field_0x3c.y <= 0.0f) {
            return;
        }
    }
    if (kind == 0x17) {
        if (actor->field_0x3c.x <= 0.0f) {
            return;
        }
    }
    if (kind == 0x18) {
        if (actor->field_0x3c.x >= 0.0f) {
            return;
        }
    }
    if (actor->mActorId == 'BOME') {
        cls_804A0738* bome = static_cast<cls_804A0738*>(actor);
        getPlayerLayer_8023DE58(bome->field_0x90 & 0xF);
        bool at_rest = bome->field_0x230 == 1;
        if (!at_rest) {
            if (nearZero(bome->field_0x3c.x - bome->field_0x364[0]) &&
                nearZero(bome->field_0x3c.y - bome->field_0x364[1]) &&
                nearZero(bome->field_0x3c.z - bome->field_0x364[2]))
            {
                at_rest = true;
            }
        }
        if (at_rest == 1) {
            return;
        }
        kind = field_0x90 & 0x7F;
        if (kind == 4 || kind == 0x20 || kind == 0x24) {
            at_rest = bome->field_0x230 == 1;
            if (!at_rest) {
                if (nearZero(bome->field_0x3c.x - bome->field_0x364[0]) &&
                    nearZero(bome->field_0x3c.y - bome->field_0x364[1]) &&
                    nearZero(bome->field_0x3c.z - bome->field_0x364[2]))
                {
                    at_rest = true;
                }
            }
            if (!at_rest) {
                return;
            }
        }
    }
    u32 id = actor->mActorId;
    if (id == 'HBMB' || id == 'STON' || id == 'GRAS' || id == 'TUBO' || id == 'YASI') {
        if (PSVECMag(&actor->field_0x3c) <= 1.0f) {
            return;
        }
    }
    kind = field_0x90 & 0x7F;
    if (kind == 4 || (u32)(kind - 7) <= 2 || kind == 0x12 || kind == 0x20 || kind == 0x24) {
        if (actor->mPos.z > 4.0f) {
            return;
        }
    }
    id = actor->mActorId;
    if (id == 'HNRY' || id == 'BOME' || id == 'PCDM' || id == 'SWBM' ||
        (id == 'FIRE' && ((actor->field_0x90 >> 6) & 7) != 1))
    {
        kind = field_0x90 & 0x7F;
        if (kind == 4 || kind == 0x20 || kind == 0x24) {
            if (actor->mPos.z > 0.0f) {
                return;
            }
        }
        JGeometry::TVec3f dest = field_0x238;
        dest.x += actor->mPos.x - mPos.x;
        dest.y += actor->mPos.y - mPos.y;
        dest.z += actor->mPos.z;
        actor->fn_801F349C(warpLayer(field_0x90), &dest);
        return;
    }
    if (actor->mActorId == 'ESRY') {
        kind = field_0x90 & 0x7F;
        if (kind == 0 || kind == 3 || kind == 0x15 || kind == 0x23) {
            actor->field_0x3c.set(0.0f, -1.8f, 0.0f);
        } else if (kind == 1 || kind == 2) {
            actor->field_0x3c.set(0.0f, 1.8f, 0.0f);
        } else if (kind == 0x17) {
            actor->field_0x3c.set(1.8f, 0.0f, 0.0f);
        } else if (kind == 0x18) {
            actor->field_0x3c.set(-1.8f, 0.0f, 0.0f);
        }
    }
    if (actor->mActorId == 'FIRE') {
        if (((actor->field_0x90 >> 6) & 3) == 1) {
            return;
        }
        if (actor->field_0xc4) {
            return;
        }
    }
    actor->fn_801F349C(warpLayer(field_0x90), &field_0x238);
}

extern const f32 lbl_8046AC58[24]; // x shifts of the sand while the warp shakes

// 0x80227CE8.
// Remaining: original compares (field_0x90 & 0x7F) with a signed cmpwi; reloc names.
// FIXME: 99.45 %, different + reloc-name; behaviour drift.
void Warp_804A6DC8::draw() {
    if ((field_0x90 & 0x7F) == 0x20 && field_0x255 == 0) {
        SpriteParams_801F5AE0 params;
        JGeometry::TVec3f saved = mPos;
        mPos += JGeometry::TVec3f(-16.0f, -16.0f, 0.0f);
        if (field_0x256 == 1 && field_0x278 >= 0) {
            mPos.x += lbl_8046AC58[field_0x278];
            field_0x278++;
            if (field_0x278 >= 0x18) {
                field_0x278 = fn_801CD664(10) - 0x69;
            }
        }
        params.field_0x10.a = 0xA0;
        params.field_0x1d = -2;
        params.field_0x20 = 1;
        params.field_0x2c = 0;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SMP ', "sand2_smp.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F6CF4(&mPos, res, &params);
        }
        mPos = saved;
    } else {
        mPos.y += 0.1f;
        mPos.y -= 0.1f;
    }
}

// 0x802278A8. Per player: the player's reaction to standing in the warp's box (by warp kind, the
// low 7 bits of the spawn parameter): marks it, hurts it, or sends it through (fn_802317F0).
// FIXME(shotgun): 79.60 %, missing/extra; behaviour drift.
void Warp_804A6DC8::fn_802278A8(JGeometry::TBox2f* box) {
    for (s32 player = 0; player < 4; player++) {
        *box = mBox;
        box->addPos(JGeometry::TVec2<f32>(mPos.x, mPos.y));
        if (!fn_8022D75C(player) && fn_8022D534(player)) {
            field_0x234[player] = 1;
            continue;
        }
        if ((field_0x90 & 0x7F) == 0x10) {
            return;
        }
        if (fn_8022F514(player)) {
            u32 kind = field_0x90 & 0x7F;
            if (kind <= 3 || kind == 0x17 || kind == 0x18) {
                if (fn_8023B0F4(player, box, field_0x4, false, 8.0f)) {
                    fn_802381B8(player);
                }
            }
        }
        if ((field_0x90 & 0x7F) == 0x13) {
            if (!fn_80234E30(player)) {
                continue;
            }
        }
        if (gGameRoot_8053AAF8->field_0x7e && !fn_8022C8EC(player)) {
            if (getPlayerLayer_8023DE58(player) != warpLayer(field_0x90)) {
                continue;
            }
        }
        if (fn_8023B0F4(player, box, field_0x4, false, 8.0f)) {
            u32 kind = field_0x90 & 0x7F;
            bool pass = false;
            if (kind == 0x15 || kind == 0x23) {
                s32 layer = field_0x4;
                if (viewLayerFlag(layer)) {
                    if (!fn_8022C5EC(player, 8) && !fn_8022C5EC(player, 4) && !fn_8022C46C(player, 8) &&
                        !fn_8022C46C(player, 4))
                    {
                        continue;
                    }
                    if (fn_8022DBA0(player)) {
                        continue;
                    }
                    pass = true;
                }
            }
            if (!pass) {
                if (field_0x234[player] == 1) {
                    continue;
                }
                if (fn_8022D75C(player)) {
                    field_0x234[player] = 1;
                    continue;
                }
            }
            if (fn_8022F478(player) && (field_0x90 & 0x7F) != 4) {
                continue;
            }
            kind = field_0x90 & 0x7F;
            if (kind == 4 || kind == 0x20 || kind == 0x24) {
                if (fn_802373EC(player) == 1 && fn_8023725C(player) >= 2 && fn_8023513C(player)) {
                    continue;
                }
            }
            if (!fn_802317F0(player, warpLayer(field_0x90), &mPos, &field_0x238, field_0x90 & 0x7F)) {
                continue;
            }
            if ((field_0x90 & 0x7F) != 4 || !fn_8022F514(player)) {
                continue;
            }
            s32 layer = getPlayerLayer_8023DE58(player);
            if (viewLayerFlag(layer)) {
                continue;
            }
            if (fn_8022CAF8(player) == 4 || fn_8022CAF8(player) == 5) {
                continue;
            }
            JGeometry::TVec3f* pos = getPlayerPos_8023E724(player);
            s32 playerLayer = getPlayerLayer_8023DE58(player);
            if (!fn_8022BC38(fn_8022F3DC(player), playerLayer, pos)) {
                continue;
            }
            fn_802381B8(fn_8022F3DC(player));
            pos = getPlayerPos_8023E724(player);
            playerLayer = getPlayerLayer_8023DE58(player);
            fn_802348C4(fn_8022F3DC(player), playerLayer, pos);
        } else {
            box->i.x -= 1.0f;
            box->i.y -= 1.0f;
            box->f.x += 1.0f;
            box->f.y += 1.0f;
            if (!fn_8023B0F4(player, box, field_0x4, false, 0.0f)) {
                field_0x234[player] = 0;
            }
        }
    }
}

// 0x80227530. Per player: runs fn_80226FC8, then marks the player on the warp and, for a player
// that just stepped on and passes the checks, rewrites the four tiles around it and spawns the effect.
// FIXME(shotgun): 82.26 %, missing/extra; behaviour near.
void Warp_804A6DC8::fn_80227530(JGeometry::TBox2f* box) {
    for (s32 player = 0; player < 4; player++) {
        fn_80226FC8(player);
        box->set(mBox);
        box->addPos(mPos.x, mPos.y);
        if (gGameRoot_8053AAF8->field_0x7e && !fn_8022C8EC(player)) {
            if (getPlayerLayer_8023DE58(player) != warpLayer(field_0x90)) {
                continue;
            }
        }
        if (fn_8023B0F4(player, box, field_0x4, false, 8.0f)) {
            if (field_0x234[player] == 1) {
                continue;
            }
            field_0x234[player] = 1;
            if (fn_802373EC(player) == 1 && fn_8023725C(player) >= 2 && fn_8023513C(player)) {
                continue;
            }
            if (fn_8022F514(player) && fn_8022CAF8(player) != 4) {
                s32 leader = fn_8022F3DC(player);
                if (fn_802373EC(leader) == 1 && fn_8023725C(leader) >= 2 && fn_8023513C(leader)) {
                    continue;
                }
            }
            if (!fn_802317F0(player, warpLayer(field_0x90), &mPos, &field_0x238, field_0x90 & 0x7F)) {
                continue;
            }
            if (field_0x254 != 1) {
                continue;
            }
            JGeometry::TVec3f p0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
            gBgRewrite_8053AC98->fn_802A3948(&p0, player, field_0x4, 0xC);
            JGeometry::TVec3f p1(mPos.x + 8.0f, mPos.y - 8.0f, 0.0f);
            gBgRewrite_8053AC98->fn_802A3948(&p1, player, field_0x4, 0xC);
            JGeometry::TVec3f p2(mPos.x - 8.0f, mPos.y + 8.0f, 0.0f);
            gBgRewrite_8053AC98->fn_802A3948(&p2, player, field_0x4, 0xC);
            JGeometry::TVec3f p3(mPos.x + 8.0f, mPos.y + 8.0f, 0.0f);
            gBgRewrite_8053AC98->fn_802A3948(&p3, player, field_0x4, 0xC);
            if (field_0x255 == 0) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E3, 0, -2, NULL, NULL, true, 1.0f);
                field_0x255 = 1;
            }
        } else {
            box->i.x -= 1.0f;
            box->i.y -= 1.0f;
            box->f.x += 1.0f;
            box->f.y += 1.0f;
            if (!fn_8023B0F4(player, box, field_0x4, false, 0.0f)) {
                field_0x234[player] = 0;
            }
        }
    }
}

// 0x80227290. The serial of a live actor on the warp's layer (not excludeSerial, one of `ids`, within
// maxDz in z unless the layer ignores z) whose box overlaps the warp's, or -1.
// FIXME(shotgun): 70.73 %, missing/extra; behaviour drift.
s32 Warp_804A6DC8::fn_80227290(f32 maxDz, const u32* ids, s32 excludeSerial) {
    s32 found = -1;
    u8 anyZ = viewLayerFlag(field_0x4);
    s32 end = getActorManager_801F856C()->mActorEnd;
    for (s32 i = 0; i < end; i++) {
        ActorBase_804A5770* other = getActorManager_801F856C()->mActors[i];
        if (other == NULL || !other->mAlive || other->field_0x4 != field_0x4 || excludeSerial == other->mSerial) {
            continue;
        }
        if (!anyZ && __fabsf(mPos.z - other->mPos.z) > maxDz) {
            continue;
        }
        for (const u32* id = ids; *id != 'NULL'; id++) {
            if (*id == other->mActorId) {
                JGeometry::TBox2f box;
                JGeometry::TBox2f otherBox;
                toScreenBox(&box, mBox, mPos);
                toScreenBox(&otherBox, other->mBox, other->mPos);
                if (boxesOverlap(box, otherBox)) {
                    found = other->mSerial;
                    break;
                }
            }
        }
        if (found >= 0) {
            break;
        }
    }
    return found;
}

// 0x80226FC8. Per player: finds the 'YASI' actor the player carries and, once it is moving and
// inside the warp's box, drops it onto the pad (four tiles around the warp are rewritten).
// FIXME(shotgun): 90.85 %, missing/extra; behaviour near.
void Warp_804A6DC8::fn_80226FC8(s32 player) {
    if (field_0x255 == 1) {
        return;
    }
    if (field_0x27c[player] == NULL) {
        if (fn_8023EFEC(player) && fn_8023EEB4(player) == 'YASI') {
            field_0x27c[player] = getActorManager_801F856C()->findActorBySerial_801F666C(fn_8023EF50(player));
        }
        return;
    }
    ActorBase_804A5770* carried = field_0x27c[player];
    if (!carried->mAlive) {
        field_0x27c[player] = NULL;
        return;
    }
    if (carried->field_0x3c.squared() <= 32.0f * FLT_EPSILON) {
        return;
    }
    if (!(carried->mPos.z < 4.0f)) {
        return;
    }
    JGeometry::TVec3f rel = carried->mPos - mPos;
    bool inside = false;
    if (rel.x >= -20.0f && rel.y >= -20.0f && rel.x < 20.0f && rel.y < 20.0f) {
        inside = true;
    }
    if (!inside) {
        return;
    }
    field_0x27c[player]->vfunc_0x48();
    field_0x27c[player] = NULL;
    JGeometry::TVec3f p0(mPos.x - 8.0f, mPos.y - 8.0f, 0.0f);
    gBgRewrite_8053AC98->fn_802A3948(&p0, player, field_0x4, 0xC);
    JGeometry::TVec3f p1(mPos.x + 8.0f, mPos.y - 8.0f, 0.0f);
    gBgRewrite_8053AC98->fn_802A3948(&p1, player, field_0x4, 0xC);
    JGeometry::TVec3f p2(mPos.x - 8.0f, mPos.y + 8.0f, 0.0f);
    gBgRewrite_8053AC98->fn_802A3948(&p2, player, field_0x4, 0xC);
    JGeometry::TVec3f p3(mPos.x + 8.0f, mPos.y + 8.0f, 0.0f);
    gBgRewrite_8053AC98->fn_802A3948(&p3, player, field_0x4, 0xC);
    if (field_0x255 == 0) {
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3E3, 0, -2, NULL, NULL, true, 1.0f);
        field_0x255 = 1;
    }
}
