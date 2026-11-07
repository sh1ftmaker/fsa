#include "game/GameAudio.h"
#include "game/act/act_warp.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/Player.h"
#include <float.h>

Warp_804A6DC8::~Warp_804A6DC8() {
    releaseEmitter_801EE434(mEmitter24c);
    releaseEmitter_801EE434(mEmitter250);
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
