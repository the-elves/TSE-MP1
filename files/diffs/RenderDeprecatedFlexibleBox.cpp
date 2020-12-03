--- files/before/RenderDeprecatedFlexibleBox.cpp	2020-12-03 05:30:52.791417794 +0530
+++ files/after/RenderDeprecatedFlexibleBox.cpp	2020-12-03 05:30:53.739569387 +0530
@@ -147,7 +147,7 @@
 static bool childDoesNotAffectWidthOrFlexing(RenderObject* child)
 {
     // Positioned children and collapsed children don't affect the min/max width.
-    return child->isPositioned() || child->style()->visibility() == COLLAPSE;
+    return child->isOutOfFlowPositioned() || child->style()->visibility() == COLLAPSE;
 }
 
 void RenderDeprecatedFlexibleBox::styleWillChange(StyleDifference diff, const RenderStyle* newStyle)
@@ -285,7 +285,7 @@
 
     computeRegionRangeForBlock();
 
-    if (!isFloatingOrPositioned() && height() == 0) {
+    if (!isFloatingOrOutOfFlowPositioned() && height() == 0) {
         // We are a block with no border and padding and a computed height
         // of 0.  The CSS spec states that zero-height blocks collapse their margins
         // together.
@@ -382,7 +382,7 @@
             if (relayoutChildren || (child->isReplaced() && (child->style()->width().isPercent() || child->style()->height().isPercent())))
                 child->setChildNeedsLayout(true, MarkOnlyThis);
 
-            if (child->isPositioned())
+            if (child->isOutOfFlowPositioned())
                 continue;
 
             // Compute the child's vertical margins.
@@ -430,7 +430,7 @@
         // Now that our height is actually known, we can place our boxes.
         m_stretchingChildren = (style()->boxAlign() == BSTRETCH);
         for (RenderBox* child = iterator.first(); child; child = iterator.next()) {
-            if (child->isPositioned()) {
+            if (child->isOutOfFlowPositioned()) {
                 child->containingBlock()->insertPositionedObject(child);
                 RenderLayer* childLayer = child->layer();
                 childLayer->setStaticInlinePosition(xPos); // FIXME: Not right for regions.
@@ -440,7 +440,9 @@
                         child->setChildNeedsLayout(true, MarkOnlyThis);
                 }
                 continue;
-            } else if (child->style()->visibility() == COLLAPSE) {
+            }
+            
+            if (child->style()->visibility() == COLLAPSE) {
                 // visibility: collapsed children do not participate in our positioning.
                 // But we need to lay them down.
                 child->layoutIfNeeded();
@@ -671,7 +673,7 @@
             if (!haveLineClamp && (relayoutChildren || (child->isReplaced() && (child->style()->width().isPercent() || child->style()->height().isPercent()))))
                 child->setChildNeedsLayout(true, MarkOnlyThis);
 
-            if (child->isPositioned()) {
+            if (child->isOutOfFlowPositioned()) {
                 child->containingBlock()->insertPositionedObject(child);
                 RenderLayer* childLayer = child->layer();
                 childLayer->setStaticInlinePosition(borderStart() + paddingStart()); // FIXME: Not right for regions.
@@ -681,7 +683,9 @@
                         child->setChildNeedsLayout(true, MarkOnlyThis);
                 }
                 continue;
-            } else if (child->style()->visibility() == COLLAPSE) {
+            }
+            
+            if (child->style()->visibility() == COLLAPSE) {
                 // visibility: collapsed children do not participate in our positioning.
                 // But we need to lay them down.
                 child->layoutIfNeeded();
@@ -1078,7 +1082,7 @@
 {
     if (isFloating())
         return "RenderDeprecatedFlexibleBox (floating)";
-    if (isPositioned())
+    if (isOutOfFlowPositioned())
         return "RenderDeprecatedFlexibleBox (positioned)";
     if (isAnonymous())
         return "RenderDeprecatedFlexibleBox (generated)";
