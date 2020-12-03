--- files/before/carray.c	2020-12-03 05:32:52.282522354 +0530
+++ files/after/carray.c	2020-12-03 05:32:53.222672668 +0530
@@ -73,7 +73,7 @@
 struct carray_cursor {
   sqlite3_vtab_cursor base;  /* Base class - must be first */
   sqlite3_int64 iRowid;      /* The rowid */
-  sqlite3_int64 iPtr;        /* Pointer to array of values */
+  void* pPtr;                /* Pointer to the array of values */
   sqlite3_int64 iCnt;        /* Number of integers in the array */
   unsigned char eType;       /* One of the CARRAY_type values */
 };
@@ -167,7 +167,7 @@
   carray_cursor *pCur = (carray_cursor*)cur;
   sqlite3_int64 x = 0;
   switch( i ){
-    case CARRAY_COLUMN_POINTER:   x = pCur->iPtr;   break;
+    case CARRAY_COLUMN_POINTER:   return SQLITE_OK;
     case CARRAY_COLUMN_COUNT:     x = pCur->iCnt;   break;
     case CARRAY_COLUMN_CTYPE: {
       sqlite3_result_text(ctx, azType[pCur->eType], -1, SQLITE_STATIC);
@@ -232,8 +232,8 @@
 ){
   carray_cursor *pCur = (carray_cursor *)pVtabCursor;
   if( idxNum ){
-    pCur->iPtr = sqlite3_value_int64(argv[0]);
-    pCur->iCnt = sqlite3_value_int64(argv[1]);
+    pCur->pPtr = sqlite3_value_pointer(argv[0]);
+    pCur->iCnt = pCur->pPtr ? sqlite3_value_int64(argv[1]) : 0;
     if( idxNum<3 ){
       pCur->eType = CARRAY_INT32;
     }else{
@@ -251,7 +251,7 @@
       }
     }
   }else{
-    pCur->iPtr = 0;
+    pCur->pPtr = 0;
     pCur->iCnt = 0;
   }
   pCur->iRowid = 1;
@@ -345,6 +345,34 @@
   0,                         /* xRename */
 };
 
+/*
+** For testing purpose in the TCL test harness, we need a method for
+** setting the pointer value.  The inttoptr(X) SQL function accomplishes
+** this.  Tcl script will bind an integer to X and the inttoptr() SQL
+** function will use sqlite3_result_pointer() to convert that integer into
+** a pointer.
+**
+** This is for testing on TCL only.
+*/
+#ifdef SQLITE_TEST
+static void inttoptrFunc(
+  sqlite3_context *context,
+  int argc,
+  sqlite3_value **argv
+){
+  void *p;
+  sqlite3_int64 i64;
+  i64 = sqlite3_value_int64(argv[0]);
+  if( sizeof(i64)==sizeof(p) ){
+    memcpy(&p, &i64, sizeof(p));
+  }else{
+    int i32 = i64 & 0xffffffff;
+    memcpy(&p, &i32, sizeof(p));
+  }
+  sqlite3_result_pointer(context, p);
+}
+#endif /* SQLITE_TEST */
+
 #endif /* SQLITE_OMIT_VIRTUALTABLE */
 
 #ifdef _WIN32
@@ -359,6 +387,12 @@
   SQLITE_EXTENSION_INIT2(pApi);
 #ifndef SQLITE_OMIT_VIRTUALTABLE
   rc = sqlite3_create_module(db, "carray", &carrayModule, 0);
-#endif
+#ifdef SQLITE_TEST
+  if( rc==SQLITE_OK ){
+    rc = sqlite3_create_function(db, "inttoptr", 1, SQLITE_UTF8, 0,
+                                 inttoptrFunc, 0, 0);
+  }
+#endif /* SQLITE_TEST */
+#endif /* SQLITE_OMIT_VIRTUALTABLE */
   return rc;
 }
