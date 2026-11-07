#include "game/GameAudio.h"
#include "game/act/act_block.h"
#include "game/act/act_colour_block.h"
#include "game/GameGlobals.h"
#include "game/ActorManager.h"
#include "game/SpriteParams.h"
#include "game/Player.h"
#include "game/Map.h"
#include "game/GameView.h"

static const Vec lbl_804750C0[4] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

static const Vec lbl_804750F0[4] = {
    {20.0f, 0.0f, 0.0f},
    {-20.0f, 0.0f, 0.0f},
    {0.0f, 20.0f, 0.0f},
    {0.0f, -20.0f, 0.0f},
};

// FIXME: 99.80. regalloc-only: r6 <-> r7 swap inside the random colour while loop.
void ColourBlock1Player_804AE620::init() {
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    if ((s32)((field_0x90 >> 2) & 7) == 5) {
        u32 mask = 0;
        s32 serial = -1;
        while (true) {
            serial = getActorManager_801F856C()->fn_801F78F4('BCK2', serial);
            if (serial < 0) {
                break;
            }
            if (serial == mSerial) {
                continue;
            }
            ActorBase_804A5770* other = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (!other) {
                continue;
            }
            ColourBlock1Player_804AE620* block = static_cast<ColourBlock1Player_804AE620*>(other);
            if (block->field_0x240 == 0) {
                continue;
            }
            mask |= (1 << block->field_0x240);
        }

        while (true) {
            field_0x240 = (s32)(4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 1.0f);
            if ((s32)(mask & 0x1E) == 0x1E) {
                break;
            }
            if (!((1 << field_0x240) & mask)) {
                break;
            }
        }
    } else {
        field_0x240 = (field_0x90 >> 2) & 7;
    }

    mPos.x = mSpawnPos.x = (f32)(((s32)mPos.x / 16) * 16 + 8);
    mPos.y = mSpawnPos.y = (f32)(((s32)mPos.y / 16) * 16 + 8);
    field_0x230 = mPos;
    mBox.set(-7.5f, -7.5f, 7.5f, 7.5f);
    field_0x80.set(-7.5f, -7.5f, 7.5f, 7.5f);
    field_0x1a4 = 1;
    field_0xb8 = 2;
    field_0x244 = 0;
    field_0x248 = (field_0x90 >> 5) & 1;
    if (field_0x248 == 0) {
        if (fn_8028D4DC(&mPos, 0, field_0x16c)) {
            field_0x244 = 1;
            vfunc_0x30(0xFF, 1);
            field_0x240 = 0;
        }
    }
    if ((field_0x90 >> 8) & 0xF) {
        field_0x244 = 1;
    }
}

// FIXME: 99.12. missing/extra: box expansion float addition scheduling and savedPos stack offset.
void ColourBlock1Player_804AE620::update() {
    JGeometry::TBox2f box = mBox;
    box.i.x += mPos.x;
    box.f.x += mPos.x;
    box.i.y += mPos.y;
    box.f.y += mPos.y;

    if (field_0x244 != 0) {
        box.i.x -= 2.0f;
        box.i.y -= 2.0f;
        box.f.x += 2.0f;
        box.f.y += 2.0f;
    } else {
        box.i.x -= 1.0f;
        box.i.y -= 1.0f;
        box.f.x += 1.0f;
        box.f.y += 1.0f;
    }

    fn_80239914(&box, field_0x4, mSerial);

    if (field_0x244 != 0) {
        box.i.x -= -2.0f;
        box.i.y -= -2.0f;
        box.f.x += -2.0f;
        box.f.y += -2.0f;
    } else {
        box.i.x -= -1.0f;
        box.i.y -= -1.0f;
        box.f.x += -1.0f;
        box.f.y += -1.0f;
    }

    fn_8020D58C()->addBox(mSerial);

    if (field_0x244 != 0) {
        mPos.x = (f32)(((s32)mPos.x + 4) / 8 * 8);
        mPos.y = (f32)(((s32)mPos.y + 4) / 8 * 8);
        return;
    }

    if (PSVECDistance(&mPos, &field_0x230) >= 15.9f) {
        if (!fn_801F2618(NULL, false)) {
            JGeometry::TVec3f probe = field_0x3c;
            probe.normalize();
            probe.scale(16.0f);
            probe += mPos;

            u16 tileA = fn_8022461C(&probe, field_0x4);
            u16 tileB = fn_8022461C(&mPos, field_0x4);
            if (tileB == 0x50 && (tileA == 0 || tileA == 0x50)) {
                JGeometry::TVec3f savedSpeed = field_0x3c;
                if (fn_80374964(field_0x90 & 3) == 1) {
                    field_0x3c = savedSpeed;
                    field_0x230.x = mPos.x;
                    field_0x230.y = mPos.y;
                    field_0x230.z = mPos.z;
                    field_0x230.x = (f32)(((s32)field_0x230.x + 4) / 8 * 8);
                    field_0x230.y = (f32)(((s32)field_0x230.y + 4) / 8 * 8);
                    return;
                }
            }
        }

        if (mPos.x <= mSpawnPos.x + 15.5f && mPos.x >= mSpawnPos.x - 15.5f) {
            mPos.x = mSpawnPos.x;
        } else {
            mPos.x = (f32)(((s32)mPos.x + 4) / 8 * 8);
        }
        if (mPos.y <= mSpawnPos.y + 15.5f && mPos.y >= mSpawnPos.y - 15.5f) {
            mPos.y = mSpawnPos.y;
        } else {
            mPos.y = (f32)(((s32)mPos.y + 4) / 8 * 8);
        }

        field_0x230 = mPos;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        mPrevPos = mPos;
        vfunc_0x30(0xFF, 1);
        field_0x240 = 0;
        field_0x244 = 1;
        return;
    }

    JGeometry::TVec3f savedPos = mPos;
    mPos += field_0x3c;
    if (!field_0x3c.isZero()) {
        if (field_0x9c >= 0) {
            mPos = savedPos;
        }
        return;
    }

    if (fn_80239108(mSerial) >= 1) {
        if (PSVECDistance(&mPos, &mSpawnPos) < 15.9f) {
            s32 dir = fn_80238BC4(mSerial);
            savedPos = mPos;
            JGeometry::TVec3f offset(lbl_804A79E0[dir][0], lbl_804A79E0[dir][1], lbl_804A79E0[dir][2]);
            if (field_0x240 == 0 || fn_80239558(&box, field_0x4, mSerial) == field_0x240 - 1) {
                if (dir == (s32)(field_0x90 & 3)) {
                    if (fn_80374964(dir)) {
                        if (field_0x248 == 0) {
                            getGameView_802D6A9C()->fn_802D7584(field_0x16c, dir + 1);
                        }
                        mPos += offset;
                        if (fn_801F2618(NULL, false)) {
                            mPos = savedPos;
                            return;
                        }
                        mPos = savedPos;
                        if (dir >= 0 && dir < 4) {
                            fn_80238E08(mSerial);
                            field_0x3c.x = lbl_804750C0[dir].x;
                            field_0x3c.y = lbl_804750C0[dir].y;
                            field_0x3c.z = lbl_804750C0[dir].z;
                            JGeometry::TVec3f probe = field_0x3c;
                            probe.normalize();
                            probe.scale(16.0f);
                            probe += mPos;
                            if (fn_8022461C(&probe, field_0x4) == 0x50) {
                                field_0x3c.normalize();
                                field_0x3c.scale(1.0f);
                            }
                            field_0x230.x = mPos.x;
                            field_0x230.y = mPos.y;
                            field_0x230.z = mPos.z;
                            startSound_801F0E34(0x30004, 0);
                        }
                    }
                }
            }
        }
    }
}

// FIXME: 95.88. scheduling+regalloc: loop counter register and box float addition scheduling.
bool ColourBlock1Player_804AE620::fn_80374964(u8 dir) {
    bool canMove = false;
    JGeometry::TVec3f savedPos = mPos;
    field_0x3c.x = lbl_804750F0[dir].x;
    field_0x3c.y = lbl_804750F0[dir].y;
    field_0x3c.z = lbl_804750F0[dir].z;
    mPos += field_0x3c;
    JGeometry::TVec3f probe(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&probe, false)) {
        switch (dir) {
        case 0:
            if (probe.x >= 0.0f) {
                canMove = true;
            }
            break;
        case 1:
            if (probe.x <= 0.0f) {
                canMove = true;
            }
            break;
        case 2:
            if (probe.y >= 0.0f) {
                canMove = true;
            }
            break;
        case 3:
            if (probe.y <= 0.0f) {
                canMove = true;
            }
            break;
        }
    } else {
        canMove = true;
    }
    if (!canMove) {
        canMove = true;
        JGeometry::TBox2f box = mBox;
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;
        for (int i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                if (fn_8022CAF8(i) != 5 || !fn_8022F478(i)) {
                    s32 serial = mSerial;
                    if (fn_80238CB4(i) != serial) {
                        canMove = false;
                    }
                }
            }
        }
    }
    mPos = savedPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    return canMove;
}

void ColourBlock1Player_804AE620::draw() {
    SpriteParams_801F5AE0 params;
    s32 frame = 0;
    if (field_0x240 != 0) {
        frame = field_0x240 + 3;
    }
    params.field_0x1d = -1;
    drawSprite_801F06F0("enemy_both_sob.bin", frame + 0x45, &params, 0, 1.0f, 1.0f);
    fn_80307AC8(mSerial, field_0x4, &mPos, frame + 0x45, 0, 0, -1);
}

ColourBlock1Player_804AE620::~ColourBlock1Player_804AE620() {}
