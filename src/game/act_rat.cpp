// Actor RAT ("Rat" in docs/names/actors.tsv): the spawner (fn_801F9484)
// allocates 0x248 bytes for it (0x801FBE30) and calls its constructor
// fn_80202EEC, which stores the vtable 0x8049C400.

#include "game/GameAudio.h"
#include "game/ActorBase.h"

// Frame offsets per animation step.
static u8 lbl_80539E68[] = {0, 3, 1, 2};

// Named after its vtable (0x8049C400).
class Rat_8049C400 : public ActorBase_804A5770 {
public:
    Rat_8049C400() {}
    virtual ~Rat_8049C400();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    /* 0x230 */ s32 field_0x230;
    /* 0x234 */ u8 mDir;  // draw: 2 frames per step
    /* 0x235 */ u8 mStep; // draw: index into lbl_80539E68
    /* 0x238 */ s32 field_0x238;
    /* 0x23C */ s32 field_0x23c;
    /* 0x240 */ s32 mFrameBase; // draw: added to every frame
    /* 0x244 */ s32 field_0x244;
}; // Size: 0x248

// FIXME: update (0x800F3E08) and init (0x800F43D0) are not written yet:
// init draws a random number through the TRandom_fast_ at
// lbl_8053AAF8->0xB4, which GameGlobals.h does not declare yet (G1 item 2).

Rat_8049C400::~Rat_8049C400() {}

// Drawn 1.2 times larger when the spawn parameter (0x90) is nonzero.
void Rat_8049C400::draw() {
    SpriteParams_801F5AE0 params;
    Mtx mtx;
    mtx[0][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][3] = 0.0f;
    if (field_0x90 != 0) {
        mtx[0][0] = 1.2f;
        mtx[0][1] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 1.2f;
        mtx[1][2] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[2][2] = 1.2f;
        params.mMtx = mtx;
    }
    // The direction's block, then the walk cycle's frame in it.
    fn_801F06F0("enemy_gc_sob.bin", mFrameBase + mDir * 2 + (lbl_80539E68[mStep] * 4 + 0xA1),
                &params, 0, 1.0f, 1.0f);
}
