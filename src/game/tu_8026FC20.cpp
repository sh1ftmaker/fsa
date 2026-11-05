#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/EmitterDelete.h"
#include "game/BgRewrite.h"
#include "game/GameState.h"
#include "game/Player.h"
#include "game/ViewLayer.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"

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

void Player_804A6F38::fn_8026FD14() {
    if (fn_80246F28() > 0.0f) {
        if (field_0x11c8 >= 3) {
            field_0x11c4 = 3;
        } else if (field_0x11c4 > 0) {
            field_0x11c4--;
        }
        field_0x11c8 = 0;
    } else {
        if (field_0x11c8 < 3) {
            field_0x11c8++;
        }
        if (field_0x11c4 > 0) {
            field_0x11c4--;
        }
    }
    if (field_0x11d4 > 0) {
        field_0x11d4--;
    }
    if (field_0x11cc > 0) {
        field_0x11cc--;
    }

    if (fn_80247330() > 0.0f && fn_8024712C() == 0.0f) {
        field_0x117c[0] = 60;
        if (field_0x117c[2] < field_0x117c[1] && field_0x117c[3] < field_0x117c[1]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[0] > 0) {
        field_0x117c[0]--;
    }
    if (fn_80247330() < 0.0f && fn_8024712C() == 0.0f) {
        field_0x117c[1] = 60;
        if (field_0x117c[2] < field_0x117c[0] && field_0x117c[3] < field_0x117c[0]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[1] > 0) {
        field_0x117c[1]--;
    }
    if (fn_8024712C() < 0.0f && fn_80247330() == 0.0f) {
        field_0x117c[2] = 60;
        if (field_0x117c[0] < field_0x117c[3] && field_0x117c[1] < field_0x117c[3]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[2] > 0) {
        field_0x117c[2]--;
    }
    if (fn_8024712C() > 0.0f && fn_80247330() == 0.0f) {
        field_0x117c[3] = 60;
        if (field_0x117c[0] < field_0x117c[2] && field_0x117c[1] < field_0x117c[2]) {
            field_0x117c[2] = 0;
        }
    } else if (field_0x117c[3] > 0) {
        field_0x117c[3]--;
    }

    if (fn_80247330() <= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][0] = 0;
        }
    }
    if (fn_80247330() >= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][1] = 0;
        }
    }
    if (fn_8024712C() >= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][2] = 0;
        }
    }
    if (fn_8024712C() <= 0.0f) {
        for (int i = 0; i < 4; i++) {
            field_0x119c[i][3] = 0;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (field_0x117c[i + 4] > 0) {
            field_0x117c[i + 4]--;
        }
    }
}

void Player_804A6F38::fn_80270088() {
    if (fn_80246F28() > 0.0f) {
        if (field_0x119c[0][0] == 0 && field_0x119c[0][1] == 0 && field_0x119c[0][2] == 0 &&
            field_0x119c[0][3] == 0) {
            field_0x117c[7] = 0;
            field_0x117c[6] = 0;
            field_0x117c[5] = 0;
            field_0x117c[4] = 0;
        }
    } else if (field_0x119c[0][0] == 1 || field_0x119c[0][1] == 1 || field_0x119c[0][2] == 1 ||
               field_0x119c[0][3] == 1) {
        field_0x117c[7] = 0;
        field_0x117c[6] = 0;
        field_0x117c[5] = 0;
        field_0x117c[4] = 0;
    }
    for (int i = 3; i > 0; i--) {
        field_0x117c[i + 4] = field_0x117c[i + 3];
    }
    field_0x117c[4] = 60;
    if (fn_80247330() > 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][0] = field_0x119c[i - 1][0];
        }
        field_0x119c[0][0] = 1;
    }
    if (fn_80247330() < 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][1] = field_0x119c[i - 1][1];
        }
        field_0x119c[0][1] = 1;
    }
    if (fn_8024712C() < 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][2] = field_0x119c[i - 1][2];
        }
        field_0x119c[0][2] = 1;
    }
    if (fn_8024712C() > 0.0f) {
        for (int i = 3; i > 0; i--) {
            field_0x119c[i][3] = field_0x119c[i - 1][3];
        }
        field_0x119c[0][3] = 1;
    }
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

// Counts a defeated enemy by kind into field_0xc1c.
void Player_804A6F38::fn_80270360(s32 serial) {
    if (field_0xc50 < 9999999) {
        field_0xc50++;
    }
    ActorBase_804A5770* actor = fn_801F856C()->fn_801F666C(serial);
    if (actor == NULL) {
        return;
    }
    s32 kind;
    switch (actor->mActorId) {
    case 'OKTA':
        kind = 0;
        break;
    case 'AMOS':
        kind = 1;
        break;
    case 'POOH':
        kind = 2;
        break;
    case 'BZBR':
        kind = 3;
        break;
    case 'DDRK':
        kind = 4;
        break;
    case 'DGTL':
        kind = 5;
        break;
    case 'HINO':
        kind = 6;
        break;
    case 'SLR1':
        kind = 7;
        break;
    case 'TKTA':
        kind = 8;
        break;
    case 'SLR2':
        kind = 7;
        break;
    case 'KETH':
        kind = 9;
        break;
    case 'BPOH':
        kind = 10;
        break;
    case 'MOZO':
        kind = 11;
        break;
    case 'Poon':
        kind = 12;
        break;
    default:
        kind = 13;
        break;
    }
    if (kind < 13 && field_0xc1c[kind] < 9999999) {
        field_0xc1c[kind]++;
    }
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

// FIXME: 98.26. Only the first lbz of 0xce8 is hoisted above the prologue's stw r31 / mr in the
// original: the method is const (rules card), which needs fn_802412CC, fn_802411D8, fn_80240808,
// fn_80247BD4 (tu_8023FBF0) and fn_80270A58 / fn_80270A8C const too.
bool Player_804A6F38::fn_80270ABC() {
    if (field_0xce8 == 1) {
        return true;
    }
    if (field_0xcee == 1) {
        return true;
    }
    if (field_0x1216) {
        return true;
    }
    if (field_0x1215) {
        return true;
    }
    if (field_0xcf0 == 1) {
        return true;
    }
    if (fn_802412CC() == true) {
        return true;
    }
    if (fn_802411D8() == true) {
        return true;
    }
    if (fn_80270A8C()) {
        return true;
    }
    if (fn_80270A58()) {
        return true;
    }
    if (fn_80240808() && fn_802486A0() == 5) {
        return true;
    }
    if (field_0x1240 == 1) {
        return true;
    }
    if (field_0x1240 == 0x19 || field_0x1244 == 0x19) {
        return true;
    }
    if (fn_80247BD4() > 0) {
        return true;
    }
    if (field_0x468 > 0) {
        return true;
    }
    return field_0x464 > 0;
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

// The view's box for `layer`: mVisibleBox below layer 8, else field_0xdc
// (as in tu_802684A4).
static inline JGeometry::TBox2f* layerBox(s32 layer) {
    GameView_804A9950* view = fn_802D6A9C();
    return layer < 8 ? &view->mVisibleBox : &view->field_0xdc;
}

// `box` made relative to the top-left corner of `layer`'s view box.
static inline void toView(JGeometry::TBox2f& box, s32 layer) {
    const JGeometry::TVec2f& origin = layerBox(layer)->i;
    JGeometry::TVec2f offset;
    offset.x = -origin.x;
    offset.y = -origin.y;
    box.addPos(offset);
}

// Whether two boxes overlap (the intersection is not empty); the result is
// the compare chain itself, not TBox::intersect's `? true : false`.
static inline bool overlaps(const JGeometry::TBox2f& a, const JGeometry::TBox2f& b) {
    JGeometry::TBox2f overlap(a);
    overlap.i.setMax(b.i);
    overlap.f.setMin(b.f);
    return overlap.f.x >= overlap.i.x && overlap.f.y >= overlap.i.y;
}

// `box` (on `layer`) against the player's box 0x4d0 (on its layer 0x3b4),
// both made relative to their layer's view corner (fn_802724E0, fn_80272E98).
static inline bool overlapsInView(const Player_804A6F38* player, const JGeometry::TBox2f& box, s32 layer) {
    JGeometry::TBox2f other(box);
    const JGeometry::TVec2f& origin = layerBox(layer)->i;
    JGeometry::TVec2f offset;
    offset.x = -origin.x;
    offset.y = -origin.y;
    other.addPos(offset);
    JGeometry::TBox2f own(player->field_0x4d0);
    const JGeometry::TVec2f& ownOrigin = layerBox(player->field_0x3b4)->i;
    JGeometry::TVec2f ownOffset;
    ownOffset.x = -ownOrigin.x;
    ownOffset.y = -ownOrigin.y;
    own.addPos(ownOffset);
    own.i.setMax(other.i);
    own.f.setMin(other.f);
    return own.f.x >= own.i.x && own.f.y >= own.i.y;
}

bool Player_804A6F38::fn_802724E0(const JGeometry::TBox2f* box, s32 layer) {
    if (!fn_802413F0()) {
        return false;
    }
    if (!(layer == field_0x3b4 || (field_0x3b4 < 8 && layer == 8))) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (!(fn_80270A8C() && fn_80270A04()) && !fn_80251BFC(0x19)) {
        return false;
    }
    return overlapsInView(this, *box, layer);
}

bool Player_804A6F38::fn_80272DCC(const JGeometry::TBox2f* box, s32 layer, s32 serial) {
    if (fn_80249068()) {
        return false;
    }
    for (int i = 0; i < 0x50; i++) {
        if (field_0xdb0[i] < 0) {
            field_0xdb0[i] = serial;
            field_0x1030[i] = layer;
            field_0x5fc[i].set(*box);
            return true;
        }
    }
    return false;
}

bool Player_804A6F38::fn_80272E98(const JGeometry::TBox2f* box, s32 layer) {
    if (layer != field_0x3b4) {
        return false;
    }
    if (mPos.z > 0.0f) {
        return false;
    }
    if (!fn_80245938()) {
        return false;
    }
    if (!fn_8025F2CC()) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    if (!fn_80251BFC(0x11)) {
        return false;
    }
    return overlapsInView(this, *box, layer);
}

bool Player_804A6F38::fn_802730D8(const JGeometry::TBox2f* box, s32 layer, bool flag, f32 height) {
    if (layer != field_0x3b4) {
        return false;
    }
    if (!flag && fn_80270A8C()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!viewLayerFlag(field_0x3b4) && mPos.z > height) {
        return false;
    }
    if (field_0xcee) {
        return false;
    }
    if (field_0x1216) {
        return false;
    }
    if (field_0x1215) {
        return false;
    }
    if (field_0xcf0) {
        return false;
    }
    if (field_0x1238 > 0) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    return overlaps(field_0x4c0, *box);
}

bool Player_804A6F38::fn_802732C4(const JGeometry::TBox2f* box, s32 layer) {
    if (fn_80270C88()) {
        return false;
    }
    if (field_0x42c) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    if (fn_80270A8C()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (!viewLayerFlag(field_0x3b4) && mPos.z > 0.0f) {
        return false;
    }
    if (field_0xcee) {
        return false;
    }
    if (field_0x1216) {
        return false;
    }
    if (field_0x1215) {
        return false;
    }
    if (field_0xcf0) {
        return false;
    }
    if (field_0x1238 > 0) {
        return false;
    }
    if (!fn_802413F0()) {
        return false;
    }
    return overlaps(field_0x4c0, *box);
}

// FIXME: 99.93. Code identical; its 0.25 / 0.5 literals are reloc-name until the earlier
// literal users (fn_802710C8 first) are written and the .sdata2 pool is in the original order.
bool Player_804A6F38::fn_802738F0(const JGeometry::TBox2f* box, JGeometry::TVec2f* out, bool flag, bool center) {
    if (!fn_802413F0()) {
        return false;
    }
    if (fn_80249068()) {
        return false;
    }
    if (fn_802413E8()) {
        return false;
    }
    if (field_0x458 == 1) {
        JGeometry::TBox2f a(field_0x4a0);
        toView(a, field_0x3b4);
        JGeometry::TBox2f b(field_0x4b0);
        toView(b, field_0x3b4);
        if (overlaps(a, *box)) {
            out->x = (a.i.x + a.f.x + b.i.x + b.f.x) / 4;
            out->y = (a.i.y + a.f.y + b.i.y + b.f.y) / 4;
            return true;
        }
        if (flag && overlaps(b, *box)) {
            if (center) {
                out->x = (a.i.x + a.f.x + b.i.x + b.f.x) / 4;
                out->y = (a.i.y + a.f.y + b.i.y + b.f.y) / 4;
                return true;
            }
            out->x = b.f.x + 0.5f * (b.i.x - b.f.x);
            out->y = b.f.y + 0.5f * (b.i.y - b.f.y);
            return true;
        }
    }
    return false;
}

u8 Player_804A6F38::fn_80273E7C(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f viewBox(*box);
    toView(viewBox, layer);
    bool hit = fn_802738F0(&viewBox, out, flag, false);
    if (hit == true) {
        out->add(layerBox(layer)->i);
    }
    return hit;
}

// FIXME: 94.20. Only the end differs: the original converts fn_80256F44's result to bool
// (clrlwi; neg; or; srwi), so that callee returns a u8 in the original; it is declared bool
// here because the generated forwarder fn_8023AAF8 (tu_8022AD64) passes it through.
bool Player_804A6F38::fn_80273C30(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (fn_80274238(box, layer, out, flag)) {
        return true;
    }
    if (fn_802576CC(box, layer, out)) {
        return true;
    }
    if (fn_802575B8(box, layer, out)) {
        return true;
    }
    if (fn_802574CC(box, layer, out)) {
        return true;
    }
    if (fn_802570BC(box, layer, out)) {
        return true;
    }
    return fn_80256F44(box, layer, out);
}

// FIXME: 94.87. Only the end differs: the original converts fn_80256F44's result to bool
// (clrlwi; neg; or; srwi), so that callee returns a u8 in the original; it is declared bool
// here because the generated forwarder fn_8023AAF8 (tu_8022AD64) passes it through.
bool Player_804A6F38::fn_80273D44(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* out, bool flag) {
    if (fn_80274238(box, layer, out, flag)) {
        return true;
    }
    if (fn_802576CC(box, layer, out)) {
        return true;
    }
    if (fn_802575B8(box, layer, out)) {
        return true;
    }
    if (fn_802574CC(box, layer, out)) {
        return true;
    }
    if (fn_802573E0(box, layer, out)) {
        return true;
    }
    if (fn_802570BC(box, layer, out)) {
        return true;
    }
    return fn_80256F44(box, layer, out);
}

// FIXME: 99.33. Only the return differs: the original truncates `hit` (clrlwi), i.e. returns
// a u8 as fn_80273E7C does; kept bool because the generated pass-through forwarder in
// tu_8022AD64 (0x8023BA4C) returns it as bool.
bool Player_804A6F38::fn_80274238(const JGeometry::TBox2f* box, s32 layer, JGeometry::TVec2f* pos, bool flag) {
    field_0x492 = 0;
    if (!fn_802413F0()) {
        return false;
    }
    if (layer != field_0x3b4) {
        return false;
    }
    JGeometry::TBox2f viewBox(*box);
    toView(viewBox, layer);
    bool hit = fn_802738F0(&viewBox, pos, flag, true);
    if (hit == true) {
        pos->add(layerBox(layer)->i);
    }
    if (!hit) {
        hit = fn_80273FB8(box, layer, pos);
    }
    return hit;
}

void Player_804A6F38::fn_80274728() {
    if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
        field_0x3fa++;
    } else if (field_0x3fa >= 14 && field_0x3fa < 37) {
        field_0x3fa += 2;
        if (field_0x3fa > 37) {
            field_0x3fa = 37;
        }
    } else {
        field_0x3fa++;
    }
    field_0x11c0 = 0;
    if (field_0x3fa < 6) {
        field_0x3fb = 8;
    } else if (field_0x3fa < 8) {
        field_0x3fb = 9;
    } else if (field_0x3fa < 9) {
        field_0x3fb = 10;
    } else if (field_0x3fa < 10) {
        field_0x3fb = 0;
    } else if (field_0x3fa < 31) {
        field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
        field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
    } else if (field_0x3fa < 33) {
        field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
        field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
    } else {
        lbl_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
        field_0x11b0++;
        s32 limit;
        if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
            limit = 6;
        } else {
            limit = 12;
        }
        if (field_0x11b0 < limit) {
            field_0x3fa = 10;
            field_0x3fb = 0;
        } else {
            field_0x3fb = 11;
            fn_80251AEC(12);
        }
        field_0x11c0 = 0;
    }
    switch (field_0x3dc) {
    case 0:
        break;
    case 1:
        field_0x11c0 += 0x8000;
        break;
    case 2:
        field_0x11c0 += 0x4000;
        break;
    case 3:
        field_0x11c0 -= 0x4000;
        break;
    }
    fn_802765C0();
}

void Player_804A6F38::fn_80274988() {
    field_0x3fa++;
    field_0x11c0 = 0;
    if (fn_802486A0() == 3 && !fn_8023434C(field_0x4)) {
        if (field_0x3fa < 3) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 8;
            field_0x11c0 = -0x7000;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 10;
            field_0x11c0 = -0x4000;
        } else if (field_0x3fa < 10) {
            field_0x3fb = 0;
            field_0x11c0 = -0x2000;
        } else if (field_0x3fa < 31) {
            field_0x3fb = ((field_0x3fa - 10) / 3 + 1) & 7;
            field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
        } else if (field_0x3fa < 34) {
            field_0x3fb = 11;
            field_0x11c0 = (field_0x3fa - 10) * 0xaaa;
        } else {
            field_0x3fb = 11;
            lbl_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
            fn_80251AEC(0);
        }
    } else {
        if (field_0x3fa < 3) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 7) {
            field_0x3fb = 8;
            field_0x11c0 = -0x7000;
        } else if (field_0x3fa < 8) {
            field_0x3fb = 9;
            field_0x11c0 = -0x6000;
        } else if (field_0x3fa < 9) {
            field_0x3fb = 10;
            field_0x11c0 = -0x4000;
        } else if (field_0x3fa < 10) {
            field_0x3fb = 0;
            field_0x11c0 = -0x2000;
        } else if (field_0x3fa < 24) {
            field_0x3fb = ((field_0x3fa - 10) / 2 + 1) & 7;
            field_0x11c0 = (field_0x3fa - 10) * 0x1000;
        } else if (field_0x3fa < 26) {
            field_0x3fb = 11;
            field_0x11c0 = (field_0x3fa - 10) * 0x1000;
        } else {
            field_0x3fb = 11;
            lbl_8053AC98->fn_802A20F0(&mPos, field_0x4, field_0x3b4, 0, 1);
            fn_80251AEC(0);
        }
    }
    switch (field_0x3dc) {
    case 0:
        break;
    case 1:
        field_0x11c0 += 0x8000;
        break;
    case 2:
        field_0x11c0 += 0x4000;
        break;
    case 3:
        field_0x11c0 -= 0x4000;
        break;
    }
    if (viewLayerFlag(field_0x3b4)) {
        field_0x11c0 = -field_0x11c0;
    }
    fn_802765C0();
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
