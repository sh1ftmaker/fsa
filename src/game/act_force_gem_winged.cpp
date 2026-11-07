#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/act/act_force_gem_winged.h"
#include "JSystem/JMath/JMath.h"
#include "game/Player.h"
#include "dolphin/mtx.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/Effect.h"
#include "game/Message.h"

// Emitter ids by kind (field_0x270) for the three effects of state 1, then the frame
// lengths of the four-frame animation (normal, and while carried in state 6).
static const u16 lbl_80471678[8] = {0x34E, 0x34E, 0x34E, 0x34E, 0x34E, 0x351, 0x351, 0x354};
static const u16 lbl_80471688[8] = {0x34F, 0x34F, 0x34F, 0x34F, 0x34F, 0x352, 0x352, 0x355};
static const u16 lbl_80471698[8] = {0x350, 0x350, 0x350, 0x350, 0x350, 0x353, 0x353, 0x356};
static const s32 lbl_804716A8[4] = {60, 62, 64, 66};
static const s32 lbl_804716B8[4] = {60, 62, 64, 66};

// Sprite frames by kind (field_0x270, 0-7); 8-15 are the frames for states 5 and 6.
s32 lbl_804AB038[16] = {
    0x11, 0x15, 0x19, 0x1D, 0x11, 0x09, 0x0D, 0x05,
    0x11, 0x15, 0x19, 0x1D, 0x11, 0x09, 0x0D, 0x05,
};

// Force gem value by kind (fn_8043D874 / fn_802362C4).
s32 lbl_804AB078[8] = {100, 150, 200, 300, 100, 20, 50, 5};

// The gem is picked up by `player`.
static inline void collect(WingedForceGem_804AB098* self, s32 player) {
    self->field_0x26c = player;
    self->field_0x230 = 5;
    self->field_0xb0 = 0;
}

// Kind 4 shows kind 0 for the first three colour steps, then kind 3.
static inline void fixKind(WingedForceGem_804AB098* self) {
    if (self->field_0x270 == 4 && self->field_0x278 < 3) {
        self->field_0x270 = 0;
    } else if (self->field_0x270 == 4 && self->field_0x278 == 3) {
        self->field_0x270 = 3;
    }
}

// The "force gem got" message, once per view.
static inline void showMessage(s32 who) {
    if (getGameView_802D6A9C()->field_0x4 == 0 && getGameView_802D6A9C()->field_0x8 == 0 &&
        !getGameView_802D6A9C()->fn_802D7F40(0)) {
        lbl_8053AED0->fn_8043D67C(0x13F, who, 0);
        getGameView_802D6A9C()->fn_802D7F84(0, true);
    }
}

// FIXME(shotgun): 95.94 %, missing/extra + reloc-name
void WingedForceGem_804AB098::init() {
    field_0x94 = (field_0x90 >> 16) & 0xFF;
    field_0x270 = field_0x90 & 7;
    field_0x274 = (field_0x90 >> 12) & 1;
    field_0x284 = (field_0x90 >> 14) & 1;
    field_0x286 = (field_0x90 >> 10) & 1;
    field_0x282 = (field_0x90 >> 25) & 1;
    field_0x283 = (field_0x90 >> 26) & 1;
    field_0x25c = 0;
    field_0x260 = 0;
    field_0x264 = 0;
    field_0x268 = 0;
    field_0x24c = 0;
    field_0x250 = 0;
    field_0x254 = 0;
    field_0x258 = 0;
    field_0x288 = 0;
    mLife = 10000;
    field_0x26c = -1;
    field_0x98 = 0;
    field_0x298.x = mPos.x;
    field_0x298.y = mPos.y;
    field_0x298.z = mPos.z;
    field_0x2a4.zero();
    field_0x268 = 100;
    field_0x278 = 0;
    field_0x23c = 1.0f;
    field_0x240 = 0.0f;
    field_0x244 = 0.0f;
    field_0x248 = 0.0f;
    field_0x280 = 0;
    field_0x281 = (field_0x90 >> 24) & 1;
    field_0x285 = (field_0x90 >> 8) & 1;
    field_0x238 = 30.0f;
    field_0x2b0.zero();
    field_0x27c = (s32)(65536.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    field_0x230 = 0;
    field_0x234 = field_0x230;
    field_0x264 = (s32)(65535.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
    mPos.z = 24.0f;
    if (field_0x274 != 0) {
        mPos.z = 8.0f;
        field_0xb0 = 0x20000;
    }
    if (getGameView_802D6A9C()->field_0x131df[field_0x4]) {
        mPos.y -= mPos.z;
        mPos.z = 0.0f;
        field_0xb0 = 0x20000;
    }
    switch (field_0x94) {
    case 0:
        field_0x17c = 0;
        field_0x174 = 50;
        break;
    case 1:
    case 6:
        field_0x98 = 2;
        field_0x98 |= (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 2:
    case 7:
        field_0x98 |= (s32)(1.99f * gGameRoot_8053AAF8->mRandom.get_ufloat_1());
        break;
    case 3:
        field_0x268 = -100;
    case 4:
        mPos.x = field_0x298.x + field_0x238 * JMath::sincosTable_.sinShort(field_0x264);
        mPos.y = field_0x298.y + field_0x238 * JMath::sincosTable_.cosShort(field_0x264);
        break;
    case 5:
        field_0x180 = 60;
        break;
    case 8:
        field_0x230 = 3;
        mPos.z = 0.0f;
        break;
    }
    field_0x28c.x = 0.0f;
    field_0x28c.y = 0.0f;
    field_0x28c.z = 0.0f;
    field_0x3c.x = 0.0f;
    field_0x3c.y = 0.0f;
    field_0x3c.z = 0.0f;
    field_0x80.set(-7.0f, -4.0f, 7.0f, 4.0f);
    if (field_0x94 == 5 || field_0x274 != 0) {
        mBox.set(-7.0f, -23.0f, 7.0f, 7.0f);
    } else {
        mBox.set(-8.0f, -32.0f, 8.0f, 16.0f);
    }
    if (gGameRoot_8053AAF8->field_0x7f == 0) {
        fn_801F3668();
    } else if (field_0x283 == 1 && gGameRoot_8053AAF8->field_0x7e == 0) {
        fn_801F3668();
    } else if (field_0x285 == 1) {
        field_0x234 = field_0x230;
        field_0x230 = 1;
    }
}

// FIXME(shotgun): 93.14 %, missing/extra + reloc-name
void WingedForceGem_804AB098::update() {
    if (field_0x281 == 1) {
        if (vfunc_0x34() == 1) {
            s32 layer = field_0x4;
            if (layer >= 8) {
                getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, &mPos, 3, -1, -1);
            } else {
                spawnEmitter_8013CC50(layer, &mPos, 0x503, 1, 0, NULL, NULL, true, 1.0f);
            }
            fn_801F0D20(0x42);
            field_0x281 = 0;
        }
        return;
    }
    u8 hitWall = 0;
    JGeometry::TVec2f xy;
    xy.x = mPos.x;
    xy.y = mPos.y;
    if (field_0x94 == 0) {
        field_0x17c = 0;
        field_0x174 = 50;
    }
    if (field_0x230 == 0) {
        addCalc2_801F71A4(&field_0x3c.x, field_0x28c.x, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.y, field_0x28c.y, 0.3f, 1.0f);
        addCalc2_801F71A4(&field_0x3c.z, field_0x28c.z, 0.3f, 1.0f);
    }
    if (field_0x230 != 1 && field_0x230 != 2) {
        mPos.add(field_0x3c);
    }
    if (field_0x230 != 5 && field_0x230 != 6) {
        field_0x27c = (s32)(256.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + field_0x27c + 0x200;
        addCalc2_801F71A4(&field_0x2b0.z, 3.0f + 3.0f * JMath::sincosTable_.sinShort(field_0x27c), 0.1f, 0.2f);
    }
    JGeometry::TBox2f box(mBox);
    box.i.x += xy.x;
    box.i.y += xy.y;
    box.f.x += xy.x;
    box.f.y += xy.y;
    if (field_0x230 == 0) {
        JGeometry::TVec3f saved(mPos);
        mPos.y -= field_0x248;
        mPos.y -= mPos.z;
        s32 player = fn_8023AF14(&box, field_0x4, false, mPos.z);
        if (getGameView_802D6A9C()->field_0x131df[field_0x4]) {
            hitWall = fn_801F2618(NULL, false);
        }
        if (player < 0) {
            for (s32 i = 0; i < 4; i++) {
                if (!fn_80230188(i) && !fn_8023EA28(i) &&
                    (fn_8023B9CC(i, &box, field_0x4, &xy, true) == 1 || fn_8023B848(i, &box, field_0x4, &xy) == 1)) {
                    player = i;
                    break;
                }
            }
        }
        if (player >= 0 && !fn_80234C5C(player) && !fn_8022B7C4(player) && !fn_80230188(player) &&
            !fn_8023EA28(player)) {
            if (field_0x94 == 5) {
                if (getGameView_802D6A9C()->field_0x131df[field_0x4]) {
                    collect(this, player);
                } else if (mPos.z - getPlayerPos_8023E724(player)->z <= 20.0f) {
                    collect(this, player);
                }
            } else if (field_0x274 == 0) {
                if (getGameView_802D6A9C()->field_0x131df[field_0x4] || getPlayerPos_8023E724(player)->z > 10.0f ||
                    fn_80234ECC(player)) {
                    collect(this, player);
                }
            } else if (getGameView_802D6A9C()->field_0x131df[field_0x4] || fn_8023ED8C(player) ||
                       fn_8023EBB8(player) || fn_80233DA4(player) || field_0x270 == 5 || field_0x270 == 6 ||
                       field_0x270 == 7) {
                collect(this, player);
            } else if (fn_802308E0(player) == 1) {
                fixKind(this);
                JGeometry::TVec3f pos(*getPlayerPos_8023E724(player));
                JGeometry::TVec3f at(pos.x, pos.y - 12.0f, pos.z);
                if (fn_80233E40(player)) {
                    fn_80230498(player, &pos, 0x336, 0xFF, -0x80, -0x80, true);
                } else {
                    fn_80230498(player, &pos, 0xED, 0xFF, -0x80, -0x80, true);
                }
                s32 who = player;
                if (fn_8022F514(player) == 1) {
                    who = fn_8022F3DC(player);
                }
                showMessage(who);
                getActorManager_801F856C()->spawnActor_801F9484(
                    'IMUP', field_0x4, &at, field_0x270 + 0xA00000 + (player << 24) + (player << 26) + 4, -1, -1);
                fn_801F3668();
                return;
            }
        }
        if (field_0x230 == 0) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_80233D20(i) == 1) {
                    JGeometry::TVec3f a(*getPlayerPos_8023E724(i));
                    a.y -= a.z;
                    a.z = 0.0f;
                    JGeometry::TVec3f b(mPos);
                    b.y -= b.z;
                    b.z = 0.0f;
                    if (PSVECDistance(&a, &b) < 16.0f) {
                        collect(this, i);
                        break;
                    }
                }
            }
        }
        mPos = saved;
    }
    switch (field_0x230) {
    case 0:
        if (field_0x286 == 1) {
            s32 serial = -1;
            bool found = false;
            while ((serial = getActorManager_801F856C()->fn_801F78F4('BOME', serial)) >= 0) {
                ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (bomb == NULL) {
                    continue;
                }
                f32 dx = mPos.x - bomb->mPos.x;
                f32 dy = (mPos.y - mPos.z) - bomb->mPos.y;
                if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) < 48.0f) {
                    found = true;
                    break;
                }
            }
            JGeometry::TVec3f dir;
            f32 dist;
            if (!found) {
                fn_801EE558(&dir, &dist, 0);
            }
            if (found == 1 || dist < 48.0f) {
                addCalc2_801F71A4(&field_0x248, 20.0f, 0.3f, 4.0f);
            } else {
                addCalc2_801F71A4(&field_0x248, 0.0f, 0.05f, 0.2f);
            }
        }
        field_0x260 = (s32)(256.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1()) + field_0x260 + 0x200;
        switch (field_0x94) {
        case 1:
        case 2:
        case 6:
        case 7:
            switch (field_0x98) {
            case 0:
                field_0x28c.set(0.8f, 0.0f, 0.0f);
                break;
            case 1:
                field_0x28c.set(-0.8f, 0.0f, 0.0f);
                break;
            case 2:
                field_0x28c.set(0.0f, 0.8f, 0.0f);
                break;
            case 3:
                field_0x28c.set(0.0f, -0.8f, 0.0f);
                break;
            }
            if (field_0x94 == 1 || field_0x94 == 2) {
                if (field_0x250 == 0) {
                    field_0x250 = 200;
                    field_0x98 ^= 1;
                }
            } else if (field_0x250 == 100) {
                field_0x98 ^= 1;
            } else if (field_0x250 == 0) {
                field_0x250 = 200;
            }
            break;
        case 3:
        case 4:
            field_0x264 += field_0x268;
            mPos.x = field_0x298.x + field_0x238 * JMath::sincosTable_.sinShort(field_0x264);
            mPos.y = field_0x298.y + field_0x238 * JMath::sincosTable_.cosShort(field_0x264);
            break;
        case 5: {
            u8 onWall = getGameView_802D6A9C()->field_0x131df[field_0x4];
            bool hit = false;
            if (onWall && hitWall == 1) {
                hit = true;
            }
            bool high = false;
            if (mPos.z > 50.0f && field_0x98 == 1) {
                high = true;
            }
            if (field_0x250 == 0 || field_0x280 != onWall || hit || high) {
                if (field_0x250 == 0 || hit || high) {
                    if (high) {
                        mPos.z = 49.7f;
                    }
                    field_0x250 = 500;
                    field_0x98 ^= 1;
                }
                if (onWall) {
                    switch (field_0x98) {
                    case 0:
                        field_0x28c.set(0.0f, 0.5f, 0.0f);
                        break;
                    case 1:
                        field_0x28c.set(0.0f, -0.5f, 0.0f);
                        break;
                    }
                } else {
                    switch (field_0x98) {
                    case 0:
                        field_0x28c.set(0.0f, 0.0f, -0.5f);
                        break;
                    case 1:
                        field_0x28c.set(0.0f, 0.0f, 0.5f);
                        break;
                    }
                }
                if (field_0x280 != onWall) {
                    if (onWall) {
                        field_0x3c.y = -field_0x3c.z;
                        field_0x3c.z = 0.0f;
                        mPos.z = 0.0f;
                    } else {
                        field_0x3c.z = -field_0x3c.y;
                        field_0x3c.y = 0.0f;
                        mPos.z = 0.0f;
                    }
                    field_0x280 = onWall;
                }
            }
            break;
        }
        }
        break;
    case 1:
        startSound_801F0E34(0x5006D, 0);
        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80471678[field_0x270], 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80471688[field_0x270], 0, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &mPos, lbl_80471698[field_0x270], 0, 0, NULL, NULL, true, 1.0f);
        field_0x258 = 10;
        field_0x230 = 2;
    case 2:
        if (field_0x258 <= 0) {
            field_0x230 = field_0x234;
        }
        break;
    case 3: {
        field_0x3c.x = field_0x2a4.x - mPos.x;
        field_0x3c.y = field_0x2a4.y - mPos.y;
        f32 dist = JGeometry::TUtil<f32>::sqrt(field_0x3c.x * field_0x3c.x + field_0x3c.y * field_0x3c.y);
        field_0x3c.normalize();
        field_0x3c.z = 248.0f / dist;
        field_0x244 = field_0x3c.z * field_0x3c.z * 0.0078125f;
        mPos.add(field_0x3c);
        field_0x230 = 4;
    }
    case 4: {
        field_0x3c.z -= field_0x244;
        f32 dx = mPos.x - field_0x2a4.x;
        f32 dy = mPos.y - field_0x2a4.y;
        if (JGeometry::TUtil<f32>::sqrt(dx * dx + dy * dy) < 2.0f) {
            mPos.x = field_0x2a4.x;
            mPos.y = field_0x2a4.y;
            field_0x274 = 1;
            field_0x230 = 0;
        }
        break;
    }
    case 5: {
        fixKind(this);
        s32 who = field_0x26c;
        if (fn_8022F514(field_0x26c) == 1) {
            who = fn_8022F3DC(field_0x26c);
        }
        showMessage(who);
        if (field_0x270 != 5 && field_0x270 != 6 && field_0x270 != 7) {
            lbl_8053AED0->fn_8043D874(who, lbl_804AB078[field_0x270], 0);
        } else {
            fn_80230FD0(who, 0x20011);
            fn_802362C4(who, lbl_804AB078[field_0x270]);
        }
        if (getPlayerLayer_8023DE58(field_0x26c) >= 8 && gGameRoot_8053AAF8->field_0x7e == 1 && field_0x270 < 4) {
            lbl_8053AED0->fn_8043D67C(0x194 - field_0x270, who, 0);
        }
        mPos.z = field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        field_0x25c = 0;
        field_0x24c = 50;
        field_0x250 = 30;
        field_0x3c.z = 3.0f;
        field_0x230 = 6;
        fn_801F3604();
    }
    case 6:
        field_0x3c.x = field_0x3c.y = field_0x3c.z = 0.0f;
        if (fn_8023F264(field_0x26c)) {
            mPos.z = getPlayerPos_8023E724(field_0x26c)->z;
            mPos.x = getPlayerPos_8023E724(field_0x26c)->x;
            mPos.y = getPlayerPos_8023E724(field_0x26c)->y - 24.0f;
        }
        if (field_0x250 == 0) {
            vfunc_0x30(0xFF, true);
            fn_801F3668();
        }
        break;
    }
    if (field_0x3c.squared() > 0.05f) {
        u16 id;
        if (field_0x270 == 7) {
            id = 0x347;
        } else if (field_0x270 == 5 || field_0x270 == 6) {
            id = 0x346;
        } else {
            id = 0x345;
        }
        spawnEmitter_8013CC50(field_0x4, &mPos, id, 0, 0, NULL, NULL, true, 0.15f);
    }
    field_0x24c++;
    if (field_0x230 == 6) {
        if (field_0x24c > lbl_804716B8[field_0x25c]) {
            field_0x25c++;
        }
        if (field_0x25c >= 4) {
            field_0x25c = 0;
            field_0x24c = 0;
        }
    } else {
        if (field_0x24c > lbl_804716A8[field_0x25c]) {
            field_0x25c++;
        }
        if (field_0x25c >= 4) {
            field_0x25c = 0;
            field_0x24c = 0;
            if (field_0x270 == 4) {
                field_0x278++;
                field_0x278 &= 3;
            }
        }
    }
    if (field_0x4 < 8 && field_0x94 == 8 && field_0x282 == 0) {
        JGeometry::TVec3f focus(mPos.x, mPos.y - mPos.z, 0.0f);
        gGameRoot2_8053AB10->field_0x24->addFocusPoint_8030C210(&focus);
    }
    if (field_0x250 > 0) {
        field_0x250--;
    }
    if (field_0x254 > 0) {
        field_0x254--;
    }
    if (field_0x258 > 0) {
        field_0x258--;
    }
}

// FIXME(shotgun): 92.91 %, different + reloc-name
void WingedForceGem_804AB098::draw() {
    if (field_0x281 == 1 || field_0x230 == 1 || field_0x230 == 2) {
        return;
    }
    SpriteParams_801F5AE0 params;
    s32 kind;
    if (field_0x270 == 4) {
        kind = (field_0x278 < 3) ? 0 : 3;
    } else {
        kind = field_0x270;
    }
    params.field_0x20 = 0;
    params.field_0x2d = 1;
    if (field_0x284 == 0) {
        params.field_0x1c = 1;
        params.field_0x1d = 1;
    } else {
        params.field_0x1c = 0;
        params.field_0x1d = 3;
    }
    if (field_0x230 == 5 || field_0x230 == 6) {
        params.field_0x2d = 1;
        params.field_0x1f = 0;
        if (field_0x26c >= 0 && field_0x26c <= 3) {
            params.field_0x1d = fn_8023CB24(field_0x26c) + 1;
            params.field_0x1c = fn_8023CA88(field_0x26c);
        }
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind + 8], &params, -1, 1.0f,
                            1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x25c + lbl_804AB038[kind + 8], 0, 2, -1, true, 0, 0);
        return;
    }
    JGeometry::TVec3f saved(mPos);
    mPos.add(field_0x2b0);
    mPos.z += field_0x248;
    f32 scale = 1.0f + (0.3f * field_0x248) / 20.0f;
    Mtx mtx;
    PSMTXIdentity(mtx);
    mtx[0][0] = scale;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = scale;
    mtx[1][2] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = scale;
    params.mMtx = mtx;
    if (field_0x274 == 1) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind], &params, -1, 1.0f, 1.0f);
        params.field_0x1d = -1;
        params.field_0x1c = 0;
        if (mSpriteParams.mMtx == NULL) {
            fn_801EF830(field_0x4, &mPos, &params, 7, "enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind],
                        NULL);
        }
    } else {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x25c + lbl_804AB038[kind], &params, 7, 1.0f, 1.0f);
    }
    fn_803075AC(mSerial, field_0x4, &mPos, field_0x25c + lbl_804AB038[kind], 0, 2, -1, false, 0, 0);
    mPos = saved;
}

WingedForceGem_804AB098::~WingedForceGem_804AB098() {}
