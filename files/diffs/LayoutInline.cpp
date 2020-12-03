--- files/before/LayoutInline.cpp	2020-12-03 05:33:00.627856629 +0530
+++ files/after/LayoutInline.cpp	2020-12-03 05:33:01.660021658 +0530
@@ -1140,11 +1140,17 @@
   for (LayoutBlock* currBlock = containingBlock();
        currBlock && currBlock->isAnonymousBlock();
        currBlock = toLayoutBlock(currBlock->nextSibling())) {
+    bool walkChildrenOnly = !currBlock->childrenInline();
     for (LayoutObject* curr = currBlock->firstChild(); curr;
          curr = curr->nextSibling()) {
       LayoutRect rect(curr->localVisualRect());
       context(FloatRect(rect));
-      if (curr == endContinuation) {
+      if (walkChildrenOnly)
+        continue;
+      for (LayoutObject* walker = curr; walker;
+           walker = walker->nextInPreOrder(curr)) {
+        if (walker != endContinuation)
+          continue;
         LayoutRect rect(enclosingIntRect(floatResult));
         mapToVisualRectInAncestorSpace(view(), rect);
         return rect;
