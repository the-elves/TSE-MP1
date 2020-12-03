--- files/before/RenderStyle.cpp	2020-12-03 05:32:14.784527073 +0530
+++ files/after/RenderStyle.cpp	2020-12-03 05:32:15.916708089 +0530
@@ -368,13 +368,18 @@
 
 StyleDifference RenderStyle::visualInvalidationDiff(const RenderStyle& other, unsigned& changedContextSensitiveProperties) const
 {
+    return StyleDifference(visualInvalidationDiffLegacy(other, changedContextSensitiveProperties));
+}
+
+StyleDifferenceLegacy RenderStyle::visualInvalidationDiffLegacy(const RenderStyle& other, unsigned& changedContextSensitiveProperties) const
+{
     changedContextSensitiveProperties = ContextSensitivePropertyNone;
 
     // Note, we use .get() on each DataRef below because DataRef::operator== will do a deep
     // compare, which is duplicate work when we're going to compare each property inside
     // this function anyway.
 
-    StyleDifference svgChange = StyleDifferenceEqual;
+    StyleDifferenceLegacy svgChange = StyleDifferenceEqual;
     if (m_svgStyle.get() != other.m_svgStyle.get()) {
         svgChange = m_svgStyle->diff(other.m_svgStyle.get());
         if (svgChange == StyleDifferenceLayout)
@@ -593,12 +598,12 @@
         return StyleDifferenceLayout;
     }
 
-    StyleDifference repaintDifference = repaintOnlyDiff(other, changedContextSensitiveProperties);
+    StyleDifferenceLegacy repaintDifference = repaintOnlyDiff(other, changedContextSensitiveProperties);
     ASSERT(repaintDifference <= StyleDifferenceRepaintLayer);
     return repaintDifference;
 }
 
-StyleDifference RenderStyle::repaintOnlyDiff(const RenderStyle& other, unsigned& changedContextSensitiveProperties) const
+StyleDifferenceLegacy RenderStyle::repaintOnlyDiff(const RenderStyle& other, unsigned& changedContextSensitiveProperties) const
 {
     if (position() != StaticPosition && (m_box->zIndex() != other.m_box->zIndex() || m_box->hasAutoZIndex() != other.m_box->hasAutoZIndex()
         || visual->clip != other.visual->clip || visual->hasClip != other.visual->hasClip))
