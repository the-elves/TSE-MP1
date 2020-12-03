--- files/before/nw_buf.c	2020-12-03 05:29:06.430412509 +0530
+++ files/after/nw_buf.c	2020-12-03 05:29:07.282548750 +0530
@@ -8,7 +8,9 @@
 # include "nw_buf.h"
 
 # define NW_BUF_POOL_INIT_SIZE 64
+# define NW_BUF_POOL_MAX_SIZE  65535
 # define NW_CACHE_INIT_SIZE    64
+# define NW_CACHE_MAX_SIZE     65535
 
 size_t nw_buf_size(nw_buf *buf)
 {
@@ -85,7 +87,7 @@
 {
     if (pool->free < pool->free_total) {
         pool->free_arr[pool->free++] = buf;
-    } else {
+    } else if (pool->free_total < NW_BUF_POOL_MAX_SIZE) {
         uint32_t new_free_total = pool->free_total * 2;
         void *new_arr = realloc(pool->free_arr, new_free_total * sizeof(nw_buf *));
         if (new_arr) {
@@ -95,6 +97,8 @@
         } else {
             free(buf);
         }
+    } else {
+        free(buf);
     }
 }
 
@@ -230,7 +234,7 @@
 {
     if (cache->free < cache->free_total) {
         cache->free_arr[cache->free++] = obj;
-    } else {
+    } else if (cache->free_total < NW_CACHE_MAX_SIZE) {
         uint32_t new_free_total = cache->free_total * 2;
         void *new_arr = realloc(cache->free_arr, new_free_total * sizeof(void *));
         if (new_arr) {
@@ -240,6 +244,8 @@
         } else {
             free(obj);
         }
+    } else {
+        free(obj);
     }
 }
 
