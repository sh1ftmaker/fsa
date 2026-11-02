#include "global.h"
#include "dolphin/os/OS.h"

/**
 * @note Size: 0x4
 */
void MWInitializeCriticalSection(uint* section)
{
}

/**
 * @note Size: 0x30
 */
void MWEnterCriticalSection(uint* section)
{
	*section = OSDisableInterrupts();
}

/**
 * @note Size: 0x24
 */
void MWExitCriticalSection(uint* section)
{
	OSRestoreInterrupts(*section);
}

/**
 * @note Size: 0x4
 */
void MWTerminateCriticalSection()
{
	// UNUSED FUNCTION
}
