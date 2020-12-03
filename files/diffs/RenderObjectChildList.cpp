--- files/before/RenderObjectChildList.cpp	2020-12-03 05:31:16.527212737 +0530
+++ files/after/RenderObjectChildList.cpp	2020-12-03 05:31:17.471363690 +0530
@@ -75,7 +75,7 @@
 {
     ASSERT(oldChild->parent() == owner);
 
-    if (oldChild->isFloatingOrPositioned())
+    if (oldChild->isFloatingOrOutOfFlowPositioned())
         toRenderBox(oldChild)->removeFloatingOrPositionedChildFromBlockLists();
 
     // So that we'll get the appropriate dirty bit set (either that a normal flow child got yanked or
@@ -111,7 +111,7 @@
         if (oldChild->isListItem())
             toRenderListItem(oldChild)->updateListMarkerNumbers();
 
-        if (oldChild->isPositioned() && owner->childrenInline())
+        if (oldChild->isOutOfFlowPositioned() && owner->childrenInline())
             owner->dirtyLinesFromChangedChild(oldChild);
 
         if (oldChild->isRenderRegion())
