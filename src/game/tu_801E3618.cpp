#include "game/GameAudio.h"
#include "game/act/act_vaati_boss.h"
#include "game/Player.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/EnemyBase.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"


// FIXME(shotgun): 80.00 %, different + reloc-name
void VaatiBoss_804A4958::fn_801E4790(bool big) {
    f32 s = lbl_8053F0BC;
    if (big) {
        s = 1.0f;
    }
    mBox.i.x = -16.0f * s;
    mBox.i.y = -26.0f * s;
    mBox.f.x = 16.0f * s;
    mBox.f.y = 4.0f * s;
    mBox2 = mBox;
}

// FIXME(shotgun): 98.93 %, reloc-name
void VaatiBoss_804A4958::fn_801E4758() {
    field_0x80.i.x = -16.0f * lbl_8053F0BC;
    field_0x80.i.y = -26.0f * lbl_8053F0BC;
    field_0x80.f.x = 16.0f * lbl_8053F0BC;
    field_0x80.f.y = 4.0f * lbl_8053F0BC;
}

// FIXME(shotgun): 44.77 %, different
void VaatiBoss_804A4958::fn_801E4618(JGeometry::TBox2f* out, bool region) {
    if (region) {
        out->i.x = field_0x5e0;
        out->i.y = field_0x5e4;
        out->f.x = field_0x5e0 + 16.0f * (f32)field_0x5e8;
        out->f.y = field_0x5e4 + 16.0f * (f32)field_0x5e9;
    } else {
        GameView_804A9950* view = getGameView_802D6A9C();
        JGeometry::TBox2f* box = field_0x5d8 < 8 ? &view->mVisibleBox : &view->field_0xdc;
        if (mFlags & 0x2000) {
            out->i.x = 160.0f + box->i.x;
            out->i.y = 80.0f + box->i.y;
            out->f.x = box->f.x - 160.0f;
            out->f.y = box->f.y - 112.0f;
        } else {
            out->i.x = 80.0f + box->i.x;
            out->i.y = 128.0f + box->i.y;
            out->f.x = box->f.x - 80.0f;
            out->f.y = box->f.y - 96.0f;
        }
    }
}

// FIXME(shotgun): 78.23 %, missing/extra + reloc-name
bool VaatiBoss_804A4958::fn_801E44CC(const JGeometry::TBox2f* area, JGeometry::TVec3f* pos, JGeometry::TVec3f* outDir) {
    JGeometry::TBox2f box;
    box.i.x = field_0x80.i.x + pos->x;
    box.i.y = field_0x80.i.y + pos->y;
    box.f.x = field_0x80.f.x + pos->x;
    box.f.y = field_0x80.f.y + pos->y;
    JGeometry::TVec2f push;
    bool moved = fn_801CC29C(area, &box, &push);
    pos->x += push.x;
    pos->y += push.y;
    if (outDir != NULL) {
        push.normalize();
        outDir->x = push.x;
        outDir->y = push.y;
        outDir->z = 0.0f;
    }
    return moved;
}

// Vaati's per-frame player check: when the boss is in one of its states and
// a player is in its box, the attack states answer with a sound or effect.
bool VaatiBoss_804A4958::fn_801E4348() {
    bool result = false;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        stateOk = false;
        break;
    }
    if (stateOk && (mFlags & 0x2000)) {
        JGeometry::TBox2f box;
        fn_801F317C(&box);
        JGeometry::TVec2f xy;
        s32 player = fn_8023AE34(&box, field_0x4, &xy);
        if (player >= 0) {
            switch (mState) {
            case 0xc:
            case 0xd:
            case 0x13:
                if (fn_801CC730(player, NULL, NULL)) {
                    result = true;
                }
                break;
            default:
                if (fn_801CC820(player, NULL)) {
                    JGeometry::TVec3f pos(xy.x, xy.y + mPos.z, mPos.z);
                    fn_801CB688(field_0x4, &pos, 1, 1);
                    startSound_801F0E34(0x5003C, 0);
                    switch (mState) {
                    case 0xa:
                    case 0xe:
                    case 0xf:
                    case 0x10:
                    case 0x11:
                    case 0x12:
                    case 0x14:
                        fn_801D5D84(0xb);
                        break;
                    }
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}

// FIXME(shotgun): 70.00 %, missing/extra + reloc-name
u8 VaatiBoss_804A4958::fn_801E4094() {
    u8 result = 0;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && !(mFlags & 0x2000) && mBox.f.x - mBox.i.x > 0.0f && mBox.f.y - mBox.i.y > 0.0f) {
        JGeometry::TBox2f saved = mBox;
        mBox.i.x *= 1.5f;
        mBox.i.y *= 1.5f;
        mBox.f.x *= 1.5f;
        mBox.f.y *= 1.5f;
        s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 48.0f);
        mBox = saved;
        if (serial >= 0 && serial != mSerial) {
            ActorBase_804A5770* bomb = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (bomb != NULL && bomb->mActorId == 'HBMB' && bomb->field_0x94 == 1) {
                JGeometry::TVec3f d = mPos - bomb->field_0x3c;
                result = fn_801F37AC(0, &d, 2, bomb->field_0x168, bomb->field_0x110);
                if (result) {
                    bomb->field_0x164 = 1;
                    field_0xf8 = 0;
                    field_0x3c.set(0.0f, 0.0f, 0.0f);
                    field_0xd8 = 0xB4;
                    field_0xdc = 0xB4;
                    mSpeed = 0.0f;
                    fn_801D63D0();
                    startSound_801F0E34(0x40005, 0);
                    fn_801E4D54();
                    field_0x448.fn_80217A74();
                    JGeometry::TVec2f center;
                    fn_801F3128(&center);
                    JGeometry::TVec2f shake(center.x - bomb->mPos.x, center.y - (bomb->mPos.y - bomb->mPos.z));
                    field_0x52c.fn_801D6D38(&shake);
                    field_0x5cd++;
                    if (field_0x5cd >= field_0x5cc) {
                        fn_801D5D84(6);
                    }
                }
            }
        }
    }
    return result;
}

u8 VaatiBoss_804A4958::fn_801E3A4C() {
    u8 result = 0;
    bool stateOk = true;
    switch (mState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
        stateOk = false;
        break;
    }
    if (stateOk && (mFlags & 0x2000) && fn_801CD2F8(field_0x4) && mBox.f.x - mBox.i.x > 0.0f &&
        mBox.f.y - mBox.i.y > 0.0f) {
        result = fn_801F2B7C(field_0x114, -1, 1);
    }
    return result;
}

// FIXME(shotgun): 75.59 %, missing/extra
bool VaatiBoss_804A4958::fn_801E3618(s32 layer, const JGeometry::TVec2f* pos) {
    bool outside = false;
    bool inside = true;
    f32 x0, y0, x1, y1;
    if (fn_801CD2F8(layer)) {
        x0 = field_0x5e0;
        y0 = field_0x5e4;
        x1 = x0 + 16.0f * (f32)field_0x5e8;
        y1 = y0 + 16.0f * (f32)field_0x5e9;
    } else {
        GameView_804A9950* view = getGameView_802D6A9C();
        JGeometry::TBox2f* box = field_0x5d8 < 8 ? &view->mVisibleBox : &view->field_0xdc;
        if (mFlags & 0x2000) {
            x0 = 160.0f + box->i.x;
            y0 = 80.0f + box->i.y;
            x1 = box->f.x - 160.0f;
            y1 = box->f.y - 112.0f;
        } else {
            x0 = 80.0f + box->i.x;
            y0 = 128.0f + box->i.y;
            x1 = box->f.x - 80.0f;
            y1 = box->f.y - 96.0f;
        }
    }
    if (pos->x < x0 || pos->y < y0 || pos->x >= x1 || pos->y >= y1) {
        inside = false;
    }
    if (!inside) {
        outside = true;
    }
    return outside;
}
