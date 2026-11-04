#include "game/GameAudio.h"
#include "game/act/act_zora_escort_goal.h"

ZoraEscortGoal_8049E424::ZoraEscortGoal_8049E424()
    : field_0x230(0), field_0x234(0), field_0x238(0), field_0x23c(-1), field_0x240(0),
      field_0x244(0), field_0x248(0), field_0x24c(0), field_0x250(0), field_0x254(0xFF),
      field_0x258(0), field_0x259(0), field_0x25c(0.0f), field_0x260(1.0f), field_0x264(0.0f),
      field_0x268(0.0f), field_0x26c(0.0f), field_0x270(0.0f), field_0x274(0.0f),
      field_0x278(0.0f), mAnim27c(), field_0x37c(NULL), field_0x380(NULL) {}

ZoraEscortGoal_8049E424::~ZoraEscortGoal_8049E424() {
    fn_801EE434(field_0x37c);
    fn_801EE434(field_0x380);
    field_0x37c = NULL;
    field_0x380 = NULL;
}

void ZoraEscortGoal_8049E424::draw() {
    ActorBase_804A5770::draw();
    SpriteParams_801F5AE0 params;
    if (field_0x230 == 0xD || field_0x230 == 0xE) {
        params.field_0x14.mColor.r = field_0x254;
        params.field_0x14.mColor.g = field_0x254;
        params.field_0x14.mColor.b = field_0x254;
    }
    fn_801F02BC("enemy_both2_sob.bin", mAnim27c.mCel + 0x242, &params, 0x407, 0, false, 1.0f, 1.0f);
}

s32 ZoraEscortGoal_8049E424::fn_80126EF8() const {
    if (field_0x230 == 1 || field_0x230 == 0xC) {
        return 1;
    }
    return 0;
}
