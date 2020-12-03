--- files/before/CSSComputedStyleDeclaration.cpp	2020-12-03 05:30:28.423521788 +0530
+++ files/after/CSSComputedStyleDeclaration.cpp	2020-12-03 05:30:29.479690652 +0530
@@ -597,11 +597,12 @@
         return cssValuePool().createValue(l);
     }
 
-    if (style->position() == RelativePosition)
+    if (style->position() == RelativePosition) {
         // FIXME: It's not enough to simply return "auto" values for one offset if the other side is defined.
         // In other words if left is auto and right is not auto, then left's computed value is negative right().
         // So we should get the opposite length unit and see if it is auto.
         return cssValuePool().createValue(l);
+    }
 
     return cssValuePool().createIdentifierValue(CSSValueAuto);
 }
