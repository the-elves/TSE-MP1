--- files/before/parserInternals.c	2020-12-03 05:33:12.001675107 +0530
+++ files/after/parserInternals.c	2020-12-03 05:33:12.953827338 +0530
@@ -710,16 +710,6 @@
     return((int) *ctxt->input->cur);
 encoding_error:
     /*
-     * An encoding problem may arise from a truncated input buffer
-     * splitting a character in the middle. In that case do not raise
-     * an error but return 0 to endicate an end of stream problem
-     */
-    if (ctxt->input->end - ctxt->input->cur < 4) {
-	*len = 0;
-	return(0);
-    }
-
-    /*
      * If we detect an UTF8 error that probably mean that the
      * input encoding didn't get properly advertised in the
      * declaration header. Report the error and switch the encoding
@@ -729,9 +719,21 @@
     {
         char buffer[150];
 
-	snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
-			ctxt->input->cur[0], ctxt->input->cur[1],
-			ctxt->input->cur[2], ctxt->input->cur[3]);
+        if (ctxt->input->cur[1] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X EOF\n",
+                     ctxt->input->cur[0]);
+        } else if (ctxt->input->cur[2] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X EOF\n",
+                     ctxt->input->cur[0], ctxt->input->cur[1]);
+        } else if (ctxt->input->cur[3] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X 0x%02X EOF\n",
+                     ctxt->input->cur[0], ctxt->input->cur[1],
+                     ctxt->input->cur[2]);
+        } else {
+	    snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
+		     ctxt->input->cur[0], ctxt->input->cur[1],
+		     ctxt->input->cur[2], ctxt->input->cur[3]);
+        }
 	__xmlErrEncoding(ctxt, XML_ERR_INVALID_CHAR,
 		     "Input is not proper UTF-8, indicate encoding !\n%s",
 		     BAD_CAST buffer, NULL);
@@ -821,17 +823,6 @@
     *len = 1;
     return ((int) *cur);
 encoding_error:
-
-    /*
-     * An encoding problem may arise from a truncated input buffer
-     * splitting a character in the middle. In that case do not raise
-     * an error but return 0 to endicate an end of stream problem
-     */
-    if ((ctxt == NULL) || (ctxt->input == NULL) ||
-        (ctxt->input->end - ctxt->input->cur < 4)) {
-	*len = 0;
-	return(0);
-    }
     /*
      * If we detect an UTF8 error that probably mean that the
      * input encoding didn't get properly advertised in the
@@ -842,9 +833,19 @@
     {
         char buffer[150];
 
-	snprintf(buffer, 149, "Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
-			ctxt->input->cur[0], ctxt->input->cur[1],
-			ctxt->input->cur[2], ctxt->input->cur[3]);
+        if (cur[1] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X EOF\n",
+                     cur[0]);
+        } else if (cur[2] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X EOF\n",
+                     cur[0], cur[1]);
+        } else if (cur[3] == 0) {
+            snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X 0x%02X EOF\n",
+                     cur[0], cur[1], cur[2]);
+        } else {
+	    snprintf(&buffer[0], 149, "Bytes: 0x%02X 0x%02X 0x%02X 0x%02X\n",
+		     cur[0], cur[1], cur[2], cur[3]);
+        }
 	__xmlErrEncoding(ctxt, XML_ERR_INVALID_CHAR,
 		     "Input is not proper UTF-8, indicate encoding !\n%s",
 		     BAD_CAST buffer, NULL);
@@ -1101,8 +1102,15 @@
 	        break;
 	}
     }
-    if (handler == NULL)
+    /*
+     * TODO: We could recover from errors in external entites if we
+     * didn't stop the parser. But most callers of this function don't
+     * check the return value.
+     */
+    if (handler == NULL) {
+        xmlStopParser(ctxt);
 	return(-1);
+    }
     ctxt->charset = XML_CHAR_ENCODING_UTF8;
     ret = xmlSwitchToEncodingInt(ctxt, handler, len);
     if ((ret < 0) || (ctxt->errNo == XML_I18N_CONV_FAILED)) {
@@ -1226,6 +1234,7 @@
                  */
                 nbchars = xmlCharEncFirstLineInput(input->buf, len);
             }
+            xmlBufResetInput(input->buf->buffer, input);
             if (nbchars < 0) {
                 xmlErrInternal(ctxt,
                                "switching encoding: encoder error\n",
@@ -1233,7 +1242,6 @@
                 return (-1);
             }
 	    input->buf->rawconsumed += use - xmlBufUse(input->buf->raw);
-            xmlBufResetInput(input->buf->buffer, input);
         }
         return (0);
     } else if (input->length == 0) {
