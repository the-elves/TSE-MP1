--- files/before/RenderSVGResourceContainer.cpp	2020-12-03 05:32:30.767082407 +0530
+++ files/after/RenderSVGResourceContainer.cpp	2020-12-03 05:32:31.759241036 +0530
@@ -227,7 +227,10 @@
         RenderObject* renderer = (*it)->renderer();
         if (!renderer)
             continue;
-        SVGResourcesCache::clientStyleChanged(renderer, StyleDifferenceLayout, renderer->style());
+
+        StyleDifference diff;
+        diff.setNeedsFullLayout();
+        SVGResourcesCache::clientStyleChanged(renderer, diff, renderer->style());
         renderer->setNeedsLayout();
     }
 }
