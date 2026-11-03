# JSystem/J3DU/J3DUMotion.cpp

Status: Matching (7/7 functions, .text/.data/.sdata2 100 % in the honest
report, link_check --unit SHA-1 OK). Replaces the rejected placeholder
unit tu_80079034 (review: pikmin2's J3DU links before JKRAram).

- Source: pikmin2's J3DUMotion.cpp (CC0), adapted to the headers here:
  JMASCosShort/JMASSinShort -> JMASCos/JMASSin (libs/sdk JMath.h names),
  PSMTXQuat without pikmin2's PSQuaternion cast, MTXSetPosition (not in our
  mtx.h) written out as the three translation stores. All of its own
  functions are dead-stripped by FSA's link, so these edits don't reach the
  DOL; what remains are the weak J3DMtxCalc copies, __vt__10J3DMtxCalc and
  getWeight's 0.0f.
- Dead data stripped by the trial-link evidence: the J3DMtxCalcBlendAnm
  vtables, __vt__22J3DMtxCalcBlendAnmBase and two unused floats.
- symbols.txt: placeholder lbl_8053B6C0 0x8 -> 0x4 (0.0f + padding before
  JKRAram's .sdata2, as the map shows).
- tools/fdiff.py shows .sdata2 50 % because it diffs the unstripped object;
  the honest report (stripped) is 100 %.
- lib_map.tsv: control.cpp row ends at 0x80079034; rows for J3DUDL.cpp and
  J3DUMotion.cpp (library JSystem:J3DU).
- configure.py: new J3DU block after mkdd_JMessage (J3DUDL.cpp's branch adds
  its line to the same block).
