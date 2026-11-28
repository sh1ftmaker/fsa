#include "game/GameAudio.h"
#include "dolphin/os.h"
#include "game/GbaClient.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "dolphin/gba/GBA.h"
#include "dolphin/dvd/dvd.h"
#include "game/GameState.h"
#include <string.h>

// The GBA client threads: startGbaClients_803007D0 loads "client_thread.bin" (the GBA program
// sent with GBAJoyBoot) and starts one thread per controller port, each running
// fn_80300E1C on its client's message queue.
//
// Not written yet: fn_80300E1C (the client thread body). The file holds the unit's .bss objects, in
// the original's order, and the JAudio pool __sinit (game/GameAudio.h).

GbaPacket_80300A8C lbl_804D6AB8[4];
// The client threads and their 0x1000-byte stacks (OSCreateThread in startGbaClients_803007D0:
// stack top &lbl_804D7798[i * 0x1000 + 0x1000], priority 8).
OSThread lbl_804D6B38[4];
u8 lbl_804D7798[4 * 0x1000];
GbaClient_803007D0 lbl_804DB798[4];

// The archive the client program is loaded from (lbl_80529DEC + 0x58); startGbaClients_803007D0 sets it.
JKRArchive* lbl_8053ACB8;
// The client program image ("client_thread.bin"), a GBA multiboot ROM:
// startGbaClients_803007D0 patches header bytes 0xAC-0xAF (game code), 0xBD (header
// checksum over 0xA0-0xBC) and the word at 0xC8.
struct GbaProgram_8053ACBC {
    /* 0x00 */ u8 mBytes[0xC8];
    /* 0xC8 */ OSTick field_0xc8; // lbl_8053ACC8
};
GbaProgram_8053ACBC* lbl_8053ACBC;
s32 lbl_8053ACC0;        // its size
DVDDiskID* lbl_8053ACC4; // the current disc's id
OSTick lbl_8053ACC8;     // OSGetTick at startGbaClients_803007D0

const u8 lbl_80541618[4] = { 0x1A, 0x18, 0x10, 0x0C };
static const s8 lbl_8054161C[4] = { 2, 4, 0, 6 };

s32 fn_801365A8();

// The alarm fn_80300E1C sleeps on (on its stack): the alarm, then the thread
// to wake (OSGetCurrentThread, stored right after it at +0x28).
struct GbaAlarm_80301A88 : public OSAlarm {
    /* 0x28 */ OSThread* mThread;
};

// The alarm handler: resumes the thread that set the alarm.
void fn_80301A88(OSAlarm* alarm, OSContext* context) {
    OSResumeThread(static_cast<GbaAlarm_80301A88*>(alarm)->mThread);
}

static inline void sleep_80300E1C(OSTime ticks) {
    GbaAlarm_80301A88 alarm;
    OSCreateAlarm(&alarm);
    alarm.mThread = OSGetCurrentThread();
    BOOL enabled = OSDisableInterrupts();
    OSSetAlarm(&alarm, ticks, fn_80301A88);
    OSSuspendThread(alarm.mThread);
    OSRestoreInterrupts(enabled);
}

static inline s32 readGameCode_80300E1C(GbaClient_803007D0* client) {
    u32 code;
    s32 err = GBAGetStatus(client->mPort, &client->mStatus);
    if (err != 0) {
        return err;
    }
    if (client->mStatus != 0x28) {
        return 1;
    }
    err = GBARead(client->mPort, (u8*)&code, &client->mStatus);
    if (err != 0) {
        return err;
    }
    client->field_0x64 = code;
    client->field_0xd7 = 1;
    return 0;
}

static inline s32 writeGameCode_80300E1C(GbaClient_803007D0* client) {
    s32 err = GBAGetStatus(client->mPort, &client->mStatus);
    if (err != 0) {
        return err;
    }
    if (client->mStatus != 0x20) {
        return 1;
    }
    err = GBAWrite(client->mPort, (u8*)lbl_8053ACC4, &client->mStatus);
    if (err != 0) {
        return err;
    }
    err = GBAGetStatus(client->mPort, &client->mStatus);
    if (err != 0) {
        return err;
    }
    if (client->mStatus != 0x30) {
        return 1;
    }
    return 0;
}

static inline s32 exchangeInitialPacket_80300E1C(GbaClient_803007D0* client, u32 fallbackCode) {
    if (readGameCode_80300E1C(client) == 0) {
        if (memcmp(&client->field_0x64, lbl_8053ACC4, 4) == 0 || client->field_0x64 == fallbackCode) {
            if (memcmp(&client->field_0x64, lbl_8053ACC4, 4) == 0) {
                client->field_0x5c = 0;
            }
            if (writeGameCode_80300E1C(client) == 0) {
                u32 cmd = 0x4000000;
                if (GBAWrite(client->mPort, (u8*)&cmd, &client->mStatus) != 0 || (client->mStatus & 0x30) != 0x30) {
                    return 1;
                }
                for (s32 offset = 0; offset < 0x20; offset += 4) {
                    if (GBAGetStatus(client->mPort, &client->mStatus) != 0 || client->mStatus != 0x38) {
                        return 1;
                    }
                    if (GBARead(client->mPort, (u8*)&client->mPacket + offset, &client->mStatus) != 0 || (client->mStatus & 0x30) != 0x30) {
                        return 1;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

// FIXME(shotgun): 91.98 %, missing/extra; behaviour drift.
// What still differs: the original leaves the poll loop from inside the switch without the two
// flags (nextMsg, sleepBeforeRetry): one sleep block ahead of the loop head and one failure tail
// inside case 5, both jumped to from every case.
void* fn_80300E1C(void* arg) {
    GbaClient_803007D0* client = static_cast<GbaClient_803007D0*>(arg);
    OSMessage msg;
    while (true) {
        s32 err = OSReceiveMessage(&client->mQueue, &msg, OS_MESSAGE_NOBLOCK);
        if (err == 0) {
            client->field_0xd1 = 0;
            OSReceiveMessage(&client->mQueue, &msg, OS_MESSAGE_BLOCK);
            client->field_0xd2 = 0;
        }
        if ((u32)msg == 1) {
            return NULL;
        }
        if ((u32)msg == 7 && client->mProgram == NULL) {
            msg = (OSMessage)8;
        }
        client->mMessage[0] = msg;
        s32 writeOffset = 0;
        s32 readOffset = 0;
        s32 wordIdx = 0;
        client->field_0xd1 = 1;
        client->field_0xd6 = 0;
        OSTime timeout;
        switch ((s32)msg) {
        case 3:
            timeout = OSMillisecondsToTicks(500);
            break;
        case 5:
        case 10:
            timeout = OSMillisecondsToTicks(500);
            break;
        default:
            timeout = OSMillisecondsToTicks(1000);
            break;
        }
        OSTime start = OSGetTime();
        BOOL sleepBeforeRetry = FALSE;
        BOOL nextMsg = FALSE;
        while (!nextMsg) {
            if (sleepBeforeRetry) {
                sleep_80300E1C(OSMillisecondsToTicks(15));
                sleepBeforeRetry = FALSE;
            }
            if (client->field_0xd2 != 0) {
                client->field_0xd3 = 3;
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                break;
            }
            if (OSGetTime() - start > timeout) {
                if (err != 3) {
                    err = 1;
                }
                client->field_0xd3 = err;
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                break;
            }
            u32 cmd;
            // Inside the switch: `continue` polls again, `break` is the shared failure exit below.
            switch ((u32)msg) {
            case 2:
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                client->field_0xd3 = GBAReset(client->mPort, &client->mStatus);
                nextMsg = TRUE;
                continue;
            case 3:
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                if (GBAReset(client->mPort, &client->mStatus) == 0) {
                    err = 3;
                    sleepBeforeRetry = TRUE;
                    continue;
                }
                client->field_0xd3 = 0;
                nextMsg = TRUE;
                continue;
            case 4: {
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                err = GBAGetStatus(client->mPort, &client->mStatus);
                if (err != 0) {
                    break;
                }
                if ((client->mStatus & 2) != 0) {
                    continue;
                }
                if (wordIdx == 1) {
                    client->field_0xd3 = 0;
                    nextMsg = TRUE;
                    continue;
                }
                cmd = 0x3000000;
                err = GBAWrite(client->mPort, (u8*)&cmd, &client->mStatus);
                if (err != 0) {
                    break;
                }
                wordIdx++;
                writeOffset += 4;
                continue;
            }
            case 5:
                err = GBAGetStatus(client->mPort, &client->mStatus);
                if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                    break;
                }
                if (wordIdx < 9) {
                    if ((client->mStatus & 2) != 0) {
                        continue;
                    }
                    u8* src;
                    if (wordIdx == 0) {
                        cmd = 0x2000000;
                        src = (u8*)&cmd;
                    } else {
                        src = (u8*)&lbl_804D6AB8[client->mPort] + ((wordIdx - 1) << 2);
                    }
                    err = GBAWrite(client->mPort, src, &client->mStatus);
                    if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                        break;
                    }
                    wordIdx++;
                    writeOffset += 4;
                    continue;
                }
                if ((client->mStatus & 8) == 0) {
                    continue;
                }
                err = GBARead(client->mPort, (u8*)&client->mPacket + readOffset, &client->mStatus);
                if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                    break;
                }
                readOffset += 4;
                if (readOffset != 0x20) {
                    continue;
                }
                client->mCallback(client, msg);
                nextMsg = TRUE;
                continue;
            case 6: {
                client->field_0xd8 = 0;
                client->field_0xd9 = 0;
                err = 1;
                client->field_0x5c = 1;
                client->field_0xd7 = 0;
                u32 probe = SIProbe(client->mPort);
                client->field_0x58 = probe;
                if (probe == 0x80) {
                    sleepBeforeRetry = TRUE;
                    continue;
                }
                if (probe == 0x40000) {
                    client->mState = probe;
                    client->field_0xd3 = 0;
                    nextMsg = TRUE;
                    continue;
                }
                if (probe != 8 && probe != 0x40) {
                    client->mState = probe;
                }
                client->field_0xd3 = 1;
                nextMsg = TRUE;
                continue;
            }
            case 7:
                client->field_0xd8 = 0;
                client->field_0xd9 = 1;
                client->field_0x5c = 1;
                client->field_0xd7 = 0;
                err = GBAJoyBoot(client->mPort, lbl_8054161C[client->mPort], 2, client->mProgram, client->mLength, &client->mStatus);
                if (err == 0) {
                    GbaPacket_80300A8C* packet = &lbl_804D6AB8[client->mPort];
                    memset(packet, 0, 0x20);
                    packet->field_0x0 = 1;
                    packet->mPort = client->mPort;
                    packet->field_0x3 = 1;
                    packet->field_0x4 = lbl_8053ACC8;
                    client->field_0xdc = 1;
                    client->field_0xdd = 0;
                    msg = (OSMessage)8;
                    start = OSGetTime();
                    continue;
                }
                if (err == 3) {
                    if ((client->mStatus & 0xA) == 0xA) {
                        if ((client->mStatus & 0x30) != 0x10) {
                            u32 dummy;
                            GBARead(client->mPort, (u8*)&dummy, &client->mStatus);
                        }
                    } else {
                        u32 fallbackCode = fn_801365A8() == 0 ? *(const u32*)"g4se" : *(const u32*)"g4sj";
                        if (exchangeInitialPacket_80300E1C(client, fallbackCode) == 0) {
                            client->mCallback(client, msg);
                            nextMsg = TRUE;
                            continue;
                        }
                    }
                }
                sleepBeforeRetry = TRUE;
                continue;
            case 8:
                client->field_0xd8 = 0;
                client->field_0xd9 = 1;
                client->field_0x5c = 1;
                client->field_0xd7 = 0;
                err = GBAReset(client->mPort, &client->mStatus);
                if (err == 0) {
                    u32 fallbackCode = fn_801365A8() == 0 ? *(const u32*)"g4se" : *(const u32*)"g4sj";
                    if (exchangeInitialPacket_80300E1C(client, fallbackCode) == 0) {
                        client->mCallback(client, msg);
                        nextMsg = TRUE;
                        continue;
                    }
                }
                sleepBeforeRetry = TRUE;
                continue;
            case 10:
                if (wordIdx < client->field_0xda) {
                    if (wordIdx == 0) {
                        if (client->field_0xd8 == 0) {
                            client->field_0xd3 = 1;
                            nextMsg = TRUE;
                            continue;
                        }
                        client->field_0xdb = client->mTransfer[3] & 3;
                        client->field_0xda = lbl_80541618[client->field_0xdb];
                        if (client->field_0xdb != 0) {
                            err = GBAGetStatus(client->mPort, &client->mStatus);
                            if (err != 0 || (client->mStatus & ~2) != 0x30) {
                                break;
                            }
                            if (client->mStatus != 0x30) {
                                continue;
                            }
                        }
                    }
                    if (client->field_0xdb == 0) {
                        err = GBAGetStatus(client->mPort, &client->mStatus);
                        if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                            break;
                        }
                        if (client->mStatus != 0x30) {
                            continue;
                        }
                    }
                    err = GBAWrite(client->mPort, client->mTransfer + writeOffset, &client->mStatus);
                    if (err != 0 || (client->mStatus & ~2) != 0x30) {
                        break;
                    }
                    wordIdx++;
                    writeOffset += 4;
                    continue;
                }
                err = GBAGetStatus(client->mPort, &client->mStatus);
                if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                    break;
                }
                if ((client->mStatus & 2) != 0) {
                    continue;
                }
                u32 keys;
                err = GBARead(client->mPort, (u8*)&client->mKeys, &client->mStatus);
                if (err != 0 || (client->mStatus & 0x30) != 0x30) {
                    break;
                }
                keys = client->mKeys;
                if ((keys & 0xFF) != fn_803006E8(((keys & 0xFF0000) >> 16) | (keys & 0xFF00))) {
                    break;
                }
                client->field_0xd6 = 1;
                client->field_0xd3 = 0;
                nextMsg = TRUE;
                continue;
            case 0:
            case 1:
            case 9:
            default:
                nextMsg = TRUE;
                continue;
            }
            if (err == 0) {
                err = 3;
            }
            client->field_0xd3 = err;
            client->field_0xd8 = 0;
            client->field_0xd9 = 0;
            break;
        }
    }
}

// The client's message callback: syncs the client's packet with the port's
// shared one (lbl_804D6AB8[port]), posts 5 (new), 4 or 3 (changed) to the
// client's queue, and exchanges a tick word with the GBA.
void fn_80300A8C(GbaClient_803007D0* client, OSMessage msg) {
    OSTick sent;
    BOOL changed = TRUE;
    OSTick received;
    u8 savedField2;
    u32 savedFieldC;
    OSTick tick;
    BOOL resent = FALSE;
    client->field_0xd8 = 0;
    client->field_0xd9 = 1;
    client->field_0xd3 = 3;
    tick = client->mPacket.field_0x8;
    client->mPacket.field_0x8 = lbl_804D6AB8[client->mPort].field_0x8;
    if (client->mPacket.field_0x0 == 0) {
        if (client->mPacket.field_0x3 == 0) {
            GbaPacket_80300A8C* packet = &lbl_804D6AB8[client->mPort];
            memset(packet, 0, 0x20);
            packet->mPort = client->mPort;
            packet->field_0x3 = 1;
            packet->field_0x4 = lbl_8053ACC8;
            lbl_804D6AB8[client->mPort].field_0x2 = client->mPacket.field_0x2;
            lbl_804D6AB8[client->mPort].field_0xc = client->mPacket.field_0xc;
            client->field_0x48 = OSGetTick();
            lbl_804D6AB8[client->mPort].field_0x8 = client->field_0x48;
            OSSendMessage(&client->mQueue, (OSMessage)5, OS_MESSAGE_BLOCK);
        } else {
            if (memcmp(&client->mPacket, &lbl_804D6AB8[client->mPort], 0x20) == 0 && client->field_0xdd == 0) {
                if (tick == lbl_804D6AB8[client->mPort].field_0x8 || tick == client->field_0x48) {
                    lbl_804D6AB8[client->mPort].field_0x8 = tick;
                    changed = FALSE;
                    client->field_0x48 = tick;
                }
            }
            if (changed) {
                resent = TRUE;
                OSSendMessage(&client->mQueue, (OSMessage)4, OS_MESSAGE_BLOCK);
                client->field_0xdd = 0;
            }
        }
    } else {
        savedField2 = client->mPacket.field_0x2;
        client->mPacket.field_0x2 = lbl_804D6AB8[client->mPort].field_0x2;
        savedFieldC = client->mPacket.field_0xc;
        client->mPacket.field_0xc = lbl_804D6AB8[client->mPort].field_0xc;
        if (memcmp(&client->mPacket, &lbl_804D6AB8[client->mPort], 0x20) == 0 && client->field_0xdd == 0) {
            if (tick == lbl_804D6AB8[client->mPort].field_0x8 || tick == client->field_0x48) {
                lbl_804D6AB8[client->mPort].field_0x8 = tick;
                changed = FALSE;
                client->field_0x48 = tick;
                lbl_804D6AB8[client->mPort].field_0x2 = savedField2;
                lbl_804D6AB8[client->mPort].field_0xc = savedFieldC;
            }
        }
        if (changed) {
            OSSendMessage(&client->mQueue, (OSMessage)3, OS_MESSAGE_BLOCK);
            client->field_0xdd = 0;
        }
    }
    if (!changed || resent) {
        if (GBAGetStatus(client->mPort, &client->mStatus) != 0) {
            return;
        }
        if (client->mStatus != 0x30) {
            return;
        }
        if (resent) {
            sent = tick;
        } else {
            sent = OSGetTick();
            lbl_804D6AB8[client->mPort].field_0x8 = sent;
        }
        if (GBAWrite(client->mPort, (u8*)&sent, &client->mStatus) != 0) {
            return;
        }
        if ((client->mStatus & 0x30) != 0x30) {
            return;
        }
        if (GBAGetStatus(client->mPort, &client->mStatus) != 0) {
            return;
        }
        if (client->mStatus != 0x38) {
            return;
        }
        s32 result = GBARead(client->mPort, (u8*)&received, &client->mStatus);
        if (resent) {
            return;
        }
        if (result != 0) {
            return;
        }
        if (sent != received) {
            return;
        }
        if ((client->mStatus & 0x30) != 0x30) {
            return;
        }
        client->field_0x48 = sent;
        client->field_0xd8 = 1;
        client->field_0xd9 = 0;
        client->field_0xd3 = 0;
        client->field_0xde = 1;
    }
}

// Loads the client program, patches its header (the tick at 0xC8, the disc's
// game code at 0xAC and the header checksum at 0xBD) and starts one client
// thread per port.
// FIXME(shotgun): 96.14 %, regalloc-only; behaviour clean.
// (the port loop's register use)
void startGbaClients_803007D0() {
    lbl_8053ACC4 = DVDGetCurrentDiskID();
    GBAInit();
    lbl_8053ACB8 = lbl_80529DEC.mClientThreadArchive_0x58;
    lbl_8053ACBC = (GbaProgram_8053ACBC*)lbl_8053ACB8->getResource('????', "client_thread.bin");
    lbl_8053ACC0 = lbl_8053ACB8->getResSize(lbl_8053ACB8->getResource('????', "client_thread.bin"));
    lbl_8053ACC8 = OSGetTick();
    lbl_8053ACBC->field_0xc8 = lbl_8053ACC8;
    DVDDiskID* diskID = lbl_8053ACC4;
    u8* bytes = lbl_8053ACBC->mBytes;
    bytes[0xAC] = diskID->gameName[0];
    bytes[0xAD] = diskID->gameName[1];
    bytes[0xAE] = diskID->gameName[2];
    bytes[0xAF] = diskID->gameName[3];
    u8 sum = 0xE7;
    s32 i;
    for (i = 0xA0; i < 0xBD; i++) {
        sum -= bytes[i];
    }
    bytes[i] = sum;
    for (s32 port = 0; port < 4; port++) {
        GbaClient_803007D0* client = &lbl_804DB798[port];
        u32 length = lbl_8053ACC0;
        u8* program = lbl_8053ACBC->mBytes;
        memset(client, 0, sizeof(GbaClient_803007D0));
        client->mPort = port;
        client->mCallback = fn_80300A8C;
        client->mProgram = program;
        client->mLength = length;
        client->field_0xda = 0x1A;
        client->field_0xdc = 1;
        client->field_0xdd = 0;
        client->field_0xde = 0;
        OSInitMessageQueue(&client->mQueue, client->mMessage, 1);
        OSCreateThread(&lbl_804D6B38[port], fn_80300E1C, client, &lbl_804D7798[port * 0x1000 + 0x1000], 0x1000, 8, 0);
        OSResumeThread(&lbl_804D6B38[port]);
    }
}

void fn_803007A0() {
    lbl_8053ACB8->unmount();
}

// One bit into the check value.
static inline void crcBit_803006E8(u32& crc, u32 bit) {
    crc <<= 1;
    if (bit) {
        if (crc & 0x100) {
            crc ^= 0xCC;
        } else {
            crc += 1;
        }
    } else if (crc & 0x100) {
        crc ^= 0xCD;
    }
}

// The 8-bit check value of a 16-bit word (a CRC over two bytes, then eight clock-out steps).
u32 fn_803006E8(u32 data) {
    u32 crc = 0;
    for (u32 mask = 0x80; mask != 0; mask >>= 1) {
        crcBit_803006E8(crc, data & mask);
    }
    data >>= 8;
    for (u32 mask = 0x80; mask != 0; mask >>= 1) {
        crcBit_803006E8(crc, data & mask);
    }
    u32 i = 0;
    do {
        crcBit_803006E8(crc, 0);
        i++;
    } while (i < 8);
    return crc & 0xFF;
}
