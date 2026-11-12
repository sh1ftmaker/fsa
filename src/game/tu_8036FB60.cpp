#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTVideo.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Camera.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Message.h"
#include "game/Player.h"
#include "game/act/tu_801F9484.h"
#include "game/act/tu_8036FB60.h"
#include "game/cls_804AE9B0.h"

extern const char lbl_80474F6C[];
extern const char lbl_80474F80[];
extern s32 lbl_80474ED0[];
extern s32 lbl_80474EE0[];
extern s32 lbl_80474EF0[];
extern s32 lbl_80474F00[];
extern s32 lbl_80474F10[];
extern s32 lbl_80474F20[];
extern s32 lbl_80474F30[];
extern s32 lbl_80474F40[];
extern s32 lbl_80474F4C[];
extern s32 lbl_80474F5C[];


// FIXME(shotgun): not written: the table copy at 0x80372B00 (a compiler-generated static-init
// function: lbl_804AE420 below is initialised statically instead).
cls_804AE4B0::~cls_804AE4B0() {}

// FIXME(shotgun): 99.98 %, scheduling
void cls_804AE4B0::init() {
    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        fn_8036FDAC();
        return;
    }
    mTimer = 0x50;
    static JGeometry::TVec3f offsets[4] = {
        JGeometry::TVec3f(0.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(16.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(32.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(48.0f, 0.0f, 0.0f),
    };
    if (vfunc_0x34()) {
        mState = 0xB;
    } else {
        mState = 0;
    }
    field_0x234 = 1;
    for (s32 i = 0; i < 4; i++) {
        field_0x2c4[i] = mPos + offsets[i];
        field_0x294[i] = field_0x2c4[i];
        field_0x2f4[i].set(0.0f, 0.0f, 0.0f);
        field_0x350[i] = 0x38B;
        field_0x330[i] = 0;
        field_0x340[i] = 1;
    }
    field_0x3c = field_0x2f4[0];
    field_0x360 = 8;
    field_0x364 = 0;
    field_0x324 = field_0x294[0];
    field_0x368 = 0x106;
    field_0x36c = 0x10;
    field_0x370 = 0;
    field_0x394 = 0;
    field_0x398 = 1;
    field_0x399 = 0;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x3a0 = -1;
    field_0x3a4 = 0;
    field_0x3a8 = 0;
    field_0x3ac = 0;
    field_0x24c = 0.0f;
    field_0x248 = 0.0f;
    field_0x244 = 0.0f;
    field_0x258 = 0.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x25c = 0.0f;
    field_0x260 = 0.0f;
    for (s32 i = 0; i < 4; i++) {
        field_0x374[i] = 0;
        field_0x384[i] = 0;
    }
}

// FIXME(shotgun): the original copies this table at run time from a constant one
// (lbl_804AE390 -> lbl_804AE420, __sinit-style, fn_80372B00); here it is initialised.
static void (cls_804AE4B0::*lbl_804AE420[12])() = {
    &cls_804AE4B0::fn_803717CC, &cls_804AE4B0::fn_80371284, &cls_804AE4B0::fn_803711F0, &cls_804AE4B0::fn_8037100C,
    &cls_804AE4B0::fn_80370C84, &cls_804AE4B0::fn_80370C18, &cls_804AE4B0::fn_80370A7C, &cls_804AE4B0::fn_80370764,
    &cls_804AE4B0::fn_803706AC, &cls_804AE4B0::fn_803705A0, &cls_804AE4B0::fn_80370508, &cls_804AE4B0::fn_803704E8,
};

// FIXME(shotgun): 82.20 %, missing/extra
void cls_804AE4B0::update() {
    if (mTimer != 0) {
        mTimer--;
    }
    field_0x39c = fn_8022EA0C(0);
    for (s32 i = 0; i < 4; i++) {
        field_0x264[i] = *getPlayerPos_8023E724(i);
    }
    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        fn_8036FB60();
        return;
    }
    if (field_0x240 != 0) {
        field_0x240--;
    }
    if (field_0x3ac != 0) {
        field_0x3ac--;
    }
    if (field_0x360 != 0) {
        field_0x360--;
    }
    if (field_0x360 == 0) {
        field_0x360 = 8;
        field_0x364++;
        if (field_0x364 >= 4) {
            field_0x364 = 0;
        }
    }
    if (field_0x36c != 0) {
        field_0x36c--;
    }
    if (field_0x36c == 0) {
        if (field_0x368 == 0x108) {
            field_0x36c = 0x10;
        } else {
            field_0x36c = 10;
        }
        field_0x370 ^= 1;
    }
    JGeometry::TBox2f box = mBox;
    JGeometry::TBox2f work = box;
    for (s32 i = 0; i < 4; i++) {
        if (field_0x330[i] != 0) {
            field_0x330[i]--;
        }
        work.i.x += field_0x294[i].x;
        work.f.x += field_0x294[i].x;
        work.i.y += field_0x294[i].y;
        work.f.y += field_0x294[i].y;
        fn_80239914(&work, field_0x4, mSerial);
        work = box;
    }
    work.i.x += field_0x324.x;
    work.f.x += field_0x324.x;
    work.i.y += field_0x324.y;
    work.f.y += field_0x324.y;
    fn_80239914(&work, field_0x4, mSerial);
    (this->*lbl_804AE420[mState])();
}

// FIXME(shotgun): 76.32 %, missing/extra + reloc-name
void cls_804AE4B0::draw() {
    if (gGameRoot_8053AAF8->field_0x7e != 0) {
        return;
    }
    JGeometry::TVec3f saved = mPos;
    for (s32 i = 0; i < 4; i++) {
        mPos = field_0x294[i];
        drawSprite_801F06F0(lbl_80474F6C, field_0x350[i] + lbl_80474ED0[field_0x364], NULL, 7, 1.0f, 1.0f);
    }
    if (field_0x399 == 0) {
        mPos = saved;
    } else {
        mPos = field_0x324;
        drawSprite_801F06F0(lbl_80474F80, field_0x368 + field_0x370, NULL, 7, 1.0f, 1.0f);
        mPos = saved;
    }
}

// Not auto-inlined: update-side callers (fn_8037100C, fn_80370764, fn_803704E8) use a plain bl.
#pragma auto_inline off
// FIXME(shotgun): 97.89 %, scheduling+regalloc
void cls_804AE4B0::fn_803721F4() {
    bool close = false;
    if (field_0x398 == 1) {
        for (s32 i = 0; i < 4; i++) {
            for (s32 j = 0; j < 4; j++) {
                if (PSVECDistance(&field_0x264[j], &field_0x294[i]) < 48.0f) {
                    close = true;
                    break;
                }
            }
            if (close == true) {
                break;
            }
        }
    }
    if (close == true) {
        mTimer++;
        fn_80371A2C();
        return;
    }
    if (mState != 0 && mState != 3 && mState != 0xB && field_0x398 == 1) {
        return;
    }
    if (mTimer == 0) {
        field_0x234++;
        field_0x234 &= 3;
        fn_80371D50(field_0x234);
    } else {
        for (s32 i = 0; i < 4; i++) {
            field_0x294[i] += field_0x2f4[i];
        }
        if (mTimer == 0x76) {
            for (s32 i = 0; i < 4; i++) {
                field_0x294[i] = field_0x2c4[i];
                field_0x2f4[i].set(0.0f, 0.0f, 0.0f);
                gAudioMgr_8053AF80->fn_80458880(0x37, -1);
            }
        } else if (mTimer < 0x76) {
            field_0x398 = 1;
        }
    }
}
#pragma auto_inline reset

// Starts the next lane pattern `arg` (0-3): the four sprites slide in from their start offsets.
// FIXME(shotgun): 77 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80371D50(s32 arg) {
    mTimer = 0x80;
    field_0x398 = 0;
    switch (arg) {
    case 0: {
        static JGeometry::TVec3f offsets[4] = {
            JGeometry::TVec3f(0.0f, 0.0f, 0.0f),
            JGeometry::TVec3f(16.0f, 0.0f, 0.0f),
            JGeometry::TVec3f(0.0f, 16.0f, 0.0f),
            JGeometry::TVec3f(16.0f, 16.0f, 0.0f),
        };
        for (s32 i = 0; i < 4; i++) {
            field_0x2c4[i] = mPos + offsets[i];
            field_0x2f4[i].set((field_0x2c4[i].x - field_0x294[i].x) / 10.0f,
                               (field_0x2c4[i].y - field_0x294[i].y) / 10.0f, 0.0f);
            field_0x350[i] = 0x391;
            field_0x340[i] = 3;
        }
        break;
    }
    case 1:
        for (s32 i = 0; i < 4; i++) {
            field_0x2c4[i] = mPos + JGeometry::TVec3f(16.0f * (f32)i, 0.0f, 0.0f);
            field_0x2f4[i].set((field_0x2c4[i].x - field_0x294[i].x) / 10.0f,
                               (field_0x2c4[i].y - field_0x294[i].y) / 10.0f, 0.0f);
            field_0x350[i] = 0x388;
            field_0x340[i] = 0;
        }
        break;
    case 2:
        for (s32 i = 0; i < 4; i++) {
            field_0x2c4[i] = mPos + JGeometry::TVec3f(0.0f, 16.0f * (f32)i, 0.0f);
            field_0x2f4[i].set((field_0x2c4[i].x - field_0x294[i].x) / 10.0f,
                               (field_0x2c4[i].y - field_0x294[i].y) / 10.0f, 0.0f);
            field_0x350[i] = 0x38E;
            field_0x340[i] = 2;
        }
        break;
    case 3: {
        static JGeometry::TVec3f offsets[4] = {
            JGeometry::TVec3f(0.0f, 0.0f, 0.0f),
            JGeometry::TVec3f(-16.0f, 16.0f, 0.0f),
            JGeometry::TVec3f(16.0f, 16.0f, 0.0f),
            JGeometry::TVec3f(0.0f, 32.0f, 0.0f),
        };
        for (s32 i = 0; i < 4; i++) {
            field_0x2c4[i] = mPos + offsets[i];
            field_0x2f4[i].set((field_0x2c4[i].x - field_0x294[i].x) / 10.0f,
                               (field_0x2c4[i].y - field_0x294[i].y) / 10.0f, 0.0f);
            field_0x350[i] = lbl_80474EE0[i];
            field_0x340[i] = lbl_80474EF0[i];
        }
        break;
    }
    }
}

// Turns the sprites towards the nearest player, and lets the player pick one up.
// FIXME(shotgun): 73 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80371A2C() {
    f32 nearest[4] = {49.0f, 49.0f, 49.0f, 49.0f};
    for (s32 i = 0; i < 4; i++) {
        if (field_0x330[i] == 0) {
            for (s32 j = 0; j < 4; j++) {
                f32 dist = PSVECDistance(&field_0x264[j], &field_0x294[i]);
                if (dist < nearest[i]) {
                    nearest[i] = dist;
                    u16 angle = (s32)(JMath::atanTable_.atan2_(field_0x264[j].y - field_0x294[i].y,
                                                              field_0x264[j].x - field_0x294[i].x) *
                                      10430.378f);
                    s32 sprite;
                    s32 dir;
                    if (angle >= 0x2000 && angle < 0x6000) {
                        sprite = 0x38E;
                        dir = 2;
                    } else if (angle >= 0x6000 && angle < 0xA000) {
                        sprite = 0x38B;
                        dir = 1;
                    } else if (angle >= 0xA000 && angle < 0xE000) {
                        sprite = 0x391;
                        dir = 3;
                    } else {
                        sprite = 0x388;
                        dir = 0;
                    }
                    if (sprite != field_0x350[i]) {
                        field_0x330[i] = 0x1E;
                        field_0x350[i] = sprite;
                        field_0x340[i] = dir;
                    }
                }
            }
        }
    }
    if (field_0x3a8 == 0) {
        if (field_0x3ac == 0) {
            s32 idx = fn_80370200();
            if (idx != -1) {
                JGeometry::TBox2f box;
                box.i.x = mBox.i.x + field_0x294[idx].x;
                box.i.y = mBox.i.y + field_0x294[idx].y;
                box.f.x = mBox.f.x + field_0x294[idx].x;
                box.f.y = mBox.f.y + field_0x294[idx].y;
                fn_8022FEB8(&box, field_0x4, mSerial, true, false);
                s32 player = fn_8022FD8C(mSerial);
                if (player >= 0) {
                    if (fn_802300EC(player)) {
                        if (fn_8038BD08(player, lbl_80474F00[idx], field_0x4, field_0x294[idx], 0x22)) {
                            field_0x3a4 = player;
                            field_0x3a8 = 1;
                            field_0x3a0 = idx;
                            fn_8023077C();
                        }
                    }
                }
            }
        }
    } else if (field_0x3a0 != 4) {
        if (field_0x3a0 >= 0) {
            JGeometry::TBox2f box;
            box.i.x = mBox.i.x + field_0x294[field_0x3a0].x;
            box.i.y = mBox.i.y + field_0x294[field_0x3a0].y;
            box.f.x = mBox.f.x + field_0x294[field_0x3a0].x;
            box.f.y = mBox.f.y + field_0x294[field_0x3a0].y;
            fn_8022FF8C(field_0x3a4, &box, field_0x4, mSerial, true, false);
            if (fn_80388CAC(field_0x3a4, mSerial)) {
                field_0x3a4 = -1;
                field_0x3a8 = 0;
                fn_80230534(false);
                field_0x3ac = 3;
                field_0x3a0 = -1;
            }
        }
    }
}

// The first state: the lanes finish sliding in; the player standing on the start tile starts it.
// FIXME(shotgun): 70 %, missing/extra + reloc-name
void cls_804AE4B0::fn_803717CC() {
    fn_803721F4();
    JGeometry::TBox2f region;
    region.set(2288.0f, 1712.0f, 2320.0f, 1728.0f);
    bool inside = false;
    JGeometry::TVec3f* p = &field_0x264[field_0x39c];
    if (p->x >= region.i.x && p->y >= region.i.y && p->x < region.f.x && p->y < region.f.y) {
        inside = true;
    }
    if (inside) {
        for (s32 i = 0; i < 4; i++) {
            if (fn_8023ED8C(i)) {
                fn_80230C6C(i);
            }
        }
        fn_8022CA6C(field_0x39c, 5, false);
        for (s32 i = 0; i < 4; i++) {
            if (!fn_80230188(i) && fn_802300EC(i)) {
                fn_80230AD4(i);
            }
        }
        field_0x324.x = 2304.0f;
        field_0x324.y = 1760.0f;
        field_0x324.z = 0.0f;
        field_0x399 = 1;
        mState = 1;
        Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
        field_0x244 = -cam->field_0x4[0][3];
        field_0x248 = -cam->field_0x4[1][3];
        field_0x24c = 0.0f;
        field_0x250 = (2304.0f - field_0x244) / 60.0f;
        field_0x254 = (1696.0f - field_0x248) / 60.0f;
        JGeometry::TBox2f view = gGameRoot2_8053AB10->field_0x24->field_0x164;
        field_0x25c = (view.f.y - view.i.y) * 0.5f;
        field_0x260 = 144.0f - field_0x25c;
        cam = gGameRoot2_8053AB10->field_0x24;
        cam->field_0x179 = 1;
        cam = gGameRoot2_8053AB10->field_0x24;
        if (cam->field_0x840 == 0) {
            cam->field_0x840 = 1;
            cam->field_0x844.i.x = field_0x244;
            cam->field_0x844.i.y = field_0x248 - field_0x25c;
            cam->field_0x844.f.x = field_0x244;
            cam->field_0x844.f.y = field_0x248 + field_0x25c;
            cam->field_0x854 = 1.0f;
            cam->field_0x1d4 = -1.0f;
            cam->field_0x1d8 = -1.0f;
        }
    }
}

// Per player: counts the sprite animation down and draws the player's marker above the start
// tile (the inline both loops of fn_80371284 share).
static inline void tickPlayer_80371284(cls_804AE4B0* self, s32 i) {
    if (self->field_0x374[i] != 0) {
        self->field_0x374[i]--;
    }
    if (self->field_0x374[i] == 0) {
        self->field_0x374[i] = 6;
        self->field_0x384[i]++;
        if (fn_8023EFEC(i)) {
            if (self->field_0x384[i] >= 7) {
                self->field_0x384[i] = 0;
            }
        } else if (fn_8023EA28(i)) {
            if (self->field_0x384[i] >= 4) {
                self->field_0x384[i] = 0;
            }
        } else if (self->field_0x384[i] >= 8) {
            self->field_0x384[i] = 0;
        }
    }
    if (!fn_80230188(i) && self->mState == 1) {
        if (fn_8023EFEC(i)) {
            JGeometry::TVec3f pos = self->field_0x264[self->field_0x39c] - JGeometry::TVec3f(0.0f, 0.3f, 0.0f);
            fn_80230498(i, &pos, 0x2CF + self->field_0x384[i], 0xFF, -0x80, -0x80, true);
        } else {
            JGeometry::TVec3f pos = self->field_0x264[self->field_0x39c] - JGeometry::TVec3f(0.0f, 0.3f, 0.0f);
            fn_80230498(i, &pos, 0x18 + self->field_0x384[i], 0xFF, -0x80, -0x80, true);
        }
    }
}

// The second state: the camera pans to the lane tile; when it has arrived the message is shown.
// FIXME(shotgun): 88 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80371284() {
    for (s32 i = 0; i < 4; i++) {
        if (!fn_80230188(i) && fn_802300EC(i)) {
            fn_80230AD4(i);
        }
    }
    fn_803703C8();
    if (field_0x324.y > 1720.0f) {
        field_0x324.y -= 0.5f;
        for (s32 i = 0; i < 4; i++) {
            tickPlayer_80371284(this, i);
        }
    }
    if (field_0x324.y <= 1720.0f) {
        field_0x324.y = 1720.0f;
        field_0x384[0] = 0;
        field_0x384[1] = 0;
        field_0x384[2] = 0;
        field_0x384[3] = 0;
        JGeometry::TVec3f pos(0.5f * (f32)JUTVideo::sManager->getFbWidth(),
                              0.5f * (f32)JUTVideo::sManager->getEfbHeight(), 0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x50F, field_0x4, pos, 0x22)) {
            mState = 2;
            for (s32 i = 0; i < 4; i++) {
                tickPlayer_80371284(this, i);
            }
            field_0x3a4 = 0;
            field_0x3a8 = 1;
            field_0x3a0 = 4;
            field_0x368 = 0x108;
        }
    }
}

void cls_804AE4B0::fn_803711F0() {
    fn_803703C8();
    fn_8022FE80(0, field_0x4, mSerial, true, false);
    if (fn_80388EEC(0)) {
        if (fn_80230534(false)) {
            mState = 3;
            field_0x3a4 = -1;
            field_0x3ac = 3;
            field_0x3a8 = 0;
            field_0x3a0 = -1;
            fn_8022CA6C(0, 5, false);
        }
    }
}

// FIXME(shotgun): 79.69 %, missing/extra
void cls_804AE4B0::fn_8037100C() {
    fn_803703C8();
    fn_803721F4();
    fn_80370020();
    JGeometry::TBox2f region;
    region.set(2208.0f, 1624.0f, 2288.0f, 1690.0f);
    bool inside = false;
    JGeometry::TVec3f* p = &field_0x264[field_0x39c];
    if (p->x >= region.i.x && p->y >= region.i.y && p->x < region.f.x && p->y < region.f.y) {
        inside = true;
    }
    if (inside) {
        for (s32 i = 0; i < 4; i++) {
            if (!fn_802300EC(i)) {
                return;
            }
        }
        if (field_0x398 == 1) {
            for (s32 i = 0; i < 4; i++) {
                if (fn_8023ED8C(i)) {
                    fn_80230C6C(i);
                }
            }
            fn_8022CA6C(0, 0, false);
            if (fn_80230AD4(field_0x39c)) {
                for (s32 i = 0; i < 4; i++) {
                    if (!fn_80230188(i) && fn_802300EC(i)) {
                        fn_80230AD4(i);
                    }
                }
                mState = 4;
                lbl_8053AD70->fn_8037AE00();
                fn_80119DD0()->fn_80119998(0x3C, 0, 2, 0, 0, 10, 0);
                field_0x240 = 0x46;
                gAudioMgr_8053AF80->fn_80458FF0(0x1000007, false, 0);
            }
        }
    }
}

// FIXME(shotgun): 50.75 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80370C84() {
    for (s32 i = 0; i < 4; i++) {
        if (!fn_80230188(i) && fn_802300EC(i)) {
            fn_80230AD4(i);
        }
    }
    fn_803703C8();
    if (field_0x240 == 0x23) {
        field_0x234 = 1;
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos = mPos;
            pos.z += 0.0f;
            JGeometry::TVec3f off((f32)i * 16.0f, 0.0f, 0.0f);
            field_0x294[i] = JGeometry::TVec3f(mPos.x, mPos.y, 0.0f) + off;
            field_0x294[i].z = pos.z;
            field_0x2f4[i].set(0.0f, 0.0f, 0.0f);
            field_0x350[i] = 0x38E;
            field_0x330[i] = 0;
            field_0x340[i] = 2;
        }
        field_0x3c = field_0x2f4[0];
        field_0x398 = 1;
        mTimer = 0x80;
    }
    if (field_0x240 <= 0x23) {
        for (s32 i = 0; i < 4; i++) {
            JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
            JGeometry::TVec3f base(2232.0f, 1624.0f, 0.0f);
            pos = base + JGeometry::TVec3f(16.0f * (f32)i, 0.0f, 0.0f);
            if (fn_8023EFEC(i)) {
                fn_80230498(i, &pos, 0x2C8, 0xFF, -0x80, -0x80, true);
            } else {
                fn_80230498(i, &pos, 0x10, 0xFF, -0x80, -0x80, true);
            }
        }
    }
    if (field_0x240 == 0) {
        JGeometry::TVec3f pos(0.5f * (f32)JUTVideo::sManager->getFbWidth(),
                              0.5f * (f32)JUTVideo::sManager->getEfbHeight(), 0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_8038BD08(0, 0x502, field_0x4, pos, 0x22)) {
            mState = 5;
            lbl_8053AD70->fn_8037AE30();
        }
    }
}

void cls_804AE4B0::fn_80370C18() {
    fn_803703C8();
    fn_8022FE80(0, field_0x4, mSerial, true, false);
    if (fn_80388EEC(0)) {
        field_0x240 = 0x1E;
        mState = 6;
        field_0x23c = 0;
    }
}

// FIXME(shotgun): 89.19 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80370A7C() {
    fn_803703C8();
    if (field_0x240 != 0) {
        if (field_0x240 == 1) {
            JGeometry::TVec3f pos(0.5f * (f32)JUTVideo::sManager->getFbWidth(),
                                  0.5f * (f32)JUTVideo::sManager->getEfbHeight(), 0.0f);
            fn_8022FE80(0, field_0x4, mSerial, true, false);
            fn_8038BD08(0, lbl_80474F10[field_0x394], field_0x4, pos, 0x22);
        }
    } else {
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        if (fn_80388EEC(0)) {
            fn_80230534(false);
            fn_8022CA6C(0, 5, false);
            mState = 7;
            fn_80371D50(lbl_80474F20[field_0x394]);
            if ((gGameRoot_8053AAF8->field_0x83 & 3) == 2) {
                lbl_8053AED0->fn_8043D67C(0x50B, 0, 0);
            } else {
                lbl_8053AED0->fn_8043D67C(0x50C, 0, 0);
            }
        }
    }
}

// FIXME(shotgun): 85.72 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80370764() {
    fn_803703C8();
    for (s32 i = 0; i < 4; i++) {
        if (fn_8023ED8C(i)) {
            fn_80230C6C(i);
        }
    }
    bool found = false;
    for (s32 i = 0; i < 4 && !found; i++) {
        if (fn_8023E854(i)) {
            JGeometry::TVec3f d = JGeometry::TVec3f(2232.0f, 1624.0f, 0.0f) - *getPlayerPos_8023E724(0);
            if (PSVECMag((Vec*)&d) > 2.0f) {
                fn_8023DFF4(0, &d);
            }
            found = true;
        }
    }
    if (!found && fn_8022CAF8(0) == 4) {
        fn_8022CA6C(0, 5, false);
    }
    field_0x23c++;
    fn_803721F4();
    {
        JGeometry::TVec3f d = JGeometry::TVec3f(2232.0f, 1624.0f, 0.0f) - *getPlayerPos_8023E724(0);
        if (PSVECMag((Vec*)&d) > 2.0f) {
            fn_8023DFF4(0, &d);
        }
    }
    if (!fn_802341D0(0)) {
        field_0x240 = 2;
    } else if (field_0x398 != 0 && field_0x240 == 0 && fn_8022CAF8(0) == lbl_80474F30[field_0x394] &&
               fn_802300EC(0)) {
        JGeometry::TVec3f pos(0.5f * (f32)JUTVideo::sManager->getFbWidth(),
                              0.5f * (f32)JUTVideo::sManager->getEfbHeight(), 0.0f);
        fn_8022FE80(0, field_0x4, mSerial, true, false);
        s32 msg;
        if (field_0x394 < 3) {
            msg = lbl_80474F40[field_0x394];
            field_0x394++;
        } else if (field_0x23c < 0x168) {
            field_0x394 = 4;
            msg = 0x509;
        } else {
            field_0x394 = 5;
            msg = 0x50A;
        }
        if (fn_8038BD08(0, msg, field_0x4, pos, 0x22)) {
            fn_8023077C();
            mState = 8;
        }
    }
}

void cls_804AE4B0::fn_803706AC() {
    fn_803703C8();
    fn_8023077C();
    fn_8022FE80(0, field_0x4, mSerial, true, false);
    if (fn_80388EEC(0)) {
        if (field_0x394 >= 4) {
            if (field_0x394 == 4) {
                mState = 9;
                field_0x368 = 0x106;
            } else {
                mState = 10;
                field_0x368 = 0x104;
            }
            gAudioMgr_8053AF80->fn_80458FF0(0x100000B, false, 0);
        } else {
            mState = 6;
            field_0x240 = 0x1E;
        }
    }
}

// FIXME(shotgun): 76.97 %, missing/extra + reloc-name
void cls_804AE4B0::fn_803705A0() {
    fn_803703C8();
    if (field_0x324.y > 1680.0f) {
        field_0x324.y -= 0.5f;
        if (field_0x324.y <= 1680.0f) {
            field_0x324.y = 1680.0f;
            u8 count = (field_0x23c <= 0xB4) + 2;
            JGeometry::TVec3f pos = field_0x324 - JGeometry::TVec3f(0.0f, 32.0f, 0.0f);
            getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &pos, count | 0x30000, -1, -1);
            field_0x240 = 0x1E;
            mState = 10;
            field_0x368 = 0x108;
        }
    }
}

// FIXME(shotgun): reloc-name only: float literals lbl_80542E00/E04 not split into this unit
void cls_804AE4B0::fn_80370508() {
    fn_803703C8();
    if (field_0x240 != 0) {
        if (field_0x240 == 1) {
            field_0x368 = 0x104;
        }
    } else {
        if (field_0x324.y < 1760.0f) {
            field_0x324.y += 0.5f;
        } else {
            fn_80230534(false);
            mState = 0xB;
            vfunc_0x30(0xFF, true);
        }
    }
}

void cls_804AE4B0::fn_803704E8() {
    fn_803721F4();
}

// Not auto-inlined: the original state handlers call it with a plain bl (docs/INLINING.md, pragma rule 1).
#pragma auto_inline off
// FIXME(shotgun): 99.44 %, regalloc-only + reloc-name (literal pool names)
void cls_804AE4B0::fn_803703C8() {
    Camera_804AA9E4* cam = gGameRoot2_8053AB10->field_0x24;
    f32 a = -cam->field_0x4[0][3];
    f32 b = -cam->field_0x4[1][3];
    if (fabs(a - 2304.0f) < fabs(field_0x250)) {
        field_0x250 = 0.0f;
        field_0x244 = 2304.0f;
    } else {
        field_0x244 += field_0x250;
    }
    if (fabs(b - 1696.0f) < fabs(field_0x254)) {
        field_0x254 = 0.0f;
        field_0x248 = 1696.0f;
    } else {
        field_0x248 += field_0x254;
    }
    if (fabs(field_0x25c - 144.0f) < fabs(field_0x260)) {
        field_0x260 = 0.0f;
        field_0x260 = 144.0f;
    } else {
        field_0x25c += field_0x260;
    }
    f32 yHi = field_0x248 + field_0x25c;
    f32 yLo = field_0x248 - field_0x25c;
    f32 x = field_0x244;
    gGameRoot2_8053AB10->field_0x24->field_0x179 = 1;
    if (gGameRoot2_8053AB10->field_0x24->field_0x7f5 == 0) {
        cam = gGameRoot2_8053AB10->field_0x24;
        cam->field_0x7f4 = 1;
        cam->field_0x7f8 = 5;
        cam->field_0x808.i.x = x;
        cam->field_0x808.i.y = yLo;
        cam->field_0x808.f.x = x;
        cam->field_0x808.f.y = yHi;
        cam->field_0x7f6 = 0;
    }
}
#pragma auto_inline reset

// FIXME(shotgun): 80.21 %, missing/extra + reloc-name
s32 cls_804AE4B0::fn_80370200() {
    s32 idx = field_0x39c;
    u8 hit[4] = {0, 0, 0, 0};
    bool any = false;
    for (s32 i = 0; i < 4; i++) {
        u16 angle = (s32)(JMath::atanTable_.atan2_(field_0x264[idx].y - field_0x294[i].y,
                                                  field_0x264[idx].x - field_0x294[i].x) * 10430.378f);
        s32 want;
        if (angle >= 0x2000 && angle < 0x6000) {
            want = 2;
        } else if (angle >= 0x6000 && angle < 0xA000) {
            want = 1;
        } else if (angle >= 0xA000 && angle < 0xE000) {
            want = 3;
        } else {
            want = 0;
        }
        if (field_0x340[i] == want) {
            if (lbl_80474F4C[field_0x340[i]] == fn_80230CFC(idx)) {
                any = true;
                hit[i] = 1;
            }
        }
    }
    if (!any) {
        return -1;
    }
    f32 best = 100000.0f;
    s32 bestIdx = -1;
    for (s32 i = 0; i < 4; i++) {
        if (hit[i] == 1) {
            f32 d = PSVECDistance(&field_0x264[idx], &field_0x294[i]);
            if (d < best) {
                best = d;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

// Not auto-inlined: fn_8037100C calls it with a plain bl.
#pragma auto_inline off
// FIXME(shotgun): 75.18 %, missing/extra + reloc-name
void cls_804AE4B0::fn_80370020() {
    if (field_0x3a8 == 0) {
        if (field_0x3ac == 0) {
            JGeometry::TBox2f box;
            box.i.x = mBox.i.x + field_0x324.x;
            box.i.y = mBox.i.y + field_0x324.y;
            box.f.x = mBox.f.x + field_0x324.x;
            box.f.y = mBox.f.y + field_0x324.y;
            fn_8022FEB8(&box, field_0x4, mSerial, true, false);
            s32 player = fn_8022FD8C(mSerial);
            if (player >= 0) {
                if (fn_802300EC(player)) {
                    if (PSVECDistance(&field_0x264[player], &field_0x324) <= 20.0f) {
                        if (fn_8038BD08(player, 0x510, field_0x4, field_0x324, 0x22)) {
                            field_0x3a4 = player;
                            field_0x3a8 = 1;
                            field_0x3a0 = 4;
                            fn_8023077C();
                        }
                    }
                }
            }
        }
    } else if (field_0x3a0 == 4) {
        JGeometry::TBox2f box;
        box.i.x = mBox.i.x + field_0x324.x;
        box.i.y = mBox.i.y + field_0x324.y;
        box.f.x = mBox.f.x + field_0x324.x;
        box.f.y = mBox.f.y + field_0x324.y;
        fn_8022FF8C(field_0x3a4, &box, field_0x4, mSerial, true, false);
        if (fn_80388CAC(field_0x3a4, mSerial)) {
            field_0x3a4 = -1;
            field_0x3a8 = 0;
            fn_80230534(false);
            field_0x3ac = 3;
            field_0x3a0 = -1;
        }
    }
}
#pragma auto_inline reset

// FIXME(shotgun): 98.59 %, scheduling+regalloc + reloc-name
void cls_804AE4B0::fn_8036FDAC() {
    mTimer = 0x50;
    static JGeometry::TVec3f offsets[4] = {
        JGeometry::TVec3f(0.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(16.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(32.0f, 0.0f, 0.0f),
        JGeometry::TVec3f(48.0f, 0.0f, 0.0f),
    };
    mState = 0xB;
    field_0x234 = 1;
    for (s32 i = 0; i < 4; i++) {
        field_0x2c4[i] = mPos + offsets[i];
        field_0x294[i] = field_0x2c4[i];
        field_0x2f4[i].set(0.0f, 0.0f, 0.0f);
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('JIJI', field_0x4, &field_0x294[i], lbl_80474F5C[i], -1, -1);
        field_0x3b0[i] = (cls_804AEED0*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        field_0x340[i] = 1;
    }
    field_0x3c = field_0x2f4[0];
    field_0x360 = 8;
    field_0x364 = 0;
    field_0x324 = field_0x294[0];
    field_0x368 = 0x386;
    field_0x36c = 0x10;
    field_0x370 = 0;
    field_0x394 = 0;
    field_0x398 = 1;
    field_0x399 = 0;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x3a0 = -1;
    field_0x3a4 = 0;
    field_0x3a8 = 0;
    field_0x3ac = 0;
    field_0x24c = 0.0f;
    field_0x248 = 0.0f;
    field_0x244 = 0.0f;
    field_0x258 = 0.0f;
    field_0x254 = 0.0f;
    field_0x250 = 0.0f;
    field_0x25c = 0.0f;
    field_0x260 = 0.0f;
    for (s32 i = 0; i < 4; i++) {
        field_0x374[i] = 0;
        field_0x384[i] = 0;
    }
}

// FIXME(shotgun): 99.31 %, scheduling+regalloc
void cls_804AE4B0::fn_8036FB60() {
    bool close = false;
    if (field_0x398 == 1) {
        for (s32 i = 0; i < 4; i++) {
            for (s32 j = 0; j < 4; j++) {
                if (PSVECDistance(&field_0x264[j], &field_0x294[i]) < 48.0f) {
                    close = true;
                    break;
                }
            }
            if (close == true) {
                break;
            }
        }
    }
    if (close == true) {
        mTimer++;
        return;
    }
    for (s32 i = 0; i < 4; i++) {
        field_0x3b0[i]->fn_8037B374(field_0x294[i], field_0x340[i] * 3);
    }
    if (mState != 0 && mState != 3 && mState != 0xB && field_0x398 == 1) {
        return;
    }
    if (mTimer == 0) {
        field_0x234++;
        field_0x234 &= 3;
        fn_80371D50(field_0x234);
    } else {
        for (s32 i = 0; i < 4; i++) {
            field_0x294[i] += field_0x2f4[i];
        }
        if (mTimer == 0x76) {
            for (s32 i = 0; i < 4; i++) {
                field_0x294[i] = field_0x2c4[i];
                field_0x2f4[i].set(0.0f, 0.0f, 0.0f);
                gAudioMgr_8053AF80->fn_80458880(0x37, -1);
            }
        } else if (mTimer < 0x76) {
            field_0x398 = 1;
        }
    }
}

