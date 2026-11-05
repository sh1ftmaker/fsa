# game__tu_8028FBA0 (Player states 0x1C-0x32)

## Round 4, agent F (Opus 5.5), 2026-11-05

Baseline (matching 5babbf0b): 12 / 53, progress total 8410 / 11557.
After: 24 / 53 at 100 % in the unit file plus 26 structurally done
(reloc-name only: literal-pool order). See docs/PLAYER.md "Round 4 (state
handlers)" for what each state is.

Not written:
- fn_80290070 (0xCE8): inlines fn_80290D58 and fn_80290E90, both defined
  after it (needs deferred inlining; left unwritten per the rules).
- fn_80290FC0 (state 0x32 update): needs Player 0x1344 typed as
  `JGeometry::TVec3f field_0x1344;` (now `u8 field_0x1344[0x1350 - 0x1344]`;
  agent E owns field carving). With it: 99.62 % (scheduling: one lwz of
  0x135C and an li r3/r5 pair), and every reloc-name function of the unit
  flips (51 / 53). Patch below (apply after the field is typed, `git apply
  --3way`; drop the
  Player.h hunk if E already typed it; then delete the 26 literal-pool
  FIXMEs and give fn_80290FC0 its scheduling FIXME).

SHARED-EDITS:
- include/game/Player.h: member declarations added (raiseHeldActor,
  spinFacing, stepWalkCycle, setHeldPos, stepTransitionAnim, spawnEffect17D,
  followCamera, requestArrival, spawnEffect15B, life, fallStep,
  riseToTarget: unit-local inlines of tu_8028FBA0; fn_8026152C(bool) of
  tu_8025EB18); fn_80290070 returns bool (was u8; callers test clrlwi.).
- include/game/GameGlobals.h: `bool fn_80301CD0(s32 layer);` (tu_80301CD0,
  already defined there with C++ linkage).
- config/G4SE01/symbols.txt: fn_8026152C -> fn_8026152C__15Player_804A6F38Fb
  (r3 = Player: reads mPos / 0x14, writes 0x2F4; r4 tested with clrlwi.).

```diff
diff --git a/include/game/Player.h b/include/game/Player.h
index c4aeba87..74aecbab 100644
--- a/include/game/Player.h
+++ b/include/game/Player.h
@@ -1006,7 +1006,7 @@ public:
     /* 0x1320 */ u8 field_0x1320[0x132C - 0x1320];
     /* 0x132C */ JPABaseEmitter* field_0x132c;
     /* 0x1330 */ cls_804A64A8 field_0x1330;
-    /* 0x1344 */ u8 field_0x1344[0x1350 - 0x1344];
+    /* 0x1344 */ JGeometry::TVec3f field_0x1344; // TEMP-LOCAL
     /* 0x1350 */ JPABaseEmitter* field_0x1350;
     /* 0x1354 */ JPABaseEmitter* field_0x1354;
     /* 0x1358 */ JPABaseEmitter* field_0x1358;
diff --git a/src/game/tu_8028FBA0.cpp b/src/game/tu_8028FBA0.cpp
index 18c75968..52fbdc0c 100644
--- a/src/game/tu_8028FBA0.cpp
+++ b/src/game/tu_8028FBA0.cpp
@@ -11,6 +11,11 @@
 #include "game/BgRewrite.h"
 #include "game/Camera.h"
 #include "game/Map.h"
+#include "JSystem/JParticle/JPAMath.h"
+
+// The emitters' y rotation per facing (0x3DC: right, left, down, up), read
+// by fn_80290FC0.
+static const s32 lbl_8046C228[4] = {0x4000, 0xC000, 0x0000, 0x8000};
 
 // The spin while rising or dropping (updates of 0x2A, 0x2B, 0x22, 0x1F:
 // fn_80291B8C, fn_80291910, fn_80293398, fn_80293900): the facing 0x3DC/0x3E8
@@ -310,6 +315,154 @@ void Player_804A6F38::fn_80290F20() {
     }
 }
 
+void Player_804A6F38::fn_80290FC0() {
+    if (field_0x3fc == 40) {
+        if (field_0xcf4 == 0xC && fn_802DE9A0()) {
+            fn_8013CC50(field_0x3b4, &mPos, 0x2AA, 0, 1, NULL, NULL, true, 1.0f);
+            fn_8013CC50(field_0x3b4, &mPos, 0x2A9, -1, 0, NULL, NULL, true, 1.0f);
+        } else if (field_0xcf4 == 0xD) {
+            fn_8013CC50(field_0x3b4, &mPos, 0x45F, 0, 1, NULL, NULL, true, 1.0f);
+            fn_8013CC50(field_0x3b4, &mPos, 0x45E, -1, 0, NULL, NULL, true, 1.0f);
+            switch (field_0x4) {
+            case 0:
+                fn_8013CC50(field_0x3b4, &mPos, 0x16B, 0, 1, NULL, NULL, true, 1.0f);
+                break;
+            case 1:
+                fn_8013CC50(field_0x3b4, &mPos, 0x16D, 0, 1, NULL, NULL, true, 1.0f);
+                break;
+            case 2:
+                fn_8013CC50(field_0x3b4, &mPos, 0x16A, 0, 1, NULL, NULL, true, 1.0f);
+                break;
+            case 3:
+                fn_8013CC50(field_0x3b4, &mPos, 0x16C, 0, 1, NULL, NULL, true, 1.0f);
+                break;
+            }
+            fn_8013CC50(field_0x3b4, &mPos, 0x16E, 0, 1, NULL, NULL, true, 1.0f);
+            fn_8013CC50(field_0x3b4, &mPos, 0x16F, 0, 1, NULL, NULL, true, 1.0f);
+        }
+    }
+    if (field_0xcf4 == 0xD) {
+        if (field_0x3fc < 130) {
+            field_0x3fc++;
+            return;
+        }
+        if (!fn_80240670()) {
+            fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
+        }
+        s32 hp = life();
+        s32 state = 0;
+        if (hp <= 0) {
+            state = 0x33;
+        }
+        field_0x124d = 0;
+        if (fn_80251AEC(state)) {
+            field_0x3b4 = field_0xce4;
+            mPos = field_0xcd4;
+            mPos.z = 0.0f;
+        }
+        return;
+    }
+    if (field_0x3fc >= 58) {
+        if (field_0x3fb == 0) {
+            fn_8024F0E8(0x1000A);
+            if (field_0xcf4 == 0xC && fn_802DE9A0()) {
+                fn_8024F0E8(0x20031);
+            }
+            field_0x3fb = 1;
+            if (field_0xcf4 == 0xC && fn_802DE9A0() && field_0x135c == NULL) {
+                switch (field_0x3dc) {
+                case 0:
+                    field_0x135c = fn_8013CC50(field_0x3b4, &mPos, 0x248, 0, 0, NULL, NULL, true, 1.0f);
+                    break;
+                case 1:
+                    field_0x135c = fn_8013CC50(field_0x3b4, &mPos, 0x247, 0, 0, NULL, NULL, true, 1.0f);
+                    break;
+                case 2:
+                    field_0x135c = fn_8013CC50(field_0x3b4, &mPos, 0x246, 0, -1, NULL, NULL, true, 1.0f);
+                    break;
+                case 3:
+                    field_0x135c = fn_8013CC50(field_0x3b4, &mPos, 0x249, 0, 0, NULL, NULL, true, 1.0f);
+                    break;
+                }
+            }
+            if (field_0xcf4 == 0xC && fn_802DE9A0() && field_0x132c == NULL) {
+                switch (field_0x3dc) {
+                case 0:
+                    field_0x132c = fn_8013CC50(field_0x3b4, &mPos, 0x244, 0, 0, NULL, &field_0x1330, true, 1.0f);
+                    break;
+                case 1:
+                    field_0x132c = fn_8013CC50(field_0x3b4, &mPos, 0x243, 0, 0, NULL, &field_0x1330, true, 1.0f);
+                    break;
+                case 2:
+                    field_0x132c = fn_8013CC50(field_0x3b4, &mPos, 0x242, 0, -1, NULL, &field_0x1330, true, 1.0f);
+                    break;
+                case 3:
+                    field_0x132c = fn_8013CC50(field_0x3b4, &mPos, 0x245, 0, 0, NULL, &field_0x1330, true, 1.0f);
+                    break;
+                }
+            }
+            JGeometry::TVec3f velocity = (field_0xcd4 - mPos) * (1.0f / 64.0f);
+            field_0x344.set(velocity.x, velocity.y, 8.0f);
+        }
+        if (field_0x3fc < 62) {
+            field_0x3fd = 0;
+        } else if (field_0x3fc < 66) {
+            field_0x3fd = 1;
+        } else if (field_0x3fc < 70) {
+            field_0x3fd = 2;
+        } else if (field_0x3fc < 74) {
+            field_0x3fd = 3;
+        }
+        field_0x3fc++;
+        mPos += field_0x344;
+        field_0x344.z -= 0.25f;
+        if (field_0x344.z < 0.0f && mPos.z <= 0.0f) {
+            mPos.z = 0.0f;
+            if (!fn_80240670() && (field_0xcf4 == 0xC || field_0xcf4 == 0xD || field_0xcf4 == 0x26)) {
+                fn_80272160(2, (const JGeometry::TVec2f*)&mPos, 0x11);
+            } else {
+                fn_80272160(0, (const JGeometry::TVec2f*)&mPos, 0x11);
+            }
+            s32 hp = life();
+        s32 state = 0;
+        if (hp <= 0) {
+            state = 0x33;
+        }
+            field_0x124d = 0;
+            if (fn_80251AEC(state)) {
+                field_0x3b4 = field_0xce4;
+                mPos = field_0xcd4;
+                mPos.z = 0.0f;
+            }
+        }
+        if (field_0x135c != NULL) {
+            switch (field_0x3dc) {
+            case 0:
+            case 1:
+            case 3:
+                fn_8013CB44(field_0x135c, &mPos, 0, 0, true);
+                break;
+            case 2:
+                fn_8013CB44(field_0x135c, &mPos, 0, -1, true);
+                break;
+            }
+            JPAGetXYZRotateMtx(0, lbl_8046C228[field_0x3dc], 0, field_0x135c->mGlobalRot);
+        }
+        if (field_0x132c != NULL) {
+            field_0x1344.set(mPos);
+            if (field_0x3dc == 2) {
+                field_0x1344.y -= 3.0f;
+            } else if (field_0x3dc == 3) {
+                field_0x1344.y += 3.0f;
+            }
+            field_0x1330.field_0x4.set(field_0x1344);
+            JPAGetXYZRotateMtx(0, lbl_8046C228[field_0x3dc], 0, field_0x132c->mGlobalRot);
+        }
+    } else {
+        fn_8027EDDC();
+    }
+}
+
 void Player_804A6F38::fn_8029185C() {
     field_0x124d = 1;
     field_0x458 = 0;
```
