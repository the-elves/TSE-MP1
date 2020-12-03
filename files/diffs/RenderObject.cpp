--- files/before/RenderObject.cpp	2020-12-03 05:31:14.534894200 +0530
+++ files/after/RenderObject.cpp	2020-12-03 05:31:15.567059225 +0530
@@ -1866,48 +1866,49 @@
             // We need to set at least SimplifiedLayout, but if PositionedMovementOnly is already set
             // then we actually need SimplifiedLayoutAndPositionedMovement.
             if (!hasLayer())
-                diff = StyleDifferenceLayout; // FIXME: Do this for now since SimplifiedLayout cannot handle updating floating objects lists.
-            else if (diff < StyleDifferenceLayoutPositionedMovementOnly)
-                diff = StyleDifferenceSimplifiedLayout;
-            else if (diff < StyleDifferenceSimplifiedLayout)
-                diff = StyleDifferenceSimplifiedLayoutAndPositionedMovement;
-        } else if (diff < StyleDifferenceRecompositeLayer)
-            diff = StyleDifferenceRecompositeLayer;
+                diff.setNeedsFullLayout(); // FIXME: Do this for now since SimplifiedLayout cannot handle updating floating objects lists.
+            else
+                diff.setNeedsSimplifiedLayout();
+        } else {
+            diff.setNeedsRecompositeLayer();
+        }
     }
 
     // If opacity or filters changed, and the layer does not paint into its own separate backing, then we need to repaint (also
     // ignoring text nodes)
-    if (contextSensitiveProperties & ContextSensitivePropertyOpacity && diff <= StyleDifferenceRepaintLayer) {
+    if (contextSensitiveProperties & ContextSensitivePropertyOpacity && !diff.needsLayout()) {
         if (!isText() && (!hasLayer() || !toRenderLayerModelObject(this)->layer()->hasDirectReasonsForCompositing()))
-            diff = StyleDifferenceRepaintLayer;
-        else if (diff < StyleDifferenceRecompositeLayer)
-            diff = StyleDifferenceRecompositeLayer;
+            diff.setNeedsRepaintLayer();
+        else
+            diff.setNeedsRecompositeLayer();
     }
 
-    if ((contextSensitiveProperties & ContextSensitivePropertyFilter) && hasLayer() && diff <= StyleDifferenceRepaintLayer) {
+    if ((contextSensitiveProperties & ContextSensitivePropertyFilter) && hasLayer() && !diff.needsLayout()) {
         RenderLayer* layer = toRenderLayerModelObject(this)->layer();
         if (!layer->hasDirectReasonsForCompositing() || layer->paintsWithFilters())
-            diff = StyleDifferenceRepaintLayer;
-        else if (diff < StyleDifferenceRecompositeLayer)
-            diff = StyleDifferenceRecompositeLayer;
+            diff.setNeedsRepaintLayer();
+        else
+            diff.setNeedsRecompositeLayer();
     }
 
-    if ((contextSensitiveProperties & ContextSensitivePropertyTextOrColor) && diff < StyleDifferenceRepaint
+    if ((contextSensitiveProperties & ContextSensitivePropertyTextOrColor) && !diff.needsRepaint() && !diff.needsLayout()
         && hasImmediateNonWhitespaceTextChildOrPropertiesDependentOnColor())
-        diff = StyleDifferenceRepaint;
+        diff.setNeedsRepaintObject();
 
     // The answer to layerTypeRequired() for plugins, iframes, and canvas can change without the actual
     // style changing, since it depends on whether we decide to composite these elements. When the
     // layer status of one of these elements changes, we need to force a layout.
-    if (diff == StyleDifferenceEqual && style() && isLayerModelObject()) {
+    if (diff.hasNoChange() && style() && isLayerModelObject()) {
         bool requiresLayer = toRenderLayerModelObject(this)->layerTypeRequired() != NoLayer;
         if (hasLayer() != requiresLayer)
-            diff = StyleDifferenceLayout;
+            diff.setNeedsFullLayout();
     }
 
     // If we have no layer(), just treat a RepaintLayer hint as a normal Repaint.
-    if (diff == StyleDifferenceRepaintLayer && !hasLayer())
-        diff = StyleDifferenceRepaint;
+    if (diff.needsRepaintLayer() && !hasLayer()) {
+        diff.clearNeedsRepaint();
+        diff.setNeedsRepaintObject();
+    }
 
     return diff;
 }
@@ -1959,7 +1960,7 @@
         return;
     }
 
-    StyleDifference diff = StyleDifferenceEqual;
+    StyleDifference diff;
     unsigned contextSensitiveProperties = ContextSensitivePropertyNone;
     if (m_style)
         diff = m_style->visualInvalidationDiff(*style, contextSensitiveProperties);
@@ -1994,19 +1995,18 @@
     // check whether we should layout now, and decide if we need to repaint.
     StyleDifference updatedDiff = adjustStyleDifference(diff, contextSensitiveProperties);
 
-    if (diff <= StyleDifferenceLayoutPositionedMovementOnly) {
-        if (updatedDiff == StyleDifferenceLayout)
+    if (!diff.needsFullLayout()) {
+        if (updatedDiff.needsFullLayout()) {
             setNeedsLayoutAndPrefWidthsRecalc();
-        else if (updatedDiff == StyleDifferenceLayoutPositionedMovementOnly)
-            setNeedsPositionedMovementLayout();
-        else if (updatedDiff == StyleDifferenceSimplifiedLayoutAndPositionedMovement) {
-            setNeedsPositionedMovementLayout();
-            setNeedsSimplifiedNormalFlowLayout();
-        } else if (updatedDiff == StyleDifferenceSimplifiedLayout)
-            setNeedsSimplifiedNormalFlowLayout();
+        } else {
+            if (updatedDiff.needsPositionedMovementLayout())
+                setNeedsPositionedMovementLayout();
+            if (updatedDiff.needsSimplifiedLayout())
+                setNeedsSimplifiedNormalFlowLayout();
+        }
     }
 
-    if (updatedDiff == StyleDifferenceRepaint || updatedDiff == StyleDifferenceRepaintLayer) {
+    if (updatedDiff.needsRepaint()) {
         // Do a repaint with the new style now, e.g., for example if we go from
         // not having an outline to having an outline.
         repaint();
@@ -2040,13 +2040,13 @@
                     layer->setHasVisibleContent();
                 } else if (layer->hasVisibleContent() && (this == layer->renderer() || layer->renderer()->style()->visibility() != VISIBLE)) {
                     layer->dirtyVisibleContentStatus();
-                    if (diff > StyleDifferenceRepaintLayer)
+                    if (diff.needsLayout())
                         repaint();
                 }
             }
         }
 
-        if (m_parent && diff == StyleDifferenceRepaint)
+        if (m_parent && diff.needsRepaintObjectOnly())
             repaint();
         if (isFloating() && (m_style->floating() != newStyle.floating()))
             // For changes in float styles, we need to conceivably remove ourselves
@@ -2063,12 +2063,13 @@
 
         // Clearing these bits is required to avoid leaving stale renderers.
         // FIXME: We shouldn't need that hack if our logic was totally correct.
-        if (diff == StyleDifferenceLayout || diff == StyleDifferenceLayoutPositionedMovementOnly) {
+        if (diff.needsFullLayout() || diff.needsPositionedMovementLayout()) {
             setFloating(false);
             clearPositionedState();
         }
-    } else
+    } else {
         s_affectsParentBlock = false;
+    }
 
     if (view()->frameView()) {
         bool shouldBlitOnFixedBackgroundImage = false;
@@ -2136,7 +2137,7 @@
     if (!m_parent)
         return;
 
-    if (diff == StyleDifferenceLayout || diff == StyleDifferenceSimplifiedLayout) {
+    if (diff.needsFullLayout() || diff.needsSimplifiedLayout()) {
         RenderCounter::rendererStyleChanged(*this, oldStyle, m_style.get());
 
         // If the object already needs layout, then setNeedsLayout won't do
@@ -2147,14 +2148,12 @@
         if (needsLayout() && oldStyle->position() != m_style->position())
             markContainingBlocksForLayout();
 
-        if (diff == StyleDifferenceLayout)
+        if (diff.needsFullLayout())
             setNeedsLayoutAndPrefWidthsRecalc();
         else
             setNeedsSimplifiedNormalFlowLayout();
-    } else if (diff == StyleDifferenceSimplifiedLayoutAndPositionedMovement) {
-        setNeedsPositionedMovementLayout();
-        setNeedsSimplifiedNormalFlowLayout();
-    } else if (diff == StyleDifferenceLayoutPositionedMovementOnly)
+    }
+    if (diff.needsPositionedMovementLayout())
         setNeedsPositionedMovementLayout();
 
     // Don't check for repaint here; we need to wait until the layer has been
