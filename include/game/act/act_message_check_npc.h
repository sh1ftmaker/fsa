#ifndef GAME_ACT_ACT_MESSAGE_CHECK_NPC_H
#define GAME_ACT_ACT_MESSAGE_CHECK_NPC_H

// Code: game/act_message_check_npc.cpp.

#include "game/ActorBase.h"
#include "game/SpriteAnim.h"

// The DBMS actor ("Message-check NPC", docs/names/actors.tsv, med
// confidence): a debug NPC drawing one "SOB " sprite. Named after its
// vtable, 0x8049D0E8 (the constructor, 0x8010DCDC, stores it).
class MessageCheckNPC_8049D0E8 : public ActorBase_804A5770 {
public:
    MessageCheckNPC_8049D0E8();
    /* 0x08 */ virtual ~MessageCheckNPC_8049D0E8();
    /* 0x0C */ virtual void init();
    /* 0x14 */ virtual void update();
    /* 0x20 */ virtual void draw();
    /* 0x230 */ SpriteAnim_801D267C mAnim;
}; // Size: 0x268

#endif
