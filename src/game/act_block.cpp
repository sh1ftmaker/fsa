#include "game/GameAudio.h"
#include "game/act/act_block.h"
#include "game/BgRewrite.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/Player.h"

static const f32 lbl_8046C120[4][3] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

static const f32 lbl_8046C150[4][3] = {
    {1.0f, 0.0f, 0.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, -1.0f, 0.0f},
};

static const f32 lbl_8046C180[4][3] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

static const f32 lbl_8046C1B0[4][3] = {
    {20.0f, 0.0f, 0.0f},
    {-20.0f, 0.0f, 0.0f},
    {0.0f, 20.0f, 0.0f},
    {0.0f, -20.0f, 0.0f},
};

static const f32 lbl_8046C1E0[4][3] = {
    {16.0f, 0.0f, 0.0f},
    {-16.0f, 0.0f, 0.0f},
    {0.0f, 16.0f, 0.0f},
    {0.0f, -16.0f, 0.0f},
};

static f32 lbl_804A79E0[4][3] = {
    {12.0f, 0.0f, 0.0f},
    {-12.0f, 0.0f, 0.0f},
    {0.0f, 12.0f, 0.0f},
    {0.0f, -12.0f, 0.0f},
};

// FIXME: 97.48%. Same length; the int-to-float conversion of the first position is scheduled differently (stw/lis/stfd order)
// and one table load is lfsx instead of lfs (class: different + reloc-name).
void Block_804A7A10::init() {
    field_0x244 = 0;
    s32 type = field_0x90 & 3;
    if (type == 0) {
        mPos.x = (s32)mPos.x / 16 * 16 + 8;
        mPos.y = (s32)mPos.y / 16 * 16 + 8;
        u8 dir = (field_0x90 >> 2) & 3;
        field_0x3c.set(lbl_8046C120[dir][0], lbl_8046C120[dir][1], lbl_8046C120[dir][2]);
        JGeometry::TVec3f checkPos = field_0x3c;
        checkPos.normalize();
        checkPos.scale(16.0f);
        checkPos += mPos;
        if (fn_8022461C(&checkPos, field_0x4) == 0x50) {
            field_0x3c.normalize();
            field_0x3c.scale(1.0f);
        }
    } else {
        if (!((field_0x90 >> 9) & 1)) {
            if (fn_8028D4DC(&mPos, type, field_0x16c)) {
                field_0x244 = 1;
                vfunc_0x30(0xFF, true);
            }
        }
        field_0x3c.set(0.0f, 0.0f, 0.0f);
    }

    field_0x230 = mPos;
    field_0x248 = 0;
    field_0x24c = 1;

    type = field_0x90 & 3;
    if (type == 0) {
        mBox.set(-7.5f, -7.5f, 7.5f, 7.5f);
        field_0x80.set(-7.5f, -7.5f, 7.5f, 7.5f);
        field_0x1a4 = 1;
        field_0x24c = 0;
    } else if (type == 1) {
        mBox.set(-15.5f, -15.5f, 15.5f, 15.5f);
        field_0x80.set(-15.5f, -15.5f, 15.5f, 15.5f);
        field_0x1a4 = 2;
    } else if (type == 2) {
        mBox.set(-23.5f, -23.5f, 23.5f, 23.5f);
        field_0x80.set(-23.5f, -23.5f, 23.5f, 23.5f);
        field_0x1a4 = 3;
    } else if (type == 3) {
        mBox.set(-31.5f, -31.5f, 31.5f, 31.5f);
        field_0x80.set(-31.5f, -31.5f, 31.5f, 31.5f);
        field_0x1a4 = 4;
    }

    mPrevPos = mPos;
    JGeometry::TVec3f saved3c = field_0x3c;
    if (!fn_8028C518((field_0x90 >> 2) & 3)) {
        mAlive = false;
    } else {
        mPrevPos = mPos;
        if ((field_0x90 & 3) == 0) {
            field_0x3c.x = saved3c.x;
            field_0x3c.y = saved3c.y;
            field_0x3c.z = saved3c.z;
            gBgRewrite_8053AC98->fn_8029E9EC(&mPos, field_0x4, ((field_0x90 >> 4) & 7) + 0x21, 0, (field_0x90 >> 2) & 3);
            startSound_801F0E34(0x30004, 0);
        }
        field_0x23c = 0;
        field_0x240 = 0;
        field_0xb8 = 2;
        field_0x24d = 0;
        field_0x24e = 0;
        if ((field_0x90 & 3) != 0) {
            field_0x24e = (field_0x90 >> 10) & 1;
        }
    }
}

// FIXME: 99.92%. reloc-name: all 61 instructions identical, differing only in compiler literal label names
u8 fn_8028D4DC(JGeometry::TVec3f* pos, s32 index, s32 layer) {
    u8 viewVal = getGameView_802D6A9C()->fn_802D7544(layer);
    if (viewVal == 0) {
        return 0;
    }
    u8 dir = viewVal - 1;
    JGeometry::TVec3f offset;
    offset.x = (index + 1) * lbl_8046C150[dir][0] * 16.0f;
    offset.y = (index + 1) * lbl_8046C150[dir][1] * 16.0f;
    offset.z = 0.0f;
    *pos += offset;
    return 1;
}

bool Block_804A7A10::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    if ((s32)fn_8022461C(pos, layer) == 0xD) {
        return false;
    }
    return ActorBase_804A5770::vfunc_0x4C(layer, pos);
}

// FIXME: 95.46%. missing/extra + reloc-name: ours has 4 more instructions (812 vs 808). The target compares the (field_0x90 & 3)
// based indices with signed cmpwi and converts them with xoris (signed), ours uses cmplwi / unsigned; the (u16) tile result
// is extended in ours (clrlwi) where the target moves it; some stack temporaries sit at other offsets.
void Block_804A7A10::update() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    box.i.x -= 1.0f;
    box.f.x += 1.0f;
    box.i.y -= 1.0f;
    box.f.y += 1.0f;
    fn_80239914(&box, field_0x4, mSerial);
    box.i.x -= -1.0f;
    box.i.y -= -1.0f;
    box.f.x += -1.0f;
    box.f.y += -1.0f;
    fn_8020D58C()->addBox(mSerial);

    if ((field_0x90 & 3) == 0) {
        mPos += field_0x3c;
        JGeometry::TVec3f checkPos = field_0x3c;
        checkPos.normalize();
        checkPos.scale(16.0f);
        checkPos += mPos;
        if (PSVECDistance(&checkPos, &field_0x230) >= 16.0f) {
            s32 tile1 = fn_8022461C(&checkPos, field_0x4);
            s32 tile2 = fn_8022461C(&mPos, field_0x4);
            if ((u16)tile2 == 0x50 && ((u16)tile1 == 0 || (u16)tile1 == 0x50)) {
                JGeometry::TVec3f savedVel = field_0x3c;
                if (fn_8028C518((field_0x90 >> 2) & 3)) {
                    field_0x3c = savedVel;
                    field_0x230.x = mPos.x;
                    field_0x230.y = mPos.y;
                    field_0x230.z = mPos.z;
                    field_0x230.x = (s32)(((s32)field_0x230.x + 4) / 8) * 8;
                    field_0x230.y = (s32)(((s32)field_0x230.y + 4) / 8) * 8;
                    return;
                }
            }
            vfunc_0x24();
        } else {
            JGeometry::TBox2f box2(mBox);
            box2.i.x += mPos.x;
            box2.f.x += mPos.x;
            box2.i.y += mPos.y;
            box2.f.y += mPos.y;
            s32 player = fn_80239040(mSerial);
            for (s32 i = 0; i < 4; i++) {
                if (player != i && !fn_8022E438(player, i) && fn_8023B190(i, &box2, field_0x4)) {
                    JGeometry::TVec3f pPos = *getPlayerPos_8023E724(i);
                    JGeometry::TVec3f diff = pPos - checkPos;
                    if (JGeometry::TVec2f(diff.x, diff.y).length() < 8.0f) {
                        fn_80238548(i, 0, (const JGeometry::TVec2f*)&mPos, 4);
                        break;
                    }
                }
            }
        }
    } else if (field_0x244 == 0) {
        f32 threshold;
        if (!((field_0x90 >> 11) & 1)) {
            f32 dist = PSVECDistance(&mPos, &mSpawnPos);
            if (dist < 13.0f) {
                threshold = 11.9f;
            } else if (dist >= (f32)(((field_0x90 & 3) + 1) * 16) - 19.9f) {
                threshold = 19.9f;
            } else {
                threshold = 15.9f;
            }
        } else {
            threshold = 15.9f;
        }

        if (PSVECDistance(&mPos, &field_0x230) >= threshold) {
            if (mPos.x <= 11.5f + mSpawnPos.x && mPos.x >= mSpawnPos.x - 11.5f) {
                mPos.x = mSpawnPos.x;
            } else {
                mPos.x = fn_8033D4AC(mPos.x, field_0x230.x, threshold);
            }
            if (mPos.y <= 11.5f + mSpawnPos.y && mPos.y >= mSpawnPos.y - 11.5f) {
                mPos.y = mSpawnPos.y;
            } else {
                mPos.y = fn_8033D4AC(mPos.y, field_0x230.y, threshold);
            }
            field_0x230 = mPos;
            field_0x3c.set(0.0f, 0.0f, 0.0f);
            mPrevPos = mPos;
            if (!((field_0x90 >> 7) & 1) && !((field_0x90 >> 8) & 1) &&
                PSVECDistance(&mPos, &mSpawnPos) >= (f32)(((field_0x90 & 3) + 1) * 16) - 0.1f) {
                field_0x23c = -1;
                vfunc_0x30(0xFF, true);
            } else {
                field_0x23c = 8;
                field_0x240 = 30;
            }
        } else {
            mPos += field_0x3c;
            if ((field_0x90 >> 8) & 1) {
                if (PSVECDistance(&mPos, &field_0x230) < 4.0f && field_0x3c.x > 0.0f) {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 12) & 0x1F, true, 0xFF);
                } else {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 12) & 0x1F, false, 0xFF);
                }
                if (PSVECDistance(&mPos, &field_0x230) < 4.0f && field_0x3c.x < 0.0f) {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 17) & 0x1F, true, 0xFF);
                } else {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 17) & 0x1F, false, 0xFF);
                }
                if (PSVECDistance(&mPos, &field_0x230) < 4.0f && field_0x3c.y > 0.0f) {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 22) & 0x1F, true, 0xFF);
                } else {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 22) & 0x1F, false, 0xFF);
                }
                if (PSVECDistance(&mPos, &field_0x230) < 4.0f && field_0x3c.y < 0.0f) {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 27) & 0x1F, true, 0xFF);
                } else {
                    getGameView_802D6A9C()->fn_802D8050((field_0x90 >> 27) & 0x1F, false, 0xFF);
                }
            }

            if (field_0x23c > 0) {
                field_0x23c--;
            } else if (field_0x23c == 0) {
                if (field_0x3c.isZero()) {
                    if (fn_80239108(mSerial) >= (field_0x90 & 3) + 1) {
                        if (((field_0x90 >> 8) & 1) ||
                            PSVECDistance(&mPos, &mSpawnPos) < (f32)(((field_0x90 & 3) + 1) * 16) - 0.1f) {
                            s32 dir = fn_80238BC4(mSerial);
                            JGeometry::TVec3f offset(lbl_804A79E0[dir][0], lbl_804A79E0[dir][1], lbl_804A79E0[dir][2]);
                            JGeometry::TVec3f savedPos = mPos;
                            mPos += offset;
                            if (fn_801F2618(NULL, false)) {
                                mPos = savedPos;
                                return;
                            }
                            mPos = savedPos;
                            if (field_0x24e == 1) {
                                if ((field_0x90 & 3) == 1) {
                                    if ((u8)(dir >> 1) != ((field_0x90 >> 2) & 3)) {
                                        return;
                                    }
                                } else {
                                    if (dir != ((field_0x90 >> 2) & 3)) {
                                        return;
                                    }
                                }
                            }
                            if (!fn_8028C518(dir)) {
                                return;
                            }
                            if (field_0x24c == 1) {
                                if (field_0x248 == 0) {
                                    field_0x248 = dir + 1;
                                } else if (field_0x248 != dir + 1) {
                                    return;
                                }
                            }
                            if (dir >= 0 && dir < 4) {
                                fn_80238E08(mSerial);
                                if ((field_0x90 & 3) != 0 && !fn_8028C364(dir)) {
                                    field_0x248 = 0;
                                    fn_80238D50(mSerial);
                                    return;
                                }
                                if (!((field_0x90 >> 9) & 1)) {
                                    getGameView_802D6A9C()->fn_802D7584(field_0x16c, dir + 1);
                                }
                                field_0x3c.set(lbl_8046C180[dir][0], lbl_8046C180[dir][1], lbl_8046C180[dir][2]);
                                if ((field_0x90 & 3) == 3) {
                                    field_0x3c.scale(0.4f);
                                } else if ((field_0x90 & 3) != 0) {
                                    field_0x3c.scale(0.7f);
                                }
                                if (field_0x24d == 1) {
                                    field_0x24d = 0;
                                } else {
                                    field_0x230.x = mPos.x;
                                    field_0x230.y = mPos.y;
                                    field_0x230.z = mPos.z;
                                }
                                startSound_801F0E34(0x30004, 0);
                            }
                        }
                    } else if ((field_0x90 >> 7) & 1) {
                        if (PSVECDistance(&mPos, &mSpawnPos) > 1.0f) {
                            if (field_0x240 == 0) {
                                field_0x3c.set(0.0f, 0.0f, 0.0f);
                                if (mPos.x > mSpawnPos.x) {
                                    field_0x3c.x = -0.5f;
                                } else if (mPos.x < mSpawnPos.x) {
                                    field_0x3c.x = 0.5f;
                                } else if (mPos.y > mSpawnPos.y) {
                                    field_0x3c.y = -0.5f;
                                } else if (mPos.y < mSpawnPos.y) {
                                    field_0x3c.y = 0.5f;
                                }
                                field_0x230.x = mPos.x;
                                field_0x230.y = mPos.y;
                                field_0x230.z = mPos.z;
                                startSound_801F0E34(0x30004, 0);
                            } else {
                                field_0x240--;
                            }
                        }
                    }
                } else {
                    bool ok;
                    JGeometry::TVec3f vel = field_0x3c;
                    if ((field_0x90 & 3) != 0 && threshold == 19.9f &&
                        PSVECDistance(&mPos, &field_0x230) >= 7.9f &&
                        field_0x248 != 0 &&
                        !fn_8028C518(field_0x248 - 1)) {
                        ok = false;
                    } else {
                        field_0x3c = vel;
                        ok = true;
                    }
                    if (!ok) {
                        field_0x23c = 8;
                        field_0x24d = 1;
                    }
                }
            }
        }
    }
}

// FIXME: 98.28%. scheduling+regalloc: same instructions modulo registers, different order
bool Block_804A7A10::fn_8028C518(u8 dir) {
    bool result = false;
    s32 kind = field_0x90 & 3;
    JGeometry::TVec3f origPos = mPos;

    if (kind == 3) {
        return true;
    }

    if (kind == 0) {
        field_0x3c.set(lbl_8046C1E0[dir][0], lbl_8046C1E0[dir][1], lbl_8046C1E0[dir][2]);
        mPrevPos += field_0x3c;
    }

    field_0x3c.set(lbl_8046C1B0[dir][0], lbl_8046C1B0[dir][1], lbl_8046C1B0[dir][2]);
    mPos += field_0x3c;

    JGeometry::TVec3f v18(0.0f, 0.0f, 0.0f);
    if (fn_801F2618(&v18, false)) {
        switch (dir) {
        case 0:
            if (v18.x >= 0.0f) {
                result = true;
            }
            break;
        case 1:
            if (v18.x <= 0.0f) {
                result = true;
            }
            break;
        case 2:
            if (v18.y >= 0.0f) {
                result = true;
            }
            break;
        case 3:
            if (v18.y <= 0.0f) {
                result = true;
            }
            break;
        }
    } else {
        result = true;
    }

    if (!result) {
        result = true;
        JGeometry::TBox2f box(mBox);
        box.i.x += mPos.x;
        box.f.x += mPos.x;
        box.i.y += mPos.y;
        box.f.y += mPos.y;

        s32 serial;
        for (int i = 0; i < 4; i++) {
            if (fn_8023B0F4(i, &box, field_0x4, false, 0.0f)) {
                if (fn_8022CAF8(i) == 5) {
                    if (fn_8022F478(i)) {
                        continue;
                    }
                }
                serial = mSerial;
                if (fn_80238CB4(i) == serial) {
                    continue;
                }
                result = false;
            }
        }
    }

    mPos = origPos;
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    return result;
}

// FIXME: 99.40%. regalloc-only: target->ours r30->r31, r31->r30
bool Block_804A7A10::fn_8028C364(u8 dir) {
    f32 max;
    f32 min;
    f32 center;
    if (dir <= 1) {
        max = mBox.f.y + mPos.y;
        min = mBox.i.y + mPos.y;
        center = mPos.x;
    } else {
        max = mBox.f.x + mPos.x;
        min = mBox.i.x + mPos.x;
        center = mPos.y;
    }

    int i;
    s32 serial;
    for (i = 0; i < 4; i++) {
        serial = mSerial;
        s32 pushing = fn_80238CB4(i);
        if (pushing != serial) {
            continue;
        }
        if (dir <= 1) {
            f32 y = getPlayerPos_8023E724(i)->y;
            if (y < min && max < y) {
                return false;
            }
            if (dir == 0) {
                if (getPlayerPos_8023E724(i)->x > center) {
                    return false;
                }
            } else {
                if (getPlayerPos_8023E724(i)->x < center) {
                    return false;
                }
            }
        } else {
            f32 x = getPlayerPos_8023E724(i)->x;
            if (x < min && max < x) {
                return false;
            }
            if (dir == 2) {
                if (getPlayerPos_8023E724(i)->y > center) {
                    return false;
                }
            } else {
                if (getPlayerPos_8023E724(i)->y < center) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Block_804A7A10::draw() {
    s32 val = 0;
    SpriteParams_801F5AE0 params;
    s32 kind = field_0x90 & 3;
    if (kind != 0) {
        params.field_0x1d = -1;
        params.field_0x1c = 0;
        drawSprite_801F06F0("enemy_both_sob.bin", kind + 4, &params, 0, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, kind + 4, 0, 0, -1);
    } else {
        s32 v = (field_0x90 >> 4) & 7;
        if (v != 0) {
            val = v + 3;
        }
        drawSprite_801F06F0("enemy_both_sob.bin", val + 0x45, NULL, 0, 1.0f, 1.0f);
        fn_80307AC8(mSerial, field_0x4, &mPos, val + 0x45, 0, 0, -1);
    }
}

void Block_804A7A10::vfunc_0x24() {
    mPos.x = (s32)((s32)mPos.x + 4) / 8 * 8;
    mPos.y = (s32)((s32)mPos.y + 4) / 8 * 8;
    if ((field_0x90 & 3) == 0) {
        gBgRewrite_8053AC98->fn_8029E928(&mPos, field_0x4, 0x20);
    }
    mAlive = false;
}

Block_804A7A10::~Block_804A7A10() {}
