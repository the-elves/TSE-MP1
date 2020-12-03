--- files/before/RenderSVGInlineText.cpp	2020-12-03 05:32:26.174348109 +0530
+++ files/after/RenderSVGInlineText.cpp	2020-12-03 05:32:27.246519530 +0530
@@ -89,7 +89,7 @@
         return;
     }
 
-    if (diff != StyleDifferenceLayout)
+    if (!diff.needsFullLayout())
         return;
 
     // The text metrics may be influenced by style changes.
