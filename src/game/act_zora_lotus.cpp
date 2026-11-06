#include "game/GameAudio.h"
#include "game/Effect.h"
#include "game/Player.h"
#include "game/act/act_zora_lotus.h"

static inline bool contains(const JGeometry::TBox2f& b, const JGeometry::TVec3f& p) {
    return p.x >= b.i.x && p.y >= b.i.y && p.x < b.f.x && p.y < b.f.y;
}

cls_804AC3E0::cls_804AC3E0() {
    field_0x2b8 = 0;
}

void cls_804AC3E0::init() {
    ZoraEnemy_804A3D7C::init();
    mPos.x -= 8.0f;
    mSpawnPos.x -= 8.0f;
    mPos.y -= 8.0f;
    mSpawnPos.y -= 8.0f;
    field_0x2b4 = 1.0f;
    field_0x2b1 = 1;
}

void cls_804AC3E0::vfunc_0xA4() {
    field_0x2ac[0] = 0;
    field_0x2ac[1] = 0;
    field_0x2ac[2] = 0;
    field_0x2ac[3] = 0;
    field_0x2b2 = 0;
    field_0x2b0 = 0;
    mPos.y = mSpawnPos.y;
}

// FIXME: 92.84 % (missing/extra + reloc-name: ours has 3 more instrs (198 vs 195); target vs ours: sincosTable___5JMath vs atanTable___5JMath).
void cls_804AC3E0::vfunc_0xA8() {
    if (field_0x2b0 != 0) {
        field_0x2b2 += 0x800;
        mPos.y = mSpawnPos.y + JMath::sincosTable_.sinShort(field_0x2b2);
        if (field_0x2b2 == 0) {
            field_0x2b0 = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        bool r28;
        if (field_0x4 != getPlayerLayer_8023DE58(i)) {
            r28 = false;
        } else {
            int j = 0;
            while (true) {
                JGeometry::TVec3f point;
                fn_8022E088(i, j, &point);
                point -= mPos;
                point.z = 0.0f;
                f32 rad = JMath::atanTable_.atan2_(point.y, point.x);
                s16 angle = rad * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                f32 dist = JGeometry::TUtil<f32>::sqrt(point.x * point.x + point.y * point.y);
                point.z = 0.0f;
                point.y = dist * JMath::sincosTable_.sinShort(angle);
                point.x = dist * JMath::sincosTable_.cosShort(angle);
                if (contains(mBox, point)) {
                    fn_8022DF48(i);
                    r28 = true;
                    break;
                }
                j++;
                if (j >= 8) {
                    r28 = false;
                    break;
                }
            }
        }
        if (fn_80233E40(i)) {
            if ((1 << i) & fn_801F2740(-1)) {
                if (field_0x90 & 1) {
                    fn_8022DC3C(i);
                }
            }
        } else {
            if (r28) {
                if (mPos.z <= 0.0f) {
                    JGeometry::TVec3f vec = mPos - mPrevPos;
                    fn_8023DF68(i, &vec);
                }
            }
            if (getPlayerPos_8023E724(i)->z > mPos.z) {
                r28 = false;
            }
            if (r28 != field_0x2ac[i]) {
                spawnEmitter_8013CC50(field_0x4, &mPos, 0x54e, -1, -2, NULL, NULL, true, 1.2f);
                field_0x2b0 = 1;
            }
            field_0x2ac[i] = r28;
        }
    }
}

bool cls_804AC3E0::vfunc_0xAC() {
    bool flag = false;
    if (field_0x90 & 1) {
        for (int i = 0; i < 4; i++) {
            if (fn_80233E40(i)) {
                f32 diffX = mPos.x - getPlayerPos_8023E724(i)->x;
                f32 diffY = mPos.y - getPlayerPos_8023E724(i)->y;
                f32 distSq = diffX * diffX + diffY * diffY;
                if (distSq < 9216.0f) {
                    f32 rad = JMath::atanTable_.atan2_(diffX, diffY);
                    s32 a = rad * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                    s16 angle = (u16)a + 0x3000;
                    JGeometry::TVec3f vec;
                    vec.y = JMath::sincosTable_.cosShort(angle);
                    vec.x = JMath::sincosTable_.sinShort(angle);
                    vec.z = 0.0f;
                    fn_8023DFF4(i, &vec);
                    flag = true;
                }
            }
        }
        if (flag) {
            if (field_0x2b8 == NULL) {
                field_0x2b8 = spawnEmitter_8013CC50(field_0x4, &mPos, 0x553, -1, -2, NULL, NULL, true, 2.0f);
            }
        } else {
            if (field_0x2b8 != NULL) {
                field_0x2b8->becomeInvalidEmitter();
                field_0x2b8 = NULL;
            }
        }
    }
    return flag;
}

void cls_804AC3E0::vfunc_0xB0() {
    bool flag = false;
    for (int i = 0; i < 4; i++) {
        if (field_0x2ac[i] != 0) {
            fn_802314D8(i, JGeometry::TVec3f(0.0f, 0.0f, 10.0f));
            flag = true;
        }
    }
    if (flag == true) {
        startSound_801F0E34(0x50082, 0);
        vfunc_0xA4();
    }
}

u8 cls_804AC3E0::vfunc_0x5C() {
    return 0;
}

u8 cls_804AC3E0::vfunc_0x60() {
    return 0;
}

u8 cls_804AC3E0::vfunc_0x68() {
    return 0;
}

// FIXME: 91.31 % (missing/extra + reloc-name: ours has 3 more instrs (534 vs 531); target vs ours: sincosTable___5JMath vs atanTable___5JMath).
void cls_804AC3E0::vfunc_0x90() {
    switch (mState) {
    case 2:
    case 3:
    case 5:
        if (mActorId == 'LTZR') {
            mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
            mBox2.set(mBox);
        }
        if (mState == 2 && mStateTimer < 3) {
            vfunc_0xA4();
        } else {
            JGeometry::TBox2f box(mBox);
            box.i.y += mPos.y;
            box.i.x += mPos.x;
            box.f.x += mPos.x;
            box.f.y += mPos.y;
            for (int i = 0; i < 4; i++) {
                if (fn_80233E40(i) == 1) {
                    fn_802399D8(i, &box, field_0x4, mSerial);
                } else if (fn_8023B0F4(i, &box, field_0x4, false, 8.0f) == 1) {
                    if (!fn_8022F478(i) || fn_8022CAF8(i) != 5) {
                        if (mState != 5) {
                            startSound_801F0E34(0x50032, 0);
                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x15b, 0, -2, NULL, NULL, true, 1.0f);
                            spawnEmitter_8013CC50(field_0x4, &mPos, 0x15c, -1, 0, NULL, NULL, true, 1.0f);
                        }
                        fn_801BACE8(0);
                    }
                    if (getPlayerLayer_8023DE58(i) == field_0x4) {
                        for (int j = 0; j < 8; j++) {
                            JGeometry::TVec3f point;
                            fn_8022E088(i, j, &point);
                            point -= mPos;
                            point.z = 0.0f;
                            f32 rad = JMath::atanTable_.atan2_(point.y, point.x);
                            s16 angle = rad * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                            f32 dist = JGeometry::TUtil<f32>::sqrt(point.x * point.x + point.y * point.y);
                            point.z = 0.0f;
                            point.y = dist * JMath::sincosTable_.sinShort(angle);
                            point.x = dist * JMath::sincosTable_.cosShort(angle);
                            if (contains(mBox, point)) {
                                fn_8022DF48(i);
                                break;
                            }
                        }
                    }
                }
            }
            if (mState == 5 && mActorId == 'LTZR') {
                mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
                mBox2 = mBox;
            }
        }
        break;
    }
    if (mState == 1) {
        if (field_0x2b1 == 1) {
            field_0x2b4 += 0.03f;
            if (field_0x2b4 >= 1.2f) {
                field_0x2b4 = 1.2f;
                field_0x2b1 = 0;
            }
        } else {
            field_0x2b4 -= 0.03f;
            if (field_0x2b4 <= 1.0f) {
                field_0x2b4 = 1.0f;
                field_0x2b1 = 1;
            }
        }
    } else {
        if (field_0x2b4 != 1.0f) {
            field_0x2b4 = 1.0f;
        }
    }
    switch (mState) {
    case 0:
        if (mActorId == 'LTZR') {
            mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
            mBox2.set(mBox);
        }
        vfunc_0xA8();
        if (mStateTimer >= field_0x250) {
            if (fn_801D05F8(&mPos, 300.0f) >= 0) {
                fn_801BACE8(1);
            }
        }
        break;
    case 1:
        startSound_801F0E34(0x50081, 0);
        if (mActorId == 'LTZR') {
            mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
            mBox2.set(mBox);
        }
        vfunc_0xA8();
        if (field_0x54.x != 1.5f) {
            fn_801CD950(&field_0x54.x, 1.5f, 0.05f);
            field_0x54.y = field_0x54.x;
            field_0x54.z = field_0x54.x;
        } else if (mStateTimer >= field_0x250 && mAnim.fn_801D217C()) {
            fn_801BACE8(2);
            vfunc_0xB0();
        }
        break;
    case 2:
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801BACE8(3);
        }
        break;
    case 3:
        if (mStateTimer >= field_0x250 && mAnim.fn_801D217C()) {
            fn_801BACE8(5);
        }
        break;
    case 5:
        if (mActorId == 'LTZR') {
            mBox.set(-12.0f, -12.0f, 12.0f, 12.0f);
            mBox2.set(mBox);
        }
        for (int i = 0; i < 4; i++) {
            if (getPlayerLayer_8023DE58(i) == field_0x4) {
                for (int j = 0; j < 8; j++) {
                    JGeometry::TVec3f point;
                    fn_8022E088(i, j, &point);
                    point -= mPos;
                    point.z = 0.0f;
                    f32 rad = JMath::atanTable_.atan2_(point.y, point.x);
                    s16 angle = rad * (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                    f32 dist = JGeometry::TUtil<f32>::sqrt(point.x * point.x + point.y * point.y);
                    point.z = 0.0f;
                    point.y = dist * JMath::sincosTable_.sinShort(angle);
                    point.x = dist * JMath::sincosTable_.cosShort(angle);
                    if (contains(mBox, point)) {
                        fn_8022DF48(i);
                        break;
                    }
                }
            }
        }
        if (mStateTimer > 1 && mAnim.fn_801D21F0()) {
            fn_801BACE8(0);
        }
        break;
    }
    vfunc_0xAC();
}

// Called out-of-line by cls_804AC3E0::draw
// FIXME: 99.50 % (regalloc-only: target->ours f24->f26, f26->f25, f25->f24).
#pragma auto_inline off
extern "C" void fn_8033FC4C(Mtx m, const JGeometry::TVec3f& scale, const JGeometry::TVec3f& rot, const JGeometry::TVec3f& trans) {
    f32 sx = sin(rot.x);
    f32 sy = sin(rot.y);
    f32 sz = sin(rot.z);
    f32 cx = cos(rot.x);
    f32 cy = cos(rot.y);
    f32 cz = cos(rot.z);

    f32 cy_sz = cy * sz;
    f32 neg_sy = -sy;
    f32 cy_cz = cy * cz;
    f32 sx_cy = sx * cy;
    f32 sx_sy = sx * sy;
    f32 cx_sz = cx * sz;
    f32 cx_cz = cx * cz;
    f32 sx_sz = sx * sz;
    f32 sx_cz = sx * cz;
    f32 cx_cy = cx * cy;

    m[0][0] = scale.x * cy_cz;
    m[1][0] = scale.x * cy_sz;
    m[2][0] = scale.x * neg_sy;

    m[0][1] = scale.y * (sx_sy * cz - cx_sz);
    m[1][1] = scale.y * (sx_sy * sz + cx_cz);
    m[2][1] = scale.y * sx_cy;

    m[0][2] = scale.z * (cx_cz * sy + sx_sz);
    m[1][2] = scale.z * (cx_sz * sy - sx_cz);
    m[2][2] = scale.z * cx_cy;

    m[0][3] = trans.x;
    m[1][3] = trans.y;
    m[2][3] = trans.z;
}
#pragma auto_inline reset

// 99.90 % (reloc-name only: target vs ours: jumptable_804AC494 vs @858).
void cls_804AC3E0::draw() {
    ZoraEnemy_804A3D7C::draw();
    SpriteParams_801F5AE0 params;
    params.field_0x20 = 0;
    params.field_0x21 = 1;
    params.field_0x1c = 0;
    Mtx mtx;
    PSMTXIdentity(mtx);
    JGeometry::TVec3f trans;
    trans.z = 0.0f;
    trans.y = 0.0f;
    trans.x = 0.0f;
    if (mState != 0) {
        switch (mAnim.mCel) {
        case 0:
        case 4:
            params.field_0x1d = 1;
            trans.z = 20.0f;
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            params.field_0x1d = 1;
            trans.z = 18.5f;
            break;
        case 2:
        case 6:
            params.field_0x1d = 1;
            trans.z = 17.5f;
            break;
        case 8:
            params.field_0x1d = 0;
            trans.z = 5.0f;
            break;
        default:
            if (mState == 2 || mState == 5) {
                params.field_0x1d = 0;
            } else {
                params.field_0x1d = -1;
            }
            break;
        }
    } else {
        params.field_0x1d = -1;
    }
    fn_8033FC4C(mtx, JGeometry::TVec3f(field_0x2b4, field_0x2b4, field_0x2b4), JGeometry::TVec3f(0.0f, 0.0f, 0.0f), trans);
    params.mMtx = mtx;
    drawSprite_801F06F0("enemy_gc_sob.bin", 0x173, &params, -1, 1.0f, 1.0f);
}

cls_804AC3E0::~cls_804AC3E0() {
}
