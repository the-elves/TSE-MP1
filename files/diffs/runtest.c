--- files/before/runtest.c	2020-12-03 05:33:15.974310262 +0530
+++ files/after/runtest.c	2020-12-03 05:33:17.038480404 +0530
@@ -1854,6 +1854,7 @@
     const char *base;
     int size, res;
     int cur = 0;
+    int chunkSize = 4;
 
     nb_tests++;
     /*
@@ -1864,17 +1865,21 @@
 	return(-1);
     }
 
+    if (chunkSize > size)
+        chunkSize = size;
+
 #ifdef LIBXML_HTML_ENABLED
     if (options & XML_PARSE_HTML)
-	ctxt = htmlCreatePushParserCtxt(NULL, NULL, base + cur, 4, filename,
+	ctxt = htmlCreatePushParserCtxt(NULL, NULL, base + cur, chunkSize, filename,
 	                                XML_CHAR_ENCODING_NONE);
     else
 #endif
-    ctxt = xmlCreatePushParserCtxt(NULL, NULL, base + cur, 4, filename);
+    ctxt = xmlCreatePushParserCtxt(NULL, NULL, base + cur, chunkSize, filename);
     xmlCtxtUseOptions(ctxt, options);
-    cur += 4;
+    cur += chunkSize;
+    chunkSize = 1024;
     do {
-        if (cur + 1024 >= size) {
+        if (cur + chunkSize >= size) {
 #ifdef LIBXML_HTML_ENABLED
 	    if (options & XML_PARSE_HTML)
 		htmlParseChunk(ctxt, base + cur, size - cur, 1);
@@ -1885,11 +1890,11 @@
 	} else {
 #ifdef LIBXML_HTML_ENABLED
 	    if (options & XML_PARSE_HTML)
-		htmlParseChunk(ctxt, base + cur, 1024, 0);
+		htmlParseChunk(ctxt, base + cur, chunkSize, 0);
 	    else
 #endif
-	    xmlParseChunk(ctxt, base + cur, 1024, 0);
-	    cur += 1024;
+	    xmlParseChunk(ctxt, base + cur, chunkSize, 0);
+	    cur += chunkSize;
 	}
     } while (cur < size);
     doc = ctxt->myDoc;
@@ -4214,6 +4219,9 @@
     { "Error cases regression tests",
       errParseTest, "./test/errors/*.xml", "result/errors/", "", ".err",
       0 },
+    { "Error cases regression tests (old 1.0)",
+      errParseTest, "./test/errors10/*.xml", "result/errors10/", "", ".err",
+      XML_PARSE_OLD10 },
 #ifdef LIBXML_READER_ENABLED
     { "Error cases stream regression tests",
       streamParseTest, "./test/errors/*.xml", "result/errors/", NULL, ".str",
