--- files/before/StyleResolver.cpp	2020-12-03 05:30:30.591868469 +0530
+++ files/after/StyleResolver.cpp	2020-12-03 05:30:31.604030297 +0530
@@ -2076,7 +2076,7 @@
 {
     return style->display() == TABLE || style->display() == INLINE_TABLE || style->display() == RUN_IN
         || style->display() == INLINE_BLOCK || style->display() == INLINE_BOX || isAtShadowBoundary(e)
-        || style->isFloating() || style->isPositioned();
+        || style->isFloating() || style->isOutOfFlowPositioned();
 }
 
 void StyleResolver::adjustRenderStyle(RenderStyle* style, RenderStyle* parentStyle, Element *e)
