--- files/before/valid.c	2020-12-03 05:33:25.099769265 +0530
+++ files/after/valid.c	2020-12-03 05:33:26.107930453 +0530
@@ -1172,33 +1172,33 @@
 	    xmlBufferWriteCHAR(buf, content->name);
 	    break;
 	case XML_ELEMENT_CONTENT_SEQ:
-	    if (content->c1 == NULL) return;
-	    if ((content->c1->type == XML_ELEMENT_CONTENT_OR) ||
-	        (content->c1->type == XML_ELEMENT_CONTENT_SEQ))
+	    if ((content->c1 != NULL) &&
+	        ((content->c1->type == XML_ELEMENT_CONTENT_OR) ||
+	         (content->c1->type == XML_ELEMENT_CONTENT_SEQ)))
 		xmlDumpElementContent(buf, content->c1, 1);
 	    else
 		xmlDumpElementContent(buf, content->c1, 0);
             xmlBufferWriteChar(buf, " , ");
-	    if (content->c2 == NULL) return;
-	    if ((content->c2->type == XML_ELEMENT_CONTENT_OR) ||
-	        ((content->c2->type == XML_ELEMENT_CONTENT_SEQ) &&
-		 (content->c2->ocur != XML_ELEMENT_CONTENT_ONCE)))
+	    if ((content->c2 != NULL) &&
+	        ((content->c2->type == XML_ELEMENT_CONTENT_OR) ||
+	         ((content->c2->type == XML_ELEMENT_CONTENT_SEQ) &&
+		  (content->c2->ocur != XML_ELEMENT_CONTENT_ONCE))))
 		xmlDumpElementContent(buf, content->c2, 1);
 	    else
 		xmlDumpElementContent(buf, content->c2, 0);
 	    break;
 	case XML_ELEMENT_CONTENT_OR:
-	    if (content->c1 == NULL) return;
-	    if ((content->c1->type == XML_ELEMENT_CONTENT_OR) ||
-	        (content->c1->type == XML_ELEMENT_CONTENT_SEQ))
+	    if ((content->c1 != NULL) &&
+	        ((content->c1->type == XML_ELEMENT_CONTENT_OR) ||
+	         (content->c1->type == XML_ELEMENT_CONTENT_SEQ)))
 		xmlDumpElementContent(buf, content->c1, 1);
 	    else
 		xmlDumpElementContent(buf, content->c1, 0);
             xmlBufferWriteChar(buf, " | ");
-	    if (content->c2 == NULL) return;
-	    if ((content->c2->type == XML_ELEMENT_CONTENT_SEQ) ||
-	        ((content->c2->type == XML_ELEMENT_CONTENT_OR) &&
-		 (content->c2->ocur != XML_ELEMENT_CONTENT_ONCE)))
+	    if ((content->c2 != NULL) &&
+	        ((content->c2->type == XML_ELEMENT_CONTENT_SEQ) ||
+	         ((content->c2->type == XML_ELEMENT_CONTENT_OR) &&
+		  (content->c2->ocur != XML_ELEMENT_CONTENT_ONCE))))
 		xmlDumpElementContent(buf, content->c2, 1);
 	    else
 		xmlDumpElementContent(buf, content->c2, 0);
@@ -1266,22 +1266,23 @@
         case XML_ELEMENT_CONTENT_PCDATA:
             strcat(buf, "#PCDATA");
 	    break;
-	case XML_ELEMENT_CONTENT_ELEMENT:
+	case XML_ELEMENT_CONTENT_ELEMENT: {
+            int qnameLen = xmlStrlen(content->name);
+
+	    if (content->prefix != NULL)
+                qnameLen += xmlStrlen(content->prefix) + 1;
+	    if (size - len < qnameLen + 10) {
+		strcat(buf, " ...");
+		return;
+	    }
 	    if (content->prefix != NULL) {
-		if (size - len < xmlStrlen(content->prefix) + 10) {
-		    strcat(buf, " ...");
-		    return;
-		}
 		strcat(buf, (char *) content->prefix);
 		strcat(buf, ":");
 	    }
-	    if (size - len < xmlStrlen(content->name) + 10) {
-		strcat(buf, " ...");
-		return;
-	    }
 	    if (content->name != NULL)
 		strcat(buf, (char *) content->name);
 	    break;
+        }
 	case XML_ELEMENT_CONTENT_SEQ:
 	    if ((content->c1->type == XML_ELEMENT_CONTENT_OR) ||
 	        (content->c1->type == XML_ELEMENT_CONTENT_SEQ))
@@ -1323,6 +1324,7 @@
 		xmlSnprintfElementContent(buf, size, content->c2, 0);
 	    break;
     }
+    if (size - strlen(buf) <= 2) return;
     if (englob)
         strcat(buf, ")");
     switch (content->ocur) {
@@ -4625,6 +4627,12 @@
 	}
     }
 
+    /*
+     * Casting ns to xmlAttrPtr is wrong. We'd need separate functions
+     * xmlAddID and xmlAddRef for namespace declarations, but it makes
+     * no practical sense to use ID types anyway.
+     */
+#if 0
     /* Validity Constraint: ID uniqueness */
     if (attrDecl->atype == XML_ATTRIBUTE_ID) {
         if (xmlAddID(ctxt, doc, value, (xmlAttrPtr) ns) == NULL)
@@ -4636,6 +4644,7 @@
         if (xmlAddRef(ctxt, doc, value, (xmlAttrPtr) ns) == NULL)
 	    ret = 0;
     }
+#endif
 
     /* Validity Constraint: Notation Attributes */
     if (attrDecl->atype == XML_ATTRIBUTE_NOTATION) {
