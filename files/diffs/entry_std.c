--- files/before/entry_std.c	2020-12-03 05:29:46.732856171 +0530
+++ files/after/entry_std.c	2020-12-03 05:29:47.628999448 +0530
@@ -101,6 +101,7 @@
 static TEE_Result set_rmem_param(const struct optee_msg_param_rmem *rmem,
 				 struct param_mem *mem)
 {
+	size_t req_size = 0;
 	uint64_t shm_ref = READ_ONCE(rmem->shm_ref);
 
 	mem->mobj = mobj_reg_shm_get_by_cookie(shm_ref);
@@ -110,6 +111,14 @@
 	mem->offs = READ_ONCE(rmem->offs);
 	mem->size = READ_ONCE(rmem->size);
 
+	/*
+	 * Check that the supplied offset and size is covered by the
+	 * previously verified MOBJ.
+	 */
+	if (ADD_OVERFLOW(mem->offs, mem->size, &req_size) ||
+	    mem->mobj->size < req_size)
+		return TEE_ERROR_SECURITY;
+
 	return TEE_SUCCESS;
 }
 
