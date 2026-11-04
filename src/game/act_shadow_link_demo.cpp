#include "game/GameAudio.h"
#include "game/act/act_shadow_link_demo.h"

cls_8049DB90::cls_8049DB90() {
    field_0x230 = 1;
    field_0x231 = 0;
    field_0x234 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x25c = 0.0f;
    field_0x258 = 0.0f;
}

cls_8049DB90::~cls_8049DB90() {}

s32 cls_8049DB90::vfunc_0x58() {
    return field_0x238;
}

s32 cls_8049DB90::vfunc_0x5C() {
    switch (field_0x238) {
    case 12:
    case 21:
    case 24:
    case 33:
    case 34:
        return 1;
    }
    return 0;
}
