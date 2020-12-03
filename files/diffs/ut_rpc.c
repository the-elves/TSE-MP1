--- files/before/ut_rpc.c	2020-12-03 05:29:08.730780296 +0530
+++ files/after/ut_rpc.c	2020-12-03 05:29:09.642926131 +0530
@@ -44,13 +44,19 @@
 {
     static void *send_buf;
     static size_t send_buf_size;
-    uint32_t pkg_size = RPC_PKG_HEAD_SIZE + pkg->ext_size + pkg->body_size;
+    uint32_t pkg_size;
+    if (pkg->body_size > RPC_PKG_MAX_BODY_SIZE) {
+        return -1;
+    }
+    pkg_size = RPC_PKG_HEAD_SIZE + pkg->ext_size + pkg->body_size;
     if (send_buf_size < pkg_size) {
         if (send_buf)
             free(send_buf);
         send_buf_size = pkg_size * 2;
         send_buf = malloc(send_buf_size);
-        assert(send_buf != NULL);
+        if (send_buf == NULL) {
+            return -1;
+        }
     }
 
     memcpy(send_buf, pkg, RPC_PKG_HEAD_SIZE);
