#include "PowerPC_EABI_Support/MetroTRK/trk.h"

static u8 bUseSerialIO;

/**
 * @note Size: 0xC
 */
void SetUseSerialIO(u8 sio)
{
	bUseSerialIO = sio;
	return;
}

/**
 * @note Size: 0x10
 */
u8 GetUseSerialIO(void)
{
	return bUseSerialIO;
}
