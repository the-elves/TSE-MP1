--- files/before/RenderBlockFlow.cpp	2020-12-03 05:31:56.001523996 +0530
+++ files/after/RenderBlockFlow.cpp	2020-12-03 05:31:57.133705013 +0530
@@ -1814,7 +1814,7 @@
 {
     RenderStyle* oldStyle = style();
     s_canPropagateFloatIntoSibling = oldStyle ? !isFloatingOrOutOfFlowPositioned() && !avoidsFloats() : false;
-    if (oldStyle && parent() && diff == StyleDifferenceLayout && oldStyle->position() != newStyle.position()
+    if (oldStyle && parent() && diff.needsFullLayout() && oldStyle->position() != newStyle.position()
         && containsFloats() && !isFloating() && !isOutOfFlowPositioned() && newStyle.hasOutOfFlowPosition())
             markAllDescendantsWithFloatsForLayout();
 
@@ -1830,7 +1830,7 @@
     // then mark its descendants with floats for layout and clear all floats from its next
     // sibling blocks that exist in our floating objects list. See bug 56299 and 62875.
     bool canPropagateFloatIntoSibling = !isFloatingOrOutOfFlowPositioned() && !avoidsFloats();
-    if (diff == StyleDifferenceLayout && s_canPropagateFloatIntoSibling && !canPropagateFloatIntoSibling && hasOverhangingFloats()) {
+    if (diff.needsFullLayout() && s_canPropagateFloatIntoSibling && !canPropagateFloatIntoSibling && hasOverhangingFloats()) {
         RenderBlockFlow* parentBlockFlow = this;
         const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
         FloatingObjectSetIterator end = floatingObjectSet.end();
@@ -1855,7 +1855,7 @@
         parentBlockFlow->markSiblingsWithFloatsForLayout();
     }
 
-    if (diff == StyleDifferenceLayout || !oldStyle)
+    if (diff.needsFullLayout() || !oldStyle)
         createOrDestroyMultiColumnFlowThreadIfNeeded();
 }
 
