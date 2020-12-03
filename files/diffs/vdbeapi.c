--- files/before/vdbeapi.c	2020-12-03 05:32:56.043123610 +0530
+++ files/after/vdbeapi.c	2020-12-03 05:32:57.059286076 +0530
@@ -198,6 +198,14 @@
   Mem *pMem = (Mem*)pVal;
   return ((pMem->flags & MEM_Subtype) ? pMem->eSubtype : 0);
 }
+void *sqlite3_value_pointer(sqlite3_value *pVal){
+  Mem *p = (Mem*)pVal;
+  if( (p->flags & MEM_TypeMask)==(MEM_Null|MEM_Subtype) && p->eSubtype=='p' ){
+    return p->u.pPtr;
+  }else{
+    return 0;
+  }
+}
 const unsigned char *sqlite3_value_text(sqlite3_value *pVal){
   return (const unsigned char *)sqlite3ValueText(pVal, SQLITE_UTF8);
 }
@@ -376,6 +384,12 @@
   assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
   sqlite3VdbeMemSetNull(pCtx->pOut);
 }
+void sqlite3_result_pointer(sqlite3_context *pCtx, void *pPtr){
+  Mem *pOut = pCtx->pOut;
+  assert( sqlite3_mutex_held(pOut->db->mutex) );
+  sqlite3VdbeMemSetNull(pOut);
+  sqlite3VdbeMemSetPointer(pOut, pPtr);
+}
 void sqlite3_result_subtype(sqlite3_context *pCtx, unsigned int eSubtype){
   Mem *pOut = pCtx->pOut;
   assert( sqlite3_mutex_held(pOut->db->mutex) );
@@ -1360,6 +1374,16 @@
     sqlite3_mutex_leave(p->db->mutex);
   }
   return rc;
+}
+int sqlite3_bind_pointer(sqlite3_stmt *pStmt, int i, void *pPtr){
+  int rc;
+  Vdbe *p = (Vdbe*)pStmt;
+  rc = vdbeUnbind(p, i);
+  if( rc==SQLITE_OK ){
+    sqlite3VdbeMemSetPointer(&p->aVar[i-1], pPtr);
+    sqlite3_mutex_leave(p->db->mutex);
+  }
+  return rc;
 }
 int sqlite3_bind_text( 
   sqlite3_stmt *pStmt, 
