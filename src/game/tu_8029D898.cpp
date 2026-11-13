#include "game/GameAudio.h"
#include "game/ActorBase.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Effect.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/PlayerBit.h"

BgRewrite_804A7C98::BgRewrite_804A7C98() {
    gBgRewrite_8053AC98 = this;
    field_0x4 = new ActorSoundObj_804578F4(NULL, NULL, 8);
}

BgRewrite_804A7C98::~BgRewrite_804A7C98() {
    gBgRewrite_8053AC98 = NULL;
    delete field_0x4;
    field_0x4 = NULL;
}

void BgRewrite_804A7C98::fn_802A3D80() {
    field_0x8 = 10;
    field_0xc = 0;
    field_0x10 = 10;
    field_0x14 = 0;
    field_0x18 = 10;
    field_0x1c = 0;
}

extern const u16 lbl_8046C4A8[16]; // FIXME(shotgun): not defined in the source yet (unit .rodata)

// FIXME(shotgun): 82.8 %, scheduling; rodata table/strings addressed per object (unit .rodata not defined in source)
void BgRewrite_804A7C98::fn_802A3A0C() {
    field_0x8--;
    if (field_0x8 == 0) {
        field_0x8 = 10;
        field_0xc += 2;
        if (field_0xc >= 0x10) {
            field_0xc = 0;
        }
        if (getGameView_802D6A9C()->field_0x40 == 0x10) {
            u32 off = lbl_8046C4A8[field_0xc] + 0x4000;
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_b_sch.bin"), off, 0, 0x3800, 0x800);
        } else {
            u32 off = lbl_8046C4A8[field_0xc];
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_b_sch.bin"), off, 0, 0x3800, 0x800);
        }
    }
    field_0x18--;
    if (field_0x18 == 0) {
        field_0x18 = 10;
        field_0x1c++;
        if (field_0x1c >= 8) {
            field_0x1c = 0;
        }
        const char* name = getGameView_802D6A9C()->fn_802D9C4C();
        char c = *name;
        if (c == 'l') {
            u32 off = lbl_8046C4A8[field_0x1c] + 0x2000;
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_sch.bin"), off, 0, 0x5800, 0x400);
            field_0x18 = 0x14;
        } else if (c == 's' || c == 'w') {
            u32 off = lbl_8046C4A8[field_0x1c] + 0x6000;
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_sch.bin"), off, 0, 0x1c00, 0x400);
        } else if (c == 'k' || c == 'v') {
            if (field_0x1c >= 4) {
                field_0x1c = 0;
            }
            u32 off = lbl_8046C4A8[field_0x1c] + 0x1000;
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_sch.bin"), off, 0, 0x5c00, 0x400);
        } else if (getGameView_802D6A9C()->field_0x40 == 0x1b || *name == 'y') {
            if (field_0x1c >= 4) {
                field_0x1c = 0;
            }
            u32 off = lbl_8046C4A8[field_0x1c * 2] + 0x4000;
            gGameRoot2_8053AB10->field_0x20->fn_802FA160(
                gGameRoot_8053AAF8->field_0xa4->getResource('SCH ', "bg_anim_sch.bin"), off, 0, 0x5800, 0x800);
        }
    }
}

// FIXME: 98.8. Only the clrlwi r6, r30, 16 on tile before fn_802F9974 is missing
// (ours passes the u16 parameter with mr); an s32 / s16 tile matches here but
// drops the u16-passing callers (tu_8012DAC0, act_shutter, act_burning_block).
void BgRewrite_804A7C98::fn_802A3948(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer & 0xFE);
    s32 cell = field_0x20;
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, field_0x20);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
    }
}

// FIXME: 98.8. As fn_802A3948: only the clrlwi r6, r30, 16 on tile is missing.
void BgRewrite_804A7C98::fn_802A3884(const JGeometry::TVec3f* pos, s32 player, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer & 0xFE);
    s32 cell = field_0x20;
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, field_0x20);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, false);
    }
}

// FIXME: 97.7. As fn_802A3948: only the clrlwi r6, r31, 16 on tile is missing.
void BgRewrite_804A7C98::fn_802A381C(const JGeometry::TVec3f* pos, s32 layer, u16 tile) {
    JGeometry::TVec2f p(pos->x, pos->y);
    s32 cell = fn_80226BC4(&p, layer & 0xFE);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
}

// FIXME(shotgun): 99.1 %, scheduling+regalloc
bool BgRewrite_804A7C98::fn_802A3164(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    bool result = true;
    if (field_0x24 >= 0x40 && field_0x24 <= 0x44) {
        if (field_0x24 == 0x41) {
            gBgRewrite_8053AC98->fn_802A3884(pos, 0, layer, 0x12);
            s32 count = getActorManager_801F856C()->mActorEnd;
            for (s32 i = 0; i < count; i++) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
                if (actor != NULL && actor->mActorId == 'TBO2' && layer == actor->field_0x4) {
                    JGeometry::TVec3f actorPos = actor->mPos;
                    JGeometry::TVec2f q(actorPos.x, actorPos.y);
                    if (field_0x20 == fn_80226BC4(&q, layer)) {
                        actor->fn_801F3668();
                        break;
                    }
                }
            }
        } else if (field_0x24 == 0x43) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x50);
        } else {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x11);
        }
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUBO', layer, pos, field_0x24 - 0x40, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->vfunc_0x24();
        }
    } else if (field_0x24 >= 0x45 && field_0x24 <= 0x49) {
        if (field_0x24 == 0x46) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x14);
        } else if (field_0x24 == 0x48) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x52);
        } else {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x15);
        }
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUBO', layer, pos, field_0x24 - 0x45, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->vfunc_0x24();
        }
    } else if (field_0x24 == 0x4E) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x11);
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUBO', layer, pos, 0x80, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->vfunc_0x24();
        }
    } else if (field_0x24 == 0x4F) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x11);
        s32 serial = getActorManager_801F856C()->spawnActor_801F9484('TUBO', layer, pos, 0x100, -1, -1);
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->vfunc_0x24();
        }
    } else {
        result = false;
    }
    return result;
}

// FIXME(shotgun): 99.8 %, regalloc-only
bool BgRewrite_804A7C98::fn_802A1D44(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    bool result = true;
    if (field_0x24 == 0x76) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x77);
        fn_80230FD0(player, 0x20026);
    } else if (field_0x24 == 0x3A) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x3B);
        fn_80230FD0(player, 0x20026);
    } else if (field_0x24 == 0x3F) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x3E);
        fn_80230FD0(player, 0x20026);
    } else if (field_0x24 == 0x90) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x91);
        fn_80230FD0(player, 0x50013);
    } else if (field_0x24 == 0x91 || field_0x24 == 0x22E) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x92);
        fn_80230FD0(player, 0x50012);
    } else if (!fn_802A3164(pos, player, layer)) {
        result = false;
    }
    return result;
}

// FIXME(shotgun): 99.4 %, 1 fewer insn (literal pool order)
s32 BgRewrite_804A7C98::fn_802A1844(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    if ((layer == 0 && field_0x24 >= 0x1B0 && field_0x24 <= 0x1CF) || field_0x24 == 0x231 || field_0x24 == 0x8A ||
        field_0x24 == 0x79 || field_0x24 == 0x7B || field_0x24 == 0x7D) {
        u16 tile = field_0x24;
        if (tile == 0x231) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x7A);
        } else if ((tile >= 0x1B5 && tile <= 0x1BA) || (u16)(tile - 0x1BC) <= 1 || tile == 0x8A ||
                   (tile >= 0x1C5 && tile <= 0x1CD)) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x83);
        } else if (tile == 0x79 || tile == 0x7B || tile == 0x7D) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x7C);
        } else {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x7C);
        }
        if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f) {
            getActorManager_801F856C()->spawnActor_801F9484('HART', layer, pos, 0, -1, -1);
        }
        if (getGameView_802D6A9C()->field_0x40 == 0x14) {
            return 3;
        }
        if (getGameView_802D6A9C()->field_0x40 == 0x11 || getGameView_802D6A9C()->field_0x40 == 0x18) {
            return 4;
        }
        tile = field_0x24;
        if ((tile >= 0x1B5 && tile <= 0x1BA) || (u16)(tile - 0x1BC) <= 1 || (tile >= 0x1C5 && tile <= 0x1CD)) {
            return 2;
        }
        return 1;
    }
    if (getGameView_802D6A9C()->field_0x4 == 4 && getGameView_802D6A9C()->field_0x8 == 3) {
        return 8;
    }
    if ((getGameView_802D6A9C()->field_0x4 == 1 && getGameView_802D6A9C()->field_0x8 == 1 && field_0x24 == 0x270) ||
        field_0x24 == 0x271 || field_0x24 == 0x361) {
        return 8;
    }
    u16 tile = field_0x24;
    if ((u16)(tile - 0x11) <= 2 || tile == 0x38 || tile == 0x3B || (u16)(tile - 0x50) <= 1 || tile == 0x78 ||
        tile == 0x8F || (u16)(tile - 0x9D) <= 1 || tile == 0x1AE || tile == 0x1AF) {
        return 8;
    }
    if (tile == 0x7C || tile == 0x83) {
        return 9;
    }
    return 0;
}

// FIXME(shotgun): 99.5 %, regalloc-only
bool BgRewrite_804A7C98::fn_802A1688(const JGeometry::TVec3f* pos, s32 layer, u32 tag) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    if (field_0x24 == 0x81) {
        if (tag != 'SBAL') {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x9D);
            if (layer >= 8) {
                getActorManager_801F856C()->spawnActor_801F9484('KMRI', layer, pos, 0, -1, -1);
            } else {
                spawnEmitter_8013CC50(layer, pos, 0xF9, 1, 0, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(layer, pos, 0xFA, 1, 0, NULL, NULL, true, 1.0f);
            }
        }
        field_0x4->startSound(0x50045);
    }
    return fn_802A3164(pos, -1, layer);
}

bool BgRewrite_804A7C98::fn_802A1554(const JGeometry::TVec3f* pos) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, 0);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x20);
    u16 tile = field_0x24;
    if (lbl_8046CB88[tile] == 0) {
        return true;
    }
    if (tile == 0x75 || tile == 0x68 || tile == 0x69 || tile == 0x6A || tile == 0x6B || tile == 0x82 ||
        tile == 0x5F || tile == 0x70 || tile == 0x71 || tile == 0x72 || tile == 0x40 || tile == 0x41 ||
        tile == 0x42 || tile == 0x43 || tile == 0x44 || tile == 0xE5 || tile == 0x58) {
        return true;
    }
    s32 code = fn_8022461C(pos, 0);
    if (code == 0xE || code == 3 || (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0)) {
        return true;
    }
    return false;
}

bool BgRewrite_804A7C98::fn_802A1490(const JGeometry::TVec3f* pos) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, 0);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x20);
    if (lbl_8046CB88[field_0x24] == 0) {
        return true;
    }
    s32 code = fn_8022461C(pos, 0);
    if (code == 0x22 && getGameView_802D6A9C()->field_0x170 == 0) {
        return true;
    }
    return false;
}

// FIXME(shotgun): 99.0 %, 3 opcodes differ (literal pool order)
void BgRewrite_804A7C98::fn_802A1050(const JGeometry::TVec3f* pos) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, 0);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(0, field_0x20);
    u16 tile = field_0x24;
    if (tile == 0x69 || tile == 0x68 || tile == 0x75 || (u16)(tile - 0x6A) <= 1 || tile == 0x82 || tile == 0x5F ||
        (u16)(tile - 0x70) <= 3 || (u16)(tile - 0x40) <= 4 || tile == 0xE5) {
        switch (tile) {
        case 0x72:
        case 0x73:
        case 0x82: {
            if (tile == 0x72 || tile == 0x73) {
                gBgRewrite_8053AC98->fn_802A3948(pos, 0, 0, 0x7C);
            } else {
                gBgRewrite_8053AC98->fn_802A3948(pos, 0, 0, 0x83);
            }
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('STON', 0, pos, 0, -1, -1);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->vfunc_0x24();
            }
            break;
        }
        case 0x5F: {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, 0, 0x83);
            s32 serial = getActorManager_801F856C()->spawnActor_801F9484('YASI', 0, pos, 0, -1, -1);
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (actor != NULL) {
                actor->vfunc_0x24();
            }
            break;
        }
        case 0xE5:
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, 0, 0x9D);
            spawnEmitter_8013CC50(0, pos, 0xF9, 1, 0, NULL, NULL, true, 1.0f);
            spawnEmitter_8013CC50(0, pos, 0xFA, 1, 0, NULL, NULL, true, 1.0f);
            field_0x4->startSound(0x50045);
            break;
        }
    }
}

// FIXME(shotgun): 97.8 %, 2 more insns (register copies)
s32 BgRewrite_804A7C98::fn_802A02A8(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    s32 kind = -1;
    s32 code = 0x79;
    switch (field_0x24) {
    case 0x69:
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x7A);
        fn_80230FD0(player, 0x20007);
        gAudioMgr_8053AF80->fn_80458880(3, -1);
        return 0;
    case 0x16:
    case 0x17:
        if (fn_802A0800(player, layer) == true) {
            return 0x25;
        }
        return -1;
    case 0x68:
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x12);
        fn_80230FD0(player, 0x20007);
        return 0;
    case 0x70:
        kind = 0;
        break;
    case 0x75:
        kind = 1;
        break;
    case 0x6A:
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x50);
        fn_80230FD0(player, 0x20007);
        return 0;
    case 0x6B:
        kind = 3;
        break;
    case 0x72:
    case 0x82:
        kind = 5;
        if (field_0x24 == 0x72) {
            code = 0x7C;
        } else {
            code = 0x83;
        }
        break;
    case 0x5F:
        kind = 0x27;
        code = 0x83;
        break;
    case 0x71:
        kind = 7;
        code = 0x7B;
        break;
    case 0x40:
        kind = 0x18;
        code = 0x11;
        break;
    case 0x41:
        kind = 0x19;
        code = 0x12;
        break;
    case 0x42:
        kind = 0x1A;
        code = 0x11;
        break;
    case 0x43:
        kind = 0x1B;
        code = 0x50;
        break;
    case 0x44:
        kind = 0x1C;
        code = 0x11;
        break;
    case 0x45:
        kind = 0x1D;
        code = 0x15;
        break;
    case 0x46:
        kind = 0x1E;
        code = 0x14;
        break;
    case 0x47:
        kind = 0x1F;
        code = 0x15;
        break;
    case 0x48:
        kind = 0x20;
        code = 0x52;
        break;
    case 0x49:
        kind = 0x21;
        code = 0x15;
        break;
    case 0x4E:
        kind = 0x22;
        code = 0x11;
        break;
    case 0x4F:
        kind = 0x23;
        code = 0x11;
        break;
    }
    if (kind == -1 || kind == 0x25 || kind == 0xC) {
        return kind;
    }
    if (kind == 0x19) {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, code);
        fn_80230FD0(player, 0x20007);
        s32 count = getActorManager_801F856C()->mActorEnd;
        for (s32 i = 0; i < count; i++) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->mActors[i];
            if (actor != NULL && actor->mActorId == 'TBO2' && layer == actor->field_0x4) {
                JGeometry::TVec3f actorPos = actor->mPos;
                JGeometry::TVec2f q(actorPos.x, actorPos.y);
                if (field_0x20 == fn_80226BC4(&q, layer)) {
                    actor->fn_801F3668();
                    break;
                }
            }
        }
    } else {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, code);
        fn_80230FD0(player, 0x20007);
    }
    return kind;
}

bool BgRewrite_804A7C98::fn_802A01F0(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    bool result = false;
    switch (field_0x24) {
    case 0x16:
    case 0x17:
        result = fn_802A0800(player, layer);
        break;
    case 0x71:
        result = true;
        break;
    }
    return result;
}

extern const s32 lbl_8046C4C8[16]; // FIXME(shotgun): not defined in the source yet (unit .rodata)

// FIXME(shotgun): 76.8 %, regalloc/scheduling (cell kept in its own register, index a copy)
void BgRewrite_804A7C98::fn_8029EC84(s32 cell, s32 layer, s32 layerOffset, u16 tileA, u16 tileB) {
    s32 layerPlus = layer + layerOffset;
    s32 index = cell;
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layerPlus, index, tileB);
    s32 left = index - 0x10;
    if ((index & 0x100) != (left & 0x100)) {
        index -= 0x110;
    } else {
        index -= 0x10;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layerPlus, index, tileA);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, cell);
    s32 value = lbl_8046C4C8[(u32)slot >> 2];
    s32 tile = tileA;
    switch (tile) {
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
        getGameView_802D6A9C()->fn_802D7BE8(layer, value, tile - 0x16);
        break;
    case 0xA0:
    case 0xA1:
    case 0xA2:
    case 0xA3:
    case 0xFFF:
        getGameView_802D6A9C()->fn_802D7BE8(layer, value, tile - 0x96);
        break;
    case 0:
    default:
        getGameView_802D6A9C()->fn_802D7BE8(layer, value, 1);
        break;
    }
}

// Pos offsets of the four directions.
extern const f32 lbl_8046C508[4][2]; // FIXME(shotgun): not defined in the source yet (unit .rodata)

// FIXME(shotgun): 99.1 %, 2 opcodes differ
void BgRewrite_804A7C98::fn_8029E9EC(const JGeometry::TVec3f* pos, s32 layer, u16 tile, s32 arg4, u8 dir) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    if (arg4 == 0) {
        if ((tile >= 0x21 && tile <= 0x25) || tile == 0x26 || tile == 0x27 || tile == 0x36 || tile == 0x37) {
            JGeometry::TVec2f q(pos->x + lbl_8046C508[dir][0], pos->y + lbl_8046C508[dir][1]);
            field_0x20 = fn_80226BC4(&q, layer);
            field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, field_0x24);
        } else {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, tile);
        }
    } else {
        gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, tile);
    }
}

// FIXME: 99.1. Register allocation only: the original keeps gBgRewrite_8053AC98 in
// r31 and the cell / slot in r30 (ours swaps them).
void BgRewrite_804A7C98::fn_8029E928(const JGeometry::TVec3f* pos, s32 layer, s32 tile) {
    gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, tile);
}

u8 BgRewrite_804A7C98::fn_8029E888(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
    return field_0x24 == 0x30;
}

// FIXME(shotgun): 92.8 %, missing/extra (the player-mask test: the original keeps the bool and compares it with 1)
bool BgRewrite_804A7C98::fn_8029E508(const JGeometry::TVec3f* pos, s32 player, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    JGeometry::TVec3f playerPos = *getPlayerPos_8023E724(player);
    if (field_0x24 == 0x19C || field_0x24 == 0x19D) {
        s32 blocked = false;
        if (gGameRoot_8053AAF8->field_0x7e == 1) {
            for (s32 i = 0; i < 4; i++) {
                if (isPlayerBitSet(gGameRoot_8053AAF8, i) == true && fn_8023F264(i) == true && fn_80303750(i) == 1 && fn_8022C8EC(i) == 1) {
                    blocked = true;
                }
            }
        } else if (fn_80303750(0) == 1 && fn_8022C8EC(0) == 1) {
            blocked = true;
        }
        if (!blocked) {
            if (field_0x24 == 0x19D) {
                s32 cell = field_0x20;
                getGameView_802D6A9C()->fn_802D782C(1);
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, 0x19F);
                s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, cell);
                if (slot != -1) {
                    getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
                }
                JGeometry::TVec3f left(pos->x - 16.0f, pos->y, 0.0f);
                gBgRewrite_8053AC98->fn_802A3948(&left, 0, layer, 0x19E);
            } else {
                s32 cell = field_0x20;
                getGameView_802D6A9C()->fn_802D782C(1);
                gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, 0x19E);
                s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, cell);
                if (slot != -1) {
                    getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
                }
                JGeometry::TVec3f right(16.0f + pos->x, pos->y, 0.0f);
                gBgRewrite_8053AC98->fn_802A3948(&right, 0, layer, 0x19F);
            }
            fn_80230FD0(player, 0x30007);
        } else {
            fn_80230FD0(player, 0x30017);
        }
        return true;
    }
    return false;
}

// FIXME(shotgun): 97.2 %, 2 fewer insns (register copies of layer)
bool BgRewrite_804A7C98::fn_8029E248(const JGeometry::TVec3f* pos, s32 layer) {
    JGeometry::TVec2f p(pos->x, pos->y);
    field_0x20 = fn_80226BC4(&p, layer);
    field_0x24 = gGameRoot2_8053AB10->field_0x20->fn_802F9908(layer, field_0x20);
    bool result = false;
    if (field_0x24 == 0x19C || field_0x24 == 0x19D) {
        if (field_0x24 == 0x19D) {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x19F);
            JGeometry::TVec3f left(pos->x - 16.0f, pos->y, pos->z);
            gBgRewrite_8053AC98->fn_802A3948(&left, 0, layer, 0x19E);
        } else {
            gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, 0x19E);
            JGeometry::TVec3f right(pos->x + 16.0f, pos->y, pos->z);
            gBgRewrite_8053AC98->fn_802A3948(&right, 0, layer, 0x19F);
        }
        fn_80230FD0(0, 0x30007);
        result = true;
    }
    return result;
}

// FIXME(shotgun): 96.7 %, 4 opcodes differ
void BgRewrite_804A7C98::fn_8029E050(const JGeometry::TVec3f* pos, s32 layer, u16 a, u16 b, u16 c, u16 d) {
    gBgRewrite_8053AC98->fn_802A3948(pos, 0, layer, a);
    s32 cell = field_0x20 + 1;
    if ((cell & 0xF) == 0) {
        cell += 0xF0;
    }
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
    s32 slot = getGameView_802D6A9C()->fn_802D4C10(layer, cell);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
    }
    cell = field_0x20;
    s32 below = cell + 0x10;
    if ((cell & 0x100) != (below & 0x100)) {
        below = cell + 0x110;
    }
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, below, c);
    slot = getGameView_802D6A9C()->fn_802D4C10(layer, below);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
    }
    cell = below + 1;
    if ((cell & 0xF) == 0) {
        cell += 0xF0;
    }
    getGameView_802D6A9C()->fn_802D782C(1);
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, d);
    slot = getGameView_802D6A9C()->fn_802D4C10(layer, cell);
    if (slot != -1) {
        getGameView_802D6A9C()->fn_802D7D00(layer, slot, true);
    }
}

// FIXME: 93.4. The original copies cell into r31 before the first write and steps
// that copy in place (addi r31, r31, 1); ours computes the right neighbour into
// r5 after the call (2 instructions fewer, registers r26-r31 shifted).
void BgRewrite_804A7C98::fn_8029DF80(s32 cell, s32 layer, s32 a, s32 b, s32 c, s32 d) {
    s32 index = cell;
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, a);
    index++;
    if ((index & 0xF) == 0) {
        index += 0xF0;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, b);
    index = cell + 0x10;
    if ((cell & 0x100) != (index & 0x100)) {
        index = cell + 0x110;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, index, c);
    s32 next = index + 1;
    if ((next & 0xF) == 0) {
        next += 0xF0;
    }
    gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, next, d);
}

// FIXME: 92.4. Register allocation and placement only: the original saves b in
// r30 and cell in r29 up front and copies layer into r31 inside each branch
// (mr r31, r4 twice); ours saves layer in r29 before the branch.
void BgRewrite_804A7C98::fn_8029DEBC(s32 layer, s32 cell, s32 vertical, s32 a, s32 b) {
    if (vertical == 0) {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
    } else {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        s32 below = cell + 0x10;
        if ((cell & 0x100) != (below & 0x100)) {
            below = cell + 0x110;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, below, b);
    }
}

// FIXME(shotgun): 90.3 %, missing/extra (register copies of layer / cell)
void BgRewrite_804A7C98::fn_8029DD50(s32 layer, s32 cell, s32 vertical, u16 a, u16 b, u16 c, u16 d) {
    if (vertical == 0) {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, c);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, d);
    } else {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        s32 below0 = cell + 0x10;
        if ((cell & 0x100) != (below0 & 0x100)) {
            below0 = cell + 0x110;
        }
        cell = below0;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
        s32 below1 = cell + 0x10;
        if ((cell & 0x100) != (below1 & 0x100)) {
            below1 = cell + 0x110;
        }
        cell = below1;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, c);
        s32 below2 = cell + 0x10;
        if ((cell & 0x100) != (below2 & 0x100)) {
            below2 = cell + 0x110;
        }
        cell = below2;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, d);
    }
}

// FIXME(shotgun): 92.3 %, missing/extra (register copies of layer / cell)
void BgRewrite_804A7C98::fn_8029DB2C(s32 layer, s32 cell, s32 vertical, u16 a, u16 b, u16 c, u16 d, u16 e, u16 f) {
    if (vertical == 0) {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, c);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, d);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, e);
        cell++;
        if ((cell & 0xF) == 0) {
            cell += 0xF0;
        }
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, f);
    } else {
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, a);
        s32 below0 = cell + 0x10;
        if ((cell & 0x100) != (below0 & 0x100)) {
            below0 = cell + 0x110;
        }
        cell = below0;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, b);
        s32 below1 = cell + 0x10;
        if ((cell & 0x100) != (below1 & 0x100)) {
            below1 = cell + 0x110;
        }
        cell = below1;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, c);
        s32 below2 = cell + 0x10;
        if ((cell & 0x100) != (below2 & 0x100)) {
            below2 = cell + 0x110;
        }
        cell = below2;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, d);
        s32 below3 = cell + 0x10;
        if ((cell & 0x100) != (below3 & 0x100)) {
            below3 = cell + 0x110;
        }
        cell = below3;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, e);
        s32 below4 = cell + 0x10;
        if ((cell & 0x100) != (below4 & 0x100)) {
            below4 = cell + 0x110;
        }
        cell = below4;
        gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, f);
    }
}

// The 4 x 4 tile blocks (patterns of 20 u16: four rows of four tiles and padding).
extern const u16 lbl_8046C5A0[4][20]; // FIXME(shotgun): not defined in the source yet (unit .rodata)
extern const u16 lbl_8046C528[3][20]; // FIXME(shotgun): not defined in the source yet (unit .rodata)

// FIXME(shotgun): 91.9 %, regalloc (cell kept in other registers); pattern tables are externs
void BgRewrite_804A7C98::fn_8029D8F0(const JGeometry::TVec3f* pos, s32 layer, s32 layerOffset, s32 pattern) {
    GameView_804A9950* view = getGameView_802D6A9C();
    const JGeometry::TBox2f* src = (layer & 0xFE) < 8 ? &view->mVisibleBox : &view->field_0xdc;
    JGeometry::TBox2f box;
    box = *src;
    s32 x = (s32)(pos->x - box.i.x);
    s32 y = (s32)(pos->y - box.i.y);
    if (x < 0) {
        x = 0;
    } else if (x >= 0x200) {
        x = 0x1FF;
    }
    if (y < 0) {
        y = 0;
    } else if (y >= 0x200) {
        y = 0x1FF;
    }
    x -= 0x10;
    y -= 0x30;
    s32 start = (x & 0x100) | ((y & 0x100) << 1) | (y & 0xF0) | ((x >> 4) & 0xF);
    s32 layerPlus = layer + layerOffset;
    s32 row = start;
    for (int r = 0; r < 4; r++) {
        s32 cell = row;
        const u16* p = &lbl_8046C5A0[pattern][r * 4];
        for (int i = 0; i < 4; i++) {
            u16 tile = *p++;
            getGameView_802D6A9C()->fn_802D782C(1);
            gGameRoot2_8053AB10->field_0x20->fn_802F9974(layerPlus, cell, tile);
            cell++;
            if ((cell & 0xF) == 0) {
                cell += 0xF0;
            }
        }
        s32 next = row + 0x10;
        if ((row & 0x100) != (next & 0x100)) {
            next = row + 0x110;
        }
        row = next;
    }
    s32 row2 = start + 0x10;
    if ((start & 0x100) != (row2 & 0x100)) {
        row2 = start + 0x110;
    }
    for (int r = 0; r < 4; r++) {
        s32 cell = row2;
        const u16* p = &lbl_8046C528[pattern][r * 4];
        for (int i = 0; i < 4; i++) {
            u16 tile = *p++;
            getGameView_802D6A9C()->fn_802D782C(1);
            gGameRoot2_8053AB10->field_0x20->fn_802F9974(layer, cell, tile);
            cell++;
            if ((cell & 0xF) == 0) {
                cell += 0xF0;
            }
        }
        s32 next = row2 + 0x10;
        if ((row2 & 0x100) != (next & 0x100)) {
            next = row2 + 0x110;
        }
        row2 = next;
    }
}

void BgRewrite_804A7C98::fn_8029D898(s32 value) {
    s32 saved = getGameView_802D6A9C()->field_0x34;
    getGameView_802D6A9C()->fn_802D7970(value);
    gGameRoot_8053AAF8->fn_80138B7C();
    getGameView_802D6A9C()->fn_802D7970(saved);
}
