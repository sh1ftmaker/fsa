#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameState.h"
#include "game/Message.h"
#include "JSystem/J2D/J2DScreen.h"
#include "JSystem/J2D/J2DGrafContext.h"
#include "JSystem/J2D/J2DPicture.h"
#include <string.h>
#include "game/tu_80379F84.h"
#include "game/tu_803B8C74.h"
#include "JSystem/JMath/JMath.h"
#include "game/cls_804B1288.h"
#include "game/act/tu_80440BF4.h"
#include "game/StageFlag.h"
#include "game/cls_804B1B60.h"
#include "game/cls_804B1600.h"
#include "game/cls_804B20F8.h"
#include "game/EmitterDelete.h"
#include "JSystem/J2D/J2DAnimation.h"

// Read by fn_804054B8 and fn_80405D00 (lbzx), and fn_8040B5A0 (lhax).
static const u8 lbl_80543880[8] = {3, 1, 1, 0, 1, 1, 0, 1};
static const u8 lbl_80543888[8] = {1, 3, 3, 2, 3, 3, 2, 3};
static const s16 lbl_80543890[4] = {0, 1, 2, 1};

bool fn_804149C0() {
    return lbl_8050EC80.getCurrentFile().field_0x67 & 1;
}

void fn_8041499C(JUtility::TColor* dst, const JUtility::TColor* src) {
    *dst = *src;
}

MapScreen_804B1288::~MapScreen_804B1288() {
    lbl_8053AEC4->fn_8042EB14();
    for (int i = 0; i < 4; i++) {
        deleteEmitter(&field_0x2cc[i]);
        deleteEmitter(&field_0x2dc[i]);
        if (field_0x2f8[i] != NULL) {
            field_0x2f8[i]->becomeInvalidEmitter();
            field_0x2f8[i] = NULL;
        }
    }
    for (int i = 0; i < 3; i++) {
        deleteEmitter(&field_0x2ec[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (field_0x308[i] != NULL) {
            field_0x308[i]->becomeInvalidEmitter();
            field_0x308[i] = NULL;
        }
    }
    delete field_0x4;
    field_0x4 = NULL;
    delete field_0x8;
    field_0x8 = NULL;
    delete field_0xc;
    field_0xc = NULL;
    delete field_0x1c;
    field_0x1c = NULL;
    delete field_0x20;
    field_0x20 = NULL;
    delete field_0x10;
    field_0x10 = NULL;
    delete field_0x14;
    field_0x14 = NULL;
    delete field_0x18;
    field_0x18 = NULL;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            delete field_0x28[i][j];
            field_0x28[i][j] = NULL;
        }
    }
    for (int i = 0; i < 15; i++) {
        delete field_0x278[i];
        field_0x278[i] = NULL;
    }
    delete field_0xc8;
    field_0xc8 = NULL;
    delete field_0xcc;
    field_0xcc = NULL;
    delete field_0xd0;
    field_0xd0 = NULL;
    for (int i = 0; i < 18; i++) {
        delete field_0xd4[i];
        field_0xd4[i] = NULL;
    }
    delete field_0x11c;
    field_0x11c = NULL;
    for (int i = 0; i < 2; i++) {
        delete field_0x120[i];
        field_0x120[i] = NULL;
    }
    for (int i = 0; i < 32; i++) {
        delete field_0x128[i];
        field_0x128[i] = NULL;
    }
    for (int i = 0; i < 24; i++) {
        delete field_0x1a8[i];
        field_0x1a8[i] = NULL;
    }
    for (int i = 0; i < 8; i++) {
        delete field_0x208[i];
        field_0x208[i] = NULL;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            delete field_0x228[i][j];
            field_0x228[i][j] = NULL;
        }
    }
    delete field_0x238;
    field_0x238 = NULL;
    delete field_0x23c;
    field_0x23c = NULL;
    for (int i = 0; i < 12; i++) {
        delete field_0x240[i];
        field_0x240[i] = NULL;
    }
    delete field_0x270[0];
    field_0x270[0] = NULL;
    delete field_0x270[1];
    field_0x270[1] = NULL;
    for (int i = 0; i < 4; i++) {
        delete field_0x2b8[i];
        field_0x2b8[i] = NULL;
    }
    delete field_0x2c8;
    field_0x2c8 = NULL;
    lbl_8053ADF8 = NULL;
}

void MapScreen_804B1288::fn_80411A94(f32 alpha) {
    field_0x10->search(0x54746C763131ull /* "Ttlv11" */)->setAlpha(field_0x64b * alpha);
    field_0x10->search(0x54746C763132ull /* "Ttlv12" */)->setAlpha(field_0x64c * alpha);
    field_0x10->search(0x54746C763133ull /* "Ttlv13" */)->setAlpha(field_0x64b * (1.0f - alpha));
    field_0x10->search(0x54746C763134ull /* "Ttlv14" */)->setAlpha(field_0x64c * (1.0f - alpha));
}

void MapScreen_804B1288::fn_804111C4() {
    if (field_0x350 == 0x15) {
        fn_8037AA7C();
    }
    gGameRoot2_8053AB10->fn_8013B490();
    field_0x18->setScissor(true);
    field_0x18->draw(0.0f, 0.0f, gGameRoot2_8053AB10->field_0x10);
    lbl_8053AED8->fn_80440EE8();
}

s32 fn_8040F474() {
    return lbl_8053ADF8->field_0x668;
}

void MapScreen_804B1288::fn_8040F3AC() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            field_0x228[i][j]->fn_80402940(0x14, 0, -150.0f, 0.0f);
        }
    }
    field_0x238->fn_80402940(0x14, 0, -150.0f, 0.0f);
    field_0x23c->fn_80402940(0x14, 0, -150.0f, 0.0f);
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
}

void MapScreen_804B1288::fn_8040F35C() {
    if (field_0x11c->fn_80402940(0x14, 0, 200.0f, 0.0f)) {
        field_0x350 = 0xB;
    }
}

// FIXME: 91.7. The original keeps -250 + 250 * t * t unfused (fmuls, fmuls,
// fadds); ours fuses it into an fmadds.
void MapScreen_804B1288::fn_8040F288() {
    f32 t = (f32)field_0x358 / 90.0f;
    field_0x39c = -250.0f + 250.0f * t * t;
    if (field_0x39c >= 0.0f) {
        field_0x39c = 0.0f;
        field_0x358 = 0;
        if (field_0x65d == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        field_0x350 = 0xF;
    }
}

// FIXME: 92.84. scheduling+regalloc only: the original keeps `this` in r31 and the
// counter in r29 (loaded in the prologue after the saves); ours swaps them.
void MapScreen_804B1288::fn_8040E720() {
    int gone = 0;
    field_0x35c++;
    if (field_0x35c > 10) {
        if (field_0x35c == 22) {
            field_0x2c8->startSound(JAISoundID(0x30039));
        }
        for (int i = 0; i < 3; i++) {
            if (field_0x2ec[i] != NULL) {
                if (field_0x2ec[i]->isEnableDeleteEmitter()) {
                    JPAEmitterManager* manager = getEffectSystem_8013C824()->mEmitterManager;
                    manager->forceDeleteEmitter(field_0x2ec[i]);
                    gone++;
                    field_0x2ec[i] = NULL;
                }
            } else {
                gone++;
            }
        }
    }
    if (gone == 3) {
        field_0x35c = 0;
        field_0x664 = 0;
        field_0x350 = 8;
    }
}

void MapScreen_804B1288::fn_8040E700() {
    if (field_0x65a == 0) {
        field_0x664 = 0;
        field_0x350 = 8;
    }
}

void MapScreen_804B1288::fn_8040E298() {
    field_0x42c++;
    if (field_0x42c > 60) {
        if (field_0x664 != 0) {
            field_0x664 = 2;
            field_0x350 = 6;
        } else if (fn_80404270(field_0x340)) {
            field_0x658 = 1;
            field_0x350 = 0x10;
            if (field_0x65d == 1) {
                for (int i = 0; i < 15; i++) {
                    field_0x278[i]->field_0x26 = 0;
                    field_0x65d = 0;
                }
            }
            field_0x4->fn_804446F0(0);
            gAudioMgr_8053AF80->fn_80458880(0x56, -1);
        } else {
            field_0x350 = 0x15;
        }
    }
}

void MapScreen_804B1288::fn_8040E278() {
    if (++field_0x384 > 100) {
        field_0x350 = 0x15;
    }
}

void MapScreen_804B1288::fn_80409330() {
    fn_8037AA34();
    if (fn_8037AA28() == 0x1A) {
        if (field_0x65d == 0) {
            for (int i = 0; i < 15; i++) {
                field_0x278[i]->field_0x26 = 0;
                field_0x65d = 1;
            }
        }
        field_0x350 = 0xF;
    } else if (fn_8037AA28() == 0x19) {
        field_0x668 = 1;
    }
    if (field_0x663 != 0) {
        field_0x434 += 0x200;
        field_0x436 += 0x400;
        field_0x3a0 = 15.0f * JMath::sincosTable_.sinShort(field_0x434);
        field_0x3a4 = 5.0f * JMath::sincosTable_.sinShort(field_0x436);
    }
}

void MapScreen_804B1288::fn_80404D70() {
    field_0x370 = 0;
    if (fn_803B8EBC(7)) {
        for (int stage = 1; stage < 7; stage++) {
            hideStagePanes(stage);
        }
        field_0xcc->mPane->show();
        field_0x370 = 7;
        return;
    }
    for (int i = 0; i < 8; i++) {
        if (fn_803B8EBC(i)) {
            s32 stage = i + 1;
            if (lbl_8050EC80.getCurrentFile().field_0x62[0] & (u8)(1 << stage)) {
                hideStagePanes(stage);
                field_0x378 += 200;
                field_0x37c += 200;
            } else {
                field_0x364 = stage;
            }
            field_0x370++;
        } else {
            return;
        }
        if (i == 5) {
            field_0xcc->mPane->show();
        }
    }
}

// Dead-stripped: fn_80404D70 inlines it in both of its loops.
void MapScreen_804B1288::hideStagePanes(s32 stage) {
    switch (stage) {
    case 1:
        field_0xc->search('P21')->hide();
        field_0xc->search('P22')->hide();
        field_0xc->search('P23')->hide();
        break;
    case 2:
        field_0xc->search('P31')->hide();
        field_0xc->search('P32')->hide();
        field_0xc->search('P33')->hide();
        field_0xc->search('P34')->hide();
        break;
    case 3:
        field_0xc->search('P41')->hide();
        field_0xc->search('P42')->hide();
        field_0xc->search('P43')->hide();
        break;
    case 4:
        field_0xc->search('P51')->hide();
        field_0xc->search('P52')->hide();
        field_0xc->search('P53')->hide();
        break;
    case 5:
        field_0xc->search('P61')->hide();
        field_0xc->search('P62')->hide();
        break;
    case 6:
        field_0xc->search('P71')->hide();
        field_0xc->search('P72')->hide();
        field_0xc->search('P73')->hide();
        break;
    }
}

BOOL MapScreen_804B1288::fn_804045F8(s32 bit) {
    if (gGameRoot_8053AAF8->field_0x7e) {
        return (lbl_8050EC80.getCurrentFile().field_0x5a[0] & (u8)(1 << bit)) ? TRUE : FALSE;
    }
    return FALSE;
}

bool MapScreen_804B1288::fn_80404270(s32 stage) {
    if (lbl_80529DEC.field_0x23e[1] == 0 && gGameRoot_8053AAF8->field_0x7e && field_0x656 == 0 &&
        !fn_804045F8(stage))
    {
        if ((stage == 0 && isStageFlagSet(stage, 1)) || (stage == 1 && isStageFlagSet(stage, 1)) ||
            (stage == 2 && isStageFlagSet(stage, 1)) || (stage == 3 && isStageFlagSet(stage, 1)) ||
            (stage == 4 && isStageFlagSet(stage, 1)) || (stage == 5 && isStageFlagSet(stage, 1)) ||
            (stage == 6 && isStageFlagSet(stage, 1)) || (stage == 7 && isStageFlagSet(stage, 2)))
        {
            return true;
        }
    }
    return false;
}

// FIXME: 84.17. Every field, call and branch matches; only the loop differs: ours
// hoists the save row's flags address out of the unrolled loop, the original keeps
// only the row index (r12) and recomputes lbl_8050EC80 + index * 0xC8 each pass,
// which costs it r31 and a stack frame (the same as tu_804023D0's fn_80403514).
bool MapScreen_804B1288::fn_80404074(s32 stage, s32 index, s32* out) {
    s32 count = 0;
    for (int i = 0; i < 4; i++) {
        if (isStageFlagSet(stage, i)) {
            count++;
        }
    }
    s32 skip;
    if (stage == 7) {
        if (count <= 1) {
            count--;
        }
        skip = 1;
        if (field_0x388 != -1 && (field_0x350 == 6 || field_0x664 == 1)) {
            if (field_0x388 == 0) {
                *out = 2;
            } else if (field_0x388 == 1) {
                *out = 3;
            }
        }
    } else {
        if (count <= 2) {
            count--;
        }
        skip = 2;
        if (field_0x388 != -1 && (field_0x350 == 6 || field_0x664 == 1)) {
            if (field_0x388 == 0) {
                *out = 1;
            } else if (field_0x388 == 1) {
                *out = 3;
            }
        }
    }
    if (index != skip && (index > count || index == *out)) {
        return true;
    }
    return false;
}

void MapScreen_804B1288::fn_80403F50() {
    if (field_0x65a) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        fn_803837A4(field_0x5dc, field_0x390, 0, 0, field_0x270[0]->field_0x18, 1.0f);
        for (s32 i = 0; i < 2; i++) {
            strcpy(((J2DTextBox*)field_0x270[i]->mPane)->getStringPtr(), field_0x5dc);
        }
        for (s32 i = 0; i < 12; i++) {
            field_0x240[i]->moveFromCentre_80402E08(0.0f, 100.0f);
        }
        field_0x659 = 1;
    }
}

void MapScreen_804B1288::fn_80403EAC() {
    u8 done = field_0x240[0]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    for (s32 i = 1; i < 12; i++) {
        field_0x240[i]->fn_80402940(0x13, 0, 100.0f, 0.0f);
    }
    if (done) {
        field_0x42a = 100;
        field_0x659 = 2;
    }
}

void MapScreen_804B1288::fn_80403E24() {
    field_0x42a--;
    if (field_0x42a <= 0) {
        for (int i = 0; i < 12; i++) {
            field_0x240[i]->field_0x22 = 0;
        }
        field_0x659 = 3;
    }
}

void MapScreen_804B1288::fn_80403D84() {
    u8 done = field_0x240[0]->fn_80402940(0xB, 0, 0.0f, 100.0f);
    for (s32 i = 1; i < 12; i++) {
        field_0x240[i]->fn_80402940(0xB, 0, 0.0f, 100.0f);
    }
    if (done) {
        field_0x65a = 0;
        field_0x659 = 0;
    }
}

// Slides field_0x2b4 from 200 above its place down to it and fades it in, eased by
// (frame / frames)^2. Dead-stripped: fn_804108B4 inlines it twice (sliding in and
// out); its own copy created the unit's first .sdata2 literals (0.0f, -200.0f,
// 200.0f and the two int-to-float constants) before fn_80403D84.
void MapScreen_804B1288::slidePane(s16 frame, s16 frames) {
    f32 t = (f32)(frame * frame) / (f32)(frames * frames);
    field_0x2b4->moveFromCentre_80402E08(0.0f, -200.0f + 200.0f * t);
    field_0x2b4->mPane->setAlpha(t * field_0x2b4->field_0x28);
}
