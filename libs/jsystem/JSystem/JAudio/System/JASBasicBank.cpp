#include "JSystem/JAudio/System/JASBank.h"
#include "JSystem/JAudio/System/JASCalc.h"
#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "types.h"

JASBasicBank::JASBasicBank() {
    JASCalc::bzero(mInstTable, sizeof(mInstTable));
}

void JASBasicBank::setInst(int prg_no, JASInst *inst) {
    JUT_ASSERT(prg_no < INST_MAX);
    JUT_ASSERT(prg_no >= 0);

    JUT_ASSERT(mInstTable[prg_no] == 0);
    mInstTable[prg_no] = inst;
}

bool JASBasicBank::getInstParam(int prg_no, int key, int vel, JASInstParam *param) const {
    JASInst *inst = getInst(prg_no);
    if (inst == NULL) {
        return false;
    }
    return inst->getParam(key, vel, param);
}

JASInst *JASBasicBank::getInst(int prg_no) const {
    if (prg_no < 0) {
        return NULL;
    }
    if (prg_no >= INST_MAX) {
        return NULL;
    }
    return mInstTable[prg_no];
}
