--- files/before/RenderSVGResourceFilterPrimitive.cpp	2020-12-03 05:32:32.767402223 +0530
+++ files/after/RenderSVGResourceFilterPrimitive.cpp	2020-12-03 05:32:33.763561492 +0530
@@ -40,7 +40,7 @@
         return;
     ASSERT(filter->isSVGResourceFilter());
 
-    if (diff == StyleDifferenceEqual || !oldStyle)
+    if (diff.hasNoChange() || !oldStyle)
         return;
 
     const SVGRenderStyle* newStyle = this->style()->svgStyle();
