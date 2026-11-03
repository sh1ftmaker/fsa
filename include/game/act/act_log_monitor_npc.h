#ifndef GAME_ACT_ACT_LOG_MONITOR_NPC_H
#define GAME_ACT_ACT_LOG_MONITOR_NPC_H

// Code: game/act_log_monitor_npc.cpp.

#include "game/ActorBase.h"

// Named after its vtable (0x804AD278).
class LogMonitorNpc_804AD278 : public ActorBase_804A5770 {
public:
    LogMonitorNpc_804AD278() {}
    virtual ~LogMonitorNpc_804AD278();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();

    s32 findLog();

    /* 0x230 */ s32 field_0x230; // cleared by init
    /* 0x234 */ s32 mMode;       // low 4 bits of the spawn parameter (0x90):
                                 // 0, 1 on the log; 2 beside it in x, 3 in y
}; // Size: 0x238

#endif
