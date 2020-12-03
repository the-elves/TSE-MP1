--- files/before/tee_svc.c	2020-12-03 05:29:43.184288816 +0530
+++ files/after/tee_svc.c	2020-12-03 05:29:44.072430815 +0530
@@ -494,7 +494,9 @@
 	return res;
 }
 
-static void utee_param_to_param(struct tee_ta_param *p, struct utee_params *up)
+static TEE_Result utee_param_to_param(struct user_ta_ctx *utc,
+				      struct tee_ta_param *p,
+				      struct utee_params *up)
 {
 	size_t n;
 	uint32_t types = up->types;
@@ -503,14 +505,20 @@
 	for (n = 0; n < TEE_NUM_PARAMS; n++) {
 		uintptr_t a = up->vals[n * 2];
 		size_t b = up->vals[n * 2 + 1];
+		uint32_t flags = TEE_MEMORY_ACCESS_READ |
+				 TEE_MEMORY_ACCESS_ANY_OWNER;
 
 		switch (TEE_PARAM_TYPE_GET(types, n)) {
-		case TEE_PARAM_TYPE_MEMREF_INPUT:
 		case TEE_PARAM_TYPE_MEMREF_OUTPUT:
 		case TEE_PARAM_TYPE_MEMREF_INOUT:
+			flags |= TEE_MEMORY_ACCESS_WRITE;
+			/*FALLTHROUGH*/
+		case TEE_PARAM_TYPE_MEMREF_INPUT:
 			p->u[n].mem.mobj = &mobj_virt;
 			p->u[n].mem.offs = a;
 			p->u[n].mem.size = b;
+			if (tee_mmu_check_access_rights(utc, flags, a, b))
+				return TEE_ERROR_ACCESS_DENIED;
 			break;
 		case TEE_PARAM_TYPE_VALUE_INPUT:
 		case TEE_PARAM_TYPE_VALUE_INOUT:
@@ -522,6 +530,8 @@
 			break;
 		}
 	}
+
+	return TEE_SUCCESS;
 }
 
 static TEE_Result alloc_temp_sec_mem(size_t size, struct mobj **mobj,
@@ -575,7 +585,9 @@
 			(uaddr_t)callee_params, sizeof(struct utee_params));
 		if (res != TEE_SUCCESS)
 			return res;
-		utee_param_to_param(param, callee_params);
+		res = utee_param_to_param(utc, param, callee_params);
+		if (res != TEE_SUCCESS)
+			return res;
 	}
 
 	if (called_sess && is_pseudo_ta_ctx(called_sess->ctx)) {
