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

// FIXME(shotgun): not written, .rodata tables lbl_80474C38 (string),
// lbl_80474C68 (0x1E0 table), lbl_80474E48 (two filenames) and the .sdata/.sdata2 anim tables are not
// declared: nothing compiled here reads them yet.
// FIXME(shotgun): not written: ctor, dtor, init, vfunc_0x54, vfunc_0x60 (jumptable_804AE184),
// fn_8036AC58, fn_8036AA60, fn_8036A894, fn_8036A65C, fn_80369DDC (jumptable_804AE13C),
// fn_80368CD8 (jumptable_804AE0EC), fn_80368660, fn_803683AC, fn_80368198.

// Functions are listed in descending address order (highest first, lowest last).

s32 cls_804AE088::vfunc_0x58() {
    return field_0x240;
}

// Table lbl_804AE070 is filled by this file's static initializer; its definition is not written yet.
extern void (cls_804AE088::*lbl_804AE070[2])();

void cls_804AE088::update() {
    if (field_0x2f0 != 0) {
        field_0x2f0--;
        if (field_0x2f0 == 0xE1) {
            startSound_801F0E34(0x30082, 0);
        } else if (field_0x2f0 == 0xB4) {
            startSound_801F0E34(0x30082, 0);
            startSound_801F0E34(0x30083, 0);
        }
    }
    (this->*lbl_804AE070[field_0x23c - 4])();
}

const s32 lbl_80474C48[8] = {0x199, 0x1E7, 0x17F, 0x201, 0x1CD, 0x1B3, 0x21B, 0x21B};

void cls_804AE088::draw() {
    JGeometry::TVec3f savedPos = mPos;
    if (field_0x23c == 5 && field_0x240 >= 0x24 && field_0x240 != 0x26) {
        fn_803683AC();
        return;
    }
    cls_8049D140::draw();
    fn_8036A894();
    if (field_0x236 != 0) {
        fn_8036AC58(lbl_80474C48[field_0x23c]);
    }
    fn_80368660();
    mPos = savedPos;
}

s32 cls_804AE088::vfunc_0x5C() {
    s32 s = field_0x240;
    if (s == 0x11 || s == 0x26)
        return 1;
    return 0;
}
