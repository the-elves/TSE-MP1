--- files/before/RenderBox.cpp	2020-12-03 05:30:48.798779444 +0530
+++ files/after/RenderBox.cpp	2020-12-03 05:30:49.842946388 +0530
@@ -143,8 +143,8 @@
     if (oldStyle) {
         // The background of the root element or the body element could propagate up to
         // the canvas.  Just dirty the entire canvas when our style changes substantially.
-        if (diff >= StyleDifferenceRepaint && node() &&
-            (isHTMLHtmlElement(*node()) || isHTMLBodyElement(*node()))) {
+        if ((diff.needsRepaint() || diff.needsLayout()) && node()
+            && (isHTMLHtmlElement(*node()) || isHTMLBodyElement(*node()))) {
             view()->repaint();
 
             if (oldStyle->hasEntirelyFixedBackground() != newStyle.hasEntirelyFixedBackground())
@@ -153,7 +153,7 @@
 
         // When a layout hint happens and an object's position style changes, we have to do a layout
         // to dirty the render tree using the old position value now.
-        if (diff == StyleDifferenceLayout && parent() && oldStyle->position() != newStyle.position()) {
+        if (diff.needsFullLayout() && parent() && oldStyle->position() != newStyle.position()) {
             markContainingBlocksForLayout();
             if (oldStyle->position() == StaticPosition)
                 repaint();
@@ -209,7 +209,7 @@
     }
 
     // Our opaqueness might have changed without triggering layout.
-    if (diff == StyleDifferenceRepaint || diff == StyleDifferenceRepaintLayer) {
+    if (diff.needsRepaint()) {
         RenderObject* parentToInvalidate = parent();
         for (unsigned i = 0; i < backgroundObscurationTestMaxDepth && parentToInvalidate; ++i) {
             parentToInvalidate->invalidateBackgroundObscurationStatus();
