--- files/before/RenderSVGGradientStop.cpp	2020-12-03 05:32:21.693631719 +0530
+++ files/after/RenderSVGGradientStop.cpp	2020-12-03 05:32:22.657785872 +0530
@@ -42,7 +42,7 @@
 void RenderSVGGradientStop::styleDidChange(StyleDifference diff, const RenderStyle* oldStyle)
 {
     RenderObject::styleDidChange(diff, oldStyle);
-    if (diff == StyleDifferenceEqual)
+    if (diff.hasNoChange())
         return;
 
     // <stop> elements should only be allowed to make renderers under gradient elements
