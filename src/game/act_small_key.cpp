#include "game/GameAudio.h"
#include "game/GameView.h"
#include "game/act/act_small_key.h"

SmallKey_804A8CD8::~SmallKey_804A8CD8() {}

void SmallKey_804A8CD8::fn_802C5284() {
    JGeometry::TVec2f pos(field_0x230.x, field_0x230.y);
    s32 layer = field_0x238;
    fn_802D6A9C()->fn_802D6C10(pos, layer);
}
