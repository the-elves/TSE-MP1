--- files/before/ScreenOrientation.cpp	2020-12-03 05:31:50.496643861 +0530
+++ files/after/ScreenOrientation.cpp	2020-12-03 05:31:52.220919543 +0530
@@ -53,9 +53,14 @@
 
 static blink::WebScreenOrientations stringToOrientations(const AtomicString& orientationString)
 {
+    DEFINE_STATIC_LOCAL(const AtomicString, any, ("any", AtomicString::ConstructFromLiteral));
     DEFINE_STATIC_LOCAL(const AtomicString, portrait, ("portrait", AtomicString::ConstructFromLiteral));
     DEFINE_STATIC_LOCAL(const AtomicString, landscape, ("landscape", AtomicString::ConstructFromLiteral));
 
+    if (orientationString == any) {
+        return blink::WebScreenOrientationPortraitPrimary | blink::WebScreenOrientationPortraitSecondary |
+            blink::WebScreenOrientationLandscapePrimary | blink::WebScreenOrientationLandscapeSecondary;
+    }
     if (orientationString == portrait)
         return blink::WebScreenOrientationPortraitPrimary | blink::WebScreenOrientationPortraitSecondary;
     if (orientationString == landscape)
