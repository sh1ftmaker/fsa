#ifndef GAME_VIEWLAYER_H
#define GAME_VIEWLAYER_H

#include "game/GameView.h"

// GameView byte 0x131DF + layer (getGameView_802D6A9C()->field_0x131df[layer]),
// inlined in 411 functions of 180 units (bl getGameView_802D6A9C; addis 1; add;
// lbz 0x31DF). The callers load the layer before getGameView_802D6A9C is called, as
// for an argument, so the layer is this helper's parameter. It is set for
// the layers where landing goes through vfunc_0x80 (the floor check,
// tu_801CDC8C 0x801CE1C8) and where tu_802242F4's actors skip the z test.
inline u8 viewLayerFlag(s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    return view->field_0x131df[layer];
}

// The view's box for `layer`: mVisibleBox below layer 8, else field_0xdc.
// Inlined by tu_801EE24C and the Player units (tu_8023FBF0, tu_8025EB18,
// tu_8026FC20, tu_802684A4).
inline JGeometry::TBox2f* layerBox(s32 layer) {
    GameView_804A9950* view = getGameView_802D6A9C();
    return layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
}

#endif
