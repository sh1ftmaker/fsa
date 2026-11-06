#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/act/act_attack_hitbox_when_player_is_thrown.h"

void AttackHitboxWhenPlayerIsThrown_804A7C40::init() {
    mBox.set(-10.0f, -10.0f, 10.0f, 10.0f);
    field_0x80.set(-4.0f, -4.0f, 4.0f, 4.0f);
    field_0x230 = field_0x90 & 3;
}

void AttackHitboxWhenPlayerIsThrown_804A7C40::update() {
    mPos.set(*getPlayerPos_8023E724(field_0x230));
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    s32 damage = gGameRoot_8053AAF8->field_0x7f ? 0 : field_0x114;
    for (s32 i = 0; i < 4; i++) {
        if (fn_80236BEC(i) || fn_8022E438(field_0x110, i) || fn_8022E438(field_0x230, i)) {
            continue;
        }
        if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
            // fn_80238548 reads only x and y of the position.
            fn_80238548(i, damage, (const JGeometry::TVec2f*)&mPos, 4);
            vfunc_0x24();
            return;
        }
    }
    if (fn_801F2B20(0) || mPos.z <= 0.0f) {
        vfunc_0x24();
    }
}

void AttackHitboxWhenPlayerIsThrown_804A7C40::draw() {}

void AttackHitboxWhenPlayerIsThrown_804A7C40::vfunc_0x24() {
    mAlive = false;
}

bool AttackHitboxWhenPlayerIsThrown_804A7C40::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    return false;
}

AttackHitboxWhenPlayerIsThrown_804A7C40::~AttackHitboxWhenPlayerIsThrown_804A7C40() {}
