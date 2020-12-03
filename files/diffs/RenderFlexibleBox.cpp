--- files/before/RenderFlexibleBox.cpp	2020-12-03 05:30:56.752051034 +0530
+++ files/after/RenderFlexibleBox.cpp	2020-12-03 05:30:57.792217337 +0530
@@ -213,7 +213,7 @@
 {
     RenderBlock::styleDidChange(diff, oldStyle);
 
-    if (oldStyle && oldStyle->alignItems() == ItemPositionStretch && diff == StyleDifferenceLayout) {
+    if (oldStyle && oldStyle->alignItems() == ItemPositionStretch && diff.needsFullLayout()) {
         // Flex items that were previously stretching need to be relayed out so we can compute new available cross axis space.
         // This is only necessary for stretching since other alignment values don't change the size of the box.
         for (RenderBox* child = firstChildBox(); child; child = child->nextSiblingBox()) {
