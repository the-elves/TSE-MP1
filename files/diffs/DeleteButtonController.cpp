--- files/before/DeleteButtonController.cpp	2020-12-03 05:30:34.512495310 +0530
+++ files/after/DeleteButtonController.cpp	2020-12-03 05:30:35.436643067 +0530
@@ -105,7 +105,7 @@
     if (node->hasTagName(ulTag) || node->hasTagName(olTag) || node->hasTagName(iframeTag))
         return true;
 
-    if (renderer->isPositioned())
+    if (renderer->isOutOfFlowPositioned())
         return true;
 
     if (renderer->isRenderBlock() && !renderer->isTableCell()) {
