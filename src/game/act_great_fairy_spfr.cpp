#include "game/GameAudio.h"
#include "game/act/act_great_fairy_spfr.h"

cls_8049EB8C::cls_8049EB8C()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(0),
      field_0x244(0), field_0x248(0), field_0x24c(0), field_0x288(0) {}

cls_8049EB8C::~cls_8049EB8C() {
    fn_801EE434(field_0x288);
    field_0x288 = 0;
}

s32 cls_8049EB8C::vfunc_0x58() {
    return field_0x234;
}
