// Actor FSPO ("Fuse point" in docs/names/actors.tsv): an EnemyBase_804A4808
// with no fields of its own.
//

#include "game/GameAudio.h"
#include "game/act/act_fuse_point.h"
#include "game/tu_801E9AFC.h"

// Offsets of the four facings (game/tu_80210AF8.cpp, not written yet).
f32 fn_80210B34(s32 dir);
f32 fn_80210AF8(s32 dir);

FusePoint_804A54C0::FusePoint_804A54C0() {}

// FIXME(shotgun): 48.60 %, missing/extra. The mask of the sub-kind (cntlzw trick) and the TVec2 view of mPos are written plainly.
void FusePoint_804A54C0::init() {
    EnemyBase_804A4808::init();
    if ((field_0x90 >> 23) & 1) {
        s32 dir = (field_0x90 >> 21) & 3;
        mSpawnPos.x += fn_80210B34(dir);
        mSpawnPos.y += fn_80210AF8(dir);
        mPos.set(mSpawnPos);
        mPrevPos.set(mSpawnPos);
    }
    mBox.i.x = 1.0f;
    mBox.i.y = 1.0f;
    mBox.f.x = -1.0f;
    mBox.f.y = -1.0f;
    mBox2 = mBox;
    field_0x80.i.x = 1.0f;
    field_0x80.i.y = 1.0f;
    field_0x80.f.x = -1.0f;
    field_0x80.f.y = -1.0f;
    field_0xb0 = 0;
    mFlags |= 0x100;
    u32 param = field_0x90;
    u8 kind = param & 0xFF;
    u8 sub = (kind == 0) ? ((param >> 16) & 0x1F) : 0;
    s32 layer = field_0x4;
    cls_801E9AFC* list = fn_801E9AFC();
    JGeometry::TVec2f pos2(mPos.x, mPos.y);
    list->field_0x0[list->field_0xf0].fn_801EA75C(sub, param >> 24, kind, layer, &pos2);
    mAlive = false;
}

// Does nothing: unlike EnemyBase_804A4808's, no hit checks or movement.
void FusePoint_804A54C0::update() {}

FusePoint_804A54C0::~FusePoint_804A54C0() {}
