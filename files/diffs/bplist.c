--- files/before/bplist.c	2020-12-03 05:28:44.890968722 +0530
+++ files/after/bplist.c	2020-12-03 05:28:45.899129912 +0530
@@ -289,6 +289,11 @@
 
     data->type = PLIST_STRING;
     data->strval = (char *) malloc(sizeof(char) * (size + 1));
+    if (!data->strval) {
+        plist_free_data(data);
+        PLIST_BIN_ERR("%s: Could not allocate %" PRIu64 " bytes\n", __func__, sizeof(char) * (size + 1));
+        return NULL;
+    }
     memcpy(data->strval, *bnode, size);
     data->strval[size] = '\0';
     data->length = strlen(data->strval);
