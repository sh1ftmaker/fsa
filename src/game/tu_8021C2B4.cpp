#include "game/GameAudio.h"
#include "JSystem/JMath/JMath.h"
#include "game/GameGlobals.h"
#include "game/Player.h"
#include "game/ActorManager.h"
#include "game/cls_8021C2B4.h"

cls_8021C5B0::cls_8021C5B0()
    : mActive(false), mDone(false), mPlayer(-1), mStart(0.0f, 0.0f), mTarget(0.0f, 0.0f),
      mPos(0.0f, 0.0f, 0.0f), mVel(0.0f, 0.0f, 0.0f) {}

void cls_8021C2B4::fn_8021C468() {
    for (int i = 0; i < 4; i++) {
        cls_8021C5B0* piece = &mPieces[i];
        if (fn_8023F264(i)) {
            JGeometry::TVec2f target;
            if (fn_801C0D70()->fn_801C0FEC(getPlayerPos_8023E724(i), &target)) {
                piece->mActive = true;
                piece->mDone = false;
                piece->mPlayer = i;
                JGeometry::TVec3f* playerPos = getPlayerPos_8023E724(piece->mPlayer);
                piece->mStart.x = playerPos->x;
                piece->mStart.y = playerPos->y;
                piece->mTarget.x = target.x;
                piece->mTarget.y = target.y;
                playerPos = getPlayerPos_8023E724(piece->mPlayer);
                piece->mPos.x = playerPos->x;
                piece->mPos.y = playerPos->y;
                piece->mPos.z = playerPos->z;
                piece->mVel.x = 0.0f;
                piece->mVel.y = 0.0f;
                piece->mVel.z = 0.0f;
                s32 angle = JMath::atanTable_.atan2_(piece->mStart.x - piece->mTarget.x,
                                                     piece->mStart.y - piece->mTarget.y) *
                            (32768.0f / JMath::TAngleConstant_<f32>::RADIAN_DEG180());
                piece->mAnim.fn_8021C9C4(fn_801F681C(angle));
                continue;
            }
        }
        piece->mDone = true;
    }
}

// FIXME: 99.7 %. Regalloc only in the vel.x / vel.y computation after
// fn_8021CB18: the target does fsubs f3, f4, f3 / fmuls f2, f6, f2 / fmuls f3, f6, f3
// (reusing the operand registers); ours puts the results in f4, f3, f2.
void cls_8021C2B4::fn_8021C2B4() {
    for (int i = 0; i < 4; i++) {
        cls_8021C5B0* piece = &mPieces[i];
        if (piece->mActive && !piece->mDone) {
            piece->mVel.z += -0.2f;
            piece->mPos += piece->mVel;
            if (piece->mPos.z <= 0.0f) {
                piece->mPos.z = 0.0f;
                piece->mVel.z = 0.0f;
            }
            if (piece->mAnim.fn_8021C848()) {
                switch (piece->mAnim.mType) {
                case 0: {
                    piece->mAnim.fn_8021CA70(1);
                    f32 frames = piece->mAnim.fn_8021CB18();
                    f32 dx = piece->mTarget.x - piece->mStart.x;
                    f32 dy = piece->mTarget.y - piece->mStart.y;
                    f32 scale = 1.0f / frames;
                    piece->mVel.x = scale * dx;
                    piece->mVel.y = scale * dy;
                    piece->mVel.z = 0.5f * (0.2f * frames);
                    break;
                }
                case 1:
                    if (piece->mPos.z <= 0.0f) {
                        piece->mAnim.fn_8021CA70(2);
                        piece->mVel.x = 0.0f;
                        piece->mVel.y = 0.0f;
                        piece->mVel.z = 0.0f;
                    }
                    break;
                case 2:
                    piece->mDone = true;
                    break;
                }
            }
            fn_80230498(piece->mPlayer, &piece->mPos, piece->mAnim.mFrames, 255, -128, -128, true);
        }
    }
}
