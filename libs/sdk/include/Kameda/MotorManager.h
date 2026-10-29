#ifndef MOTORMANAGER_H
#define MOTORMANAGER_H

#include "JSystem/JKernel/JKRDisposer.h"
#include "Kameda/Motor.h"
#include "types.h"

class MotorManager : JKRDisposer
{
public:
    enum MotorType
    {
        MotorType_0,
        MotorType_1,
        MotorType_2,
        MotorType_3,
        MotorType_4,
        MotorType_5,
        MotorType_6,
        MotorType_7,
        MotorType_8,
        MotorType_9,
        MotorType_10,
        MotorType_11,
        MotorType_12,
        MotorType_13,
        MotorType_14,
        MotorType_15,
        MotorType_16,
        MotorType_17,
        MotorType_18,
        MotorType_19,
        MotorType_20,
        MotorType_21,
        MotorType_22,
        MotorType_23,
        MotorType_24,
        MotorType_MAX,
    };

    MotorManager(JKRHeap *heap);  // 0x80130e58
    virtual ~MotorManager();      // 0x80130ee0

    void reset();  // 0x80130f48
    void exec();   // 0x80130fc0

    static void setMotor(MotorType motorType, int kart);  // 0x8013103c
    static void setAwardMotor(MotorType motorType);       // 0x80131218
    static void setPowerMotor(int kart,
                              f32 strength,
                              u8 phase1Duration,
                              u8 phase2Duration);  // 0x801312e8

    static MotorManager *getManager() { return mThis; };

private:
    static int mCountFrame[8][MotorType_MAX];  // 0x803fb640

    static MotorManager *mThis;  // 0x80416280

    Motor *mMotor;    // 0x18
    int mKartNumber;  // 0x1c
};

#endif  // MOTORMANAGER_H
