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

// A step of a sequence table: the value (negative ends the sequence) and the
// time the sequencer waits before reading the next step.
struct EndingStep_804AD460 {
    s32 value;
    s32 time;
};

// Eleven sequence tables, indexed by fn_8035C15C's argument (.data).
extern EndingStep_804AD460* lbl_804AD460[11];

// Advances the sequencer; true once the sequence is over.
bool EnemyForEndingControl_804AD48C::fn_8035C15C(s32 seq) {
    if (field_0x244 == 0) {
        EndingStep_804AD460* steps = lbl_804AD460[seq];
        if (steps[field_0x24c].value < 0) {
            return true;
        }
        field_0x244 = steps[field_0x24c].time;
        field_0x248 = steps[field_0x24c].value;
        field_0x24c++;
    }
    return false;
}

EnemyForEndingControl_804AD48C::~EnemyForEndingControl_804AD48C() {}

void EnemyForEndingControl_804AD48C::vfunc_0x10() {}

// Empty callback in the static table lbl_804AD3A0 (read by fn_8035C650).
extern "C" void fn_8035BE20() {}
