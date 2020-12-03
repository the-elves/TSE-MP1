--- files/before/SVGRenderStyle.cpp	2020-12-03 05:32:16.968876312 +0530
+++ files/after/SVGRenderStyle.cpp	2020-12-03 05:32:18.749160949 +0530
@@ -118,7 +118,7 @@
     resources = other->resources;
 }
 
-StyleDifference SVGRenderStyle::diff(const SVGRenderStyle* other) const
+StyleDifferenceLegacy SVGRenderStyle::diff(const SVGRenderStyle* other) const
 {
     // NOTE: All comparisions that may return StyleDifferenceLayout have to go before those who return StyleDifferenceRepaint
 
