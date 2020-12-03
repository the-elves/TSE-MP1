--- files/before/RenderFieldset.cpp	2020-12-03 05:30:54.731728016 +0530
+++ files/after/RenderFieldset.cpp	2020-12-03 05:30:55.787896879 +0530
@@ -126,7 +126,7 @@
 RenderBox* RenderFieldset::findLegend() const
 {
     for (RenderObject* legend = firstChild(); legend; legend = legend->nextSibling()) {
-        if (!legend->isFloatingOrPositioned() && legend->node() && (legend->node()->hasTagName(legendTag)))
+        if (!legend->isFloatingOrOutOfFlowPositioned() && legend->node() && (legend->node()->hasTagName(legendTag)))
             return toRenderBox(legend);
     }
     return 0;
