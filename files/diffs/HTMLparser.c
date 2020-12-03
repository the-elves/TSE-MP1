--- files/before/HTMLparser.c	2020-12-03 05:33:02.744194997 +0530
+++ files/after/HTMLparser.c	2020-12-03 05:33:04.568486668 +0530
@@ -2528,8 +2528,12 @@
 	}
     }
 
-    if (ctxt->input->base > ctxt->input->cur - len)
-	return(NULL);
+    if (ctxt->input->cur - ctxt->input->base < len) {
+        /* Sanity check */
+	htmlParseErr(ctxt, XML_ERR_INTERNAL_ERROR,
+                     "unexpected change of input buffer", NULL, NULL);
+        return (NULL);
+    }
 
     return(xmlDictLookup(ctxt->dict, ctxt->input->cur - len, len));
 }
@@ -4444,7 +4448,7 @@
 htmlParseElementInternal(htmlParserCtxtPtr ctxt) {
     const xmlChar *name;
     const htmlElemDesc * info;
-    htmlParserNodeInfo node_info = { 0, };
+    htmlParserNodeInfo node_info = { NULL, 0, 0, 0, 0 };
     int failed;
 
     if ((ctxt == NULL) || (ctxt->input == NULL)) {
@@ -4941,6 +4945,7 @@
     ctxt->wellFormed = 1;
     ctxt->replaceEntities = 0;
     ctxt->linenumbers = xmlLineNumbersDefaultValue;
+    ctxt->keepBlanks = xmlKeepBlanksDefaultValue;
     ctxt->html = 1;
     ctxt->vctxt.finishDtd = XML_CTXT_FINISH_DTD_0;
     ctxt->vctxt.userData = ctxt;
