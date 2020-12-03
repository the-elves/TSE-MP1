--- files/before/RenderSVGRoot.cpp	2020-12-03 05:32:34.935748905 +0530
+++ files/after/RenderSVGRoot.cpp	2020-12-03 05:32:35.923906894 +0530
@@ -282,9 +282,10 @@
 
 void RenderSVGRoot::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
 {
-    if (diff == StyleDifferenceLayout)
+    if (diff.needsFullLayout())
         setNeedsBoundariesUpdate();
-    if (diff == StyleDifferenceRepaint) {
+    // FIXME: How about other diff flags?
+    if (diff.needsRepaintObjectOnly()) {
         // Box decorations may have appeared/disappeared - recompute status.
         m_hasBoxDecorations = calculateHasBoxDecorations();
     }
