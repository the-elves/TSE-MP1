--- files/before/RenderRubyRun.cpp	2020-12-03 05:31:20.323819749 +0530
+++ files/after/RenderRubyRun.cpp	2020-12-03 05:31:21.263970062 +0530
@@ -76,7 +76,7 @@
     RenderObject* child = firstChild();
     // If in future it becomes necessary to support floating or positioned ruby text,
     // layout will have to be changed to handle them properly.
-    ASSERT(!child || !child->isRubyText() || !child->isFloatingOrPositioned());
+    ASSERT(!child || !child->isRubyText() || !child->isFloatingOrOutOfFlowPositioned());
     return child && child->isRubyText() ? static_cast<RenderRubyText*>(child) : 0;
 }
 
