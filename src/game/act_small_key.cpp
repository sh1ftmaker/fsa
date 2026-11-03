#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/act/act_small_key.h"

// The 1.0f in .sdata2 is named (lbl_80540C7C) in the original, which needs an
// array here: a scalar const is folded into an anonymous literal.
static const f32 lbl_80540C7C[1] = {1.0f};

SmallKey_804A8CD8::~SmallKey_804A8CD8() {}

bool SmallKey_804A8CD8::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x288 == 0) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            return true;
        }
        return fn_802C1970(layer, pos);
    }
    field_0x288--;
    return false;
}

void SmallKey_804A8CD8::fn_802C1D30(const JGeometry::TVec3f& pos, s32 layer) {
    field_0x260 = pos;
    field_0x26c = layer;
    s32 row = fn_802D6A9C()->field_0x18;
    s32 col = fn_802D6A9C()->field_0x14;
    field_0x2aa = ((col & 0xFF) << 8) | (row & 0xFF);
}

void SmallKey_804A8CD8::fn_802C2BD0() {
    field_0x2b0 = true;
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
}

void SmallKey_804A8CD8::fn_802C2B64() {
    if (field_0x240 == 2) {
        field_0x2b0 = true;
        JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
        s32 layer = field_0x238;
        fn_802D6A9C()->fn_802D6C10(pos, layer);
    }
}

void SmallKey_804A8CD8::fn_802C1D90() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

void SmallKey_804A8CD8::fn_802C1DFC() {
    s32 layer = field_0x4;
    if (layer >= 8) {
        fn_801F856C()->fn_801F9484(ACTOR_KMRI, layer, &mPos, 0, -1, -1);
    } else {
        fn_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, lbl_80540C7C[0]);
    }
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

void SmallKey_804A8CD8::fn_802C5284() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
}
