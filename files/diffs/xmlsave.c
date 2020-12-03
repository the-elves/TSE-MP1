--- files/before/xmlsave.c	2020-12-03 05:33:30.904697373 +0530
+++ files/after/xmlsave.c	2020-12-03 05:33:31.864850885 +0530
@@ -2109,8 +2109,6 @@
                 xmlBufAdd(buf, base, cur - base);
             if (*cur < 0xC0) {
                 xmlSaveErr(XML_SAVE_NOT_UTF8, (xmlNodePtr) attr, NULL);
-                if (doc != NULL)
-                    doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
 		xmlSerializeHexCharRef(tmp, *cur);
                 xmlBufAdd(buf, (xmlChar *) tmp, -1);
                 cur++;
@@ -2140,9 +2138,6 @@
             }
             if ((l == 1) || (!IS_CHAR(val))) {
                 xmlSaveErr(XML_SAVE_CHAR_INVALID, (xmlNodePtr) attr, NULL);
-                if (doc != NULL)
-                    doc->encoding = xmlStrdup(BAD_CAST "ISO-8859-1");
-
 		xmlSerializeHexCharRef(tmp, *cur);
                 xmlBufAdd(buf, (xmlChar *) tmp, -1);
                 cur++;
