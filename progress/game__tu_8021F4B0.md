# game/tu_8021F4B0 (G4 wave 24)

Baseline: total: 6287 / 11557 functions, 1258900 / 4442032 bytes of code (28.34 %)
After:    total: 6293 / 11557 functions, 1260664 / 4442032 bytes of code (28.38 %)

Matched: 7/7 (update, setState, deleteEmitters, init, dtor, ctor, __sinit).
Resistant: none. Extab/extabindex show 52.6 % in try.py (weak-duplicate entries; same as other units).
SHARED-EDITS: config/G4SE01/gen_game.tsv (removed this unit's one row, sinit); symbols.txt placeholder renames (own lines).
Notes: fn_8021F924's loop is duplicated as a file-static inline (deleteEmitters) for the state-5 end of update;
the out-of-line function only matches with the raw loop. Local `JPAEmitterManager* manager` is what makes the
reload of mEmitters[i] after the call match. Not Matching-flagged (coordinator does that).
