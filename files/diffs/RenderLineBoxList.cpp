--- files/before/RenderLineBoxList.cpp	2020-12-03 05:31:08.561939228 +0530
+++ files/after/RenderLineBoxList.cpp	2020-12-03 05:31:09.486086983 +0530
@@ -334,7 +334,7 @@
     RootInlineBox* box = 0;
     RenderObject* curr = 0;
     for (curr = child->previousSibling(); curr; curr = curr->previousSibling()) {
-        if (curr->isFloatingOrPositioned())
+        if (curr->isFloatingOrOutOfFlowPositioned())
             continue;
 
         if (curr->isReplaced()) {
