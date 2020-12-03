--- files/before/sqlite3.c	2020-12-03 05:32:46.497597446 +0530
+++ files/after/sqlite3.c	2020-12-03 05:32:49.174025361 +0530
@@ -4065,6 +4065,15 @@
 ** [sqlite3_blob_open | incremental BLOB I/O] routines.
 ** ^A negative value for the zeroblob results in a zero-length BLOB.
 **
+** ^The sqlite3_bind_pointer(S,I,P) routine causes the I-th parameter in
+** [prepared statement] S to have an SQL value of NULL, but to also be
+** associated with the pointer P.
+** ^The sqlite3_bind_pointer() routine can be used to pass
+** host-language pointers into [application-defined SQL functions].
+** ^A parameter that is initialized using [sqlite3_bind_pointer()] appears
+** to be an ordinary SQL NULL value to everything other than
+** [sqlite3_value_pointer()].
+**
 ** ^If any of the sqlite3_bind_*() routines are called with a NULL pointer
 ** for the [prepared statement] or with a prepared statement for which
 ** [sqlite3_step()] has been called more recently than [sqlite3_reset()],
@@ -4098,6 +4107,7 @@
 SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64,
                          void(*)(void*), unsigned char encoding);
 SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
+SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*);
 SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
 SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);
 
@@ -4867,6 +4877,11 @@
 ** sqlite3_value_text16be() and sqlite3_value_text16le() interfaces
 ** extract UTF-16 strings as big-endian and little-endian respectively.
 **
+** ^If [sqlite3_value] object V was initialized
+** using [sqlite3_bind_pointer(S,I,P)] or [sqlite3_result_pointer(C,P)], then
+** sqlite3_value_pointer(V) will return the pointer P.  Otherwise,
+** sqlite3_value_pointer(V) returns a NULL.
+**
 ** ^(The sqlite3_value_numeric_type() interface attempts to apply
 ** numeric affinity to the value.  This means that an attempt is
 ** made to convert the value to an integer or floating point.  If
@@ -4894,6 +4909,7 @@
 SQLITE_API const void *sqlite3_value_text16(sqlite3_value*);
 SQLITE_API const void *sqlite3_value_text16le(sqlite3_value*);
 SQLITE_API const void *sqlite3_value_text16be(sqlite3_value*);
+SQLITE_API void *sqlite3_value_pointer(sqlite3_value*);
 SQLITE_API int sqlite3_value_type(sqlite3_value*);
 SQLITE_API int sqlite3_value_numeric_type(sqlite3_value*);
 
@@ -4906,10 +4922,6 @@
 ** information can be used to pass a limited amount of context from
 ** one SQL function to another.  Use the [sqlite3_result_subtype()]
 ** routine to set the subtype for the return value of an SQL function.
-**
-** SQLite makes no use of subtype itself.  It merely passes the subtype
-** from the result of one [application-defined SQL function] into the
-** input of another.
 */
 SQLITE_API unsigned int sqlite3_value_subtype(sqlite3_value*);
 
@@ -5187,6 +5199,14 @@
 ** [unprotected sqlite3_value] object is required, so either
 ** kind of [sqlite3_value] object can be used with this interface.
 **
+** ^The sqlite3_result_pointer(C,P) interface sets the result to an
+** SQL NULL value, just like [sqlite3_result_null(C)], except that it
+** also associates the host-language pointer P with that NULL value such
+** that the pointer can be retrieved within an
+** [application-defined SQL function] using [sqlite3_value_pointer()].
+** This mechanism can be used to pass non-SQL values between
+** application-defined functions.
+**
 ** If these routines are called from within the different thread
 ** than the one containing the application-defined function that received
 ** the [sqlite3_context] pointer, the results are undefined.
@@ -5210,6 +5230,7 @@
 SQLITE_API void sqlite3_result_text16le(sqlite3_context*, const void*, int,void(*)(void*));
 SQLITE_API void sqlite3_result_text16be(sqlite3_context*, const void*, int,void(*)(void*));
 SQLITE_API void sqlite3_result_value(sqlite3_context*, sqlite3_value*);
+SQLITE_API void sqlite3_result_pointer(sqlite3_context*, void*);
 SQLITE_API void sqlite3_result_zeroblob(sqlite3_context*, int n);
 SQLITE_API int sqlite3_result_zeroblob64(sqlite3_context*, sqlite3_uint64 n);
 
@@ -18089,6 +18110,7 @@
     double r;           /* Real value used when MEM_Real is set in flags */
     i64 i;              /* Integer value used when MEM_Int is set in flags */
     int nZero;          /* Used when bit MEM_Zero is set in flags */
+    void *pPtr;         /* Pointer when flags=MEM_NULL and eSubtype='p' */
     FuncDef *pDef;      /* Used only when flags==MEM_Agg */
     RowSet *pRowSet;    /* Used only when flags==MEM_RowSet */
     VdbeFrame *pFrame;  /* Used when flags==MEM_Frame */
@@ -18374,6 +18396,7 @@
 #else
 SQLITE_PRIVATE   void sqlite3VdbeMemSetDouble(Mem*, double);
 #endif
+SQLITE_PRIVATE void sqlite3VdbeMemSetPointer(Mem*, void*);
 SQLITE_PRIVATE void sqlite3VdbeMemInit(Mem*,sqlite3*,u16);
 SQLITE_PRIVATE void sqlite3VdbeMemSetNull(Mem*);
 SQLITE_PRIVATE void sqlite3VdbeMemSetZeroBlob(Mem*,int);
@@ -70280,6 +70303,17 @@
   }
 }
 
+/*
+** Set the value stored in *pMem should already be a NULL.
+** Also store a pointer to go with it.
+*/
+SQLITE_PRIVATE void sqlite3VdbeMemSetPointer(Mem *pMem, void *pPtr){
+  assert( pMem->flags==MEM_Null );
+  pMem->flags = MEM_Null|MEM_Subtype;
+  pMem->u.pPtr = pPtr;
+  pMem->eSubtype = 'p';
+}
+
 #ifndef SQLITE_OMIT_FLOATING_POINT
 /*
 ** Delete any previous value and set the value stored in *pMem to val,
@@ -76159,6 +76193,14 @@
   Mem *pMem = (Mem*)pVal;
   return ((pMem->flags & MEM_Subtype) ? pMem->eSubtype : 0);
 }
+SQLITE_API void *sqlite3_value_pointer(sqlite3_value *pVal){
+  Mem *p = (Mem*)pVal;
+  if( (p->flags & MEM_TypeMask)==(MEM_Null|MEM_Subtype) && p->eSubtype=='p' ){
+    return p->u.pPtr;
+  }else{
+    return 0;
+  }
+}
 SQLITE_API const unsigned char *sqlite3_value_text(sqlite3_value *pVal){
   return (const unsigned char *)sqlite3ValueText(pVal, SQLITE_UTF8);
 }
@@ -76337,6 +76379,12 @@
   assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
   sqlite3VdbeMemSetNull(pCtx->pOut);
 }
+SQLITE_API void sqlite3_result_pointer(sqlite3_context *pCtx, void *pPtr){
+  Mem *pOut = pCtx->pOut;
+  assert( sqlite3_mutex_held(pOut->db->mutex) );
+  sqlite3VdbeMemSetNull(pOut);
+  sqlite3VdbeMemSetPointer(pOut, pPtr);
+}
 SQLITE_API void sqlite3_result_subtype(sqlite3_context *pCtx, unsigned int eSubtype){
   Mem *pOut = pCtx->pOut;
   assert( sqlite3_mutex_held(pOut->db->mutex) );
@@ -77322,6 +77370,16 @@
   }
   return rc;
 }
+SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt *pStmt, int i, void *pPtr){
+  int rc;
+  Vdbe *p = (Vdbe*)pStmt;
+  rc = vdbeUnbind(p, i);
+  if( rc==SQLITE_OK ){
+    sqlite3VdbeMemSetPointer(&p->aVar[i-1], pPtr);
+    sqlite3_mutex_leave(p->db->mutex);
+  }
+  return rc;
+}
 SQLITE_API int sqlite3_bind_text( 
   sqlite3_stmt *pStmt, 
   int i, 
@@ -148237,9 +148295,8 @@
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
@@ -148451,17 +148508,13 @@
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
