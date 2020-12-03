--- files/before/vdbemem.c	2020-12-03 05:32:58.023440230 +0530
+++ files/after/vdbemem.c	2020-12-03 05:32:59.575688406 +0530
@@ -697,6 +697,17 @@
   }
 }
 
+/*
+** Set the value stored in *pMem should already be a NULL.
+** Also store a pointer to go with it.
+*/
+void sqlite3VdbeMemSetPointer(Mem *pMem, void *pPtr){
+  assert( pMem->flags==MEM_Null );
+  pMem->flags = MEM_Null|MEM_Subtype;
+  pMem->u.pPtr = pPtr;
+  pMem->eSubtype = 'p';
+}
+
 #ifndef SQLITE_OMIT_FLOATING_POINT
 /*
 ** Delete any previous value and set the value stored in *pMem to val,
