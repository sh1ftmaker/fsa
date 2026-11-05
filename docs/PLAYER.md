# Player_804A6F38: architecture and work plan

The four players (class `Player_804A6F38`, include/game/Player.h, size 0x13BC,
vtable 0x804A6F38 holding only the destructor) and their manager
(`PlayerManager_804A6F28`, `*lbl_8053AC90`, player i at +4+4*i) occupy one
contiguous text range, 0x8022AD64-0x802957CC, split over ten units. Names
are provisional (`fn_<addr>`, `cls_<vtable>`): nothing in the binary names
the states or handlers. Measured on 2026-11-05 from the asm,
report_honest.json and the boot-path coverage dumps
(fsa-wt/bootpath-data/cov). "Fwd" = a per-index forwarder
`fn(s32 player)` in tu_8022AD64.

## Units

| unit | text range | fns | 100 % | unmatched B | role (provisional) |
|---|---|---:|---:|---:|---|
| tu_8022AD64 | 8022AD64-8023FBF0 | 373 | 278 | 45,760 | PlayerManager (ctor 8023F8B0, dtor 8023F7F8, update 8023F390, draw 8023F2BC), ~270 fwds, cross-player passes |
| tu_8023FBF0 | 8023FBF0-80256C74 | 254 | 147 | ~83,500 | Player core: getters/setters, input/actions, state request, update A/B, reset, ctor/dtor |
| tu_80256C74 | 80256C74-8025EB18 | 63 | 38 | 27,132 | state handlers 0x39-0x3D, 0x41-0x48, helpers |
| tu_8025EB18 | 8025EB18-80264998 | 59 | 33 | 22,220 | state handlers 0x11-0x19; shared exit fn_8025F698(bool) |
| tu_80264998 | 80264998-802684A4 | 26 | 1 | 14,568 | draw fn_80264998, skeleton/animation fn_80265E88 + joint callbacks |
| tu_802684A4 | 802684A4-8026FC20 | 25 | 5 | 29,428 | cls_804A71A8 (helper object at Player+0xBF0) |
| tu_8026FC20 | 8026FC20-8027E5BC | 120 | 54 | 55,608 | state handlers 0x04-0x10, 0x33-0x37; movement helpers |
| tu_8027E5BC | 8027E5BC-80285E58 | 73 | 36 | 25,556 | state handlers 0x00-0x03, 0x0D-0x0E, 0x1A-0x1B, 0x38, 0x3E-0x40, 0x49-0x51 |
| tu_80285E58 | 80285E58-80289648 | 8 | 1 | 13,772 | Player update fn_80285E58 + state tick fn_802870C8 (three switch jump tables) |
| tu_8028FBA0 | 8028FBA0-802957CC | 53 | 12 | 22,292 | state handlers 0x1C-0x32 |

Calls out of the Player units (counts): EffectSystem_8049F178 / JPA emitters
347, ActorManager_804A5800 265, GameView_804A9950 236, tile queries
(tu_8022461C) 152, pad object cls_804AA8C8 (tu_803099FC) 149, ActorBase 90,
cls_804AA8B8 (tu_80301CD0) 75, cls_804AA8D8 (per-player 240x160 view box,
tu_8030AE84, Player+0xBEC) 29, BgRewrite_804A7C98 27, Unk_8045A080 (audio) 14,
Camera_804AA9E4 13. Sound otherwise goes through the two cls_804B20F8 sound
objects at +0x1250/+0x1254.

## Object layout (proven by the constructor, 0x80255B28)

| offset | member | evidence |
|---|---|---|
| 0x0 | vtable 0x804A6F38 | ctor |
| 0x4 | s32 index (0-3) | ctor argument |
| 0x38 | TVec3f[0x31] position history | __construct_array(0xC, 0x31) |
| 0x4E0, 0x510, 0x550 | TBox2f[3] each | __construct_array(0x10, 3) |
| 0x5FC | TBox2f[0x50] | __construct_array(0x10, 0x50) |
| 0xBEC | cls_804AA8D8* | new(0x2C0), deleted by the dtor |
| 0xBF0 | cls_804A71A8* | new(0xC0) cls_804A71A8(this) |
| 0xCAC | bool mActive | cleared last |
| 0x1250, 0x1254 | cls_804B20F8* sound objects | new(0x60) (index, &0x1258 / NULL, NULL, 8) |
| 0x1258 | TVec3f sound position | zeroed |
| 0x1278, 0x1290, 0x12D0, 0x12E8, 0x130C, 0x1330 | cls_804A64A8 particle callbacks (0x14) | inline ctor/dtor |
| 0x1398, 0x13A8 | cls_8049F140 'PEFF' emitter callbacks (0x10) | inline ctor/dtor |

The destructor (0x802558E4) calls fn_80241E64 (emitter teardown), deletes
0xBF0, 0xBEC, 0x1254, 0x1250, then destroys the eight callbacks;
`__dt__12cls_804B20F8Fv` (weak) follows it. Everything else is set by the
per-slot reset fn_80254744 (0x11A0 bytes) and fn_80254660; those two prove
most of the remaining fields.

Remaining padding (after fn_80254744, agent A 2026-11-05): 47 `u8` holes,
267 bytes. Largest: 0x20 at 0x4A0 / 0x5DC, 0x10 at 0x580 / 0xBC4, 0xC at
0x12A4 / 0x12FC / 0x1320 / 0x1344 / 0x1268 / 0xD10.

### Fields proven by the reset fn_80254744 / fn_802544B0 / update A

Types come from the store widths plus loads elsewhere (`psq_l` = TVec3f;
`lhz` = u16). Names stay `field_0x...`: nothing names them.

| offset | type | evidence |
|---|---|---|
| 0x20 | TVec3f | psq copy chain 0x14 -> 0x20 -> 0x2C |
| 0x2DC | TVec3f | zeroed z, y, x like 0x2D0 / 0x2E8 (`zero()`) |
| 0x32C, 0x338 | TVec3f | psq copy of mPos; 0x338 = mPos moved 16 along the facing 0x3DC in battle mode |
| 0x350, 0x35C | TVec3f | psq_l in tu_8026FC20 / tu_80285E58; the old f32 0x358 is 0x350.z |
| 0x38C, 0x390 | f32 | stfs 0.0f |
| 0x3B0, 0x3B8, 0x3E4, 0x3EC, 0x3F0, 0x3F4 | s32 | stw; 0x3DC/0x3E4/0x3E8/0x3EC/0x3F0 all take 0x3E0 |
| 0x3C6, 0x3C8, 0x3CA | u16 | 0x3C6 = 0x3C4; 0x3C8 = 0x195, 0x3CA = 0x18F (lhz in tu_80264998) |
| 0x3CC-0x3D4 | s32 x3 | stw 0 |
| 0x402-0x404, 0x43A-0x43D, 0x456, 0x457, 0x490, 0x491, 0x499 | u8 | stb; 0x437-0x43C are copies of 0x3F8-0x3FD |
| 0x408, 0x448, 0x480, 0x488, 0x49C | s32 | 0x448 = 3, 0x49C = -1 |
| 0x450, 0x452 | u16 | sth 0xC3 / 0x121; lhz at 0x8024DA2C |
| 0xAFC, 0xB68, 0xBB8-0xBBA, 0xBD4, 0xCEB, 0xCEC, 0xCEF-0xCF1, 0xD2C, 0xD5C, 0x1174, 0x11E8, 0x11E9, 0x13B8 | u8 | stb 0 |
| 0xB74, 0xB94, 0xBA8, 0xBB4, 0xBD8, 0xCF8, 0xD38, 0xD48, 0xD54, 0xD58, 0xDA0, 0xDA8, 0x11B0, 0x11C4, 0x11C8, 0x1208, 0x1210, 0x122C, 0x1234, 0x123C, 0x1264 | s32 | stw; 0xBB4 = -1, 0xCF8 = 0x29 |
| 0xBBC | f32 | 3.0f |
| 0xC1C | s32[13] | zeroed in a loop by fn_802544B0; indexed by tu_8026FC20 |
| 0xC54-0xC60, 0xC68, 0xC6C | s32 | counters zeroed one by one with 0xC50 / 0xC64 / 0xC70-0xC7C |
| 0xCA1, 0xCA2 / 0xCA3, 0xCA4 | u8 / s8 | 0xFF; -0x80 |
| 0xCBC, 0xD1C | TVec3f | psq copy chain mPos -> 0x2F4 -> 0xCBC -> 0xCB0 -> 0xCD4 -> 0xCC8 -> 0x32C -> 0xD1C |
| 0xEF0 | s32[0x50] | one loop with 0xDB0 and 0x1030 (the 0x1010 hole is gone) |
| 0x12B0, 0x12B4, 0x132C, 0x1354-0x1370, 0x1384, 0x138C-0x1394 | JPABaseEmitter* | deleted in address order by fn_80241E64; their 0xF4 is the emitter's mFlags (StopDraw / StopCalc in update A) |
| 0x1398.field_0x4/0x8, 0x13A8.field_0x4/0x8 | | the 'PEFF' callbacks get the index and 0 / 1 |

cls_804A71A8 (0xBF0): f32 0x14 and 0x18, zeroed by update A every frame.

### Reset and state request (written)

- **fn_80254744** (99.30 %): per-slot setup. `field_0xd00 >= 2` with the
  view's save-row stage flag set clears 0xD00/0xCFE. The slot's part comes
  from the game mode: no 0x7F and the player not taking part (bit
  field_0x4 of lbl_8053AAF8->field_0x82) -> inactive, start state 1 (also
  with 0x7F, fn_802DE814 and the bit clear); fn_802DEBF8 or 0x7E clear ->
  active, 0xD60 = -1 for player 0 and 0 for the others; otherwise players
  taking part lead (0xD60 = -1, pad 0x11EC = index) and the rest follow:
  2 players (fn_80138A30) -> 0xD60 = index - 2, 3 players -> 0xD60 =
  lbl_80529DEC.field_0x64 (0-2), else inactive in state 1. A follower starts at its leader's
  position (fn_8023E724) plus 16 per place in line (inlined fn_80248778).
  Then mPos is copied into the position history, fn_80276538 / fn_80276078,
  cls_804A71A8::fn_8026F710, fn_8027221C(0), fn_8027023C; with
  fn_802DEBF8 it requests state 3 (inlined fn_80251AEC(3)); last the camera
  box fn_8030BD3C and, for 0xCFE / 0xD00 == 2, fn_8024DCD0 + fn_8024B2E0.
- **fn_802544B0** (code identical, literal pool only): fn_80254660's body,
  then 0xC04 and the counters 0xC1C-0xC7C.
- **fn_80251AEC / fn_80251BFC** (100 %): as in State system below.
- **fn_80253E6C** (update A, code identical; literal pool only): the
  order is fn_80270D08 + fn_80286688 + fn_80265E88 + clear 0xDB0/0x1030
  and return while lbl_8053AAF8->field_0x94 == 1 or 0x121F (held);
  otherwise 0x2F4 = mPos + (0, 0, 18), fn_802490A8 (unless 0x80),
  fn_8026FD14, fn_80251D50, then either the join sequence (0x80 set:
  counter 0xD38, effects 0x418-0x41A at 0x10, cues per player at
  0xC0/0xE0/0xF8/0x108) or fn_80285E58; then fn_8024BDBC, fn_80276538,
  fn_80276078, cls_804AA8D8::fn_8030B774, fn_8024E0A0.
- **fn_8023F2BC** (manager draw, 100 %).
- **fn_8023F390** (manager update, 99.7 %): written in round 3. The
  forwarders fn_8022F478 / fn_8023DE58 test `!isPlayerActive` first (their
  out-of-line copies stay 100 %, the inlined ones now have the original
  block order); one `mr`/`li` of the last loop's offset differs.

## Per-frame flow

Update (GameRootSub04 fn_80139D9C -> **fn_8023F390**, PlayerManager update):
1. each player: **fn_80253E6C** (update A): pad slot lookup (tu_803099FC),
   **fn_80285E58** (Player update), **fn_802490A8** (input/actions, 0x1F50),
   fn_8026FD14, fn_80251D50 (position), fn_8024F0E8 (view box), per-player
   HUD/input blocks, fn_8024BDBC, fn_80276538, fn_80276078, fn_8024E0A0,
   cls_804AA8D8::fn_8030B774.
2. unless lbl_8053AAF8+0x94 == 1: each player **fn_80252FC0** (update B:
   player-vs-actor interactions, fn_8027FA08, fn_8025F448); clear the
   per-frame bytes 0x434/0x435; **fn_80232A0C** (cross-player pass, 0x1194);
   **fn_80231DE4** only when lbl_8053AAF8+0x7f; each player fn_80252FBC (empty).
3. active players: fn_80240670; then the audio manager byte.

**fn_80285E58** (Player update): while the freeze counter 0x11F4 > 0,
count it down and reset the 0xDB0 / 0x1030 word arrays to -1; otherwise
**fn_802870C8** (state tick). Then fn_80265E88 (skeleton) and fn_8024CA20.

Draw (GameRoot fn_80136828 -> fn_80139CC4 -> **fn_8023F2BC**): players 3..0
in two passes split by bit i of lbl_8053AAF8+0x82, each **fn_80264998(player)**.

Reset: fn_8023F78C -> **fn_80254744** (scene enter, GameRootSub04
fn_8013A1A4; stage change, fn_802DA8DC / fn_802DBD60); fn_8023F728 ->
fn_802544B0; fn_8023F6C4 -> fn_80254660; fn_8023F088 -> fn_80252F80 when
lbl_8053AAF8+0x7e == 1.

cls_804A71A8 (Player+0xBF0): Player::fn_802685D4 (from state updates 0x46,
0x19, 0x14) -> fn_80268BC4 -> fn_80269574 (0x22A8, the largest Player-area
function), fn_8026C350 (0x1E90), fn_8026E1E0.

## State system

Fields: **0x1240 current state, 0x1244 requested, 0x1248 previous**, 0x124C
request pending, 0x124D lock (set unless the request is 0, 2 or 3). 82
states, 0x00-0x51.

- **fn_80251AEC(state)** requests: **fn_80251BFC(state)** decides whether
  the switch is allowed (0x124C/0x124D, 0xCEE, 0xD60, 0xCE8, 0x1214/0x1215,
  not current, not pending, lbl_8053AAF8+0x7e then fn_80236890); then it
  stores 0x1244 and sets the flags. Direct `stw 0x1244` sites also exist:
  fn_80242624 (0x4D), fn_80242704 (0x4C), fn_8024B108 (0x49), fn_8024BBB8
  (0x3F), fn_8024F63C (0x1A), fn_802520C0 (0x1B), fn_8024762C / fn_80247C0C
  (0x0B), fn_80248DD8, fn_80248E94 (3).
- **fn_802870C8** (0x235C) runs, in order: an EXIT switch on (0x1240 - 3)
  (jump table 0x804A7770, 79 entries); the commit `0x1248 = 0x1240; 0x1240 =
  0x1244` followed by fn_8025F698(true); an ENTER switch (0x804A7628, 82
  entries); and an UPDATE switch (0x804A74E0, 82 entries). Outside
  fn_80254744 the commit is the only store to 0x1240. All three tables are
  compiler `switch` jump tables, so the source is three `switch`
  statements, each case being `handler(); break;`, not function-pointer
  tables.
- The getters that compare 0x1240 against a constant are the closest thing
  to state names: 0x49 (fn_80240818), 0x0F (fn_80240B00), 0x11
  (fn_80240B20), 0x51 (fn_80240B34/B5C), 0x50 (fn_80240B70), 0x1B
  (fn_80240D1C), 0x4F (fn_80240D50), 0x4C (fn_80240D64), 0x38 (fn_80240DAC),
  0x1E (fn_80240ED4).

Handlers (update / enter / exit; `*` = ran in the Shadow Battle dump; states 0-2 have no exit):

```
0x00 802859C4*/80285C28/-       0x01 802857F0/802859B8/-        0x02 80285690/802857C8/-
0x03 8028538C/80285668/80285388  0x04 8027D5F0/8027DEA0/8027D4F4  0x05 8027CC90/8027D320/8027CBB0
0x06 8027CB44/8027CB9C/8027CB24  0x07 8027C6C4/8027CAA8/8027C6B8  0x08 8027BE74/8027C4FC/8027BC90
0x09 8027B648/8027BA2C/8027B5F0  0x0A 8027AAD0/8027B304/8027AA10  0x0B 8027A1E0/8027A908/8027A178
0x0C 80279FE8/8027A0D4/80279F94  0x0D 80285098/8028537C/80285074  0x0E 80284E04/80285068/80284DE4
0x0F 80279824/80279CFC/802796B8  0x10 802791B0/8027966C/802791AC  0x11 8026441C/80264768/8025F698
0x12 80263C64/80264410/8025F698  0x13 80262BAC/80263078/80262B8C  0x14 80262254/80262B6C/80262234
0x15 80263860/80263BA0/80263854  0x16 802636D4/80263848/802636D0  0x17 802635D8/802636C4/802635D4
0x18 802632A8/802635C8/802632A4  0x19 80261750/80262188/8026173C  0x1A 80284978/80284DD0/8028496C
0x1B 802844F8/802848F0/8028447C  0x1C 802948C0/80294ADC/80290E90  0x1D 80293FE4/80294374/80290D58
0x1E 80293B30/80293E8C/80290E90  0x1F 80293900/80293B1C/80290D58  0x20 80293774/802938B8/80290E90
0x21 802935A4/80293760/80290D58  0x22 80293398/80293534/80290E90  0x23 80292ECC/802932BC/80290D58
0x24 802929FC/80292E90/80290E90  0x25 80292820/80292944/80290D58  0x26 802925E4/802927B0/80290E90
0x27 802924AC/802925C4/80290D58  0x28 802922EC/80292454/80290E90  0x29 80292198/802922D0/80290D58
0x2A 80291B8C/80291CB8/80290E90  0x2B 80291910/80291B64/80290D58  0x2C 802954DC/80295584/80290E90
0x2D 802953A0/802954B8/80290D58  0x2E 80295268/80295334/80290E90  0x2F 8029513C/80295240/80290D58
0x30 80291F10/80292124/80290E90  0x31 80291D28/80291E44/80290D58  0x32 80290FC0/8029185C/80290D58+80290F20
0x33 80277F98/802790C0/80277EBC  0x34 8027776C/80277E18/8027774C  0x35 8027761C/8027769C/802775C8
0x36 80277220/80277580/802771C8  0x37 80277104/802771BC/80277100  0x38 80283E70/8028414C/80283D2C
0x39 8025DACC/8025E79C/8025D9FC  0x3A 8025C894/8025D090/8025C7BC  0x3B 8025BEB0/8025C79C/8025BDD8
0x3C 8025B3F0/8025BDC8/8025B368  0x3D 8025B22C/8025B35C/8025F698  0x3E 80283D00/80283D20/8025F698
0x3F 8028434C/80284430/8025F698  0x40 80284294/80284328/80284290  0x41 8025D6AC/8025D7E4/8025D6A0
0x42 8025AED0/8025B218/8025AEC4  0x43 8025AB00/8025AEB0/8025AAF4  0x44 8025A15C/8025A9C0/8025A0B0
0x45 80259830/8025A04C/80259784  0x46 80258BE8/80259660/80258BBC  0x47 80258890/80258BA8/8025888C
0x48 802586D0/80258880/802586CC  0x49 80283948/80283CEC/80283944  0x4A 80282FE0/80283848/80282FC8
0x4B 80282ACC/80282E6C/80282AA8  0x4C 80282958/80282A24/802828A8  0x4D 80282178/8028286C/8028211C
0x4E 80281E98/80282094/80281E84  0x4F 80281A90/80281D50/80281A18  0x50 8028126C/80281760/802811C0
0x51 80280E44/802810E4/80280DD8
```

## Big functions (> 0x400 bytes)

Format: address, size, then instructions executed on the battle path / total
instructions where coverage exists. A function is 0 % unless a percentage is
given. "(0xNN)" means the function is that state's handler.

tu_8022AD64
- 8022B334 0x490 86/292: composite query, 7 Player getters + 3 tu_80256C74 calls
- 8022E438 0x49c 78/295: composite query over 6 Player getters
- 8022EA0C 0x934 58/589: multi-player scan (28 fwds + 14 Player calls)
- 80231DE4 0xc28 0/782: manager pass, only when lbl_8053AAF8+0x7f set (cutscene/demo?); GameView, state getters
- 80232A0C 0x1194 370/1125: manager cross-player pass in update (111 fwds, effects, GameView, ChiefSoldier)
- 8023434C, 8023BEA8, 8023C36C 0x4c4 0: three same-shape fwd blocks (10 Player + 9 fwd calls)
- 80235B68 0x480, 80237A80 0x738 0: fwd composites (ActorBase, tu_8026FC20 calls)

tu_8023FBF0
- 80241E64 0x460: emitter teardown (30 x EffectSystem + forceDeleteEmitter)
- 80243234 0xa30, 80243C64 0x9cc, 80244630 0x9cc: pad (cls_804AA8C8) button/stick queries, 30 fwds each
- 8024762C 0x570 80/348: state-request helper (stores 0x1244 = 0/1/2/0xB), fwds, effects
- 80248004 0x600: 35 fwds + GameView
- 802490A8 0x1f50 180/2004: input/action body of update A: 116 fwds, 31 pad calls, JUTGamePad
- 8024B2E0 0x708; 8024D134 0x858 (30 fwds, effects); 8024E694 0x638 (ActorManager/ActorBase searches)
- 8024BDBC 0x5a0 29/360: input checks (cls_804AA8C8, cls_804A03A8, cls_804AA8B8)
- 8024F0E8 0x464 64/281: camera box (cls_804AA8B8 x9, GameView x7)
- 8024F63C 0x83c 113/527, 80250090 0xa3c 78/655, 80250FD4 0x810 13/516: tile/collision queries via fwds + pad
- 802520C0 0x44c: requests state 0x1B
- 80252FC0 0xeac 100/939: update B (see Per-frame flow)
- 80253E6C 0x644 188/401: update A per player (see Per-frame flow)
- 80254744 0x11a0 646/1128: per-slot reset/(re)spawn; writes 0x1240/44/48 = 0/1/2/3, GameRoot mode checks

tu_80256C74: 8025785C 0x558 29/342 (36 Player calls, requests state via 80251AEC); 80258BE8 0xa78 (state 0x46 update); 80259830 0x81c (0x45);
 8025A15C 0x864 (0x44); 8025B3F0 0x9d8 (0x3C, 14 effect calls); 8025BEB0 0x8ec (0x3B); 8025C894 0x7fc (0x3A); 8025D0B0 0x5f0 (fwd-only helper);
 8025DACC 0xcd0 (0x39, 15 tile queries via 8022461C). All use ActorManager/ActorBase + EffectSystem (JPA).

tu_8025EB18: 8025F698 0x630 (Player::fn_8025F698(bool), shared cleanup, called in the commit step of every state change (states 0x11/0x12/0x3D-0x3F have no exit case));
 802604CC 0x73c 35/463; 80260C08 0x860 56/536 (26 ActorManager + 18 ActorBase calls: spawn/carry); 80261750 0xa38 (0x19); 80262254 0x918 (0x14);
 80262BAC 0x4cc (0x13); 80263C64 0x7ac (0x12).

tu_80264998: 80264998 0x114c 0 (draw, see Per-frame flow); 80265E88 0x1190 0 (skeleton/anim; ~23 joint callbacks 80267018..80267ECC).

tu_802684A4: 802685D4 0x568 **92.3 %** 121/346 (Player method; calls the helper's fn_80268BC4); 80268BC4 0x85c 274/535 (helper update; calls
 80269574, 8026C350, 8026E1E0); 80269574 0x22a8 474/2218 (largest: 55 Player calls, GameView x15, 10 calls into tu_8028FBA0);
 8026B81C 0xb34 (GameView only); 8026C350 0x1e90 44/1956 (66 Player calls, effects, BgRewrite); 8026E68C 0x490.

tu_8026FC20: 802710C8 0x1098 (movement, atan2, effects); 80274CB8 0xa2c (16 effect calls); 802756E4 0x6b8; 80276078 0x4c0 37/304 (Player calls only);
 802765C0 0x5f8 (no calls, math); 8027776C 0x6ac (0x34); 80277F98 0x1128 (0x33, 13 ActorManager); 802791B0 0x4bc (0x10); 80279824 0x4d8 (0x0F);
 8027A1E0 0x728 (0x0B); 8027AAD0 0x834 (0x0A, 13 tile queries); 8027BE74 0x688 (0x08); 8027CC90 0x690 (0x05); 8027D5F0 0x8b0 (0x04, 40 Player calls);
 8027DEA0 0x4f8 (enter 0x04).

tu_8027E5BC: 8027F1C8 0x68c **100 %** (Player::fn_8027F1C8, 22 emitter calls, 47 instrs run); 8027FA08 0x66c 49/411 (called by update B);
 80280074 0x46c, 802804E0 0x408 (PSVECMag distance checks); 8028126C 0x4f4 (0x50); 80282178 0x6f4 (0x4D); 80282FE0 0x868 (0x4A); 80284978 0x458 (0x1A).

tu_80285E58: 80285F44 0x744 96/465 (pre-tick gate; sets state, effects); 8028676C 0x408 20/258; 802870C8 0x235c 362/2263 (state tick, see State system).
tu_8028FBA0 (0 instrs run): 8028FC2C 0x444; 80290070 0xce8 (44 Player + 8 cls_804AA8B8); 80290FC0 0x89c (0x32, 20 effect calls); 802929FC 0x494 (0x24);
 80294374 0x54c (0x1D); 80294ADC 0x660 (0x1C enter).

Callee classes used by the Player units (call counts): EffectSystem_8049F178 / JPA emitters (tu_8013C5C8) 347; ActorManager_804A5800 265;
GameView_804A9950 (stage/map state, tu_802D39B8) 236; tu_8022461C tile/collision queries 152; cls_804AA8C8 (pad object, tu_803099FC) 149;
cls_804AA8B8 (tu_80301CD0, unknown) 75; ActorBase 90; cls_804AA8D8 (per-player camera box 240x160, tu_8030AE84) 29; BgRewrite_804A7C98 27;
Unk_8045A080 (JAudio manager, tu_80457DFC) 14; cls_804AA858 14; Camera_804AA9E4 13. Sound: only ActorSoundObj_804578F4 (fn_80456154, in
tu_8023FBF0); no JAI calls inside the Player units (audio comes through fwds / Unk_8045A080).

## Boot path

No Player object exists from boot to the main menu (the manager at
lbl_8053AC90 is not constructed), and the only Player-range code that runs
there is the units' `__sinit` functions, the static-init function
fn_8023F9B8 and three manager guards that return early on the null manager:
fn_8023F264 (the player-active check), fn_8023DE58 and fn_8022EA0C (14
instructions). Measured from the p1-p3a coverage dumps. Everything else runs in the menu ->
Shadow Battle phase: about 7.8k unmatched instructions (~31 KB). The most
executed unmatched functions are:
- tu_8023FBF0: fn_80254744 646, fn_80253E6C 188, fn_802490A8 180, fn_8024F63C 113, fn_80251D50 101, fn_80252FC0 100, fn_8025250C 81, fn_8024762C 80, fn_802544B0 80.
- tu_8022AD64: fn_80232A0C 370, fn_8023F390 121, fn_8022B334 86, fn_80239F10 86, fn_8023F088 79.
- tu_802684A4: fn_80269574 474, fn_80268BC4 274, fn_8026F710 162, fn_802685D4 121.
- tu_80285E58: fn_802870C8 362, fn_80285F44 96, fn_80286E10 76, fn_80286B74 67.
- tu_8026FC20: fn_8026FD14 101, fn_80273518 87, fn_802732C4 86, fn_802730D8 81.

Only state 0's update (fn_802859C4) ran, so 81 of the 82 states are not on
the boot path. tu_8028FBA0 (states 0x1C-0x32) executes nothing there.

## Recommended order

1. **tu_8023FBF0, the reset functions fn_80254744 (0x11A0) and fn_802544B0.**
   They run first on the battle path (646 instructions executed) and store
   most of the fields that are still unnamed, so they finish the layout the
   constructor started.
2. **The update spine.** fn_80253E6C (update A); then fn_80285E58 and
   fn_802870C8 (tu_80285E58: three switches with literal cases, after
   fn_80285F44, fn_8028676C, fn_80286B74 and fn_80286E10); then fn_8023F390
   and fn_8023F2BC (manager update and draw, tu_8022AD64). Write them
   structure-first: every callee they name becomes a declared member.
3. **The state machinery.** fn_80251AEC and fn_80251BFC, then the state 0
   handlers (fn_802859C4, fn_80285C28; tu_8027E5BC).
4. **The large bodies on the battle path.** fn_802490A8 (input/actions),
   fn_80252FC0 (update B), fn_80232A0C (cross-player pass); in
   cls_804A71A8, fn_80268BC4 and fn_80269574.
5. **Off the boot path, unit by unit.** The state handler blocks:
   tu_8026FC20, tu_80256C74, tu_8025EB18, tu_8027E5BC, tu_8028FBA0. Draw
   and skeleton (tu_80264998) can come in any order.

Small getters in every unit are cheap matches once their fields are carved;
the Haiku/Sonnet passes have taken most of them already.

## tu_80285E58: state tick details (agent B, 2026-11-05)

Proven from the asm of fn_802870C8 and its three jump tables (generated
from the tables' relocations, so every case below is the table entry):

- The exit table (0x804A7770) is indexed by `state - 3` (`subi 3; cmplwi
  0x4e`), so states 0-2 have no exit case. Five entries point at the
  switch's end, i.e. have **no exit case**: 0x11, 0x12, 0x3D, 0x3E, 0x3F
  (the table above lists fn_8025F698 for them; that is the call in the
  commit, which runs for every state change, not their exit).
- The commit calls **fn_8025F698(true)** (`li r4, 1`), not (0), after
  `0x124D = 0; 0x1248 = 0x1240; 0x1240 = 0x1244`, copying bytes
  0x3F8-0x3FD to 0x437-0x43C, clearing 0x3F8-0x3FD and 0x43D, and zeroing
  the floats 0x38C, 0x390 and the TVec3f 0x2B8.
- Exit of state 0x32 calls two functions: fn_80290D58 then fn_80290F20.
  Every other case is a single `handler(); break;`.
- The exit switch only runs when 0x124C (request pending) is set, and the
  whole state machine is skipped while 0xCEE is set or fn_802413E8()
  (byte 0x454) holds; then the tick only restores mPos from the TVec3f at
  0x32C.
- Between ENTER and UPDATE: fn_80131F0C(); a damage call
  fn_80238548(index, 0x20, &mPos, 0x12) when fn_80131D98() or
  lbl_8053ABA8 / lbl_8053ABF0 is set, else a request of state **0x33**
  when fn_8024189C() <= 0 and !fn_80270A8C(); then the push vector at
  0x35C is added to mPos and damped (x0.95 above length 2, x0.9 below) in
  states 8 / 0x37 (or when fn_80241380()), added and damped x0.6 in states
  0x12, 0x15-0x18, else damped x0.9, and zeroed below length 0.2.
- After UPDATE the tick runs, in order: fn_80286E10, state 3 request when
  0x1215 == 1, fn_80242BC0, fn_80242940, fn_80242CB0, fn_80280B30,
  fn_8025250C, ..., fn_8027F1C8, eight per-frame countdowns (0x488, 0xD58,
  0x122C, 0x3F4, 0xD84, 0x1218, 0x44C, 0x1220), fn_80250090, fn_8024F63C,
  fn_80285F44, **fn_80286688 inlined** (it is defined first; the 0x13B8
  toggle on pad trigger 0x10), fn_80286B74, fn_8028676C, and the three
  emitters at 0x138C / 0x1390 / 0x1394 (ids 0x4FF-0x502 by 0x3DC, 0xD7,
  0x1B1).
- fn_80285E58 and fn_802870C8 share the reset loop of the s32[0x50]
  arrays at 0xDB0 / 0x1030 (`for (int i = 0; i < 0x50; i++)`; an `s32`
  counter adds a bound check the original lacks).

Round 2 (all eight functions written): fn_80285E58 and fn_80285F44 100 %,
fn_802870C8 98.3 % (missing/extra, every call and branch in place; FIXME in
the source), fn_8028676C / fn_80286E10 / fn_80286B74 99 % (registers /
scheduling), fn_80286688 88.7 % (the mode-0xB byte test). The jump tables
are compiler `@NNN` symbols. Each switch groups the case labels of states
that share a handler (the tables point several entries at one label: e.g.
the exits of 0x1C/0x1E/.../0x30 are all fn_80290E90 and those of
0x1D/0x1F/.../0x31 fn_80290D58), so the source lists those cases together.
fn_80285F44 (landing): picks the landing sound by the tile under the player
(fn_8024F0E8 0x2001F/0x20054-0x20065) and splash/dust emitters 0x3CF/0x3D0,
0x4F3-0x4FA plus a 'PEFF' actor, with a 4-frame cooldown in 0x448; then
the step sound (0x2001C/0x2001E) while moving on the ground.

## cls_804A71A8 (agent D, 2026-11-05)

The Player's helper at Player+0xBF0 (0xC0 bytes, tu_802684A4) is the
player's **wall / tile probe**. Proven from fn_8026F710, fn_80268BC4 and
fn_8026E464 (include/game/cls_804A71A8.h has the layout):

- 0x8 TVec3f probe point, 0x1C s32 probe direction (0-7), 0x14 / 0x18 the
  owner's mPos x / y movement over fn_80268BC4.
- 0x20-0x8F: fourteen `u8[8]` per-direction flag arrays, indexed by the
  probe direction (stbx at field_0x1c + base). The directions are
  lbl_8046BAE8: +x, -x, +y, -y (6.999 out), then the diagonals (+-5, +-5).
  0x28 = probe inside one of the owner's 0x5FC boxes, 0x38 = blocked /
  outside the view box, 0x40 = last frame's 0x48. The owner's fn_802517F8 /
  fn_802518AC / fn_8025190C read field_0x48[0..7].
- 0x98 / 0x99 u8 (fn_80269420 sets 0x98; fn_80268BC4 moves it to 0x99),
  0x9A u8 "probe again" (fn_80268BC4 repeats its pass while it is 1, at most
  100 times), 0xA0 / 0xA4 s32 push / idle frame counters (fn_8026E1E0),
  0xAC, 0xB0 (pass counter), 0xBC s32; 0xB4-0xBA u8 (0xB4 -> 0xB5 each frame).
  0x90-0x97 and 0x9B-0x9F are still unknown.
- **fn_80268BC4** (per frame, from the owner's fn_802685D4): fn_8026E464
  (start-of-frame reset, inlined: it also sets the owner's field_0xef0[0x50]
  to -1); unless fn_80240B84 / fn_8025F448, passes of: clear the flags,
  probe the 8 directions (inside an owner box -> 0x28; else in view or
  fn_8026FC20 > 0 or fn_802DCD0C -> fn_8026C350(tile); else 0x38), with a
  second probe on another player's layer (fn_8023DE58(owner->0xD70)) when
  the owner is on layer 8 and fn_8025F590 holds (fn_8026B81C on a miss);
  then fn_80269574, and fn_8026E1E0 after the first pass. Then 0xC90 = (1,
  1, 1) unless 0x98 / 0x99.
- Player::**fn_802684A4**(pos, layer) (100 %) is the box test (0x50 boxes
  at 0x5FC with id 0xDB0 >= 0 and layer 0x1030; copies 0xDB0[i] to
  0xEF0[i]); fn_80268BC4 has it inlined twice.
- fn_8026C350's tile handlers: **fn_8026E68C** (ledge tiles 0x10-0x17, the
  jump target scan in steps of lbl_8046B9C8, then the owner's fn_8026E5F8
  inlined), **fn_8026EB1C** (which facings a ledge tile accepts),
  **fn_8026EE60** (push a block: spawns 'BLCK' 16 units ahead check,
  serial into the owner's 0xD94), **fn_8026EC88** (the same for player
  blocks, tile 0x31 + player), **fn_8026F000 / F1C4 / F388 / F54C**
  (fn_8026EE60 for one facing: 1 / 0 / 2 / 3).
- Status: 13 / 25 at 100 %; fn_80268BC4 99.13 % (4 dead stack stores),
  fn_8026E68C 99.97 % (literal names only), fn_8026EE60 / EC88 / F000 family
  86-90 % (step-vector add order; owner/facing kept across fn_801F856C).
  Unwritten: fn_80269574 (0x22A8, a state machine on 0xBC that walks the
  direction arrays, searches 'WARP'-range actors and requests states),
  fn_8026B81C (0xB34), fn_8026C350 (0x1E90, the tile switch with jump table
  0x804A7338 / 0x804A71B4).

## Round 3, agent C (Opus 5.5, 2026-11-05): input, update B, manager update

### Groups (leader / followers) and the input gate

Proven by the inlining (MWCC only inlines what is defined earlier, and these
bodies are inlined into functions placed before their out-of-line copies, so
they are file inlines in tu_8023FBF0.cpp with the out-of-line copies as
wrappers, as playerFlags_802413F0):
- **groupSize** (fn_80248970): this player, its leader 0xD60 and every
  follower (fn_8022F514) whose leader (fn_8022F3DC) is that leader (or this
  player when 0xD60 < 0).
- **groupMode** (fn_802486A0): 5 alone, else the leader's mode (fn_8022CAF8)
  or, leading, **0x120C**. Mode 4 = turn-based: **groupTurn** (fn_80240888:
  the leader's fn_80234004 or, leading, **0x1224**) against **linePlace**
  (fn_80248778).
- **follows_80240670** (fn_80240670): not taking part without a leader; in
  mode 4 not its turn; else has a leader. fn_8023FFC4 / fn_8024018C /
  fn_80240354 return the leader's value (0xBE4 / 0xBE0 / 0xBD8, valid while
  0xBE8 / 0x42C / 0xBDC) through it.
- **teamCheck** (no out-of-line copy): playerFlags_802413F0, the mode-4 turn
  test, and **0x11EA**. Every pad query of the player goes through it:
  fn_80246904 / fn_80246B10 / fn_80246D1C (cls_804AA8C8 A78 / D24 / D6C with a
  mask, u32), the fixed-mask bools fn_802454C0-fn_802466B4 (also require
  0x456 clear and 0x11F0 <= 0; fn_80246464 / fn_802466B4 also !fn_8022B244),
  fn_80244FFC / fn_80245238, and the axes fn_8024712C (fn_80309F20) /
  fn_80247330 (fn_80309FCC). fn_80243234 / fn_80243C64 / fn_80244630 compare
  those axes with the facing 0x3DC (fn_80243234 also turns 0x3DC/0x3E8 to 3).
- fn_802486A0 / fn_802487FC / fn_80248970 / fn_80240670 / fn_80240888 /
  fn_8023FC5C are **const**: the loop test's field_0x4 is hoisted, which
  MWCC only does through a const `this` (as in fn_80248778).

**The 0xCE9 test (coordinator decision, 2026-11-05):** the ~40 inlined
copies branch on byte 0xCE9 (`cmplwi; bne; li 0 / li 1`), which
fn_802413F0's own body (`return ce9 != 0`, 100 %) cannot give, so the
inlined gate is its own inline, playerFlagsGate (game/PlayerFlags.h, if
form). With it every gated function here is registers-only (99.5-99.9 %).

### fn_802490A8 (input, 99.9 %, registers only)

Skipped while 0x1220 > 0. Returns when the acting player (this one, or in
mode 4 fn_8022EA0C of it) is fn_80230188 / fn_80236B50 / airborne (z > 0) /
fn_8023EA28, or a player it can reach (fn_8022E438) is busy (fn_8023E98C
without fn_8022E438 back, fn_8022DEAC outside mode 4, ten state queries);
also in demo mode with 0xCFE == 1, fn_8022AF5C, or **0x121E == 1**. Then:
a leader (0xD60 < 0) of a group of 2+ pressing 0x40 (fn_80309D6C) either
spawns the 'FORM' actor (mode 5; serial in **0x1230**, fn_80134FE0(1)
outside demo mode) or calls fn_80248004(5, true); on a layer without
viewLayerFlag, button 0x40 held plus a direction (8/1/4/2 without the
crossing ones) calls fn_80248004(2/3/1/0, true); last the C stick
(JUTGamePad sub stick value > 0.5, angle sectors of 0x2000) picks the same
four directions, unless a reachable player has fn_8022DEAC or the pad
(fn_8030A308) is busy.

### fn_80252FC0 (update B, 99.1 %)

Skipped for 0x121F, 0xCE8, 0x454, fn_80270A8C, fn_80241200, fn_8025F448,
state 0x0E, mode 4. A follower (0xD60 >= 0, not held: **0xD9C / 0xDAC**
< 0) or a player with **0x1216** set walks to fn_8027FA08's spot at 2 a
frame while **0x1218** > 0, else 1.5 when cls_804A71A8::anyFlag48 (the
eight bytes at 0x48 are an array), else 6; on arrival 0x1216 = 0, 0x1217
= 1 (unless 0x1214 or 0x1218), 0x1215 = 1 (follower with 0x1214) and the
camera box is reset (fn_8030BD3C). Skipped while fn_80240670 says the
leader's fn_80233EDC, state 0x1B current or requested, state 0x19, 0x122C
> 0. A held player (0xD9C, else 0xDAC) moves to 16 beside the holder by
its facing 0x3DC (1.5 or 3 a frame). Otherwise: snapped back to 0x14 while
a reachable player's fn_8023EF50 actor has field_0x1a4; not in mode 5,
fn_80250D30 or a flagged layer, and only with fn_802341D0: pulled 5 a frame
toward fn_8027F854 by a player in fn_8022D930/D894/D9CC, else pushed by the
largest x / y of the reachable players' fn_8022DFD4 (cls_804A71A8 0x14 /
0x18) beyond its own.

## Round 4 (tu_8026FC20) (agent E, Opus 5.5, 2026-11-05)

54 -> 67 of 120 at 100 %, plus 9 structurally done / close. Proven:

- **Input history** (fn_8026FD14, every frame from update A; fn_80270088 from
  enter state 4): field_0x117c[0..3] are per-direction hold timers (set to 60
  while the stick axis fn_80247330 / fn_8024712C points that way alone, else
  count down); field_0x117c[4..7] is a 4-deep history shifted by fn_80270088
  (newest 60 at [4]) and counted down in a loop over `[i + 4]` (the original
  steps a base register by 8 for the last two); field_0x119c[4][4] is a 4-deep
  history per direction column (shifted down, newest 1 in row 0; a column is
  cleared while its axis is not pressed). 0x11C4 / 0x11C8 / 0x11CC / 0x11D4 are
  countdowns (0x11C8 counts frames with fn_80246F28() <= 0, up to 3).
- **Kill counter** fn_80270360(serial): field_0xc50 (total, capped 9999999) and
  field_0xc1c[13] by the dead actor's mActorId: OKTA 0, AMOS 1, POOH 2, BZBR 3,
  DDRK 4, DGTL 5, HINO 6, SLR1 / SLR2 7, TKTA 8, KETH 9, BPOH 10, MOZO 11, Poon
  12 (`switch (actor->mActorId)` compiles to the original's cmpw tree).
- **Boxes**: 0x4A0 / 0x4B0, 0x5DC / 0x5EC are TBox2f (no other Player unit
  touches 0x4A0-0x4BF; 0x5DC is also read by tu_80256C74). fn_80276538 sets
  0x4C0 = mPos +-7 and 0x4D0 = mPos +-15 every frame; fn_80276078 sets 0x5EC to
  the 8x16 box in front of the player for direction 0x3EC (0 +x, 1 -x, 2 +y,
  3 -y, -1 = a flat +-15 box), from the facing 0x3DC in states 0/2/3, and from
  a remapped facing under fn_80241338 / fn_802412E0 (a file inline with a
  switch, inlined with field_0x3dc and with constants), and 0xB98 = 1 when set.
- **Box tests**: the original's overlap test is the intersection compare chain
  itself (`f.x >= i.x && f.y >= i.y` on a copy of the box, no `? true : false`,
  so not JGeometry's TBox::intersect). Several tests make both boxes relative to
  the layer's view corner (mVisibleBox below layer 8, else GameView 0xdc) by
  adding a negated TVec2f filled member by member (a TVec2f(x, y) temporary
  loads y first). fn_802730D8 / fn_802732C4 (boot path) are the "player can be
  hit by `box`" tests (fn_802732C4 starts with fn_80270C88 inlined);
  fn_802724E0 / fn_80272E98 the view-relative ones; fn_80272DCC adds a box to
  the 0x50-slot table 0x5FC / 0xDB0 / 0x1030 (TBox2::set, float copies).
- fn_802738F0(box, out, flag, center) tests the boxes 0x4A0 / 0x4B0 while
  0x458 == 1 and writes their joint centre (sum / 4) or 0x4B0's midpoint to
  *out; fn_80273E7C / fn_80274238 call it with a view-relative copy and add the
  view corner back. fn_80273E7C returns a u8 (clrlwi of a bool local);
  fn_80274238 does too (100 % as u8; its only caller is the s32 loop
  fn_8023B8E4). fn_80256F44 (seen from fn_80273C30 / D44's neg/or/srwi) returns
  a u8 as well: with u8 in tu_80256C74 / Player.h both are 100 %, but the
  generated forwarder fn_8023AAF8 (tu_8022AD64, bool) drops to 89.7 % until it
  returns u8, so fn_80273C30 / D44 stay at 94 % with FIXMEs.
- fn_80274728 / fn_80274988 (from the 0x0A / 0x09 updates) step the spin
  animation: 0x3FA frame, 0x3FB pose, 0x11C0 s16 angle by facing; mode 3
  without fn_8023434C (tu_8022AD64, declared extern "C") changes the timing.
- **Literal pool**: fn_802710C8 (0x1098, unwritten) is the first user of
  eleven .sdata2 floats (6, 10430.378, 32, 0.5, 3, 1.5, 4, 0.75, 2, 1.4,
  0.25); every later float user stays reloc-name until it is written.
  fn_80276538, fn_80276078, fn_802738F0 and fn_80273518 have identical code
  and only that.
- Left unwritten: fn_8027057C / fn_80270DDC / fn_80270F00 (fn_80270DDC is the
  heal routine on 0xBFC, inlined into the other two; it contains
  `old = x; x++; if (x == old) for (;;); x = old;` whose source form is
  unknown, and inlines fn_80270A8C before its definition), fn_80270948 (inlines
  fn_80270A8C before it is defined: needs a header inline + wrapper as for
  playerFlags_802413F0), fn_802796B8 (emitter stop sequence, as before).

## Round 4 (state handlers)

### tu_8028FBA0: states 0x1C-0x32 are screen / layer transitions (agent F, 2026-11-05)

Every state of this unit moves the player between two places: 0xCF4 is the
transition kind (0x29 = none; the arrive exit stores it in 0xCF8), 0xCD4 /
0xCE4 the destination position / layer, 0xCC8 / 0xCE0 the departure, 0xCB0
the last position on the TV. Layers 8 and up are the GBA screens (houses,
caves): the handlers wait while the player's GBA link is busy
(fn_80301CD0, fn_80303750), notify it (fn_803037D4(player, 0xD),
fn_80301D54) and switch the music (lbl_8053AF80->fn_8045815C by 0xCE4 < 8).
The states come in pairs: the even one leaves (exit **fn_80290E90**: GBA
notice, 0x400 = 0, music), the odd one arrives (exit **fn_80290D58**:
respawn point 0xCBC / 0xD1C unless fn_80241E28, 0xCF8 = 0xCF4, 0xCF4 =
0x29, 0x468 = 60, camera box, pad mode 5, fn_80248604). 0x3FC is the
frame counter of all of them; 0x3F8 / 0x3F9 the animation counter / frame;
0x3DC / 0x3E8 the facing (0 right, 1 left, 2 down, 3 up, from the enter of
0x1D). Names are provisional, from the motion, sounds (fn_8024F0E8) and
effects (fn_8013CC50):

| states | enter / update | what it is |
|---|---|---|
| 0x1C / 0x1D | 80294ADC, 802948C0 / 80294374, 80293FE4 | **walk out through an entrance / walk in**: 0x398 = start, 0x3A4 = 20 (stairs: 1.25) along the kind's direction (0x17 right, 0x18 left, 1/2 down, else up; 0x15 / 0x23 by layer), steps toward it (fn_8027E5BC(0.5)) for up to 50 / 80 frames; the walk mode comes from the previous state (0x4A/0x4B -> 0xCEB, 0xB -> 0xCEC: walk, push, carry animations). Arrival requests 0, 0x12 (carrying), 0xB, 0x4A/0x4B or 0x33 (no life) |
| 0x1E / 0x1F | 80293E8C, 80293B30 / 80293B1C, 80293900 | **drop through a hole**: after 58 frames the landing is checked by tile (fn_8022461C 3 / 0xE, fn_802265DC, fn_802684A4; else 0x2C4 / 0xCBC) and the player appears at height 128 (or 0); 0x1F falls 4 a frame spinning, then 0 / 0x12 / 0x33 |
| 0x20 / 0x21 | 802938B8, 80293774 / 80293760, 802935A4 | **leave / arrive with sparkle 0x17D** (118 frames out; in after the GBA wait) |
| 0x22 / 0x23 | 80293534, 80293398 / 802932BC, 80292ECC | **spin up out of the screen** (riseToTarget, camera on mPos - 150) / **warp in**: per-player effects 0x13D-0x149, 0x143, 0x545 at 0xCD4, 171 frames |
| 0x24 / 0x25 | 80292E90, 802929FC / 80292944, 80292820 | **warp out** (per-player 0x14A-0x15A, 0x152, 0x546 at 0xCC8; 118 frames) / **arrive** (60 frames, then requestArrival) while a held actor (0xD68) follows at 0x2F4 |
| 0x26 / 0x27 | 802927B0, 802925E4 / 802925C4, 802924AC | **leave through a door, animated** (sound 0x20060, effect 0x547, 50-frame animation 0-11, effects 0x15B/0x15C) / **walk down out of the door** (walk cycle, y + 1 for 32 frames) -> 0x4B |
| 0x28 / 0x29 | 80292454, 802922EC / 802922D0, 80292198 | **walk up into a door** (y - 0.5, frames 4-7, 67 frames) / **arrive** (walk cycle; 0x3FB / 0x400 wait) -> 0x4A |
| 0x2A / 0x2B | 80291CB8, 80291B8C / 80291B64, 80291910 | **spin up** (60 frames, then riseToTarget) / **spin down** with gravity from 0x344 = (0, 0, 4), landing 0 / 0x3E / 0x33 |
| 0x2C / 0x2D | 80295584, 802954DC / 802954B8, 802953A0 | **fall into a hole** (60 frames) / **drop onto the floor below** from height 128 with gravity (fallStep) |
| 0x2E / 0x2F | 80295334, 80295268 / 80295240, 8029513C | **rise out** (after 10 frames, 4 a frame to 128) / **drop in** with gravity from 0x344 = (0, -0.5, 0) |
| 0x30 / 0x31 | 80292124, 80291F10 / 80291E44, 80291D28 | **walk up through a door** (y - 0.1, frames 0-5, 'PEFF' dust at 4 and 8) / **arrive on the TV** (effects 0x15B/0x15C, sounds 0x20041/0x1000F, 0x5C frames; then 0x4A) |
| 0x32 | 8029185C, 80290FC0 | **jump down a ledge** to 0xCD4: speed (0xCD4 - mPos) / 64 with z 8, gravity 0.25; kind 0xC adds the stage's splash emitters 0x242-0x249 (0x132C / 0x135C, turned by the facing table lbl_8046C228), kind 0xD a 130-frame scripted drop with per-player effects 0x16A-0x16F; extra exit fn_80290F20 kills both emitters |

The transitions are started by **fn_80290070**(layer, from, to, kind)
(0x80290070, 0xCE8 bytes; not written: it inlines fn_80290D58 and
fn_80290E90, which come after it, so it needs deferred inlining).
fn_8028FBA0 starts kind 5 back to 0xCB0; **fn_8028FC2C** (100 %) picks a
per-stage respawn point for kind 0x27 (stage / level / screen of the view,
the floor 0x3B4 - 7).

Written in round 4: 51 of 53 at 100 % (all but two). fn_80290FC0 (state 0x32
update) is 99.62 % (scheduling only; Player 0x1344 is now a TVec3f, the
emitter position copied from mPos, moved 3 by facing and handed to the
callback 0x1330's field_0x4); its .sdata2 literals set the pool order that
makes the other 26 handlers match. Not written: fn_80290070 (above).

## Round 5 (tu_80256C74) (agent H, Opus 5.5, 2026-11-05)

38 -> 44 of 63 at 100 % (plus fn_80273C30 / fn_80273D44 in tu_8026FC20 now 100 %:
fn_80256F44 and its forwarder fn_8023AAF8 return u8). Structurally done / close:
fn_8025785C 90.4 %, fn_8025B22C 95.6 %, fn_8025D7E4 96.4 %, fn_8025E79C 89.9 %,
fn_8025AED0 99.8 %, fn_80258890 99.97 % (rodata placement only), fn_8025D6AC 99.9 %
(literal names).

**fn_8025785C, the item check** (every frame; first user of the .sdata2 floats 2.0 / 0.0):
while fn_8022B334(player) and fn_8022C310(player) hold, the selected item
**fn_802416C4** (Player 0xC10, set with its level 0xC14 by fn_80241618) requests its
state. A leader / follower copies the leader's item first (fn_80240808, fn_802373EC).
Evidence for the item names is the actor each state spawns (docs/names/actors.tsv):

| item | state (update / enter / exit) | what it is |
|---|---|---|
| 1 | 0x39 (8025DACC / 8025E79C / 8025D9FC) | enter: facing from the leader or the pad (fn_80246B10 2/1/8/4), fn_8027221C(6); exit releases the emitters 0x1360-0x1368 and clears 0xCE8 while 0xBAC / fn_8026FC20 > 0 |
| 2 | 0x48 (802586D0 / 80258880 / 802586CC) | **Fire Rod**: frame 2 spawns ACTOR_FIRE ("Fire Rod fire") by the player when fn_80247594(0) allows and the last fire (0xB3C) is gone; level in bits 12+ of the argument; 8 frames |
| 3 | 0x3D (8025B22C / 8025B35C / 8025F698) | **'BOME'** (docs/names/actors.tsv: unknown, low; not named a bomb there): at most three alive (0xB00[3]) and none held (0xB18); frame 6 spawns 'BOME' (argument 0x100 at level >= 2) into 0xB18; done at frame 16 |
| 4 | 0x3A | refused while 0xB18 is held, the 0xB74 cooldown runs, or (fn_8022B160) the player's byte of lbl_8053ABB8 is set |
| 5 | 0x3C | - |
| 6 | 0x45 (level >= 2) / 0x44 (at most three 0xB1C[3]) | - |
| 7 | 0x46, or 0x14 while carrying (fn_8025F684) | refused while a living actor is held (0xD68), any player holds 'RCK2', the floor probe 2 above the hand point (fn_8024B298(2)) is tile 2 on floor layers, or mPos.z > 0 elsewhere |
| 8 | (no state) | **bombs (HBMB, "Bomb")**: two at a time (0xB30[2]), placed 12.999 in front (lbl_8046B220) unless that tile is 0xE; sound 0x20006 |
| 9 | 0x47 (80258890 / 80258BA8 / 8025888C) | **dig** 16 in front at floor level: frame 11 sets the 8x8 box 0x5CC (0xB9E), frame 12 asks BgRewrite fn_802A1844 (result in 0xBA0; 1-4 dig something up, 8 another sound); results 1-4 give an effect by result and facing (lbl_8046B200) at frame 28; a miss switches to the short animation (0x3FD = 1) |
| 10 | 0x3B | refused while 0xB18 is held |
| none | - | fn_80246228() sets 0x1200 |

- **0x41 recoil hop** (enter 8025D7E4, update 8025D6AC): speed lbl_8046AFE0[facing]
  = 2 back against the facing and 4 up, sound 0x20010, a view shake
  (GameView fn_802D79E8), the TV effect of kind 3 aimed along the facing
  (cls_804AA858 fn_802F601C); then 0x344 is added to mPos with gravity 0.25 (to -4)
  until the player lands (state 0).
- **0x42** (update 8025AED0, requested by fn_8025AA8C(serial) from tu_801688AC):
  the player spins (facing every two frames) and is drawn 6 a frame across the floor
  toward actor 0x11F8, until it is gone / on another layer, 0xAFC is 1 or a side
  flag of 0xBF0 is set (anyFlag48). **0x43** (8025AB00, from act_arrow via
  fn_8022E1A0) is the same with a speed min(distance, 6), the facing turned away
  from the actor, and an end test on the arrow's field 0x230 (unwritten: that field
  is padding in act_arrow.h).
- fn_80256C74 is a fourth box test (field_0x5DC, only while fn_80270A8C and 0xBA4);
  fn_80257DB4 is false when any of 21 state / flag tests holds, else 0xD3C < 0.
- Data: .rodata is file-scope tables addressed off one base in the big handlers
  (offsets 0x30 / 0x50 / 0xB0 from fn_8025DACC, 0x190-0x2C0 from the others);
  lbl_8046AF00 (0x30), lbl_8046AFE0, lbl_8046B200, lbl_8046B220 are written.
- Not written: fn_802570BC (needs Player 0x580 as a TBox2f, the box of actor 0xB28),
  fn_8025AB00 (arrow field 0x230), fn_8025D0B0 and the big handlers 0x39, 0x3A, 0x3B,
  0x3C, 0x44, 0x45, 0x46 updates (0x7FC-0xCD0 bytes).
