--- files/before/buf.c	2020-12-03 05:33:05.572647215 +0530
+++ files/after/buf.c	2020-12-03 05:33:06.672823114 +0530
@@ -231,7 +231,7 @@
 xmlBufCreateStatic(void *mem, size_t size) {
     xmlBufPtr ret;
 
-    if ((mem == NULL) || (size == 0))
+    if (mem == NULL)
         return(NULL);
 
     ret = (xmlBufPtr) xmlMalloc(sizeof(xmlBuf));
