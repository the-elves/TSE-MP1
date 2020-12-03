--- files/before/RenderLayerCompositor.cpp	2020-12-03 05:31:06.625629646 +0530
+++ files/after/RenderLayerCompositor.cpp	2020-12-03 05:31:07.597785076 +0530
@@ -1813,7 +1813,7 @@
     // position:fixed elements that create their own stacking context (e.g. have an explicit z-index,
     // opacity, transform) can get their own composited layer. A stacking context is required otherwise
     // z-index and clipping will be broken.
-    if (!(renderer->isPositioned() && renderer->style()->position() == FixedPosition && layer->isStackingContext()))
+    if (!(renderer->isOutOfFlowPositioned() && renderer->style()->position() == FixedPosition && layer->isStackingContext()))
         return false;
 
     if (Settings* settings = m_renderView->document()->settings())
