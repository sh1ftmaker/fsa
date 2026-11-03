# game/act_iron_ball_manager

Baseline: total: 5820 / 11557 functions, 1165664 / 4442032 bytes of code (26.24 %)
After: total: 5821 / 11557 functions, 1165872 / 4442032 bytes (26.25 %)

Matched: dtor, init, __sinit (3/4).
Resistant: update 98.88 % (missing/extra). Original: `li r0,-2` hoisted, counter
decrement in r3, `and r0,r3,r0` for `field_0x90 &= ~1`; ours `clrrwi r0,r0,1`, one
instruction fewer. Tried `&= ~1`, `&= 0xFFFFFFFE`, clear before decrement (worse).
.sdata2: ours is 0x2c, target 0x30 (a trailing 0.0f pad word after 0.5f, nothing here uses it).
SHARED-EDITS: none besides this unit's generated header (rows deleted from gen_game.tsv).
