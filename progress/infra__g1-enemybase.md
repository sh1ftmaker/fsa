# infra/g1-enemybase (G1 item 7: EnemyBase.h)

Baseline 5021 / 11557 functions (game 309); after: 5056 (game 344).

## Headers
- `include/game/EnemyBase.h`: `EnemyBase_804A4808` (vtable 0x804A4808, 39
  slots: ActorBase's 19 + vfunc_0x54..vfunc_0xA0 with evidence per slot),
  inline virtual dtor (weak copy in act_rope, dtor_801B7E1C), fields
  0x230-0x26C + `SpriteAnim_801D267C mAnim` at 0x270, size 0x2A8 (spawner
  __nw__(0x2A8) for FSPO), `EnemyFlag_804A4808` bits, 37 exported members of
  tu_801CDC8C, `lbl_8053EFC0` (extern const bool), fn_801CD2F8, fn_801CD950.
- `include/game/SpriteAnim.h`: the 0x270 member (tu_801D1C18), 0x38 bytes.
- `include/JSystem/J3DGraphAnimator/J3DAnimation.h`: J3DFrameCtrl (tww).
- `include/game/act/act_fuse_point.h`: FusePoint_804A54C0.

## SHARED-EDITS
- ActorBase.h `field_0x94` s16 -> u16 (311/324 loads are lhz; fn_801CC910
  cmplwi). No matched code changes.
- ActorBase.h `fn_801F3D94(bool)` -> `(u8)` and symbols.txt
  `...Fb` -> `...FUc`: update() passes a bool local with clrlwi 24, the
  code MWCC emits for an unsigned char parameter (bool -> bool is `mr`).
  Same reason for vfunc_0x90 / vfunc_0x9C / fn_801CE2BC (FUc).
- ActorManager.h: declarations of fn_801CC910 / 960 / 9B0 (Force Gem tests).

## Units
- tu_801CDC8C 0 -> 23/57; tu_801D1C18 0 -> 5/12; tu_801CB270 5 -> 8 (+
  fn_801CC910/960/9B0, fn_801CD2F8); act_fuse_point 0 -> 4/5.
- Resistant (FIXMEs in source): update 92.3 (regalloc this/flag, mFlags
  reloads, hit-bit bool test); vfunc_0x98 / fn_801CE32C ~96; fn_801CE5D4
  99.8 regalloc; fn_801D0DA0 / fn_801D0D50 (target does not fuse
  dx*dx + dy*dy into fmadds); fn_801D0280 / 02E0 (atan2 operand order);
  fn_801D0DD4; fn_801CDCD0 / fn_801CE010; SpriteAnim calcCel regalloc
  (four functions at ~98.5), fn_801D208C 94.4, fn_801D21F0 86.2,
  fn_801D2608 91.7. init / ctor / vfunc_0x84 / fn_801CFCF0 differ only in
  .sdata2 literal order, which completes once the unwritten functions
  (fn_801CE694 uses 8.0f, fn_801CEA60 32.0f, ...) are in.

## Next
Write the 20 remaining tu_801CDC8C functions (list at the top of the
file) in address order, which fixes the literal order; fuse point init.
