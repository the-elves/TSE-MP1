--- files/before/Text.cpp	2020-12-03 05:30:32.612191486 +0530
+++ files/after/Text.cpp	2020-12-03 05:30:33.584346916 +0530
@@ -218,7 +218,7 @@
             return false;
         
         RenderObject* first = parent->firstChild();
-        while (first && first->isFloatingOrPositioned())
+        while (first && first->isFloatingOrOutOfFlowPositioned())
             first = first->nextSibling();
         RenderObject* next = context.nextRenderer();
         if (!first || next == first)
