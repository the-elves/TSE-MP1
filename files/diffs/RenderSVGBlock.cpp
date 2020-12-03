--- files/before/RenderSVGBlock.cpp	2020-12-03 05:32:19.701313182 +0530
+++ files/after/RenderSVGBlock.cpp	2020-12-03 05:32:20.701473090 +0530
@@ -78,7 +78,7 @@
 
 void RenderSVGBlock::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
 {
-    if (diff == StyleDifferenceLayout)
+    if (diff.needsFullLayout())
         setNeedsBoundariesUpdate();
 
     RenderBlock::styleDidChange(diff, oldStyle);
