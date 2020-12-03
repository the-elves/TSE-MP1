--- files/before/RenderSVGInline.cpp	2020-12-03 05:32:23.653945140 +0530
+++ files/after/RenderSVGInline.cpp	2020-12-03 05:32:25.206193319 +0530
@@ -125,7 +125,7 @@
 
 void RenderSVGInline::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
 {
-    if (diff == StyleDifferenceLayout)
+    if (diff.needsFullLayout())
         setNeedsBoundariesUpdate();
 
     RenderInline::styleDidChange(diff, oldStyle);
