#ifndef MOTOR_H
#define MOTOR_H

#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JORReflexible.h"
#include "Kaneshige/HioMgr.h"
#include "Osako/kartPad.h"
#include "types.h"

class MotorParam : public JORReflexible
{
public:
    MotorParam();
    virtual ~MotorParam() {}

    struct
    {
        bool flag;          // 0x48
        u8 _49[0x100 - 1];  // 0x49
    } mUnknownContainer[4];

    f32 mUnknownFloat;  // 0x448
    u8 mUnknownValue;   // 0x44c
    bool mUnknownFlag;  // 0x44d
};

class MotorHioNode : public HioNode
{
public:
    MotorHioNode() : HioNode("モーター", &mParam, 0, 0) {}
    virtual ~MotorHioNode() {}

    MotorParam mParam;
};

class Motor
{
public:
    Motor(JKRHeap *heap);                                   // 0x801304d8
    void init();                                            // 0x801305c4
    ~Motor();                                               // 0x80130668
    void exec();                                            // 0x8013073c
    void loopWave(KartGamePad *, int);                      // Unused
    void loopWave(KartGamePad *, char *);                   // Unused
    void checkLoop(int, const char *);                      // Unused
    void triggerWave(KartGamePad *kartGamePad, int index);  // 0x80130ad8
    void triggerWave(KartGamePad *, char *);                // Unused
    void powerWave(KartGamePad *kartGamePad,
                   f32 strength,
                   u8 phase1Duration,
                   u8 phase2Duration);  // 0x80130b50
    void stop();                        // 0x80130cbc

private:
    static const char *mPatternWave[26];  // 0x803942e8

    struct
    {
        KartGamePad *mKartGamePad;  // 0x0

        struct
        {
            const char *pattern;  // 0x4
            s32 pos;              // 0x8
        } mDriverMotor[4];

        struct
        {
            const char *pattern;  // 0x24
            s32 pos;              // 0x28
        } mPartnerMotor[4];

        s32 mUnknown[4];  // 0x44
    } mMotorData[4];

    s32 mUnknown;    // 0x150
    s32 mSomeCount;  // 0x154

    MotorHioNode *mHioNode;  // 0x158 (344)
};

#endif  // MOTOR_H
