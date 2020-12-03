--- files/before/RenderLayerModelObject.cpp	2020-12-03 05:31:58.229880272 +0530
+++ files/after/RenderLayerModelObject.cpp	2020-12-03 05:32:00.114181540 +0530
@@ -98,14 +98,14 @@
     if (oldStyle) {
         // Do a repaint with the old style first through RenderLayerRepainter.
         // RenderObject::styleWillChange takes care of repainting objects without RenderLayers.
-        if (parent() && diff == StyleDifferenceRepaintLayer) {
+        if (parent() && diff.needsRepaintLayer()) {
             layer()->repainter().repaintIncludingNonCompositingDescendants(containerForRepaint());
             if (oldStyle->hasClip() != newStyle.hasClip()
                 || oldStyle->clip() != newStyle.clip())
                 layer()->clipper().clearClipRectsIncludingDescendants();
         }
 
-        if (diff == StyleDifferenceLayout || diff == StyleDifferenceSimplifiedLayout) {
+        if (diff.needsFullLayout() || diff.needsSimplifiedLayout()) {
             // When a layout hint happens, we go ahead and do a repaint of the layer, since the layer could
             // end up being destroyed.
             if (hasLayer()) {
