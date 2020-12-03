--- files/before/WebPage.cpp	2020-12-03 05:31:24.268450427 +0530
+++ files/after/WebPage.cpp	2020-12-03 05:31:25.280612253 +0530
@@ -2506,7 +2506,7 @@
 static bool isPositionedContainer(RenderLayer* layer)
 {
     RenderObject* o = layer->renderer();
-    return o->isRenderView() || o->isPositioned() || o->isRelPositioned() || layer->hasTransform();
+    return o->isRenderView() || o->isOutOfFlowPositioned() || o->isRelPositioned() || layer->hasTransform();
 }
 
 static bool isNonRenderViewFixedPositionedContainer(RenderLayer* layer)
@@ -2515,13 +2515,13 @@
     if (o->isRenderView())
         return false;
 
-    return o->isPositioned() && o->style()->position() == FixedPosition;
+    return o->isOutOfFlowPositioned() && o->style()->position() == FixedPosition;
 }
 
 static bool isFixedPositionedContainer(RenderLayer* layer)
 {
     RenderObject* o = layer->renderer();
-    return o->isRenderView() || (o->isPositioned() && o->style()->position() == FixedPosition);
+    return o->isRenderView() || (o->isOutOfFlowPositioned() && o->style()->position() == FixedPosition);
 }
 
 static RenderLayer* findAncestorOrSelfNotMatching(PredicateFunction predicate, RenderLayer* layer)
