#include "game/GameAudio.h"
#include "game/act/act_warp.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
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
