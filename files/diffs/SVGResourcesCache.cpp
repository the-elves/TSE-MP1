--- files/before/SVGResourcesCache.cpp	2020-12-03 05:32:38.968393655 +0530
+++ files/after/SVGResourcesCache.cpp	2020-12-03 05:32:39.964552923 +0530
@@ -122,11 +122,11 @@
     ASSERT(renderer->node());
     ASSERT(renderer->node()->isSVGElement());
 
-    if (diff == StyleDifferenceEqual || !renderer->parent())
+    if (diff.hasNoChange() || !renderer->parent())
         return;
 
     // In this case the proper SVGFE*Element will decide whether the modified CSS properties require a relayout or repaint.
-    if (renderer->isSVGResourceFilterPrimitive() && diff == StyleDifferenceRepaint)
+    if (renderer->isSVGResourceFilterPrimitive() && diff.needsRepaintObjectOnly())
         return;
 
     // Dynamic changes of CSS properties like 'clip-path' may require us to recompute the associated resources for a renderer.
