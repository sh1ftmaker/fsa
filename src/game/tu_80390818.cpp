#include "game/GameAudio.h"
#include "game/Message.h"

void MessageWindow_804AF490::fn_80390E68() {
    bool flag = false;
    u16 flags = field_0x28;
    if ((flags >> 1) & 1) {
        if (flags & 0x40) {
            field_0x48->fn_80394690();
        } else {
            if (flags & 0x2000) {
                flag = true;
            }
            field_0x48->fn_8039499C(flag);
        }
    }
}

s32 MessageWindow_804AF490::fn_80390850() {
    // A signed word, not the byte: the original compares it with cmpwi.
    s32 state = field_0x24;
    s32 value = -1;
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        value = field_0x4c->field_0x174;
        break;
    case 6:
    case 7:
    case 8:
        value = field_0x48->field_0x174;
        break;
    }
    return value;
}

void MessageWindow_804AF490::fn_80390B64() {
    if (field_0x24 == 5) {
        MessageProcessor_803944C8* proc = field_0x4c;
        proc->field_0x19e = 0xFF;
        proc->field_0x19f = 1;
    } else if (field_0x24 == 8) {
        MessageProcessor_803944C8* proc = field_0x48;
        proc->field_0x19e = 0xFF;
        proc->field_0x19f = 1;
    }
}

u8 MessageWindow_804AF490::fn_80390BA8() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x19e;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x19e;
    }
    return 0xFF;
}

u8 MessageWindow_804AF490::fn_80390894(s32 index) {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x3b2[index];
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x3b2[index];
    }
    return 0;
}

u32 MessageWindow_804AF490::fn_80390BDC() {
    if (field_0x24 == 5) {
        MessageProcessor_803944C8* proc = field_0x4c;
        if (proc->field_0x150 != 0) {
            proc->field_0x150--;
        }
        return proc->field_0x150;
    }
    if (field_0x24 == 8) {
        MessageProcessor_803944C8* proc = field_0x48;
        if (proc->field_0x150 != 0) {
            proc->field_0x150--;
        }
        return proc->field_0x150;
    }
    return 0;
}

s32 MessageWindow_804AF490::fn_80390C38() {
    if (field_0x24 == 5) {
        return field_0x4c->field_0x148;
    }
    if (field_0x24 == 8) {
        return field_0x48->field_0x148;
    }
    return -1;
}

MessageWindow_804AF490::MessageWindow_804AF490(s32 index) {
    mIndex = index;
    field_0x48 = new MessageProcessor_803944C8(this, mIndex, "gc_four_swords_text_e.bmg");
    field_0x4c = new MessageProcessor_803944C8(this, mIndex, "gc_four_swords_text_ek.bmg");
    field_0x44 = 0;
    field_0x1c = 1;
}
