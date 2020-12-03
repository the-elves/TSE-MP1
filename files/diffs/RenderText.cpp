--- files/before/RenderText.cpp	2020-12-03 05:32:09.031607284 +0530
+++ files/after/RenderText.cpp	2020-12-03 05:32:10.875902153 +0530
@@ -193,7 +193,7 @@
     // we already did this for the parent of the text run.
     // We do have to schedule layouts, though, since a style change can force us to
     // need to relayout.
-    if (diff == StyleDifferenceLayout) {
+    if (diff.needsFullLayout()) {
         setNeedsLayoutAndPrefWidthsRecalc();
         m_knownToHaveNoOverflowAndNoFallbackFonts = false;
     }
