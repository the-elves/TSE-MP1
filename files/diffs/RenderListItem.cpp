--- files/before/RenderListItem.cpp	2020-12-03 05:31:10.438239216 +0530
+++ files/after/RenderListItem.cpp	2020-12-03 05:31:11.602425348 +0530
@@ -189,7 +189,7 @@
         if (currChild->isInline() && (!currChild->isRenderInline() || curr->generatesLineBoxesForInlineChild(currChild)))
             return curr;
 
-        if (currChild->isFloating() || currChild->isPositioned())
+        if (currChild->isFloating() || currChild->isOutOfFlowPositioned())
             continue;
 
         if (currChild->isTable() || !currChild->isRenderBlock() || (currChild->isBox() && toRenderBox(currChild)->isWritingModeRoot()))
