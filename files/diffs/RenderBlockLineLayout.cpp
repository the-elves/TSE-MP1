--- files/before/RenderBlockLineLayout.cpp	2020-12-03 05:30:46.706444912 +0530
+++ files/after/RenderBlockLineLayout.cpp	2020-12-03 05:30:47.746611216 +0530
@@ -617,7 +617,7 @@
     int endOverhang;
     RenderObject* nextObject = 0;
     for (BidiRun* runWithNextObject = run->next(); runWithNextObject; runWithNextObject = runWithNextObject->next()) {
-        if (!runWithNextObject->m_object->isPositioned() && !runWithNextObject->m_box->isLineBreak()) {
+        if (!runWithNextObject->m_object->isOutOfFlowPositioned() && !runWithNextObject->m_box->isLineBreak()) {
             nextObject = runWithNextObject->m_object;
             break;
         }
@@ -762,7 +762,7 @@
     RenderObject* previousObject = 0;
 
     for (BidiRun* r = firstRun; r; r = r->next()) {
-        if (!r->m_box || r->m_object->isPositioned() || r->m_box->isLineBreak())
+        if (!r->m_box || r->m_object->isOutOfFlowPositioned() || r->m_box->isLineBreak())
             continue; // Positioned objects are only participating to figure out their
                       // correct static x position.  They have no effect on the width.
                       // Similarly, line break boxes have no effect on the width.
@@ -826,7 +826,7 @@
 
         // Align positioned boxes with the top of the line box.  This is
         // a reasonable approximation of an appropriate y position.
-        if (r->m_object->isPositioned())
+        if (r->m_object->isOutOfFlowPositioned())
             r->m_box->setLogicalTop(logicalHeight());
 
         // Position is used to properly position both replaced elements and
@@ -1478,7 +1478,7 @@
             if (!hasInlineChild && o->isInline())
                 hasInlineChild = true;
 
-            if (o->isReplaced() || o->isFloating() || o->isPositioned()) {
+            if (o->isReplaced() || o->isFloating() || o->isOutOfFlowPositioned()) {
                 RenderBox* box = toRenderBox(o);
 
                 if (relayoutChildren || box->hasRelativeDimensions())
@@ -1488,7 +1488,7 @@
                 if (relayoutChildren && box->needsPreferredWidthsRecalculation())
                     o->setPreferredLogicalWidthsDirty(true, MarkOnlyThis);
 
-                if (o->isPositioned())
+                if (o->isOutOfFlowPositioned())
                     o->containingBlock()->insertPositionedObject(box);
                 else if (o->isFloating())
                     layoutState.floats().append(FloatWithRect(box));
@@ -1849,7 +1849,7 @@
 
 static bool requiresLineBox(const InlineIterator& it, const LineInfo& lineInfo = LineInfo(), WhitespacePosition whitespacePosition = LeadingWhitespace)
 {
-    if (it.m_obj->isFloatingOrPositioned())
+    if (it.m_obj->isFloatingOrOutOfFlowPositioned())
         return false;
 
     if (it.m_obj->isRenderInline() && !alwaysRequiresLineBox(toRenderInline(it.m_obj)) && !requiresLineBoxForContent(toRenderInline(it.m_obj), lineInfo))
@@ -1885,7 +1885,7 @@
 {
     while (!iterator.atEnd() && !requiresLineBox(iterator, lineInfo, TrailingWhitespace)) {
         RenderObject* object = iterator.m_obj;
-        if (object->isPositioned())
+        if (object->isOutOfFlowPositioned())
             setStaticPositions(m_block, toRenderBox(object));
         else if (object->isFloating())
             m_block->insertFloatingObject(toRenderBox(object));
@@ -1898,7 +1898,7 @@
 {
     while (!resolver.position().atEnd() && !requiresLineBox(resolver.position(), lineInfo, LeadingWhitespace)) {
         RenderObject* object = resolver.position().m_obj;
-        if (object->isPositioned()) {
+        if (object->isOutOfFlowPositioned()) {
             setStaticPositions(m_block, toRenderBox(object));
             if (object->style()->isOriginalDisplayInlineType()) {
                 resolver.runs().addRun(createRun(0, 1, object, resolver));
@@ -2192,7 +2192,7 @@
             goto end;
         }
 
-        if (current.m_obj->isPositioned()) {
+        if (current.m_obj->isOutOfFlowPositioned()) {
             // If our original display wasn't an inline type, then we can
             // go ahead and determine our static inline position now.
             RenderBox* box = toRenderBox(current.m_obj);
@@ -2599,7 +2599,7 @@
                 goto end;
         }
 
-        if (!current.m_obj->isFloatingOrPositioned()) {
+        if (!current.m_obj->isFloatingOrOutOfFlowPositioned()) {
             last = current.m_obj;
             if (last->isReplaced() && autoWrap && (!last->isImage() || allowImagesToBreak) && (!last->isListMarker() || toRenderListMarker(last)->isInside())) {
                 width.commit();
