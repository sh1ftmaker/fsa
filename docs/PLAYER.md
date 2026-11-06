# Player_804A6F38: architecture and work plan

The four players (class `Player_804A6F38`, include/game/Player.h, size 0x13BC,
vtable 0x804A6F38 holding only the destructor) and their manager
(`PlayerManager_804A6F28`, `*gPlayerManager_8053AC90`, player i at +4+4*i) occupy one
contiguous text range, 0x8022AD64-0x802957CC, split over ten units. Names
are provisional (`fn_<addr>`, `cls_<vtable>`): nothing in the binary names
the states or handlers. Measured on 2026-11-05 from the asm,
report_honest.json and the boot-path coverage dumps
(fsa-wt/bootpath-data/cov). "Fwd" = a per-index forwarder
`fn(s32 player)` in tu_8022AD64.

## Units

Matched counts and unmatched bytes below: build/G4SE01/report_honest.json on
2026-11-05 evening, after the deferred-inlining switch (`-inline auto,deferred`,
docs/INLINING.md); the Player units total 876 / 1,054 functions at 100 %.
The prose in the rounds below was written along the way and keeps its own
(older) numbers.

| unit | text range | fns | 100 % | unmatched B | role (provisional) |
|---|---|---:|---:|---:|---|
| tu_8022AD64 | 8022AD64-8023FBF0 | 373 | 352 | 24,820 | PlayerManager (ctor 8023F8B0, dtor 8023F7F8, update 8023F390, draw 8023F2BC), ~270 fwds, cross-player passes |
| tu_8023FBF0 | 8023FBF0-80256C74 | 254 | 172 | 77,036 | Player core: getters/setters, input/actions, state request, update A/B, reset, ctor/dtor |
| tu_80256C74 | 80256C74-8025EB18 | 63 | 52 | 20,080 | state handlers 0x39-0x3D, 0x41-0x48, helpers |
| tu_8025EB18 | 8025EB18-80264998 | 59 | 45 | 15,552 | state handlers 0x11-0x19 (all written); shared exit fn_8025F698(bool) |
| tu_80264998 | 80264998-802684A4 | 26 | 24 | 5,140 | draw fn_80264998, body-frame select fn_80265E88 + per-state frame functions (round 6) |
| tu_802684A4 | 802684A4-8026FC20 | 25 | 20 | 13,284 | cls_804A71A8 (helper object at Player+0xBF0) |
| tu_8026FC20 | 8026FC20-8027E5BC | 120 | 97 | 34,272 | state handlers 0x04-0x10, 0x33-0x37; movement helpers |
| tu_8027E5BC | 8027E5BC-80285E58 | 73 | 67 | 7,024 | state handlers 0x00-0x03, 0x0D-0x0E, 0x1A-0x1B, 0x38, 0x3E-0x40, 0x49-0x51 |
| tu_80285E58 | 80285E58-80289648 | 8 | 3 | 11,676 | Player update fn_80285E58 + state tick fn_802870C8 (three switch jump tables) |
| tu_8028FBA0 | 8028FBA0-802957CC | 53 | 51 | 5,508 | state handlers 0x1C-0x32 |

Calls out of the Player units (counts): EffectSystem_8049F178 / JPA emitters
347, ActorManager_804A5800 265, GameView_804A9950 236, tile queries
(tu_8022461C) 152, pad object cls_804AA8C8 (tu_803099FC) 149, ActorBase 90,
cls_804AA8B8 (tu_80301CD0) 75, cls_804AA8D8 (per-player 240x160 view box,
tu_8030AE84, Player+0xBEC) 29, BgRewrite_804A7C98 27, AudioMgr_8045A080 (audio) 14,
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

cls_804A71A8 (0xBF0): TVec2f 0x14 (fn_802422C4 returns its address as
fn_8022DFD4's `const TVec2f*`), zeroed by update A every frame.

### Reset and state request (written)

- **fn_80254744** (99.30 %): per-slot setup. `field_0xd00 >= 2` with the
  view's save-row stage flag set clears 0xD00/0xCFE. The slot's part comes
  from the game mode: no 0x7F and the player not taking part (bit
  field_0x4 of gGameRoot_8053AAF8->field_0x82) -> inactive, start state 1 (also
  with 0x7F, fn_802DE814 and the bit clear); fn_802DEBF8 or 0x7E clear ->
  active, 0xD60 = -1 for player 0 and 0 for the others; otherwise players
  taking part lead (0xD60 = -1, pad 0x11EC = index) and the rest follow:
  2 players (fn_80138A30) -> 0xD60 = index - 2, 3 players -> 0xD60 =
  lbl_80529DEC.field_0x64 (0-2), else inactive in state 1. A follower starts at its leader's
  position (getPlayerPos_8023E724) plus 16 per place in line (inlined fn_80248778).
  Then mPos is copied into the position history, fn_80276538 / fn_80276078,
  cls_804A71A8::fn_8026F710, fn_8027221C(0), fn_8027023C; with
  fn_802DEBF8 it requests state 3 (inlined fn_80251AEC(3)); last the camera
  box fn_8030BD3C and, for 0xCFE / 0xD00 == 2, fn_8024DCD0 + fn_8024B2E0.
- **fn_802544B0** (code identical, literal pool only): fn_80254660's body,
  then 0xC04 and the counters 0xC1C-0xC7C.
- **fn_80251AEC / fn_80251BFC** (100 %): as in State system below.
- **fn_80253E6C** (update A, code identical; literal pool only): the
  order is fn_80270D08 + fn_80286688 + fn_80265E88 + clear 0xDB0/0x1030
  and return while gGameRoot_8053AAF8->field_0x94 == 1 or 0x121F (held);
  otherwise 0x2F4 = mPos + (0, 0, 18), fn_802490A8 (unless 0x80),
  fn_8026FD14, fn_80251D50, then either the join sequence (0x80 set:
  counter 0xD38, effects 0x418-0x41A at 0x10, cues per player at
  0xC0/0xE0/0xF8/0x108) or fn_80285E58; then fn_8024BDBC, fn_80276538,
  fn_80276078, cls_804AA8D8::fn_8030B774, fn_8024E0A0.
- **fn_8023F2BC** (manager draw, 100 %).
- **fn_8023F390** (manager update, 99.7 %): written in round 3. The
  forwarders fn_8022F478 / getPlayerLayer_8023DE58 test `!isPlayerActive` first (their
  out-of-line copies stay 100 %, the inlined ones now have the original
  block order); one `mr`/`li` of the last loop's offset differs.

## Per-frame flow

Update (GameRootSub04 fn_80139D9C -> **fn_8023F390**, PlayerManager update):
1. each player: **fn_80253E6C** (update A): pad slot lookup (tu_803099FC),
   **fn_80285E58** (Player update), **fn_802490A8** (input/actions, 0x1F50),
   fn_8026FD14, fn_80251D50 (position), fn_8024F0E8 (view box), per-player
   HUD/input blocks, fn_8024BDBC, fn_80276538, fn_80276078, fn_8024E0A0,
   cls_804AA8D8::fn_8030B774.
2. unless gGameRoot_8053AAF8+0x94 == 1: each player **fn_80252FC0** (update B:
   player-vs-actor interactions, fn_8027FA08, fn_8025F448); clear the
   per-frame bytes 0x434/0x435; **fn_80232A0C** (cross-player pass, 0x1194);
   **fn_80231DE4** only when gGameRoot_8053AAF8+0x7f; each player fn_80252FBC (empty).
3. active players: fn_80240670; then the audio manager byte.

**fn_80285E58** (Player update): while the freeze counter 0x11F4 > 0,
count it down and reset the 0xDB0 / 0x1030 word arrays to -1; otherwise
**fn_802870C8** (state tick). Then fn_80265E88 (skeleton) and fn_8024CA20.

Draw (GameRoot fn_80136828 -> fn_80139CC4 -> **fn_8023F2BC**): players 3..0
in two passes split by bit i of gGameRoot_8053AAF8+0x82, each **fn_80264998(player)**.

Reset: fn_8023F78C -> **fn_80254744** (scene enter, GameRootSub04
fn_8013A1A4; stage change, fn_802DA8DC / fn_802DBD60); fn_8023F728 ->
fn_802544B0; fn_8023F6C4 -> fn_80254660; fn_8023F088 -> fn_80252F80 when
gGameRoot_8053AAF8+0x7e == 1.

cls_804A71A8 (Player+0xBF0): Player::fn_802685D4 (from state updates 0x46,
0x19, 0x14) -> fn_80268BC4 -> fn_80269574 (0x22A8, the largest Player-area
function), fn_8026C350 (0x1E90), fn_8026E1E0.

## State system

Fields: **0x1240 current state, 0x1244 requested, 0x1248 previous**, 0x124C
request pending, 0x124D lock (set unless the request is 0, 2 or 3). 82
states, 0x00-0x51.

- **fn_80251AEC(state)** requests: **fn_80251BFC(state)** decides whether
  the switch is allowed (0x124C/0x124D, 0xCEE, 0xD60, 0xCE8, 0x1214/0x1215,
  not current, not pending, gGameRoot_8053AAF8+0x7e then fn_80236890); then it
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
- 80231DE4 0xc28 0/782: manager pass, only when gGameRoot_8053AAF8+0x7f set (cutscene/demo?); GameView, state getters
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
AudioMgr_8045A080 (JAudio manager, tu_80457DFC) 14; TileMap_804AA858 14; Camera_804AA9E4 13. Sound: only ActorSoundObj_804578F4 (fn_80456154, in
tu_8023FBF0); no JAI calls inside the Player units (audio comes through fwds / AudioMgr_8045A080).

## Boot path

No Player object exists from boot to the main menu (the manager at
gPlayerManager_8053AC90 is not constructed), and the only Player-range code that runs
there is the units' `__sinit` functions, the static-init function
fn_8023F9B8 and three manager guards that return early on the null manager:
fn_8023F264 (the player-active check), getPlayerLayer_8023DE58 and fn_8022EA0C (14
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
  fn_80285F44, **fn_80286688 inlined** (the 0x13B8
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
  second probe on another player's layer (getPlayerLayer_8023DE58(owner->0xD70)) when
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
  86-90 % (step-vector add order; owner/facing kept across getActorManager_801F856C).
  Round 8 (agent M): **fn_80269574** written structure-first (96.3 %,
  missing/extra, 40 instructions longer: the original branches straight out of its early-exit tests
  where the source materialises bools). Order: field_0x50 directions become
  blocked; a follower closed in on all sides asks fn_8022BC38; the counter
  0xBC (frames closed in, < 6 sides; 3 at once on a flagged layer with an
  open 0x68 side) leads, unless group mode 4 is off its turn (screen change
  9), to the 'WARP' search on the owner's layer: type 4 near (< 7) resets,
  type 7 sends to cell ((bits 16-21) * 8 + 8, (bits 8-13) * 8 + 8) of layer
  bits 24-26 (+ 7) (kind 7), type 8 to the same place on that layer (kind
  8), 9 when that layer is >= 8 under gGameRoot 0x7E without fn_80247534;
  no warp: 0x457 = 1 at 2, kind 9 at 3. Then the 16-unit cell snap (0x3C1 /
  0x3C2), 0xB7 (slow floor: tiles 0x3EB / 0x34 above on a flagged layer,
  0x50 under), state 0x4B / 0x4A (closed by 0x78), kind 0xD / 0xC (closed
  by 0x88, fn_802DEB58), 0xB4 / 0xB6 and a 4-unit drop on a flagged layer,
  kind 0xD to 0xCBC on 0xBA, a 0.125 lift with sound 0x20045 (0x60), then
  the pushes (kind 0x22 when shut in: 7 sides with fn_80241230, all without,
  or 6 for 60 frames, counted in 0xA8) by lbl_8046BA08 (0.25 on 0xB7 or
  next to a blocked side) marking 0x48, and the respawn point 0xD1C.
  Its literals put fn_8026E68C at 100 %. No code in the Player units reads
  0x90-0x97, 0x9B-0x9F or 0xBB (left unnamed).
  Round 12 wrote fn_8026B81C (0xB34) and fn_8026C350 (0x1E90, the tile switch with
  jump table 0x804A7338 / 0x804A71B4).

## Round 3, agent C (Opus 5.5, 2026-11-05): input, update B, manager update

### Groups (leader / followers) and the input gate

(Historical, 2026-11-05: the "file inline + out-of-line wrapper" shape below was a
workaround for the old `-inline auto`, which only inlines what is defined earlier.
Since the switch to `-inline auto,deferred` with descending source order
(docs/INLINING.md, "Status: adopted") a plain function is inlined into callers
placed before it, so these can become plain functions where the asm has both an
out-of-line copy and inlined uses; converting them is a follow-up and only
tu_8023FBF0 is affected. The findings below are what the inlining proved.)
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
  generated forwarder fn_8023AAF8 (tu_8022AD64, bool) dropped to 89.7 % until it
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
  unknown, and inlines fn_80270A8C, defined after it), fn_80270948 (inlines
  fn_80270A8C, also defined after it; with the deferred model a plain function
  may do, otherwise the header inline + wrapper of playerFlags_802413F0), fn_802796B8 (emitter stop sequence, as before).

## Round 4 (state handlers)

### tu_8028FBA0: states 0x1C-0x32 are screen / layer transitions (agent F, 2026-11-05)

Every state of this unit moves the player between two places: 0xCF4 is the
transition kind (0x29 = none; the arrive exit stores it in 0xCF8), 0xCD4 /
0xCE4 the destination position / layer, 0xCC8 / 0xCE0 the departure, 0xCB0
the last position on the TV. Layers 8 and up are the GBA screens (houses,
caves): the handlers wait while the player's GBA link is busy
(fn_80301CD0, fn_80303750), notify it (fn_803037D4(player, 0xD),
fn_80301D54) and switch the music (gAudioMgr_8053AF80->fn_8045815C by 0xCE4 < 8).
The states come in pairs: the even one leaves (exit **fn_80290E90**: GBA
notice, 0x400 = 0, music), the odd one arrives (exit **fn_80290D58**:
respawn point 0xCBC / 0xD1C unless fn_80241E28, 0xCF8 = 0xCF4, 0xCF4 =
0x29, 0x468 = 60, camera box, pad mode 5, fn_80248604). 0x3FC is the
frame counter of all of them; 0x3F8 / 0x3F9 the animation counter / frame;
0x3DC / 0x3E8 the facing (0 right, 1 left, 2 down, 3 up, from the enter of
0x1D). Names are provisional, from the motion, sounds (fn_8024F0E8) and
effects (spawnEmitter_8013CC50):

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
fn_80290E90, which come after it, so it needs the deferred model, now the
project default).
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
  (TileMap_804AA858 fn_802F601C); then 0x344 is added to mPos with gravity 0.25 (to -4)
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

## Round 5 (tu_8026FC20) (agent G, Opus 5.5, 2026-11-05)

67 -> 74 of 120 at 100 % (+7 honest), structural +6. Proven:

- **fn_802710C8 = Player::fn_802710C8(attacker, damage, const TVec2f* pos, kind)**,
  the hit / damage entry (one caller: fn_80238548 in tu_8022AD64, per player).
  98.54 % structure-first. Damage 0 under fn_80240B5C, halved under fn_80241498;
  refused while isDown(), for kinds 0x11-0x13 / 0xD-0xF / others by fn_80240670 /
  fn_80270C88, kind 0x1B in stage (0, 2) of the save. 0x11E4 = last kind; kind 0x12
  sets 0x499 and kills (0xBFC = 0); the attacker's own hit goes to fn_80236468
  (group damage, tu_8022AD64). Life <= 0 requests 0x33. 0x488 = 4 on damage;
  fn_80134DE4(2 / 1, 0) on damage >= 4 / >= 1 (rumble?). Kind 0x16 (with 0x484 /
  0x480 = 60 and effects 0x28D / 0x28E 6 above mPos, sound 0x2003D) and 0x18
  request 0x50 / 0x51 and turn 0x3DC / 0x3E8 toward the hit (4-way by the s16
  atan2 angle; 2-way on layers with viewLayerFlag). Otherwise a knockback
  `(mPos - pos)` normalised ((0, 1) when tiny) and scaled per kind (1.5 / 3 / 6 /
  0.75 / 1 / 0.25 / 0.5; z 4 or 3 by damage > 4, or 2), stored in 0x350 when the
  state request (0xF default, 0x35, 0x37, 0x33, 0x40, 0x10, 0xC) succeeds; kinds
  0xD-0xF push 0x35C away instead (length 1.4 / 2 / 4) and request 0x37 / 2.
  Kinds 1-9, 0x13-0x15, 0x1A-0x1C spawn effects 0x24A / 0x24B and, on GBA layers
  (>= 8), a 'PEFF' actor. 0xC9C = 60 / 300 / 600 (kinds 0x14 / 0x15 / 0x1B) with
  fn_8022B1F8 / fn_8022B244; 0xC84.z = 0.5 for 0x1B. It fixes the .sdata2 order:
  fn_802738F0 flipped to 100 %, fn_80273518 to regalloc-only.
- **SaveRow_8050EC80 0x7E / 0x7F** (tu_80379F84.h): the hit path reads 0x7E while
  GameRoot 0x7F is set, else 0x7F; value 1 calls fn_8024E500 on damage >= 4.
- **fn_80270A8C is an inline (isDown)**: fn_80270948 and fn_8027057C inline it
  before its definition. (Historical: this was found under `-inline auto`, where
  the .sdata2 literals pooled differently under deferred; the project has since
  switched to deferred with descending source order, docs/INLINING.md, so check
  whether a plain fn_80270A8C now gives the same bytes.) Then isDown() is defined
  at the top of the unit and fn_80270A8C returns it (precedent playerFlags_802413F0). fn_80270948 (100 %) stores 0xD10 (now a
  TVec3f: the revive position, pos or mPos) once while down.
- **fn_80270DDC is a real function (heal, 100 %)**, not an inline wrapper: its
  out-of-line shape (`beq; b` after the isDown test) comes only from calling
  fn_80270A8C() (an auto-inlined function); an inline heal + wrapper gives 98.49 %.
  fn_80270F00 (max life 0xBF8 += n, cap 32, then full heal) calls it and is 100 %.
  The `old = x; x++; if (x == old) for (;;); x = old;` construct on 0xBFC is
  written literally (the reload of 0xBFC after the store is MWCC not forwarding a
  member store); it is a write probe / overflow trap of the life word, source
  form unknown beyond that. **fn_8027057C (revive, from the 0x33 update) stays
  unwritten**: it inlines fn_80270DDC, which is defined after it and must be a
  function, so no legal source gives both (98.76 % with an inline heal; its
  `field_0x4c == 7` test also materialises a u8: subfic / cntlzw / extrwi.).
- fn_80272224 (front-box hit, TBox::intersect on a copy of 0x5EC, effects 0x117 /
  0x118 at its centre, sound 0x20003, 0x3F4 = 5), fn_80272768 / fn_802728AC (point
  12 / 16 in front by facing, tables lbl_8046BD68 / BD48 as `static const f32
  [4][2]`, serial into 0xD94 / 0xDA4): 100 %. fn_80272C20: 97.24 %, extra `i >= 8`.
- Next .sdata2 users before fn_80276078 / fn_80276538 can flip: fn_8027439C (0.2,
  20, 12), fn_802756E4 (-16, 16; 0x6B8, effects), fn_80275D9C (-4).

## Round 6 (tu_8025EB18) (agent I, Opus 5.5, 2026-11-05)

33 -> 40 of 59 at 100 % (+7 honest); structurally done fn_8025EB18 99.71 %, fn_8025FDD0
99.10 % (registers only); structurally close fn_8025F698 95.94 %, fn_8025EE48 96.32 %,
fn_8025EFFC 95.94 %, fn_8025F2CC 96.81 %, fn_8025FCC8 87.39 % (FIXMEs in the source).
The unit is the player's **hold / lift / throw** code. State names are provisional
(from the actors spawned, docs/names/actors.tsv, and the motion):

- **What is held**: 0xD64 = 1 lifting (overhead), 0 = grabbing; 0xD70 the held player
  index, 0xD68 the held actor serial, 0xD6C its mActorId ('NULL' when none), 0x2F4 the
  held thing's position (mPos + a per-facing / per-pose table offset every frame).
  File inlines (out-of-line copies in brackets): holdsPlayer [fn_8025F590],
  isGrabbing_8025F448 [fn_8025F448], isLifting_8025F684 [fn_8025F684], canLift_8025F2CC
  [fn_8025F2CC], letsGo_8025FCC8 [fn_8025FCC8]; earlier functions inline them.
- **0x19 grab** (enter fn_80262188, sound 0x10007): fn_8025EE48(serial) grabs an actor;
  fn_802600A0(player) grabs a player, or pairs up with it instead by spawning 'TSNO'
  (two-sword motion) when neither is in a group, mode 4, nor on a GBA layer.
- **0x11 / 0x12 lift / carry** (fn_8025EFFC(serial, lift, grabOnly): state 0x11 when
  lift, else 0x12; sound 0x20022; enter of 0x12 fn_80263BA0 picks the carry pose by
  the held actor's field_0x118 ("atcn"), 4 for a player). fn_8026152C(fast) is the
  carry walk cycle (u8 pose table lbl_8046B790, offsets lbl_8046B7B0).
- **0x16 throw wind-up / 0x17 throw** (fn_802636D4 / fn_802635D8, 100 %): 10 frames of
  poses 0-1, then 0x17 and, for a held player, 'THCL' ("attack hitbox when player is
  thrown") at its position; 0x17 runs poses 2-4 for 14 frames, then state 0.
- **fn_8025F698(checkState), the release** (in every state commit): only when
  letsGo_8025FCC8 says the new state drops the hold. A held player is thrown with
  speed 3.3 along the facing, or toward the tile centre (/ 14 frames) when two of the
  three 16-steps ahead are wall tiles (3, 0xE, 0x3A), and none in states 0x1E / 0x32 or
  without life (fn_80230D80 -> fn_8025FDD0 on the thrown player); an actor gets
  2.5 (CRTL crystal ball, BMTY Bombchu), 4 (GRAS grass), 6 (dead), else 4 along the
  facing (fn_801F3708); sound 0x20014.
- fn_8025EB18 is the put-down check (tile / box / vfunc_0x4C at 0x2F4, a held player on
  a GBA layer checked on its own layer with the view-corner offset); fn_80261480 (from
  the states 0-3 updates) tries fn_80260C08, fn_802604CC, then fn_8026022C (lift / push
  starts; requests 0x11, 0x1A, 0x4F).
- Data: every .rodata table is written (file scope, address order; the per-pose
  tables are flat `Vec[4 * n]`, indexed `[pose + facing * n]` as the asm does).
  fn_8025F698 is the .sdata2 pool's second user (16, 1/14, 3.3, 2.5, 4, 6 and the
  int-to-float double); the next unwritten users are fn_802604CC (0.5), fn_8026152C
  (32), fn_80261750 (2, 10, 1, 0.25d, -16).
- Not written: fn_8026022C, fn_802604CC, fn_80260C08 (jump table 0x804A6FC0),
  fn_8026152C (TVec3 epsilon test of mPos against 0x14), the state handlers 0x11-0x15,
  0x18, 0x19 (fn_80261750, fn_80262254, fn_80262BAC, fn_80263078, fn_802632A8,
  fn_80263860, fn_80263C64, fn_8026441C). Player 0x580 as a TBox2f (for fn_802570BC)
  was not looked at.

## Round 6 (draw, tu_80264998) (agent J, Opus 5.5, 2026-11-05)

1 -> 24 of 26 at 100 %; fn_80264998 (draw) 99.74 % (stack slots / registers
only), fn_80267ECC 96.69 % (one shared tail). There is no skeleton: the unit
is the 2D body sprite. Names stay provisional.

**fn_80265E88 = the body-frame select** (fn_80285E58 each frame, after the
state tick; 100 %). field_0x3c6 = the old field_0x3c4, then field_0x3c4 =
the "player_sob.bin" cel to draw: 0xC2 / 0xC3 while 0xCFE (by z > 0);
field_0x450 with alpha 0xCA2 -> 0xCA0 for one frame after 0x456, or while
fn_802413E8; 0x121 (= nothing drawn, see below) while 0x464 bit 1 and no
0x488; else a `switch (field_0x1240)` (82 cases, jump table 0x804A7060) that
picks a base by facing 0x3DC (right, left, down, up) and adds a counter:
0x3F9 (walk cycle), 0x3FB (action), 0x3FD (sub-step). Per-state helpers
fn_80267018-fn_80267ECC (all Player members now, r3 = this). Examples:
standing 0x3C-0x3F (state 0 / 0x1F / 0x2A / 0x42; fn_80268194), walk 0x00 +
8 * facing + 0x3F9 (states 2, 3, 0x38), spin 0x8F-0x96 (fn_80267D30, states
9 / 0xA), the carried pose 0x1BF-0x1C3 of player 4 of a group outside the
leader's slot (fn_802486A0() == 4 && fn_80240888() != fn_80248778()), state
0x49 by field_0x1178 (0 / 1 / 2 -> 0x35F / 0x3D0 / 0x296 + 0x3FD).
**fn_80268194 is inlined** into the frame functions after fn_80265E88
(fn_80267384 / fn_80267598 are just its body, fn_80267ECC and fn_80267470 use it)
and has an out-of-line copy at 0x80268194 (just before __sinit) for fn_80265E88's
call. It is written as an `inline` member definition after fn_80265E88 (100 %), a
form chosen under the old `-inline auto`; under the deferred model a plain
function placed so that the unit's descending order puts the callers first would
do the same (historical note, not re-tested).

**fn_80264998 = the draw** (manager fn_8023F2BC). Pipeline, in order:
1. Gate: mActive, 0xCEE != 1, and with 0x1215 the player's bit of
   gGameRoot_8053AAF8->0x82. alpha = fn_8023FC5C(), index = fn_80240868(); the frame
   is clamped to 0x3D4.
2. Round shadow: field_0xb90 > 0 -> drawShadow(layer, &0x338, black, 0xb90)
   (ShadowCircle_804A0160, game/DrawShadow.h).
3. GBA marker: layer > 8 and fn_80246B10(0x40) -> "enemy_gc_sob.bin" cel
   0x73 + index * 3 + (frame counter 0x5C / 6) % 3 at 0xCB0 (a 3-frame blink
   per player), params 0x1C = 0x2D = 1.
4. Sprite shadow ("player_sob.bin", the body cel itself darkened), unless
   fn_80240808 && fn_80240DAC, and only with fn_802413F0: at (x, y - 0.01, -5)
   (y from 0x328 on fn_80240DAC), plus on layer 8 the view's top-left
   (GameView mVisibleBox.i, the per-layer view offset) and (0, -z) (height
   drawn as a y offset); alpha min(alpha, 0xCA1), cap 0x9F, minus z / 2 in
   the air (floor 79); colour 0 / blend 0x28; scale matrix (1, 1, 1) on layer
   8, else (0.9, 0.5, 0.5) (PSMTXScale, a squashed shadow). Skipped while
   0x43F. In the air on the GBA layers (>= 8, view flag clear) a shadow cel
   0x3CD goes through fn_803075AC (serial -40000 - index).
5. Body: SpriteParams with [7] = index + 12 (the player's palette), [10] = 6
   at fn_80241544() >= 2, 0x2D = 1, 0x30 = index + 4 with 0x11E9, alpha;
   mMtx = diag(0xC84 * 0xC90) (squash / stretch); priorities 0x1C / 0x1D from
   the held actor 0xD70 (fn_8023CA88 / fn_8023CB24 + 1) while fn_8025F418,
   else +1 / +1 at z >= 20, +1 in the air or with fn_80240818; overrides
   0xCA3 / 0xCA4 (-0x80 = none) under fn_802413E8 (copied back to 0x3D8 /
   0x3D9). **Colour flash**: 0xCA8 counts down with the player's colour
   (0 green, 1 red, 2 cyan, 3 magenta); else 0x400 darkens by 32 a step;
   else (no 0x7F) 0x1208 < 64 fades in from black (x4). Drawn only if
   fn_802413F0, alpha >= 0x20, not a follower whose leader is hidden, frame
   != 0x121, at mPos + 0x2B8 rounded to whole units, TV layers only.
6. Floor overlays ("enemy_both2_sob.bin", params 0x13 = 0x80 while 0x458 and
   facing down) at mPos + (0, 1) when on the floor: fn_8022517C -> cel 0x3C8
   (fn_80265AE4's 0x195-0x197 cycle); tile 0x39 / 0xD / 0x3B with fn_80270A8C
   -> cel 0x3CA (0x18F-0x191; +3 on frames 0xC2-0xC4); else, unless
   fn_802412A4 without fn_80241200 or fn_8024BBB0, tile 0x39 or fn_80225BE4 ->
   cel 0x3CA. GBA layers: fn_803075AC (serial -10000 / -20000 - index).
Helpers in the unit: drawSob / drawSobOnTv (the TV check is tested again
after the frame load, so it is inside a helper), drawGba (the position is
the call's argument, no named copy), setScaleMtx (the three scales are
computed z first, i.e. as inline-call arguments).

- fn_80265AE4 (idle counters 0x3CC / 0x3D0 / 0x3D4 -> cels 0x3C8 / 0x3CA;
  effect 0x4D2 at the feet on tile 0x39, emitter 0x1384, released with
  becomeInvalidEmitter), fn_80265D68 / fn_80265D8C (0xCA0 fade -8 / +8),
  fn_80265DB0 (shadow alpha 0xCA1: 0 on frames 0x122 / 0xB7-0xBC or layer > 8,
  0xFF on layer 8, fades while fn_8025F448 and the held actor is not on 8): 100 %.
- Data: .rodata lbl_8046B960 was one 0x38 symbol; split into the three
  strings (lbl_8046B960 / B974 / B984); .sdata2, .data (jump table) 100 %.
- Field added: 0x1178 s32 (state 0x49's variant; stw at 0x8024B1A0).

## Round 7 (tu_8027E5BC) (agent K, Opus 5.5, 2026-11-05)

36 -> 45 of 73 at 100 % (+9 honest); structurally written (FIXMEs in the source): fn_8027FA08
99.94 %, fn_802808E8 99.97 %, fn_80281E98 99.65 %, fn_80282094 99.41 % (literal names only),
fn_8028538C 99.90 % (two stack slots), fn_80283D2C 99.38 % (registers), fn_802859C4 99.54 % and
fn_80285690 98.27 % (one cmpwi each, see "Forwarder types" below). States (update / enter / exit,
names provisional, evidence = what the handlers do):

| state | handlers | what it is |
|---|---|---|
| 0x00 | 802859C4 / 80285C28 / - | **standing**: walks (2) when the stick moves (fn_80246F28 != 0), runs (3) with pad 0x40; a follower (fn_80240808) runs when its leader holds 0x40 (fn_8022C5EC), moves (fn_8022C6AC) or is ahead on the same layer (fn_8022CFE8 > 0); facing toward the nearest player as player 4 outside the leader's slot (fn_8027E7E0), else fn_8027EB04; then pick-ups (fn_80261480) and the body frame |
| 0x01 | 802857F0 / 802859B8 / - | **inactive / waiting slot** (fn_80254744 starts non-participants in 1): on tiles 3 / 0xE / 0x3A effect 0x503 and back to the return point 0x2C4 on layer 0; a leaderless player whose box 0x4C0 touches another present player that can pair (fn_802300EC) follows it (0xD60 = i), and with state 0 granted stores the pad slot 0x11EC and spawns **'TSNO'** (two-sword motion, actors.tsv) |
| 0x02 | 80285690 / 802857C8 / - | **walk**: fn_802808E8(1.5) (the stick walk step), fn_8027EB04, the walk cycle fn_8027EEB4; a follower switches to 3 like state 0 |
| 0x03 | 8028538C / 80285668 / 80285388 | **run / follow**: a follower with 0x1215 takes the leader's layer and position; formation 5 with cls_804A71A8 0xB4 walks by stick at 1.5; otherwise (0x122C <= 0) steps 0.5 a frame toward fn_8027FA08's target in x/y and on arrival stands (0) or walks (2) |
| 0x4E | 80281E98 / 80282094 / 80281E84 | **bounce**: enter sets (-1, 3, 0), a view shake (GameView fn_802D79E8, 4) and the TV effect of kind 1 at 0xCB0; after 20 frames (0x3FC) it flies with 0x344 (y -0.1 a frame to -1, x +-0.1 a frame to +-5) and rebounds at (+-1, 3) on a wall (fn_802517F8) or past 5, counting rebounds in 0x3FA; from the second one on, landing (y <= 0), 0x3FD or fn_802518AC stops it; squash 0xC84 = (1, 1, 0.5) in flight; exit restores 0xC84 = 1 |
| 0x4F | 80281A90 / 80281D50 / 80281A18 | **dash**: enter faces by the leader (fn_80230CFC) or the pad and sets speed 2 along the facing (lbl_8046BE78), sound 0x1000C; update moves by 0x344 plus the stick (fn_8022C82C / fn_8022C76C; y not on tile 2 of view-flag layers), poses 0x3FB 0-5 over 20 frames (0x3FA), then state 0; after frame 5 with fn_802466B4 and not fn_802DEBF8 it requests state 9 (0x11AC = 4); dust effect 0x3D3 (0x1354, 'PEFF' callback 0x1398) or on tile 0x39 0x3D4 (0x1358, 0x13A8); exit releases both and keeps the speed in 0x368 |
| 0x50 | 8028126C / 80281760 / 802811C0 | **burning** (hit kind 0x16, round 5): enter hops (2 along the facing, 3 up: lbl_8046BEA8), fire effects by facing into 0x135C (0x246-0x249) and 0x132C (0x242-0x245, particle callback 0x1330), spawns **ACTOR_FIRE** ("Fire Rod fire", actors.tsv high) on the player into **0xB28** with argument 0x180 \| facing << 4 \| index, sounds 0x10014 / 0x2004C; exit releases 0x132C / 0x135C and kills the 0xB28 actor (fn_802BAA00) |
| 0x51 | 80280E44 / 802810E4 / 80280DD8 | (hit kind 0x18): enter effects 0x289 / 0x28A (0x1350), 0x46C = 300, sound 0x2003B; exit releases 0x1350, takes the offset 0x474 back out of mPos, 0x464 = 60 |
| 0x38 | 80283E70 / 8028414C / 80283D2C | exit (written): 0x440 keeps mPos as the return point 0x2C4; unless 0xCF4 is 0x24 it clears 0xCE8 and, without the carry flag 0x43F, releases the players following this one (fn_802384BC on each i with fn_8022F3DC(i) == index) and on tile 0xD spawns effect 0x3D0 (scale 1.2); with 0x43F it keeps 0x444 = 0x344.z for state 0x1B or clears 0x43F / 0x440 / 0x444 |

- **fn_8027EB04 = facing / walk direction** (0x3DC / 0x3E8, 100 %): in a group from the formation
  (formation 3 with fn_80248970() >= 2: lbl_8046C0B8 / lbl_8046C0C8 by index, by viewLayerFlag) or
  the leader (fn_80230CFC / fn_80230B70); else from the pad bits fn_80246B10 (2 right, 1 left,
  8 up, 4 down; diagonals give 0x3E8 4-7 and keep a compatible facing).
- **fn_8027FA08(out) = the follow target** (update B, the state 3 update): the history head 0x38[0]
  (0x1216), mPos, or the leader's position (fn_80240644) plus this player's place by
  fn_802486A0: 0 a row (16 * fn_80248778 along x), 1 a column (along y, sign by viewLayerFlag),
  2 a 2x2 square, 3 a diamond around the leader (rotated by 0x11C0 around fn_80252F48 =
  &0x11B4 with fn_80241284, else by both indices), 4 mPos, 5 the leader's history 12 frames per
  place back (fn_8023E4E4).
- **fn_802808E8(speed) = the stick walk step**: speed per axis (0 while 0x35C is not zero or
  0x3C1 / 0x3C2 block it; y halved on tiles 0x2C-0x2F; x1.4 with fn_8022B244 and item 5) into
  0x38C / 0x390, then mPos += speed * stick (fn_8022C82C x, -fn_8022C76C y); upward on view-flag
  layers only while one of the eight probes (fn_8024B298) is off tile 2.
- **Player 0x580 is a TBox2f** (carved this round, for fn_802570BC in tu_80256C74): fn_80259830
  (0x45 update) and fn_8025A15C (0x44 update) store a per-facing box built on the stack (i at 0x580,
  f at 0x588); fn_802570BC compares points with 0x580 / 0x588 (x) and 0x584 / 0x58C (y).
- **.sdata2 blocker** (all later reloc-name misses): the original pool after the 0x4330 double is
  1.5, 0.5, 3.0, then fn_8027FA08's 4.0. fn_8027FA08 itself uses only 16, the double, 4.0, 0.5,
  in case order 3, 2, 0, 1, 5, 4 (its code order). 1.5 / 0.5 / 3.0 in that order is exactly the
  literal sequence of the state 3 update's walk (fn_802808E8(1.5f), then |dir| >= 0.5 and the
  normalize's 3.0), so a function or inline with that body was probably defined between
  fn_8027F854 and fn_8027FA08 and dead-stripped; its body is not proven, so nothing was invented.
  Until it is found every function using a literal from 0x80540378 on stays reloc-name.
- **Forwarder types**: fn_8022C5EC and fn_8022C52C return u32 (all 22 callers of fn_8022C5EC test
  with cmplwi) but are declared and defined s32 in the generator-owned tu_8022AD64.cpp; changing
  that flips fn_802859C4 and fn_80285690's compare.
- Data: .rodata lbl_8046BE78 / lbl_8046BEA8 (Vec[4] speeds), lbl_8046C0B8 / lbl_8046C0C8 (s32[4]
  formation facings) written as file-scope tables in address order (MWCC emits file-scope
  tables in declaration order). Left: lbl_8046BDA8, lbl_8046BDD8, lbl_8046BED8, lbl_8046BF38,
  lbl_8046C098 (users unwritten).
- Not written: helpers fn_80280074, fn_802804E0, fn_80280B30 (callers not traced), fn_80280E44 (0x51
  update), fn_8028126C (0x50 update), fn_80282178 (0x4D), fn_80282ACC (0x4B), fn_80282FE0 (0x4A),
  fn_80283948 (0x49), fn_80283E70 (0x38), fn_802844F8 (0x1B), fn_80284978 (0x1A), fn_80284E04
  (0x0E), fn_80285098 (0x0D).


## Round 7 (tu_8023FBF0) (agent L, Opus 5.5, 2026-11-05)

- **Group mode** fn_80248004(mode, cue): only for groups of 2+, not while
  walking to the spots (0x1216) or in the demo slot (0xCFE). A flagged view
  layer forces mode 5; with 2-3 players taking part directions 2/3 become
  1/0. Leaving mode 4 on this player's turn moves it onto the acting
  player (layer 0x3B4 / 0xCE0 / 0xCE4, mPos, 0x2C, 0xCBC, 0xD1C, trail from
  fn_8023E4E4 / fn_8023E45C). The old mode goes to 0x1210, 0x1224 = 0; a
  leader plays the cues (0x10010 for mode 4, 0x20037 + 0x10017-0x1001A per
  direction, 0x10011 when leaving 4) and resets the trail.
- **Trail** (0x38 TVec3f[0x31] + 0x284 u8[0x31]): fn_8024B9E8 pushes mPos
  and cls_804A71A8 0xB4 on the front; resetTrail (fn_80248604) refills it
  with mPos (fn_8024B2E0 while 0x454); fn_802408C8 fills it with a spot
  on the ground and starts mode 5.
- **Countdown 0x48C** (fn_8024762C, per frame): state 0xB while it runs,
  shared by the leader (fn_80247BD4 / fn_80247B9C), back to state 0 with
  effect 0x503; music 0x1000019 calls fn_8022AD64 once nobody's runs.
- fn_80240B84 (= fn_80233EDC's query) is `fn_80240670() ? leader's
  fn_80233EDC : inState1B`, const like fn_80240670; fn_80252FC0 inlines it.
- cls_804AA8D8::fn_8030B084 / fn_8030B110 take a TVec3f **by value**
  (the callers' 12-byte stack copies).
- Fields wanted (agent K): 0x1268 TVec3f (fn_8024C378 writes x/y/z
  componentwise); cls_804A71A8 0x14/0x18 as a TVec2f (fn_802422C4 returns
  its address).

## Round 9 (tu_80256C74) (agent P, Opus 5.5, 2026-11-05)

44 -> 50 of 63 at 100 % (+5 honest against matching: fn_802570BC, fn_80259660,
fn_80259778, fn_8025AB00, fn_8025D6AC; fn_80258890 was already counted). .rodata is
100 %: every table the handlers read off one base is written as file-scope data at
its original size (splits of lbl_8046AF30 and lbl_8046B070 at the offsets the code
reads; lbl_8046B1D0 is read by no function of the unit). The tables come in reverse
function order (0x8025E79C's first, 0x8025785C's last), so they were probably
function-local statics; written file-scope in address order per RULES_CARD.
Structural (FIXME in source): fn_80258BE8 99.69, fn_80259830 99.29 (scheduling+regalloc),
fn_8025D0B0 99.89 (literal names only).

- **0x46, the jump** (item 7; update fn_80258BE8, enter fn_80259660, exit fn_80258BBC):
  the enter sets field_0x344 = (0, 0, 5.4) and sound fn_8024F0E8(0x10002); the update
  steers 0x344.x / y with the pad (fn_8022C82C / fn_8022C76C: 0.8 from rest, 0.3 a
  frame rising, 0.2 falling, clamped by the side flags field_0xbf0->field_0x48[0-3])
  to at most 1.6, applies gravity 0.25 down to -4, moves y instead of z on a view-flag
  layer (viewLayerFlag), keeps the landing speed in field_0x368. A second jump in the
  air at item level >= 2 (fn_802415CC) on fn_8022C310 (field_0xb79). 0x3FB is the
  animation frame (8 frames of 4). Name: descriptive (z speed, gravity, landing), no
  string evidence for the item.
- **gatherOthers** (inline member, no out-of-line copy; inlined 4x): in formation mode 5
  (fn_802486A0) and not following (fn_80240808), every other present player is moved
  to mPos (fn_8023E690), then fn_8024BAEC. Called on landing / jump start.
- **0x45** (item 6 at level >= 2; update fn_80259830): frame 11 places a Cane of
  Somaria block ('SMRI', actors.tsv med) 16 in front into field_0xb2c (message 0xE8
  once per player, field_0xbb8) or calls fn_8030EA00 on the existing one; while the
  button is held (field_0xb84) it stays at frame 12, walks at 1.125 (fn_802808E8) with a
  28-frame walk cycle (lbl_8046B190), keeps a 'FIRE' actor (field_0xb28, argument
  0x1040 | facing << 4 | player) centred on the box field_0x580 in front
  (FireRodFire_804A89F0::fn_802BAA0C), and emitters 0x1AB / 0x54A (0x12C4 / 0x12C8).
  0x44 (fn_8025A15C, unwritten) spawns 'FIRE' the same way without the block.
- **0x43** (fn_8025AB00): act_arrow's field_0x230 is an s32 (lwz / stw, tested 0 and 4
  in act_arrow); the pull goes on while it is 0. Speed min(distance, 6).
- **fn_8025D0B0** is a Player member (r3 = the player), the step of state 0x39's update:
  3 a frame along the facing while field_0xbac >= 1, else steered by the pad with
  diagonal 2.1 steps and a turn after 8 frames.
- Fields: none needed beyond the carved ones (0x580 TBox2f used as is).
- Left: fn_8025A15C (0x44), fn_8025B3F0 (0x3C), fn_8025BEB0 (0x3B), fn_8025C894 (0x3A),
  fn_8025DACC (0x39) updates; their tables are already in place. .sdata2 order is right
  up to 5.4f (lbl_8054010C); the next literals (18, 6, 36, 12, 20, 1.125, 8, -16, -8, 5)
  come from these handlers.

## Round 9 (tu_8026FC20) (agent O, Opus 5.5, 2026-11-05)

76 -> 97 of 120 at 100 % (+21 honest: 8628 -> 8649 on matching 3a0cfee0), structural +4
more (fn_80273FB8, fn_80275D9C, fn_8027B648, fn_8027C6C4). No drops in any unit.

- **Literal pool unblocked**: fn_8027439C (0.2, 20, 12), fn_802756E4 (-16, 16) and
  fn_80275D9C (-4) written in order, so fn_80276078 / fn_80276538 are 100 % (reloc-name gone).
- **State handlers** (enter / update / exit from the three switches of fn_802870C8):
  - 0x05: enter fn_8027D320 (push 0x344 = (0, 0, -4), at least 0.5 high on flagged layers,
    sounds 0x10001 / 0x20001 / 0x2000E, immortal emitters 0xCB / 0xCC with callbacks
    0x1278 / 0x1290 and a 'PEFF' actor (serial 0x12C0) at 0x12A4 = mPos + lbl_8046BB48[facing]);
    exit fn_8027CBB0 (mPos.z = 0, releases them: clearStatus(Immortal) + becomeInvalidEmitter,
    the actor's mAlive = false).
  - 0x07: update fn_8027C6C4 (99.88 %, the knock-up: gravity 0.25 on 0x344.z down to -8,
    poses by 0x3FA, landing from frame 15 into state 8 (fn_80246464) or 0, handing 0x344 x / y
    on to 0x368).
  - 0x08: enter fn_8027C4FC (0xCAD / 0x403 counter, emitter 0x26F below 48 frames of 0x403,
    else 0x270-0x273 at 0x12A4 by life 2 and facing pair), exit fn_8027BC90 (also calls
    fn_802356B8 for the group with 0xCAE; clears 0xCAD / 0x403 unless leaving for 0x37 / 0x38).
  - 0x09: enter fn_8027BA2C (0xBF4 by the reason 0x11AC: 1 -> 8, 2 -> 6, 3 -> 4, 4 -> 8),
    update fn_8027B648 (96.22 %, the spin: walks to fn_8027FA08's spot at 6 per frame in group
    mode 3, spawns 'SWBM' (field_0xb38, layer to 0xB6C) at frame 10 when the life is full,
    emitter 0x12BC circling mPos at 20 by the angle 0x11C0).
  - 0x0A: enter fn_8027B304 (emitters 0x26A and 0x266-0x269 by player index). 0x09 and 0x0A
    share setSpinCenter: 0x11B4 = mPos moved 16 toward the player's own side (4 on flagged
    layers).
  - 0x0B: enter fn_8027A908 (inlines fn_8027023C, the input history clear; with 0x490 sound
    0x40075 + effect 0x503, else 0x3FD = 1), pose step fn_8027439C (0x3FD 0 / 2: fixed pose
    sequences ending in 0x3FD = 1, with 0x490 the cue 0x190 via lbl_8053AED0; otherwise a 19-frame
    loop, doubled while 0xBC0 > 0 or pad 0x200, slowing 0xBBC by 0.2 to 2).
  - 0x0C: enter fn_8027A0D4 (emitter 0x565 with particle callback field_0x130c, 'PEFF', sound
    0x2002A), update fn_80279FE8 (back to 0 after 120 / 90 frames by GameRoot 0x7F; the callback
    follows field_0x1320 = mPos + 16 up).
  - 0x0E: pose step fn_802756E4 (from fn_8027D5F0) by the sub-mode 0x45C; sub-modes 0 / 1 turn the
    pose into an s16 angle and put 0x30C 16 away in that 8-way direction.
  - 0x0F: enter fn_80279CFC (hit kinds 7-9 / 0x1C start emitters 0x135C / 0x132C by facing).
  - 0x36: update fn_80277220 (moves by 0x350, step emitters 0x3D4 on tile 0x39 else 0x3D3).
- **fn_80275D9C** (98.61 %, regalloc): the attack-input check of the move updates. With
  fn_802466B4 pressed: all four hold timers 0x117C[0..3] running -> state 9 (0x11AC = 2);
  3+ consecutive entries in the press history (0x117C[4 + i] running and a column of
  0x119C[i] set) -> 9 (3); 0x11D4 running -> 9 (4); else cls_804A71A8 0xB4 -> 7 (pushed
  1.5 along the facing from lbl_8046BCA8, z -4) under fn_80243234, else 5; otherwise 4.
- **fn_80276E5C / fn_80276BB8**: the facing boxes (0x4B0 16x16 attack box 8-24 ahead, 0x4A0
  body box, 0x30C point 16 ahead, 0x3F0 = facing), for the facing and the opposite facing.
- **becomeInvalidEmitter is the emitter stop**: game code includes libs/sdk's JPAEmitter.h
  (mFlags 0xF4, mMaxFrame 0x24), so the "+0xF4 |= 1, +0x24 = 1" sequence of earlier notes is
  that inline; fn_802771C8, fn_802775C8, fn_8027A178, fn_802796B8 are 100 %.
- **fn_80273FB8** (98.59 %, structure): returns `clrlwi` of a bool local, but fn_80274238
  takes its result as a bool and the original's test chain is an inline; as a static inline
  returning u8 it is 100 % but then -inline auto inlines all of fn_80273FB8 into fn_80274238
  (the original calls it). Written with the chain assigning `hit` directly.
- **lbl_8046BCA8 is one 0xA0 object** (only its start is referenced, by fn_80275D9C): the push
  table f32[4][2], then 8 rows of 4 bytes and 8 float triples, typed by content only.
- Fields: 0x12A4 TVec3f (emitter position of 0x12B0 / 0x12B4 and 0x1274 / 0x128C, also stfs /
  addi in tu_80256C74), 0x1320 TVec3f (callback point). Members: fn_8027439C, fn_802756E4,
  fn_80276BB8, fn_80276E5C (renamed in symbols.txt).
- Left: fn_8027057C (inlines fn_80270DDC, see round 5), fn_80274CB8 (0xA2C), fn_802765C0,
  fn_8027776C, fn_80277F98, fn_802791B0, fn_80279824, fn_8027A1E0, fn_8027AAD0, fn_8027BE74,
  fn_8027CC90, fn_8027D5F0, fn_8027DEA0 (all 0x4BC+).

## Round 10 (tu_8025EB18) (agent R, Opus 5.5, 2026-11-05)

40 -> 43 of 59 at 100 % (+3 honest: fn_802632A8, fn_80263860, fn_8026441C); structurally
done fn_8026022C 95.46, fn_80260C08 96.63, fn_8026152C 99.17, fn_80262254 99.64; close
fn_80261750 99.65, fn_80262BAC 98.88, fn_802604CC 88.23 (FIXMEs in the source). .sdata2
40 -> 96 %: every literal user up to 1.25 / 1.5 (fn_80263C64) is written, in pool order.
State names stay provisional (evidence: the state requested, the tables, the actors):

- **0x11 lift** (update fn_8026441C, 100 %): lift positions lbl_8046B250[pose + 4 * facing]
  over 24 frames, then 0x12; a living actor (field_0x1a4 != 0) is shaken for 48 frames
  (pose 3) first; nothing to lift: state 0 at frame 22.
- **0x12 carry** (update fn_80262BAC): field_0x344 is the speed; the carry walk cycle
  lbl_8046B790 / lbl_8046B7B0 (also fn_8026152C(bool), the cycle for other states: 24
  frames, 48 when true); a held cucco ('NIWA') glides toward field_0x324.x (0.5 a frame,
  bobbing by sinShort(4096 * dx)), lands back in 0x12.
- **0x14 jump while holding** (update fn_80262254; enter fn_80262234 / exit fn_80262B6C call
  the jump's fn_80258BBC / fn_80259660): the body of the jump fn_80258BE8 (tu_80256C74)
  plus the held thing at lbl_8046B670 once 0x3FB (fn_80245938), let go at frame 8; lands in
  0x12 while lifting, else 0.
- **0x15** (update fn_80263860, 100 %): poses 0-4 over 22 frames then 0x12; a held thief
  ('TZOK') gets Thief_804A5A30::fn_80206168 at frame 11; at pose 2 the tile under the held
  thing (BgRewrite fn_802A3164) triggers its vfunc_0x48.
- **0x18 put down** (update fn_802632A8, 100 %): poses 0-3 (4 for a player) over 12 frames,
  then state 0 with the hold cleared (an actor gets speed 0 via fn_801F3708).
- **0x19 grab** (update fn_80261750): a grabbed player is followed (its position moved to
  this layer's view corner, raised 2; effect 0x28F once at height 10); let go when it stops
  lifting (fn_8023EFEC), with speed lbl_8046B760 (4 along the facing) above z 4. Not
  grabbing: flies along field_0xd78 (gravity 0.25 as a double), bounces at half speed
  off walls (cls_804A71A8::pushedAlongX / anyFlag48), lands in 0 / 1 or 7 (fn_802466B4).
- Starts (from fn_80261480): fn_80260C08 picks up the tile object 16 ahead, spawned and
  lifted at once by fn_802A02A8's kind: 0-3 grass, 5 'STON', 6 'SSTN', 7 'BORD', 8 'ROCK',
  12 'SSTN' held as 'ROCK' (both hidden: mDrawEnabled = false), 24-35 'TUBO' (variant 1
  / 2 for 34 / 35), 39 'YASI'; fn_802604CC lifts a player in front ('PLAY'); fn_8026022C
  lifts (0x11) / pushes (0x1A) / 0x4F by the tiles 10 behind and 9.999 ahead.
- Left: fn_80263078 (enter of the cucco glide: needs Cucco_804AB8C0 byte 0x2A1, tested == 1;
  act_cucco.h is generated, so its gen_game rows must go first) and fn_80263C64 (0x7AC,
  first user of 1.25 / 1.5).

## Round 10 (tu_8023FBF0) (agent Q, Opus 5.5, 2026-11-05)

- **Counter 0xC04** (with a leader, the leader's: fn_8023619C / fn_802362C4): fn_8024ECCC adds
  and clamps to -5000..9999; fn_8024ED4C (file inline addCounter_8024ED4C, inlined twice with
  -count by fn_8024E694, the 'GOLD' drop) adds up to 9999, a negative value stops at 0. Both
  start with halt checks: `if (c > 9999) { for (;;) {} }` gives the original's `ble; b .` and
  fn_8024ED4C also writes c + 1, reads it back and halts if unchanged, then restores it (the same
  check guards 0xBFC in tu_8026FC20). Written as plain code; no assert macro is proven.
- **Probe up** fn_80248A24 / fn_80248B9C (Player members, symbols renamed): step a point 8 up
  from mPos until tile 0x231 (fn_80226998), clamped to 24 inside the view box (viewBox /
  inViewBox file inlines; layer read before getGameView), then fn_8026E5F8 24 further on sets
  0x43F (and 0x440 when clamped); B9C first climbs to tile 0x3E (fn_802265FC) and clears 0x444.
  fn_8024BBB8 requests state 0x3F when an unreachable player on the layer is within 50 / 80 / 100.
- fn_80241BA0 = the four-corner view test of a TBox2f (any / all); the bounds call takes
  `&TBox2f(*box)` (a temporary copy below the corner temporaries).
- fn_80240DCC: true in state 0x49 or when carried on a layer without view bit 1 while the
  previous layer (0xCE4) has it; the original's li / mr / mr zeroing comes from inlined
  `bool r = false; ... return r;` queries, written as such (86.2 %, scheduling+regalloc).
- cls_804A71A8::anyFlag48 takes `all` (default true): fn_8025197C(false) checks only flags 0-3.
- 0xD3C / 0xD4E: the pending conversation serial and flag (fn_8024CAC8, via fn_8022FE80).

## Round 11 (tu_80256C74) (agent S, Opus 5.5, 2026-11-05)

50 -> 52 of 63 at 100 % (+2 honest against matching: fn_8025D0B0 flips now that the
handlers before it fix the .sdata2 order; fn_8025E79C flips with the table entry copied
component by component). All five remaining update handlers are written structure-first,
so the unit's .text is 99.1 % and .sdata2 holds every literal in the original order (the
section's last 4 bytes are padding). Left unwritten: none of the state handlers.

| state | update | item | % / try.py class | what it is |
|---|---|---|---|---|
| 0x44 | fn_8025A15C | 6, level < 2 | 99.24 sched+regalloc | the Fire Rod flame of 0x45 without the block (below) |
| 0x3C | fn_8025B3F0 | 5 | 99.95 regalloc-only | a ground smash (below) |
| 0x3B | fn_8025BEB0 | 10 | 98.83 missing/extra (1) | **slingshot**: 'PCDM' (actors.tsv "Slingshot seed", med) |
| 0x3A | fn_8025C894 | 4 | 98.89 (one instruction pair) | **bow**: 'ARRW' (actors.tsv "Arrow (player)", high) |
| 0x39 | fn_8025DACC | 1 | 98.90 missing/extra (2) | a dash that burns its trail (below) |

- **0x3B slingshot / 0x3A bow** (one shape, probably one source template): six frames of
  drawing, then frame 6 holds while the button is held (fn_8022C274): field_0x3fc charges
  to 48 (the bow twice as fast at level >= 2). A full charge puts the charge point in
  field_0x12fc (carved: TVec3f; lbl_8046B040 / lbl_8046B010 by facing), lights it with
  emitters 0xCB / 0xCC that follow it through the particle callbacks field_0x12d0 /
  field_0x12e8 (the slingshot only below layer 8), plus a 'PEFF' (player effect) actor
  field_0x12c0, sound 0x2000D. Meanwhile the player walks at 1.125 (14-frame cycle,
  epsilonEqualsXY) or hops on button 0x20 (fn_80246B10): 2.5 along the stick, 1.5 up,
  gravity 0.25, 3 frames' rest (field_0x43d) after landing. Letting go shoots into the
  first free slot (slingshot field_0xb0c[3] / field_0xb4c, bow field_0xb00[3] /
  field_0xb40; argument 0x100 when uncharged), sound 0x20004. A charged slingshot shot
  at level >= 2 with all slots free sends two more along lbl_8046B070's side directions;
  a bow shot under fn_8022B160 starts the 60-frame cooldown field_0xb74. Frame 14 ends
  the state or, still held (and no cooldown), draws again. Both tick field_0x40c to 30,
  then set the camera's field_0x7f5 (gGameRoot2_8053AB10->field_0x24) unless 0x7f4.
- **0x3C** (item 5; descriptive, no actor or string names the item): the same charge to
  48 (25 while fn_8022B244) with the charge point field_0x12a4 (lbl_8046B090); on the hit
  frame (field_0xb99 rising: 1 or 5) the point field_0x5c0 (lbl_8046B0C0, the charged set
  at +4) gets the box field_0x5b0 (16 or 8 around it), BgRewrite **fn_802A1D44** (now a
  member, returns bool, caller ignores it) acts on the tile 16 in front (lbl_8046B120),
  the TV effect kind 2 / 1 (fn_802F601C) with strength 1 / player count (fn_802487FC,
  unless formation 5 or fn_80240AF0), halved below level 2; dust emitters 0x23B-0x23F and
  0x3CF / 0x3D0 by tile (0x39 / 0xD), a view shake and fn_80134DE4(2) when charged.
- **0x44** (item 6 below level 2): frame 11 of 0x45 without the Somaria block: while held,
  'FIRE' (0x1040 | facing << 4 | player) sits on field_0x580 in front; letting go places
  one 'FIRE' (0x1000 | ...) 8 in front into field_0xb1c (layer field_0xb5c, field_0xb78)
  when no flame is out and no 'FIRE' of field_0xb1c is alive (a one-slot count).
- **0x39** (item 1): a dash. Trail emitters by the ground tile (0x3D4 on 0x39; 0x116 and
  0x115 elsewhere); the first 10 frames wind up and stop when the button is let go, then
  the player runs (fn_8025D0B0) burning the eight points lbl_8046AF50 around it
  (fn_802A20F0) until field_0xba8 counts down; a side flag along the facing or another
  player ahead (fn_8022E438 / fn_802350A0) recoils (0x41). At level >= 2 off view-flag
  layers it runs ledges: the leader with its player bit (isPlayerBitSet) and
  field_0xbf0->field_0xb8 scans 16 a step inside the view box less 16 for the facing's
  ledge tile (0x12 / 0x13 / 0x10 / 0x11), counting field_0xbac; followers copy the
  leader's count (fn_8023A1A4) and at 4 jump to the leader. The actor field_0xbb4 is kept
  34 ahead (lbl_8046AFB0). Item name: not proven (no actor spawned).
- Shared: **game/BoxTest.h** (containsPoint, growBox) moved out of tu_8026FC20 (identical
  bodies; tu_8026FC20 includes it), used by fn_8025DACC.
- Idiom: a Vec table entry the original copies with three lfs / stfs (not psq) is
  `const Vec& e = table[i]; v.set(e.x, e.y, e.z);` for a field, or plain component stores
  for a local (`TVec3f(Vec)` gives psq). It flipped fn_8025E79C and lifted fn_8025D7E4
  96.5 -> 98.3.
- Open: `T pos = a + b` makes one extra stack copy where the original builds pos in place
  (fn_8025DACC, fn_8025D7E4); the charged shot's argument `(index & 3) | (facing & 3) << 4`
  comes out with the operands of the rlwimi swapped (fn_8025BEB0, fn_8025C894).
## Round 11 (tu_8022AD64) (agent T, Opus 5.5, 2026-11-05)

- Unit 331 -> 349 / 373 (+18 honest), .bss 100 %. Written: the group / all-player passes
  (fn_8022CDD4 ... fn_8023C830), fn_80239F10 (battle end test, boot path), screen x / y
  fn_8023D9B0 / fn_8023D4F8, fn_80238430 (const TVec2f*), __sinit, the weak
  Player::fn_8023F984; fn_8023F088 99.88 % (reloc-name), recursive leader queries
  fn_8023434C / fn_8023BEA8 / fn_8023C36C 99.95 % (reloc-name), fn_80235B68 96.49 %.
- **Forwarder form** (found under the old `-inline auto`; still the shape that matches): a
  forwarder that another function in this unit inlines is written inverted
  (`if (!isPlayerActive(p)) return F; return mPlayers[p]->get();`): standalone both forms
  compile alike, inlined only the inverted one gives the original's blocks. Ten are now.
- **Deferred inlining evidence** (this is what the project's `-inline auto,deferred` switch
  rests on, docs/INLINING.md): fn_8023434C inlines its own recursion three levels and calls
  the player test out of line at the bottom as fn_8023F264, so isPlayerActive is fn_8023F264
  itself (a plain function at 0x8023F264); fn_80235B68's bottom calls fn_8018A614. The 16
  functions that were unwritten then all inline functions defined after them (forwarders,
  getPlayerPos_8023E724, getPlayerLayer_8023DE58, fn_80238A68); the deferred model makes
  them writable (CRACK_PASS's open question, answered).
- **PS JGeometry**: fn_80233C08 is libs' JGeometry/Vec.h TVec3<f32>::normalize() (PSVECSquareMag
  + PSVECScale); the game's JGeometry.h has no paired-single TVec3. fn_80232A0C (cross-player
  pass, 370 boot instructions) needs the same.
## Round 12 (tu_802684A4) (agent V, Opus 5.5, 2026-11-05)

- Unit 14 -> 15 / 25 (+1 honest), every function now written. **fn_8026B81C** 100 %,
  **fn_8026C350** 98.52 % (structurally close). Their jump tables are named after the
  compiler's locals in symbols.txt (`@2537`, `@3120`, as `@1454` for fn_8026EB1C); .data
  5 % -> 93 %.
- **What the probe tiles do** (fn_8026C350, the tile under fn_80268BC4's probe point in
  direction field_0x1c; tile codes from fn_8022461C on the owner's layer):
  - 0x00 and every unlisted tile: open floor, field_0x20, unless fn_80226850 (the layer's
    other tile query) gives a hole there (6, 0xB8, 0x59). 0x0C, 0x39: nothing.
  - Solid: 0x01, 0x21, 0x24, 0x40-0x43 always; 0x22 while GameView field_0x170, 0x25 / 0x26
    while GameView field_0x171 is 0 / 1; 0x3E unless fn_80237A80(index) (tu_8022AD64).
  - Shapes by where the point lies in its 16-unit cell (`(s32)x % 16`, cellX / cellY): half
    tiles 4-7 (y < 8, y > 8, x < 8, x > 8), diagonals 8-0xB (x + y >= 16, x < y, x >= y,
    x + y < 16), corners 0x18-0x1B (both halves) and their complements 0x1C-0x1F (either).
  - Walls 0x27-0x2F: solid while the owner is >= 8 up (unless fn_8025F448), fn_80240F14, or
    group mode 4 with fn_80240670; 0x28-0x2F also over a half cell, 0x2A / 0x2B (0x2E / 0x2F)
    only at height 0 for field_0x3dc 3 (2).
  - Ledges: 0x10-0x13 set field_0xb8 and fall into 0x14-0x17, fn_8026E68C(tile). 0x3C / 0x3D
    are split cells: upper half as 0x0D, one x half solid, the other the ledge 0x10 (facing
    -y) / 0x11 (+y) or floor.
  - Push blocks: 0x30 fn_8026EE60 then (falls through) fn_8026EC88(-1); 0x31-0x34
    fn_8026EC88(tile - 0x31); 0x35-0x38 fn_8026F54C / F388 / F1C4 / F000.
  - Per-direction marks for fn_80269574, all only on the first pass (field_0xb0 <= 1):
    field_0x58 (tile 2, low part of 0x23; at height 0), field_0x60 (0x60, height 0),
    field_0x68 (3: on the ground (fn_802413F0, <= 1 up) and not fn_8024BBB0; 0x4C-0x4F the
    same for another player's tile 0x4C + i, i != index, when the root's player mask has the
    owner or, in group mode 4, fn_80240888() == fn_80248778(), and neither fn_80270A8C nor
    fn_80241230), field_0x78 (0x0D, 0x3B: solid while fn_80241230, else on the ground),
    field_0x88 (0x0E, 0x3A: on the ground), field_0x50 (0x20: fn_802413F0, below 1),
    field_0x70 (0x50). 0x58-0x5F: diagonal cells, solid half as 8-0xB, the other half
    field_0x88 under fn_802DEB58 (0x5C-0x5F: else as 0x0D). 0x0E tests fn_802DEB58 and does
    the same either way.
  - Breakables: 0x51 solid; unless fn_80270C88, field_0xba when the tile on layer + 8 (or 1)
    is 0x60, else fn_80272160(1, probe, 4). 0x52 / 0x53 solid; on the first pass with
    fn_80240F28, BgRewrite fn_802A3948 to tile 0x9D / 0x3E0 (0x53 also sound 0x20025 via
    fn_80230FD0) and 'KMRI' smoke (layer >= 8) or emitters 0xF9 / 0xFA.
  - While fn_8026FC20() >= 1 the switch is skipped: only 0x10-0x13 set field_0xb8.
- fn_8026B81C (second probe, another player's layer) has the same case groups (one
  jump-table label per group, as the original) but every acting tile only blocks.
- Source shape: the marks are early-return inlines (mark58 ... mark88; `z > 1` exits give
  the original's `bgt`, a `<=` test would add cror); the wall test is written out in each case
  (a bool inline materialises it); the 0x10-0x13 test before the switch is itself a switch.
- Open: fn_8026C350 keeps isPlayerBitSet's bool in the original (cmplwi 1) and has this / tile
  / switch copy in r30 / r29 / r31. fn_80269574 not revisited (allSet in blockMarked50 is
  worse, 95.9 %). canPush (fn_8026EE60 family): the original builds the step with a
  component TVec3f (three lfs / stfs, no psq); `TVec3f step(v.x, v.y, v.z)` reaches
  fn_8026EE60 90.2 % / F000 91.0 % but drops fn_8026EC88 (86.8 -> 85.6), so it stays.
- Player.h: declared `extern "C" bool fn_80237A80(s32 player)` (tu_8022AD64, unwritten). No
  new Player fields needed.



## Round 12 (tu_8027E5BC) (agent U, Opus 5.5, 2026-11-05)

45 -> 67 of 73 at 100 % (+22 honest); every function of the unit is written. Structural (FIXMEs in the
source): fn_8027E7E0 98.85 (scheduling), fn_8027EEB4 99.31, fn_8028126C 99.92, fn_80283D2C 99.44
(registers), fn_80282178 99.33 (scheduling, &mPos before the ground mark), fn_80282FE0 99.25 (the
distance moved: the original stores its TVec2f and squares unfused; neither length() nor square()
reproduces both).

- **The .sdata2 blocker was a dead-stripped inline, now `followStep()`**: the state 3 movement
  (formation 5 with cls_804A71A8 0xB4: fn_802808E8(1.5f); else outside 0x122C a 0.5 step toward
  fn_8027FA08's target). Defined between fn_8027F854 and fn_8027FA08 its own copy creates 1.5,
  then 0.5 and 3.0 (TUtilf::inv_sqrt inside TVec3f::normalize; 32 and 0 already exist), the
  original's order before fn_8027FA08's 4.0. Proof beyond the pool: fn_8028538C, which inlines it,
  only gets the original's stack slots (`to` 0x14, the operator- copy 0x2C) with it inlined
  (99.90 -> 100 %), and fn_8027FA08 / fn_80284430 flipped. No header inline explains the order:
  normalize alone gives 0.5 / 3.0 but not the 1.5 in front.
- MWCC evaluates call arguments right to left: `TVec3f v(t.x, t.y, t.z)` / `set(x, y, z)` from a
  Vec table loads z first. The original's x, y, z loads with three stores are member-wise
  assignments (fn_80284978, fn_80284E04, fn_80285098).
- `layer == getPlayerLayer_8023DE58(...)` with the layer read first: name the call result
  (`s32 leaderLayer = ...; if (leaderLayer == layer)`) to get `cmpw r3, rN` (fn_80285690,
  fn_802859C4 to 100 %; their old cmpwi/cmplwi FIXME no longer applied).
- Data: lbl_8046BDD8 (0xA0) split at its real boundaries into lbl_8046BDD8 Vec[4] (+-12),
  lbl_8046BE08 u32[4] (pad bits 2, 1, 4, 8 by facing), lbl_8046BE18 / lbl_8046BE48 Vec[4]
  (+-10 ahead / behind): fn_80284978 indexes them at base + 0x60 / 0x70 / 0xA0 off
  lbl_8046BDA8. lbl_8046BED8 / lbl_8046BF38 are Vec[8] unit directions by 0x3E8 (diagonals
  0.707, not 1/sqrt 2); lbl_8046C098 u8[4][8] frames by facing. .rodata is 100 %; .sdata2 differs
  only by the 4-byte tail of lbl_80540408 (size 8 in symbols.txt, likely the next unit's 8-byte
  alignment gap).

| state | update | what it is (provisional, from the update; state numbers as in round 7, consistent with the requests between them) |
|---|---|---|
| 0x0D | 80285098 | **holding a block**: stands when the tile 10 ahead is free (or 0xE / 0x39 / 0xD) and neither fn_802684A4 nor fn_80250090 holds; 0xE while the grabbed actor (fn_8024FF84) moves; the stick moves along the facing axis at 1.5; a turn or cls_804A71A8 0xA0 at 0 stands and clears 0xD98 / 0xD9C / 0xA0 / 0xA4 |
| 0x0E | 80284E04 | **moving with the block**: follows the grabbed actor's motion (its 0x3C) while it moves, 128-frame cycle, frames lbl_8046C098, sound 0x10012; back to 0x1A (fn_802456FC and not fn_80244630) or 0xD |
| 0x1A | 80284978 | **pushing**: front / back probes lbl_8046BE18 / lbl_8046BE48; carried by the pushed actor 0xDA4 (fn_8024FF6C) while it moves; letting go stands, the facing's pad bit (lbl_8046BE08) goes to 0xD |
| 0x1B | 802844F8 | **walking onto the screen**: fn_8027E5BC steps (3 for pose 0x3FD 3, else 2) until inside the visible box shrunk 39 across the facing (always after 40 frames, 0x3FE); when every present player in 0x1B (fn_80233EDC) is in it, carried (0x43F) hands over (fn_80248A24), else goes on by pose (0x12 / 0x4A / 0x4B / 0xB / 0); the acting player (lbl_8053A190) calls fn_80231368 |
| 0x38 | 80283E70 | **dropping down a ledge**: 0x344 motion, gravity 0.25 (0.17 to -6 carried / 0x441); lands at height 8 below the ledge y 0x328 when carried, else stands (0, or 8 with 0xCAD) with landing effects 0x4F3-0x4F6 (half size without 0x440) and fn_80134DE4(1, 0) |
| 0x49 | 80283948 | **held down / struggling**: pending layer switch 0xCE8 (fn_80303750 wait, fn_80290E90 / fn_80290D58); mode 0x1178 2 a 36-frame pose cycle; else button mashing (pad trigger 0xF) counts 0x3FC (free at 15, or 30 with more than one player, mode 1), decaying every 20 frames |
| 0x4A | 80282FE0 | **mode 0x42C walk** (off view-flag layers; 0x4B on them): the mode (0xBE0 = 300, effects 0x15B / 0x15C, sounds 0x20040 / 0x20041) follows the leader's (fn_8022B7C4) or toggles by pad (fn_802466B4) after the cooldown 0x430 (90); movement fn_802804E0 + fn_80242DAC; dust 0x54E by distance moved (0x401) |
| 0x4B | 80282ACC | the same on view-flag layers: movement fn_80280074, 'PEFF' actor every 50 counts of the random counter 0x402, bob 0x410 on y |
| 0x4D | 80282178 | **fired from a cannon**: sound 0x10003 at 72; landing wait 0x3FC (GBA link fn_80301CD0), layer switch 0xCEA to 0xCE4 at 0xCD4 (return point 0xCB0), ground mark fn_802F601C, effect 0x17D; in flight 0x344 + 0x380 (stick-steered, 0.8 / 0.1 / 1.6), bounce 0x4E at a wall (fn_8025190C), state 5 by pad (fn_8022C3AC) |
| 0x50 | 8028126C | (round 7) update: runs at 2 along the facing to 300 frames (0x408) or tile 0x39; the ACTOR_FIRE actor (FireRodFire_804A89F0::fn_802BAA0C) follows box 0x4C0; fire effects respawned on a turn |
| 0x51 | 80280E44 | (round 7) update: button mashing (pad trigger 0xF) takes 10 off the timer 0x46C and jolts by the pad direction (0x474, 5 frames); at 0 stands with effects 0x28B / 0x28C and sound 0x2003C |

- Helpers: **fn_80280074 / fn_802804E0** (now members) = the 0x4B / 0x4A movement: a follower steps
  4.5 toward fn_8027FA08's target, else a run along 0x3E8 whose speed 0x344 is shaped by the dash
  count 0x3FF (started by fn_80245238, sound 0x20039). **fn_80280B30** = ice sliding (cls_804A71A8
  0xB7), called by fn_802870C8. **fn_80242DAC** (tu_8023FBF0) renamed to a Player member: tile
  pushes on 0x414 / 0x418 by the tile under mPos.

## Round 14 (tu_8023FBF0) (agent Y, Opus 5.5, 2026-11-05)

- **Off-screen exit**: fn_802527B0 (99.9 %) finds an edge of the visible box within 20
  (layer < 8, z <= 16 unless fn_8025F448, moving towards it by fn_8022C82C / fn_8022C76C, or demo
  mode, or a tile fn_80225EF4 lists) whose tile 8 beyond is not code 1 (or 0x22 with GameView
  0x170); fn_802520C0 (now a Player member) moves a follower onto the acting player / leader,
  sets group mode 5, resets the trail and requests state 0x1B with targets 0x398 / 0x3A4 (56
  inside the opposite edge).
- **0x454 mode** fn_8024DCD0: frame 0x450 by state (0x4A/0x4B, fn_8025F684, else not fn_80241200)
  and facing; deletes all emitters (fn_80241E64) and ends the 'FORM' actor.
- **Conveyors** fn_80242DAC: tiles 0xB4-0xB7 (0.05, cap 0.5) and 0x95-0x98 (0.15, cap 2.0) push
  0x414; uses JGeometry TVec3f::normalize() (the file's normalizeVec inline is gone).
- **Cue** fn_8024F0E8: rumble ids 0xC / 4 / 8 / 0xB / 7 for 0x20034 / 0x20011 / 0x30009 /
  0x30004 / 0x2003A; else sound object 0x1250 (layer < 8 or 8) / 0x1254, ports 0x44 / 0x45 by
  view flag 0x800, layer 8, GameView 0x54 and fn_802D7948.
- Group loop: `s32 leader;` declared first fixes fn_80248970; the inlined copies still want leader
  and own index in two registers (lowest / highest), no shape found (24 orders measured).

## Round 15 (tu_8026FC20) (agent Z, Opus 5.5, 2026-11-06)

99 -> 102 of 120 at 100 % (+3 honest: 8813 -> 8816 on matching f0ca1f96), structural +7
(9270 -> 9277). All nine left functions are written; no drops in any unit (by address).

| function | state | % / try.py class | what it is |
|---|---|---|---|
| fn_80277F98 | 0x33 update | 99.92 regalloc | **down (no life left)**, see below |
| fn_80279824 | 0x0F update | 99.32 scheduling | **knocked back by a hit**: push 0x350 with gravity 0.25 (to -4), poses 0-3 (0 / 1 by 0x488), lands from frame 20 (0x33 without life, back to a previous 0x4A / 0x4B, else 0); hit kind 3 keeps to the view, the others slide (fn_802808E8(1.5)); hit emitters 0x135C / 0x132C turned by facing (s32 yaw table lbl_8046BC98) |
| fn_8027A1E0 | 0x0B update | 97.20 scheduling+regalloc | **the dash** (Pegasus-boots like): pose 1 runs along 0x3E8 (eight unit steps lbl_8046BC38, 0.7 diagonals) at 0xBBC, pad 0x100 hops (z 2, gravity 0.1), dust 0x24E / 0x24F (sand 0x250 / 0x251 on tile 0x39) restarted on a turn, BgRewrite at 0x30C and the eight probe points pushed out x2, 0xBC4 = 40x40 box (Player.h TBox2f); into a wall (fn_8025196C) it bounces (pose 2, sound 0x20010, view shake, 0x344 = 2.5 back, z 2) or, under fn_8022AF5C's alternative, turns the group (fn_80230BF4) |
| fn_8027AAD0 | 0x0A update | 99.60 stack offsets | **the spin attack**: fn_80274728 / fn_8027EB04; group spin (mode 3) moves the centre 0x11B4 2 a frame by the stick unless one of three tiles 24 ahead blocks (fn_802265DC), walks to the group spot from frame 11; else drifts 2 along 0x3E8 (lbl_8046BBD8); sound 0x20029; emitters 0x12B8 / 0x12BC (circling at 20 by 0x11C0) with depth / height from fn_8024EE0C / fn_8024EE14 |
| fn_8027BE74 | 0x08 update | **100** | **the sword charge**: 0x403 charge, 0x404 frames, hop with pad 0x20 (0x344 = 2.5 * stick or 0x368), walk 1.125, attack check fn_80275D9C, wall push -> 0x37 (0x350 = lbl_8046BBA8 0.25 per facing, sound 0x20062 / 0x20061 by fn_8022486C), blade emitter 0x12B0 moving out `0x403 * 14 / 48 + 6`, 0x12B4 / 0x12B8, camera 0x7F5 after 30 frames, group cancel 0xCAE / 0xCAF |
| fn_8027CC90 | 0x05 update | 99.37 scheduling | **the ground pound**: from frame 11 falls (gravity 0.25 to -6.5; on flagged layers drawn up the screen); frame 11 on the ground: tiles 3 / 0xE without fn_8024B24C cancel, else shake, rumble (fn_80134DE4(1, 0)), TV effect 1, sound 0x20024, emitters 0x23D-0x23F, 0x3D0 + 'PEFF' at 0x5C0 on tiles 0x39 / 0xD (else 0x3CF); state 6 at frame 20; emitter point 0x12A4 = mPos + lbl_8046BB78[facing] |
| fn_8027D5F0 | 0x04 update | **100** | **the sword attack**: fn_80275D9C, fn_802756E4, BgRewrite; attacks 0 / 1 / 3 fire the sword beam ('SWBM') at frame 4, 2 at frame 2, the lunge (4) on landing (body hits 0x458 frames 6-15) |
| fn_8027DEA0 | 0x04 enter | 99.04 regalloc | **attack pick**: 3+ presses in a row or window 0x11C4 -> lunge 4 (history cleared, 0x344 = 4 along the facing, z 1, sound 0x1000B, 0xBF4 = 8); window 0x11CC with the stick toward 0x11D0 -> 3; else combo step 2 / 0 / 1 by the history depth (0x117C[4..7]) |
| fn_80274CB8 | (charge step, from 0x08) | 95.40 missing/extra | the charge: air poses, walk counter 0x3F8 (reset while epsilonEquals(mPos, 0x14)), poses 0x3F9 (lbl_8046BCA8.chargePose[facing][step] before full charge), emitter point 0x12A4 = mPos + chargeOffset[full][facing]; 0x403 counts (past 48 only at sword level >= 2); 48: emitters 0xCB / 0xCC (immortal), 0x270-0x273, 'PEFF', sound 0x2000D; 168 (full): 0xCB / 0xCC below layer 8, 'PEFF', sound 0x20027; afterwards 0x26B-0x26E by player; spent immortal emitters deleted (isEnableDeleteEmitter); release (fn_80246464 false): < 48 state 0, < 168 spin 9 (0x11AC = 1), else spin 0x0A |

**State 0x33 (down)**, fn_80277F98: a fall into tile 3 / 0xE / 0x3A (not in mode 11, 0xCFC
set, not held) puts the player back at 0x2C4 (0xCBC on GBA layers) with effect 0x503; the
camera follows (Camera 0x7F4 / 0x7F8 / 0x7F6 unless 0x7F5). Frames 0-71 collapse ((frame / 6) %
4), 72-75 pose 4, 76-135 pose 6; at frame 106 (player taking part) **fn_80241920 (word 0xC04)
is halved, at most 2000 lost (fn_8024E694)** with message 0x198 (the amount) or 0x19A (nothing
lost): this is the Force Gem loss on defeat (behaviour only; the counter's name stays
provisional). From frame 136, in battle mode 7 with GameRoot 0x7E / 0x7F, the revive wait 0x494
by rank among the taking-part players by fn_80236228 (600 / 420 / 300 / 180), a helper actor
by rank (fn_801F6718 kind 3 / 5 / 6 / 7 into 0x49C, not while a 'BLZD' blizzard exists), the
emitter 0x565 (callback 0x130C, unless fn_80239D38) and a 'PEFF'; held by another player its
actor is killed. Pad 0x100 on the ground wiggles (0x3FA = 24, sound 0x20032; frame 16 sets the
box 0x5DC = mPos + (-24, -12, -8, 8) and 0xBA4). First frame: 0xCFC = 1 and, under fn_80239D38,
the music stops (lbl_80529DEC 0x1AC = AudioMgr 0x58 once, 0x253; fn_80458F9C(20)). Then one of:
held (fn_8025F418): rides on the holder (mPos from fn_8023E7B0 moved between the view corners,
+2 y / z, facing fn_80230CFC; fn_80230C6C when the holder is on a GBA layer); revived (0xD2D):
sound 0x20033, at 60 a hop (0x344 = (0, 0, 2), facing down), landing back in state 0 healed
(fn_80270DDC(max - 4), inlined); otherwise the slide 0xD78 (gravity 0.25 as a double, bounce
x / y * -0.5 off walls) and the grab: pad 0x100 / 0x200 / 0x20 inside the box (-24, -12, -8, 8)
of a partner (fn_8023B0F4) sets 0xD08 = that player (sound 0x2004B, 0xD0C = 18), who drags
the player 3 a frame toward 16 right of / 8 below itself (released at 16 or by fn_80234D94 with
0xD0C = 120); the stick crawl is fn_802808E8(1.5 / 0.12). Last fn_8027057C (revive) and the
callback point 0x1320 = mPos + (0, -4, 16).

Evidence and fixes:
- `!fn_80240808() || (fn_80240808() && !fn_80236B50(fn_80240644()))`: the asm skips when the
  second fn_80240808() is false (fn_8027776C had `!(... && ...)`, which reads the other way;
  corrected there too, 99.75 -> 99.77).
- **fireSwordBeam** (shared by fn_8027B648 and fn_8027D5F0): the life test written as early
  returns branches straight into the spawn as in the original; the old bool `lifeFull` inline
  materialised li 0 / 1 (fn_8027B648 96.22 -> 99.89, stack offsets only now).
- Shared inlines: updateStepEmitters (fn_80277220, fn_80279824), landFromHit, rewriteProbes
  (fn_8027A1E0, fn_8027AAD0), walkToGroupSpot (fn_8027B648, fn_8027AAD0), blockedAt.
- .rodata 100 %: lbl_8046BB78 (= lbl_8046BB48's values, its own object), lbl_8046BBA8 (Vec[4]),
  lbl_8046BBD8 / lbl_8046BC38 (Vec[8]), lbl_8046BC98 (s32[4] yaw); lbl_8046BCA8's two later
  members are now typed by their reader fn_80274CB8 (u8 chargePose[4][8], f32
  chargeOffset[2][4][3]). fn_80274CB8 reaches them off one .rodata base hoisted at entry; no
  source form found (separate statics, function-local statics: 96.06 / 90.97 and .rodata
  split), so it stays structurally close at 95.40.
- Argument evaluation: MWCC evaluates `a < b` with b's call first (fn_80241920() <
  fn_80236228(i) gives the original's order) and call arguments right to left
  (setEmitterPos_8013CB44(e, p, fn_8024EE14(), fn_8024EE0C() - 1, ...)).
- `isPlayerBitSet(...) != false` at frame 106: the original materialises the bit test there
  (neg / or / srwi.); `? true : false` gives the same.
- SHARED-EDITS: include/game/Player.h (0xBC4 TBox2f from u8[0x10]; fn_80274CB8 member decl),
  include/game/Map.h (extern "C" fn_8022486C decl). symbols.txt: fn_80274CB8 ->
  fn_80274CB8__15Player_804A6F38Fv (only this unit references it).

## Round 16 (tu_8023FBF0) (Opus 5.5, 2026-11-06)

181 -> 189 of 254 at 100 % (+8 honest: 8920 -> 8928 on matching 831c008b), structural
9439 -> 9446. All seven big unwritten functions are written, plus fn_802487FC (their shared
inline); .rodata is 100 % (the three tables in front of lbl_8046AE78 are source data at their
original sizes). The +8 are literal-pool order gains (fn_80254660, fn_802544B0, fn_80253E6C,
fn_802527B0, fn_8024E344 / E3D8 / E46C, fn_8024DC00). Only fn_80241B44 is still unwritten.

| function | % / try.py class | what it is |
|---|---|---|
| fn_8024BDBC | 99.53 regalloc | **call marker** of a player on its own (fn_80248970 < 2) in GameRoot mode 0x7F: pad bit 0x40 held (fn_80246B10) shows effect-system marker 5 (fn_8015F8FC) at the player (layer < 8 on its layer; layer 8 at mPos + GameView mVisibleBox.i, layer 0; above at 0xCB0); a fresh press (fn_80246D1C) adds the pad rumble (fn_8030381C) and sound 0x1E (fn_80458880, extsb index). Not in 0x454 mode |
| fn_8024E694(value, single) | 99.82 regalloc | **Force Gem drop**: at most the counter (fn_80241920) as an ACTOR_GOLD at the player (or at the acting player when it is not this player's turn in mode 4); on a GBA layer (8+) or with `single` the amount is rounded down to one gem value (1 / 5 / 20 / 50 / 100 / 150 / 200 / 300 / 1000 / 1500 / 2000 / 3000 = kinds 0-11, param kind \| 0x40000, 0x70000 in state 0x51), else the amount itself (\| 0x60000 in state 0x51); then fn_8024ED4C(-amount). Callers: fn_8024E344 / E3D8 / E46C (5 / 20 / 50 + random), state 0x33's defeat loss (fn_80277F98), fn_80271FD8, fn_80236074 |
| fn_8024B2E0 | 92.26 missing/extra (-4) | **trail reset in 0x454 mode** (fn_8024BAEC): follower: trail = 0x32C, grounded; alone: 0x32C; leader: 12 entries per follower interpolated linearly from its position to the next group member (fn_8022E438), the rest at the last; then 0x284[i] = entry z > 0 |
| fn_8024D134 | 99.65 regalloc | **group-spot marker restart** (leader, not GameRoot 0x7E, modes 0-3): clears 0x121D, deletes 0x1388 and spawns 0x35E / 0x360 / 0x35F / 0x361 by mode at fn_8027F1C8's spot (8 a place beside the group in modes 0 / 1 via fn_802487FC, (8, 8) in mode 2, 0x11B4 in states 9 / 0xA else 16 toward the player's side in mode 3) |
| fn_8024F63C | 99.07 scheduling+regalloc | **join a push** (state 0x1A): with pad 0x20 held (fn_802456FC), the partner 0xDAC's pushed actor (its 0xDA4) still alive, or a fresh press (fn_80245938), not a follower in mode 5, the point lbl_8046AE28[facing] (10 behind) free of fn_802265DC / fn_802684A4: a present player inside the box +-1 at mPos + lbl_8046AE58[facing], facing the same way and held by nobody (fn_80238EDC), is taken: from states 0 / 2 / 3 (0x11 unless fn_8025F500 / fn_8025F590) the push 0x1A is requested, else 0xDAC = it, 0xDA4 = its pushed actor, 0xDA8 = 3 frames; returns 0xDA8 > 0. Callers: fn_8024FE78, state 0x1A, tu_80285E58 |
| fn_80250090 | 99.21 regalloc | **join a block carry** (state 0x0D's partner): on the ground and not a follower in mode 5, keeps 0xD9C while its fn_80241478 holds, else takes a present player whose fn_80250ACC holds inside the box +-1 at mPos + lbl_8046AE08[facing] while the stick points along the facing (fn_80247330 / fn_8024712C): 0xD9C = it, 0xD94 = its grabbed actor (fn_8024FF84), 0xD98 = 3 |
| fn_80250FD4 | 99.86 regalloc | **pushing along the facing**: state 0x1A and the stick toward the facing (fn_80244630's test without the input gate); used by fn_80250D30 |
| fn_802487FC | 94.95 different | places in use: 1 in mode 4, else fn_80248970; the plain function its callers inline (fn_8024D134 here, fn_8027F1C8 calls it). Its own copy zeroes count / index li / li, ours li / mr (the known group-loop issue); the inlined copies are li / mr in both |

Evidence and fixes:
- **0x31 loops count with `int`**: with `s32` (long) MWCC emits an entry test (cmpwi 0x31;
  bge) before the 7x-unrolled ctr loop, which the original never has. fn_802408C8 96.0 ->
  99.05 (regalloc only) with `int i`, fn_8024B2E0 likewise.
- fn_80241920 as an `if` (`value >= 0` return value, else 0): its own copy stays branchless
  (100 %), the inlined copies in fn_8024E694 branch (cmpwi / blt) as in the original; `?:`
  made them branchless.
- The box `TBox2f(-1, -1, 1, 1)` moved by mPos is written field by field (i.x, f.x, i.y,
  f.y): the original adds x to both corners first; addPos(x, y) adds by corner and loses two
  instructions.
- fn_8024E694's flag is tested by its low byte (clrlwi.): a bool in the original, but the
  forwarder fn_80236074 (tu_8022AD64, generator-pinned) and the symbol (Fll) have s32, so the
  test is `(u8)single` with a comment.
- fn_8024F63C: `s32 facing = field_0x3dc;` before fn_80230CFC(i) (the original loads the field
  first into a saved register); `if (field_0xda8 > 0) return true; return false;` branches,
  fn_80250090's `return field_0xd98 > 0;` does not, as in the original.
- .rodata: lbl_8046AE08 / lbl_8046AE58 f32[4][2] (+-10 along the facing), lbl_8046AE28
  Vec[4] (10 behind the facing).
- SHARED-EDITS: include/game/Player.h (fn_8024E694's parameter named `single`).

## Round 17 (tu_8025EB18) (Opus 5.5, 2026-11-06)

43 -> 45 of 59 at 100 % (+2 honest: 8960 -> 8962 on matching c0c9afa0, no drops by address).
Both unwritten functions are written, so every function of the unit is in place (.text
97.9 %; .sdata2 bytes equal to the original, only the 4-byte tail padding before
tu_80264998's 8-aligned data differs).

| function | % / try.py class | what it is |
|---|---|---|
| fn_80263C64 | 100 | **update of state 0x12, the carry** (tu_80285E58's dispatch: case 0x12). The stick step (fn_8027FA08's z of 0 puts mPos.z on the floor unless 0x434); with fn_8022C13C (unless fn_802416C4 is 7): put down (0x18) a held player that fn_80236B50 rejects, a bomb ('HBMB') or a small key ('KEY0' via fn_8025F648), else throw (0x16, also when fn_8024B24C); fn_8022C0A0 throws (0x16); fn_802466B4 starts 0x15 unless the held actor's field_0x1a4 >= 2. Then walks (fn_802808E8 at 1.25 with a player, 1.5 with an actor whose field_0x1a4 is 0, none otherwise), fn_8027EB04, and the carry cycle fn_8026152C(false) inlined three times; a held player that stops being one (fn_8023EA28) or a lost actor ends the carry (state 0). First user of 1.25 / 1.5 |
| fn_8026152C(slow) | 100 | the carry walk cycle (flipped by the VecEpsilon.h change below) |
| fn_80263078 | 99.78 regalloc | **enter of state 0x13** (dispatch case 0x13; update fn_80262BAC): after fn_8028414C, with a held cucco ('NIWA', Cucco_804AB8C0 byte 0x2A1 == 1) and no 0x441, probe 16-steps along field_0x344.x inside the layer's view box (layerBox + containsPoint) for the second tile 0x10-0x17 (fn_802265FC); found, field_0x324 = the point one step further and the glide speed z 2; else mPos.y = field_0x324.y (z takes the difference). Faces along field_0x344.x (0x3DC / 0x3E8 = 0 right, 1 left) |

Corrections and evidence:
- **State numbers**: tu_80285E58's update switch has case 0x12 -> fn_80263C64 and case 0x13 ->
  fn_80262BAC (enter 0x13 -> fn_80263078, enter 0x15 -> fn_80263BA0). Round 10's "0x12 carry
  (update fn_80262BAC)" is state 0x13: the fall / cucco glide while carrying, which lands back
  in 0x12; fn_80263BA0 (pose by the held actor's field_0x118) is the enter of 0x15, not 0x12.
- **VecEpsilon.h**: the vector epsilon tests now pass the component difference
  (`nearZero(a.x - b.x, eps)`) instead of TUtil::epsilonEquals(a.x, b.x, eps), whose `a1 -= a2`
  on its parameter kept 32 * FLT_EPSILON and the difference in fresh registers in every caller
  (the "keeps 32 * FLT_EPSILON in f2 (ours f3 / f4)" FIXMEs). Flipped fn_8026152C and
  fn_80263C64; raised tu_80256C74 fn_80259830 99.29 -> 99.50, fn_8025A15C 99.24 -> 99.44,
  fn_8025BEB0 98.83 -> 98.98, fn_8025C894 98.89 -> 99.06, tu_8027E5BC fn_8027EEB4 99.31 -> 99.94,
  tu_8026FC20 fn_80274CB8 95.40 -> 95.57 (none lower; their FIXME numbers are not updated).
- **Cucco_804AB8C0 0x2A1** (u8): the cucco's own code tests it with cmplwi and sets it from
  bit 0x1000 of 0x90 (0x803331DC); the player compares it with **cmpwi 1**, which an int
  accessor gives (`getField0x2a1()`; the plain byte compare gives cmplwi, 99.35 %).
- **Plain functions instead of file inlines** (deferred model): fn_8025F590 (holds a player),
  fn_8025F448 (grabbing), fn_8025F2CC (may lift), fn_8025FCC8 (lets go) and fn_8025F684
  (lifting) now hold their bodies and the callers call them; no wrapper copies are left.
  Same results except fn_8025FCC8 87.39 -> 87.02 (still missing/extra: 0x11 tested on its own
  and the field_0xd64 == 1 byte recomputed in each inlined fn_8025F590). Tried and measured:
  a lone `state == 0x11` (separate early return or `x >= 0x12 && x <= 0x16`) does not fold
  the range as the original; fn_8025F590 via `fn_8025F684() == true` gives FCC8 90.8 % but
  turns fn_8025F698 into `different` (neg / or / srwi bool conversions).
- fn_80262BAC calls fn_8026152C(false) instead of a hand-expanded copy of its body (same code).
- Still open: fn_80261750 (one extra lwz of field_0xd70 before fn_8023EFEC; a local or
  fn_8025F534 do not help), fn_8025FDD0 / fn_8025EB18 (view-corner float registers),
  fn_80262254 (as tu_80256C74 fn_80258BE8: &field_0x344 computed before the x / y test),
  fn_802604CC (stack TVec2 temporaries), fn_80260C08 / fn_8026022C (vector load order).


## Round 18 (tu_80285E58) (Opus 5.5, 2026-11-06)

3 -> 5 of 8 at 100 % (+2 honest: 9000 -> 9002 on matching 1b96d7eb, no drops or lower
percentages anywhere by address). .text 98.67 -> 99.90 %, .data (the three jump tables) 50 ->
100 %. The state tick fn_802870C8 (9 KB, exit / commit / enter / update switches) is 100 %, so
the three switches, their case groupings and every handler call are proven as written.

| function | before | after / class | what flipped it |
|---|---|---|---|
| fn_802870C8 (state tick) | 98.34 missing/extra | 100 | the five fixes below |
| fn_80286688 (0x13B8 Z toggle) | 88.68 | 100 | u8 mode inline + one early-out condition |
| fn_80286E10 | 99.20 | 99.20 regalloc-only | unchanged (loop counter / player / start share r27 in the original) |
| fn_80286B74 | 99.17 scheduling+regalloc | 99.28 regalloc-only | `target.set(mPos)` in the plain branch (x, y, z stored in order); normalize f-registers left |
| fn_8028676C | 99.67 | 99.67 regalloc-only | unchanged (f4 / f5 of the two planar lengths; operand swap gives 95.5) |

What the asm proves (all source forms in docs/RULES_CARD.md "Known game-code shapes"):
- **Mode 0xB byte test** (subfic / cntlzw / extrwi.): `inline u8 isGameMode(s32 mode)` in
  game/GameMode.h (`return ...field_0x4c == mode ? true : false;`, shared with tu_8026FC20's
  mode-7 test). fn_80286688's three early-outs (mode 0xB, root 0x7E, player bit clear) are
  one `||` condition; separate `if`s give `beq` where the original has `bne +8; b end`.
- **Index and layer getters**: the original reads field_0x4 and field_0x3b4 through inlined
  getters. Evidence: it loads them into r29 before the other calls of the same expression
  (spawnEmitter_8013CC50's layer before fn_8024EE0C / fn_8024EE14, five trail emitters;
  fn_802255F4's layer; `fn_80239040(0xD9C) == index`, `fn_80238F78(0xDAC) == index`), which
  MWCC does for an inlined call, never for a member read. Player_804A6F38::getIndex() /
  getLayer() (Player.h) are used for every read in the unit; they flip those sites and change
  nothing else (the other functions are byte-identical with them).
- **Partner checks**: `fn_80240888() != fn_80248778()` (fn_80248778 is called first and kept in
  r29; `cmpw r3, r29`), three sites, no local.
- **States 8 / 9-10 / 0x39** (partner warp): `field_0x1240 == 8 || field_0x1240 == 9 ||
  field_0x1240 == 10 || field_0x1240 == 0x39` on the member; a local folds 8-10.
- **0xCBC / 0xD1C**: `field_0xd1c = field_0xcbc = mPos;` (both layer-change sites).
- **lbl_8053ABA8 / lbl_8053ABF0** (ZLD2 / dodongo hurt flags): compared with `cmpwi`:
  `(s32)flag != 0`, the reviewed cucco form.

## Round 20 (tu_802684A4) (Opus 5.5, 2026-11-06)

Unit 15 -> 20 / 25 (+5 honest). The 7.8 KB probe-tile switch fn_8026C350 is 100 %, so its
case groups, the wall / half / diagonal / corner tests and every mark helper are proven as
written; .data (its jump table) is 100 % with it.

| function | before | after / class | what flipped it |
|---|---|---|---|
| fn_8026C350 (tile switch) | 95.33 missing/extra | 100 | inline budget via setDir, u8 isPlayerBitSet, getIndex / getLayer |
| fn_8026F000 / F1C4 / F388 / F54C | 90.04 each | 100 each | canPush's member-wise step, getFacing (they inline fn_8026EE60) |
| fn_8026EE60 (push 'BLCK') | 86.41 | 99.86 regalloc-only | same; the serial takes the owner's r29, not the facing's r30 |
| fn_8026EC88 (player block) | 86.81 | 99.87 regalloc-only | same, `(dir & 3) << 2 \| ((player + 1) & 7) << 4` |
| fn_80269574 / 80268BC4 / 802685D4 | 96.27 / 99.13 / 92.27 | unchanged | see below |

What the asm proves:
- **fn_8026C350's out-of-line calls are its inline budget** (docs/INLINING.md rule 5), not
  per-callee pragmas. The original calls fn_8026EE60, fn_8026EC88 and fn_8026F000-F54C out of
  line although fn_8026F000-F54C inline fn_8026EE60. With every `flags[field_0x1c] = 1` written
  through one helper, `setDir(p, flags)` (57 sites in fn_8026C350 and its mark helpers; used
  file-wide), the caller's size reaches the budget at exactly that call list with no pragma.
  Measured: written out, ours inlined all six; the two auto_inline regions around
  fn_8026F000-F54C and fn_8026EC88 hid four of them, and fn_8026EE60 stayed inlined. Padding
  the caller showed the threshold (21 padding statements without the helper, 0 with it) and
  that the position of the extra size in the function does not matter. Only fn_8026EB1C keeps
  its region (fn_8026E68C, a small caller, calls it out of line).
- **isPlayerBitSet returns u8** (game/PlayerBit.h): the 0x4C-0x4F case keeps the value (neg /
  or / srwi, then `cmplwi 1` for `== true`); a bool return folds it into `and.`. Full build:
  no other function changes.
- **Player::getFacing()** (0x3DC), a getter like getIndex / getLayer: fn_8026EE60 / EC88 load
  the facing into a saved register ahead of getActorManager_801F856C, and canPush's
  `field_0x1c == getFacing()` gets the original's registers (cmpw r0, r4). Plain reads, a local
  read once, or a local manager pointer do neither.
- **canPush's step**: `const Vec& d = lbl_8046B998[facing]; TVec3f step; step.x = d.x; ...;
  ahead += step;` (lfs x / y / z, stfs, then psq_l). A TVec3f(Vec) copy gives psq_l, the
  (x, y, z) constructor or a temporary loads z first, indexing the table per member gives lfsx.
- The getters at fn_8026C350's other sites: `tile - 0x4C != getIndex()` and BgRewrite
  fn_802A3948(&p, index, getLayer(), tile) (layer loaded before the index, as the original).

Still open:
- fn_8026EE60 / EC88: the serial's register. Declaring `s32 serial; s32 dir;` first (dir
  assigned from a `Player*` local, then an `ActorManager*` local) gives fn_8026EE60 100 % but
  fn_8026F000-F54C 99.65 %; no form found that gives all five.
- fn_80269574 (96.27): the counter / warp section. The original's reset paths branch straight to
  the code after the warp search and the `field_0xbc = 3` exits of the unrolled 8-way loop to
  the search; a bool inline (advanceCounter), a found flag, an index test or a nested loop each
  materialise a value or keep the loop rolled (allSet 95.92, found flag 95.94, nested loop
  95.61, `n >= 8` 96.27). blockMarked50's second loop is the same shape.
- fn_80268BC4 (99.13): the end's `moved` TVec2f is stored to the stack before and after the
  subtraction in the original; `field_0x14 = moved`, `field_0x14.set(moved)`,
  `moved.set(...)` and a default-constructed `moved` all keep it in registers.
