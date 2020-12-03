--- files/before/LayoutState.cpp	2020-12-03 05:30:42.493771378 +0530
+++ files/after/LayoutState.cpp	2020-12-03 05:30:43.417919133 +0530
@@ -45,7 +45,7 @@
 {
     ASSERT(m_next);
 
-    bool fixed = renderer->isPositioned() && renderer->style()->position() == FixedPosition;
+    bool fixed = renderer->isOutOfFlowPositioned() && renderer->style()->position() == FixedPosition;
     if (fixed) {
         // FIXME: This doesn't work correctly with transforms.
         FloatPoint fixedOffset = renderer->view()->localToAbsolute(FloatPoint(), true);
@@ -53,7 +53,7 @@
     } else
         m_paintOffset = prev->m_paintOffset + offset;
 
-    if (renderer->isPositioned() && !fixed) {
+    if (renderer->isOutOfFlowPositioned() && !fixed) {
         if (RenderObject* container = renderer->container()) {
             if (container->isRelPositioned() && container->isRenderInline())
                 m_paintOffset += toRenderInline(container)->relativePositionedInlineOffset(renderer);
