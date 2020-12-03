--- files/before/SVGInlineTextBox.cpp	2020-12-03 05:32:36.944070000 +0530
+++ files/after/SVGInlineTextBox.cpp	2020-12-03 05:32:37.956231828 +0530
@@ -681,13 +681,19 @@
         paintTextWithShadows(context, style, textRun, fragment, 0, startPosition, resourceMode);
 
     // Draw text using selection style from the start to the end position of the selection
-    if (style != selectionStyle)
-        SVGResourcesCache::clientStyleChanged(&parent()->renderer(), StyleDifferenceRepaint, selectionStyle);
+    if (style != selectionStyle) {
+        StyleDifference diff;
+        diff.setNeedsRepaintObject();
+        SVGResourcesCache::clientStyleChanged(&parent()->renderer(), diff, selectionStyle);
+    }
 
     paintTextWithShadows(context, selectionStyle, textRun, fragment, startPosition, endPosition, resourceMode);
 
-    if (style != selectionStyle)
-        SVGResourcesCache::clientStyleChanged(&parent()->renderer(), StyleDifferenceRepaint, style);
+    if (style != selectionStyle) {
+        StyleDifference diff;
+        diff.setNeedsRepaintObject();
+        SVGResourcesCache::clientStyleChanged(&parent()->renderer(), diff, selectionStyle);
+    }
 
     // Eventually draw text using regular style from the end position of the selection to the end of the current chunk part
     if (endPosition < static_cast<int>(fragment.length) && !paintSelectedTextOnly)
