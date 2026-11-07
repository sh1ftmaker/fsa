// Crystal (cutscene) actor: 'DCR2', spawned by tu_801F9484.cpp.
// Declared in include/game/act/tu_801F9484.h (cls_804AE088).
// Inherits from cls_8049D140 (tu_8010DF44).

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_8010DF44.h"
#include "game/SpriteAnim.h"
#include "game/Unk_8045A080.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): not written, .rodata tables lbl_80474C38 (string), lbl_80474C48 (8 s32),
// lbl_80474C68 (0x1E0 table), lbl_80474E48 (two filenames) and the .sdata/.sdata2 anim tables are not
// declared: nothing compiled here reads them yet.
// FIXME(shotgun): not written: ctor, dtor, init, update, draw, vfunc_0x54, vfunc_0x60 (jumptable_804AE184),
// fn_8036AC58, fn_8036AA60, fn_8036A894, fn_8036A65C, fn_80369DDC (jumptable_804AE13C),
// fn_80368CD8 (jumptable_804AE0EC), fn_80368660, fn_803683AC, fn_80368198.

// Functions are listed in descending address order (highest first, lowest last).

// FIXME(shotgun): 100 %, class
s32 cls_804AE088::vfunc_0x58() {
    return field_0x240;
}

// FIXME(shotgun): 100 %, class
s32 cls_804AE088::vfunc_0x5C() {
    s32 s = field_0x240;
    if (s == 0x11 || s == 0x26)
        return 1;
    return 0;
}
