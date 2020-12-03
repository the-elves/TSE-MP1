--- files/before/RenderTableRow.cpp	2020-12-03 05:32:05.491041212 +0530
+++ files/after/RenderTableRow.cpp	2020-12-03 05:32:07.191313054 +0530
@@ -79,7 +79,7 @@
         if (table && !table->selfNeedsLayout() && !table->normalChildNeedsLayout() && oldStyle && oldStyle->border() != style()->border())
             table->invalidateCollapsedBorders();
 
-        if (table && oldStyle && diff == StyleDifferenceLayout && needsLayout() && table->collapseBorders() && borderWidthChanged(oldStyle, style())) {
+        if (table && oldStyle && diff.needsFullLayout() && needsLayout() && table->collapseBorders() && borderWidthChanged(oldStyle, style())) {
             // If the border width changes on a row, we need to make sure the cells in the row know to lay out again.
             // This only happens when borders are collapsed, since they end up affecting the border sides of the cell
             // itself.
