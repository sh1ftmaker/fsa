#include "game/GameAudio.h"
#include "game/Message.h"
#include "game/GameGlobals.h"
#include "game/GameMode.h"
#include "game/ActorManager.h"
#include "game/ActorId.h"
#include "game/tu_803944C8.h"
#include "game/Camera.h"
#include "JSystem/JMessage/TControl.h"
#include "game/act/act_treasure_chest.h"

extern u8 lbl_8053AD04[4];

MessageWindow_804AF490::MessageWindow_804AF490(s32 index) {
    mIndex = index;
    field_0x48 = new cls_804AF5A0(this, mIndex, "gc_four_swords_text_e.bmg");
    field_0x4c = new cls_804AF5A0(this, mIndex, "gc_four_swords_text_ek.bmg");
    field_0x44 = 0;
    field_0x1c = 1;
}

MessageWindow_804AF490::~MessageWindow_804AF490() {
    delete field_0x48;
    field_0x48 = NULL;
    delete field_0x4c;
    field_0x4c = NULL;
}

// FIXME(shotgun): 99.97 %, reloc-name
void MessageWindow_804AF490::fn_80391678() {
    field_0x8 = 0;
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x25 = field_0x24;
    field_0x26 = 0;
    field_0x41 = 0;
    field_0x40 = 1;
    field_0x28 = 0;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x42 = 0;
    field_0x43 = 0;
    field_0x3c = 0;
    field_0x3e = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x30 = -1;
    field_0x50.x = 0.0f;
    field_0x50.y = 0.0f;
    field_0x50.z = 0.0f;
    field_0x44 = 0;
    if (lbl_8053AD78->field_0xed != 0 && mIndex == lbl_8053AD78->field_0x110) {
        lbl_8053AD78->field_0xed = 0;
    }
    for (s32 i = 0; i < 4; i++) {
        s32 serial = field_0x48->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C();
                }
            }
            field_0x48->field_0x180[i] = -1;
        }
        serial = field_0x4c->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C();
                }
            }
            field_0x4c->field_0x180[i] = -1;
        }
    }
}

void MessageWindow_804AF490::fn_803913E0() {
    field_0x8 = 0;
    field_0x25 = 0;
    field_0x26 = 0;
    field_0x41 = 0;
    field_0x40 = 1;
    field_0x10 = 0;
    field_0x14 = 0;
    field_0x42 = 0;
    field_0x43 = 0;
    for (s32 i = 0; i < 4; i++) {
        s32 serial = field_0x48->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C();
                }
            }
            field_0x48->field_0x180[i] = -1;
        }
        serial = field_0x4c->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C();
                }
            }
            field_0x4c->field_0x180[i] = -1;
        }
    }
}

// FIXME(shotgun): 89.11 %, missing/extra (the two position copies and their stack slots)
void MessageWindow_804AF490::fn_80390EBC() {
    switch (field_0x24) {
    case 0:
        field_0x28 &= ~2;
        break;
    case 1: {
        field_0x43 = 0;
        field_0x2c = -1;
        field_0x4c->field_0x1aa = 1;
        field_0x4c->field_0x190 = field_0x3c;
        field_0x4c->field_0x192 = field_0x3e;
        field_0x4c->field_0x178 = field_0x34;
        field_0x4c->field_0x17c = field_0x38;
        cls_804AF5A0* proc = field_0x4c;
        proc->field_0x8->reset();
        proc->field_0x8->resetResourceCache();
        proc->field_0x1a2 = 0;
        field_0x44 = 2;
        JGeometry::TVec3f pos = field_0x50;
        JGeometry::TVec3f boxPos = field_0x50;
        proc = field_0x4c;
        u32 msg = field_0x20;
        proc->field_0x14c = msg;
        proc->field_0x8->setMessageCode(msg >> 16, msg & 0xFFFF);
        proc->field_0x98 = pos.x;
        proc->field_0x9c = pos.y;
        proc->field_0xa0 = pos.z;
        proc->field_0xa4 = boxPos;
        field_0x24 = 2;
        break;
    }
    case 2:
        if (field_0x25 == 5) {
            field_0x4c->field_0x10->fn_803A2F74();
        }
        field_0x4c->fn_80396F94();
        field_0x4c->fn_8039499C(false);
        field_0x24 = field_0x4c->field_0x3ae;
        field_0xc = (const u8*)&field_0x4c->field_0x1ab;
        break;
    case 3:
        field_0x2c = field_0x4c->field_0x168;
        break;
    case 4:
        field_0x2c = field_0x4c->field_0x168;
        if (lbl_8053AD04[mIndex] != 0) {
            field_0x43 = 0;
            field_0x24 = 5;
        }
        break;
    case 5:
        field_0x4c->fn_80396884();
        field_0x2c = field_0x4c->field_0x168;
        break;
    case 6: {
        if (field_0x28 & 0x40) {
            field_0x48->fn_803944EC(lbl_8050DB0C.field_0x30[2]);
        } else {
            field_0x48->fn_803944EC(1.0f);
        }
        field_0x2c = -1;
        field_0x48->field_0x1aa = 0;
        field_0x48->field_0x190 = field_0x3c;
        field_0x48->field_0x192 = field_0x3e;
        field_0x48->field_0x178 = field_0x34;
        field_0x48->field_0x17c = field_0x38;
        field_0x44 = 1;
        s32 kind = 0;
        u16 flags = field_0x28;
        if (flags & 0x200) {
            kind = 1;
        } else if (flags & 0x400) {
            kind = 2;
        }
        cls_804AF5A0* proc = field_0x48;
        proc->field_0x8->reset();
        proc->field_0x8->resetResourceCache();
        proc->field_0x1a2 = kind;
        s32 serial = field_0x30;
        if (serial != -1) {
            ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            JGeometry::TVec3f pos;
            if (actor != NULL) {
                pos = actor->mPos;
            } else {
                pos = field_0x50;
            }
            JGeometry::TVec3f boxPos = field_0x50;
            proc = field_0x48;
            u32 msg = field_0x20;
            proc->field_0x14c = msg;
            proc->field_0x8->setMessageCode(msg >> 16, msg & 0xFFFF);
            proc->field_0x98 = pos.x;
            proc->field_0x9c = pos.y;
            proc->field_0xa0 = pos.z;
            proc->field_0xa4 = boxPos;
        } else {
            JGeometry::TVec3f pos = field_0x50;
            JGeometry::TVec3f boxPos = field_0x50;
            proc = field_0x48;
            u32 msg = field_0x20;
            proc->field_0x14c = msg;
            proc->field_0x8->setMessageCode(msg >> 16, msg & 0xFFFF);
            proc->field_0x98 = pos.x;
            proc->field_0x9c = pos.y;
            proc->field_0xa0 = pos.z;
            proc->field_0xa4 = boxPos;
        }
        field_0x24 = 7;
        break;
    }
    case 7:
        field_0x48->field_0x3ae = 7;
        field_0x48->fn_80396F94();
        field_0x28 |= 2;
        field_0x24 = field_0x48->field_0x3ae;
        if (field_0x24 == 8) {
            field_0x40 = field_0x48->field_0x3af;
        }
        break;
    case 8:
        field_0x48->fn_80396F94();
        field_0x28 |= 2;
        field_0x40 = field_0x48->field_0x3af;
        field_0x2c = field_0x48->field_0x168;
        break;
    }
    if (lbl_8053AD78->field_0x44 == 0) {
        cls_804AF5A0* proc = field_0x48;
        if (proc->field_0x3b1 != 0 || field_0x4c->field_0x3b1 != 0) {
            if (field_0x24 == 0) {
                proc->field_0x3b1 = 0;
                field_0x4c->field_0x3b1 = 0;
                lbl_8053AD78->field_0xe9 = 1;
            }
        }
    }
    field_0x25 = field_0x24;
    if (field_0x24 == 6 || field_0x24 == 7 || field_0x24 == 8) {
        Camera_804AA9E4* camera = gGameRoot2_8053AB10->field_0x24;
        if (camera->field_0x179 == 0) {
            camera->field_0x17b = 1;
        }
    }
}

void MessageWindow_804AF490::fn_80390E68() {
    bool flag = false;
    u16 flags = field_0x28;
    if ((flags >> 1) & 1) {
        if (flags & 0x40) {
            field_0x48->fn_80394690();
        } else {
            if (flags & 0x2000) {
                flag = true;
            }
            field_0x48->fn_8039499C(flag);
        }
    }
}

bool MessageWindow_804AF490::fn_80390E40(s32 value, bool flag) {
    if (field_0x24 == 0 || flag) {
        field_0x20 = value;
        return true;
    }
    return false;
}

// FIXME(shotgun): 99.54 %, scheduling+regalloc
u16 MessageWindow_804AF490::fn_80390C6C() {
    if (field_0x24 == 3) {
        switch (field_0x41) {
        case 0:
            field_0x26 = (field_0xc[0] << 8) | field_0xc[1];
            field_0x41 = 1;
            field_0x10 = 2;
            field_0x14 = 0;
            field_0x43++;
            field_0xc++;
            field_0xc++;
            break;
        case 1:
            if (field_0x28 & 1) {
                field_0x26 = 0xFFFC;
                field_0x41 = 2;
                field_0x43++;
                break;
            }
            field_0x41 = 2;
        case 2:
            field_0x26 = (field_0xc[0] << 8) | field_0xc[1];
            if (field_0x26 == 0xFFF0) {
                field_0x10 += 2;
                field_0x14++;
                field_0xc++;
                field_0xc++;
            } else if (field_0x26 == 0xFFFE) {
                field_0x41 = 3;
                field_0x40 = field_0x4c->field_0x3af;
            } else {
                field_0x10 += 2;
                field_0xc++;
                field_0xc++;
            }
            if (field_0x10 >= field_0x4c->field_0x14->field_0x2d0) {
                field_0x41 = 3;
                field_0x40 = field_0x4c->field_0x3af;
            }
            field_0x43++;
            break;
        case 3:
            field_0x24 = 4;
            field_0x26 = 0xFFFE;
            field_0x41 = 0;
            field_0x43++;
            break;
        }
    } else {
        field_0x26 = 0;
    }
    return field_0x26;
}

s32 MessageWindow_804AF490::fn_80390C38() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x148;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x148;
    }
    return -1;
}

u32 MessageWindow_804AF490::fn_80390BDC() {
    if (field_0x24 == 5) {
        cls_804AF5A0* proc = field_0x4c;
        if (proc->field_0x150 != 0) {
            proc->field_0x150--;
        }
        return proc->field_0x150;
    }
    if (field_0x24 == 8) {
        cls_804AF5A0* proc = field_0x48;
        if (proc->field_0x150 != 0) {
            proc->field_0x150--;
        }
        return proc->field_0x150;
    }
    return 0;
}

u8 MessageWindow_804AF490::fn_80390BA8() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x19e;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x19e;
    }
    return 0xFF;
}

void MessageWindow_804AF490::fn_80390B64() {
    if (field_0x24 == 5) {
        cls_804AF5A0* proc = field_0x4c;
        proc->field_0x19e = 0xFF;
        proc->field_0x19f = 1;
    } else if (field_0x24 == 8) {
        cls_804AF5A0* proc = field_0x48;
        proc->field_0x19e = 0xFF;
        proc->field_0x19f = 1;
    }
}

bool MessageWindow_804AF490::fn_803908D0() {
    for (s32 i = 0; i < 4; i++) {
        s32 serial = field_0x48->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    if (!static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C()) {
                        return false;
                    }
                }
            }
            field_0x48->field_0x180[i] = -1;
        }
        serial = field_0x4c->field_0x180[i];
        if (serial >= 0) {
            if (isGameModeSet() == 1) {
                ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                if (actor != NULL && actor->mActorId == ACTOR_TKRA) {
                    if (!static_cast<TreasureChest_804AA178*>(actor)->fn_802EB27C()) {
                        return false;
                    }
                }
            }
            field_0x4c->field_0x180[i] = -1;
        }
    }
    return true;
}

u8 MessageWindow_804AF490::fn_80390894(s32 index) {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x3b2[index];
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x3b2[index];
    }
    return 0;
}

s32 MessageWindow_804AF490::fn_80390850() {
    // A signed word, not the byte: the original compares it with cmpwi.
    s32 state = field_0x24;
    s32 value = -1;
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        value = field_0x4c->field_0x174;
        break;
    case 6:
    case 7:
    case 8:
        value = field_0x48->field_0x174;
        break;
    }
    return value;
}

u8 MessageWindow_804AF490::fn_80390818() {
    u8 result = 0;
    switch (field_0x24) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        result = 2;
        break;
    case 6:
    case 7:
    case 8:
        result = 1;
        break;
    }
    return result;
}
