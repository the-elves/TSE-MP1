--- files/before/testapi.c	2020-12-03 05:33:18.230671016 +0530
+++ files/after/testapi.c	2020-12-03 05:33:19.602890409 +0530
@@ -1480,6 +1480,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = htmlCreateMemoryParserCtxt((const char *)buffer, size);
         desret_htmlParserCtxtPtr(ret_val);
@@ -1547,6 +1550,9 @@
         size = gen_int(n_size, 3);
         filename = gen_fileoutput(n_filename, 4);
         enc = gen_xmlCharEncoding(n_enc, 5);
+        if ((chunk != NULL) &&
+            (size > (int) strlen((const char *) chunk) + 1))
+            continue;
 
         ret_val = htmlCreatePushParserCtxt(sax, user_data, (const char *)chunk, size, filename, enc);
         desret_htmlParserCtxtPtr(ret_val);
@@ -1721,6 +1727,9 @@
         URL = gen_filepath(n_URL, 3);
         encoding = gen_const_char_ptr(n_encoding, 4);
         options = gen_int(n_options, 5);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = htmlCtxtReadMemory(ctxt, (const char *)buffer, size, URL, (const char *)encoding, options);
         desret_htmlDocPtr(ret_val);
@@ -2278,6 +2287,9 @@
         chunk = gen_const_char_ptr(n_chunk, 1);
         size = gen_int(n_size, 2);
         terminate = gen_int(n_terminate, 3);
+        if ((chunk != NULL) &&
+            (size > (int) strlen((const char *) chunk) + 1))
+            continue;
 
         ret_val = htmlParseChunk(ctxt, (const char *)chunk, size, terminate);
         if (ctxt != NULL) {xmlFreeDoc(ctxt->myDoc); ctxt->myDoc = NULL;}
@@ -2621,6 +2633,9 @@
         URL = gen_filepath(n_URL, 2);
         encoding = gen_const_char_ptr(n_encoding, 3);
         options = gen_int(n_options, 4);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = htmlReadMemory((const char *)buffer, size, URL, (const char *)encoding, options);
         desret_htmlDocPtr(ret_val);
@@ -3870,6 +3885,9 @@
         ctx = gen_void_ptr(n_ctx, 0);
         value = gen_const_xmlChar_ptr(n_value, 1);
         len = gen_int(n_len, 2);
+        if ((value != NULL) &&
+            (len > (int) strlen((const char *) value) + 1))
+            continue;
 
         xmlSAX2CDataBlock(ctx, (const xmlChar *)value, len);
         call_tests++;
@@ -3914,6 +3932,9 @@
         ctx = gen_void_ptr(n_ctx, 0);
         ch = gen_const_xmlChar_ptr(n_ch, 1);
         len = gen_int(n_len, 2);
+        if ((ch != NULL) &&
+            (len > (int) strlen((const char *) ch) + 1))
+            continue;
 
         xmlSAX2Characters(ctx, (const xmlChar *)ch, len);
         call_tests++;
@@ -4554,6 +4575,9 @@
         ctx = gen_void_ptr(n_ctx, 0);
         ch = gen_const_xmlChar_ptr(n_ch, 1);
         len = gen_int(n_len, 2);
+        if ((ch != NULL) &&
+            (len > (int) strlen((const char *) ch) + 1))
+            continue;
 
         xmlSAX2IgnorableWhitespace(ctx, (const xmlChar *)ch, len);
         call_tests++;
@@ -8221,6 +8245,9 @@
         dict = gen_xmlDictPtr(n_dict, 0);
         name = gen_const_xmlChar_ptr(n_name, 1);
         len = gen_int(n_len, 2);
+        if ((name != NULL) &&
+            (len > (int) strlen((const char *) name) + 1))
+            continue;
 
         ret_val = xmlDictExists(dict, (const xmlChar *)name, len);
         desret_const_xmlChar_ptr(ret_val);
@@ -8277,6 +8304,9 @@
         dict = gen_xmlDictPtr(n_dict, 0);
         name = gen_const_xmlChar_ptr(n_name, 1);
         len = gen_int(n_len, 2);
+        if ((name != NULL) &&
+            (len > (int) strlen((const char *) name) + 1))
+            continue;
 
         ret_val = xmlDictLookup(dict, (const xmlChar *)name, len);
         desret_const_xmlChar_ptr(ret_val);
@@ -12727,6 +12757,9 @@
         chunk = gen_const_char_ptr(n_chunk, 2);
         size = gen_int(n_size, 3);
         filename = gen_fileoutput(n_filename, 4);
+        if ((chunk != NULL) &&
+            (size > (int) strlen((const char *) chunk) + 1))
+            continue;
 
         ret_val = xmlCreatePushParserCtxt(sax, user_data, (const char *)chunk, size, filename);
         desret_xmlParserCtxtPtr(ret_val);
@@ -12905,6 +12938,9 @@
         URL = gen_filepath(n_URL, 3);
         encoding = gen_const_char_ptr(n_encoding, 4);
         options = gen_parseroptions(n_options, 5);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlCtxtReadMemory(ctxt, (const char *)buffer, size, URL, (const char *)encoding, options);
         desret_xmlDocPtr(ret_val);
@@ -12998,6 +13034,9 @@
         size = gen_int(n_size, 2);
         filename = gen_filepath(n_filename, 3);
         encoding = gen_const_char_ptr(n_encoding, 4);
+        if ((chunk != NULL) &&
+            (size > (int) strlen((const char *) chunk) + 1))
+            continue;
 
         ret_val = xmlCtxtResetPush(ctxt, (const char *)chunk, size, filename, (const char *)encoding);
         desret_int(ret_val);
@@ -13709,6 +13748,9 @@
         chunk = gen_const_char_ptr(n_chunk, 1);
         size = gen_int(n_size, 2);
         terminate = gen_int(n_terminate, 3);
+        if ((chunk != NULL) &&
+            (size > (int) strlen((const char *) chunk) + 1))
+            continue;
 
         ret_val = xmlParseChunk(ctxt, (const char *)chunk, size, terminate);
         if (ctxt != NULL) {xmlFreeDoc(ctxt->myDoc); ctxt->myDoc = NULL;}
@@ -14166,6 +14208,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlParseMemory((const char *)buffer, size);
         desret_xmlDocPtr(ret_val);
@@ -14578,6 +14623,9 @@
         URL = gen_filepath(n_URL, 2);
         encoding = gen_const_char_ptr(n_encoding, 3);
         options = gen_parseroptions(n_options, 4);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlReadMemory((const char *)buffer, size, URL, (const char *)encoding, options);
         desret_xmlDocPtr(ret_val);
@@ -14700,6 +14748,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlRecoverMemory((const char *)buffer, size);
         desret_xmlDocPtr(ret_val);
@@ -15001,6 +15052,9 @@
         buffer = gen_const_char_ptr(n_buffer, 1);
         size = gen_int(n_size, 2);
         recovery = gen_int(n_recovery, 3);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlSAXParseMemory(sax, (const char *)buffer, size, recovery);
         desret_xmlDocPtr(ret_val);
@@ -15062,6 +15116,9 @@
         size = gen_int(n_size, 2);
         recovery = gen_int(n_recovery, 3);
         data = gen_userdata(n_data, 4);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlSAXParseMemoryWithData(sax, (const char *)buffer, size, recovery, data);
         desret_xmlDocPtr(ret_val);
@@ -15177,6 +15234,9 @@
         user_data = gen_userdata(n_user_data, 1);
         buffer = gen_const_char_ptr(n_buffer, 2);
         size = gen_int(n_size, 3);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
         
 #ifdef LIBXML_SAX1_ENABLED
         if (sax == (xmlSAXHandlerPtr)&xmlDefaultSAXHandler) user_data = NULL;
@@ -15948,6 +16008,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlCreateMemoryParserCtxt((const char *)buffer, size);
         desret_xmlParserCtxtPtr(ret_val);
@@ -16603,6 +16666,9 @@
         end = gen_xmlChar(n_end, 4);
         end2 = gen_xmlChar(n_end2, 5);
         end3 = gen_xmlChar(n_end3, 6);
+        if ((str != NULL) &&
+            (len > (int) strlen((const char *) str) + 1))
+            continue;
 
         ret_val = xmlStringLenDecodeEntities(ctxt, (const xmlChar *)str, len, what, end, end2, end3);
         desret_xmlChar_ptr(ret_val);
@@ -17573,6 +17639,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlRelaxNGNewMemParserCtxt((const char *)buffer, size);
         desret_xmlRelaxNGParserCtxtPtr(ret_val);
@@ -17849,6 +17918,9 @@
         ctxt = gen_xmlRelaxNGValidCtxtPtr(n_ctxt, 0);
         data = gen_const_xmlChar_ptr(n_data, 1);
         len = gen_int(n_len, 2);
+        if ((data != NULL) &&
+            (len > (int) strlen((const char *) data) + 1))
+            continue;
 
         ret_val = xmlRelaxNGValidatePushCData(ctxt, (const xmlChar *)data, len);
         desret_int(ret_val);
@@ -18591,6 +18663,9 @@
         buf = gen_xmlBufferPtr(n_buf, 0);
         str = gen_const_xmlChar_ptr(n_str, 1);
         len = gen_int(n_len, 2);
+        if ((str != NULL) &&
+            (len > (int) strlen((const char *) str) + 1))
+            continue;
 
         ret_val = xmlBufferAdd(buf, (const xmlChar *)str, len);
         desret_int(ret_val);
@@ -18637,6 +18712,9 @@
         buf = gen_xmlBufferPtr(n_buf, 0);
         str = gen_const_xmlChar_ptr(n_str, 1);
         len = gen_int(n_len, 2);
+        if ((str != NULL) &&
+            (len > (int) strlen((const char *) str) + 1))
+            continue;
 
         ret_val = xmlBufferAddHead(buf, (const xmlChar *)str, len);
         desret_int(ret_val);
@@ -19209,6 +19287,9 @@
         prefix = gen_const_xmlChar_ptr(n_prefix, 1);
         memory = gen_xmlChar_ptr(n_memory, 2);
         len = gen_int(n_len, 3);
+        if ((prefix != NULL) &&
+            (len > (int) strlen((const char *) prefix) + 1))
+            continue;
 
         ret_val = xmlBuildQName((const xmlChar *)ncname, (const xmlChar *)prefix, memory, len);
         if ((ret_val != NULL) && (ret_val != ncname) &&
@@ -20980,6 +21061,9 @@
         doc = gen_xmlDocPtr(n_doc, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         ret_val = xmlNewCDataBlock(doc, (const xmlChar *)content, len);
         desret_xmlNodePtr(ret_val);
@@ -21553,6 +21637,9 @@
         doc = gen_xmlDocPtr(n_doc, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         ret_val = xmlNewDocTextLen(doc, (const xmlChar *)content, len);
         desret_xmlNodePtr(ret_val);
@@ -22096,6 +22183,9 @@
         mem_base = xmlMemBlocks();
         content = gen_const_xmlChar_ptr(n_content, 0);
         len = gen_int(n_len, 1);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         ret_val = xmlNewTextLen((const xmlChar *)content, len);
         desret_xmlNodePtr(ret_val);
@@ -22209,6 +22299,9 @@
         cur = gen_xmlNodePtr(n_cur, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         xmlNodeAddContentLen(cur, (const xmlChar *)content, len);
         call_tests++;
@@ -22759,6 +22852,9 @@
         cur = gen_xmlNodePtr(n_cur, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         xmlNodeSetContentLen(cur, (const xmlChar *)content, len);
         call_tests++;
@@ -23823,6 +23919,9 @@
         doc = gen_const_xmlDoc_ptr(n_doc, 0);
         value = gen_const_xmlChar_ptr(n_value, 1);
         len = gen_int(n_len, 2);
+        if ((value != NULL) &&
+            (len > (int) strlen((const char *) value) + 1))
+            continue;
 
         ret_val = xmlStringLenGetNodeList((const xmlDoc *)doc, (const xmlChar *)value, len);
         desret_xmlNodePtr(ret_val);
@@ -23869,6 +23968,9 @@
         node = gen_xmlNodePtr(n_node, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         ret_val = xmlTextConcat(node, (const xmlChar *)content, len);
         desret_int(ret_val);
@@ -27125,6 +27227,9 @@
         ctxt = gen_xmlValidCtxtPtr(n_ctxt, 0);
         data = gen_const_xmlChar_ptr(n_data, 1);
         len = gen_int(n_len, 2);
+        if ((data != NULL) &&
+            (len > (int) strlen((const char *) data) + 1))
+            continue;
 
         ret_val = xmlValidatePushCData(ctxt, (const xmlChar *)data, len);
         desret_int(ret_val);
@@ -28661,6 +28766,9 @@
         out = gen_xmlOutputBufferPtr(n_out, 0);
         len = gen_int(n_len, 1);
         buf = gen_const_char_ptr(n_buf, 2);
+        if ((buf != NULL) &&
+            (len > (int) strlen((const char *) buf) + 1))
+            continue;
 
         ret_val = xmlOutputBufferWrite(out, len, (const char *)buf);
         desret_int(ret_val);
@@ -28887,6 +28995,9 @@
         mem = gen_const_char_ptr(n_mem, 0);
         size = gen_int(n_size, 1);
         enc = gen_xmlCharEncoding(n_enc, 2);
+        if ((mem != NULL) &&
+            (size > (int) strlen((const char *) mem) + 1))
+            continue;
 
         ret_val = xmlParserInputBufferCreateMem((const char *)mem, size, enc);
         desret_xmlParserInputBufferPtr(ret_val);
@@ -28933,6 +29044,9 @@
         mem = gen_const_char_ptr(n_mem, 0);
         size = gen_int(n_size, 1);
         enc = gen_xmlCharEncoding(n_enc, 2);
+        if ((mem != NULL) &&
+            (size > (int) strlen((const char *) mem) + 1))
+            continue;
 
         ret_val = xmlParserInputBufferCreateStatic((const char *)mem, size, enc);
         desret_xmlParserInputBufferPtr(ret_val);
@@ -29018,6 +29132,9 @@
         in = gen_xmlParserInputBufferPtr(n_in, 0);
         len = gen_int(n_len, 1);
         buf = gen_const_char_ptr(n_buf, 2);
+        if ((buf != NULL) &&
+            (len > (int) strlen((const char *) buf) + 1))
+            continue;
 
         ret_val = xmlParserInputBufferPush(in, len, (const char *)buf);
         desret_int(ret_val);
@@ -30200,6 +30317,9 @@
         URL = gen_filepath(n_URL, 2);
         encoding = gen_const_char_ptr(n_encoding, 3);
         options = gen_parseroptions(n_options, 4);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlReaderForMemory((const char *)buffer, size, URL, (const char *)encoding, options);
         desret_xmlTextReaderPtr(ret_val);
@@ -30383,6 +30503,9 @@
         URL = gen_filepath(n_URL, 3);
         encoding = gen_const_char_ptr(n_encoding, 4);
         options = gen_parseroptions(n_options, 5);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlReaderNewMemory(reader, (const char *)buffer, size, URL, (const char *)encoding, options);
         desret_int(ret_val);
@@ -34420,6 +34543,9 @@
         mem_base = xmlMemBlocks();
         buffer = gen_const_char_ptr(n_buffer, 0);
         size = gen_int(n_size, 1);
+        if ((buffer != NULL) &&
+            (size > (int) strlen((const char *) buffer) + 1))
+            continue;
 
         ret_val = xmlSchemaNewMemParserCtxt((const char *)buffer, size);
         desret_xmlSchemaParserCtxtPtr(ret_val);
@@ -36304,6 +36430,9 @@
         mem_base = xmlMemBlocks();
         cur = gen_const_char_ptr(n_cur, 0);
         len = gen_int(n_len, 1);
+        if ((cur != NULL) &&
+            (len > (int) strlen((const char *) cur) + 1))
+            continue;
 
         ret_val = xmlCharStrndup((const char *)cur, len);
         desret_xmlChar_ptr(ret_val);
@@ -36743,6 +36872,9 @@
         str1 = gen_const_xmlChar_ptr(n_str1, 0);
         str2 = gen_const_xmlChar_ptr(n_str2, 1);
         len = gen_int(n_len, 2);
+        if ((str2 != NULL) &&
+            (len > (int) strlen((const char *) str2) + 1))
+            continue;
 
         ret_val = xmlStrncasecmp((const xmlChar *)str1, (const xmlChar *)str2, len);
         desret_int(ret_val);
@@ -36789,6 +36921,9 @@
         str1 = gen_const_xmlChar_ptr(n_str1, 0);
         str2 = gen_const_xmlChar_ptr(n_str2, 1);
         len = gen_int(n_len, 2);
+        if ((str2 != NULL) &&
+            (len > (int) strlen((const char *) str2) + 1))
+            continue;
 
         ret_val = xmlStrncatNew((const xmlChar *)str1, (const xmlChar *)str2, len);
         desret_xmlChar_ptr(ret_val);
@@ -36835,6 +36970,9 @@
         str1 = gen_const_xmlChar_ptr(n_str1, 0);
         str2 = gen_const_xmlChar_ptr(n_str2, 1);
         len = gen_int(n_len, 2);
+        if ((str2 != NULL) &&
+            (len > (int) strlen((const char *) str2) + 1))
+            continue;
 
         ret_val = xmlStrncmp((const xmlChar *)str1, (const xmlChar *)str2, len);
         desret_int(ret_val);
@@ -36877,6 +37015,9 @@
         mem_base = xmlMemBlocks();
         cur = gen_const_xmlChar_ptr(n_cur, 0);
         len = gen_int(n_len, 1);
+        if ((cur != NULL) &&
+            (len > (int) strlen((const char *) cur) + 1))
+            continue;
 
         ret_val = xmlStrndup((const xmlChar *)cur, len);
         desret_xmlChar_ptr(ret_val);
@@ -36959,6 +37100,12 @@
         str = gen_const_xmlChar_ptr(n_str, 0);
         start = gen_int(n_start, 1);
         len = gen_int(n_len, 2);
+        if ((str != NULL) &&
+            (start > (int) strlen((const char *) str) + 1))
+            continue;
+        if ((str != NULL) &&
+            (len > (int) strlen((const char *) str) + 1))
+            continue;
 
         ret_val = xmlStrsub((const xmlChar *)str, start, len);
         desret_xmlChar_ptr(ret_val);
@@ -37143,6 +37290,9 @@
         mem_base = xmlMemBlocks();
         utf = gen_const_xmlChar_ptr(n_utf, 0);
         len = gen_int(n_len, 1);
+        if ((utf != NULL) &&
+            (len > (int) strlen((const char *) utf) + 1))
+            continue;
 
         ret_val = xmlUTF8Strndup((const xmlChar *)utf, len);
         desret_xmlChar_ptr(ret_val);
@@ -37221,6 +37371,9 @@
         mem_base = xmlMemBlocks();
         utf = gen_const_xmlChar_ptr(n_utf, 0);
         len = gen_int(n_len, 1);
+        if ((utf != NULL) &&
+            (len > (int) strlen((const char *) utf) + 1))
+            continue;
 
         ret_val = xmlUTF8Strsize((const xmlChar *)utf, len);
         desret_int(ret_val);
@@ -37264,6 +37417,12 @@
         utf = gen_const_xmlChar_ptr(n_utf, 0);
         start = gen_int(n_start, 1);
         len = gen_int(n_len, 2);
+        if ((utf != NULL) &&
+            (start > (int) strlen((const char *) utf) + 1))
+            continue;
+        if ((utf != NULL) &&
+            (len > (int) strlen((const char *) utf) + 1))
+            continue;
 
         ret_val = xmlUTF8Strsub((const xmlChar *)utf, start, len);
         desret_xmlChar_ptr(ret_val);
@@ -44576,6 +44735,12 @@
         data = gen_const_char_ptr(n_data, 1);
         start = gen_int(n_start, 2);
         len = gen_int(n_len, 3);
+        if ((data != NULL) &&
+            (start > (int) strlen((const char *) data) + 1))
+            continue;
+        if ((data != NULL) &&
+            (len > (int) strlen((const char *) data) + 1))
+            continue;
 
         ret_val = xmlTextWriterWriteBase64(writer, (const char *)data, start, len);
         desret_int(ret_val);
@@ -44631,6 +44796,12 @@
         data = gen_const_char_ptr(n_data, 1);
         start = gen_int(n_start, 2);
         len = gen_int(n_len, 3);
+        if ((data != NULL) &&
+            (start > (int) strlen((const char *) data) + 1))
+            continue;
+        if ((data != NULL) &&
+            (len > (int) strlen((const char *) data) + 1))
+            continue;
 
         ret_val = xmlTextWriterWriteBinHex(writer, (const char *)data, start, len);
         desret_int(ret_val);
@@ -45561,6 +45732,9 @@
         writer = gen_xmlTextWriterPtr(n_writer, 0);
         content = gen_const_xmlChar_ptr(n_content, 1);
         len = gen_int(n_len, 2);
+        if ((content != NULL) &&
+            (len > (int) strlen((const char *) content) + 1))
+            continue;
 
         ret_val = xmlTextWriterWriteRawLen(writer, (const xmlChar *)content, len);
         desret_int(ret_val);
