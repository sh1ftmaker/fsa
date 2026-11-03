# game/act_npc_ride_switch
Baseline: total: 5780 / 11557 functions, 1159560 / 4442032 bytes of code (26.10 %). After: 5782, 1159740.
Matched: dtor, draw, init (100 %). update 99.12 % (scheduling+regalloc).
Resistant: update: the target loads 0x60, 0xC, 0x68, 0x64 (log box + pos) and
adds in that order; ours loads 0x68 first. 6 tries (x0/x1/y0/y1 temporaries,
TVec2f min/max).
SHARED-EDITS: new include/game/act/act_log.h (Log_804ACD70, owner act_log,
unwritten); gen_game.tsv rows for act_npc_ride_switch removed.
Next: update scheduling; extab/extabindex follow once update matches.
