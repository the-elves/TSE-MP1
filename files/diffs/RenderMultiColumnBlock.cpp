--- files/before/RenderMultiColumnBlock.cpp	2020-12-03 05:31:12.554577582 +0530
+++ files/after/RenderMultiColumnBlock.cpp	2020-12-03 05:31:13.502729174 +0530
@@ -152,7 +152,7 @@
 {    
     if (isFloating())
         return "RenderMultiColumnBlock (floating)";
-    if (isPositioned())
+    if (isOutOfFlowPositioned())
         return "RenderMultiColumnBlock (positioned)";
     if (isAnonymousBlock())
         return "RenderMultiColumnBlock (anonymous)";
