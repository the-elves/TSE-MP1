--- files/before/AutoTableLayout.cpp	2020-12-03 05:30:38.417119592 +0530
+++ files/after/AutoTableLayout.cpp	2020-12-03 05:30:39.389275024 +0530
@@ -186,10 +186,10 @@
     bool scale = true;
     while (table) {
         Length tw = table->style()->width();
-        if ((tw.isAuto() || tw.isPercent()) && !table->isPositioned()) {
+        if ((tw.isAuto() || tw.isPercent()) && !table->isOutOfFlowPositioned()) {
             RenderBlock* cb = table->containingBlock();
             while (cb && !cb->isRenderView() && !cb->isTableCell() &&
-                cb->style()->width().isAuto() && !cb->isPositioned())
+                cb->style()->width().isAuto() && !cb->isOutOfFlowPositioned())
                 cb = cb->containingBlock();
 
             table = 0;
