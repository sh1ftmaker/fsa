#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/GameGlobals.h"
#include "game/act/act_off_switch.h"

void OFFSwitch_804A9B70::init() {
    field_0x234 = field_0x90 & 0x1F;
    field_0x238 = (field_0x90 >> 22) & 0x1F;
    if (getGameView_802D6A9C()->fn_802D800C(field_0x238)) {
        field_0x23c = true;
    } else {
        field_0x23c = false;
    }
    field_0x230 = 0;
    update();
}

void OFFSwitch_804A9B70::update() {
    switch (field_0x230) {
    case 0:
        switch (field_0x234) {
        case 0:
            if (vfunc_0x34()) {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, false, 0xFF);
                field_0x230 = 1;
            }
            break;
        case 1:
            if (vfunc_0x34()) {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, false, 0xFF);
            } else {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, field_0x23c, 0xFF);
            }
            break;
        case 2:
            if (vfunc_0x34()) {
                if (field_0x23c) {
                    getGameView_802D6A9C()->fn_802D8050(field_0x238, false, 0xFF);
                    field_0x23c = false;
                } else {
                    getGameView_802D6A9C()->fn_802D8050(field_0x238, true, 0xFF);
                    field_0x23c = true;
                }
                field_0x230 = 1;
            }
            break;
        case 3:
            if (vfunc_0x34()) {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, false, 0xFF);
            } else {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, true, 0xFF);
            }
            break;
        case 4:
            if (vfunc_0x34()) {
                getGameView_802D6A9C()->fn_802D8050(field_0x238, false, 0xFF);
            }
            break;
        }
        break;
    case 1:
        switch (field_0x234) {
        case 0:
        case 2:
            if (!vfunc_0x34()) {
                field_0x230 = 0;
            }
            break;
        case 4:
            break;
        }
        break;
    case 2: // nothing in this unit sets state 2
        break;
    }
}

void OFFSwitch_804A9B70::draw() {}

OFFSwitch_804A9B70::~OFFSwitch_804A9B70() {}
