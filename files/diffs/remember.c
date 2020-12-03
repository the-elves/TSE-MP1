--- files/before/remember.c	2020-12-03 05:32:54.138819146 +0530
+++ files/after/remember.c	2020-12-03 05:32:55.078969458 +0530
@@ -44,11 +44,11 @@
   sqlite3_value **argv
 ){
   sqlite3_int64 v;
-  sqlite3_int64 ptr;
+  sqlite3_int64 *ptr;
   assert( argc==2 );
   v = sqlite3_value_int64(argv[0]);
-  ptr = sqlite3_value_int64(argv[1]);
-  *((sqlite3_int64*)ptr) = v;
+  ptr = sqlite3_value_pointer(argv[1]);
+  if( ptr ) *ptr = v;
   sqlite3_result_int64(pCtx, v);
 }
 
