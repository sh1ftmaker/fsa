#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_enemy_for_ending_control.h"

// .sdata2 floats, read only by fn_8035BE00.
extern const f32 lbl_805428E8;
extern const f32 lbl_805428EC;

// .sdata2 floats: the pillar position's x and y, and its z.
extern const f32 lbl_805428D0;
extern const f32 lbl_805428D4;

// Phase handler in lbl_804AD3A0 (read by fn_8035C650).
void EnemyForEndingControl_804AD48C::fn_8035B650() {
    if (field_0x234 == 0) {
        fn_80119DD0()->fn_80119998(0x96, 0, 1, 0, 0, 0xFA, 0);
        field_0x234++;
        field_0x260 = 0x11C;
    } else if (field_0x260 == 0) {
        if (mEmitter298 != NULL) {
            mEmitter298->setStatus(JPAEMIT_ForceDelete);
            mEmitter298 = NULL;
        }
        if (mEmitter29C != NULL) {
            mEmitter29C->setStatus(JPAEMIT_ForceDelete);
            mEmitter29C = NULL;
        }
        if (mEmitter2A0 != NULL) {
            mEmitter2A0->setStatus(JPAEMIT_ForceDelete);
            mEmitter2A0 = NULL;
        }
        mAlive = false;
    } else if (field_0x260 == 1) {
        fn_801F856C()->fn_801F9484(ACTOR_ENDM, field_0x4, &mPos, 0, -1, -1);
        JGeometry::TVec3f pos(lbl_805428D0, lbl_805428D0, lbl_805428D4);
        for (s32 i = 0; i < 4; i++) {
            fn_80230498(i, &pos, 0, 0xFF, -0x80, -0x80, true);
        }
    }
}

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

// .rodata string, the "SOB " bank draw() shows.
static const char lbl_80472700[] = "enemy_gc_sob.bin";

// Draws the sprite with mPos raised by lbl_805428EC for this call only.
void EnemyForEndingControl_804AD48C::draw() {
    if (field_0x238 != 0) {
        JGeometry::TVec3f saved(mPos);
        mPos.y += lbl_805428EC;
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -1;
        fn_801F06F0(lbl_80472700, 0x179, &params, -1, lbl_805428EC, lbl_805428EC);
        mPos = saved;
    }
}

EnemyForEndingControl_804AD48C::~EnemyForEndingControl_804AD48C() {}

void EnemyForEndingControl_804AD48C::vfunc_0x10() {}

// Empty callback in the static table lbl_804AD3A0 (read by fn_8035C650).
extern "C" void fn_8035BE20() {}
