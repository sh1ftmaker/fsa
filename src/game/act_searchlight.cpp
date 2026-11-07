#include "game/GameAudio.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "JSystem/JMath/JMath.h"
#include "game/act/act_searchlight.h"
#include "JSystem/JUtility/TColor.h"

// Two dummy ids searchlights share with tu_8018A8D8 (both -1 until a searchlight is made).
s32 lbl_80539F80 = -1;
s32 lbl_80539F84 = -1;

// 0x80179F64.
// FIXME(shotgun): 99.75 %, reloc-name
void Searchlight_804A0FF8::init() {
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x328 = (field_0x90 >> 4) & 0xF;
    field_0x32c = (field_0x90 >> 8) & 0xF;
    field_0x330 = (field_0x90 >> 12) & 0xF;
    field_0x334 = (field_0x90 >> 16) & 0xFF;
    field_0x90 = field_0x90 & 0xF;
    field_0x338 = 0;
    field_0x33c = 0;
    lbl_804C0EA0[2] = 0.0f;
    lbl_804C0EA0[1] = 0.0f;
    lbl_804C0EA0[0] = 0.0f;
    lbl_8053AB68 = -1;
    lbl_80539F80 = -1;
    lbl_80539F84 = -1;
    field_0x30c = 0;
    field_0x310 = 0;
    field_0x314 = 0;
    field_0x318 = 0;
    field_0x31c = 0;
    field_0x320 = 0;
    field_0x300 = 0;
    field_0x304 = 0;
    field_0x308 = 0;
    field_0x324 = 0;
    field_0x344 = 32.0f;
    field_0x370.z = 0.0f;
    field_0x370.y = 0.0f;
    field_0x370.x = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x34c.x = mPos.x;
    field_0x34c.y = mPos.y;
    field_0x34c.z = mPos.z;
    field_0x358.x = mPos.x;
    field_0x358.y = mPos.y;
    field_0x358.z = mPos.z;
    field_0x364.x = mPos.x;
    field_0x364.y = mPos.y;
    field_0x364.z = mPos.z;
    field_0x348 = 50.0f;
    field_0x30c = 100;
    switch (field_0x90) {
    case 0:
    case 4:
    case 8:
        field_0x98 = 3;
        break;
    case 1:
    case 5:
    case 9:
        field_0x98 = 2;
        break;
    case 2:
    case 6:
    case 10:
        field_0x98 = 1;
        break;
    case 3:
    case 7:
    case 11:
        field_0x98 = 0;
        break;
    }
    if (field_0x90 >= 4 && field_0x90 <= 7) {
        field_0x30c = -100;
    }
    field_0x340 = fn_801F6874(field_0x98);
    if (field_0x90 == 12) {
        field_0x2f8 = 2;
        field_0x2fc = field_0x2f8;
    } else if (field_0x90 == 13) {
        field_0x2f8 = 4;
        field_0x2fc = field_0x2f8;
    } else {
        if (field_0x90 >= 4 && field_0x90 <= 11) {
            mPos.x = field_0x348 * JMath::sincosTable_.sinShort(field_0x340) + field_0x34c.x;
            mPos.y = field_0x348 * JMath::sincosTable_.cosShort(field_0x340) + field_0x34c.y;
            field_0x2f8 = 1;
        } else {
            field_0x300 = 300;
            field_0x370.x = 0.5f * JMath::sincosTable_.sinShort(field_0x340);
            field_0x370.y = 0.5f * JMath::sincosTable_.cosShort(field_0x340);
            field_0x2f8 = 0;
        }
        field_0x2fc = field_0x2f8;
    }
}

// 0x801791B0.
// FIXME(shotgun): 87.37 %, different
void Searchlight_804A0FF8::draw() {
    if (field_0x90 == 0xD) {
        SpriteParams_801F5AE0 params;
        params.field_0x1d = -1;
        drawSprite_801F06F0("enemy_gc_sob.bin", 0x431, &params, -1, 1.0f, 1.0f);
        fn_801F02BC("enemy_gc_sob.bin", 0x431, &params, -1, 0, false, 1.0f, 1.0f);
    } else {
        gGameRoot2_8053AB10->field_0x2c->fn_80158624(&field_0x34c, &mPos, JUtility::TColor(0x3F3F1F7F), 0.7f * field_0x344);
        gGameRoot2_8053AB10->field_0x2c->fn_80158624(&field_0x34c, &mPos, JUtility::TColor(0x3F3F1F3F), field_0x344);
    }
}

// Destructor at 0x80179150.
Searchlight_804A0FF8::~Searchlight_804A0FF8() {}
