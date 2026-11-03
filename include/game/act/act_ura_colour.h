#ifndef GAME_ACT_ACT_URA_COLOUR_H
#define GAME_ACT_ACT_URA_COLOUR_H

// Code: game/act_ura_colour.cpp.

#include "game/ActorBase.h"

// Named after its vtable, 0x804A9A70.
class UraColour_804A9A70 : public ActorBase_804A5770 {
public:
    UraColour_804A9A70() {}
    virtual ~UraColour_804A9A70();
    virtual void init();
    virtual void update();
    virtual void draw();
}; // Size: 0x230

#endif
