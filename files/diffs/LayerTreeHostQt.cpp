--- files/before/LayerTreeHostQt.cpp	2020-12-03 05:31:26.224763207 +0530
+++ files/after/LayerTreeHostQt.cpp	2020-12-03 05:31:27.148910964 +0530
@@ -297,7 +297,7 @@
     if (!style)
         return;
 
-    if (!renderLayer->renderer()->isPositioned() || renderLayer->renderer()->style()->position() != FixedPosition)
+    if (!renderLayer->renderer()->isOutOfFlowPositioned() || renderLayer->renderer()->style()->position() != FixedPosition)
         return;
 
     if (!renderLayer->renderer()->container()->isRenderView())
