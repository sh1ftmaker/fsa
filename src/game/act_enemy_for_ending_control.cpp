#include "game/GameAudio.h"
#include "game/act/act_enemy_for_ending_control.h"

// .sdata2 floats, read only by fn_8035BE00.
extern const f32 lbl_805428E8;
extern const f32 lbl_805428EC;

void EnemyForEndingControl_804AD48C::fn_8035BE00() {
    if (field_0x254 > lbl_805428E8) {
        field_0x254 -= lbl_805428EC;
    }
}

EnemyForEndingControl_804AD48C::~EnemyForEndingControl_804AD48C() {}

void EnemyForEndingControl_804AD48C::vfunc_0x10() {}

// Empty callback in the static table lbl_804AD3A0 (read by fn_8035C650).
extern "C" void fn_8035BE20() {}
