--- files/before/Str.c	2020-12-03 05:27:38.212307962 +0530
+++ files/after/Str.c	2020-12-03 05:27:39.060443563 +0530
@@ -232,8 +232,8 @@
 {
     char *old = x->ptr;
     int newlen;
-    newlen = x->length * 6 / 5;
-    if (newlen == x->length)
+    newlen = x->area_size * 6 / 5;
+    if (newlen == x->area_size)
 	newlen += 2;
     x->ptr = GC_MALLOC_ATOMIC(newlen);
     x->area_size = newlen;
