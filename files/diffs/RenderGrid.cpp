--- files/before/RenderGrid.cpp	2020-12-03 05:30:58.736368290 +0530
+++ files/after/RenderGrid.cpp	2020-12-03 05:30:59.664516684 +0530
@@ -50,7 +50,7 @@
 {
     if (isFloating())
         return "RenderGrid (floating)";
-    if (isPositioned())
+    if (isOutOfFlowPositioned())
         return "RenderGrid (positioned)";
     if (isAnonymous())
         return "RenderGrid (generated)";
