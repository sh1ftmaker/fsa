#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/act/act_small_key.h"

SmallKey_804A8CD8::~SmallKey_804A8CD8() {}

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

void SmallKey_804A8CD8::fn_802C1D90() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
    mAlive = false;
}

void SmallKey_804A8CD8::fn_802C5284() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
}
