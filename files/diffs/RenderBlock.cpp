--- files/before/RenderBlock.cpp	2020-12-03 05:30:44.514094393 +0530
+++ files/after/RenderBlock.cpp	2020-12-03 05:30:45.662277968 +0530
@@ -285,7 +285,7 @@
 
     setReplaced(newStyle.isDisplayInlineType());
 
-    if (oldStyle && parent() && diff == StyleDifferenceLayout && oldStyle->position() != newStyle.position()) {
+    if (oldStyle && parent() && diff.needsFullLayout() && oldStyle->position() != newStyle.position()) {
         if (newStyle.position() == StaticPosition)
             // Clear our positioned objects list. Our absolutely positioned descendants will be
             // inserted into our containing block's positioned objects list during layout.
@@ -348,7 +348,7 @@
 
     // It's possible for our border/padding to change, but for the overall logical width of the block to
     // end up being the same. We keep track of this change so in layoutBlock, we can know to set relayoutChildren=true.
-    m_hasBorderOrPaddingLogicalWidthChanged = oldStyle && diff == StyleDifferenceLayout && needsLayout() && borderOrPaddingLogicalWidthChanged(oldStyle, newStyle);
+    m_hasBorderOrPaddingLogicalWidthChanged = oldStyle && diff.needsFullLayout() && needsLayout() && borderOrPaddingLogicalWidthChanged(oldStyle, newStyle);
 
     // If the style has unloaded images, want to notify the ResourceLoadPriorityOptimizer so that
     // network priorities can be set.
