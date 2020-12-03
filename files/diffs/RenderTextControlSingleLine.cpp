--- files/before/RenderTextControlSingleLine.cpp	2020-12-03 05:32:12.064092123 +0530
+++ files/after/RenderTextControlSingleLine.cpp	2020-12-03 05:32:13.780366525 +0530
@@ -241,7 +241,7 @@
         containerRenderer->style()->setWidth(Length());
     }
     RenderObject* innerTextRenderer = innerTextElement()->renderer();
-    if (innerTextRenderer && diff == StyleDifferenceLayout)
+    if (innerTextRenderer && diff.needsFullLayout())
         innerTextRenderer->setNeedsLayout();
     if (HTMLElement* placeholder = inputElement()->placeholderElement())
         placeholder->setInlineStyleProperty(CSSPropertyTextOverflow, textShouldBeTruncated() ? CSSValueEllipsis : CSSValueClip);
