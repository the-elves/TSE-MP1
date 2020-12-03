--- files/before/xmlIO.c	2020-12-03 05:33:27.096088442 +0530
+++ files/after/xmlIO.c	2020-12-03 05:33:28.096248350 +0530
@@ -210,7 +210,6 @@
     "adddress in use",		/* EADDRINUSE */
     "already in use",		/* EALREADY */
     "unknown address familly",	/* EAFNOSUPPORT */
-    "Attempt to load external entity %s", /* XML_IO_ILLEGAL_XXE */
 };
 
 #if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
@@ -3028,7 +3027,7 @@
     xmlParserInputBufferPtr ret;
     int errcode;
 
-    if (size <= 0) return(NULL);
+    if (size < 0) return(NULL);
     if (mem == NULL) return(NULL);
 
     ret = xmlAllocParserInputBuffer(enc);
@@ -3064,7 +3063,7 @@
                                  xmlCharEncoding enc) {
     xmlParserInputBufferPtr ret;
 
-    if (size <= 0) return(NULL);
+    if (size < 0) return(NULL);
     if (mem == NULL) return(NULL);
 
     ret = (xmlParserInputBufferPtr) xmlMalloc(sizeof(xmlParserInputBuffer));
@@ -4054,22 +4053,13 @@
     xmlGenericError(xmlGenericErrorContext,
                     "xmlDefaultExternalEntityLoader(%s, xxx)\n", URL);
 #endif
-    if (ctxt != NULL) {
+    if ((ctxt != NULL) && (ctxt->options & XML_PARSE_NONET)) {
         int options = ctxt->options;
 
-        if (options & XML_PARSE_NOXXE) {
-            ctxt->options -= XML_PARSE_NOXXE;
-            ret = xmlNoXxeExternalEntityLoader(URL, ID, ctxt);
-            ctxt->options = options;
-            return(ret);
-        }
- 
-        if (options & XML_PARSE_NONET) {
-            ctxt->options -= XML_PARSE_NONET;
-            ret = xmlNoNetExternalEntityLoader(URL, ID, ctxt);
-            ctxt->options = options;
-            return(ret);
-        }
+	ctxt->options -= XML_PARSE_NONET;
+        ret = xmlNoNetExternalEntityLoader(URL, ID, ctxt);
+	ctxt->options = options;
+	return(ret);
     }
 #ifdef LIBXML_CATALOG_ENABLED
     resource = xmlResolveResourceFromCatalog(URL, ID, ctxt);
@@ -4170,13 +4160,6 @@
     xmlParserInputPtr input = NULL;
     xmlChar *resource = NULL;
 
-    if (ctxt == NULL) {
-        return(NULL);
-    }
-    if (ctxt->input_id == 1) {
-        return xmlDefaultExternalEntityLoader((const char *) URL, ID, ctxt);
-    }
-
 #ifdef LIBXML_CATALOG_ENABLED
     resource = xmlResolveResourceFromCatalog(URL, ID, ctxt);
 #endif
@@ -4199,18 +4182,5 @@
     return(input);
 }
 
-xmlParserInputPtr
-xmlNoXxeExternalEntityLoader(const char *URL, const char *ID,
-                          xmlParserCtxtPtr ctxt) {
-    if (ctxt == NULL) {
-        return(NULL);
-    }
-    if (ctxt->input_id == 1) {
-        return xmlDefaultExternalEntityLoader((const char *) URL, ID, ctxt);
-    }
-    xmlIOErr(XML_IO_ILLEGAL_XXE, (const char *) URL);
-    return(NULL);
-}
-
 #define bottom_xmlIO
 #include "elfgcchack.h"
