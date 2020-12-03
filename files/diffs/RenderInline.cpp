--- files/before/RenderInline.cpp	2020-12-03 05:31:02.592984898 +0530
+++ files/after/RenderInline.cpp	2020-12-03 05:31:03.557139048 +0530
@@ -302,7 +302,7 @@
     if (!beforeChild && isAfterContent(lastChild()))
         beforeChild = lastChild();
 
-    if (!newChild->isInline() && !newChild->isFloatingOrPositioned()) {
+    if (!newChild->isInline() && !newChild->isFloatingOrOutOfFlowPositioned()) {
         // We are placing a block inside an inline. We have to perform a split of this
         // inline into continuations.  This involves creating an anonymous block box to hold
         // |newChild|.  We then make that block box a continuation of this inline.  We take all of
@@ -504,7 +504,7 @@
             beforeChildParent = flow;
     }
 
-    if (newChild->isFloatingOrPositioned())
+    if (newChild->isFloatingOrOutOfFlowPositioned())
         return beforeChildParent->addChildIgnoringContinuation(newChild, beforeChild);
 
     // A continuation always consists of two potential candidates: an inline or an anonymous
@@ -555,7 +555,7 @@
     bool isHorizontal = style()->isHorizontalWritingMode();
 
     for (RenderObject* curr = firstChild(); curr; curr = curr->nextSibling()) {
-        if (curr->isFloatingOrPositioned())
+        if (curr->isFloatingOrOutOfFlowPositioned())
             continue;
             
         // We want to get the margin box in the inline direction, and then use our font ascent/descent in the block
@@ -837,7 +837,7 @@
 InlineBox* RenderInline::culledInlineFirstLineBox() const
 {
     for (RenderObject* curr = firstChild(); curr; curr = curr->nextSibling()) {
-        if (curr->isFloatingOrPositioned())
+        if (curr->isFloatingOrOutOfFlowPositioned())
             continue;
             
         // We want to get the margin box in the inline direction, and then use our font ascent/descent in the block
@@ -861,7 +861,7 @@
 InlineBox* RenderInline::culledInlineLastLineBox() const
 {
     for (RenderObject* curr = lastChild(); curr; curr = curr->previousSibling()) {
-        if (curr->isFloatingOrPositioned())
+        if (curr->isFloatingOrOutOfFlowPositioned())
             continue;
             
         // We want to get the margin box in the inline direction, and then use our font ascent/descent in the block
@@ -889,7 +889,7 @@
     LayoutRect result(enclosingLayoutRect(floatResult));
     bool isHorizontal = style()->isHorizontalWritingMode();
     for (RenderObject* curr = firstChild(); curr; curr = curr->nextSibling()) {
-        if (curr->isFloatingOrPositioned())
+        if (curr->isFloatingOrOutOfFlowPositioned())
             continue;
             
         // For overflow we just have to propagate by hand and recompute it all.
@@ -1044,7 +1044,7 @@
 
     LayoutPoint topLeft = rect.location();
 
-    if (o->isBlockFlow() && !style()->isPositioned()) {
+    if (o->isBlockFlow() && !style()->isOutOfFlowPositioned()) {
         RenderBlock* cb = toRenderBlock(o);
         if (cb->hasColumns()) {
             LayoutRect repaintRect(topLeft, rect.size());
@@ -1243,7 +1243,7 @@
     if (!alwaysCreateLineBoxes()) {
         // We have to grovel into our children in order to dirty the appropriate lines.
         for (RenderObject* curr = firstChild(); curr; curr = curr->nextSibling()) {
-            if (curr->isFloatingOrPositioned())
+            if (curr->isFloatingOrOutOfFlowPositioned())
                 continue;
             if (curr->isBox() && !curr->needsLayout()) {
                 RenderBox* currBox = toRenderBox(curr);
