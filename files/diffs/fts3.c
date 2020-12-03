--- files/before/fts3.c	2020-12-03 05:32:50.246196782 +0530
+++ files/after/fts3.c	2020-12-03 05:32:51.282362446 +0530
@@ -3347,9 +3347,8 @@
     sqlite3_result_int64(pCtx, pCsr->iPrevId);
   }else if( iCol==p->nColumn ){
     /* The extra column whose name is the same as the table.
-    ** Return a blob which is a pointer to the cursor.  */
-    sqlite3_result_blob(pCtx, &pCsr, sizeof(pCsr), SQLITE_TRANSIENT);
-    sqlite3_result_subtype(pCtx, '3');
+    ** Return a pointer to the cursor.  */
+    sqlite3_result_pointer(pCtx, pCsr);
   }else if( iCol==p->nColumn+2 && pCsr->pExpr ){
     sqlite3_result_int64(pCtx, pCsr->iLangid);
   }else{
@@ -3561,17 +3560,13 @@
   sqlite3_value *pVal,            /* argv[0] passed to function */
   Fts3Cursor **ppCsr              /* OUT: Store cursor handle here */
 ){
-  Fts3Cursor *pRet;
-  if( sqlite3_value_type(pVal)!=SQLITE_BLOB 
-   || sqlite3_value_subtype(pVal)!='3'
-   || sqlite3_value_bytes(pVal)!=sizeof(Fts3Cursor *)
-  ){
+  Fts3Cursor *pRet = (Fts3Cursor*)sqlite3_value_pointer(pVal);
+  if( pRet==0 ){
     char *zErr = sqlite3_mprintf("illegal first argument to %s", zFunc);
     sqlite3_result_error(pContext, zErr, -1);
     sqlite3_free(zErr);
     return SQLITE_ERROR;
   }
-  memcpy(&pRet, sqlite3_value_blob(pVal), sizeof(Fts3Cursor *));
   *ppCsr = pRet;
   return SQLITE_OK;
 }
