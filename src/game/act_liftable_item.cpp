#include "game/Effect.h"
#include "game/GameAudio.h"
#include "game/Message.h"
#include "game/act/act_liftable_item.h"
#include "game/act/act_stop_npc.h"
#include "game/ActorManager.h"
#include "game/GameGlobals.h"
#include "game/Map.h"
#include "game/Player.h"

extern const s32 lbl_80465E38[80] = {
    0x0000072F, 0x00000730, 0x00000731, 0x00000732,
    0x000000D7, 0x000000D8, 0x000000D9, 0x000000DA,
    0x000000E6, 0x0000010F, 0x00000110, 0x00000111,
    0x000000D6, 0x000000D6, 0x000000D6, 0x000000D6,
    0x000000D1, 0x000000D2, 0x00000733, 0x00000266,
    0x000000C8, 0x00000734, 0x00000112, 0x00000735,
    0x00000736, 0x00000735, 0x000000C8, 0x000000C8,
    0x000000C8, 0x000000C8, 0x000000C8, 0x000000C8,
    0x000000D3, 0x000000E6, 0x000000E6, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000E6, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000C8, 0x000000C8,
    0x000000C8, 0x000000C8, 0x0000010E, 0x000000DB,
    0x000000C7, 0x000000DC, 0x000000C9, 0x000000CA,
    0x000000CB, 0x000000CC, 0x000000CD, 0x000000CE,
    0x000000CF, 0x000000D0, 0x000000E6, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000E6, 0x000000E6,
    0x0000026A, 0x000000DC, 0x00000263, 0x00000264,
    0x00000262, 0x00000261, 0x00000269, 0x00000268,
    0x0000027F, 0x00000260, 0x00000265, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000E6, 0x000000E6,
};

extern const s32 lbl_80465F78[80] = {
    0x000001BC, 0x000001BD, 0x000001BE, 0x000001BF,
    0x00000194, 0x00000193, 0x00000192, 0x00000191,
    0x000001B7, 0x000001B8, 0x000001B9, 0x000001BA,
    0x000000D6, 0x000000D6, 0x000000D6, 0x000000D6,
    0x000001A6, 0x000001A7, 0x00000411, 0x00000412,
    0x0000019C, 0x00000413, 0x000001BB, 0x00000414,
    0x00000415, 0x00000415, 0x0000019C, 0x0000019C,
    0x0000019C, 0x0000019C, 0x0000019C, 0x0000019C,
    0x000001A1, 0x00000416, 0x00000417, 0x00000418,
    0x00000419, 0x00000115, 0x000000E6, 0x000000E6,
    0x000000E6, 0x000000E6, 0x0000019C, 0x0000019C,
    0x0000019C, 0x0000019C, 0x00000431, 0x00000432,
    0x0000019E, 0x000001AB, 0x000001A5, 0x0000019D,
    0x0000019F, 0x000001A9, 0x000001A2, 0x000001A3,
    0x000001A4, 0x000001A0, 0x000000E6, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000E6, 0x000000E6,
    0x0000019E, 0x000001AB, 0x000001A5, 0x0000019D,
    0x0000019F, 0x000001A9, 0x000001A2, 0x000001A3,
    0x000001A4, 0x000001A0, 0x00000149, 0x000000E6,
    0x000000E6, 0x000000E6, 0x000000E6, 0x000000E6,
};

s32 lbl_804A0170[12] = {
    1, 5, 20, 50, 100, 150, 200, 300, 1000, 1500, 2000, 3000,
};
s32 lbl_804A01A0[12] = {
    1, 5, 9, 13, 17, 21, 25, 29, 0x45A, 0x456, 0x462, 0x45E,
};

void LiftableItem_804A01D0::init() {
    field_0x234 = field_0x90 & 0xFFF;
    field_0x250 = (field_0x90 >> 24) & 3;
    field_0x24c = (field_0x90 >> 26) & 3;
    field_0x254 = (field_0x90 >> 28) & 1;
    field_0x268 = (field_0x90 >> 23) & 1;
    field_0x26c = (field_0x90 >> 22) & 1;
    field_0x270 = (field_0x90 >> 21) & 1;
    field_0x258 = (field_0x90 >> 31) & 1;
    field_0x25c = (field_0x90 >> 30) & 1;
    field_0x264 = (field_0x90 >> 29) & 1;
    mEmitter2a8 = NULL;
    mEmitter2ac = NULL;
    mEmitter2b0 = NULL;
    field_0x290 = -1;
    field_0x244 = 0;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 26;
    field_0x29c.set(0.0f, 0.0f, 0.0f);
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    if (gGameRoot_8053AAF8->field_0x7f == 1) {
        field_0x238 = 90;
    } else {
        field_0x238 = 45;
    }
    field_0x288 = 0;
    field_0x27c = *getPlayerPos_8023E724(field_0x250);
    field_0x274 = field_0x250;
    if (fn_8022F514(field_0x250)) {
        field_0x274 = fn_8022F3DC(field_0x250);
    }
    field_0x28c = 10;
    switch (field_0x234) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (field_0x25c == 0) {
            fn_802362C4(field_0x274, lbl_804A0170[field_0x234]);
        }
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        if (field_0x25c == 0) {
            lbl_8053AED0->fn_8043D874(field_0x274, lbl_804A0170[field_0x234], 0);
        }
        if (field_0x270 != 0) {
            field_0x28c = 2;
        }
        break;
    case 0x2E:
        mPos.z = field_0x27c.z + 22.0f;
        field_0x290 = getActorManager_801F856C()->spawnActor_801F9484('BBMB', field_0x4, &mPos, 0x20000, -1, -1);
        break;
    }
    field_0x230 = 0;
    if (field_0x258 == 0) {
        if (field_0x28c != 2) {
            field_0x260 = getActorManager_801F856C()->spawnActor_801F9484('STPN', field_0x4, &mPos, 0, -1, -1);
            if (gGameRoot_8053AAF8->field_0x96 == 0) {
                field_0x260 = 0;
            }
        } else {
            if (gGameRoot_8053AAF8->field_0x7e == 0) {
                field_0x238 = 45;
            } else {
                field_0x28c = 10;
            }
            field_0x260 = 0;
        }
    } else {
        field_0x260 = 0;
    }
    field_0x278 = -1;
    field_0x294 = -1;
}

LiftableItem_804A01D0::~LiftableItem_804A01D0() {
    releaseEmitter_801EE434(mEmitter2a8);
    releaseEmitter_801EE434(mEmitter2ac);
    releaseEmitter_801EE434(mEmitter2b0);
}

static inline u32 checkInputs_8015A374() {
    for (s32 i = 0; i < 4; i++) {
        u32 input = lbl_8053AD30->fn_80309D6C(getPlayerInputSlot_8030AAF4(i), 0x100);
        if (input != 0) {
            return input;
        }
    }
    return 0;
}

// FIXME: 98.86% (different: same length, 28 opcodes differ; arg_mismatch on stack offsets from 0xa0 vs 0x90 frame size)
void LiftableItem_804A01D0::update() {
    JGeometry::TVec3f pos;
    JGeometry::TVec3f pos2;

    addCalc2_801F71A4(&field_0x3c.x, field_0x29c.x, 0.3f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x29c.y, 0.3f, 1.0f);
    mPos += field_0x3c;

    switch (field_0x230) {
    case 0:
        if (field_0x238 <= 0) {
            if (field_0x26c != 0) {
                field_0x230 = 1;
                break;
            }
            if (field_0x258 == 0) {
                if (field_0x4 >= 8 && gGameRoot_8053AAF8->field_0x7e == 1) {
                    lbl_8053AED0->fn_8043D67C(lbl_80465F78[field_0x234], field_0x274, 0);
                }
                if ((u32)lbl_80465E38[field_0x234] == 0x27F) {
                    if ((u32)((gGameRoot_8053AAF8->field_0x83 >> (field_0x274 * 2)) & 3) == 2) {
                        if (field_0x250 == field_0x24c) {
                            field_0x278 = getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x4, &mPos, (field_0x250 & 3) | 0x24, -1, -1);
                            fn_8038BD08(field_0x24c, lbl_80465E38[field_0x234], field_0x4, mPos, 0x21);
                        }
                    } else {
                        if (field_0x250 == field_0x24c) {
                            field_0x278 = getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x4, &mPos, (field_0x250 & 3) | 0x24, -1, -1);
                            fn_8038BD08(field_0x24c, 0x284, field_0x4, mPos, 0x21);
                        }
                    }
                } else if (field_0x234 < 0x10) {
                    if (field_0x250 == field_0x24c) {
                        field_0x278 = getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x4, &mPos, (field_0x250 & 3) | 0x24, -1, -1);
                        fn_8038BD08(field_0x24c, lbl_80465E38[field_0x234], field_0x4, mPos, 1);
                    }
                } else {
                    if (field_0x250 == field_0x24c) {
                        field_0x278 = getActorManager_801F856C()->spawnActor_801F9484('PEFF', field_0x4, &mPos, (field_0x250 & 3) | 0x24, -1, -1);
                        fn_8038BD08(field_0x24c, lbl_80465E38[field_0x234], field_0x4, mPos, 0x21);
                    }
                }
                if (field_0x250 == field_0x24c) {
                    fn_801597EC();
                }
            } else {
                fn_801597EC();
            }
            field_0x230 = 1;
        } else {
            if (field_0x240 == 1 && field_0x234 == 0x2E) {
                fn_80230EB8(field_0x250, 0x3E);
            }
            field_0x28c--;
            if (field_0x28c == 0) {
                field_0x28c = 1;
                if (field_0x26c != 0) {
                    s32 serial = field_0x260;
                    if (serial != 0) {
                        StopNPC_804AA2D8* actor = (StopNPC_804AA2D8*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                        if (actor != NULL) {
                            actor->fn_802ED140();
                        }
                    }
                }
                if (field_0x234 != 0x2F) {
                    if (field_0x234 == 0x14) {
                        if (field_0x288 == 0) {
                            mPos.x = field_0x27c.x + 4.0f;
                            mPos.y = field_0x27c.y - 16.0f;
                            mPos.z = field_0x27c.z;
                            field_0x28c = 4;
                            fn_80236804(fn_8022F340(field_0x274), 4);
                            fn_80230F44(field_0x24c, 0x01000002);
                        } else if (field_0x288 == 1) {
                            mPos.x = field_0x27c.x + 6.0f;
                            mPos.y = field_0x27c.y - 19.0f;
                            mPos.z = field_0x27c.z;
                            mEmitter2b0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x562, 0, 1, NULL, NULL, true, 0.8f);
                            mEmitter2a8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x563, 0, 1, NULL, NULL, true, 0.6f);
                        }
                        if (field_0x234 != 0x2F && field_0x288 < 4) {
                            field_0x288++;
                        }
                        if (field_0x288 < 4) {
                            field_0x288++;
                        }
                        s32 r5 = field_0x288;
                        if (r5 >= 4) {
                            r5 = 3;
                        }
                        fn_80230498(field_0x250, &field_0x27c, r5 + 0xE9, 0xFF, -0x80, -0x80, true);
                    } else if (field_0x234 == 0x2E) {
                        if (field_0x288 == 0) {
                            mPos.z = field_0x27c.z + 22.0f;
                            field_0x28c = 4;
                        } else if (field_0x288 == 1) {
                            mPos.z = field_0x27c.z + 39.0f;
                            field_0x28c = 4;
                        } else if (field_0x288 == 2) {
                            mPos.z = field_0x27c.z + 28.0f;
                        }
                        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(field_0x290);
                        if (actor != NULL) {
                            pos = mPos;
                            actor->mPos = pos;
                        }
                        if (field_0x288 < 4) {
                            field_0x288++;
                        }
                        s32 r30 = field_0x288;
                        if (r30 >= 4) {
                            r30 = 3;
                        }
                        if (fn_80233E40(field_0x250)) {
                            fn_80230498(field_0x250, &field_0x27c, r30 + 0x336, 0xFF, -0x80, -0x80, true);
                        } else {
                            fn_80230498(field_0x250, &field_0x27c, r30 + 0xED, 0xFF, -0x80, -0x80, true);
                        }
                    } else if (field_0x234 <= 0x0B) {
                        if (field_0x288 == 0) {
                            mPos.y = field_0x27c.y - 18.0f;
                            if (field_0x270 != 0 && gGameRoot_8053AAF8->field_0x7e == 1) {
                                field_0x28c = 4;
                            }
                        } else if (field_0x288 == 1) {
                            mPos.y = field_0x27c.y - 35.0f;
                            if (field_0x270 != 0 && gGameRoot_8053AAF8->field_0x7e == 1) {
                                field_0x28c = 4;
                            }
                            if (field_0x25c == 0) {
                                fn_80230FD0(field_0x250, 0x10008);
                            }
                        } else if (field_0x288 == 2) {
                            if (field_0x270 != 0 && gGameRoot_8053AAF8->field_0x7e == 1) {
                                field_0x28c = 50;
                            }
                            mPos.y = field_0x27c.y - 24.0f;
                            if (field_0x234 >= 8) {
                                mEmitter2b0 = spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(mPos.x, mPos.y - 12.0f, mPos.z), 0x562, 0, 1, NULL, NULL, true, 1.6f);
                                mEmitter2a8 = spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(mPos.x, mPos.y - 6.0f, mPos.z), 0x563, 0, 1, NULL, NULL, true, 1.2f);
                                if (field_0x25c == 0) {
                                    pos = mPos;
                                    pos.y -= 32.0f;
                                    getActorManager_801F856C()->fn_801F6718(field_0x274, field_0x4, &pos, field_0x234, 2);
                                }
                            } else {
                                mEmitter2b0 = spawnEmitter_8013CC50(field_0x4, &JGeometry::TVec3f(mPos.x, mPos.y - 6.0f, mPos.z), 0x562, 0, 1, NULL, NULL, true, 0.8f);
                                mEmitter2a8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x563, 0, 1, NULL, NULL, true, 0.6f);
                                if (field_0x25c == 0) {
                                    pos2 = mPos;
                                    pos2.y -= 16.0f;
                                    getActorManager_801F856C()->fn_801F6718(field_0x274, field_0x4, &pos2, field_0x234, 2);
                                }
                            }
                        }
                        if (field_0x288 < 3) {
                            field_0x288++;
                        }
                        if (field_0x25c == 0) {
                            s32 r30 = field_0x288;
                            if (r30 >= 4) {
                                r30 = 3;
                            }
                            if (fn_80233E40(field_0x250)) {
                                fn_80230498(field_0x250, &field_0x27c, r30 + 0x336, 0xFF, -0x80, -0x80, true);
                            } else {
                                fn_80230498(field_0x250, &field_0x27c, r30 + 0xED, 0xFF, -0x80, -0x80, true);
                            }
                        }
                    } else if ((u32)(field_0x234 - 0x10) <= 2 || (u32)(field_0x234 - 0x16) <= 3 || field_0x234 == 0x25) {
                        if (field_0x288 == 0) {
                            mPos.y = field_0x27c.y - 16.0f;
                            mPos.z = field_0x27c.z;
                            field_0x28c = 4;
                            fn_80230F44(field_0x24c, 0x01000002);
                        } else if (field_0x288 == 1) {
                            mPos.y = field_0x27c.y - 19.0f;
                            mPos.z = field_0x27c.z;
                            mEmitter2b0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x562, 0, 1, NULL, NULL, true, 0.7f);
                            mEmitter2a8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x563, 0, 1, NULL, NULL, true, 0.6f);
                        }
                        if (field_0x288 < 4) {
                            field_0x288++;
                        }
                        s32 r30 = field_0x288;
                        if (r30 >= 4) {
                            r30 = 3;
                        }
                        if (fn_80233E40(field_0x250)) {
                            fn_80230498(field_0x250, &field_0x27c, r30 + 0x336, 0xFF, -0x80, -0x80, true);
                        } else {
                            fn_80230498(field_0x250, &field_0x27c, r30 + 0xED, 0xFF, -0x80, -0x80, true);
                        }
                    } else {
                        if (field_0x288 == 0) {
                            mPos.x = field_0x27c.x + 4.0f;
                            mPos.y = field_0x27c.y - 16.0f;
                            mPos.z = field_0x27c.z;
                            fn_80230F44(field_0x24c, 0x01000002);
                            field_0x28c = 4;
                        } else if (field_0x288 == 1) {
                            mPos.x = field_0x27c.x + 6.0f;
                            mPos.y = field_0x27c.y - 19.0f;
                            mPos.z = field_0x27c.z;
                            mEmitter2b0 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x562, 0, 1, NULL, NULL, true, 0.7f);
                            mEmitter2a8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x563, 0, 1, NULL, NULL, true, 0.6f);
                        }
                        if (field_0x288 < 4) {
                            field_0x288++;
                        }
                        s32 r5 = field_0x288;
                        if (r5 >= 4) {
                            r5 = 3;
                        }
                        fn_80230498(field_0x250, &field_0x27c, r5 + 0xE9, 0xFF, -0x80, -0x80, true);
                    }
                }
            }
        }
        if (field_0x288 >= 4) {
            if (checkInputs_8015A374() != 0) {
                field_0x288 = 3;
                field_0x238 = 0;
            }
        }
        break;

    case 1:
        if (field_0x238 <= 0) {
            if (field_0x26c != 0) {
                if (field_0x25c != 0 || fn_803865B8(field_0x250)) {
                    vfunc_0x24();
                }
                break;
            }
            if (field_0x234 == 0x2E) {
                if (field_0x4 < 8 && fn_80388EEC(field_0x250)) {
                    field_0x230 = 2;
                    field_0x238 = 20;
                    LiftableItem_804A01D0* actor = (LiftableItem_804A01D0*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x290);
                    if (actor != NULL) {
                        actor->field_0x250 = gAudioMgr_8053AF80->field_0x58;
                        actor->field_0x230 = 4;
                        actor->field_0x3c = JGeometry::TVec3f(0.0f, 3.0f, 0.0f);
                    }
                }
                break;
            }
            if (field_0x258 == 0) {
                if (gGameRoot_8053AAF8->field_0x7e == 1) {
                    if (fn_80388EEC(field_0x24c)) {
                        if (field_0x268 != 0) {
                            if (fn_8023061C(field_0x274, 0) == true) {
                                vfunc_0x24();
                            }
                        } else {
                            if (fn_80230224(field_0x250, 0) == true) {
                                vfunc_0x24();
                            }
                        }
                    }
                } else {
                    if (fn_80388EEC(field_0x24c)) {
                        if (field_0x268 != 0) {
                            if (fn_80230534(false) == true) {
                                vfunc_0x24();
                            }
                        } else {
                            if (fn_80230224(field_0x250, 0) == true) {
                                vfunc_0x24();
                            }
                        }
                    }
                }
            } else {
                field_0x230 = 3;
            }
        }
        if (checkInputs_8015A374() != 0) {
            field_0x238 = 0;
            fn_8037E8D4(field_0x250);
        }
        break;

    case 2:
        if (field_0x238 > 17) {
            field_0x288 = 14;
        } else if (field_0x238 > 9) {
            field_0x288 = 13;
        } else if (field_0x238 > 8) {
            field_0x288 = 12;
        } else {
            field_0x288 = 11;
            if (gGameRoot_8053AAF8->field_0x7e == 1) {
                if (fn_80388EEC(field_0x24c)) {
                    if (field_0x268 != 0) {
                        if (fn_8023061C(field_0x274, 0) == true) {
                            vfunc_0x24();
                            break;
                        }
                    } else {
                        if (fn_80230224(field_0x250, 0) == true) {
                            vfunc_0x24();
                            break;
                        }
                    }
                }
            } else {
                if (fn_80388EEC(field_0x24c)) {
                    if (field_0x268 != 0) {
                        if (fn_80230534(false) == true) {
                            vfunc_0x24();
                            break;
                        }
                    } else {
                        if (fn_80230224(field_0x250, 0) == true) {
                            vfunc_0x24();
                            break;
                        }
                    }
                }
            }
        }
        fn_80230498(field_0x250, &field_0x27c, field_0x288 + 0x322, 0xFF, -0x80, -0x80, true);
        break;

    case 3:
        break;
    }

    if (field_0x234 == 0x20) {
        if (field_0x23c <= 0) {
            field_0x244 ^= 1;
            field_0x23c = 10;
        }
    } else if (field_0x234 <= 0x0B) {
        if (field_0x23c <= 0) {
            field_0x244++;
            field_0x244 &= 3;
            if (field_0x244 == 0) {
                field_0x23c = 20;
            } else {
                field_0x23c = 2;
            }
        }
    } else {
        if (field_0x23c <= 0) {
            field_0x244 ^= 1;
            field_0x23c = 10;
        }
    }

    if (field_0x238 > 0) {
        field_0x238--;
    }
    if (field_0x23c > 0) {
        field_0x23c--;
    }
    if (field_0x240 > 0) {
        field_0x240--;
    }
}

// FIXME: 97.99% (missing/extra + reloc-name: ours has 3 more instrs; regalloc on field_0x234 in r4 vs r5)
void LiftableItem_804A01D0::draw() {
    if (!fn_8023188C(field_0x250)) {
        return;
    }
    s32 r30 = 0;
    if (field_0x234 >= 8 && field_0x234 <= 11) {
        r30 = 1;
    }
    SpriteParams_801F5AE0 params;
    params.field_0x1d = 1;
    params.field_0x1c = 1;
    params.field_0x2d = 1;

    if (field_0x234 == 0x2f) {
        return;
    }

    if (field_0x234 <= 7) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + lbl_804A01A0[field_0x234], &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + lbl_804A01A0[field_0x234], r30 != 0, 2, -1, true, 0, 0);
    } else if (field_0x234 <= 11) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x244 + lbl_804A01A0[field_0x234], &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + lbl_804A01A0[field_0x234 - 4], r30 != 0, 2, -1, true, 0, 0);
    } else if (field_0x234 >= 0x10 && field_0x234 <= 0x19) {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x1d;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x1d, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x20) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x90, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x90, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x21) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x2af, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x2af, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x22) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x8c, &params, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x8c, false, 2, -1, false, 0, 0);
    } else if (field_0x234 == 0x23) {
        params.field_0x2d = 1;
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x2aa, &params, 7, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x2aa, false, 2, -1, false, 0, 0);
    } else if (field_0x234 == 0x24) {
        drawSprite_801F06F0("enemy_both2_sob.bin", field_0x244 + 0x2c7, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x244 + 0x2c7, false, 2, -1, true, 0, 0);
    } else if (field_0x234 == 0x25) {
        drawSprite_801F06F0("enemy_both2_sob.bin", 0x98, &params, -1, 1.0f, 1.0f);
        fn_803075AC(mSerial, field_0x4, &mPos, 0x98, false, 2, -1, true, 0, 0);
    } else if (field_0x234 >= 0x2a && field_0x234 <= 0x2d) {
        drawSprite_801F06F0("enemy_gc_sob.bin", field_0x234 + 0x255, &params, -1, 1.0f, 1.0f);
    } else if (field_0x234 == 0x2e) {
        return;
    } else if (field_0x234 < 0x40) {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x11;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x11, false, 2, -1, true, 0, 0);
    } else {
        JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
        s32 id = field_0x234 + 0x1cd;
        if (field_0x4 < 8) {
            void* res = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
            gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&pos, res, id, &params, 1.0f, 1.0f);
        }
        fn_803075AC(mSerial, field_0x4, &mPos, field_0x234 + 0x1cd, false, 2, -1, true, 0, 0);
    }
}

void LiftableItem_804A01D0::vfunc_0x24() {
    if (mEmitter2a8 != NULL) {
        mEmitter2a8->becomeInvalidEmitter();
        mEmitter2a8->stopCreateParticle();
        mEmitter2a8 = NULL;
    }
    if (mEmitter2ac != NULL) {
        mEmitter2ac->becomeInvalidEmitter();
        mEmitter2ac->stopCreateParticle();
        mEmitter2ac = NULL;
    }
    if (mEmitter2b0 != NULL) {
        mEmitter2b0->becomeInvalidEmitter();
        mEmitter2b0->stopCreateParticle();
        mEmitter2b0 = NULL;
    }
    s32 serial = field_0x260;
    if (serial != 0) {
        StopNPC_804AA2D8* actor = (StopNPC_804AA2D8*)getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->fn_802ED140();
        }
    }
    mAlive = false;
    serial = field_0x278;
    if (serial >= 0) {
        ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (actor != NULL) {
            actor->mAlive = false;
        }
        field_0x278 = -1;
    }
    if (field_0x294 >= 0) {
        field_0x27c = *getPlayerPos_8023E724(field_0x250);
        getActorManager_801F856C()->spawnActor_801F9484('LSTI', field_0x4, &field_0x27c, (field_0x298 << 22) | field_0x294, -1, -1);
    }
}

void LiftableItem_804A01D0::fn_801597EC() {
    s32 item = field_0x234;
    if (item == 0x10 || (u32)(item - 0x11) <= 1 || (u32)(item - 0x16) <= 2 || item == 0x19) {
        getActorManager_801F856C()->spawnActor_801F9484('KEY0', field_0x4, &mPos, item + 0xF0 + (field_0x250 << 12), -1, -1);
    } else if (item == 0x13) {
        fn_80236DB0(field_0x274);
    } else if (item == 0x15) {
        fn_80236C88(field_0x274);
    } else if (item == 0x20) {
        getActorManager_801F856C()->spawnActor_801F9484('CRTL', field_0x4, &mPos, (field_0x250 << 16) | 1, -1, -1);
    } else if (item == 0x21) {
        getActorManager_801F856C()->spawnActor_801F9484('NIWA', field_0x4, &mPos, (field_0x250 << 8) | 5, -1, -1);
    } else if (item == 0x22) {
        getActorManager_801F856C()->spawnActor_801F9484('BMTY', field_0x4, &mPos, (field_0x250 << 16) | 5, -1, -1);
    } else if (item == 0x23) {
        getActorManager_801F856C()->spawnActor_801F9484('NNJN', field_0x4, &mPos, 0, -1, -1);
    } else if (item == 0x24) {
        getActorManager_801F856C()->spawnActor_801F9484('NIWA', field_0x4, &mPos, (field_0x250 << 8) | 0x1005, -1, -1);
    } else if (item == 0x25) {
        getActorManager_801F856C()->spawnActor_801F9484('CRTL', field_0x4, &mPos, (field_0x250 << 16) | 3, -1, -1);
    } else if (item >= 0x30) {
        if (item == 0x4a) {
            fn_80236ED8(field_0x274, 2);
        } else {
            s32 r29 = 1;
            s32 r28 = item - 0x2f;
            if (item >= 0x40) {
                r28 = item - 0x3f;
                r29 = 2;
            }
            if (field_0x264 == 0) {
                if (field_0x254 == 0 && fn_802373EC(field_0x250) != 0 && gGameRoot_8053AAF8->field_0x7f == 1 && field_0x258 == 0) {
                    field_0x294 = fn_802373EC(field_0x274);
                    field_0x298 = fn_8023725C(field_0x274);
                }
                fn_802372F8(field_0x250, r28, r29);
            }
        }
    }
}

void LiftableItem_804A01D0::fn_801597C0() {
    vfunc_0x24();
}

s32 LiftableItem_804A01D0::fn_801597B8() {
    return field_0x230;
}
