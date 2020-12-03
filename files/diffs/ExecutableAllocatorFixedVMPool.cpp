--- files/before/ExecutableAllocatorFixedVMPool.cpp	2020-12-03 05:30:26.331187262 +0530
+++ files/after/ExecutableAllocatorFixedVMPool.cpp	2020-12-03 05:30:27.319345250 +0530
@@ -36,6 +36,10 @@
 #include <wtf/PageReservation.h>
 #include <wtf/VMTags.h>
 
+#if OS(DARWIN)
+#include <sys/mman.h>
+#endif
+
 #if OS(LINUX)
 #include <stdio.h>
 #endif
