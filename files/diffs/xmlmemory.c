--- files/before/xmlmemory.c	2020-12-03 05:33:29.024396746 +0530
+++ files/after/xmlmemory.c	2020-12-03 05:33:29.968547699 +0530
@@ -172,6 +172,13 @@
 
     TEST_POINT
 
+    if (size > (MAX_SIZE_T - RESERVE_SIZE)) {
+	xmlGenericError(xmlGenericErrorContext,
+		"xmlMallocLoc : Unsigned overflow\n");
+	xmlMemoryDump();
+	return(NULL);
+    }
+
     p = (MEMHDR *) malloc(RESERVE_SIZE+size);
 
     if (!p) {
@@ -243,7 +250,7 @@
 
     if (size > (MAX_SIZE_T - RESERVE_SIZE)) {
 	xmlGenericError(xmlGenericErrorContext,
-		"xmlMallocAtomicLoc : Unsigned overflow prevented\n");
+		"xmlMallocAtomicLoc : Unsigned overflow\n");
 	xmlMemoryDump();
 	return(NULL);
     }
@@ -352,6 +359,13 @@
 #endif
     xmlMutexUnlock(xmlMemMutex);
 
+    if (size > (MAX_SIZE_T - RESERVE_SIZE)) {
+	xmlGenericError(xmlGenericErrorContext,
+		"xmlReallocLoc : Unsigned overflow\n");
+	xmlMemoryDump();
+	return(NULL);
+    }
+
     tmp = (MEMHDR *) realloc(p,RESERVE_SIZE+size);
     if (!tmp) {
 	 free(p);
@@ -499,6 +513,13 @@
     if (!xmlMemInitialized) xmlInitMemory();
     TEST_POINT
 
+    if (size > (MAX_SIZE_T - RESERVE_SIZE)) {
+	xmlGenericError(xmlGenericErrorContext,
+		"xmlMemStrdupLoc : Unsigned overflow\n");
+	xmlMemoryDump();
+	return(NULL);
+    }
+
     p = (MEMHDR *) malloc(RESERVE_SIZE+size);
     if (!p) {
       goto error;
