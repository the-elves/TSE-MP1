--- files/before/RenderImage.cpp	2020-12-03 05:31:00.672677871 +0530
+++ files/after/RenderImage.cpp	2020-12-03 05:31:01.612828185 +0530
@@ -535,7 +535,7 @@
 
     // Our intrinsicSize is empty if we're rendering generated images with relative width/height. Figure out the right intrinsic size to use.
     if (intrinsicSize.isEmpty() && (m_imageResource->imageHasRelativeWidth() || m_imageResource->imageHasRelativeHeight())) {
-        RenderObject* containingBlock = isPositioned() ? container() : this->containingBlock();
+        RenderObject* containingBlock = isOutOfFlowPositioned() ? container() : this->containingBlock();
         if (containingBlock->isBox()) {
             RenderBox* box = toRenderBox(containingBlock);
             intrinsicSize.setWidth(box->availableLogicalWidth());
