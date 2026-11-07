#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/Player.h"
#include "game/act/act_bee_manager.h"
#include "game/act/tu_803489A4.h"
#include "JSystem/JMath/JMath.h"

// FIXME(shotgun): 99.46 %, scheduling + reloc-name
void BeeFormation_804AC7B8::init() {
    mLife = 8;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    field_0x298.set(0.0f, 0.0f, 0.0f);
    field_0x268.set(0.0f, 0.0f, 0.0f);
    field_0x274.set(0.0f, 0.0f, 0.0f);
    field_0x280.set(0.0f, 0.0f, 0.0f);
    field_0x28c.set(0.0f, 0.0f, 0.0f);
    field_0x258 = 5.0f;
    field_0x25c = 32.0f;
    field_0x248 = 0;
    field_0x260 = 0;
    field_0x254 = 0;
    field_0x234 = 0x78;
    field_0x238 = 0;
    field_0x23c = 0;
    field_0x240 = 0;
    field_0x244 = -1;
    field_0x250 = -1;
    field_0x24c = -1;
    field_0x262 = 0;
    field_0x263 = 0;
    field_0x264 = 0;
    field_0x265 = 0;
    mBox.i.set(-8.0f, -8.0f);
    mBox.f.set(8.0f, 8.0f);
    field_0x230 = 0;
}

// FIXME(shotgun): 92.5 %, missing/extra
void BeeFormation_804AC7B8::update() {
    BeeManager_804AC810* mgr = (BeeManager_804AC810*)getActorManager_801F856C()->findActorBySerial_801F666C(field_0x250);
    if (field_0x262 == 1) {
        return;
    }
    if (fn_801F3D94(0) != 0) {
        return;
    }

    if (field_0x230 != 0 && field_0x264 == 0) {
        field_0x24c = fn_801F285C(NULL, NULL, true, -1);
        if (field_0x24c >= 0 && fn_802353AC(field_0x24c) == true) {
            field_0x264 = 1;
            field_0x240 = 10;
            if (field_0x230 != 4) {
                field_0x3c.set(0.0f, 0.0f, 0.0f);
                field_0x230 = 4;
            }
            return;
        }
    }

    if (field_0x265 == 1) {
        const JGeometry::TVec3f* pos = getPlayerPos_8023E724(field_0x24c);
        fn_801F37AC(fn_8023B6CC(field_0x24c), pos, 0, 1, -1);
        s32 mgrSerial = field_0x250;
        if (mgrSerial >= 0) {
            BeeManager_804AC810* m = (BeeManager_804AC810*)getActorManager_801F856C()->findActorBySerial_801F666C(mgrSerial);
            if (m != NULL) {
                s32 player = field_0x24c;
                if (player >= 0 && fn_8022F514(player) == true) {
                    m->field_0x27c = fn_8022F3DC(player);
                } else {
                    m->field_0x27c = player;
                }
            }
        }
        return;
    }

    if (field_0x264 == 1 && field_0x240 > 0) {
        field_0x240--;
        return;
    }

    addCalc2_801F71A4(&field_0x3c.x, field_0x298.x, 0.4f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.y, field_0x298.y, 0.4f, 1.0f);
    addCalc2_801F71A4(&field_0x3c.z, field_0x298.z, 0.3f, 0.5f);

    if (field_0x230 != 2 && field_0x230 != 3) {
        mPos.add(field_0x3c);
    }

    if (field_0x230 != 4) {
        if (field_0x230 == 0) {
            if (mgr != NULL) {
                field_0x268 = mgr->mPos;
            }
        } else if (field_0x244 < 0) {
            field_0x230 = 4;
        } else if (getPlayerLayer_8023DE58(field_0x244) < 8) {
            field_0x274 = field_0x268;
            field_0x268 = *getPlayerPos_8023E724(field_0x244);
            s32 unk = fn_8022E970(field_0x244);
            switch (fn_8022CAF8(field_0x244)) {
            case 0:
                if (unk > 1 && unk < 5) {
                    field_0x268.x += 8.0f * (f32)(unk - 1);
                }
                break;
            case 1:
                if (unk > 1 && unk < 5) {
                    field_0x268.y += 8.0f * (f32)(unk - 1);
                }
                break;
            case 2:
                if (unk == 2) {
                    field_0x268.x += 8.0f;
                } else if (unk >= 3 && unk < 5) {
                    field_0x268.x += 8.0f;
                    field_0x268.y += 8.0f;
                }
                break;
            case 3:
                if (unk != 1) {
                    if (fn_802353AC(field_0x244) == true) {
                        field_0x268 = field_0x274;
                    } else {
                        switch (field_0x244) {
                        case 0:
                            field_0x268.y += 16.0f;
                            break;
                        case 1:
                            field_0x268.x += 16.0f;
                            break;
                        case 2:
                            field_0x268.x -= 16.0f;
                            break;
                        case 3:
                            field_0x268.y -= 16.0f;
                            break;
                        }
                    }
                }
                break;
            case 4:
                field_0x268 = *getPlayerPos_8023E724(fn_8022EA0C(field_0x244));
                break;
            case 5:
                break;
            }
        }
    }

    s16 count = mgr != NULL ? 14 : 14;
    field_0x28c = field_0x280;
    s16 angle = field_0x260 + (s32)((f32)(field_0x254 << 16) / (f32)count);
    field_0x280.x = field_0x268.x + field_0x25c * JMath::sincosTable_.sinShort(angle);
    field_0x280.y = field_0x268.y + field_0x25c * JMath::sincosTable_.cosShort(angle);

    if (field_0x230 == 2 || field_0x230 == 3) {
        addCalc2_801F71A4(&mPos.x, field_0x280.x, 0.4f, 5.0f);
        addCalc2_801F71A4(&mPos.y, field_0x280.y, 0.4f, 5.0f);
        if (field_0x3c.squared() < 0.01f) {
            field_0x3c.set(0.0f, 0.0f, 0.0f);
        }
    }

    if (field_0x230 == 2 || field_0x230 == 3) {
        if (fn_801F2B7C(0, -1, 1)) {
            if (field_0x230 == 3) {
                field_0x234 = 30;
                field_0x230 = 2;
            }
            return;
        }
    }

    switch (field_0x230) {
    case 0:
        if (field_0x234 <= 0) {
            field_0x234 = 8;
            field_0x25c = 64.0f;
            field_0x230 = 1;
            return;
        }
        field_0x298.x = field_0x280.x - mPos.x;
        field_0x298.y = field_0x280.y - mPos.y;
        if (PSVECMag(&field_0x298) > 5.0f) {
            field_0x298.normalize();
            field_0x298.scale(5.0f);
        }
        break;
    case 1:
        if (field_0x244 >= 0 && PSVECDistance(&field_0x280, &mPos) < 8.0f) {
            field_0x234 = 0;
            field_0x230 = 2;
        } else {
            f32 maxSpeed = 5.0f;
            if (PSVECDistance(&mPos, &field_0x268) > 96.0f) {
                maxSpeed = field_0x258;
            }
            field_0x298.x = field_0x280.x - mPos.x;
            field_0x298.y = field_0x280.y - mPos.y;
            if (PSVECMag(&field_0x298) > maxSpeed) {
                field_0x298.normalize();
                field_0x298.scale(maxSpeed);
            }
            if (field_0x268.x > mPos.x) {
                field_0x248 = 0;
            } else {
                field_0x248 = 2;
            }
            if (field_0x234 <= 0) {
                field_0x234 = 8;
            }
        }
        break;
    case 2:
    case 3:
        if (field_0x244 < 0) {
            field_0x230 = 1;
        } else {
            if (PSVECDistance(&mPos, &field_0x268) < 128.0f) {
                field_0x298.x = field_0x280.x - mPos.x;
                field_0x298.y = field_0x280.y - mPos.y;
                field_0x298.normalize();
                field_0x298.scale(5.0f);
            } else {
                field_0x230 = 1;
            }
            if (field_0x230 == 3) {
                if (field_0x25c > 0.1f) {
                    field_0x25c -= 1.0f;
                    if (field_0x25c <= 0.1f) {
                        field_0x25c = 0.1f;
                    }
                }
            } else {
                if (field_0x25c < 64.0f) {
                    field_0x25c += 1.0f;
                    if (field_0x25c >= 64.0f) {
                        field_0x25c = 64.0f;
                    }
                }
            }
            if (field_0x268.x > mPos.x) {
                field_0x248 = 0;
            } else {
                field_0x248 = 2;
            }
        }
        if (field_0x234 <= 0) {
            field_0x234 = 8;
        }
        break;
    case 4:
        if (fn_801F6758(&mPos, field_0x4, 16.0f) != 0 && fn_801F6758(&mPos, field_0x4, 16.0f) == 1) {
            field_0x262 = 1;
        }
        break;
    }

    field_0x238--;
    if (field_0x238 <= 0) {
        field_0x23c = (field_0x23c + 1) & 1;
        field_0x238 = 2;
    }
    if (field_0x234 > 0) {
        field_0x234--;
    }
}

// FIXME(shotgun): 97.29 %, scheduling+regalloc
void BeeFormation_804AC7B8::draw() {
    SpriteParams_801F5AE0 params;
    params.field_0x1c = 1;
    params.field_0x1d = 1;
    drawSprite_801F06F0("enemy_gc_sob.bin", field_0x23c + field_0x248 + 0x2C5, &params, 1, 1.0f, 1.0f);
}

BeeFormation_804AC7B8::~BeeFormation_804AC7B8() {}

