--- files/before/RenderSVGModelObject.cpp	2020-12-03 05:32:28.234677520 +0530
+++ files/after/RenderSVGModelObject.cpp	2020-12-03 05:32:29.750919941 +0530
@@ -100,7 +100,7 @@
 
 void RenderSVGModelObject::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
 {
-    if (diff == StyleDifferenceLayout) {
+    if (diff.needsFullLayout()) {
         setNeedsBoundariesUpdate();
         if (style()->hasTransform())
             setNeedsTransformUpdate();
