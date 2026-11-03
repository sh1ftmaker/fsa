#include "game/GameAudio.h"
#include "game/act/act_vaati_demo.h"

cls_8049D940::cls_8049D940() : field_0x230(0), field_0x234(0) {
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x32c = 0;
    field_0x31c = 0;
    field_0x320 = 0;
    field_0x324 = 0;
    field_0x328 = 0;
}

cls_8049D940::~cls_8049D940() {
    field_0x31c = 0;
    field_0x320 = 0;
    field_0x324 = 0;
    field_0x328 = 0;
    field_0x32c = 0;
    fn_801EE434(field_0x314);
    field_0x314 = NULL;
    fn_801EE434(field_0x318);
    field_0x318 = NULL;
}

s32 cls_8049D940::vfunc_0x58() {
    return field_0x230;
}
