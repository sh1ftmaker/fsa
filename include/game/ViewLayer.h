#ifndef GAME_VIEWLAYER_H
#define GAME_VIEWLAYER_H

#include "game/GameView.h"

// GameView byte 0x131DF + layer (fn_802D6A9C()->field_0x131df[layer]),
// inlined in 411 functions of 180 units (bl fn_802D6A9C; addis 1; add;
// lbz 0x31DF). The callers load the layer before fn_802D6A9C is called, as
// for an argument, so the layer is this helper's parameter. It is set for
// the layers where landing goes through vfunc_0x80 (the floor check,
// tu_801CDC8C 0x801CE1C8) and where tu_802242F4's actors skip the z test.
inline u8 viewLayerFlag(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    return view->field_0x131df[layer];
}

#endif
