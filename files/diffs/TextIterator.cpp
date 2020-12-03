--- files/before/TextIterator.cpp	2020-12-03 05:30:36.500813208 +0530
+++ files/after/TextIterator.cpp	2020-12-03 05:30:37.480969918 +0530
@@ -775,7 +775,7 @@
             return true;
     }
     
-    return !r->isInline() && r->isRenderBlock() && !r->isFloatingOrPositioned() && !r->isBody();
+    return !r->isInline() && r->isRenderBlock() && !r->isFloatingOrOutOfFlowPositioned() && !r->isBody();
 }
 
 static bool shouldEmitNewlineAfterNode(Node* node)
