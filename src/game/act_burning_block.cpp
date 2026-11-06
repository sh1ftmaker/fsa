#include "game/GameAudio.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/BgRewrite.h"
#include "game/Map.h"
#include "game/Player.h"
#include "game/act/act_burning_block.h"

// Offsets (x, y) from mPos probed for the pushed block, per direction (0x8046C398).
static const f32 lbl_8046C398[4][2] = {
    {10.0f, 0.0f},
    {-10.0f, 0.0f},
    {0.0f, 10.0f},
    {0.0f, -10.0f},
};

// Velocity given to the block, per direction (0x8046C3B8).
static const f32 lbl_8046C3B8[4][3] = {
    {0.5f, 0.0f, 0.0f},
    {-0.5f, 0.0f, 0.0f},
    {0.0f, 0.5f, 0.0f},
    {0.0f, -0.5f, 0.0f},
};

void BurningBlockWoodenCrate_804A7B90::init() {
    mPos.x += (s32)(8.0f - (f32)((s32)mPos.x % 8));
    mPos.y += (s32)(8.0f - (f32)((s32)mPos.y % 8));
    field_0x3c.set(0.0f, 0.0f, 0.0f);
    mSpawnPos = mPos;
    field_0x230 = mSpawnPos;
    mBox.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x80.set(-8.0f, -8.0f, 8.0f, 8.0f);
    field_0x1a4 = 1;
    field_0x23c = 0;
    field_0xb8 = 2;
    field_0x240 = fn_80226850(&mPos, field_0x4);
    gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x159);
}

void BurningBlockWoodenCrate_804A7B90::update() {
    JGeometry::TBox2f box(mBox);
    box.i.x += mPos.x;
    box.i.y += mPos.y;
    box.f.x += mPos.x;
    box.f.y += mPos.y;
    fn_80239914(&box, field_0x4, mSerial);
    if (PSVECDistance(&mPos, &field_0x230) >= 15.9f) {
        if (mPos.x <= 15.5f + mSpawnPos.x && mPos.x >= mSpawnPos.x - 15.5f) {
            mPos.x = mSpawnPos.x;
        } else {
            mPos.x = (s32)(((s32)mPos.x + 4) / 8) * 8;
        }
        if (mPos.y <= 15.5f + mSpawnPos.y && mPos.y >= mSpawnPos.y - 15.5f) {
            mPos.y = mSpawnPos.y;
        } else {
            mPos.y = (s32)(((s32)mPos.y + 4) / 8) * 8;
        }
        field_0x230 = mPos;
        field_0x3c.set(0.0f, 0.0f, 0.0f);
        mPrevPos = mPos;
        field_0x23c = 8;
        field_0x240 = fn_80226850(&mPos, field_0x4);
        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, 0x159);
    } else {
        mPos += field_0x3c;
        if (field_0x23c > 0) {
            field_0x23c--;
        } else if (field_0x3c.isZero()) {
            if (fn_80226850(&mPos, field_0x4) != 0x159) {
                switch (field_0x90 & 0xFF) {
                case 1:
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 2, -1, -1);
                    break;
                case 2:
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 4, -1, -1);
                    break;
                case 3:
                    getActorManager_801F856C()->spawnActor_801F9484(ACTOR_RUPY, field_0x4, &mPos, 7, -1, -1);
                    break;
                }
                mAlive = false;
            } else if (fn_80239108(mSerial) == 1) {
                s32 dir = fn_80238BC4(mSerial);
                if (dir >= 0 && dir < 4) {
                    JGeometry::TVec2f probe(lbl_8046C398[dir][0], lbl_8046C398[dir][1]);
                    probe.x += mPos.x;
                    probe.y += mPos.y;
                    if (!fn_802260DC(&probe, field_0x4, false)) {
                        fn_80238E08(mSerial);
                        field_0x3c.set(lbl_8046C3B8[dir][0], lbl_8046C3B8[dir][1],
                                       lbl_8046C3B8[dir][2]);
                        field_0x230.x = mPos.x;
                        field_0x230.y = mPos.y;
                        field_0x230.z = mPos.z;
                        startSound_801F0E34(0x30004, 0);
                        gBgRewrite_8053AC98->fn_802A3948(&mPos, 0, field_0x4, field_0x240);
                    }
                }
            }
        }
    }
}

void BurningBlockWoodenCrate_804A7B90::draw() {
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x17e, NULL, 0, 1.0f, 1.0f);
}

void BurningBlockWoodenCrate_804A7B90::vfunc_0x24() {
    mAlive = false;
}

BurningBlockWoodenCrate_804A7B90::~BurningBlockWoodenCrate_804A7B90() {}
