# A/B report: game/tu_8029D898.cpp (BgRewrite_804A7C98), Opus 5.5

Baseline (start): total 8776 / 11557 functions; unit 1/33 (the generated `__sinit`).
After: total 8784 / 11557 (+8 honest), structural 9236 (was 9227); unit 9/33 at 100 %.
Full `ninja` ends `fsa_game_code.dol: OK`; `gate.py --all-units` ok; `gate.py --base faa79b43` ok;
`gen_game.py --check` ok; `order_check.py`: this unit in order (the 11 listed units are other,
pre-existing ones). Per-function compare by address against fsa-decomp's report_honest.json: no drops.

## 100 % (8 new)
__ct, __dt, fn_802A3D80, fn_8029D898, fn_8029E888, fn_802A01F0, fn_802A1490, fn_802A1554.

## Written below 100 % (FIXME in source)
| function | % | try.py class | what differs |
|---|---|---|---|
| fn_802A3948 | 98.8 | different (1 instr) | missing clrlwi r6,16 on the u16 tile before fn_802F9974 |
| fn_802A3884 | 98.8 | different (1 instr) | same |
| fn_802A381C | 97.7 | different (1 instr) | same |
| fn_8029E928 | 99.1 | regalloc-only | r30/r31 swapped (gBgRewrite vs cell) |
| fn_8029DF80 | 93.4 | missing/extra (2) | original steps a copy of cell in r31 in place |
| fn_8029DEBC | 92.4 | missing/extra | layer copied into r31 inside each branch in the original |

## Proved / learned
- field 0x24 is a u16 (sth/lhz): the tile code fn_802F9908 returns for field_0x20.
- fn_802A0800 takes (player, layer), not pos (r4/r5 in its body and in fn_802A01F0's call).
- fn_8029DEBC (layer, cell, vertical, a, b), fn_8029DF80 (cell, layer, a..d), fn_8029E928 (pos, layer, tile),
  fn_802A1490 / fn_802A1554 (pos) are BgRewrite members (callers load gBgRewrite_8053AC98 into r3); symbols renamed.
- fn_8029E928 is fn_802A3948 inlined on gBgRewrite_8053AC98 with player 0; its s32 tile shows the clrlwi.
- The tile clrlwi in fn_802A381C/3884/3948: an s32 (or s16) tile parameter makes all three 100 %, but
  s32 drops tu_8012DAC0 update (100 -> 94.6, lhzx scheduled differently) and act_panel_rewrite_npc;
  s16 drops tu_8012DAC0, act_shutter, act_burning_block. Kept u16; unresolved.
- Ranges like 0x68..0x6B compiled as subi/clrlwi/cmplwi come from enumerated `==` tests, not `>= && <=`;
  a u16 call result compared with cmpwi is read into an s32 local.

## SHARED-EDITS
- include/game/BgRewrite.h: field_0x24 -> `u16 field_0x24` + `u8 field_0x26[2]` (sth 0x24 in fn_8029E888 etc.);
  new member declarations fn_8029DEBC, fn_8029DF80, fn_8029E928, fn_802A1490, fn_802A1554, fn_802A0800.
- include/game/Map.h: `extern const u16 lbl_8046CB88[0x400]` (act_fire_rod_fire.cpp's .rodata, read by fn_802A1490/1554).
- config/G4SE01/gen_game.tsv: tu_8029D898 sinit row removed (file now hand-written); symbols.txt .bss chain @395..@403 -> @904..@912.

## Left
24 functions unwritten: fn_802A3A0C needs the unit's .rodata (frame table lbl_8046C4A8, 'SCH ' names at +0x170/+0x184)
written as data; the big ones (fn_8029EDB8 0x1550 B, fn_802A20F0, fn_802A0800, fn_802A02A8, fn_802A3164,
fn_802A1844/1D44, fn_8029E508 ...) and the tile pattern writers fn_8029D8F0/DB2C/DD50/E050/E248/E9EC/EC84.
