#include "game/GameAudio.h"
#include "game/act/act_warp.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include <float.h>

Warp_804A6DC8::~Warp_804A6DC8() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
}

extern const f32 lbl_8046AC58[24]; // x shifts of the sand while the warp shakes

// 0x80227CE8.
// FIXME(shotgun): 90.15 %, missing/extra + reloc-name
void Warp_804A6DC8::draw() {
    if ((field_0x90 & 0x7F) == 0x20 && field_0x255 == 0) {
        SpriteParams_801F5AE0 params;
        JGeometry::TVec3f saved = mPos;
        mPos.x += -16.0f;
        mPos.y += -16.0f;
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

// The box `ofs` (relative to pos) in top-down screen space (x, y - z).
static inline void toScreenBox(JGeometry::TBox2f* out, const JGeometry::TBox2f& ofs, const Vec& pos) {
    out->set(ofs.i.x + pos.x, ofs.i.y + pos.y - pos.z, ofs.f.x + pos.x, ofs.f.y + pos.y - pos.z);
}

// 0x802278A8. Per player: the player's reaction to standing in the warp's box (by warp kind, the
// low 7 bits of the spawn parameter): marks it, hurts it, or sends it through (fn_802317F0).
// FIXME(shotgun): 79.60 %, missing/extra
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
// FIXME(shotgun): 77.69 %, missing/extra
void Warp_804A6DC8::fn_80227530(JGeometry::TBox2f* box) {
    for (s32 player = 0; player < 4; player++) {
        fn_80226FC8(player);
        *box = mBox;
        box->addPos(JGeometry::TVec2<f32>(mPos.x, mPos.y));
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
// FIXME(shotgun): 64.46 %, missing/extra
s32 Warp_804A6DC8::fn_80227290(f32 maxDz, const u32* ids, s32 excludeSerial) {
    s32 found = -1;
    u8 anyZ = viewLayerFlag(field_0x4);
    ActorManager_804A5800* mgr = getActorManager_801F856C();
    for (s32 i = 0; i < mgr->mActorEnd; i++) {
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
// FIXME(shotgun): 85.51 %, missing/extra
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
    f32 speed = carried->field_0x3c.x * carried->field_0x3c.x + carried->field_0x3c.y * carried->field_0x3c.y +
                carried->field_0x3c.z * carried->field_0x3c.z;
    if (speed <= 32.0f * FLT_EPSILON) {
        return;
    }
    if (carried->mPos.z >= 4.0f) {
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
    carried->vfunc_0x48();
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
