--- files/before/InlineFlowBox.cpp	2020-12-03 05:30:40.377433012 +0530
+++ files/after/InlineFlowBox.cpp	2020-12-03 05:30:41.553621065 +0530
@@ -119,7 +119,7 @@
             setHasTextDescendantsOnAncestors(this);
     }
 
-    if (descendantsHaveSameLineHeightAndBaseline() && !child->renderer()->isPositioned()) {
+    if (descendantsHaveSameLineHeightAndBaseline() && !child->renderer()->isOutOfFlowPositioned()) {
         RenderStyle* parentStyle = renderer()->style(isFirstLineStyle());
         RenderStyle* childStyle = child->renderer()->style(isFirstLineStyle());
         bool shouldClearDescendantsHaveSameLineHeightAndBaseline = false;
@@ -156,7 +156,7 @@
             clearDescendantsHaveSameLineHeightAndBaseline();
     }
 
-    if (!child->renderer()->isPositioned()) {
+    if (!child->renderer()->isOutOfFlowPositioned()) {
         if (child->isText()) {
             RenderStyle* childStyle = child->renderer()->style(isFirstLineStyle());
             if (childStyle->letterSpacing() < 0 || childStyle->textShadow() || childStyle->textEmphasisMark() != TextEmphasisMarkNone || childStyle->textStrokeWidth())
@@ -391,7 +391,7 @@
             if (knownToHaveNoOverflow())
                 maxLogicalRight = max(logicalLeft, maxLogicalRight);
         } else {
-            if (curr->renderer()->isPositioned()) {
+            if (curr->renderer()->isOutOfFlowPositioned()) {
                 if (curr->renderer()->parent()->style()->isLeftToRightDirection())
                     curr->setLogicalLeft(logicalLeft);
                 else
@@ -445,7 +445,7 @@
         return true;
 
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
         
         if (curr->isInlineFlowBox()) {
@@ -478,7 +478,7 @@
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
         // The computed lineheight needs to be extended for the
         // positioned elements
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
         if (curr->verticalAlign() == TOP || curr->verticalAlign() == BOTTOM) {
             LayoutUnit lineHeight = curr->lineHeight();
@@ -545,7 +545,7 @@
         return;
 
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
         
         InlineFlowBox* inlineFlowBox = curr->isInlineFlowBox() ? toInlineFlowBox(curr) : 0;
@@ -616,7 +616,7 @@
     }
 
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
 
         if (descendantsHaveSameLineHeightAndBaseline()) {
@@ -736,7 +736,7 @@
     setLogicalTop(lineBottom - (logicalTop() - lineTop) - logicalHeight());
     
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders aren't affected here.
         
         if (curr->isInlineFlowBox())
@@ -912,7 +912,7 @@
     addBorderOutsetVisualOverflow(logicalVisualOverflow);
 
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
         
         if (curr->renderer()->isText()) {
@@ -1429,7 +1429,7 @@
 {
     LayoutUnit result = 0;
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
         
         if (curr->isInlineFlowBox())
@@ -1477,7 +1477,7 @@
 {
     LayoutUnit result = 0;
     for (InlineBox* curr = firstChild(); curr; curr = curr->nextOnLine()) {
-        if (curr->renderer()->isPositioned())
+        if (curr->renderer()->isOutOfFlowPositioned())
             continue; // Positioned placeholders don't affect calculations.
 
         if (curr->isInlineFlowBox())
