# dolphin/gx/GXInit.c

Status: Matching (5/5 functions, all sections 100 %). No source changes were needed.

Fixes were all in config:
- splits.txt: added `.sbss 0x8053A6A0-0x8053A6C8` to the GXInit stanza
  (__piReg/__cpReg/__peReg/__memReg, peCount/time/calledOnce, resetFuncRegistered).
- symbols.txt renames (evidence: function order in GXFifo.c / GXMisc.c; GXInit's
  references for the four render modes it uses; 0x80497194 holds TV mode 0 / XFB
  mode 1 = GXNtsc480Int per dolsdk2004 GXFrameBuf.c, and its only user
  JUTException::createFB uses &GXNtsc480Int in mkdd; oot-gc's declaration order
  differs from FSA's SDK build):
  fn_8005A194 -> GXSetBreakPtCallback, fn_8005BFC8 -> GXSetDrawSyncCallback,
  fn_8005C094 -> GXSetDrawDoneCallback, lbl_80497158 -> GXNtsc480IntDf,
  lbl_80497194 -> GXNtsc480Int, lbl_804971D0 -> GXMpal480IntDf,
  lbl_8049720C -> GXPal528IntDf, lbl_80497248 -> GXEurgb60Hz480IntDf.

SHARED-EDITS: none.
Next: GXFrameBuf.c has no .data split yet; the render-mode objects at
0x80497158-0x80497288 belong to it.
