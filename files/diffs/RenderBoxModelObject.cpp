--- files/before/RenderBoxModelObject.cpp	2020-12-03 05:30:50.839105655 +0530
+++ files/after/RenderBoxModelObject.cpp	2020-12-03 05:30:51.851267482 +0530
@@ -531,7 +531,7 @@
     if (const RenderBoxModelObject* offsetParent = this->offsetParent()) {
         if (offsetParent->isBox() && !offsetParent->isBody())
             referencePoint.move(-toRenderBox(offsetParent)->borderLeft(), -toRenderBox(offsetParent)->borderTop());
-        if (!isPositioned()) {
+        if (!isOutOfFlowPositioned()) {
             if (isRelPositioned())
                 referencePoint.move(relativePositionOffset());
             const RenderObject* curr = parent();
@@ -542,7 +542,7 @@
                 referencePoint.move(curr->parent()->offsetForColumns(referencePoint));
                 curr = curr->parent();
             }
-            if (offsetParent->isBox() && offsetParent->isBody() && !offsetParent->isRelPositioned() && !offsetParent->isPositioned())
+            if (offsetParent->isBox() && offsetParent->isBody() && !offsetParent->isRelPositioned() && !offsetParent->isOutOfFlowPositioned())
                 referencePoint.moveBy(toRenderBox(offsetParent)->topLeftLocation());
         }
     }
@@ -2740,7 +2740,7 @@
 
     LayoutSize containerOffset = offsetFromContainer(o, LayoutPoint());
 
-    if (!style()->isPositioned() && o->hasColumns()) {
+    if (!style()->isOutOfFlowPositioned() && o->hasColumns()) {
         RenderBlock* block = static_cast<RenderBlock*>(o);
         LayoutPoint point(roundedLayoutPoint(transformState.mappedPoint()));
         point -= containerOffset;
