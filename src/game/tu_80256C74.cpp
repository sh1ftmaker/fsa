#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "game/act/act_somaria_block.h"
#include "game/Map.h"
#include "game/cls_804A71A8.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/ViewLayer.h"
#include "game/act/act_arrow.h"
#include "game/act/tu_801F9484.h"
#include "game/VecEpsilon.h"
#include "JSystem/JParticle/JPAMath.h"

// One entry per facing (0x3DC: right, left, down, up): 8 to the side and 10
// below, read by the enter of state 0x39 (fn_8025E79C).
static const Vec lbl_8046AF00[4] = {
    {-8.0f, 0.0f, -10.0f}, {8.0f, 0.0f, -10.0f}, {0.0f, -8.0f, -10.0f}, {0.0f, 0.0f, -10.0f},
};

// State 0x39's update (fn_8025DACC) reads these three from one base; the
// walk frame per eighth of the cycle and facing, and two per-facing offset
// sets (not yet read as code: fn_8025DACC is unwritten).
static const u8 lbl_8046AF30[4][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 4, 5, 6},
    {0, 1, 2, 3, 0, 4, 5, 6},
};
static const Vec lbl_8046AF50[8] = {
    {7.999f, 0.0f, 0.0f}, {-7.999f, 0.0f, 0.0f}, {0.0f, 7.999f, 0.0f}, {0.0f, -7.999f, 0.0f},
    {6.0f, 6.0f, 0.0f},   {-6.0f, 6.0f, 0.0f},   {6.0f, -6.0f, 0.0f},  {-6.0f, -6.0f, 0.0f},
};
static const Vec lbl_8046AFB0[4] = {
    {34.0f, 0.0f, 0.0f}, {-34.0f, 0.0f, 0.0f}, {0.0f, 34.0f, 0.0f}, {0.0f, -34.0f, 0.0f},
};

// The recoil hop's speed per facing (0x3DC: right, left, down, up): 2 back
// against the facing and 4 up (enter of state 0x41, fn_8025D7E4).
static const Vec lbl_8046AFE0[4] = {
    {-2.0f, 0.0f, 4.0f}, {2.0f, 0.0f, 4.0f}, {0.0f, -2.0f, 4.0f}, {0.0f, 2.0f, 4.0f},
};

// Per-facing offsets read by state 0x3A's update (fn_8025C894) and state 0x3B's
// update (fn_8025BEB0; with the index pairs lbl_8046B070), both unwritten.
static const Vec lbl_8046B010[4] = {
    {12.0f, 18.0f, 20.0f}, {-12.0f, 18.0f, 20.0f}, {2.0f, 28.0f, 20.0f}, {-2.0f, 2.0f, 20.0f},
};
static const Vec lbl_8046B040[4] = {
    {12.0f, 18.0f, 20.0f}, {-12.0f, 18.0f, 20.0f}, {2.0f, 28.0f, 20.0f}, {-2.0f, 2.0f, 20.0f},
};
static const s32 lbl_8046B070[4][2] = {
    {4, 6}, {5, 7}, {4, 5}, {6, 7},
};

// Per-facing offsets read off one base by state 0x3C's update (fn_8025B3F0, unwritten).
static const Vec lbl_8046B090[4] = {
    {-16.0f, -2.0f, 16.0f}, {16.0f, -2.0f, 16.0f}, {0.0f, -6.0f, 16.0f}, {0.0f, -4.0f, 16.0f},
};
static const Vec lbl_8046B0C0[8] = {
    {20.0f, 7.0f, 0.0f}, {-20.0f, 7.0f, 0.0f}, {0.0f, 12.0f, 0.0f}, {0.0f, -8.0f, 0.0f},
    {24.0f, 7.0f, 0.0f}, {-24.0f, 7.0f, 0.0f}, {0.0f, 11.0f, 0.0f}, {0.0f, -6.0f, 0.0f},
};
static const Vec lbl_8046B120[4] = {
    {16.0f, 0.0f, 0.0f}, {-16.0f, 0.0f, 0.0f}, {0.0f, 16.0f, 0.0f}, {0.0f, -16.0f, 0.0f},
};

// State 0x44 (fn_8025A15C, unwritten): the same walk frames and emitter rotations
// as state 0x45's below.
static const u8 lbl_8046B150[4][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 4, 5, 6},
    {0, 1, 2, 3, 0, 4, 5, 6},
};
static const s32 lbl_8046B170[4] = {0xC000, 0x4000, 0x8000, 0x0000};
static const s32 lbl_8046B180[4] = {0xC000, 0x4000, 0x0000, 0x8000};

// State 0x45 (fn_80259830): the walk frame (0x3F9) for each eighth of the
// 28-frame cycle 0x3F8, per facing (0x3DC).
static const u8 lbl_8046B190[4][8] = {
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 1, 2, 3},
    {0, 1, 2, 3, 0, 4, 5, 6},
    {0, 1, 2, 3, 0, 4, 5, 6},
};

// State 0x45: the y / z rotations of the emitters 0x12C4 / 0x12C8 per facing.
static const s32 lbl_8046B1B0[4] = {0xC000, 0x4000, 0x8000, 0x0000};
static const s32 lbl_8046B1C0[4] = {0xC000, 0x4000, 0x0000, 0x8000};

// Not read by any function of the unit (between state 0x45's tables and the dig
// effects; kept at its original place and size).
static const Vec lbl_8046B1D0[4] = {
    {-1.0f, 0.0f, -8.0f}, {1.0f, 0.0f, -8.0f}, {5.0f, 0.0f, -8.0f}, {-5.0f, 0.0f, -8.0f},
};

// The dig effect per result of fn_802A1844 (1-4) and facing (fn_80258890).
static const u16 lbl_8046B200[4][4] = {
    {0x137, 0x132, 0x12D, 0x13C},
    {0x135, 0x134, 0x133, 0x136},
    {0x13A, 0x139, 0x138, 0x13B},
    {0x130, 0x12F, 0x12E, 0x131},
};

// One step of 12.999 along each facing (0: +x, 1: -x, 2: +y, 3: -y): where item 8
// ('HBMB') is placed in front of the player (fn_8025785C).
static const Vec lbl_8046B220[4] = {
    {12.999f, 0.0f, 0.0f}, {-12.999f, 0.0f, 0.0f}, {0.0f, 12.999f, 0.0f}, {0.0f, -12.999f, 0.0f},
};

// Inlined by fn_80259660 and fn_80258BE8 (no out-of-line copy).
inline void Player_804A6F38::gatherOthers() {
    if (fn_802486A0() == 5 && !fn_80240808()) {
        for (int i = 0; i < 4; i++) {
            if (fn_80243194(i) && i != field_0x4) {
                fn_8023E690(i, &mPos);
            }
        }
        fn_8024BAEC();
    }
}

bool Player_804A6F38::fn_80256C74(const JGeometry::TBox2f* box, s32 layer) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!fn_80270A8C()) {
        return false;
    }
    if (!field_0xba4) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5dc);
    if (overlap.intersect(*box)) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80256DCC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!field_0xb9e) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5cc);
    if (overlap.intersect(*box)) {
        out->x = field_0x5cc.f.x + (field_0x5cc.i.x - field_0x5cc.f.x) * 0.5f;
        out->y = field_0x5cc.f.y + (field_0x5cc.i.y - field_0x5cc.f.y) * 0.5f;
        return true;
    }
    return false;
}

u8 Player_804A6F38::fn_80256F44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (!field_0xb99) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x5b0);
    if (overlap.intersect(*box)) {
        out->x = field_0x5b0.f.x + (field_0x5b0.i.x - field_0x5b0.f.x) * 0.5f;
        out->y = field_0x5b0.f.y + (field_0x5b0.i.y - field_0x5b0.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802570BC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    for (s32 i = 0; i < 3; i++) {
        if (field_0xb1c[i] >= 0 && field_0xb5c[i] == layer) {
            JGeometry::TBox2f overlap(field_0x550[i]);
            if (overlap.intersect(*box)) {
                out->x = field_0x550[i].f.x + (field_0x550[i].i.x - field_0x550[i].f.x) * 0.5f;
                out->y = field_0x550[i].f.y + (field_0x550[i].i.y - field_0x550[i].f.y) * 0.5f;
                return true;
            }
        }
    }
    if (field_0xb28 >= 0 && layer == field_0x3b4) {
        JGeometry::TBox2f overlap(field_0x580);
        if (overlap.intersect(*box)) {
            out->x = field_0x580.f.x + (field_0x580.i.x - field_0x580.f.x) * 0.5f;
            out->y = field_0x580.f.y + (field_0x580.i.y - field_0x580.f.y) * 0.5f;
            return true;
        }
    }
    if (field_0xb3c >= 0 && field_0xb70 == layer) {
        JGeometry::TBox2f overlap(field_0x5a0);
        if (overlap.intersect(*box)) {
            out->x = field_0x5a0.f.x + (field_0x5a0.i.x - field_0x5a0.f.x) * 0.5f;
            out->y = field_0x5a0.f.y + (field_0x5a0.i.y - field_0x5a0.f.y) * 0.5f;
            return true;
        }
    }
    return false;
}

bool Player_804A6F38::fn_802573E0(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (field_0xb38 < 0) {
        return false;
    }
    if (field_0xb6c != layer) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x590);
    if (overlap.intersect(*box)) {
        out->x = field_0x590.f.x + (field_0x590.i.x - field_0x590.f.x) * 0.5f;
        out->y = field_0x590.f.y + (field_0x590.i.y - field_0x590.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802574CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    if (field_0xb18 < 0) {
        return false;
    }
    if (field_0xb58 != layer) {
        return false;
    }
    JGeometry::TBox2f overlap(field_0x540);
    if (overlap.intersect(*box)) {
        out->x = field_0x540.f.x + (field_0x540.i.x - field_0x540.f.x) * 0.5f;
        out->y = field_0x540.f.y + (field_0x540.i.y - field_0x540.f.y) * 0.5f;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_802575B8(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    for (s32 i = 0; i < 3; i++) {
        if (field_0xb0c[i] >= 0 && field_0xb4c[i] == layer) {
            JGeometry::TBox2f overlap(field_0x510[i]);
            if (overlap.intersect(*box)) {
                out->x = field_0x510[i].f.x + (field_0x510[i].i.x - field_0x510[i].f.x) * 0.5f;
                out->y = field_0x510[i].f.y + (field_0x510[i].i.y - field_0x510[i].f.y) * 0.5f;
                field_0xb80 = i;
                return true;
            }
        }
    }
    field_0xb80 = -1;
    return false;
}

bool Player_804A6F38::fn_802576CC(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out) {
    for (s32 i = 0; i < 3; i++) {
        if (field_0xb00[i] >= 0 && field_0xb40[i] == layer) {
            JGeometry::TBox2f overlap(field_0x4e0[i]);
            if (overlap.intersect(*box)) {
                out->x = field_0x4e0[i].f.x + (field_0x4e0[i].i.x - field_0x4e0[i].f.x) * 0.5f;
                out->y = field_0x4e0[i].f.y + (field_0x4e0[i].i.y - field_0x4e0[i].f.y) * 0.5f;
                field_0xb7c = i;
                return true;
            }
        }
    }
    field_0xb7c = -1;
    return false;
}

bool Player_804A6F38::fn_802577E0() {
    if (fn_80270C88()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (field_0x1240 == 0x3f || field_0x1240 == 0xc) {
        return false;
    }
    return fn_80251AEC(0x3f);
}

// FIXME: 90.40. Structurally close: the original leaves item 7's checks (living held
// actor, a player holding 'RCK2', the floor probe / height) and item 6's actor count by
// branching straight to the final store, which `blocked` / `count` stand in for here
// (9 extra instructions, and this / the item index swap r30 / r31).
// The per-frame item check: while fn_8022B334 and fn_8022C310 hold, the selected
// item fn_802416C4 (1-10) requests its state. Items 3 and 6 allow three of their
// actors (field_0xb00 / field_0xb1c) at a time, item 8 ('HBMB') two (field_0xb30).
void Player_804A6F38::fn_8025785C() {
    if (field_0xb74 > 0) {
        field_0xb74--;
    }
    if (fn_8022B334(field_0x4)) {
        if (fn_80240808()) {
            s32 item = fn_802416C4();
            if (item != fn_802373EC(fn_80240644())) {
                fn_80241618(fn_802373EC(fn_80240644()), 1);
            }
        }
        if (fn_8022C310(field_0x4)) {
            if (fn_802416C4() == 1) {
                fn_80251AEC(0x39);
            } else if (fn_802416C4() == 2) {
                fn_80251AEC(0x48);
            } else if (fn_802416C4() == 3) {
                if (field_0xb18 < 0) {
                    s32 count = 0;
                    for (s32 i = 0; i < 3; i++) {
                        if (field_0xb00[i] >= 0) {
                            count++;
                        }
                    }
                    if (count < 3) {
                        fn_80251AEC(0x3d);
                    }
                }
            } else if (fn_802416C4() == 4) {
                if (field_0xb18 < 0 && field_0xb74 <= 0 &&
                    (!fn_8022B160() || ((!fn_80240808() || fn_802486A0() == 4) && !lbl_8053ABB8[field_0x4]))) {
                    fn_80251AEC(0x3a);
                }
            } else if (fn_802416C4() == 5) {
                fn_80251AEC(0x3c);
            } else if (fn_802416C4() == 6) {
                if (fn_802415CC() >= 2) {
                    fn_80251AEC(0x45);
                } else {
                    s32 count = 0;
                    if (fn_802415CC() < 2) {
                        for (s32 i = 0; i < 3; i++) {
                            if (field_0xb1c[i] >= 0) {
                                count++;
                            }
                        }
                    }
                    if (count < 3) {
                        fn_80251AEC(0x44);
                    }
                }
            } else if (fn_802416C4() == 7) {
                bool blocked = false;
                if (fn_8025F500()) {
                    ActorBase_804A5770* held = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xd68);
                    if (held != NULL && held->field_0x1a4 != 0) {
                        blocked = true;
                    }
                }
                for (s32 i = 0; !blocked && i < 4; i++) {
                    if (fn_80243194(i) && fn_8023EEB4(i) == 'RCK2') {
                        blocked = true;
                    }
                }
                if (!blocked) {
                    if (!viewLayerFlag(field_0x3b4)) {
                        if (mPos.z > 0.0f) {
                            blocked = true;
                        }
                    } else if (!fn_802518AC(true)) {
                        JGeometry::TVec3f pos;
                        fn_8024B298(2, &pos);
                        pos.y += 2.0f;
                        if (fn_8022461C(&pos, field_0x3b4) == 2) {
                            blocked = true;
                        }
                    }
                }
                if (!blocked) {
                    if (fn_8025F684()) {
                        fn_80251AEC(0x14);
                    } else {
                        fn_80251AEC(0x46);
                    }
                }
            } else if (fn_802416C4() == 8) {
                for (s32 i = 0; i < 2; i++) {
                    if (field_0xb30[i] < 0) {
                        JGeometry::TVec3f pos(lbl_8046B220[field_0x3dc]);
                        pos += mPos;
                        pos.z = 0.0f;
                        if (fn_8022461C(&pos, field_0x3b4) == 0xe) {
                            break;
                        }
                        s32 serial;
                        if (fn_802415CC() >= 2) {
                            serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, field_0x3b4, &pos, 8, -1, field_0x4);
                        } else {
                            serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_HBMB, field_0x3b4, &pos, 0, -1, field_0x4);
                        }
                        if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
                            field_0xb30[i] = serial;
                            fn_8024F0E8(0x20006);
                            break;
                        }
                    }
                }
            } else if (fn_802416C4() == 9) {
                fn_80251AEC(0x47);
            } else if (fn_802416C4() == 10) {
                if (field_0xb18 < 0) {
                    fn_80251AEC(0x3b);
                }
            } else if (fn_80246228()) {
                field_0x1200 = true;
            }
        }
    }
    field_0x1174 = 0;
}

bool Player_804A6F38::fn_80257DB4() {
    if (field_0x1174) {
        return false;
    }
    if (fn_80240808() && fn_80230188(fn_80240644())) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_8025F684() && fn_802416C4() != 7) {
        return false;
    }
    if (fn_8025F448()) {
        return false;
    }
    if (fn_802412A4()) {
        return false;
    }
    if (fn_8024C448()) {
        return false;
    }
    if (fn_80241230()) {
        return false;
    }
    if (fn_80250D30()) {
        return false;
    }
    if (fn_802411C4()) {
        return false;
    }
    if (fn_80240B70()) {
        return false;
    }
    if (fn_80240B5C()) {
        return false;
    }
    if (fn_80240B84()) {
        return false;
    }
    if (fn_8024BBB0()) {
        return false;
    }
    if (fn_80240D64()) {
        return false;
    }
    if (fn_80240DAC()) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (fn_8026FCB0()) {
        return false;
    }
    if (fn_802412C4()) {
        return false;
    }
    if (field_0xd74 != 0) {
        return false;
    }
    return field_0xd3c < 0;
}

void Player_804A6F38::fn_80257FF4() {
    s32 serial; // declared outside the loop: the original keeps the counter in the lower saved register
    for (s32 i = 0; i < 2; i++) {
        serial = field_0xb30[i];
        if (serial >= 0 && !getActorManager_801F856C()->findActorBySerial_801F666C(serial)) {
            field_0xb30[i] = -1;
        }
    }
}

void Player_804A6F38::fn_80258070() {
    s32 serial = field_0xb2c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0xb2c = -1;
        } else if (!static_cast<CaneOfSomariaBlock_804AA9F0*>(actor)->fn_8030EA0C()) {
            field_0xb2c = -1;
        }
    }
}

void Player_804A6F38::fn_802580E0() {
    f32 target;
    if (fn_80270A8C()) {
        target = 0.0f;
    } else if (gGameRoot_8053AAF8->field_0x7f) {
        if (fn_802416C4() == 2) {
            target = 100.0f;
        } else {
            target = 0.0f;
        }
    } else {
        target = 85.0f;
    }
    if (field_0xb90 > target) {
        field_0xb90 -= 1.0f;
        if (field_0xb90 < target) {
            field_0xb90 = target;
        }
    } else if (field_0xb90 < target) {
        field_0xb90 += 1.0f;
        if (field_0xb90 > target) {
            field_0xb90 = target;
        }
    }
}

void Player_804A6F38::fn_8025819C() {
    s32 serial = field_0xb3c;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor == NULL) {
            field_0xb3c = -1;
            field_0xb70 = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb70 = actor->field_0x4;
            field_0x5a0.set(actor->mBox);
            field_0x5a0.addPos(pos);
        }
    }
}

void Player_804A6F38::fn_8025825C() {
    s32 serial = field_0xb38;
    if (serial < 0) {
        field_0xb68 = 0;
        return;
    }
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    if (actor == NULL) {
        field_0xb68 = 0;
        field_0xb38 = -1;
        field_0xb6c = -1;
        return;
    }
    if (actor->field_0x3c.isZero()) {
        field_0xb68 = 0;
    } else {
        field_0xb68 = 1;
    }
    JGeometry::TVec2<f32> pos;
    pos.x = actor->mPos.x;
    pos.y = actor->mPos.y;
    field_0xb6c = actor->field_0x4;
    field_0x590.set(actor->mBox);
    field_0x590.addPos(pos);
}

void Player_804A6F38::fn_80258384() {
    s32 serial; // declared outside the loop: the original keeps the counter in the lower saved register
    for (s32 i = 0; i < 3; i++) {
        serial = field_0xb1c[i];
        if (serial >= 0) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor == NULL) {
                field_0xb1c[i] = -1;
                field_0xb5c[i] = -1;
            } else {
                JGeometry::TVec2<f32> pos;
                pos.x = actor->mPos.x;
                pos.y = actor->mPos.y;
                field_0xb5c[i] = actor->field_0x4;
                field_0x550[i].set(actor->mBox);
                field_0x550[i].addPos(pos);
            }
        }
    }
}

void Player_804A6F38::fn_80258470() {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb18);
    if (actor == NULL) {
        field_0xb18 = -1;
        field_0xb58 = -1;
    } else {
        JGeometry::TVec2<f32> pos;
        pos.x = actor->mPos.x;
        pos.y = actor->mPos.y;
        field_0xb58 = actor->field_0x4;
        field_0x540.set(actor->mBox);
        field_0x540.addPos(pos);
    }
}

void Player_804A6F38::fn_8025851C() {
    for (s32 i = 0; i < 3; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb0c[i]);
        if (actor == NULL) {
            field_0xb0c[i] = -1;
            field_0xb4c[i] = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb4c[i] = actor->field_0x4;
            field_0x510[i].set(actor->mBox);
            field_0x510[i].addPos(pos);
        }
    }
}

void Player_804A6F38::fn_802585F4() {
    for (s32 i = 0; i < 3; i++) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb00[i]);
        if (actor == NULL) {
            field_0xb00[i] = -1;
            field_0xb40[i] = -1;
        } else {
            JGeometry::TVec2<f32> pos;
            pos.x = actor->mPos.x;
            pos.y = actor->mPos.y;
            field_0xb40[i] = actor->field_0x4;
            field_0x4e0[i].set(actor->mBox);
            field_0x4e0[i].addPos(pos);
        }
    }
}

void Player_804A6F38::fn_802586CC() {}

// Update of state 0x48 (item 2, the Fire Rod: it spawns ACTOR_FIRE, "Fire Rod fire"):
// at frame 2, when fn_80247594(0) allows it and the player's last fire (field_0xb3c)
// is gone, one is spawned by the player, with the item level fn_802415CC in bits 12+
// of its argument; the state ends after 8 frames.
void Player_804A6F38::fn_802586D0() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 0;
    } else {
        field_0x3fb = 0;
        fn_80251AEC(0);
    }
    if (field_0x3fa == 2 && fn_80247594(0) && field_0xb3c < 0) {
        JGeometry::TVec3f pos = mPos;
        switch (field_0x3dc) {
        case 0:
            pos.y -= 2.0f;
            break;
        case 1:
            pos.y -= 2.0f;
            break;
        case 2:
            pos.x += 2.0f;
            break;
        case 3:
            pos.x -= 2.0f;
            break;
        }
        s32 serial; // declared before index: the original gives serial the higher saved register
        s32 index = field_0x4;
        serial = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_FIRE, field_0x3b4, &pos,
                                                (fn_802415CC() << 12) | ((field_0x3dc & 3) << 4) | (index & 3) | 0x140,
                                                -1, index);
        if (getActorManager_801F856C()->findActorBySerial_801F666C(serial) != NULL) {
            field_0xb3c = serial;
        }
    }
    field_0x3fa++;
    fn_80268B3C();
}

void Player_804A6F38::fn_80258880() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025888C() {}

// FIXME: 99.97. Instructions identical; only the lhz of the effect table lbl_8046B200 is marked
// (the table sits in this unit's still-unwritten .rodata, so its placement differs).
// Update of state 0x47 (item 9): a dig 16 in front of the player at floor level.
// Frame 11 sets the 8x8 box field_0x5cc there (field_0xb9e), frame 12 asks the
// background (fn_802A1844) and plays a sound by the result; results 1-4 give an
// effect by facing at frame 28. 0x3FD selects the short animation (frames 4-6).
void Player_804A6F38::fn_80258890() {
    JGeometry::TVec3f pos = mPos;
    pos.z = 0.0f;
    switch (field_0x3dc) {
    case 0:
        pos.x += 16.0f;
        break;
    case 1:
        pos.x -= 16.0f;
        break;
    case 2:
        pos.y += 16.0f;
        break;
    case 3:
        pos.y -= 16.0f;
        break;
    }
    if (field_0x3fd == 1) {
        if (field_0x3fa < 16) {
            field_0x3fb = 4;
        } else if (field_0x3fa < 19) {
            field_0x3fb = 5;
        } else if (field_0x3fa < 22) {
            field_0x3fb = 6;
        } else {
            field_0x3fb = 6;
            fn_80251AEC(0);
        }
    } else {
        if (field_0x3fa < 12) {
            field_0x3fb = 0;
        } else if (field_0x3fa < 28) {
            field_0x3fb = 1;
        } else if (field_0x3fa < 32) {
            field_0x3fb = 2;
        } else if (field_0x3fa < 40) {
            field_0x3fb = 3;
        } else {
            field_0x3fb = 3;
            fn_80251AEC(0);
        }
        if (field_0x3fa == 11) {
            field_0xb9e = true;
            field_0x5cc.set(pos.x - 4.0f, pos.y - 4.0f, pos.x + 4.0f, pos.y + 4.0f);
        }
        if (field_0x3fa == 12) {
            field_0xba0 = gBgRewrite_8053AC98->fn_802A1844(&pos, field_0x4, field_0x3b4);
            if ((field_0xba0 > 0 && field_0xba0 <= 4) || field_0xb9f) {
                fn_8024F0E8(0x20044);
            } else {
                field_0x3fb = 0;
                field_0x3fd = 1;
                if (fn_8022461C(&pos, field_0x3b4) == 0x39) {
                    fn_8024F0E8(0x20040);
                } else if (field_0xba0 == 8) {
                    fn_8024F0E8(0x2004F);
                } else {
                    fn_8024F0E8(0x20066);
                }
            }
            field_0xb9e = false;
            field_0xb9f = false;
        }
        if (field_0x3fa == 28 && field_0x3fd == 0 && field_0xba0 > 0 && field_0xba0 <= 4) {
            spawnEmitter_8013CC50(field_0x3b4, &mPos, lbl_8046B200[field_0xba0 - 1][field_0x3dc], 0, 0, NULL, NULL, true, 1.0f);
        }
    }
    field_0x3fa++;
    fn_80268B3C();
}

void Player_804A6F38::fn_80258BA8() {
    field_0x458 = 0;
    field_0xb9f = false;
    field_0xba0 = 0;
}

void Player_804A6F38::fn_80258BBC() {
    if (field_0x1244 != 5 && field_0x1244 != 7) {
        mPos.z = 0.0f;
    }
    field_0xb79 = 0;
    field_0x1204 = 0;
}

// FIXME: 99.69. scheduling+regalloc: before the x/y speed test of the no-pad branch the original
// already computes &field_0x344 (PSVECMag's argument, addi r3) and so takes r4 for
// __float_epsilon; ours computes it after the test. Otherwise only .sdata2 literal names.
// The jump (enter fn_80259660, exit fn_80258BBC): the pad steers field_0x344 in x / y
// (0.8 from rest, 0.3 a frame rising, 0.2 falling, stopped by the side flags of
// field_0xbf0) at most 1.6 a frame; gravity 0.25 down to -4; on a 2D layer
// (viewLayerFlag) z moves y. Landing keeps the last speed in field_0x368 and gathers the
// others (gatherOthers); at item level >= 2 (fn_802415CC) a second jump (field_0xb79);
// after 33 frames on the floor the button jumps again, else 3 frames end it (state 0);
// 0x3FB is the animation frame.
void Player_804A6F38::fn_80258BE8() {
    if (fn_8022C6AC(field_0x4) > 0.0f) {
        if (0.0f == field_0x344.x) {
            field_0x344.x = 0.8f * fn_8022C82C(field_0x4);
        } else if (field_0x344.x > 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.x += 0.3f * fn_8022C82C(field_0x4);
            } else {
                field_0x344.x += 0.2f * fn_8022C82C(field_0x4);
            }
            if (field_0x344.x <= 0.001f || field_0xbf0->field_0x48[0]) {
                field_0x344.x = 0.001f;
            }
        } else if (field_0x344.x < 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.x += 0.3f * fn_8022C82C(field_0x4);
            } else {
                field_0x344.x += 0.2f * fn_8022C82C(field_0x4);
            }
            if (field_0x344.x >= -0.001f || field_0xbf0->field_0x48[1]) {
                field_0x344.x = -0.001f;
            }
        }
        if (0.0f == field_0x344.y) {
            field_0x344.y = 0.8f * -fn_8022C76C(field_0x4);
        } else if (field_0x344.y > 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.y -= 0.3f * fn_8022C76C(field_0x4);
            } else {
                field_0x344.y -= 0.2f * fn_8022C76C(field_0x4);
            }
            if (field_0x344.y <= 0.001f || field_0xbf0->field_0x48[2]) {
                field_0x344.y = 0.001f;
            }
        } else if (field_0x344.y < 0.0f) {
            if (field_0x344.z >= 0.0f) {
                field_0x344.y -= 0.3f * fn_8022C76C(field_0x4);
            } else {
                field_0x344.y -= 0.2f * fn_8022C76C(field_0x4);
            }
            if (field_0x344.y >= -0.001f || field_0xbf0->field_0x48[3]) {
                field_0x344.y = -0.001f;
            }
        }
        f32 z = field_0x344.z;
        field_0x344.z = 0.0f;
        f32 length = field_0x344.length();
        f32 speed = length;
        if (length >= 1.6f) {
            speed = 1.6f;
        }
        field_0x344.normalize();
        field_0x344.scale(speed);
        field_0x344.z = z;
    } else if (!(field_0x344.x * field_0x344.x + field_0x344.y * field_0x344.y <= JGeometry::TUtilf::epsilon())) {
        f32 z = field_0x344.z;
        field_0x344.z = 0.0f;
        f32 speed = field_0x344.length();
        if (fn_8022C274(field_0x4)) {
            if (z >= 0.0f) {
                speed += 0.3f;
                if (speed >= 1.6f) {
                    speed = 1.6f;
                }
            } else {
                speed += 0.2f;
                if (speed >= 1.6f) {
                    speed = 1.6f;
                }
            }
        } else {
            speed -= 0.1f;
            if (speed <= 0.001f) {
                speed = 0.001f;
            }
        }
        field_0x344.normalize();
        field_0x344.scale(speed);
        field_0x344.z = z;
    }

    if (!field_0x344.isZero() || mPos.z > 0.0f) {
        if (field_0x3fa >= 1) {
            field_0x344.z -= 0.25f;
            if (field_0x344.z < -4.0f) {
                field_0x344.z = -4.0f;
            }
            f32 height = mPos.z;
            if (viewLayerFlag(field_0x3b4)) {
                mPos.x += field_0x344.x;
                mPos.y -= field_0x344.z;
                mPos.z = 0.5f;
                if (field_0x344.z < 0.0f && field_0xbf0->field_0x48[2]) {
                    field_0x368.set(field_0x344.x, field_0x344.y, 0.0f);
                    field_0x344.set(0.0f, 0.0f, 0.0f);
                    mPos.z = 0.0f;
                    if (height > 0.0f && (fn_8024B24C() || field_0x1204)) {
                        gatherOthers();
                    }
                }
            } else {
                mPos += field_0x344;
                if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
                    mPos.z = 0.0f;
                    field_0x368.set(field_0x344.x, field_0x344.y, 0.0f);
                    field_0x344.set(0.0f, 0.0f, 0.0f);
                    if (height > 0.0f && (fn_8024B24C() || field_0x1204)) {
                        gatherOthers();
                    }
                }
            }
        }
    }

    if (!field_0xb79 && mPos.z > 0.0f && fn_802415CC() >= 2 && fn_8022C310(field_0x4)) {
        field_0x3fb = 0;
        field_0x3fa = 0;
        field_0x3fc = 0;
        field_0x344.z = 5.4f;
        field_0xb79 = 1;
    }

    if (field_0x3fa < 4) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 9) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 13) {
        field_0x3fb = 2;
    } else if (field_0x3fa < 17) {
        field_0x3fb = 3;
    } else if (field_0x3fa < 21) {
        field_0x3fb = 4;
    } else if (field_0x3fa < 25) {
        field_0x3fb = 5;
    } else if (field_0x3fa < 29) {
        field_0x3fb = 6;
    } else if (field_0x3fa < 33) {
        field_0x3fb = 7;
    } else if (mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        if (fn_8022C310(field_0x4)) {
            field_0x3fb = 0;
            field_0x3fa = 0;
            field_0x3fc = 0;
            field_0x344.z = 5.4f;
            field_0xb79 = 0;
            if (fn_8024B24C()) {
                gatherOthers();
            }
            if (fn_8024B24C()) {
                field_0x1204 = 1;
            } else {
                field_0x1204 = 0;
            }
        } else {
            field_0x3fb = 0;
            field_0x3fc++;
            if (field_0x3fc >= 3) {
                fn_80251AEC(0);
            }
        }
    } else {
        field_0x3fb = 7;
    }

    if (field_0x3fa >= 8 && viewLayerFlag(field_0x3b4) && field_0x344.z < 0.0f && mPos.z <= 1.0f) {
        JGeometry::TVec3f probe;
        fn_8024B298(2, &probe);
        probe.y += 2.0f;
        if (fn_8022461C(&probe, field_0x3b4) != 2) {
            fn_80251AEC(0);
        }
    }

    if (!field_0x458 && field_0x3fa >= 8 && fn_8022C3AC(field_0x4)) {
        if (fn_80243234()) {
            fn_80251AEC(7);
        } else {
            fn_80251AEC(5);
        }
    }
    field_0x3fa++;
    fn_802685D4();
}

// FIXME: 99.93. reloc-name only: 0.0f / 5.4f (lbl_805400C4 / lbl_8054010C) get pool slots
// out of the original order while the big state handlers before them are unwritten.
void Player_804A6F38::fn_80259660() {
    field_0x458 = 0;
    fn_8027EB04();
    field_0x344.set(0.0f, 0.0f, 0.0f);
    field_0x344.z = 5.4f;
    fn_8024F0E8(0x10002);
    field_0x368.set(0.0f, 0.0f, 0.0f);
    field_0xb79 = 0;
    field_0x1204 = 0;
    if (fn_8024B24C()) {
        gatherOthers();
    }
    if (fn_8024B24C()) {
        field_0x1204 = 1;
    } else {
        field_0x1204 = 0;
    }
}

void Player_804A6F38::fn_80259778() {
    field_0x344.z = 5.4f;
}

void Player_804A6F38::fn_80259784() {
    if (field_0x12c4 != NULL) {
        field_0x12c4->becomeInvalidEmitter();
        field_0x12c4 = NULL;
    }
    if (field_0x12c8 != NULL) {
        field_0x12c8->becomeInvalidEmitter();
        field_0x12c8 = NULL;
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802BAA00();
        }
        field_0xb28 = -1;
    }
}

// FIXME: 99.29. scheduling+regalloc: the box's f.x / f.y (pos + 36) take their fadds
// operands the other way round in cases 0 and 2, the epsilon test of epsilonEquals keeps
// 32 * FLT_EPSILON in f2 (ours f3), and the emitters' rotation-table address is formed
// after the matrix pointer (ours before).
// State 0x45 (item 6 at level >= 2; enter fn_8025A04C, exit fn_80259784): frame 11 places
// a Cane of Somaria block ('SMRI', 16 in front, field_0xb2c; message 0xE8 once, 0xBB8)
// or acts on the one that exists (fn_8030EA00); while the button is held (0xB84,
// fn_8022C274) the player stays at frame 12 walking at 1.125 (fn_802808E8) with the
// 28-frame walk cycle, keeps a 'FIRE' actor (0xB28; 60 frames 0xB8C after it goes) on
// the box field_0x580 in front (fn_802BAA0C) and the emitters 0x1AB / 0x54A
// (0x12C4 / 0x12C8) turned to the facing; letting go stops them and runs to frame 13.
void Player_804A6F38::fn_80259830() {
    if (field_0xb8c > 0) {
        field_0xb8c--;
    }
    if (field_0xb84 == 1 && !fn_8022C274(field_0x4)) {
        field_0xb84 = 0;
    }
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 3) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 4) {
        field_0x3fb = 2;
    } else if (field_0x3fa < 10) {
        field_0x3fb = 3;
    } else if (field_0x3fa < 11) {
        field_0x3fb = 4;
    } else if (field_0x3fa < 13) {
        field_0x3fb = 5;
    } else {
        field_0x3fb = 5;
        fn_80251AEC(0);
    }
    if (!field_0xb84 && field_0x3fa == 11) {
        if (!field_0xbb8) {
            lbl_8053AED0->fn_8043D67C(0xe8, field_0x4, 0);
            field_0xbb8 = 1;
        }
        CaneOfSomariaBlock_804AA9F0* block = static_cast<CaneOfSomariaBlock_804AA9F0*>(
            getActorManager_801F856C()->findActorBySerial_801F666C(field_0xb2c));
        if (block == NULL) {
            if (fn_80247594(0)) {
                JGeometry::TVec3f pos = mPos;
                switch (field_0x3dc) {
                case 0:
                    pos.x += 16.0f;
                    break;
                case 1:
                    pos.x -= 16.0f;
                    break;
                case 2:
                    pos.y += 16.0f;
                    break;
                case 3:
                    pos.y -= 16.0f;
                    break;
                }
                field_0xb2c = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_SMRI, field_0x3b4, &pos, 0, -1,
                                                                              field_0x4);
            }
        } else {
            block->fn_8030EA00();
        }
    }
    if (field_0x3fa == 12) {
        JGeometry::TVec3f pos = mPos;
        JGeometry::TBox2f box;
        switch (field_0x3dc) {
        case 0:
            pos.x += 18.0f;
            box.set(pos.x, pos.y - 6.0f, pos.x + 36.0f, pos.y + 6.0f);
            field_0x580.set(box);
            break;
        case 1:
            pos.x -= 18.0f;
            box.set(pos.x - 36.0f, pos.y - 6.0f, pos.x, pos.y + 6.0f);
            field_0x580.set(box);
            break;
        case 2:
            pos.x += 4.0f;
            pos.y += 12.0f;
            box.set(pos.x - 6.0f, pos.y, pos.x + 6.0f, pos.y + 36.0f);
            field_0x580.set(box);
            break;
        case 3:
            pos.x -= 4.0f;
            pos.y -= 20.0f;
            box.set(pos.x - 6.0f, pos.y - 36.0f, pos.x + 6.0f, pos.y);
            field_0x580.set(box);
            break;
        }
        if (field_0xb84) {
            fn_802808E8(1.125f);
            if (epsilonEquals(mPos, field_0x14, JGeometry::TUtilf::epsilon())) {
                field_0x3f8 = 0;
            } else {
                field_0x3f8++;
                if (field_0x3f8 >= 28) {
                    field_0x3f8 = 0;
                }
            }
            if (field_0x3f8 < 4) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][0];
            } else if (field_0x3f8 < 6) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][1];
            } else if (field_0x3f8 < 10) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][2];
            } else if (field_0x3f8 < 14) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][3];
            } else if (field_0x3f8 < 18) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][4];
            } else if (field_0x3f8 < 20) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][5];
            } else if (field_0x3f8 < 24) {
                field_0x3f9 = lbl_8046B190[field_0x3dc][6];
            } else {
                field_0x3f9 = lbl_8046B190[field_0x3dc][7];
            }
            if (!fn_80247594(1)) {
                field_0xb84 = 0;
            }
            if (field_0xb8c <= 0 && field_0xb28 < 0) {
                s32 index = field_0x4;
                field_0xb28 = getActorManager_801F856C()->spawnActor_801F9484(
                    ACTOR_FIRE, field_0x3b4, &pos, ((field_0x3dc & 3) << 4) | 0x1040 | (index & 3), -1, index);
            }
            s32 fire = field_0xb28;
            if (fire >= 0) {
                FireRodFire_804A89F0* actor = static_cast<FireRodFire_804A89F0*>(
                    getActorManager_801F856C()->findActorBySerial_801F666C(fire));
                if (actor != NULL) {
                    actor->fn_802BAA0C(&box);
                } else {
                    field_0xb28 = -1;
                    field_0xb8c = 60;
                }
            }
            if (field_0x12c4 == NULL) {
                field_0x12c4 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x1ab, 0, 2, NULL, NULL, true, 1.0f);
            }
            if (field_0x12c4 != NULL) {
                JPAGetXYZRotateMtx(0, lbl_8046B1B0[field_0x3dc], -0x8000, field_0x12c4->mGlobalRot);
                setEmitterPos_8013CB44(field_0x12c4, &pos, 0, 2, true);
            }
            if (field_0x12c8 == NULL) {
                field_0x12c8 = spawnEmitter_8013CC50(field_0x3b4, &pos, 0x54a, -1, 0, NULL, NULL, true, 1.0f);
            }
            if (field_0x12c8 != NULL) {
                JPAGetXYZRotateMtx(0, 0, lbl_8046B1C0[field_0x3dc], field_0x12c8->mGlobalRot);
                setEmitterPos_8013CB44(field_0x12c8, &pos, -1, 0, true);
            }
            fn_8024F0E8(0x30011);
        } else {
            field_0x3fa++;
            if (field_0x12c8 != NULL) {
                field_0x12c8->stopCreateParticle();
            }
            if (field_0x12c4 != NULL) {
                field_0x12c4->stopCreateParticle();
            }
        }
    } else {
        field_0x3fa++;
    }
    fn_80268B3C();
}

void Player_804A6F38::fn_8025A04C() {
    field_0x458 = 0;
    if (fn_80241794() <= 0) {
        field_0xb84 = false;
    } else {
        field_0xb84 = true;
    }
    field_0xb88 = 0;
    field_0xb28 = -1;
    field_0xb8c = 0;
}

void Player_804A6F38::fn_8025A0B0() {
    if (field_0x12c4 != NULL) {
        field_0x12c4->becomeInvalidEmitter();
        field_0x12c4 = NULL;
    }
    if (field_0x12c8 != NULL) {
        field_0x12c8->becomeInvalidEmitter();
        field_0x12c8 = NULL;
    }
    s32 serial = field_0xb28;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802BAA00();
        }
        field_0xb28 = -1;
    }
}

void Player_804A6F38::fn_8025A9C0() {
    field_0x458 = 0;
    if (fn_80241794() <= 0) {
        field_0xb84 = false;
    } else {
        field_0xb84 = true;
    }
    field_0xb88 = 0;
    field_0xb28 = -1;
    field_0xb8c = 0;
}

bool Player_804A6F38::fn_8025AA24(s32 arg) {
    if (field_0x11f8 >= 0) {
        return false;
    }
    if (fn_80251AEC(0x43)) {
        field_0x11f8 = arg;
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_8025AA8C(s32 arg) {
    if (field_0x11f8 >= 0) {
        return false;
    }
    if (fn_80251AEC(0x42)) {
        field_0x11f8 = arg;
        return true;
    }
    return false;
}

void Player_804A6F38::fn_8025AAF4() {
    field_0x11f8 = -1;
}

// FIXME: 99.96. reloc-name only: 6.0f (lbl_80540114) and 3.0f (lbl_805400FC) get pool
// slots out of the original order while the big state handlers before them are unwritten.
// State 0x43 (from act_arrow via fn_8022E1A0): the player is drawn toward the arrow
// actor 0x11F8 at min(distance, 6) a frame across the floor, facing along the pull,
// until the arrow is gone or its field_0x230 leaves 0, field_0xafc is 1 or a side flag
// of field_0xbf0 is set (anyFlag48); the animation frame 0x3FD steps at 10 and 14.
void Player_804A6F38::fn_8025AB00() {
    field_0x3fc++;
    if (field_0x3fc < 10) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 14) {
        field_0x3fd = 1;
    } else {
        field_0x3fd = 2;
    }
    fn_80268B3C();
    if (field_0xafc != 1 && !field_0xbf0->anyFlag48()) {
        ArrowPlayer_804A05E0* arrow =
            static_cast<ArrowPlayer_804A05E0*>(getActorManager_801F856C()->findActorBySerial_801F666C(field_0x11f8));
        if (arrow != NULL && arrow->field_0x230 == 0) {
            JGeometry::TVec3f dir = arrow->mPos - mPos;
            f32 dist = dir.length();
            f32 speed = dist;
            if (dist > 6.0f) {
                speed = 6.0f;
            }
            dir.z = 0.0f;
            dir.normalize();
            if (!dir.isZero()) {
                dir.scale(speed);
                mPos += dir;
                if (dir.x + dir.y < 0.0f) {
                    if (dir.x - dir.y < 0.0f) {
                        field_0x3dc = 0;
                        field_0x3e8 = 0;
                    } else {
                        field_0x3dc = 2;
                        field_0x3e8 = 2;
                    }
                } else if (dir.x - dir.y < 0.0f) {
                    field_0x3dc = 3;
                    field_0x3e8 = 3;
                } else {
                    field_0x3dc = 1;
                    field_0x3e8 = 1;
                }
            }
            mPos.z = 0.0f;
            return;
        }
    }
    field_0x124d = 0;
    fn_80251AEC(0);
}

void Player_804A6F38::fn_8025AEB0() {
    field_0x124d = 1;
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025AEC4() {
    field_0x11f8 = -1;
}

// FIXME: 99.81. regalloc-only: the original keeps the speed 6.0f in f30 and dir.z of
// the isZero test in f31 (ours swaps them), as fn_8025AB00 (state 0x43) does with its
// variable speed.
// Update of state 0x42 (requested by fn_8025AA8C with the actor serial field_0x11f8):
// the player spins (the facing turns every two frames) and is drawn 6 a frame across
// the floor toward that actor; it ends (state 0) when the actor is gone or on another
// layer, field_0xafc is 1, or a side flag of field_0xbf0 is set (anyFlag48).
void Player_804A6F38::fn_8025AED0() {
    field_0x3fc++;
    s32 turn = (field_0x3fc >> 1) & 3;
    if (turn == 0) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (turn == 1) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    } else if (turn == 2) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    }
    fn_80268B3C();
    if (field_0xafc != 1 && !field_0xbf0->anyFlag48()) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x11f8);
        if (actor != NULL && actor->field_0x4 == field_0x3b4) {
            JGeometry::TVec3f dir = actor->mPos - mPos;
            dir.z = 0.0f;
            dir.normalize();
            if (!dir.isZero()) {
                dir.scale(6.0f);
                mPos += dir;
            }
            mPos.z = 0.0f;
            return;
        }
    }
    field_0x124d = 0;
    fn_80251AEC(0);
}

void Player_804A6F38::fn_8025B218() {
    field_0x124d = 1;
    field_0x458 = 0;
}

// FIXME: 95.59. scheduling+regalloc: the original keeps this in r31 and field_0x4 in r30 (ours
// the other way round) and builds the spawn argument from field_0x4 & 3 first.
void Player_804A6F38::fn_8025B22C() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 4) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 2;
    } else {
        field_0x3fb = 3;
    }
    if (field_0x3fa == 6) {
        if (fn_802415CC() >= 2) {
            s32 index = field_0x4;
            field_0xb18 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BOME, field_0x3b4, &mPos,
                                                     ((field_0x3e8 & 7) << 4) | (index & 3) | 0x100, -1, index);
        } else {
            s32 index = field_0x4;
            field_0xb18 = getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BOME, field_0x3b4, &mPos,
                                                     ((field_0x3e8 & 7) << 4) | (index & 3), -1, index);
        }
    } else if (field_0x3fa == 16) {
        fn_80251AEC(0);
    }
    field_0x3fa++;
    fn_80268B3C();
}

void Player_804A6F38::fn_8025B35C() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_8025B368() {
    field_0xb99 = false;
    field_0xb9a = false;
    field_0xb9b = false;
    if (field_0x1274 != NULL) {
        field_0x1274->clearStatus(JPAEMIT_Immortal);
        field_0x1274->becomeInvalidEmitter();
        field_0x1274 = NULL;
    }
    if (field_0x128c != NULL) {
        field_0x128c->clearStatus(JPAEMIT_Immortal);
        field_0x128c->becomeInvalidEmitter();
        field_0x128c = NULL;
    }
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025BDC8() {
    field_0x458 = 0;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025BDD8() {
    if (field_0x12cc != NULL) {
        field_0x12cc->clearStatus(JPAEMIT_Immortal);
        field_0x12cc->becomeInvalidEmitter();
        field_0x12cc = NULL;
    }
    if (field_0x12e4 != NULL) {
        field_0x12e4->clearStatus(JPAEMIT_Immortal);
        field_0x12e4->becomeInvalidEmitter();
        field_0x12e4 = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025C79C() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025C7BC() {
    if (field_0x12cc != NULL) {
        field_0x12cc->clearStatus(JPAEMIT_Immortal);
        field_0x12cc->becomeInvalidEmitter();
        field_0x12cc = NULL;
    }
    if (field_0x12e4 != NULL) {
        field_0x12e4->clearStatus(JPAEMIT_Immortal);
        field_0x12e4->becomeInvalidEmitter();
        field_0x12e4 = NULL;
    }
    s32 serial = field_0x12c0;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x12c0 = -1;
    }
    field_0x40c = 0;
}

void Player_804A6F38::fn_8025D090() {
    field_0x458 = 0;
    field_0x344.x = 0.0f;
    field_0x344.y = 0.0f;
    field_0x344.z = 0.0f;
    field_0x40c = 0;
}

// FIXME: 99.79. reloc-name only: 3.0f (lbl_805400FC) and 2.1f (lbl_80540148) get pool
// slots out of the original order while the big state handlers before them are unwritten.
// The step of state 0x39's update (fn_8025DACC): with field_0xbac >= 1, 3 a frame along
// the facing; else along the facing until the pad (fn_8022C82C / fn_8022C76C of the
// player) pushes against it (field_0xba8 = 1), 2.1 diagonally while it pushes across,
// turning to that side after 8 such frames with the facing axis released (the turn
// from a side facing ends it instead while field_0xbf0->field_0xb4 is set, which also
// keeps y).
void Player_804A6F38::fn_8025D0B0() {
    f32 y = mPos.y;
    if (field_0xbac >= 1) {
        switch (field_0x3dc) {
        case 0:
            mPos.x += 3.0f;
            break;
        case 1:
            mPos.x -= 3.0f;
            break;
        case 2:
            mPos.y += 3.0f;
            break;
        case 3:
            mPos.y -= 3.0f;
            break;
        }
        return;
    }
    switch (field_0x3dc) {
    case 0:
        if (fn_8022C82C(field_0x4) < 0.0f) {
            field_0xba8 = 1;
        } else if (fn_8022C76C(field_0x4) > 0.0f) {
            mPos.x += 2.1f;
            mPos.y -= 2.1f;
            if (0.0f == fn_8022C82C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    if (field_0xbf0->field_0xb4) {
                        field_0xba8 = 1;
                    } else {
                        field_0x3dc = 3;
                        field_0x3e8 = 3;
                        field_0x3fc = 0;
                    }
                }
            }
        } else if (fn_8022C76C(field_0x4) < 0.0f) {
            mPos.x += 2.1f;
            mPos.y += 2.1f;
            if (0.0f == fn_8022C82C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    if (field_0xbf0->field_0xb4) {
                        field_0xba8 = 1;
                    } else {
                        field_0x3dc = 2;
                        field_0x3e8 = 2;
                        field_0x3fc = 0;
                    }
                }
            }
        } else {
            mPos.x += 3.0f;
            field_0x3fc = 0;
        }
        break;
    case 1:
        if (fn_8022C82C(field_0x4) > 0.0f) {
            field_0xba8 = 1;
        } else if (fn_8022C76C(field_0x4) > 0.0f) {
            mPos.x -= 2.1f;
            mPos.y -= 2.1f;
            if (0.0f == fn_8022C82C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    if (field_0xbf0->field_0xb4) {
                        field_0xba8 = 1;
                    } else {
                        field_0x3dc = 3;
                        field_0x3e8 = 3;
                        field_0x3fc = 0;
                    }
                }
            }
        } else if (fn_8022C76C(field_0x4) < 0.0f) {
            mPos.x -= 2.1f;
            mPos.y += 2.1f;
            if (0.0f == fn_8022C82C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    if (field_0xbf0->field_0xb4) {
                        field_0xba8 = 1;
                    } else {
                        field_0x3dc = 2;
                        field_0x3e8 = 2;
                        field_0x3fc = 0;
                    }
                }
            }
        } else {
            mPos.x -= 3.0f;
            field_0x3fc = 0;
        }
        break;
    case 2:
        if (fn_8022C76C(field_0x4) > 0.0f) {
            field_0xba8 = 1;
        } else if (fn_8022C82C(field_0x4) > 0.0f) {
            mPos.x += 2.1f;
            mPos.y += 2.1f;
            if (0.0f == fn_8022C76C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    field_0x3dc = 0;
                    field_0x3e8 = 0;
                    field_0x3fc = 0;
                }
            }
        } else if (fn_8022C82C(field_0x4) < 0.0f) {
            mPos.x -= 2.1f;
            mPos.y += 2.1f;
            if (0.0f == fn_8022C76C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    field_0x3dc = 1;
                    field_0x3e8 = 1;
                    field_0x3fc = 0;
                }
            }
        } else {
            mPos.y += 3.0f;
            field_0x3fc = 0;
        }
        break;
    case 3:
        if (fn_8022C76C(field_0x4) < 0.0f) {
            field_0xba8 = 1;
        } else if (fn_8022C82C(field_0x4) > 0.0f) {
            mPos.x += 2.1f;
            mPos.y -= 2.1f;
            if (0.0f == fn_8022C76C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    field_0x3dc = 0;
                    field_0x3e8 = 0;
                    field_0x3fc = 0;
                }
            }
        } else if (fn_8022C82C(field_0x4) < 0.0f) {
            mPos.x -= 2.1f;
            mPos.y -= 2.1f;
            if (0.0f == fn_8022C76C(field_0x4)) {
                field_0x3fc++;
                if (field_0x3fc > 8) {
                    field_0x3dc = 1;
                    field_0x3e8 = 1;
                    field_0x3fc = 0;
                }
            }
        } else {
            mPos.y -= 3.0f;
            field_0x3fc = 0;
        }
        break;
    }
    if (field_0xbf0->field_0xb4) {
        mPos.y = y;
    }
}

void Player_804A6F38::fn_8025D6A0() {
    field_0xb9c = false;
}

// FIXME: 99.87. Instructions identical; only the .sdata2 literals differ (@NNN vs lbl_80540104/8: the pool
// follows first use and this unit's unwritten functions use the earlier literals).
void Player_804A6F38::fn_8025D6AC() {
    if (field_0x3fc < 4) {
        field_0x3fd = 0;
    } else if (field_0x3fc < 8) {
        field_0x3fd = 1;
    } else if (field_0x3fc < 12) {
        field_0x3fd = 2;
    } else {
        field_0x3fd = 3;
    }
    field_0x3fc++;
    if (field_0x3fc == 2) {
        field_0xb9c = false;
    }
    mPos += field_0x344;
    field_0x344.z -= 0.25f;
    if (field_0x344.z < -4.0f) {
        field_0x344.z = -4.0f;
    }
    if (field_0x344.z <= 0.0f && mPos.z <= 0.0f) {
        mPos.z = 0.0f;
        field_0x124d = 0;
        fn_80251AEC(0);
    }
    fn_80268B3C();
    if (fn_8025197C(true)) {
        field_0x344.x = 0.0f;
        field_0x344.y = 0.0f;
    }
}

// FIXME: 96.34. Structurally close: the original reads the table entry through one
// pointer (add, then lfs 0 / 4 / 8); ours indexes x with lfsx (2 more instructions).
// Enter of state 0x41, the recoil hop: speed from lbl_8046AFE0 (back against the facing,
// 4 up), sound 0x20010, a view shake (fn_802D79E8, along y when the hop has a y speed),
// and on the TV the effect of kind 3 aimed back along the hop (fn_802F601C).
void Player_804A6F38::fn_8025D7E4() {
    field_0x124d = 1;
    field_0x458 = 0;
    field_0x344.set(lbl_8046AFE0[field_0x3dc].x, lbl_8046AFE0[field_0x3dc].y, lbl_8046AFE0[field_0x3dc].z);
    fn_8024F0E8(0x20010);
    if (field_0x344.y != 0.0f) {
        getGameView_802D6A9C()->fn_802D79E8(field_0x3b4, 4.0f, 0.0f, false);
    } else {
        getGameView_802D6A9C()->fn_802D79E8(field_0x3b4, 0.0f, 4.0f, false);
    }
    gGameRoot_8053AAF8->fn_80134DE4(1, 0);
    JGeometry::TVec3f offset;
    if (field_0x3dc == 0 || field_0x3dc == 1) {
        offset.set(0.0f, -8.0f, 0.0f);
    } else {
        offset.zero();
    }
    JGeometry::TVec3f dir(-field_0x344.x, -field_0x344.y, 0.0f);
    dir.normalize();
    JGeometry::TVec3f pos = mPos + offset;
    if (field_0x3b4 < 8) {
        gGameRoot2_8053AB10->field_0x20->fn_802F601C(&pos, 3, &dir, 1.0f, 1.0f);
    }
    field_0xb9c = true;
}

void Player_804A6F38::fn_8025D9FC() {
    if (field_0x1360 != NULL) {
        field_0x1360->becomeInvalidEmitter();
        field_0x1360 = NULL;
    }
    if (field_0x1368 != NULL) {
        field_0x1368->becomeInvalidEmitter();
        field_0x1368 = NULL;
    }
    if (field_0x1364 != NULL) {
        field_0x1364->becomeInvalidEmitter();
        field_0x1364 = NULL;
    }
    if (field_0xbac > 0 || fn_8026FC20() > 0) {
        field_0xce8 = false;
    }
    field_0xbac = 0;
    field_0xba8 = 0;
}

// FIXME: 89.92. Only the unused offset position differs: the original copies the table
// entry to the stack with three lfs / stfs and reloads it with psq_l; ours loads it with
// psq_l straight from the table (3 instructions fewer).
void Player_804A6F38::fn_8025E79C() {
    fn_8027221C(6);
    field_0x458 = 1;
    if (fn_80240808() && fn_802486A0() != 4) {
        s32 facing = fn_80230CFC(fn_80240644());
        field_0x3dc = facing;
        field_0x3e8 = facing;
    } else if (fn_80246B10(2)) {
        field_0x3dc = 0;
        field_0x3e8 = 0;
    } else if (fn_80246B10(1)) {
        field_0x3dc = 1;
        field_0x3e8 = 1;
    } else if (fn_80246B10(8)) {
        field_0x3dc = 3;
        field_0x3e8 = 3;
    } else if (fn_80246B10(4)) {
        field_0x3dc = 2;
        field_0x3e8 = 2;
    }
    // The result is unused, as in the original (it computes it and drops it).
    JGeometry::TVec3f pos(lbl_8046AF00[field_0x3dc]);
    pos += mPos;
    field_0xba8 = 0;
    field_0x3fa = 0;
    field_0xbac = 0;
}
