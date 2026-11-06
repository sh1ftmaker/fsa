// The actor base class (include/game/ActorBase.h) and the code around it.

#include "game/ActorBase.h"
#include "game/ActorId.h"
#include "game/ActorManager.h"
#include "game/EmitterDelete.h"
#include "game/EnemyBase.h"
#include "game/GameAudio.h"
#include "game/GameGlobals.h"
#include "game/GameView.h"
#include "game/Map.h"
#include "game/GameState.h"
#include "game/StageFlag.h"
#include "game/ViewLayer.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "std/math.h"

// Drop chance per EnemyParam "dasu" (fn_801F52A4).
static const f32 lbl_80469D48[12] = {
    1.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
};

// The drops of fn_801F52A4 (its switch): ids 'HART' / 'RUPY', count and
// spawn parameter (_P<n>: parameter n, the rupee kind; a trailing number:
// the count; 20_20: twenty of parameter 0 and twenty of parameter 1).
enum DropKind_801F52A4 {
    DROP_NONE,
    DROP_HEART,
    DROP_HEART_2,
    DROP_RUPEE,
    DROP_RUPEE_P1,
    DROP_RUPEE_P2,
    DROP_RUPEE_P3,
    DROP_RUPEE_P4,
    DROP_RUPEE_P5,
    DROP_RUPEE_5,
    DROP_RUPEE_10,
    DROP_RUPEE_P1_5,
    DROP_RUPEE_P1_10,
    DROP_RUPEE_20_20,
};

// Eight equally likely drops per "dasu" (fn_801F52A4's switch).
static const s32 lbl_80469D78[12][8] = {
    {DROP_NONE, DROP_NONE, DROP_NONE, DROP_NONE, DROP_NONE, DROP_NONE, DROP_NONE, DROP_NONE},
    {DROP_HEART, DROP_HEART, DROP_HEART, DROP_HEART, DROP_RUPEE, DROP_HEART, DROP_HEART, DROP_RUPEE},
    {DROP_HEART_2, DROP_HEART_2, DROP_HEART_2, DROP_HEART_2, DROP_HEART, DROP_HEART_2, DROP_HEART_2, DROP_HEART},
    {DROP_RUPEE_P1, DROP_RUPEE, DROP_RUPEE_P1, DROP_RUPEE_P2, DROP_RUPEE_P1, DROP_RUPEE, DROP_RUPEE_P1, DROP_RUPEE_P1},
    {DROP_RUPEE_P2, DROP_RUPEE_P1, DROP_RUPEE_P2, DROP_RUPEE_P1, DROP_RUPEE_P2, DROP_HEART, DROP_RUPEE_P2, DROP_RUPEE},
    {DROP_RUPEE_P4, DROP_RUPEE_P4, DROP_RUPEE_P4, DROP_RUPEE_P5, DROP_RUPEE_P4, DROP_RUPEE_P4, DROP_RUPEE_P5, DROP_RUPEE_P4},
    {DROP_HEART, DROP_HEART, DROP_HEART_2, DROP_RUPEE_P1, DROP_HEART, DROP_HEART, DROP_HEART, DROP_RUPEE},
    {DROP_RUPEE_P1, DROP_RUPEE, DROP_HEART, DROP_RUPEE_5, DROP_RUPEE, DROP_RUPEE_5, DROP_RUPEE, DROP_HEART},
    {DROP_HEART, DROP_HEART_2, DROP_RUPEE_P2, DROP_RUPEE, DROP_RUPEE_P1_5, DROP_HEART, DROP_RUPEE_P2, DROP_RUPEE_P1_5},
    {DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10, DROP_RUPEE_10},
    {DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10, DROP_RUPEE_P1_10},
    {DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20, DROP_RUPEE_20_20},
};

// Hurt sound ids indexed by EnemyParam "voic" - 1 (fn_801F37AC, while the
// actor has life left).
static const u32 lbl_80469EF8[16] = {
    0x000400AE, 0x000400A7, 0x000400AC, 0x00040081, 0x00040082, 0x0004008C, 0x0004007A, 0x00040095,
    0x00040098, 0x0004009D, 0x0004009A, 0x00040014, 0x00040071, 0x00040033, 0x000400AE, 0x000400AE,
};

// Sound ids indexed by EnemyParam "voic" - 1 (fn_801F4694).
static const u32 lbl_80469F38[16] = {
    0x000400AF, 0x000400A8, 0x000400AD, 0x00040083, 0x00040084, 0x0004008D, 0x0004007B, 0x00040096,
    0x00040099, 0x0004009E, 0x0004009B, 0x000400AF, 0x00040072, 0x00040038, 0x000400A1, 0x00040037,
};

// Shadow scale per kind (fn_801EF830).
static const f32 lbl_80469FA8[8] = {1.0f, 1.0f, 1.0f, 2.0f, 3.0f, 4.0f, 1.5f, 1.0f};

// Five floats indexed by field_0xc0 (EnemyParam "efsz").
static const f32 lbl_8046A00C[5] = {
    0.0f, 1.0f, 1.3f, 1.6f, 2.0f,
};

// Tiles without a shadow (fn_801EF830; the last one only unless
// fn_802DEB58()).
static const u16 lbl_8053F730[4] = {2, 3, 14, 0};
// Frame offsets of the animated tile-0x39 shadow (fn_801EF830).
static const u8 lbl_8053F738[4] = {0, 1, 2, 1};

// fn_803037D4(player, value) for each player that takes part (field_0x82),
// is near the actor (fn_8022BC38) and passes fn_8030381C.
static inline void raisePlayersNear(const ActorBase_804A5770* actor, u32 value) {
    for (s32 i = 0; i < 4; i++) {
        if (fn_8022BC38(i, actor->getField_0x4(), &actor->mPos) && fn_8030381C(i) &&
            (gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)))
        {
            fn_803037D4(i, value);
        }
    }
}

// The box `ofs` (relative to pos) in top-down screen space (x, y - z).
static inline void toScreenBox(JGeometry::TBox2f* out, const JGeometry::TBox2f& ofs,
                               const Vec& pos) {
    out->set(ofs.i.x + pos.x, ofs.i.y + pos.y - pos.z, ofs.f.x + pos.x, ofs.f.y + pos.y - pos.z);
}

// forward declarations (functions reversed for -inline deferred)
void releaseEmitter_801EE434(JPABaseEmitter* emitter);

// FIXME: the float literals (0, 1, -8, 8, 40) pair with the target's .sdata2
// only once the unit's other functions fix its order. 40.0f also needs
// lbl_8053F7F8's size in symbols.txt corrected (0x8 there: 40.0f plus a zero
// word that is probably padding) before a 4-byte literal can pair with it.
ActorBase_804A5770::ActorBase_804A5770()
    : field_0x4(0),
      field_0x8(0),
      mPos(0.0f, 0.0f, 0.0f),
      mPrevPos(0.0f, 0.0f, 0.0f),
      mSpawnPos(0.0f, 0.0f, 0.0f),
      field_0x30(0.0f, 0.0f, 0.0f),
      field_0x3c(0.0f, 0.0f, 0.0f),
      field_0x48(1.0f, 1.0f, 1.0f),
      field_0x54(1.0f, 1.0f, 1.0f),
      mBox(-8.0f, -8.0f, 8.0f, 8.0f),
      mBox2(0.0f, 0.0f, 0.0f, 0.0f),
      field_0x80(0.0f, 0.0f, 0.0f, 0.0f),
      field_0x90(0),
      field_0x94(0),
      field_0x98(0),
      field_0x9c(-1),
      field_0xa0(-1),
      field_0xa4(0),
      field_0xa8(0),
      field_0xac(0),
      field_0xb0(0),
      field_0xb4(0),
      field_0xb8(0),
      field_0xbc(1),
      field_0xc0(1),
      field_0xc4(0),
      field_0xc8(-1),
      field_0xcc(0),
      field_0xd0(-1),
      field_0xd4(0),
      field_0xd5(0),
      field_0xd8(0),
      field_0xdc(0x18),
      field_0xe0(0),
      field_0xe4(0),
      field_0xe8(0),
      field_0xec(0.0f, 0.0f, 0.0f),
      field_0xf8(0),
      field_0xfc(0),
      mEmitter100(NULL),
      mEmitter104(NULL),
      mLife(4),
      field_0x10c(-1),
      field_0x110(-1),
      field_0x114(0),
      field_0x118(0),
      mAlive(true),
      mDrawEnabled(true),
      field_0x11e(0),
      field_0x11f(0),
      mSpriteParams(),
      mListPrev(NULL),
      mListNext(NULL),
      field_0x164(0),
      mInList(0),
      field_0x168(0),
      field_0x16c(-1),
      field_0x174(0),
      field_0x178(0),
      field_0x17c(0),
      field_0x17d(0),
      field_0x180(20),
      field_0x184(40.0f),
      field_0x188(-1),
      field_0x18c(0),
      field_0x190(0),
      field_0x194(0),
      field_0x195(0),
      field_0x196(0),
      mSerial(-1),
      mSlot(0),
      mActorId('NULL'),
      field_0x1a4(0),
      field_0x1a8(false)
{
    for (int i = 0; i < 4; i++) {
        field_0x1ac[i].set(0.0f, 0.0f, 0.0f);
    }
}

ActorBase_804A5770::~ActorBase_804A5770() {
    releaseEmitter_801EE434(mEmitter100);
    releaseEmitter_801EE434(mEmitter104);
    getActorManager_801F856C()->fn_801F8448(this);
}

void ActorBase_804A5770::init() {}

void ActorBase_804A5770::vfunc_0x10() {}

void ActorBase_804A5770::update() {}

// FIXME: float registers differ (the player's z lands in f7, the target uses
// f5; the overlap box's max in f5/f6 instead of f6/f7); the rest matches.
void ActorBase_804A5770::vfunc_0x18() {
    if (mPos.z <= 16.0f && mBox.getWidth() > 0.0f && mBox.getHeight() > 0.0f) {
        JGeometry::TBox2f box;
        toScreenBox(&box, mBox, mPos);
        for (int i = 0; i < 4; i++) {
            if (fn_8023F264(i) && getField_0x4() == getPlayerLayer_8023DE58(i)) {
                JGeometry::TBox2f playerBox;
                toScreenBox(&playerBox, JGeometry::TBox2f(-12.0f, -28.0f, 12.0f, 12.0f),
                            *getPlayerPos_8023E724(i));
                JGeometry::TBox2f overlap = box;
                if (overlap.intersect(playerBox)) {
                    fn_802397B4(&box, getField_0x4(), mSerial);
                    return;
                }
            }
        }
    }
}

void ActorBase_804A5770::savePrevPos() {
    mPrevPos = mPos;
}

void ActorBase_804A5770::draw() {}

// FIXME: 99.1. Register allocation only (r4-r11 in the switch prologue)
// besides the data: the original's array initialisers are six separate
// 4-byte objects (lbl_8053F728/72C 'NULL' at the very start of .sdata2,
// lbl_80543F40-4C zero in .sbss2); MWCC makes one 8-byte @NNN per array here.
// Drops items when the actor dies: with the chance lbl_80469D48[dasu]
// (EnemyParam "dasu", field_0x18c), one of the eight drops in row dasu of
// lbl_80469D78 (0 none, 1-2 hearts, 3-13 rupees: count and kind), spawned
// at the actor's top-down position; several of one kind are fanned out 16
// units around it.
void ActorBase_804A5770::fn_801F52A4() {
    s32 dasu = field_0x18c;
    if (gGameRoot_8053AAF8->mRandom.get_ufloat_1() < lbl_80469D48[dasu]) {
        u32 ids[2] = {'NULL', 'NULL'};
        u32 params[2] = {0, 0};
        u32 counts[2] = {0, 0};
        switch (lbl_80469D78[dasu][(s32)(8.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1())]) {
        case DROP_NONE:
            break;
        case DROP_HEART:
            ids[0] = 'HART';
            counts[0] = 1;
            break;
        case DROP_HEART_2:
            ids[0] = 'HART';
            counts[0] = 2;
            break;
        case DROP_RUPEE:
            ids[0] = 'RUPY';
            counts[0] = 1;
            break;
        case DROP_RUPEE_P1:
            ids[0] = 'RUPY';
            counts[0] = 1;
            params[0] = 1;
            break;
        case DROP_RUPEE_P2:
            ids[0] = 'RUPY';
            counts[0] = 1;
            params[0] = 2;
            break;
        case DROP_RUPEE_P3:
            ids[0] = 'RUPY';
            counts[0] = 1;
            params[0] = 3;
            break;
        case DROP_RUPEE_P4:
            ids[0] = 'RUPY';
            counts[0] = 1;
            params[0] = 4;
            break;
        case DROP_RUPEE_P5:
            ids[0] = 'RUPY';
            counts[0] = 1;
            params[0] = 5;
            break;
        case DROP_RUPEE_5:
            ids[0] = 'RUPY';
            counts[0] = 5;
            break;
        case DROP_RUPEE_10:
            ids[0] = 'RUPY';
            counts[0] = 10;
            break;
        case DROP_RUPEE_P1_5:
            ids[0] = 'RUPY';
            counts[0] = 5;
            params[0] = 1;
            break;
        case DROP_RUPEE_P1_10:
            ids[0] = 'RUPY';
            counts[0] = 10;
            params[0] = 1;
            break;
        case DROP_RUPEE_20_20:
            ids[0] = 'RUPY';
            counts[0] = 20;
            ids[1] = 'RUPY';
            counts[1] = 20;
            params[1] = 1;
            break;
        }
        f32 x = mPos.x;
        f32 y = mPos.y - mPos.z;
        for (int i = 0; i < 2 && ids[i] != 'NULL'; i++) {
            params[i] |= 0x261E00;
            for (u32 j = 0; j < counts[i]; j++) {
                JGeometry::TVec3f pos(x, y, 0.0f);
                s32 serial = getActorManager_801F856C()->spawnActor_801F9484(ids[i], field_0x4, &pos,
                                                                             params[i], -1, -1);
                if (serial >= 0 && counts[i] > 1) {
                    ActorBase_804A5770* actor =
                        getActorManager_801F856C()->findActorBySerial_801F666C(serial);
                    if (actor != NULL) {
                        JGeometry::TVec3f p = pos;
                        f32 angle = 360.0f * i / counts[i];
                        p.x += 16.0f * JMath::sincosTable_.cosDegree(angle);
                        p.y += 16.0f * JMath::sincosTable_.sinDegree(angle);
                        actor->mPos = p;
                    }
                }
            }
        }
    }
}

// The actor's death: with "dstr" set and it the last such actor in the
// adventure mode (field_0x7e), fn_801CB434 at mPos; else, with "dasu" set,
// in the adventure mode with field_0x7f, outside fn_802DE814, on a cleared
// stage (isStageFlagSet) and 1 time in 5, a crystal ball (ACTOR_CRTL) for
// the killer if it is fn_801CCDA4's player and none exists yet; else the
// "dasu" drop (fn_801F52A4). Then the death effects (0x172, 0x173; the
// killer's effect slot gets the first), the killer's score updates,
// the death sound, and the view's counters (fn_802D7704 / fn_802D766C).
// FIXME: 99.75. Register allocation only (the emitter and the killer swap
// r29/r30); 1.0f and 0.2f are @NNN until .sdata2 is in order.
void ActorBase_804A5770::vfunc_0x24() {
    if (gGameRoot_8053AAF8->field_0x7e != 0 && field_0x1a8 == true &&
        getActorManager_801F856C()->fn_801F7A6C() == 1)
    {
        fn_801CB434(field_0x4, &mPos, false);
    } else if (field_0x18c != 0 && gGameRoot_8053AAF8->field_0x7e != 0 &&
               gGameRoot_8053AAF8->field_0x7f != 0 && !fn_802DE814() &&
               isStageFlagSet(lbl_80529DEC.field_0x118, lbl_80529DEC.field_0x11c) &&
               gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.2f && field_0x10c >= 0 &&
               field_0x10c < 4 && fn_8022F340(field_0x10c) == fn_801CCDA4() &&
               getActorManager_801F856C()->fn_801F7A08(ACTOR_CRTL) < 0)
    {
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_CRTL, field_0x4, &mPos, 0, -1, -1);
    } else if (field_0x18c != 0) {
        fn_801F52A4();
    }
    JPABaseEmitter* emitter = spawnEmitter_8013CC50(field_0x4, &mPos, 0x172, 1, 1, NULL, NULL, true,
                                                    lbl_8046A00C[field_0xc0]);
    spawnEmitter_8013CC50(field_0x4, &mPos, 0x173, 1, 1, NULL, NULL, true, lbl_8046A00C[field_0xc0]);
    s32 player = field_0x10c;
    if (player >= 0 && player < 4) {
        getEffectSystem_8013C824()->fn_8013C830(player, emitter);
        if (fn_8022F514(field_0x10c)) {
            field_0x10c = fn_8022F3DC(field_0x10c);
        }
        if (field_0xfc == 1) {
            fn_802359A4(field_0x10c, fn_802357E0(field_0x10c));
            s32 count = fn_802357E0(field_0x10c);
            getActorManager_801F856C()->fn_801F6718(field_0x10c, field_0x4, &mPos, count, 0);
        }
        fn_80235B68(field_0x10c, mSerial);
    }
    if (field_0xbc == 3 || field_0xbc == 4) {
        startSound_801F0E34(0x400F0, 0);
    } else {
        startSound_801F0E34(0x40003, 0);
    }
    mAlive = false;
    s32 bit = field_0x16c;
    if (bit != -1) {
        getGameView_802D6A9C()->fn_802D7704(bit, field_0x1a8, field_0x4);
    } else {
        getGameView_802D6A9C()->fn_802D766C(field_0x1a8, field_0x4);
    }
}

// FIXME: 99.7. Instructions identical; 1.0f is @NNN (lbl_8053F774 once the
// unit's .sdata2 is in order).
// Starts the four effects 0x6D, 0x6A-0x6C at mPos + (x, y) and sets
// field_0xd8 = 0, field_0x188 = 300 and the game root's 300 (fn_80134DE4).
void ActorBase_804A5770::vfunc_0x28(s32 height, f32 x, f32 y) {
    field_0xd8 = 0;
    field_0x188 = 300;
    JGeometry::TVec3f pos = mPos;
    pos.x += x;
    pos.y += y;
    spawnEmitter_8013CC50(field_0x4, &pos, 0x6D, height, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &pos, 0x6A, height, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &pos, 0x6B, height, 0, NULL, NULL, true, 1.0f);
    spawnEmitter_8013CC50(field_0x4, &pos, 0x6C, height, 0, NULL, NULL, true, 1.0f);
    gGameRoot_8053AAF8->fn_80134DE4(300, 0);
}

// The boss's death explosion, one frame of it while field_0x188 counts
// down (-1: not running, returns false): every 8 frames two blasts (0x68,
// 0x69) at a random point within `scale` around mPos + (x, y), every 32 a
// sound; at 0 the final blast, the view's counters, a Force Gem spawner
// (ACTOR_GOLD) and five Force Gems (ACTOR_RUPY; Stone Arrghus' second form
// three) thrown out at random, then the boss music ends (or GameRoot's
// fn_80138B7C for Stone Arrghus' second form and the Helmaroc King).
// FIXME: 98.8. Register allocation only (the inlined random number
// generator's scratch registers r5-r12); literals are @NNN until .sdata2 is
// in order.
bool ActorBase_804A5770::vfunc_0x2C(f32 x, f32 y, f32 scale) {
    if (field_0x188 == -1) {
        return false;
    }
    field_0x188--;
    if ((field_0x188 & 7) == 0) {
        JGeometry::TVec3f pos(mPos);
        f32 half = 0.5f * scale;
        pos.x += x + (scale * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - half);
        pos.y += y + (scale * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - half);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x68, 2, 0, NULL, NULL, true, 1.0f);
        spawnEmitter_8013CC50(field_0x4, &pos, 0x69, 2, 0, NULL, NULL, true, 1.0f);
    }
    if ((field_0x188 & 0x1F) == 0) {
        startSound_801F0E34(0x30001, 0);
    }
    if (field_0x188 == 0) {
        if (field_0x4 < 8) {
            gGameRoot2_8053AB10->field_0x20->fn_802F601C(&mPos, 2, NULL, 1.0f, 1.0f);
        }
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x59, 0, 0, NULL, NULL, true, 0.3f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x5A, 1, 0, NULL, NULL, true, 0.375f);
        spawnEmitter_8013CC50(field_0x4, &mPos, 0x5B, 1, 0, NULL, NULL, true, 0.375f);
        startSound_801F0E34(0x30001, 0);
        getActorManager_801F856C()->fn_801F6884();
        JGeometry::TVec3f pos(mPos);
        getActorManager_801F856C()->spawnActor_801F9484(ACTOR_GOLD, field_0x4, &mPos, 0x50, -1, -1);
        s32 first = 0;
        if (mActorId == ACTOR_BWRT && field_0x90 == 1) {
            first = 2;
        }
        for (s32 i = first; i < 5; i++) {
            pos.set(mPos);
            s32 kind = 4;
            if (i < 1) {
                kind = 7;
            }
            s32 serial = getActorManager_801F856C()->fn_801F8544(ACTOR_RUPY, field_0x4, &pos, kind);
            if (serial < 0) {
                continue;
            }
            ActorBase_804A5770* gem = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
            if (mActorId == 'FGAN') {
                JGeometry::TVec3f v(2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                    0.5f - gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                    3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 3.0f);
                gem->field_0x3c = v;
            } else if (mActorId == 'BOIC') {
                JGeometry::TVec3f v(2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(), 0.0f,
                                    -5.0f);
                gem->field_0x3c = v;
            } else {
                JGeometry::TVec3f v(2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                    2.0f - 4.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1(),
                                    3.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() + 3.0f);
                gem->field_0x3c = v;
            }
        }
        mAlive = false;
        s32 bit = field_0x16c;
        if (bit != -1) {
            getGameView_802D6A9C()->fn_802D7704(bit, field_0x1a8, field_0x4);
        } else {
            getGameView_802D6A9C()->fn_802D766C(field_0x1a8, field_0x4);
        }
        if ((mActorId == ACTOR_BWRT && field_0x90 == 1) || mActorId == ACTOR_GROK) {
            gGameRoot_8053AAF8->fn_80138B7C();
        } else {
            gAudioMgr_8053AF80->fn_80458F9C(60);
        }
    }
    return true;
}

void ActorBase_804A5770::vfunc_0x30(u8 value, bool set) {
    if (vfunc_0x3C()) {
        getGameView_802D6A9C()->fn_802D8050(vfunc_0x3C(), set, value);
    }
}

bool ActorBase_804A5770::vfunc_0x34() {
    if (vfunc_0x3C()) {
        return getGameView_802D6A9C()->fn_802D800C(vfunc_0x3C());
    }
    return false;
}

s32 ActorBase_804A5770::vfunc_0x38() {
    return getGameView_802D6A9C()->fn_802D7E04(vfunc_0x3C());
}

// The actor's bit in the screen and stage tables of the view (game/GameView.h):
// the top five bits of field_0x90, 0 for none.
s32 ActorBase_804A5770::vfunc_0x3C() {
    return field_0x90 >> 27;
}

void ActorBase_804A5770::vfunc_0x40() {}

void ActorBase_804A5770::vfunc_0x44() {}

void ActorBase_804A5770::vfunc_0x48() {}

// Plays the actor's "voic" sound (if it has one) and, unless field_0xe8 is
// set, the sound 0x40002.
// FIXME: 97.9. Instructions identical; only the lbl_80469F38 reference differs until the unit's
// .rodata is in the original order.
void ActorBase_804A5770::fn_801F4694() const {
    if (field_0x190 > 0) {
        startSound_801F0E34(lbl_80469F38[field_0x190 - 1], 0);
    }
    if (field_0xe8 == 0) {
        startSound_801F0E34(0x40002, 0);
    }
}

// Applies one hit (attack points, from the attacker at `from`, strength
// clamped to 2, `type`, by `player` or -1): ignored while field_0xd8 runs
// unless it comes from a player not yet in field_0xd5; types 5 and 2 are
// ignored or absorbed (returns 1 at once) by field_0xb0's bits. Otherwise
// sets the knock-back direction (field_0xec, field_0x3c), effects, the
// timers and mLife, plays the hurt or death voice; returns 1. Returns 0
// when ignored.
// The hit reaction, once per frame (`flag`: move this frame). While
// field_0xd8 runs: the knock-back by field_0x3c (kind field_0xf8: 0 and 1
// slide with friction 0.78 / 0.88, 2 is thrown up and bounces, effect 0x3CF),
// a dust trail along the way on the ground (0x3D3, or 0x3D4 on water-like
// tiles), and when it ends with no life left the death timer field_0xe0 (1,
// or 30 for a typed hit; a burning actor leaves a fire, ACTOR_BLZE). While
// field_0xe0 runs: the death effect for field_0xfc at 30, then vfunc_0x24
// at 0. field_0xe8 more hit sparks every 5 frames. Returns 0 (nothing to
// do), 1, or -1 when the actor has no life left.
// FIXME: 99.0. One extra instruction: ours keeps the death effect id in r28
// across case 1/2's calls and copies it to r5; the original has it in r5
// (the path with the two effects skips the id test). Literals are @NNN
// until .sdata2 is in order.
s32 ActorBase_804A5770::fn_801F3D94(u8 flag) {
    s32 result = 0;
    if (field_0xd8 != 0) {
        bool done = false;
        if (flag && !field_0xc4) {
            JGeometry::TVec3f start(mPos);
            switch (field_0xf8) {
            case 0:
                field_0xd8--;
                field_0x3c.scale(0.78f);
                mPos.add(field_0x3c);
                break;
            case 1:
                field_0xd8--;
                field_0x3c.scale(0.88f);
                mPos.add(field_0x3c);
                break;
            case 2:
                if (mPos.z > 0.0f) {
                    field_0x3c.z += -1.0f;
                    field_0x3c.scale(0.96f);
                } else {
                    field_0xd8--;
                }
                mPos.add(field_0x3c);
                if (mPos.z <= 0.0f) {
                    mPos.z = 0.0f;
                    if (field_0x3c.z < -2.0f) {
                        field_0x3c.z *= -0.6f;
                        field_0x3c.x *= 0.8f;
                        field_0x3c.y *= 0.8f;
                        spawnEmitter_8013CC50(field_0x4, &mPos, 0x3CF, 0, 0, NULL, NULL, true, 1.0f);
                    } else {
                        field_0x3c.zero();
                    }
                }
                if (field_0xd8 == 0) {
                    field_0x3c.zero();
                    mPos.z = 0.0f;
                }
                break;
            }
            if (mPos.z == 0.0f && field_0x3c.squared() > 1.0f) {
                f32 size = mActorId == 'SLK2' ? 1.0f : lbl_8046A00C[field_0xc0];
                f32 scale = 0.9f * size;
                if (scale > 0.0f) {
                    JGeometry::TVec3f dir;
                    dir.sub(mPos, start);
                    f32 length = dir.normalize();
                    u16 id;
                    if (fn_802258D4(&mPos, field_0x4) || fn_80225614(&mPos, field_0x4)) {
                        id = 0x3D4;
                    } else {
                        id = 0x3D3;
                    }
                    for (f32 t = 0.0f; t < length; t += 4.0f) {
                        JGeometry::TVec3f pos(dir);
                        pos.scale(t);
                        pos.add(start);
                        JPABaseEmitter* emitter = spawnEmitter_8013CC50(field_0x4, &pos, id, 0, 0, NULL,
                                                                        NULL, true, 1.0f);
                        if (emitter != NULL) {
                            emitter->mMaxFrame = 1;
                            emitter->setRate(2.0f);
                            emitter->setGlobalParticleScale(scale, scale);
                        }
                    }
                }
            }
        } else {
            field_0xd8--;
        }
        if (mLife <= 0 && field_0xe0 == 0 &&
            field_0xd8 <= (field_0xdc - 8 <= 0 ? 0 : field_0xdc - 8))
        {
            if (field_0xfc == 0) {
                field_0xe0 = 1;
            } else {
                field_0xe0 = 30;
            }
            field_0xd8 = 0;
            done = true;
            field_0x3c.zero();
            if (mEmitter100 != NULL) {
                s32 player = field_0x10c;
                s32 owner = 15;
                if (player >= 0 && player < 4) {
                    owner = player;
                }
                getActorManager_801F856C()->spawnActor_801F9484(ACTOR_BLZE, field_0x4, &mPos, owner, -1,
                                                               player);
            }
        } else if (field_0xd8 == 0) {
            done = true;
        }
        if (done) {
            fn_801CB7A0(mEmitter100);
            fn_801CB7A0(mEmitter104);
        }
        result = 1;
    } else if (field_0xe0 != 0) {
        if (field_0xe0 == 30) {
            f32 size = lbl_8046A00C[field_0xc0];
            u16 id = 0xFFFF;
            switch (field_0xfc) {
            case 1:
            case 2:
                if (field_0x10c >= 0 && field_0x10c < 4 && fn_80237060(field_0x10c) >= 2) {
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x17A, 1, 1, NULL, NULL, true, size);
                    spawnEmitter_8013CC50(field_0x4, &mPos, 0x17B, 1, 1, NULL, NULL, true, size);
                    startSound_801F0E34(0x5006B, 0);
                } else {
                    id = 0x174;
                }
                break;
            case 5:
                id = 0x179;
                break;
            case 4:
                id = 0x17C;
                break;
            case 3:
                if (__fabsf(field_0xec.x) > __fabsf(field_0xec.y)) {
                    if (field_0xec.x < 0.0f) {
                        id = 0x176;
                    } else {
                        id = 0x177;
                    }
                } else if (field_0xec.y < 0.0f) {
                    id = 0x178;
                } else {
                    id = 0x175;
                }
                break;
            }
            if (id != 0xFFFF) {
                spawnEmitter_8013CC50(field_0x4, &mPos, id, 1, 1, NULL, NULL, true, size);
            }
        }
        field_0xe0--;
        field_0x3c.zero();
        if (field_0xe0 == 0) {
            vfunc_0x24();
        }
        result = 1;
    } else {
        field_0xd5 = 0;
    }
    if (field_0xe8 > 0) {
        if (field_0xe4 > 0) {
            field_0xe4--;
        } else {
            JGeometry::TVec3f pos(mPos.x + 8.0f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                  mPos.y + 8.0f * (2.0f * gGameRoot_8053AAF8->mRandom.get_ufloat_1() - 1.0f),
                                  mPos.z);
            if (field_0xfc == 5) {
                spawnEmitter_8013CC50(field_0x4, &pos, 0x19D, 0, 2, NULL, NULL, true, 1.0f);
            } else {
                spawnEmitter_8013CC50(field_0x4, &pos, 0x24A, 0, 2, NULL, NULL, true, 1.0f);
                spawnEmitter_8013CC50(field_0x4, &pos, 0x24B, 0, 2, NULL, NULL, true, 1.0f);
            }
            startSound_801F0E34(0x40002, 0);
            field_0xe8--;
            if (field_0xe8 > 0) {
                field_0xe4 = 5;
            }
        }
    }
    if (result != 0 && mLife <= 0) {
        result = -1;
    }
    return result;
}

// FIXME: 94.3. Two differences, both shared with siblings: MWCC keeps the
// .rodata base in r31 for lbl_8046A00C / lbl_80469F38 / lbl_80469EF8 where
// the original addresses each table on its own (as fn_801EF830; with the
// tables extern the hoist goes away, so they likely belong to another
// unit's data), and the original narrows the result at the return
// (clrlwi r3, r4, 24, as fn_801F2618). Literals are @NNN until .sdata2 is
// in order.
bool ActorBase_804A5770::fn_801F37AC(s32 attack, const JGeometry::TVec3f* from, s32 strength,
                                     s32 type, s32 player) {
    s32 depth = 0;
    bool ignored = true;
    bool result = false;
    if (strength > 2) {
        strength = 2;
        depth = 2;
    }
    if (field_0xd8 == 0 || (player >= 0 && !(field_0xd5 & (1 << player)))) {
        ignored = false;
        if (type == 5) {
            if (field_0xb0 & 0x01000000) {
                ignored = true;
            } else if (field_0xb0 & 0x02000000) {
                return true;
            }
        }
        if (type == 2) {
            if (field_0xb0 & 0x04000000) {
                ignored = true;
            } else if (field_0xb0 & 0x08000000) {
                return true;
            }
        }
    }
    if (!ignored) {
        JGeometry::TVec3f dir(mPos.x - from->x, (mPos.y - mPos.z) - (from->y - from->z), 0.0f);
        dir.normalize();
        field_0xec.set(dir);
        field_0xa4 = 0;
        if (field_0xdc - field_0xd8 > 5) {
            if (type == 5) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x19D, strength, depth, NULL, NULL, true,
                                      1.0f);
            } else if (attack > 4 && type == 1) {
                fn_801CB5CC(field_0x4, &mPos, strength, depth);
            } else {
                fn_801CB688(field_0x4, &mPos, strength, depth);
            }
            if (attack > 4 && (type == 0 || type == 1)) {
                if (mPos.z != 0.0f || gGameRoot_8053AAF8->mRandom.get_ufloat_1() < 0.5f) {
                    field_0xf8 = 1;
                    dir.scale(16.0f);
                } else {
                    field_0xf8 = 2;
                    dir.z += 0.5f + gGameRoot_8053AAF8->mRandom.get_ufloat_1();
                    dir.scale(8.0f);
                }
            } else {
                field_0xf8 = 0;
                dir.scale(16.0f);
            }
            if (field_0xfc != 6) {
                field_0x3c.add(dir);
            }
        } else {
            if (field_0xe8 == 0) {
                field_0xe4 = 5;
            }
            field_0xe8++;
        }
        if (player >= 0) {
            field_0xd5 |= 1 << player;
        }
        field_0xfc = type;
        if (field_0xfc == 5) {
            switch (mActorId) {
            case 'TZOK':
            case 'SLK2':
                break;
            default:
                if (mActorId == 'TINK' || mLife - attack <= 0) {
                    deleteEmitter(&mEmitter100);
                    deleteEmitter(&mEmitter104);
                    f32 scale = lbl_8046A00C[field_0xc0];
                    mEmitter100 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBB, 0, -1, NULL, NULL,
                                                        true, scale);
                    mEmitter104 = spawnEmitter_8013CC50(field_0x4, &mPos, 0xBC, 0, 1, NULL, NULL,
                                                        true, scale);
                } else {
                    fn_801CB5CC(field_0x4, &mPos, strength, depth);
                }
                break;
            }
        }
        if (field_0xfc == 6) {
            field_0xe0 = 1;
        } else {
            field_0xd8 = 24;
            field_0xdc = 24;
        }
        if (attack > 0) {
            field_0x10c = player;
        }
        mLife -= attack;
        if (mLife <= 0) {
            mLife = 0;
            fn_801F4694();
        } else {
            if (field_0x190 > 0) {
                startSound_801F0E34(lbl_80469EF8[field_0x190 - 1], 0);
            }
            if (field_0xe8 == 0) {
                startSound_801F0E34(0x40002, 0);
            }
        }
        result = true;
    }
    return result;
}

// Sets field_0xa4 and clears the per-hit and timer state; field_0xe0 is only
// cleared while the actor has life left.
void ActorBase_804A5770::fn_801F3780(s32 v) {
    field_0xa4 = v;
    field_0xd8 = 0;
    field_0xd5 = 0;
    field_0xe4 = 0;
    field_0xe8 = 0;
    if (mLife > 0) {
        field_0xe0 = 0;
    }
}

// If field_0xc4 is clear: sets it, field_0xc8 = field_0x110 = v, clears
// field_0xcc, field_0xd0 = -1, returns 1; else 0.
u8 ActorBase_804A5770::fn_801F3744(s32 v) {
    s32 result = 0;
    if (field_0xc4 == 0) {
        field_0xc4 = 1;
        field_0xc8 = v;
        field_0xcc = 0;
        field_0xd0 = -1;
        field_0x110 = v;
        result = 1;
    }
    return result;
}

// If field_0xc4 is set: clears it, field_0x3c = *v, returns 1; else 0.
u8 ActorBase_804A5770::fn_801F3708(const JGeometry::TVec3f* v) {
    s32 result = 0;
    if (field_0xc4 != 0) {
        field_0xc4 = 0;
        field_0x3c.set(*v);
        result = 1;
    }
    return result;
}

// If neither field_0xcc nor field_0xc4 is set, marks the actor for the update
// loop with field_0xd0 = v and returns 1 (0 otherwise).
u8 ActorBase_804A5770::fn_801F36D4(s32 v) {
    s32 result = 0;
    if (field_0xcc == 0 && field_0xc4 == 0) {
        field_0xcc = 1;
        field_0xd0 = v;
        result = 1;
    }
    return result;
}

// Marks the actor dead, then fn_801F3604's report.
void ActorBase_804A5770::fn_801F3668() {
    mAlive = false;
    fn_801F3604();
}

// Reports field_0x16c (or, if it is -1, just the layer count) to the view
// with field_0x1a8.
void ActorBase_804A5770::fn_801F3604() {
    s32 bit = field_0x16c;
    if (bit != -1) {
        getGameView_802D6A9C()->fn_802D7704(bit, field_0x1a8, field_0x4);
    } else {
        getGameView_802D6A9C()->fn_802D766C(field_0x1a8, field_0x4);
    }
}

// Marks the actor dead, then fn_801F355C's report.
void ActorBase_804A5770::fn_801F35AC() {
    mAlive = false;
    fn_801F355C();
}

// Reports field_0x16c to the view with its layer (field_0x4), unless it is -1.
void ActorBase_804A5770::fn_801F355C() {
    s32 bit = field_0x16c;
    if (bit != -1) {
        getGameView_802D6A9C()->fn_802D7704(bit, false, field_0x4);
    }
}

// Marks the actor dead, then fn_801F34DC's report.
void ActorBase_804A5770::fn_801F3518() {
    mAlive = false;
    fn_801F34DC();
}

// Reports the actor's field_0x16c to the view, unless it is -1 (none).
void ActorBase_804A5770::fn_801F34DC() {
    s32 bit = field_0x16c;
    if (bit != -1) {
        getGameView_802D6A9C()->fn_802D7618(bit);
    }
}

// Queues a warp to *pos on layer, unless one is pending (field_0x17c) or
// running (field_0x174 > 0).
void ActorBase_804A5770::fn_801F349C(s32 layer, const JGeometry::TVec3f* pos) {
    if (field_0x17c != 0) {
        return;
    }
    if (field_0x174 > 0) {
        return;
    }
    field_0x30 = *pos;
    field_0x178 = layer;
    field_0x17c = 1;
    field_0x174 = 0;
}

// Runs a warp fn_801F349C queued: while one is running (field_0x174 > 0)
// counts it down and ends it once mPos is field_0x184 away from the
// destination (field_0x30); a queued one moves mPos.xy there, switches to
// its layer and starts the count (field_0x180). field_0x17d is set for the
// frame a warp starts.
void ActorBase_804A5770::fn_801F33C4() {
    field_0x17d = 0;
    if (field_0x174 > 0) {
        field_0x174--;
        const JGeometry::TVec3f& diff = field_0x30 - mPos;
        if (PSVECMag(&diff) >= field_0x184) {
            field_0x174 = 0;
        }
    } else if (field_0x17c) {
        mPos.x = field_0x30.x;
        mPos.y = field_0x30.y;
        field_0x4 = field_0x178;
        field_0x17c = 0;
        field_0x17d = 1;
        field_0x174 = field_0x180;
    }
}

// Copies each player's position (getPlayerPos_8023E724) into field_0x1ac[], moved
// into this layer's view space (fn_801CD150) for a player below layer 8
// while this is on 8.
void ActorBase_804A5770::fn_801F32F4() {
    for (s32 i = 0; i < 4; i++) {
        if (fn_801F329C(i)) {
            JGeometry::TVec3f pos;
            fn_801CD150(getPlayerPos_8023E724(i), &pos);
            field_0x1ac[i].set(pos);
        } else {
            field_0x1ac[i].set(*getPlayerPos_8023E724(i));
        }
    }
}

// Whether `player` is on a layer below 8 while this actor is on layer 8.
bool ActorBase_804A5770::fn_801F329C(s32 player) const {
    bool result = false;
    if (getPlayerLayer_8023DE58(player) < 8 && field_0x4 == 8) {
        result = true;
    }
    return result;
}

// Whether `player` is on this actor's layer (getPlayerLayer_8023DE58), or counts as on
// it through fn_801F329C.
bool ActorBase_804A5770::fn_801F320C(s32 player) const {
    s32 layer = field_0x4;
    bool result = false;
    s32 playerLayer = getPlayerLayer_8023DE58(player);
    if (playerLayer == layer || fn_801F329C(player)) {
        result = true;
    }
    return result;
}

// mBox in screen space.
void ActorBase_804A5770::fn_801F31C4(JGeometry::TBox2f* out) const {
    toScreenBox(out, mBox, mPos);
}

// mBox2 in screen space.
void ActorBase_804A5770::fn_801F317C(JGeometry::TBox2f* out) const {
    toScreenBox(out, mBox2, mPos);
}

// The centre of mBox in screen space.
// FIXME: 99.8. Instructions identical; only the 0.5f literal (lbl_8053F74C) pairs once .sdata2 is in order.
void ActorBase_804A5770::fn_801F3128(JGeometry::TVec2f* out) {
    JGeometry::TBox2f box;
    toScreenBox(&box, mBox, mPos);
    out->x = box.f.x + (box.i.x - box.f.x) * 0.5f;
    out->y = box.f.y + (box.i.y - box.f.y) * 0.5f;
}

// The players' hits (fn_801F2FAC, inlined), else one from an overlapping
// actor (fn_801F2DB4 with no filter).
u8 ActorBase_804A5770::fn_801F305C() {
    // Assignment in the condition: the narrowing of the inlined
    // fn_801F2FAC result lands in r3 (clrlwi. r3, r29, 24) only this way.
    u8 hit;
    if (!(hit = fn_801F2FAC())) {
        hit = fn_801F2DB4('NULL', 'NULL', NULL);
    }
    return hit;
}

// Each player that hits this actor (fn_801F285C) attacks it with its
// current attack (fn_8023B6CC) from its position. Returns whether any of
// the hits landed (fn_801F37AC).
// u8, not bool: the bool local is narrowed at the return (clrlwi r3, r30,
// 24), and callers keep r3 raw (EnemyBase vfunc_0x5C returns it with mr).
u8 ActorBase_804A5770::fn_801F2FAC() {
    bool hit = false;
    for (s32 i = 0; i < 4; i++) {
        s32 type;
        if (fn_801F285C(NULL, &type, true, i) == i) {
            hit |= fn_801F37AC(fn_8023B6CC(i), getPlayerPos_8023E724(i), 0, type, i);
        }
    }
    return hit;
}

// Takes a hit from an overlapping actor (fn_801F82CC) whose id is includeId
// (any for 'NULL'), is not excludeId and is not in the 'NULL'-ended list
// excludeIds: fn_801F37AC with the attacker's "atcn" (field_0x118), its
// direction to this actor and its field_0x168 / field_0x110. An actor the
// filter leaves out still counts as a hit unless excludeId is 'HBMB'; an
// 'SWBM' one then gets fn_801CB538 at its position, a 'FIRE' one (once per
// fire, field_0x1dc) that and effect 0x19D. A hit marks the attacker
// (field_0x164 = 1). Returns whether it was hit.
// u8 (clrlwi r3, r28, 24 at the return; EnemyBase / Thief / Beamos
// vfunc_0x60 return it raw).
// FIXME: 99.96. The 32.0f and 1.0f literals (lbl_8053F748, lbl_8053F774)
// pair only once .sdata2 is in order.
u8 ActorBase_804A5770::fn_801F2DB4(u32 includeId, u32 excludeId, const u32* excludeIds) {
    bool hit = false;
    s32 serial = getActorManager_801F856C()->fn_801F82CC(this, field_0x195, 32.0f);
    if (serial >= 0 && serial != mSerial) {
        ActorBase_804A5770* attacker = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        bool accept = false;
        if ((includeId == 'NULL' || includeId == attacker->mActorId) &&
            (excludeId == 'NULL' || excludeId != attacker->mActorId))
        {
            accept = true;
            if (excludeIds != NULL) {
                for (const u32* id = excludeIds; *id != 'NULL'; id++) {
                    if (*id == attacker->mActorId) {
                        accept = false;
                        break;
                    }
                }
            }
        }
        if (accept) {
            JGeometry::TVec3f from(mPos);
            from.sub(attacker->field_0x3c);
            hit = fn_801F37AC(attacker->field_0x118, &from, 0, attacker->field_0x168,
                              attacker->field_0x110);
        } else if (excludeId != 'HBMB') {
            hit = true;
            switch (attacker->mActorId) {
            case 'SWBM':
                fn_801CB538(field_0x4, &attacker->mPos, 0, 0);
                break;
            case 'FIRE':
                if (field_0x1dc.fn_801EAC54(attacker->mSerial)) {
                    fn_801CB538(field_0x4, &attacker->mPos, 0, 0);
                    spawnEmitter_8013CC50(field_0x4, &attacker->mPos, 0x19D, 0, 0, NULL, NULL, true, 1.0f);
                }
                break;
            }
        } else {
            hit = false;
        }
        if (hit) {
            attacker->field_0x164 = 1;
        }
    }
    return hit;
}

// Hits every player but skipPlayer whose box (fn_8023B190) overlaps mBox
// on this layer, with fn_80238548 at the box's centre (moved through
// fn_801CD0D0 for a player below layer 8 while this is on 8); damage -100
// means field_0x114 ("atcp"). Returns the bits of the players hit.
// FIXME: 98.2. Register allocation (this, damage, arg and the result one
// register off; the inlined fn_801F2740's result is copied once more), and
// the 32.0f, 0.5f and 0.0f literals (lbl_8053F748, lbl_8053F74C,
// lbl_8053F740) pair only once .sdata2 is in order.
u8 ActorBase_804A5770::fn_801F2B7C(s32 damage, s32 skipPlayer, s32 arg) {
    u8 result = fn_801F2740(skipPlayer);
    if (result) {
        JGeometry::TBox2f screen;
        toScreenBox(&screen, mBox, mPos);
        f32 cx = screen.f.x + (screen.i.x - screen.f.x) * 0.5f;
        f32 cy = screen.f.y + (screen.i.y - screen.f.y) * 0.5f;
        for (s32 i = 0; i < 4; i++) {
            if (!(result & (1 << i))) {
                continue;
            }
            JGeometry::TVec2f pos;
            if (fn_801F329C(i)) {
                JGeometry::TVec3f out;
                JGeometry::TVec3f in(cx, cy, 0.0f);
                fn_801CD0D0(&in, &out);
                pos.x = out.x;
                pos.y = out.y;
            } else {
                pos.x = cx;
                pos.y = cy;
            }
            if (damage == -100) {
                damage = field_0x114;
            }
            fn_80238548(i, damage, &pos, arg);
        }
    }
    return result;
}

u8 ActorBase_804A5770::fn_801F2B20(s32 arg) {
    u8 prev = field_0x164;
    field_0x168 = arg;
    getActorManager_801F856C()->fn_801F84C4(this);
    if (field_0x164 != 0) {
        field_0x164 = 0;
    }
    return prev;
}

// FIXME: 99.9. Instructions identical; 32.0f is @NNN (lbl_8053F748).
// Whether a player other than skipPlayer's group (fn_8022E438) touches mBox
// on this layer (fn_80239B64 with type, a and b); on a layer without the
// view's flag the player's z must also be within 32 of mPos.z.
bool ActorBase_804A5770::fn_801F2A10(s32 skipPlayer, s32 type, bool a, bool b) {
    JGeometry::TBox2f box;
    toScreenBox(&box, mBox, mPos);
    for (s32 i = 0; i < 4; i++) {
        if (!viewLayerFlag(field_0x4) &&
            __fabsf(mPos.z - getPlayerPos_8023E724(i)->z) > 32.0f)
        {
            continue;
        }
        if (!fn_8022E438(i, skipPlayer) && fn_80239B64(i, &box, field_0x4, type, a, b)) {
            return true;
        }
    }
    return false;
}

// The player (`player`, or any when it is negative) whose box overlaps mBox
// around mPos on this layer: first by fn_8023B9CC / fn_8023B8E4 (*outFlag 1,
// or 0 when fn_80233DA4 holds for it), else by fn_8023B848 / fn_8023B768
// (*outFlag 0); on a layer without the view's flag its z must be within 32
// of mPos.z. *outXY gets the point the test reports. -1 when none.
// FIXME: 99.8. Register allocation only: before fn_8023B9CC the target moves
// arg into r7 before player into r3, ours the other way round.
// const: the screen box (the inlined fn_801F31C4) loads before the register
// saves.
s32 ActorBase_804A5770::fn_801F285C(JGeometry::TVec2f* outXY, s32* outFlag, bool arg,
                                     s32 player) const {
    JGeometry::TBox2f box;
    fn_801F31C4(&box);
    JGeometry::TVec2f xy;
    s32 hit = -1;
    if (player >= 0) {
        if (fn_8023B9CC(player, &box, field_0x4, &xy, arg)) {
            hit = player;
        }
    } else {
        hit = fn_8023B8E4(&box, field_0x4, &xy, arg);
    }
    if (hit == -1) {
        if (player >= 0) {
            if (fn_8023B848(player, &box, field_0x4, &xy)) {
                hit = player;
            }
        } else {
            hit = fn_8023B768(&box, field_0x4, &xy);
        }
        if (hit != -1 && outFlag != NULL) {
            *outFlag = 0;
        }
    } else if (outFlag != NULL) {
        if (fn_80233DA4(hit)) {
            *outFlag = 0;
        } else {
            *outFlag = 1;
        }
    }
    if (hit != -1 && !viewLayerFlag(field_0x4) &&
        __fabsf(mPos.z - getPlayerPos_8023E724(hit)->z) > 32.0f)
    {
        hit = -1;
    }
    if (outXY != NULL) {
        outXY->x = xy.x;
        outXY->y = xy.y;
    }
    return hit;
}

// The players other than skipPlayer whose box (fn_8023B190) overlaps mBox
// on this layer, as bits; on a layer without the view's flag the player's z
// must also be within 32 of mPos.z. fn_801F2B7C inlines it.
// FIXME: 99.15. The target ends in clrlwi r3, r30, 24 where ours returns
// hits with mr (the callers' raw use of r3 shows a u8 return; a bool local
// gives that narrowing, as in fn_801F2FAC / fn_801F2618, but a bitmask in a
// bool is not written here; an s32 local drops the in-loop clrlwi instead).
// const: the box's first floats load before the register saves.
// Besides, 32.0f is lbl_8053F748 once .sdata2 is in order.
u8 ActorBase_804A5770::fn_801F2740(s32 skipPlayer) const {
    JGeometry::TBox2f box;
    toScreenBox(&box, mBox, mPos);
    u8 hits = 0;
    for (s32 i = 0; i < 4; i++) {
        if (i == skipPlayer) {
            continue;
        }
        if (!viewLayerFlag(field_0x4) &&
            __fabsf(mPos.z - getPlayerPos_8023E724(i)->z) > 32.0f)
        {
            continue;
        }
        if (!fn_8022E438(i, skipPlayer) && !fn_8022D75C(i) && fn_8023B190(i, &box, field_0x4)) {
            hits |= 1 << i;
        }
    }
    return hits;
}

// FIXME: 99.5 %. Only the literal differs (lbl_8053F748, see above).
s32 ActorBase_804A5770::fn_801F2718(u32 actorId) const {
    return fn_801F8108(this, actorId, mSerial, 32.0f);
}

// 32.0f is lbl_8053F748; fn_801F26CC (reloc-name) and fn_801F2718 pair with
// it only once the unit's .sdata2 order is fixed (see the constructor's FIXME).
s32 ActorBase_804A5770::fn_801F26CC(const u32* ids) {
    return getActorManager_801F856C()->fn_801F7F58(this, ids, mSerial, 32.0f);
}

// Unless a warp is running (field_0x174 > 0), sweeps the map box
// (field_0x80) from mPrevPos to mPos (fn_801F23BC); on a hit moves mPos to
// the result and passes the other vector out. Returns whether it hit.
// u8 (clrlwi r3, r31, 24 at the return): EnemyBase / Thief vfunc_0x64 and
// fn_801545C0 return it raw, the update()s keep it in a u8 local.
u8 ActorBase_804A5770::fn_801F2618(JGeometry::TVec3f* out, bool flag) {
    bool result = false;
    if (field_0x174 <= 0) {
        JGeometry::TVec3f pos;
        JGeometry::TVec3f vec;
        if (fn_801F23BC(field_0x4, &mPos, &mPrevPos, &field_0x80, &pos, &vec, flag)) {
            mPos.set(pos);
            if (out != NULL) {
                out->set(vec);
            }
            result = true;
        }
    }
    return result;
}

// u8, as fn_801F2618 / fn_801F2FAC / fn_801F2DB4 (clrlwi r3, r30, 24).
// FIXME: 99.1. Float register allocation (step / negStep swap f28/f29);
// 0.01f / 8.0f / 0.5f are @NNN until .sdata2 is in order.
u8 ActorBase_804A5770::fn_801F23BC(s32 layer, const JGeometry::TVec3f* to,
                                     const JGeometry::TVec3f* from, const JGeometry::TBox2f* box,
                                     JGeometry::TVec3f* outPos, JGeometry::TVec3f* out2, bool flag) {
    JGeometry::TVec3f pos(*from);
    bool moving = true;
    bool hit = false;
    f32 width = box->getWidth();
    f32 height = box->getHeight();
    if (width > 0.01f && height > 0.01f) {
        f32 side = height;
        if (width < height) {
            side = width;
        }
        f32 step = side;
        if (side > 8.0f) {
            step = 8.0f;
        }
        step = (step - 0.01f) * 0.5f;
        f32 negStep = -step;
        while (moving) {
            JGeometry::TVec3f delta;
            delta.sub(*to, pos);
            bool xDone = false;
            bool yDone = false;
            if (delta.x > step) {
                delta.y *= step / delta.x;
                delta.x = step;
            } else if (delta.x < negStep) {
                delta.y *= negStep / delta.x;
                delta.x = negStep;
            } else {
                xDone = true;
            }
            if (delta.y > step) {
                delta.x *= step / delta.y;
                delta.y = step;
            } else if (delta.y < negStep) {
                delta.x *= negStep / delta.y;
                delta.y = negStep;
            } else {
                yDone = true;
            }
            if (xDone && yDone) {
                moving = false;
                pos.set(*to);
            } else {
                pos.add(delta);
            }
            if (fn_801F172C(layer, &pos, box, out2, flag)) {
                hit = true;
                moving = false;
                if (outPos != NULL) {
                    outPos->set(pos);
                }
            }
        }
    }
    return hit;
}

// Pushes `box` at *pos out of whatever blocks it on `layer` (vfunc_0x50).
// Each edge of the box is probed every 8 units and at its far end; the
// hits are counted as [start, middle, end] per edge, and a hit gives the
// distance to the next 8-unit cell line. An edge whose middle is blocked
// while the opposite one is free, or that is blocked at both ends, or a
// blocked corner where the other edge is shorter, moves pos by that
// distance. When it moved and `out` is given, *out is the direction of the
// push (with `flag`, the diagonal tiles 8-0xB at the corners of the old
// position bend it), normalised. Returns whether it moved.
// FIXME: 91.96. Structurally complete; two differences: MWCC keeps the
// .rodata base in r31 for the four counter initialisers (the target
// addresses each with its own lis/addi, lbl_80469F78-lbl_80469F9C; one more
// saved register and a 0x10-larger frame here, as fn_801EF830), and the
// counter increment `hits[i != 0]++` is scheduled a few slots earlier.
u8 ActorBase_804A5770::fn_801F172C(s32 layer, JGeometry::TVec3f* pos,
                                     const JGeometry::TBox2f* box, JGeometry::TVec3f* out,
                                     bool flag) {
    JGeometry::TVec3f p;
    JGeometry::TVec3f n;
    bool moved = false;
    f32 left = box->i.x;
    f32 top = box->i.y;
    f32 right = box->f.x - 0.01f;
    f32 bottom = box->f.y - 0.01f;
    s32 i;

    // Left edge.
    f32 dxL = 0.0f;
    p.set(pos->x + left, 0.0f, 0.0f);
    s32 hitsL[3] = {0, 0, 0};
    i = 0;
    for (p.y = pos->y + top; p.y < pos->y + bottom; p.y += 8.0f) {
        if (vfunc_0x50(layer, &p)) {
            hitsL[i != 0]++;
            dxL = (s32)(0.125f * p.x + 1.0f) * 8 - p.x;
        }
        i++;
    }
    p.y = pos->y + bottom;
    if (vfunc_0x50(layer, &p)) {
        hitsL[2]++;
        dxL = (s32)(0.125f * p.x + 1.0f) * 8 - p.x;
    }

    // Right edge.
    f32 dxR = 0.0f;
    p.set(pos->x + right, 0.0f, 0.0f);
    s32 hitsR[3] = {0, 0, 0};
    i = 0;
    for (p.y = pos->y + top; p.y < pos->y + bottom; p.y += 8.0f) {
        if (vfunc_0x50(layer, &p)) {
            hitsR[i != 0]++;
            dxR = (pos->x + box->f.x) - (s32)(0.125f * p.x) * 8;
        }
        i++;
    }
    p.y = pos->y + bottom;
    if (vfunc_0x50(layer, &p)) {
        hitsR[2]++;
        p.x = pos->x + box->f.x;
        dxR = p.x - (s32)(0.125f * p.x) * 8;
    }

    // Top edge.
    f32 dyT = 0.0f;
    p.set(0.0f, pos->y + top, 0.0f);
    s32 hitsT[3] = {0, 0, 0};
    i = 0;
    for (p.x = pos->x + left; p.x < pos->x + right; p.x += 8.0f) {
        if (vfunc_0x50(layer, &p)) {
            hitsT[i != 0]++;
            dyT = (s32)(0.125f * p.y + 1.0f) * 8 - p.y;
        }
        i++;
    }
    p.x = pos->x + right;
    if (vfunc_0x50(layer, &p)) {
        hitsT[2]++;
        dyT = (s32)(0.125f * p.y + 1.0f) * 8 - p.y;
    }

    // Bottom edge.
    f32 dyB = 0.0f;
    p.set(0.0f, pos->y + bottom, 0.0f);
    s32 hitsB[3] = {0, 0, 0};
    i = 0;
    for (p.x = pos->x + left; p.x < pos->x + right; p.x += 8.0f) {
        if (vfunc_0x50(layer, &p)) {
            hitsB[i != 0]++;
            dyB = (pos->y + box->f.y) - (s32)(0.125f * p.y) * 8;
        }
        i++;
    }
    p.x = pos->x + right;
    if (vfunc_0x50(layer, &p)) {
        hitsB[2]++;
        p.y = pos->y + box->f.y;
        dyB = p.y - (s32)(0.125f * p.y) * 8;
    }

    if (out != NULL) {
        p.set(*pos);
        n.zero();
    }

    if ((hitsL[1] != 0 && hitsR[1] == 0) || (hitsL[0] != 0 && hitsL[2] != 0) ||
        (hitsL[0] != 0 && hitsT[2] == 0 && dxL < dyT && (hitsR[0] == 0 || hitsR[2] == 0)) ||
        (hitsL[2] != 0 && hitsB[2] == 0 && dxL < dyB && (hitsR[2] == 0 || hitsR[0] == 0)))
    {
        pos->x += dxL;
        moved = true;
        n.x = 1.0f;
    }
    if ((hitsR[1] != 0 && hitsL[1] == 0) || (hitsR[0] != 0 && hitsR[2] != 0) ||
        (hitsR[0] != 0 && hitsT[0] == 0 && dxR < dyT && (hitsL[0] == 0 || hitsL[2] == 0)) ||
        (hitsR[2] != 0 && hitsB[0] == 0 && dxR < dyB && (hitsL[2] == 0 || hitsL[0] == 0)))
    {
        pos->x -= dxR;
        moved = true;
        n.x = -1.0f;
    }
    if ((hitsT[1] != 0 && hitsB[1] == 0) || (hitsT[0] != 0 && hitsT[2] != 0) ||
        (hitsT[0] != 0 && hitsL[2] == 0 && dyT < dxL && (hitsB[0] == 0 || hitsB[2] == 0)) ||
        (hitsT[2] != 0 && hitsR[2] == 0 && dyT < dxR && (hitsB[2] == 0 || hitsB[0] == 0)))
    {
        pos->y += dyT;
        moved = true;
        n.y = 1.0f;
    }
    if ((hitsB[1] != 0 && hitsT[1] == 0) || (hitsB[0] != 0 && hitsB[2] != 0) ||
        (hitsB[0] != 0 && hitsL[0] == 0 && dyB < dxL && (hitsT[0] == 0 || hitsT[2] == 0)) ||
        (hitsB[2] != 0 && hitsR[0] == 0 && dyB < dxR && (hitsT[2] == 0 || hitsT[0] == 0)))
    {
        pos->y -= dyB;
        moved = true;
        n.y = -1.0f;
    }

    if (moved && out != NULL) {
        if (flag) {
            f32 y0 = p.y + box->i.y;
            f32 y1 = p.y + box->f.y;
            f32 x0 = p.x + box->i.x;
            f32 x1 = p.x + box->f.x;
            JGeometry::TVec3f d(0.0f, 0.0f, 0.0f);
            // fn_802265FC reads only x and y: the corners are 8-byte points.
            if (hitsL[0] != 0 && (n.x > 0.0f || n.y > 0.0f)) {
                JGeometry::TVec2f c(x0, y0);
                if (fn_802265FC((const JGeometry::TVec3f*)&c, layer) == 0xB) {
                    d.x += 1.0f;
                    d.y += 1.0f;
                }
            }
            if (hitsR[0] != 0 && (n.x < 0.0f || n.y > 0.0f)) {
                JGeometry::TVec2f c(x1, y0);
                if (fn_802265FC((const JGeometry::TVec3f*)&c, layer) == 0xA) {
                    d.x += -1.0f;
                    d.y += 1.0f;
                }
            }
            if (hitsL[2] != 0 && (n.x > 0.0f || n.y < 0.0f)) {
                JGeometry::TVec2f c(x0, y1);
                if (fn_802265FC((const JGeometry::TVec3f*)&c, layer) == 9) {
                    d.x += 1.0f;
                    d.y += -1.0f;
                }
            }
            if (hitsR[2] != 0 && (n.x < 0.0f || n.y < 0.0f)) {
                JGeometry::TVec2f c(x1, y1);
                if (fn_802265FC((const JGeometry::TVec3f*)&c, layer) == 8) {
                    d.x += -1.0f;
                    d.y += -1.0f;
                }
            }
            if (!d.isZero()) {
                n.set(d);
            }
        }
        n.normalize();
        out->set(n);
    }
    return moved;
}

// Whether pos is blocked on layer (vfunc_0x4C) or outside the view's box
// for that layer.
bool ActorBase_804A5770::vfunc_0x50(s32 layer, const JGeometry::TVec3f* pos) {
    JGeometry::TBox2f* box = layerBox(layer);
    return vfunc_0x4C(layer, pos) || pos->x < box->i.x || pos->y < box->i.y ||
           pos->x >= box->f.x || pos->y >= box->f.y;
}

// Whether pos is blocked on layer: by a tile, or by another actor's
// collision box from the last frame.
// FIXME: 89.5 %. Same instructions; the target keeps layer in r29 and pos
// in r28, this code the other way round (so the two argument moves before
// fn_802265DC are scheduled differently).
bool ActorBase_804A5770::vfunc_0x4C(s32 layer, const JGeometry::TVec3f* pos) {
    bool blocked = true;
    if (!fn_802265DC(pos, layer, false)) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (!boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            blocked = false;
        }
    }
    return blocked;
}

// The vfunc_0x4C of the liftable objects (act_pot, act_rock, act_grass, ...):
// blocked by another actor's collision box, or by the tile at pos. While
// *timer runs it counts down and nothing blocks; stepping onto a tile of
// the 0x10-0x13 / 0x140-0x15F ranges (but not 0x146-0x148 / 0x159-0x15B)
// starts it at 0x78. Tiles 0x25 / 0x26 block by the view's field_0x171,
// 0x22 only while its field_0x170 is set.
bool ActorBase_804A5770::fn_801F132C(s32 layer, const JGeometry::TVec3f* pos, s32* timer) {
    if (*timer == 0) {
        s32 serial = mSerial;
        CollisionBoxes_8020D58C* boxes = fn_8020D58C();
        if (boxes->mLists[boxes->field_0x1].fn_8020D6AC(layer, pos, serial, NULL)) {
            return true;
        }
        s32 code = fn_8022461C(pos, layer);
        s32 tile = fn_80226850(pos, layer);
        if (tile == 0x230 || tile == 0x231) {
            return false;
        }
        if (code != 0 && code != 4 && code != 5 && code != 6 && code != 7 && code != 2 &&
            code != 3 && code != 0x50 && code != 0xE && code != 0xC && code != 0x27 &&
            code != 0x28 && code != 0x29 && code != 0x2A && code != 0x2B && code != 0x2C &&
            code != 0x2D && code != 0x2E && code != 0x2F && code != 0x20 && code != 0xD &&
            code != 0x39 && code != 0x3A && code != 0x3B && code != 0x4C && code != 0x4D &&
            code != 0x4E && code != 0x4F && code != 0x24)
        {
            if (code == 0x10 || code == 0x11 || code == 0x12 || code == 0x13 ||
                (tile >= 0x140 && tile <= 0x15F))
            {
                if (tile != 0x146 && tile != 0x147 && tile != 0x148 && tile != 0x159 &&
                    tile != 0x15A && tile != 0x15B)
                {
                    *timer = 0x78;
                    return false;
                }
            }
            if (tile == 0x3A || tile == 0x3D) {
                return false;
            }
            if (code == 0x25) {
                // (u8): the result is narrowed as a byte (extrwi), as in act_bombchu's vfunc_0x4C.
                return (u8)(getGameView_802D6A9C()->field_0x171 == 0);
            }
            if (code == 0x26) {
                return (u8)(getGameView_802D6A9C()->field_0x171 == 1);
            }
            if (code != 0x22 || getGameView_802D6A9C()->field_0x170 == 1) {
                return true;
            }
        }
    } else {
        (*timer)--;
    }
    return false;
}

// With a map box (field_0x80) and blocked on its layer (vfunc_0x4C): steps
// back to mPrevPos's x/y and 8 further along the reverse of field_0x98's
// direction; if that is outside the view's box for the layer, respawns at
// mSpawnPos on layer field_0x8. Returns whether it was blocked.
// FIXME: 99.4. Float register allocation only (the step's operands); 8.0f
// is @NNN until .sdata2 is in order (lbl_8053F7A4).
bool ActorBase_804A5770::fn_801F11B4() {
    bool blocked = false;
    if (field_0x80.getWidth() > 0.0f && field_0x80.getHeight() > 0.0f &&
        vfunc_0x4C(field_0x4, &mPos))
    {
        mPos.x = mPrevPos.x;
        mPos.y = mPrevPos.y;
        s16 angle = fn_801F6874(field_0x98);
        f32 dx = JMath::sincosTable_.sinShort(angle - 0x8000) * 8.0f;
        f32 dy = JMath::sincosTable_.cosShort(angle - 0x8000) * 8.0f;
        mPos.x += dx;
        mPos.y += dy;
        JGeometry::TBox2f* box = layerBox(field_0x4);
        bool inView = mPos.x >= box->i.x && mPos.y >= box->i.y && mPos.x < box->f.x &&
                      mPos.y < box->f.y;
        if (!inView) {
            field_0x4 = field_0x8;
            mPos = mSpawnPos;
            mPrevPos = mSpawnPos;
        }
        blocked = true;
    }
    return blocked;
}

ActorSoundObj_804578F4* ActorBase_804A5770::fn_801F1184() const {
    return gGameRoot_8053AAF8->field_0x4->mActorManager->fn_801F6644(mSlot);
}

// Starts sound soundId on the actor's sound object, with the layer
// (field_0x4 >= 8) and a mode (field_0x45) from the view. Four sounds on
// layers 8 and up go to fn_803037D4 for the players near the actor instead.
// FIXME: 99.25. Register allocation only: the target keeps the player loops'
// counter in r28 (where `sound` lives later), ours in r31 (fadeTime's).
JAISoundHandle* ActorBase_804A5770::startSound_801F0E34(u32 soundId, u32 fadeTime) const {
    if (field_0x4 >= 8 && !fn_8022BB48(field_0x4, &mPos)) {
        return NULL;
    }
    if (field_0x4 >= 8 &&
        (gGameRoot_8053AAF8->field_0x7e != 0 || (gGameRoot_8053AAF8->field_0x83 & 3U) != 2))
    {
        if (soundId == 0x30009) {
            raisePlayersNear(this, 8);
            return NULL;
        } else if (soundId == 0x30004) {
            raisePlayersNear(this, 0xB);
            return NULL;
        } else if (soundId == 0x30022) {
            raisePlayersNear(this, 7);
            return NULL;
        } else if (soundId == 0x3004B) {
            raisePlayersNear(this, 8);
            return NULL;
        }
    }
    ActorSoundObj_804578F4* sound =
        gGameRoot_8053AAF8->field_0x4->mActorManager->fn_801F6644(mSlot);
    sound->field_0x44 = field_0x4 >= 8;
    if (getGameView_802D6A9C()->fn_802D7804(field_0x4, 0x800)) {
        sound->field_0x45 = 0;
    } else {
        s32 layer = field_0x4;
        if (layer < 8) {
            if (getGameView_802D6A9C()->fn_802D7948() == 1) {
                sound->field_0x45 = 2;
            } else {
                sound->field_0x45 = 0;
            }
        } else if (layer == 8) {
            sound->field_0x45 = 3;
        } else {
            GameView_804A9950* view = getGameView_802D6A9C();
            if (field_0x4 < view->field_0x54) {
                if (getGameView_802D6A9C()->fn_802D7948() == 1) {
                    sound->field_0x45 = 2;
                } else {
                    sound->field_0x45 = 1;
                }
            } else {
                sound->field_0x45 = 2;
            }
        }
    }
    return sound->fn_80456334(soundId, fadeTime);
}

// Plays sound effect `id` through the audio object; ids 3 and 0x42 on
// layers 8 and up go to fn_803037D4 (0xF or 0xE) for the players near the
// actor instead, and only fall back to the sound when none was.
void ActorBase_804A5770::fn_801F0D20(u32 id) {
    if (id == 3 || id == 0x42) {
        if (field_0x4 >= 8 && !fn_8022BB48(field_0x4, &mPos)) {
            return;
        }
        bool raised = false;
        for (s32 i = 0; i < 4; i++) {
            if ((gGameRoot_8053AAF8->mPlayerMask & (u8)(1 << i)) && field_0x4 >= 8 &&
                fn_8022BC38(i, field_0x4, &mPos) && fn_8030381C(i))
            {
                if (id == 3) {
                    fn_803037D4(i, 0xF);
                } else {
                    fn_803037D4(i, 0xE);
                }
                raised = true;
            }
        }
        if (raised == true) {
            return;
        }
    }
    gAudioMgr_8053AF80->fn_80458880(id, -1);
}

// FIXME: 93.3. One instruction extra: MWCC keeps the .rodata base
// (lbl_80469FA8 and the two strings) in r30 from the prologue (one more
// saved register, shifting r20-r30); the original addresses each with its
// own lis/addi. Also the copy of params' 0x1C/0x1D stores 0x1C before
// loading 0x1D (the original loads both first).
// Draws the shadow of kind `flags & 0x3FF` under pos on `layer` (flags -1:
// none): "kage_sob.bin" frame `kind` scaled by lbl_80469FA8[kind], kind 7 a
// flattened copy of the sprite `name` (or the loaded `res`), and kind 0 on
// tile 0x39 at z 0 an animated "enemy_both2_sob.bin" frame instead. No
// shadow on the tiles in lbl_8053F730 (the last one only unless
// fn_802DEB58()). Its alpha fades with height; bit 0x400 multiplies it by
// params' alpha.
void fn_801EF830(s32 layer, const JGeometry::TVec3f* pos, SpriteParams_801F5AE0* params, s32 flags,
                 const char* name, s32 frame, void* res) {
    if (flags != -1 && layer < 8 && !viewLayerFlag(layer)) {
        s32 kind = flags & 0x3FF;
        u16 tile = fn_802265FC(pos, 0);
        bool draw = true;
        int count = 3;
        if (fn_802DEB58()) {
            count = 2;
        }
        for (int i = 0; i < count; i++) {
            if (tile == lbl_8053F730[i]) {
                draw = false;
                break;
            }
        }
        if (!draw) {
            return;
        }

        SpriteParams_801F5AE0 sp;
        // Declared here, in the order of their stack slots (MWCC: reverse
        // declaration order).
        Mtx m;
        JGeometry::TVec3f flatPos;
        JGeometry::TVec3f tilePos;
        sp.field_0x10.a = 0xDF;
        if (pos->z > 0.0f) {
            f32 alpha = sp.field_0x10.a - pos->z * 0.5f;
            sp.field_0x10.a = 159.0f >= alpha ? 159.0f : alpha;
        }
        if (params != NULL) {
            if (flags & 0x400) {
                sp.field_0x10.a = sp.field_0x10.a * params->field_0x10.a / 255;
            }
            sp.field_0x1c = params->field_0x1c;
            sp.field_0x1d = params->field_0x1d;
        }
        sp.field_0x1d--;
        sp.field_0x28 = 1;
        sp.field_0x2e = sp.field_0x10.a >> 1;

        if (tile == 0x39 && kind == 0 && pos->z == 0.0f) {
            s32 frame2 = lbl_8053F738[(gGameRoot_8053AAF8->field_0x4->field_0x34 >> 2) & 3] + 0x18F;
            sp.field_0x1d++;
            tilePos.set(pos->x, pos->y + 0.1f, pos->z);
            if (layer < 8) {
                void* r = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "enemy_both2_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&tilePos, r, frame2, &sp, 1.0f, 1.0f);
            }
        } else if (kind == 7) {
            flatPos.set(pos->x, pos->y, -5.0f);
            PSMTXScale(m, 0.9f, 0.5f, 0.5f);
            if (params != NULL && params->mMtx != NULL) {
                f32 x = params->mMtx[0][3];
                f32 y = params->mMtx[1][3];
                f32 z = params->mMtx[2][3];
                PSMTXConcat(m, params->mMtx, m);
                m[0][3] = x;
                m[1][3] = y;
                m[2][3] = z;
            }
            sp.field_0x10.r = 0;
            sp.field_0x10.g = 0;
            sp.field_0x10.b = 0;
            sp.field_0x14.r = 40;
            sp.field_0x14.g = 40;
            sp.field_0x14.b = 40;
            sp.field_0x14.a = 0;
            sp.mMtx = m;
            if (name != NULL) {
                if (layer < 8) {
                    void* r = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', name);
                    gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&flatPos, r, frame, &sp, 1.0f, 1.0f);
                }
            } else if (layer < 8) {
                gGameRoot2_8053AB10->field_0x20->fn_802F745C(&flatPos, res, &sp);
            }
        } else {
            JGeometry::TVec3f p(pos->x, pos->y, 0.0f);
            f32 scale = lbl_80469FA8[kind];
            if (layer < 8) {
                void* r = gGameRoot_8053AAF8->field_0xa4->getResource('SOB ', "kage_sob.bin");
                gGameRoot2_8053AB10->field_0x20->fn_802F86CC(&p, r, kind, &sp, scale, 1.0f);
            }
        }
    }
}

// fn_801F2DB4 over the 17 actor ids below (the list is copied to the stack).
// FIXME: 99.0. Instructions identical; the list is an @NNN literal here but lbl_80469FC8 in the
// target (rodata order: the unit's earlier .rodata users are unwritten).
void ActorBase_804A5770::fn_801EF510() {
    u32 list[17] = {'SWBM', 'FIRE', 'BLZE', 'HBMB', 'FLME', 'THCL', 'GRAS', 'SSTN', 'STON',
                    'ROCK', 'RCK2', 'BORD', 'TUBO', 'KEY0', 'YASI', 'SMRI', 'NULL'};
    fn_801F2DB4('NULL', 'NULL', list);
}

// The player picked by `mode` relative to the actor `serial`, looked up
// through the manager (r3 is never read): modes 0 and 2 the nearest (2 only
// `player`), 1 and 3 the highest fn_80236228 (3 skipping `player`), 4 and 5
// the highest fn_80237774 (5 skipping `player`). A player counts on the
// actor's layer, or below 8 while the actor is on 8 (then measured in the
// view box's space). Reports the 16-bit angle from the player to the actor,
// its direction index (fn_801F681C), the distance and the player; -1 for
// direction and player when there is no actor or no player.
void ActorBase_804A5770::fn_801EEC98(s32 serial, u16* outAngle, s32* outDir, f32* outDist,
                                     s32* outPlayer, s32 player, s32 mode) {
    ActorBase_804A5770* actor = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
    f32 bestDist = -1.0f;
    s32 angle = 0;
    s32 found = -1;
    s32 bestA = -1;
    s32 bestB = -1;
    s32 i = 0;
    if (actor == NULL) {
        if (outDir != NULL) {
            *outDir = -1;
        }
        if (outPlayer != NULL) {
            *outPlayer = -1;
        }
        return;
    }
    if (mode == 2) {
        i = player;
    }
    for (; i < 4; i++) {
        if (i < 0) {
            break;
        }
        s32 onLayer = 0;
        if (getPlayerLayer_8023DE58(i) < 8 && actor->getField_0x4() == 8) {
            onLayer = 2;
        } else if (actor->getField_0x4() == getPlayerLayer_8023DE58(i)) {
            onLayer = 1;
        }
        if (fn_8023F264(i) && !fn_80230188(i) && !fn_8022DCD8(i) && onLayer != 0 &&
            !fn_8022F478(i) && !fn_80236B50(i) && !fn_8022D534(i))
        {
            f32 dx;
            f32 dy;
            JGeometry::TBox2f box;
            if (onLayer == 1) {
                dx = actor->mPos.x - getPlayerPos_8023E724(i)->x;
                dy = actor->mPos.y - getPlayerPos_8023E724(i)->y;
            } else {
                box = *layerBox(getPlayerLayer_8023DE58(i));
                dx = actor->mPos.x - (getPlayerPos_8023E724(i)->x - box.i.x);
                dy = actor->mPos.y - (getPlayerPos_8023E724(i)->y - box.i.y);
            }
            if (mode == 0 || mode == 2) {
                if (bestDist == -1.0f) {
                    bestDist = std::sqrtf(dx * dx + dy * dy);
                    angle = (u16)(JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                    found = i;
                } else if (bestDist > std::sqrtf(dx * dx + dy * dy)) {
                    bestDist = std::sqrtf(dx * dx + dy * dy);
                    angle = (u16)(JMath::atanTable_.atan2_(-dx, -dy) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                    found = i;
                }
            }
            if ((mode == 1 || mode == 3) && (mode != 3 || i != player) &&
                bestA < fn_80236228(i))
            {
                bestA = fn_80236228(i);
                bestDist = std::sqrtf(dx * dx + dy * dy);
                angle = (u16)(JMath::atanTable_.atan2_(-dx, -dy) *
                        (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                found = i;
            }
            if ((mode == 4 || mode == 5) && (mode != 5 || i != player) &&
                bestB < fn_80237774(i))
            {
                bestB = fn_80237774(i);
                bestDist = std::sqrtf(dx * dx + dy * dy);
                angle = (u16)(JMath::atanTable_.atan2_(-dx, -dy) *
                        (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180()));
                found = i;
            }
            if (mode == 2) {
                break;
            }
        }
    }
    if (found < 0) {
        if (outDir != NULL) {
            *outDir = -1;
        }
        if (outPlayer != NULL) {
            *outPlayer = -1;
        }
    } else {
        if (outAngle != NULL) {
            *outAngle = angle;
        }
        if (outDir != NULL) {
            *outDir = fn_801F681C(angle);
        }
        if (outDist != NULL) {
            *outDist = bestDist;
        }
        if (outPlayer != NULL) {
            *outPlayer = found;
        }
    }
}

// FIXME: 99.6. Scheduling and float registers in the prologue, as in
// fn_801EE558; literals @NNN.
// fn_801EE558 that also skips players failing fn_80230188 or fn_8022DCD8;
// with no player found the direction is zero (and the distance unset).
void ActorBase_804A5770::fn_801EE8F4(JGeometry::TVec3f* outDir, f32* outDist, s32 player) {
    JGeometry::TVec3f best(0.0f, 0.0f, 0.0f);
    JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
    pos.y = pos.y - pos.z;
    JGeometry::TVec3f diff;
    bool first = true;
    f32 bestDist;
    for (s32 i = 0; i < 4; i++) {
        if (player != 0) {
            i = player - 1;
        }
        if (fn_8023F264(i) && fn_801F320C(i) && !fn_80230188(i) && !fn_8022DCD8(i) &&
            !fn_8022F478(i) && !fn_80236B50(i) &&
            !fn_8022D534(i))
        {
            diff.sub(field_0x1ac[i], pos);
            if (first) {
                first = false;
                bestDist = diff.squared();
                best.set(diff);
            } else {
                f32 dist = diff.squared();
                if (dist < bestDist) {
                    bestDist = dist;
                    best.set(diff);
                }
            }
        }
        if (player != 0) {
            break;
        }
    }
    if (outDir != NULL) {
        best.z = 0.0f;
        if (outDist != NULL) {
            *outDist = outDir->normalize(best);
        } else {
            outDir->normalize(best);
        }
    } else if (outDist != NULL) {
        *outDist = JGeometry::TUtilf::sqrt(bestDist);
    }
}

// FIXME: 99.4. Scheduling and float registers only: the original loads
// mPos.y before mPos.z and x last for `pos`; the literals are @NNN until the
// unit's .sdata2 is in order.
// The nearest player (all four, or only player - 1): the direction from
// this actor's top-down position (x, y - z) to the player's position in
// field_0x1ac[] goes normalised to *outDir (z 0), the distance to *outDist.
// With no player found the direction is (0, 512, 0).
void ActorBase_804A5770::fn_801EE558(JGeometry::TVec3f* outDir, f32* outDist, s32 player) {
    JGeometry::TVec3f best(0.0f, 512.0f, 0.0f);
    JGeometry::TVec3f pos(mPos.x, mPos.y, mPos.z);
    pos.y = pos.y - pos.z;
    JGeometry::TVec3f diff;
    bool first = true;
    f32 bestDist = best.squared();
    for (s32 i = 0; i < 4; i++) {
        if (player != 0) {
            i = player - 1;
        }
        if (fn_8023F264(i) && fn_801F320C(i) && !fn_8022F478(i) && !fn_80236B50(i) &&
            !fn_8022D534(i))
        {
            diff.sub(field_0x1ac[i], pos);
            if (first) {
                first = false;
                bestDist = diff.squared();
                best.set(diff);
            } else {
                f32 dist = diff.squared();
                if (dist < bestDist) {
                    bestDist = dist;
                    best.set(diff);
                }
            }
        }
        if (player != 0) {
            break;
        }
    }
    if (outDir != NULL) {
        best.z = 0.0f;
        if (outDist != NULL) {
            *outDist = outDir->normalize(best);
        } else {
            outDir->normalize(best);
        }
    } else if (outDist != NULL) {
        *outDist = JGeometry::TUtilf::sqrt(bestDist);
    }
}

// Starts the two effects at mPos (fn_801CB71C), one into each emitter slot.
void ActorBase_804A5770::fn_801EE508() {
    fn_801CB71C(&mEmitter100, &mPos, 0, -1);
    fn_801CB71C(&mEmitter104, &mPos, 0, 1);
}

f32 ActorBase_804A5770::fn_801EE4F0() {
    return lbl_8046A00C[field_0xc0];
}

// Lets an emitter that is still on screen finish its particles; deletes one
// that is off screen at once. The destructor inlines it.
void releaseEmitter_801EE434(JPABaseEmitter* emitter) {
    if (emitter != NULL) {
        GameView_804A9950* view = getGameView_802D6A9C();
        f32 x = emitter->mGlobalTrs.x;
        f32 y = emitter->mGlobalTrs.y - emitter->mGlobalTrs.z;
        bool onScreen = false;
        if (x >= view->mVisibleBox.i.x && y >= view->mVisibleBox.i.y &&
            x < view->mVisibleBox.f.x && y < view->mVisibleBox.f.y)
        {
            onScreen = true;
        }
        if (onScreen) {
            emitter->clearStatus(JPAEMIT_Immortal);
            emitter->becomeInvalidEmitter();
        } else {
            getEffectSystem_8013C824()->mEmitterManager->forceDeleteEmitter(emitter);
        }
    }
}

// Whether a map tile code is one of the listed kinds.
// The code arrives as an int (the parameter is never extended; each test
// truncates to 16 bits: subi; clrlwi; cmplwi, and one clrlwi for the
// equality tests).
bool fn_801EE3C0(s32 tile) {
    if ((u16)(tile - 0x68) <= 3 || (u16)(tile - 0x70) <= 1 || (u16)(tile - 0xAA) <= 1 ||
        (u16)tile == 0x159 || (u16)(tile - 0x74) <= 1 || (u16)tile == 0x146 || (u16)tile == 0xFA ||
        (u16)tile == 0x4E)
    {
        return true;
    }
    return false;
}

// Finds a 'WARP' actor on this layer with type 8 (field_0x90's low byte)
// and moves the actor through it: to the layer in the warp's bits 24-26
// (0 stays 0, n goes to n + 7), at the same place relative to that layer's
// view box, with z 200. Returns whether a warp was found.
bool ActorBase_804A5770::fn_801EE24C() {
    s32 serial = -1;
    for (;;) {
        serial = getActorManager_801F856C()->fn_801F78F4('WARP', serial);
        if (serial < 0) {
            break;
        }
        ActorBase_804A5770* warp = getActorManager_801F856C()->findActorBySerial_801F666C(serial);
        if (warp == NULL || field_0x4 != warp->field_0x4 || (warp->field_0x90 & 0xFF) != 8) {
            continue;
        }
        JGeometry::TVec3f pos = mPos;
        s32 layer = (warp->field_0x90 >> 24) & 7;
        if (layer != 0) {
            layer += 7;
        }
        pos.x -= layerBox(field_0x4)->i.x;
        pos.y -= layerBox(field_0x4)->i.y;
        pos.x += layerBox(layer)->i.x;
        pos.y += layerBox(layer)->i.y;
        pos.z = 200.0f;
        mPos = pos;
        field_0x4 = layer;
        return true;
    }
    return false;
}
