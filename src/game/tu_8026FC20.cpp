#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/BgRewrite.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/ViewLayer.h"

s32 Player_804A6F38::fn_8026FC20() {
    if (fn_80240808()) {
        return fn_8023A1A4(fn_80240644());
    }
    return field_0xbac;
}

s32 Player_804A6F38::fn_8026FC68() {
    if (field_0xb9c) {
        return field_0x3dc;
    }
    return -1;
}

bool Player_804A6F38::fn_8026FC84() {
    return field_0xb9b;
}

void Player_804A6F38::fn_8026FC8C() {
    field_0xc9c = 0;
    field_0xc84.x = 1.0f;
    field_0xc84.y = 1.0f;
    field_0xc84.z = 1.0f;
    field_0x468 = 10;
}

bool Player_804A6F38::fn_8026FCB0() {
    return field_0xc9c != 0;
}

JGeometry::TVec3f* Player_804A6F38::fn_8026FCC4() {
    if (field_0xb9a) {
        return &field_0x5c0;
    }
    return &mPos;
}

bool Player_804A6F38::fn_8026FCE0() {
    bool result = false;
    if (field_0xb9a || field_0xb9c || field_0xb9d) {
        result = true;
    }
    return result;
}

void Player_804A6F38::fn_8027023C() {
    for (s32 i = 0; i < 8; i++) {
        field_0x117c[i] = 0;
    }
    // The original walks the 4x4 byte table column by column.
    for (s32 i = 0; i < 4; i++) {
        for (s32 j = 0; j < 4; j++) {
            field_0x119c[j][i] = 0;
        }
    }
}

void Player_804A6F38::fn_802702A4() {
    if (field_0xc7c < 99999999) {
        field_0xc7c++;
    }
}

void Player_804A6F38::fn_802702C4() {
    if (field_0xc78 < 99999999) {
        field_0xc78++;
    }
}

s32 Player_804A6F38::fn_802702E4() {
    return field_0xc74;
}

void Player_804A6F38::fn_802702EC() {
    field_0xc74 = 0;
    lbl_80529DEC.field_0x1f4[field_0x4] = 0;
}

s32 Player_804A6F38::fn_80270310() {
    return field_0xc70;
}

// The body never reads value (the forwarder keeps it live).
void Player_804A6F38::fn_80270318(s32 value) {
    field_0xc74++;
    if (field_0xc74 >= 9999) {
        field_0xc74 = 9999;
    }
    if (field_0xc70 < field_0xc74) {
        field_0xc70 = field_0xc74;
    }
}

s32 Player_804A6F38::fn_80270350() {
    return field_0xc64;
}

s32 Player_804A6F38::fn_80270358() {
    return field_0xc50;
}

void Player_804A6F38::fn_802708D0() {
    field_0xd2d = 1;
    field_0xd30 = 0;
    field_0xcfc = 0;
}

void Player_804A6F38::fn_802708E8() {
    field_0xd2d = 1;
    field_0xd30 = 0;
    field_0xcfc = 0;
    deleteEmitter(&field_0x1308);
}

void Player_804A6F38::fn_802709C8() {
    lbl_8053AC98->fn_802A20F0(&field_0x30c, field_0x4, field_0x3b4, 0, 1);
}

bool Player_804A6F38::fn_80270A04() {
    bool result = false;
    if (fn_80241864() <= 0 && field_0xcfc) {
        result = true;
    }
    return result;
}

bool Player_804A6F38::fn_80270A58() {
    if (field_0xd2d || field_0x1244 == 0x34 || field_0x1240 == 0x34) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80270A8C() {
    if (field_0x1244 == 0x33 || field_0x1240 == 0x33 || field_0x1240 == 0x34) {
        return true;
    }
    return false;
}

bool Player_804A6F38::fn_80270C88() {
    if (fn_80241200()) {
        return true;
    }
    if (fn_802486A0() == 4) {
        if (fn_80240888() != fn_80248778()) {
            return true;
        }
    }
    return fn_80270ABC();
}

void Player_804A6F38::fn_80270D08() {
    if (field_0x464 > 0) {
        field_0x464--;
    }
    if (field_0x468 > 0) {
        field_0x468--;
    }
}

// The argument is subtracted from field_0xbfc.
void Player_804A6F38::fn_80270D34(s32 amount) {
    if (fn_80240808()) {
        field_0xbfc = fn_80237774(fn_80240644());
    } else {
        field_0xbfc -= amount;
    }
    if (field_0xbfc <= 0) {
        field_0xbfc = 0;
        fn_80251AEC(0x33);
    }
    if (amount > 0) {
        fn_802702EC();
    }
}

bool Player_804A6F38::fn_80271038(const JGeometry::TVec2f* pos) {
    return fn_80238548(field_0x4, 0, pos, 0xf);
}

bool Player_804A6F38::fn_80271068(const JGeometry::TVec2f* pos) {
    return fn_80238548(field_0x4, 0, pos, 0xe);
}

bool Player_804A6F38::fn_80271098(const JGeometry::TVec3f* pos) {
    // fn_80238548 reads only pos.x and pos.y
    return fn_80238548(field_0x4, 0, (const JGeometry::TVec2f*)pos, 0xd);
}

bool Player_804A6F38::fn_80272160(s32 damage, const JGeometry::TVec2f* pos, s32 arg) {
    return fn_80238548(field_0x4, damage, pos, arg);
}

s32 Player_804A6F38::fn_80272184() {
    return 4;
}

s32 Player_804A6F38::fn_8027218C() {
    if (field_0x1240 != 4) {
        return 0;
    }
    return field_0x45c;
}

s32 Player_804A6F38::fn_802721A8() {
    field_0xcae = 1;
    if (field_0x492 == 1) {
        return 4;
    }
    if (field_0xbf4 <= 0) {
        return 0;
    }
    if (fn_80241544() >= 2) {
        return field_0xbf4 + 2;
    }
    return field_0xbf4;
}

void Player_804A6F38::fn_8027221C(s32 value) {
    field_0xbf4 = value;
}

// FIXME: 81.51. The original unrolls the 72 entries as 8 loop trips of 9 compares, bumping
// the base register by 4 per entry; ours unrolls 9 trips of 8 with fixed offsets.
bool Player_804A6F38::fn_802729F0(s32 serial) {
    for (s32 i = 0; i < 72; i++) {
        if (field_0xef0[i] >= 0 && field_0xef0[i] == serial) {
            return true;
        }
    }
    return false;
}

static inline bool contains(const JGeometry::TBox2f& box, const JGeometry::TVec3f& p) {
    return p.x >= box.i.x && p.y >= box.i.y && p.x < box.f.x && p.y < box.f.y;
}

bool Player_804A6F38::fn_80272B4C(const JGeometry::TVec3f* pos) {
    for (s32 i = 0; i < 0x50; i++) {
        if (field_0xdb0[i] >= 0 && fn_802413F0() && field_0x1030[i] == field_0x3b4 && contains(field_0x5fc[i], *pos)) {
            return true;
        }
    }
    return false;
}

void Player_804A6F38::fn_80277100() {}

void Player_804A6F38::fn_80277104() {
    if (field_0x3fa < 2) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 6) {
        field_0x3fb = 1;
    } else if (field_0x3fa < 10) {
        field_0x3fb = 2;
    } else {
        field_0x3fb = 3;
    }
    if (field_0x3fa >= 14) {
        if (field_0xcad) {
            fn_80251AEC(8);
        } else {
            fn_80251AEC(0);
        }
    } else {
        field_0x3fa++;
    }
    fn_80268B3C();
}


void Player_804A6F38::fn_802771BC() {
    field_0x458 = 0;
}

void Player_804A6F38::fn_80277580() {
    if (field_0x1248 != 4) {
        field_0x45c = 0;
        field_0x3fa = 7;
    } else if (field_0x439 >= 7) {
        field_0x3fa = 7;
    } else {
        field_0x3fa = field_0x439;
    }
    field_0x458 = 0;
}

void Player_804A6F38::fn_8027761C() {
    if (field_0x3fc >= 0x18) {
        field_0x3fd = 3;
        fn_80251AEC(0xf);
    } else {
        field_0x3fd = (field_0x3fc >> 1) % 3;
    }
    field_0x3fc++;
    fn_80268B3C();
}

void Player_804A6F38::fn_8027769C() {
    field_0x458 = 0;
    if (field_0x137c == NULL) {
        field_0x137c = fn_8013CC50(field_0x3b4, &mPos, 0x285, 0, -1, NULL, NULL, true, 1.0f);
    }
    if (field_0x1380 == NULL) {
        field_0x1380 = fn_8013CC50(field_0x3b4, &mPos, 0x286, 0, 3, NULL, NULL, true, 1.0f);
    }
    fn_8024F0E8(0x10013);
}

void Player_804A6F38::fn_8027774C() {
    field_0x464 = 0xb4;
    field_0xd2d = 0;
    field_0xd30 = 0;
    field_0xd35 = 0;
    field_0xc00 = 0;
}

void Player_804A6F38::fn_80277E18() {
    field_0x124d = 1;
    field_0xd30 = 0;
    if (field_0xd35 == 0) {
        if (fn_80241544() <= 1) {
            fn_8013CC50(field_0x3b4, &mPos, 0x3ae, 1, 1, NULL, NULL, true, 1.0f);
        } else {
            fn_8013CC50(field_0x3b4, &mPos, 0x3af, 1, 1, NULL, NULL, true, 1.0f);
        }
    }
}

void Player_804A6F38::fn_802790C0() {
    if (fn_8025F448() && !fn_8025F418()) {
        fn_8025FEDC();
    }
    fn_8025F698(false);
    if (field_0xc64 < 99999999) {
        field_0xc64++;
    }
    field_0x124d = 1;
    field_0x464 = 0;
    field_0x494 = 0;
    field_0x498 = 1;
    field_0x458 = 0;
    field_0xcfd = 0;
    field_0xd08 = -1;
    field_0xd0c = 0;
    field_0xc00 = 1;
    fn_8024F0E8(0x10005);
    fn_8024EFF8(10);
    if (!fn_80240808()) {
        fn_802382D0(field_0x4, 0);
    }
    mPos.z = 0.0f;
    field_0xba4 = 0;
}

void Player_804A6F38::fn_802791AC() {}

void Player_804A6F38::fn_8027966C() {
    field_0x464 = 60;
    field_0x458 = 0;
    fn_8024F0E8(0x10004);
    field_0x350.z = 0.0f;
}

void Player_804A6F38::fn_80279F94() {
    deleteEmitter(&field_0x1308);
    field_0x464 = 60;
}

void Player_804A6F38::fn_8027AA10() {
    deleteEmitter(&field_0x12b8);
    deleteEmitter(&field_0x12bc);
    deleteEmitter(&field_0x1374);
    deleteEmitter(&field_0x1378);
    field_0x11c0 = 0;
}

void Player_804A6F38::fn_8027B5F0() {
    field_0x11c0 = 0;
    field_0x11ac = 0;
    deleteEmitter(&field_0x12bc);
}

void Player_804A6F38::fn_8027C6B8() {
    field_0x436 = 0;
}

// FIXME: 78.55. Same instructions; in the original the float store and the li 8 / lis come
// before the argument setup (lfs, stfs, li, lis, stw, mr, addi), ours hoists lis/li/mr above the stfs.
void Player_804A6F38::fn_8027CAA8() {
    field_0x458 = 0;
    field_0x436 = 1;
    if (viewLayerFlag(field_0x3b4)) {
        field_0x344.y = 0.0f;
        field_0xbf4 = 8;
        fn_8024F0E8(0x1000b);
    }
}

void Player_804A6F38::fn_8027CB24() {
    if (field_0x1244 != 0xf) {
        mPos.z = 0.0f;
    }
    field_0x436 = 0;
}

void Player_804A6F38::fn_8027CB44() {
    if (field_0x3fa < 15) {
        field_0x3fa++;
    } else {
        fn_80251AEC(0);
    }
    field_0x3fb = 2;
    fn_802685D4();
}

void Player_804A6F38::fn_8027CB9C() {
    field_0x458 = 0;
    field_0x436 = 1;
}

void Player_804A6F38::fn_8027D4F4() {
    s32 count;
    if (field_0x117c[4] <= 0) {
        count = 0;
    } else if (field_0x117c[5] <= 0) {
        count = 1;
    } else if (field_0x117c[6] <= 0) {
        count = 2;
    } else if (field_0x117c[7] <= 0) {
        count = 3;
    } else {
        count = 4;
    }
    if (count >= 4) {
        fn_8027023C();
    }
    if (field_0x45c == 0 || field_0x45c == 1 || field_0x45c == 2) {
        field_0x11cc = 20;
        field_0x11d0 = field_0x3dc;
    } else {
        field_0x11cc = 0;
    }
}
