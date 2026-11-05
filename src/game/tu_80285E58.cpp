#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/PlayerBit.h"
#include "game/GameMode.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/Effect.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/ViewLayer.h"
#include "game/cls_804A71A8.h"
#include "game/BgRewrite.h"
#include "game/Camera.h"
#include "game/tu_80131D98.h"
#include "game/tu_8030AE84.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JUtility/JUTGamePad.h"

// The length of v in the x/y plane (z is the height).
static inline f32 planarLength(const JGeometry::TVec3f& v) {
    return JGeometry::TUtil<f32>::sqrt(v.x * v.x + v.y * v.y);
}

// The player's per-frame update (docs/PLAYER.md, "Per-frame flow"): run by
// fn_80253E6C (update A) for each player.
void Player_804A6F38::fn_80285E58() {
    if (field_0x11f4 > 0) {
        field_0x11f4--;
        for (int i = 0; i < 0x50; i++) {
            field_0xdb0[i] = -1;
            field_0x1030[i] = -1;
        }
    } else {
        fn_802870C8();
    }
    fn_80265E88();
    fn_8024CA20();
}

// Landing effects and sounds: when the player touches down (or stands on a
// layer whose landing goes through the floor check), plays the sound for the
// tile under it and spawns its splash / dust emitters; then the step sound
// while it walks on the ground.
void Player_804A6F38::fn_80285F44() {
    if (!fn_802413F0() || !fn_80241E28(field_0x3b8, &field_0x14) ||
        (fn_80240808() && fn_80234E30(fn_80240644())) || fn_80270A8C()) {
        field_0x448 = 4;
    } else if ((mPos.z <= 0.0f && field_0x14.z > 0.0f &&
                (field_0x1240 != 0x1F || !viewLayerFlag(field_0x3b4))) ||
               (viewLayerFlag(field_0x3b4) && field_0xbf0->field_0xb5 == 1 && !field_0xbf0->field_0xb4)) {
        if (field_0x448 == 0) {
            field_0x448 = 4;
            u16 tile = fn_8022461C(&mPos, field_0x3b4);
            if (fn_8024B24C()) {
                if (fn_802DEB58()) {
                    if (fn_802412CC()) {
                        fn_8024F0E8(0x20065);
                    } else {
                        fn_8024F0E8(0x20055);
                    }
                } else {
                    fn_8024F0E8(0x2001F);
                }
            } else if (tile == 0xE || tile == 3) {
            } else if (tile == 0x39) {
                fn_8024F0E8(0x20055);
            } else if (tile == 0xD) {
                fn_8024F0E8(0x2003A);
            } else if (fn_80224B88(&mPos, field_0x3b4)) {
                fn_8024F0E8(0x20056);
            } else if (fn_8022517C(&mPos, field_0x3b4)) {
                fn_8024F0E8(0x20054);
            } else if (fn_80224E88(&mPos, field_0x3b4)) {
                fn_8024F0E8(0x2005F);
            } else {
                fn_8024F0E8(0x2001F);
            }
            if (field_0x1240 != 4 || field_0x45c != 4) {
                JGeometry::TVec3f pos = mPos;
                pos.y += 6.0f;
                if (!fn_8024B24C() && (tile == 0xE || (fn_802DEB58() && fn_80225BE4(&mPos, field_0x3b4))) &&
                    fn_802412CC()) {
                    fn_8013CC50(field_0x3b4, &pos, 0x4F7, -1, 0, NULL, NULL, true, 1.0f);
                    fn_8013CC50(field_0x3b4, &pos, 0x4F8, 0, 0, NULL, NULL, true, 1.0f);
                    fn_8013CC50(field_0x3b4, &pos, 0x4F9, 0, 0, NULL, NULL, true, 1.0f);
                    fn_8013CC50(field_0x3b4, &pos, 0x4FA, 0, 0, NULL, NULL, true, 1.0f);
                }
                if (fn_8024B24C()) {
                    fn_8013CC50(field_0x3b4, &pos, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
                } else if (tile == 0xE || tile == 3) {
                } else if (fn_80225BE4(&mPos, field_0x3b4)) {
                    if (fn_802412CC()) {
                        fn_8013CC50(field_0x3b4, &pos, 0x4F3, -1, 0, NULL, NULL, true, 1.0f);
                        fn_8013CC50(field_0x3b4, &pos, 0x4F4, 0, 0, NULL, NULL, true, 1.0f);
                        fn_8013CC50(field_0x3b4, &pos, 0x4F5, 0, 0, NULL, NULL, true, 1.0f);
                        fn_8013CC50(field_0x3b4, &pos, 0x4F6, 0, 0, NULL, NULL, true, 1.0f);
                        s32 player = field_0x4;
                        fn_801F856C()->fn_801F9484(ACTOR_PEFF, field_0x3b4, &mPos, (player & 3) | 0x2C, -1,
                                                   player);
                    } else {
                        fn_8013CC50(field_0x3b4, &pos, 0x3D0, 0, 0, NULL, NULL, true, 1.0f);
                    }
                } else if (tile == 0x39) {
                    fn_8013CC50(field_0x3b4, &pos, 0x3D0, 0, 0, NULL, NULL, true, 1.0f);
                } else {
                    fn_8013CC50(field_0x3b4, &pos, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
                }
            }
        } else {
            field_0x448 = 4;
        }
    } else if (field_0x448 > 0) {
        field_0x448--;
    }

    if ((!viewLayerFlag(field_0x3b4) && mPos.z <= 0.0f) ||
        (viewLayerFlag(field_0x3b4) && !field_0xbf0->field_0xb4)) {
        if (!fn_802413E8() && !fn_80240B84() && !fn_80240D50() && !fn_802412A4() && !fn_80240670() &&
            !fn_80240F28() && planarLength(mPos - field_0x14) >= 0.3f) {
            u16 tile = fn_8022461C(&mPos, field_0x3b4);
            if (tile == 0xE || tile == 3) {
            } else if (tile == 0xD) {
            } else if (tile == 0x39) {
                fn_8024F0E8(0x2001E);
            } else if (fn_8022517C(&mPos, field_0x3b4)) {
                fn_8024F0E8(0x2001C);
            }
        }
    }
}

// FIXME: 88.7. The mode-0xB test is a byte in the original (subfic / cntlzw /
// extrwi., bne; the same unsolved shape as tu_80158624's fn_80159330), and
// the player-bit test branches `bne; b` around the body where ours has `beq`.
// Toggles field_0x13b8 on a Z press (pad trigger 0x10) while the player
// takes part and is on layer 8 or above; below layer 8 it is cleared.
void Player_804A6F38::fn_80286688() {
    GameRoot_8049EF20* root = lbl_8053AAF8;
    if (root->field_0x4c == GAME_MODE_B) {
        return;
    }
    if (root->field_0x7e) {
        return;
    }
    if (isPlayerBitSet(root, field_0x4)) {
        if (fn_8023DE58(fn_8024064C()) < 8) {
            field_0x13b8 = 0;
            return;
        }
        JUTGamePad* pad = JUTGamePad::getGamePad(fn_8030AAF4(field_0x11ec));
        if (!fn_80247534() && pad != NULL && (pad->getTrigger() & 0x10)) {
            if (field_0x13b8 == 1) {
                field_0x13b8 = 0;
            } else {
                field_0x13b8 = 1;
            }
        }
    }
}

// FIXME: 99.67. f4/f5 swapped for the two planar distances.
void Player_804A6F38::fn_8028676C() {
    JGeometry::TVec3f target;
    fn_8027FA08(&target);
    if (!fn_80240808()) {
        if (field_0x1216) {
            field_0x1228++;
            if (fn_8025197C(true) || field_0x1228 > 180) {
                mPos.set(field_0x38[0]);
                field_0x2c.set(mPos);
                field_0x1228 = 0;
            }
        } else {
            field_0x1228 = 0;
        }
        return;
    }

    if (!fn_802413E8() && !fn_80270A8C() && !fn_80250D30() && !fn_8024FEFC() && mPos.z <= 0.0f &&
        fn_8022BC38(fn_80240644(), field_0x3b4, &mPos)) {
        if (!fn_80234ECC(fn_80240644()) &&
            planarLength(*fn_8023E724(fn_80240644()) - target) <= planarLength(mPos - target)) {
            field_0x1228++;
            if (field_0x1238 > 0) {
                field_0x1238--;
            }
            if ((!(fn_80240DAC() || fn_80240D8C() || field_0x43e) && fn_8025197C(true)) ||
                (fn_802486A0() == 5 && fn_80240B84()) || field_0x1228 > 180) {
                mPos.set(*fn_8023E724(fn_80240644()));
                field_0x1228 = 0;
                field_0x1238 = 0;
            }
        } else {
            field_0x1228 = 0;
        }
    } else {
        field_0x1228 = 0;
    }

    if (fn_80234E30(fn_80240644())) {
        field_0x1228 = 0;
    }
    if (fn_802486A0() == 4) {
        field_0x1238 = 0;
        return;
    }
    if (planarLength(target - mPos) < 8.0f) {
        field_0x1238 = 0;
    }
}

// FIXME: 99.17. Scheduling and registers only (the target-vector stores and
// the f-registers of the normalization).
// Moves field_0x338 towards the player's position (in mode 0x7F, offset by
// 16 in the direction field_0x3dc) by 4 units while it is 4-65 units away,
// else puts it there; only while field_0xb90 > 0.
void Player_804A6F38::fn_80286B74() {
    if (field_0xb90 <= 0.0f) {
        return;
    }
    JGeometry::TVec3f target;
    if (lbl_8053AAF8->field_0x7f && fn_80241794() <= 0 && !fn_80241200()) {
        if (field_0x3dc == 0) {
            target.set(mPos.x + 16.0f, mPos.y, mPos.z);
        } else if (field_0x3dc == 1) {
            target.set(mPos.x - 16.0f, mPos.y, mPos.z);
        } else if (field_0x3dc == 2) {
            target.set(mPos.x, mPos.y + 16.0f, mPos.z);
        } else {
            target.set(mPos.x, mPos.y - 16.0f, mPos.z);
        }
    } else {
        target.set(mPos.x, mPos.y, mPos.z);
    }
    JGeometry::TVec3f step = target - field_0x338;
    if (PSVECMag(&step) < 65.0f && PSVECMag(&step) > 4.0f) {
        f32 sq = step.squared();
        if (!(sq <= 32.0f * FLT_EPSILON)) {
            step.scale(JGeometry::TUtil<f32>::inv_sqrt(sq));
        }
        step.scale(4.0f);
        field_0x338 += step;
    } else {
        field_0x338.set(target.x, target.y, target.z);
    }
}

// FIXME: 99.20. Register allocation: the original keeps the player loop
// counter, `player` and `start` in r27 and `other` in r29.
void Player_804A6F38::fn_80286E10() {
    if (field_0x1220 > 0) {
        return;
    }
    if (field_0x1216 == true) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        if (fn_8022E438(field_0x4, i)) {
            if (fn_80236B50(i)) {
                return;
            }
            if (fn_8022D75C(i)) {
                return;
            }
        }
    }
    if (fn_80240808()) {
        return;
    }
    s32 player = field_0x4;
    if (fn_802486A0() == 4) {
        player = fn_8022EA0C(field_0x4);
    }
    if (fn_80230188(player)) {
        return;
    }
    if (!(fn_8023E724(player)->z <= 0.0f)) {
        return;
    }
    if (fn_8023EA28(player)) {
        return;
    }
    if (fn_80234ECC(player)) {
        return;
    }
    if (fn_80235274(player)) {
        return;
    }
    if (fn_80234CF8(player)) {
        return;
    }
    if (fn_80234C5C(player)) {
        return;
    }
    if (fn_80235744(player)) {
        return;
    }
    if (fn_802353AC(player)) {
        return;
    }
    if (fn_80233DA4(player)) {
        return;
    }
    if (fn_80233EDC(player)) {
        return;
    }
    if (fn_80234E30(player)) {
        return;
    }
    if (fn_80248970() < 2) {
        return;
    }
    if (field_0x121e) {
        return;
    }
    if (!lbl_8053AD30->fn_80309D6C(fn_8030AAF4(field_0x11ec), 0x400)) {
        return;
    }
    if (fn_802486A0() == 4) {
        s32 start = field_0x1224;
        s32 partner = fn_8022EA0C(field_0x4);
        s32 other;
        do {
            field_0x1224++;
            if (field_0x1224 >= fn_80248970()) {
                field_0x1224 = 0;
            }
            other = fn_8022EA0C(field_0x4);
            if (!fn_80234134(other) && !fn_8023EA28(other) &&
                fn_8022BC38(other, fn_8023DE58(partner), fn_8023E724(partner))) {
                break;
            }
        } while (start != field_0x1224);
        if (start != field_0x1224) {
            fn_80230FD0(other, 0x10010);
            if (fn_8023ED8C(partner)) {
                fn_8023105C(fn_8023ECF0(partner));
                fn_802310F8(partner);
            }
        }
    } else {
        fn_80248004(4, true);
    }
}

// PSVECMag on a const vector; the original's operator- temporary is passed by
// reference (as in tu_8027E5BC).
static inline f32 magnitude(const JGeometry::TVec3f& v) {
    return PSVECMag(&v);
}

// Snaps a position component to the middle of its 16-unit tile.
static inline f32 tileCenter(f32 v) {
    return (f32)((s32)v & 0xFFFFFFF0) + 8.0f;
}

// Stops an emitter (it runs out) and forgets it.
static inline void invalidateEmitter(JPABaseEmitter*& emitter) {
    if (emitter != NULL) {
        emitter->becomeInvalidEmitter();
        emitter = NULL;
    }
}

// FIXME: 98.34. Every call, field and branch is in place; what differs:
// - `state == 8 || 9 || 10 || 0x39` (the partner check): the original tests
//   8 alone and folds 9-10; ours folds 8-10.
// - lbl_8053ABA8 / lbl_8053ABF0 are tested with cmpwi in the original
//   (cmplwi in ours).
// - field_0x4 and field_0x3b4 are read before the calls they are compared
//   with / passed next to (fn_80239040, fn_80238F78, the five fn_8013CC50
//   trail emitters and fn_802255F4): a saved register in the original, a
//   reload after the call in ours.
// - The inlined fn_80286688's mode-0xB byte test (see its FIXME).
// The state tick (docs/PLAYER.md, "State system"): partner / layer upkeep,
// then while no layer change is pending the three state switches (exit of the
// current state and the commit when a request is pending, enter of the new
// one, update), then the per-frame helpers and the trail emitters.
void Player_804A6F38::fn_802870C8() {
    if (fn_802413E8()) {
        mPos = field_0x32c;
        if (field_0x455) {
            field_0x3b4 = 0;
            field_0x455 = 0;
        }
        field_0x11e9 = 0;
        if (field_0x1201 == 1) {
            field_0x1201 = 0;
        } else {
            fn_8027F16C();
        }
        field_0x1202 = false;
        field_0xca8 = 0;
    } else {
        if (fn_802486A0() == 4) {
            if (field_0x123c == 1) {
                s32 partner = fn_80248778();
                if (fn_80240888() != partner) {
                    field_0x123c = 0;
                    u16 tile = fn_8022461C(&mPos, field_0x3b4);
                    if ((tile >= 4 && tile <= 7) || (tile >= 8 && tile <= 0xB) || (tile >= 0x18 && tile <= 0x1B) ||
                        (tile >= 8 && tile <= 0xB) || (tile >= 0x28 && tile <= 0x2B) ||
                        (tile >= 0x2C && tile <= 0x2F)) {
                    } else {
                        mPos.x = tileCenter(mPos.x);
                        mPos.y = tileCenter(mPos.y);
                    }
                }
            } else {
                s32 partner = fn_80248778();
                if (fn_80240888() == partner) {
                    field_0x123c = 1;
                }
            }
        } else {
            field_0x123c = 0;
        }

        if (fn_802486A0() != 4 && fn_80240808() && (!fn_8025F418() || fn_8022F478(field_0xd70))) {
            if (fn_8022D75C(fn_80240644()) && !fn_80234ECC(fn_80240644()) && fn_8022D498(fn_80240644()) != 0xC &&
                fn_8022D498(fn_80240644()) != 0xD && fn_8022D498(fn_80240644()) != 0x26) {
                if (!fn_8024BBB0() &&
                    !fn_8022BC38(fn_80240644(), fn_8023E10C(fn_80240644()), fn_8023E214(fn_80240644()))) {
                    s32 state = field_0x1240;
                    if (state == 8 || state == 10 || state == 9 || state == 0x39) {
                        if (fn_8022D75C(fn_8022EA0C(field_0x4))) {
                            fn_80251AEC(0);
                        }
                    }
                    field_0xcee = 1;
                    if (fn_80234E30(fn_80240644())) {
                        field_0xcef = 1;
                    }
                    fn_80241E64();
                    if (fn_80234E30(fn_80240644())) {
                        field_0xce9 = 0;
                    }
                    if (!fn_8022BC38(fn_80240644(), field_0x3b4, &mPos)) {
                        bool low = false;
                        if (field_0x3b4 < 8) {
                            low = true;
                        }
                        field_0x3b4 = fn_8023DE58(fn_80240644());
                        field_0xce4 = field_0x3b4;
                        mPos = *fn_8023E724(fn_80240644());
                        field_0xcbc = mPos;
                        field_0xd1c = field_0xcbc;
                        if (low) {
                            field_0xcb0 = *fn_8023E32C(fn_80240644());
                        }
                        field_0xbec->fn_8030BD3C(field_0x4);
                    }
                }
            } else if ((field_0xcee == 1 || !fn_8022BC38(fn_80240644(), field_0x3b4, &mPos)) &&
                       (field_0xcef != 1 || !fn_80234E30(fn_80240644()))) {
                bool low = false;
                if (field_0x3b4 < 8) {
                    low = true;
                }
                field_0xcee = 0;
                field_0xcef = 0;
                field_0x3b4 = fn_8023DE58(fn_80240644());
                field_0xce4 = field_0x3b4;
                mPos = *fn_8023E724(fn_80240644());
                field_0xcbc = mPos;
                field_0xd1c = field_0xcbc;
                if (low) {
                    field_0xcb0 = *fn_8023E32C(fn_80240644());
                }
                field_0x3dc = field_0x3e8 = fn_80230CFC(fn_80240644());
                field_0xbec->fn_8030BD3C(field_0x4);
            }
        }

        if (field_0xcee == 0) {
            s32 serial = field_0xd88;
            if (serial >= 0) {
                if (fn_801F856C()->fn_801F666C(serial) != NULL) {
                    if (fn_8025EFFC(field_0xd88, true, false)) {
                        field_0xd88 = -1;
                    }
                } else {
                    field_0xd88 = -1;
                }
            }

            if (field_0x124c) {
                field_0x124c = false;
                switch (field_0x1240) {
                case 0x03:
                    fn_80285388();
                    break;
                case 0x04:
                    fn_8027D4F4();
                    break;
                case 0x05:
                    fn_8027CBB0();
                    break;
                case 0x06:
                    fn_8027CB24();
                    break;
                case 0x07:
                    fn_8027C6B8();
                    break;
                case 0x08:
                    fn_8027BC90();
                    break;
                case 0x09:
                    fn_8027B5F0();
                    break;
                case 0x0A:
                    fn_8027AA10();
                    break;
                case 0x0B:
                    fn_8027A178();
                    break;
                case 0x0D:
                    fn_80285074();
                    break;
                case 0x0E:
                    fn_80284DE4();
                    break;
                case 0x1A:
                    fn_8028496C();
                    break;
                case 0x33:
                    fn_80277EBC();
                    break;
                case 0x34:
                    fn_8027774C();
                    break;
                case 0x35:
                    fn_802775C8();
                    break;
                case 0x0C:
                    fn_80279F94();
                    break;
                case 0x0F:
                    fn_802796B8();
                    break;
                case 0x10:
                    fn_802791AC();
                    break;
                case 0x1B:
                    fn_8028447C();
                    break;
                case 0x38:
                    fn_80283D2C();
                    break;
                case 0x15:
                    fn_80263854();
                    break;
                case 0x16:
                    fn_802636D0();
                    break;
                case 0x17:
                    fn_802635D4();
                    break;
                case 0x18:
                    fn_802632A4();
                    break;
                case 0x13:
                    fn_80262B8C();
                    break;
                case 0x14:
                    fn_80262234();
                    break;
                case 0x19:
                    fn_8026173C();
                    break;
                case 0x1C:
                case 0x1E:
                case 0x20:
                case 0x22:
                case 0x24:
                case 0x26:
                case 0x28:
                case 0x2A:
                case 0x2C:
                case 0x2E:
                case 0x30:
                    fn_80290E90();
                    break;
                case 0x1D:
                case 0x1F:
                case 0x21:
                case 0x23:
                case 0x25:
                case 0x27:
                case 0x29:
                case 0x2B:
                case 0x2D:
                case 0x2F:
                case 0x31:
                    fn_80290D58();
                    break;
                case 0x32:
                    fn_80290D58();
                    fn_80290F20();
                    break;
                case 0x36:
                    fn_802771C8();
                    break;
                case 0x37:
                    fn_80277100();
                    break;
                case 0x3C:
                    fn_8025B368();
                    break;
                case 0x3A:
                    fn_8025C7BC();
                    break;
                case 0x3B:
                    fn_8025BDD8();
                    break;
                case 0x39:
                    fn_8025D9FC();
                    break;
                case 0x40:
                    fn_80284290();
                    break;
                case 0x41:
                    fn_8025D6A0();
                    break;
                case 0x42:
                    fn_8025AEC4();
                    break;
                case 0x43:
                    fn_8025AAF4();
                    break;
                case 0x44:
                    fn_8025A0B0();
                    break;
                case 0x45:
                    fn_80259784();
                    break;
                case 0x46:
                    fn_80258BBC();
                    break;
                case 0x47:
                    fn_8025888C();
                    break;
                case 0x48:
                    fn_802586CC();
                    break;
                case 0x49:
                    fn_80283944();
                    break;
                case 0x4A:
                    fn_80282FC8();
                    break;
                case 0x4B:
                    fn_80282AA8();
                    break;
                case 0x4C:
                    fn_802828A8();
                    break;
                case 0x4D:
                    fn_8028211C();
                    break;
                case 0x4E:
                    fn_80281E84();
                    break;
                case 0x4F:
                    fn_80281A18();
                    break;
                case 0x50:
                    fn_802811C0();
                    break;
                case 0x51:
                    fn_80280DD8();
                    break;
                }
                field_0x124d = 0;
                field_0x1248 = field_0x1240;
                field_0x1240 = field_0x1244;
                field_0x437 = field_0x3f8;
                field_0x438 = field_0x3f9;
                field_0x439 = field_0x3fa;
                field_0x43a = field_0x3fb;
                field_0x43b = field_0x3fc;
                field_0x43c = field_0x3fd;
                field_0x3f8 = field_0x3f9 = 0;
                field_0x3fa = field_0x3fb = 0;
                field_0x3fc = field_0x3fd = 0;
                field_0x43d = 0;
                field_0x38c = 0.0f;
                field_0x390 = 0.0f;
                field_0x2b8.set(0.0f, 0.0f, 0.0f);
                fn_8025F698(true);
                switch (field_0x1240) {
                case 0x00:
                    fn_80285C28();
                    break;
                case 0x01:
                    fn_802859B8();
                    break;
                case 0x02:
                    fn_802857C8();
                    break;
                case 0x03:
                    fn_80285668();
                    break;
                case 0x04:
                    fn_8027DEA0();
                    break;
                case 0x05:
                    fn_8027D320();
                    break;
                case 0x06:
                    fn_8027CB9C();
                    break;
                case 0x07:
                    fn_8027CAA8();
                    break;
                case 0x08:
                    fn_8027C4FC();
                    break;
                case 0x09:
                    fn_8027BA2C();
                    break;
                case 0x0A:
                    fn_8027B304();
                    break;
                case 0x0B:
                    fn_8027A908();
                    break;
                case 0x0C:
                    fn_8027A0D4();
                    break;
                case 0x11:
                    fn_80264768();
                    break;
                case 0x12:
                    fn_80264410();
                    break;
                case 0x15:
                    fn_80263BA0();
                    break;
                case 0x16:
                    fn_80263848();
                    break;
                case 0x17:
                    fn_802636C4();
                    break;
                case 0x18:
                    fn_802635C8();
                    break;
                case 0x13:
                    fn_80263078();
                    break;
                case 0x14:
                    fn_80262B6C();
                    break;
                case 0x19:
                    fn_80262188();
                    break;
                case 0x0D:
                    fn_8028537C();
                    break;
                case 0x0E:
                    fn_80285068();
                    break;
                case 0x0F:
                    fn_80279CFC();
                    break;
                case 0x10:
                    fn_8027966C();
                    break;
                case 0x1A:
                    fn_80284DD0();
                    break;
                case 0x1B:
                    fn_802848F0();
                    break;
                case 0x1C:
                    fn_80294ADC();
                    break;
                case 0x1D:
                    fn_80294374();
                    break;
                case 0x1E:
                    fn_80293E8C();
                    break;
                case 0x1F:
                    fn_80293B1C();
                    break;
                case 0x20:
                    fn_802938B8();
                    break;
                case 0x21:
                    fn_80293760();
                    break;
                case 0x22:
                    fn_80293534();
                    break;
                case 0x23:
                    fn_802932BC();
                    break;
                case 0x24:
                    fn_80292E90();
                    break;
                case 0x25:
                    fn_80292944();
                    break;
                case 0x26:
                    fn_802927B0();
                    break;
                case 0x27:
                    fn_802925C4();
                    break;
                case 0x28:
                    fn_80292454();
                    break;
                case 0x29:
                    fn_802922D0();
                    break;
                case 0x2A:
                    fn_80291CB8();
                    break;
                case 0x2B:
                    fn_80291B64();
                    break;
                case 0x2C:
                    fn_80295584();
                    break;
                case 0x2D:
                    fn_802954B8();
                    break;
                case 0x2E:
                    fn_80295334();
                    break;
                case 0x2F:
                    fn_80295240();
                    break;
                case 0x30:
                    fn_80292124();
                    break;
                case 0x31:
                    fn_80291E44();
                    break;
                case 0x32:
                    fn_8029185C();
                    break;
                case 0x33:
                    fn_802790C0();
                    break;
                case 0x34:
                    fn_80277E18();
                    break;
                case 0x35:
                    fn_8027769C();
                    break;
                case 0x36:
                    fn_80277580();
                    break;
                case 0x37:
                    fn_802771BC();
                    break;
                case 0x38:
                    fn_8028414C();
                    break;
                case 0x39:
                    fn_8025E79C();
                    break;
                case 0x3A:
                    fn_8025D090();
                    break;
                case 0x3B:
                    fn_8025C79C();
                    break;
                case 0x3C:
                    fn_8025BDC8();
                    break;
                case 0x3D:
                    fn_8025B35C();
                    break;
                case 0x3E:
                    fn_80283D20();
                    break;
                case 0x3F:
                    fn_80284430();
                    break;
                case 0x40:
                    fn_80284328();
                    break;
                case 0x41:
                    fn_8025D7E4();
                    break;
                case 0x42:
                    fn_8025B218();
                    break;
                case 0x43:
                    fn_8025AEB0();
                    break;
                case 0x44:
                    fn_8025A9C0();
                    break;
                case 0x45:
                    fn_8025A04C();
                    break;
                case 0x46:
                    fn_80259660();
                    break;
                case 0x47:
                    fn_80258BA8();
                    break;
                case 0x48:
                    fn_80258880();
                    break;
                case 0x49:
                    fn_80283CEC();
                    break;
                case 0x4A:
                    fn_80283848();
                    break;
                case 0x4B:
                    fn_80282E6C();
                    break;
                case 0x4C:
                    fn_80282A24();
                    break;
                case 0x4D:
                    fn_8028286C();
                    break;
                case 0x4E:
                    fn_80282094();
                    break;
                case 0x4F:
                    fn_80281D50();
                    break;
                case 0x50:
                    fn_80281760();
                    break;
                case 0x51:
                    fn_802810E4();
                    break;
                }
            }

            fn_80131F0C();
            if (fn_80131D98() || lbl_8053ABA8 || lbl_8053ABF0) {
                fn_80238548(field_0x4, 0x20, (const JGeometry::TVec2f*)&mPos, 0x12);
            } else if (fn_8024189C() <= 0 && !fn_80270A8C()) {
                fn_80251AEC(0x33);
            }
            field_0x11e9 = 0;
            field_0x1200 = false;
            if (field_0x1201 == 1) {
                field_0x1201 = 0;
            } else {
                fn_8027F16C();
            }
            field_0x1202 = false;
            field_0xb78 = false;
            field_0x35c.z = 0.0f;
            if (fn_80241380() || field_0x1240 == 8 || field_0x1240 == 0x37) {
                mPos += field_0x35c;
                if (PSVECMag(&field_0x35c) > 2.0f) {
                    field_0x35c.scale(0.95f);
                } else {
                    field_0x35c.scale(0.9f);
                }
            } else if (field_0x1240 == 0x12 || field_0x1240 == 0x15 || field_0x1240 == 0x16 ||
                       field_0x1240 == 0x17 || field_0x1240 == 0x18) {
                mPos += field_0x35c;
                field_0x35c.scale(0.6f);
            } else {
                field_0x35c.scale(0.9f);
            }
            if (PSVECMag(&field_0x35c) < 0.2f) {
                field_0x35c.x = 0.0f;
                field_0x35c.y = 0.0f;
                field_0x35c.z = 0.0f;
            }

            switch (field_0x1240) {
            case 0x00:
                fn_802859C4();
                break;
            case 0x01:
                fn_802857F0();
                break;
            case 0x02:
                fn_80285690();
                break;
            case 0x03:
                fn_8028538C();
                break;
            case 0x04:
                fn_8027D5F0();
                break;
            case 0x05:
                fn_8027CC90();
                break;
            case 0x06:
                fn_8027CB44();
                break;
            case 0x07:
                fn_8027C6C4();
                break;
            case 0x08:
                fn_8027BE74();
                break;
            case 0x09:
                fn_8027B648();
                break;
            case 0x0A:
                fn_8027AAD0();
                break;
            case 0x0B:
                fn_8027A1E0();
                break;
            case 0x0C:
                fn_80279FE8();
                break;
            case 0x11:
                fn_8026441C();
                break;
            case 0x12:
                fn_80263C64();
                break;
            case 0x15:
                fn_80263860();
                break;
            case 0x16:
                fn_802636D4();
                break;
            case 0x17:
                fn_802635D8();
                break;
            case 0x18:
                fn_802632A8();
                break;
            case 0x13:
                fn_80262BAC();
                break;
            case 0x14:
                fn_80262254();
                break;
            case 0x19:
                fn_80261750();
                break;
            case 0x0D:
                fn_80285098();
                break;
            case 0x0E:
                fn_80284E04();
                break;
            case 0x0F:
                fn_80279824();
                break;
            case 0x10:
                fn_802791B0();
                break;
            case 0x1A:
                fn_80284978();
                break;
            case 0x1B:
                fn_802844F8();
                break;
            case 0x1C:
                fn_802948C0();
                break;
            case 0x1D:
                fn_80293FE4();
                break;
            case 0x1E:
                fn_80293B30();
                break;
            case 0x1F:
                fn_80293900();
                break;
            case 0x20:
                fn_80293774();
                break;
            case 0x21:
                fn_802935A4();
                break;
            case 0x22:
                fn_80293398();
                break;
            case 0x23:
                fn_80292ECC();
                break;
            case 0x24:
                fn_802929FC();
                break;
            case 0x25:
                fn_80292820();
                break;
            case 0x26:
                fn_802925E4();
                break;
            case 0x27:
                fn_802924AC();
                break;
            case 0x28:
                fn_802922EC();
                break;
            case 0x29:
                fn_80292198();
                break;
            case 0x2A:
                fn_80291B8C();
                break;
            case 0x2B:
                fn_80291910();
                break;
            case 0x2C:
                fn_802954DC();
                break;
            case 0x2D:
                fn_802953A0();
                break;
            case 0x2E:
                fn_80295268();
                break;
            case 0x2F:
                fn_8029513C();
                break;
            case 0x30:
                fn_80291F10();
                break;
            case 0x31:
                fn_80291D28();
                break;
            case 0x32:
                fn_80290FC0();
                break;
            case 0x33:
                fn_80277F98();
                break;
            case 0x34:
                fn_8027776C();
                break;
            case 0x35:
                fn_8027761C();
                break;
            case 0x36:
                fn_80277220();
                break;
            case 0x37:
                fn_80277104();
                break;
            case 0x38:
                fn_80283E70();
                break;
            case 0x39:
                fn_8025DACC();
                break;
            case 0x3A:
                fn_8025C894();
                break;
            case 0x3B:
                fn_8025BEB0();
                break;
            case 0x3C:
                fn_8025B3F0();
                break;
            case 0x3D:
                fn_8025B22C();
                break;
            case 0x3E:
                fn_80283D00();
                break;
            case 0x3F:
                fn_8028434C();
                break;
            case 0x40:
                fn_80284294();
                break;
            case 0x41:
                fn_8025D6AC();
                break;
            case 0x42:
                fn_8025AED0();
                break;
            case 0x43:
                fn_8025AB00();
                break;
            case 0x44:
                fn_8025A15C();
                break;
            case 0x45:
                fn_80259830();
                break;
            case 0x46:
                fn_80258BE8();
                break;
            case 0x47:
                fn_80258890();
                break;
            case 0x48:
                fn_802586D0();
                break;
            case 0x49:
                fn_80283948();
                break;
            case 0x4A:
                fn_80282FE0();
                break;
            case 0x4B:
                fn_80282ACC();
                break;
            case 0x4C:
                fn_80282958();
                break;
            case 0x4D:
                fn_80282178();
                break;
            case 0x4E:
                fn_80281E98();
                break;
            case 0x4F:
                fn_80281A90();
                break;
            case 0x50:
                fn_8028126C();
                break;
            case 0x51:
                fn_80280E44();
                break;
            }

            if (fn_802431B8() && !fn_8024BBB0()) {
                if (viewLayerFlag(field_0x3b4)) {
                    JGeometry::TVec3f move = mPos - field_0x14;
                    move.x *= 0.25f;
                    if (move.y < 0.0f) {
                        move.y *= 0.25f;
                    }
                    if (move.z > 0.0f) {
                        move.z *= 0.25f;
                    }
                    mPos.set(field_0x14);
                    mPos += move;
                } else {
                    JGeometry::TVec3f move = mPos - field_0x14;
                    move.x *= 0.25f;
                    move.y *= 0.25f;
                    if (move.z > 0.0f) {
                        move.z *= 0.25f;
                    }
                    mPos.set(field_0x14);
                    mPos += move;
                }
            }
        }
    }

    if (!field_0x3c0) {
        field_0x2c.set(mPos);
        field_0x3bc = field_0x3b4;
    }
    field_0x3c0 = false;
    u16 tile = fn_8022461C(&field_0xd1c, field_0x3b4);
    if (fn_802265DC(&field_0xd1c, field_0x3b4, true) || tile == 3 || tile == 0xE) {
        if (field_0x3b4 < 8) {
            field_0xd1c.set(field_0x2c4);
        } else {
            field_0xd1c.set(field_0xcbc);
        }
    }
    if (field_0x3dc == 3) {
        JGeometry::TVec3f pos = mPos;
        pos.y -= 10.0f;
        pos.z = 0.0f;
        lbl_8053AC98->fn_8029E508(&pos, field_0x4, field_0x3b4);
    }
    for (int i = 0; i < 0x50; i++) {
        field_0xdb0[i] = -1;
        field_0x1030[i] = -1;
    }
    if (field_0xcad == true && !fn_80246464()) {
        field_0xcad = false;
        field_0x403 = 0;
    }
    fn_80286E10();
    if (field_0x1215 == true) {
        if (field_0x1240 != 3) {
            fn_80251AEC(3);
        }
        mPos.set(*fn_8023E724(fn_80240644()));
        mPos.z = 0.0f;
    }
    fn_80242BC0();
    fn_80242940();
    fn_80242CB0();
    fn_80280B30();
    fn_8025250C();
    if (!fn_802413E8()) {
        fn_80270D08();
    }
    fn_80261468();
    fn_80265DB0();
    fn_8024EE24();
    fn_802585F4();
    fn_8025851C();
    fn_80258470();
    fn_80258384();
    fn_8025825C();
    fn_8025819C();
    fn_802580E0();
    fn_80258070();
    fn_80257FF4();
    fn_8025785C();
    fn_8024BBB8();
    fn_80247F58();
    fn_80247F0C();
    fn_8024762C();
    fn_80265AE4();
    fn_8027F1C8();
    if (fn_8025F590() && fn_80236B50(field_0xd70) && field_0x1240 != 0x11 && field_0x1244 != 0x11) {
        fn_8025F698(false);
    }
    if (field_0x488 > 0) {
        field_0x488--;
    }
    if (field_0xd58 > 0) {
        field_0xd58--;
    }
    if (field_0x122c > 0) {
        field_0x122c--;
    }
    if (field_0x3f4 > 0) {
        field_0x3f4--;
    }
    if (field_0xd84 > 0) {
        field_0xd84--;
    }
    if (field_0x1218 > 0) {
        field_0x1218--;
    }
    if (field_0x44c > 0) {
        field_0x44c--;
    }
    if (field_0x1220 > 0) {
        field_0x1220--;
    }
    if (field_0x3b0 > 0 && !fn_80240B84()) {
        field_0x3b0--;
    }
    if (fn_802413E8()) {
        field_0x1200 = false;
    }
    fn_80250090();
    if (field_0xd98 > 0) {
        field_0xd98--;
        if (field_0xd98 == 0) {
            field_0xd9c = -1;
        }
        if (field_0xd9c >= 0 && field_0x4 == fn_80239040(field_0xd9c)) {
            field_0xd9c = -1;
            field_0xd98 = 0;
        }
    }
    fn_8024F63C();
    if (field_0xda8 > 0) {
        field_0xda8--;
        if (field_0xda8 == 0) {
            field_0xdac = -1;
        }
        if (field_0xdac >= 0 && field_0x4 == fn_80238F78(field_0xdac)) {
            field_0xdac = -1;
            field_0xda8 = 0;
        }
    }
    fn_80285F44();
    fn_80286688();
    fn_80286B74();
    fn_8028676C();
    if (fn_80240808() && field_0xcfe == true && fn_802486A0() != 4) {
        field_0xcfe = false;
    }
    if (!fn_802413E8() && !fn_802DEBF8() && !fn_802DEBA0() && !lbl_8053AAF8->field_0x7e && !fn_80240670() &&
        fn_80246B10(0x40)) {
        if (fn_80246B10(0x100)) {
            Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
            if (!camera->field_0x7f4) {
                camera->field_0x7f5 = 1;
            }
        } else if (fn_80246B10(0x200)) {
            s32 player = field_0x4;
            Camera_804AA9E4* camera = lbl_8053AB10->field_0x24;
            if (!camera->field_0x7f5) {
                camera->field_0x7f4 = 1;
                camera->field_0x7f8 = player;
                camera->field_0x7f6 = 1;
            }
        }
    }

    if (fn_8025F418()) {
        if (fn_8022D75C(field_0xd70) && fn_80234E30(field_0xd70)) {
            field_0xca0 = 0;
        } else {
            field_0xca0 = fn_8023BCD4(field_0xd70);
        }
    } else if (!fn_8024BBB0()) {
        s32 partner;
        if (fn_802486A0() == 4 && (partner = fn_80248778(), fn_80240888() != partner)) {
            fn_80265D8C();
        } else if (fn_80240808() && fn_8022D75C(fn_80240644()) && fn_8022D498(fn_80240644()) != 0x27 &&
                   !fn_8022BC38(fn_80240644(), fn_8023E10C(fn_80240644()), fn_8023E214(fn_80240644()))) {
            field_0xca0 = 0;
        } else if (fn_802486A0() != 4 && fn_80240808() && fn_8023EA28(fn_80240644()) &&
                   !fn_80236B50(fn_80240644()) && !fn_80270A8C()) {
            fn_80265D68();
        } else {
            fn_80265D8C();
        }
    }

    if (fn_802DEA34()) {
        if (field_0x138c != NULL && field_0x3e4 != field_0x3dc) {
            field_0x138c->becomeInvalidEmitter();
            field_0x138c = NULL;
        }
        if (fn_802413E8() || field_0x456 || fn_80270A8C() || fn_802412A4() || !fn_802413F0() ||
            (fn_802486A0() == 4 && fn_80240670())) {
            invalidateEmitter(field_0x138c);
        } else if (field_0x138c == NULL) {
            switch (field_0x3dc) {
            case 0:
                field_0x138c = fn_8013CC50(field_0x3b4, &mPos, 0x501, fn_8024EE14(), fn_8024EE0C(), NULL, NULL,
                                           true, 1.0f);
                break;
            case 1:
                field_0x138c = fn_8013CC50(field_0x3b4, &mPos, 0x500, fn_8024EE14(), fn_8024EE0C(), NULL, NULL,
                                           true, 1.0f);
                break;
            case 2:
                field_0x138c = fn_8013CC50(field_0x3b4, &mPos, 0x4FF, fn_8024EE14(), fn_8024EE0C(), NULL, NULL,
                                           true, 1.0f);
                break;
            case 3:
                field_0x138c = fn_8013CC50(field_0x3b4, &mPos, 0x502, fn_8024EE14(), fn_8024EE0C(), NULL, NULL,
                                           true, 1.0f);
                break;
            }
        } else {
            fn_8013CB44(field_0x138c, &mPos, fn_8024EE14(), fn_8024EE0C(), true);
        }
    }

    if (fn_802DEABC()) {
        if (fn_802413E8() || fn_80270A8C() || fn_802412A4() || !fn_802413F0() ||
            (viewLayerFlag(field_0x3b4) &&
             (!fn_802518AC(true) ||
              !fn_802255F4(&JGeometry::TVec3f(mPos.x, 8.0f + mPos.y, mPos.z), field_0x3b4))) ||
            (!viewLayerFlag(field_0x3b4) && (mPos.z > 0.0f || !fn_802255F4(&mPos, field_0x3b4))) ||
            planarLength(mPos - field_0x14) < 1.0f) {
            invalidateEmitter(field_0x1390);
        } else if (field_0x1390 == NULL) {
            field_0x1390 = fn_8013CC50(field_0x3b4, &mPos, 0xD7, fn_8024EE14(), fn_8024EE0C(), NULL, NULL, true,
                                       1.0f);
        } else {
            fn_8013CB44(field_0x1390, &mPos, fn_8024EE14(), fn_8024EE0C(), false);
        }
    }

    fn_80226850(&mPos, field_0x3b4);
    if (fn_802413E8() || fn_80270A8C() || fn_802412A4() || fn_80240D50() || !fn_802413F0() ||
        fn_8026FC20() > 0 || viewLayerFlag(field_0x3b4) || mPos.z > 0.0f ||
        planarLength(mPos - field_0x14) < 1.0f ||
        !(fn_80224E88(&mPos, field_0x3b4) || fn_80224B88(&mPos, field_0x3b4))) {
        invalidateEmitter(field_0x1394);
    } else if (field_0x1394 == NULL) {
        field_0x1394 = fn_8013CC50(field_0x3b4, &mPos, 0x1B1, 0, -1, NULL, NULL, true, 1.0f);
    } else {
        fn_8013CB44(field_0x1394, &mPos, 0, -1, true);
    }

    if (!field_0xce9) {
        if (magnitude(field_0xcbc - mPos) > 10.0f && fn_80240808() && !fn_8022D75C(fn_80240644())) {
            field_0xce9 = 1;
        }
    }
}
