--- files/before/RenderLayer.cpp	2020-12-03 05:31:04.545297037 +0530
+++ files/after/RenderLayer.cpp	2020-12-03 05:31:05.633471016 +0530
@@ -3797,7 +3797,7 @@
         m_scrollableArea->updateAfterStyleChange(oldStyle);
 
     if (!oldStyle || oldStyle->visibility() != renderer()->style()->visibility()) {
-        ASSERT(!oldStyle || diff >= StyleDifferenceRepaint);
+        ASSERT(!oldStyle || diff.needsRepaint() || diff.needsLayout());
         compositor()->setNeedsUpdateCompositingRequirementsState();
     }
 
@@ -3806,12 +3806,12 @@
     updateSelfPaintingLayer();
 
     if (!oldStyle || renderer()->style()->position() != oldStyle->position()) {
-        ASSERT(!oldStyle || diff >= StyleDifferenceLayout);
+        ASSERT(!oldStyle || diff.needsFullLayout());
         updateOutOfFlowPositioned(oldStyle);
     }
 
     if (!oldStyle || !renderer()->style()->reflectionDataEquivalent(oldStyle)) {
-        ASSERT(!oldStyle || diff >= StyleDifferenceLayout);
+        ASSERT(!oldStyle || diff.needsFullLayout());
         updateReflectionInfo(oldStyle);
     }
 
