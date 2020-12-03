--- files/before/RenderTable.cpp	2020-12-03 05:31:22.208121016 +0530
+++ files/after/RenderTable.cpp	2020-12-03 05:31:23.204280284 +0530
@@ -114,7 +114,7 @@
     if (!beforeChild)
         beforeChild = afterPseudoElementRenderer();
 
-    bool wrapInAnonymousSection = !child->isPositioned();
+    bool wrapInAnonymousSection = !child->isOutOfFlowPositioned();
 
     if (child->isTableCaption()) {
         m_captions.append(toRenderTableCaption(child));
@@ -213,7 +213,7 @@
 {
     recalcSectionsIfNeeded();
 
-    if (isPositioned())
+    if (isOutOfFlowPositioned())
         computePositionedLogicalWidth();
 
     RenderBlock* cb = containingBlock();
@@ -390,7 +390,7 @@
 
     setLogicalHeight(logicalHeight() + borderAndPaddingBefore);
 
-    if (!isPositioned())
+    if (!isOutOfFlowPositioned())
         computeLogicalHeight();
 
     Length logicalHeightLength = style()->logicalHeight();
@@ -439,7 +439,7 @@
         layoutCaption(m_captions[i]);
     }
 
-    if (isPositioned())
+    if (isOutOfFlowPositioned())
         computeLogicalHeight();
 
     // table can be containing block of positioned elements.
