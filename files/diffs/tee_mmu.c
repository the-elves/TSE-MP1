--- files/before/tee_mmu.c	2020-12-03 05:29:44.944570255 +0530
+++ files/after/tee_mmu.c	2020-12-03 05:29:45.844714172 +0530
@@ -757,10 +757,11 @@
 				       size_t len)
 {
 	uaddr_t a;
+	uaddr_t end_addr = 0;
 	size_t addr_incr = MIN(CORE_MMU_USER_CODE_SIZE,
 			       CORE_MMU_USER_PARAM_SIZE);
 
-	if (ADD_OVERFLOW(uaddr, len, &a))
+	if (ADD_OVERFLOW(uaddr, len, &end_addr))
 		return TEE_ERROR_ACCESS_DENIED;
 
 	if ((flags & TEE_MEMORY_ACCESS_NONSECURE) &&
@@ -775,7 +776,7 @@
 	   !tee_mmu_is_vbuf_inside_ta_private(utc, (void *)uaddr, len))
 		return TEE_ERROR_ACCESS_DENIED;
 
-	for (a = uaddr; a < (uaddr + len); a += addr_incr) {
+	for (a = ROUNDDOWN(uaddr, addr_incr); a < end_addr; a += addr_incr) {
 		uint32_t attr;
 		TEE_Result res;
 
