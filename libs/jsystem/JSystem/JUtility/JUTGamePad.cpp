#include "JSystem/JUtility/JUTGamePad.h"

#include <math.h>
#include <std/math.h>

u32 JUTGamePad::CRumble::sChannelMask[PAD_CONTROLLER_NUM] = {
    0x80000000 >> 0,
    0x80000000 >> 1,
    0x80000000 >> 2,
    0x80000000 >> 3};

static u32 channel_mask[PAD_CONTROLLER_NUM] = {
    0x80000000 >> 0,
    0x80000000 >> 1,
    0x80000000 >> 2,
    0x80000000 >> 3};

JUTGamePad::EStickMode JUTGamePad::sStickMode = Clamped;
JUTGamePad::EClampMode JUTGamePad::sClampMode = Clamp;
f32 JUTGamePad::sPressPoint = 0.5f;
f32 JUTGamePad::sReleasePoint = 0.25f;
u32 JUTGamePad::C3ButtonReset::sResetPattern = START | X | B;
u32 JUTGamePad::C3ButtonReset::sResetMaskPattern = 0xFFFF;

JSUList<JUTGamePad> JUTGamePad::mPadList(false);

PADStatus JUTGamePad::mPadStatus[PAD_CONTROLLER_NUM];
JUTGamePad::CButton JUTGamePad::mPadButton[PAD_CONTROLLER_NUM];
JUTGamePad::CStick JUTGamePad::mPadMStick[PAD_CONTROLLER_NUM];
JUTGamePad::CStick JUTGamePad::mPadSStick[PAD_CONTROLLER_NUM];

bool JUTGamePad::mListInitialized = false;
u8 JUTGamePad::mPadAssign[PAD_CONTROLLER_NUM];
u32 JUTGamePad::sSuppressPadReset = 0;
u32 JUTGamePad::sAnalogMode = 0;
u32 JUTGamePad::sRumbleSupported = 0;
u8 JUTGamePad::CRumble::mStatus[PAD_CONTROLLER_NUM];
u32 JUTGamePad::CRumble::mEnabled = 0;
JUTResetBtnCb JUTGamePad::C3ButtonReset::sCallback = nullptr;
void *JUTGamePad::C3ButtonReset::sCallbackArg = nullptr;
OSTime JUTGamePad::C3ButtonReset::sThreshold = (OSTime)(OS_TIMER_CLOCK / 60) * 30;
bool JUTGamePad::C3ButtonReset::sResetSwitchPushing = false;
bool JUTGamePad::C3ButtonReset::sResetOccurred = false;
s32 JUTGamePad::C3ButtonReset::sResetOccurredPort = 0;

JSUList<JUTGamePadLongPress> JUTGamePadLongPress::sPatternList(false);

JUTGamePad::JUTGamePad(EPadPort port)
    : mButtons(),
      mMainStick(),
      mSubStick(),
      mRumble(this),
      mLink(this),
      mButtonReset()
{
    mPortNum = port;
    if (mPortNum >= 0)
    {
        mPadAssign[port]++;
    }
    initList();
    JUTGamePad::mPadList.append(&mLink);
    update();
    mPadRecord = nullptr;
    mPadReplay = nullptr;
    clear();
}

JUTGamePad::JUTGamePad()
    : mButtons(),
      mMainStick(),
      mSubStick(),
      mRumble(this),
      mLink(this),
      mButtonReset()
{
    mPortNum = -1;
    initList();
    JUTGamePad::mPadList.append(&mLink);
    mPadRecord = nullptr;
    mPadReplay = nullptr;
    clear();
}

JUTGamePad::~JUTGamePad()
{
    if (mPortNum >= 0)
    {
        mPadAssign[mPortNum]--;
        mPortNum = -1;
    }

    JUTGamePad::mPadList.remove(&mLink);
}

void JUTGamePad::initList()
{
    if (!JUTGamePad::mListInitialized)
    {
        JUTGamePad::mPadList.initiate();
        JUTGamePad::mListInitialized = true;
    }
}

void JUTGamePad::init()
{
    PADSetSpec(5);
    setAnalogMode(3);
    PADInit();
}

void JUTGamePad::clear()
{
    mButtonReset.mReset = false;
    _A8 = 1;
}

u32 JUTGamePad::read()
{
    sRumbleSupported = PADRead(mPadStatus);

    switch (sClampMode)
    {
    case Clamp:
        PADClamp(mPadStatus);
        break;
    case ClampCircle:
        PADClampCircle(mPadStatus);
        break;
    }

    u32 mask_tmp;
    u32 mask = 0;
    for (int i = 0; i < PAD_CONTROLLER_NUM; i++)
    {
        mask_tmp = 0x80000000 >> i;
        if (mPadStatus[i].err == PAD_ERR_NONE)
        {
            u32 stick_buttons = mPadMStick[i].update(mPadStatus[i].stickX, mPadStatus[i].stickY,
                                                     sStickMode, WhichStick_MainStick, mPadButton[i].mButton)
                                << 0x18;
            stick_buttons |= mPadSStick[i].update(mPadStatus[i].substickX, mPadStatus[i].substickY,
                                                  sStickMode, WhichStick_SubStick, mPadButton[i].mButton)
                             << 0x10;
            mPadButton[i].update(&mPadStatus[i], stick_buttons);
        }
        else if (mPadStatus[i].err == PAD_ERR_NO_CONTROLLER)
        {
            mPadMStick[i].update(0, 0, sStickMode, WhichStick_MainStick, 0);
            mPadSStick[i].update(0, 0, sStickMode, WhichStick_SubStick, 0);
            mPadButton[i].update((PADStatus *)nullptr, 0);

            if (!(sSuppressPadReset & mask_tmp))
            {
                mask |= mask_tmp;
            }
        }
        else
        {
            mPadButton[i].mTrigger = 0;
            mPadButton[i].mRelease = 0;
            mPadButton[i].mRepeat = 0;
        }
    }

    JSUListIterator<JUTGamePad> pad = (mPadList.getFirst());
    for (; pad != mPadList.getEnd(); ++pad)
    {
        if (pad->getPadReplay() && pad->getPadReplay()->isActive())
        {
            PADStatus status;
            pad->getPadReplay()->read(&status);
            u32 stick = pad->mMainStick.update(status.stickX, status.stickY, sStickMode,
                                               WhichStick_MainStick, pad->mButtons.mButton)
                        << 0x18;
            stick |= pad->mSubStick.update(status.substickX, status.substickY, sStickMode,
                                           WhichStick_SubStick, pad->mButtons.mButton)
                     << 0x10;
            pad->mButtons.update(&status, stick);
        }
        else
        {
            if (pad->mPortNum == -1)
                pad->assign();
            pad->update();
        }

        if (pad->getPadRecord() && pad->getPadRecord()->isActive())
        {
            if (pad->mPortNum >= 0)
            {
                int portNum = pad->mPortNum;
                if (mPadStatus[portNum].err == PAD_ERR_NONE)
                    pad->getPadRecord()->write(&mPadStatus[portNum]);
            }
        }
    }

    if (mask != 0)
    {
        PADReset(mask);
    }

    checkResetSwitch();
    return sRumbleSupported;
}

void JUTGamePad::assign()
{
    for (s32 i = 0; i < PAD_CONTROLLER_NUM; i++)
    {
        if (JUTGamePad::mPadStatus[i].err == 0 && JUTGamePad::mPadAssign[i] == 0)
        {
            mPortNum = i;
            JUTGamePad::mPadAssign[i] = 1;
            JUTGamePad::mPadButton[i].setRepeat(mButtons.mRepeatMask, mButtons.mRepeatDelay, mButtons.mRepeatFrequency);
            mRumble.clear(this);
            break;
        }
    }
}

void JUTGamePad::checkResetCallback(OSTime time)
{
    if (time >= JUTGamePad::C3ButtonReset::sThreshold)
    {
        JUTGamePad::C3ButtonReset::sResetOccurred = true;
        JUTGamePad::C3ButtonReset::sResetOccurredPort = mPortNum;

        if (JUTGamePad::C3ButtonReset::sCallback)
        {
            (*JUTGamePad::C3ButtonReset::sCallback)(mPortNum, JUTGamePad::C3ButtonReset::sCallbackArg);
        }
    }
}

void JUTGamePad::update()
{
    if (mPortNum != -1)
    {
        if (mPortNum >= 0 && mPortNum < PAD_CONTROLLER_NUM)
        {
            mButtons = JUTGamePad::mPadButton[mPortNum];
            mMainStick = JUTGamePad::mPadMStick[mPortNum];
            mSubStick = JUTGamePad::mPadSStick[mPortNum];
            mErrorStatus = JUTGamePad::mPadStatus[mPortNum].err;
        }

        if (_A8 == 0 || JUTGamePad::C3ButtonReset::sResetPattern != (mButtons.mButton & JUTGamePad::C3ButtonReset::sResetMaskPattern))
        {
            mButtonReset.mReset = false;
        }
        else if (JUTGamePad::C3ButtonReset::sResetOccurred == false)
        {
            if (mButtonReset.mReset == true)
            {
                s64 diff = OSGetTime() - mResetTime;
                checkResetCallback(diff);
            }
            else
            {
                mButtonReset.mReset = true;
                mResetTime = OSGetTime();
            }
        }

        JSUListIterator<JUTGamePadLongPress> pad = JUTGamePadLongPress::sPatternList.getFirst();
        for (; pad != JUTGamePadLongPress::sPatternList.getEnd(); ++pad)
        {
            if (pad->isValid())
            {
                if (mPortNum >= 0 && mPortNum < PAD_CONTROLLER_NUM)
                {
                    if ((mButtons.mButton & pad->getMaskPattern()) == pad->getPattern())
                    {
                        if (pad->_20[mPortNum] == true)
                        {
                            s64 diff = OSGetTime() - pad->mTimer[mPortNum];
                            pad->checkCallback(mPortNum, diff);
                        }
                        else
                        {
                            pad->_20[mPortNum] = true;
                            pad->mTimer[mPortNum] = OSGetTime();
                        }
                    }
                    else
                    {
                        pad->_20[mPortNum] = false;
                    }
                }
            }
        }

        if (mPortNum >= 0 && mPortNum < PAD_CONTROLLER_NUM)
        {
            mRumble.update(mPortNum);
        }
    }
}

void JUTGamePad::checkResetSwitch()
{
    if (!JUTGamePad::C3ButtonReset::sResetOccurred)
    {
        if (OSGetResetSwitchState() != 0)
        {
            JUTGamePad::C3ButtonReset::sResetSwitchPushing = true;
        }
        else
        {
            if (JUTGamePad::C3ButtonReset::sResetSwitchPushing == true)
            {
                JUTGamePad::C3ButtonReset::sResetOccurred = true;
                JUTGamePad::C3ButtonReset::sResetOccurredPort = -1;

                if (*JUTGamePad::C3ButtonReset::sCallback != nullptr)
                {
                    (*JUTGamePad::C3ButtonReset::sCallback)(-1, JUTGamePad::C3ButtonReset::sCallbackArg);
                }
            }

            JUTGamePad::C3ButtonReset::sResetSwitchPushing = false;
        }
    }
}

void JUTGamePad::clearForReset()
{
    CRumble::setEnabled(0);
    recalibrate(PAD_CHAN0_BIT | PAD_CHAN1_BIT | PAD_CHAN2_BIT | PAD_CHAN3_BIT);
}

void JUTGamePad::CButton::clear()
{
    mButton = 0;
    mTrigger = 0;
    mRelease = 0;
    mRepeat = 0;
    mAnalogA = 0;
    mAnalogB = 0;
    mAnalogL = 0;
    mAnalogR = 0;
    mRepeatTimer = 0;
    mRepeatLastButton = 0;
    mRepeatMask = 0;
    mRepeatDelay = 0;
    mRepeatFrequency = 0;
}

void JUTGamePad::CButton::update(const PADStatus *padStatus, u32 stick_buttons)
{
    u32 tempButtons;
    u32 buttons;

    if (padStatus != nullptr)
    {
        tempButtons = padStatus->button;
    }
    else
    {
        tempButtons = 0;
    }

    buttons = stick_buttons | tempButtons;
    mRepeat = 0;

    if (mRepeatDelay != 0)
    {
        if (mRepeatMask != 0)
        {
            u32 repeatButtons = buttons & mRepeatMask;
            mRepeat = 0;

            if (repeatButtons == 0)
            {
                mRepeatLastButton = 0;
                mRepeatTimer = 0;
            }
            else if (mRepeatLastButton == repeatButtons)
            {
                mRepeatTimer++;
                if (
                    mRepeatTimer == mRepeatDelay ||
                    (mRepeatTimer > mRepeatDelay && ((mRepeatTimer - mRepeatDelay) % mRepeatFrequency) == 0))
                {
                    mRepeat = repeatButtons;
                }
            }
            else
            {
                mRepeat = repeatButtons & (mRepeatLastButton ^ 0xFFFFFFFF);
                mRepeatLastButton = repeatButtons;
                mRepeatTimer = 0;
            }
        }
    }

    mTrigger = buttons & (buttons ^ mButton);
    mRelease = mButton & (buttons ^ mButton);
    mButton = buttons;
    mRepeat |= (mRepeatMask ^ 0xFFFFFFFF) & mTrigger;

    if (padStatus != nullptr)
    {
        mAnalogA = padStatus->analogA;
        mAnalogB = padStatus->analogB;
        mAnalogL = padStatus->triggerLeft;
        mAnalogR = padStatus->triggerRight;
    }
    else
    {
        mAnalogA = 0;
        mAnalogB = 0;
        mAnalogL = 0;
        mAnalogR = 0;
    }

    mAnalogLf = (f32)(int)mAnalogL / 150.0f;
    mAnalogRf = (f32)(int)mAnalogR / 150.0f;
}

void JUTGamePad::CStick::clear()
{
    mX = 0.0f;
    mY = 0.0f;
    mValue = 0.0f;
    mAngle = 0;
}

u32 JUTGamePad::CStick::update(s8 x, s8 y, EStickMode stickMode, EWhichStick whichStick, u32 mask)
{
    s32 clamp;
    switch (getClampMode())
    {
    case Clamp:
        clamp = whichStick == WhichStick_MainStick ? 54 : 42;
        break;
    case ClampCircle:
        clamp = whichStick == WhichStick_MainStick ? 38 : 29;
        break;
    default:
        clamp = whichStick == WhichStick_MainStick ? 69 : 57;
        break;
    }

    mAnalogX = x;
    mAnalogY = y;
    mX = (f32)x / clamp;
    mY = (f32)y / clamp;

    mValue = std::sqrtf(mX * mX + mY * mY); // calls real sqrtf

    if (mValue > 1.0f)
    {
        if (stickMode == Clamped)
        {
            mX /= mValue;
            mY /= mValue;
        }

        mValue = 1.0f;
    }

    if (mValue > 0.0f)
    {
        if (mY == 0.0f)
        {
            if (mX > 0.0f)
            {
                mAngle = 0x4000;
            }
            else
            {
                mAngle = -0x4000;
            }
        }
        else
        {
            mAngle = atan2f(mX, -mY) * 10430.379f; //((f32)0x8000 / F_PI);
        }
    }
    return getButton(mask >> (whichStick == WhichStick_MainStick ? 0x18 : 0x10));
}

u32 JUTGamePad::CStick::getButton(u32 mask)
{
    u32 button = mask & 0xf;
    if (-sReleasePoint < mX && mX < sReleasePoint)
    {
        button &= ~(DPAD_LEFT | DPAD_RIGHT);
    }
    else if (mX <= -sPressPoint)
    {
        button &= ~DPAD_RIGHT;
        button |= DPAD_LEFT;
    }
    else if (mX >= sPressPoint)
    {
        button &= ~DPAD_LEFT;
        button |= DPAD_RIGHT;
    }

    if (-sReleasePoint < mY && mY < sReleasePoint)
    {
        button &= ~(DPAD_DOWN | DPAD_UP);
    }
    else if (mY <= -sPressPoint)
    {
        button &= ~DPAD_UP;
        button |= DPAD_DOWN;
    }
    else if (mY >= sPressPoint)
    {
        button &= ~DPAD_DOWN;
        button |= DPAD_UP;
    }

    return button;
}

void JUTGamePad::CRumble::clear()
{
    mFrame = 0;
    mLength = 0;
    mData = nullptr;
    mFrameCount = 0;
    _10 = 0;
    JUTGamePad::CRumble::mEnabled = 0xF0000000;
}

void JUTGamePad::CRumble::clear(JUTGamePad *gamePad)
{
    if (0 <= (s16)gamePad->getPortNum() && (s16)gamePad->getPortNum() < PAD_CONTROLLER_NUM)
    {
        JUTGamePad::CRumble::mStatus[(s16)gamePad->getPortNum()] = 0;
        gamePad->stopMotorHard();
    }

    clear();
}

void JUTGamePad::CRumble::startMotor(int port)
{
    if (JUTGamePad::CRumble::isEnabledPort(port))
    {
        PADControlMotor(port, PAD_MOTOR_RUMBLE);
        JUTGamePad::CRumble::mStatus[port] = 1;
    }
}

void JUTGamePad::CRumble::stopMotor(int port, bool hard)
{
    if (JUTGamePad::CRumble::isEnabledPort(port))
    {
        PADControlMotor(port, (u8)(hard ? PAD_MOTOR_STOP_HARD : PAD_MOTOR_STOP));
        JUTGamePad::CRumble::mStatus[port] = 0;
    }
}

static bool getNumBit(u8 *data, int bit)
{
    u8 numBit = ((0x80 >> (bit & 7)) & data[bit >> 3] );
    return numBit != 0;
}

void JUTGamePad::CRumble::update(s16 port)
{
    if (!isEnabledPort(port))
    {
        mFrame = 0;
        mLength = 0;
        mData = nullptr;
        mFrameCount = 0;
        _10 = nullptr;
    }

    if (mLength != 0)
    {
        if (mFrame >= mLength)
        {
            JUTGamePad::CRumble::stopMotorHard(port);
            mLength = 0;
        }
        else
        {
            if (mFrameCount == 0)
            {
                if (JUTGamePad::CRumble::mStatus[port] == 0)
                {
                    JUTGamePad::CRumble::startMotor(port);
                }

                return;
            }

            bool enable = getNumBit(mData, mFrame % mFrameCount);
            bool status = JUTGamePad::CRumble::mStatus[port];
            if (enable && !status)
            {
                JUTGamePad::CRumble::startMotor(port);
            }
            else if (!enable)
            {
                bool hardstop = false;
                if (_10)
                {
                    hardstop = getNumBit(_10, mFrame % mFrameCount);
                }

                if (status)
                    JUTGamePad::CRumble::stopMotor(port, hardstop);
                else if (hardstop)
                    JUTGamePad::CRumble::stopMotorHard(port);
            }
        }

        mFrame++;
    }
}

void JUTGamePad::CRumble::triggerPatternedRumble(u32 length)
{
    if (mData != nullptr && mFrameCount != 0)
    {
        mLength = length;
        mFrame = 0;
    }
}

void JUTGamePad::CRumble::startPatternedRumble(void *data, ERumble rumble, u32 length)
{
    mFrameCount = (*(u8 *)data << 8) + *((u8 *)data + 1);
    mData = (u8 *)data + 2;

    switch (rumble)
    {
    case Rumble0:
        triggerPatternedRumble(mFrameCount);
        break;
    case Rumble1:
        triggerPatternedRumble(-1);
        break;
    case Rumble2:
        triggerPatternedRumble(length);
        break;
    }
}

void JUTGamePad::CRumble::stopPatternedRumble(s16 port)
{
    mLength = 0;
    stopMotorHard(port);
}

JUTGamePad *JUTGamePad::getGamePad(int port)
{
    JSUListIterator<JUTGamePad> pad(mPadList.getFirst());
    for (; pad != mPadList.getEnd(); ++pad)
    {
        if (port == pad->mPortNum)
        {
            return pad.getObject();
        }
    }

    return nullptr;
}

void JUTGamePad::CRumble::setEnabled(u32 mask)
{
    for (int i = 0; i < PAD_CONTROLLER_NUM; i++)
    {
        if ((mEnabled & channel_mask[i]) == 0)
        {
            if (mStatus[i])
            {
                stopMotor(i, false);
            }

            JUTGamePad *pad = getGamePad(i);
            if (pad != nullptr)
            {
                pad->mRumble.stopPatternedRumble(pad->mPortNum);
            }
        }
    }

    mEnabled = mask & (PAD_CHAN0_BIT | PAD_CHAN1_BIT | PAD_CHAN2_BIT | PAD_CHAN3_BIT);
}

void JUTGamePad::CButton::setRepeat(u32 repeatMask, u32 repeatDelay, u32 repeatFreq)
{
    mRepeatLastButton = 0;
    mRepeatTimer = 0;
    mRepeatMask = repeatMask;
    mRepeatDelay = repeatDelay;
    mRepeatFrequency = repeatFreq;
}

void JUTGamePad::setButtonRepeat(u32 repeatMask, u32 repeatDelay, u32 repeatFreq)
{
    mButtons.setRepeat(repeatMask, repeatDelay, repeatFreq);

    if (mPortNum >= 0)
        mPadButton[mPortNum].setRepeat(repeatMask, repeatDelay, repeatFreq);
}

bool JUTGamePad::recalibrate(u32 channels)
{
    for (int i = 0; i < PAD_CONTROLLER_NUM; i++)
    {
        if ((JUTGamePad::sSuppressPadReset & channel_mask[i]) != 0)
        {
            channels &= channel_mask[i] ^ 0xFFFFFFFF;
        }
    }
    BOOL res = PADRecalibrate(channels);
    return res;
}

void JUTGamePadLongPress::checkCallback(int port, u32 timer)
{
    if (port >= 0)
    {
        // assert not present in mkdd debug? interesting
        // JUT_ASSERT(0 <= port && port < 4) 
        if (timer >= _1C)
        {
            _11 = true;
            _48[port] = true;
            if (mCallback)
            {
                mCallback(port, this, _50);
            }
        }
    }
}