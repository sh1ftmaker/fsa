#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_8011A0F8.h"

// FIXME: 97%. The vfunc_0x34 test ends in `beq end` in the original; here it is
// `bne body; b end`.
void cls_8049DAD8::update() {
    ActorBase_804A5770::update();
    if (!field_0x239) {
        if (field_0x238) {
            field_0x234++;
            if (field_0x234 >= 30) {
                if (fn_8011921C()->field_0x0 || !fn_801186B0(fn_8011921C(), field_0x230)) {
                    return;
                }
                field_0x239 = true;
            }
        }
        if ((field_0x90 & 0xF8000000) && vfunc_0x34()) {
            field_0x238 = true;
        }
    }
}

void cls_8049DAD8::init() {
    ActorBase_804A5770::init();
    field_0x230 = field_0x90 & 0x1F;
}

cls_8049DAD8::~cls_8049DAD8() {}

cls_8049DAD8::cls_8049DAD8() : field_0x230(0), field_0x234(0), field_0x238(false), field_0x239(false) {}
