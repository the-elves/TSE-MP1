--- files/before/RenderScrollbarPart.cpp	2020-12-03 05:32:01.874462977 +0530
+++ files/after/RenderScrollbarPart.cpp	2020-12-03 05:32:03.758764246 +0530
@@ -151,7 +151,7 @@
     clearPositionedState();
     setFloating(false);
     setHasOverflowClip(false);
-    if (oldStyle && m_scrollbar && m_part != NoPart && diff >= StyleDifferenceRepaint)
+    if (oldStyle && m_scrollbar && m_part != NoPart && (diff.needsRepaint() || diff.needsLayout()))
         m_scrollbar->theme()->invalidatePart(m_scrollbar, m_part);
 }
 
