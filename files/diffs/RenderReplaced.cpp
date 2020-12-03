--- files/before/RenderReplaced.cpp	2020-12-03 05:31:18.447519761 +0530
+++ files/after/RenderReplaced.cpp	2020-12-03 05:31:19.395671355 +0530
@@ -238,7 +238,7 @@
     // For percentage heights: The percentage is calculated with respect to the height of the generated box's
     // containing block. If the height of the containing block is not specified explicitly (i.e., it depends
     // on content height), and this element is not absolutely positioned, the value computes to 'auto'.
-    if (!logicalHeightLength.isPercent() || replaced->isPositioned() || replaced->document()->inQuirksMode())
+    if (!logicalHeightLength.isPercent() || replaced->isOutOfFlowPositioned() || replaced->document()->inQuirksMode())
         return false;
 
     for (RenderBlock* cb = replaced->containingBlock(); !cb->isRenderView(); cb = cb->containingBlock()) {
