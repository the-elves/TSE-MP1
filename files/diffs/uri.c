--- files/before/uri.c	2020-12-03 05:33:23.111451368 +0530
+++ files/after/uri.c	2020-12-03 05:33:24.095608718 +0530
@@ -12,7 +12,6 @@
 #include "libxml.h"
 
 #include <string.h>
-#include <limits.h>
 
 #include <libxml/xmlmemory.h>
 #include <libxml/uri.h>
@@ -2164,7 +2163,6 @@
     xmlChar *val = NULL;
     int ret;
     int ix;
-    int pos = 0;
     int nbslash = 0;
     int len;
     xmlURIPtr ref = NULL;
@@ -2255,19 +2253,22 @@
 	uptr = NULL;
 	len = 1;	/* this is for a string terminator only */
     } else {
-    /*
-     * Next we compare the two strings and find where they first differ
-     */
-	if ((ref->path[pos] == '.') && (ref->path[pos+1] == '/'))
-            pos += 2;
+        xmlChar *rptr = (xmlChar *) ref->path;
+        int pos = 0;
+
+        /*
+         * Next we compare the two strings and find where they first differ
+         */
+	if ((*rptr == '.') && (rptr[1] == '/'))
+            rptr += 2;
 	if ((*bptr == '.') && (bptr[1] == '/'))
             bptr += 2;
-	else if ((*bptr == '/') && (ref->path[pos] != '/'))
+	else if ((*bptr == '/') && (*rptr != '/'))
 	    bptr++;
-	while ((bptr[pos] == ref->path[pos]) && (bptr[pos] != 0))
+	while ((bptr[pos] == rptr[pos]) && (bptr[pos] != 0))
 	    pos++;
 
-	if (bptr[pos] == ref->path[pos]) {
+	if (bptr[pos] == rptr[pos]) {
 	    val = xmlStrdup(BAD_CAST "");
 	    goto done;		/* (I can't imagine why anyone would do this) */
 	}
@@ -2277,25 +2278,25 @@
 	 * beginning of the "unique" suffix of URI
 	 */
 	ix = pos;
-	if ((ref->path[ix] == '/') && (ix > 0))
+	if ((rptr[ix] == '/') && (ix > 0))
 	    ix--;
-	else if ((ref->path[ix] == 0) && (ix > 1) && (ref->path[ix - 1] == '/'))
+	else if ((rptr[ix] == 0) && (ix > 1) && (rptr[ix - 1] == '/'))
 	    ix -= 2;
 	for (; ix > 0; ix--) {
-	    if (ref->path[ix] == '/')
+	    if (rptr[ix] == '/')
 		break;
 	}
 	if (ix == 0) {
-	    uptr = (xmlChar *)ref->path;
+	    uptr = (xmlChar *)rptr;
 	} else {
 	    ix++;
-	    uptr = (xmlChar *)&ref->path[ix];
+	    uptr = (xmlChar *)&rptr[ix];
 	}
 
 	/*
 	 * In base, count the number of '/' from the differing point
 	 */
-	if (bptr[pos] != ref->path[pos]) {/* check for trivial URI == base */
+	if (bptr[pos] != rptr[pos]) {/* check for trivial URI == base */
 	    for (; bptr[ix] != 0; ix++) {
 		if (bptr[ix] == '/')
 		    nbslash++;
@@ -2456,6 +2457,7 @@
 	        xmlFreeURI(uri);
 		return escURI;
 	    }
+            xmlFree(escURI);
 	}
     }
 
