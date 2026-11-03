#include "JSystem/JAudio/System/JASVoiceBank.h"

const JASOscillator::Data JASVoiceBank::sOscData = {
    0, 1.0f, NULL, NULL, 1.0f, 0.0f,
};

JASOscillator::Data *JASVoiceBank::sOscTable;

bool JASVoiceBank::getInstParam(int prg_no, int, int, JASInstParam *param) const
{
    if (prg_no < 0) {
        return false;
    }
    sOscTable = (JASOscillator::Data *)&sOscData;
    param->_18 = prg_no;
    param->_20 = 1;
    param->_1c = &sOscTable;
    return true;
}

JASVoiceBank::~JASVoiceBank() {}

u32 JASVoiceBank::getType() const
{
    return 'VOIC';
}
