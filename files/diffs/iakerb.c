--- files/before/iakerb.c	2020-12-03 05:27:30.663100970 +0530
+++ files/after/iakerb.c	2020-12-03 05:27:31.675262797 +0530
@@ -1057,7 +1057,7 @@
                               gss_buffer_t interprocess_token)
 {
     OM_uint32 maj;
-    iakerb_ctx_id_t ctx = (iakerb_ctx_id_t)context_handle;
+    iakerb_ctx_id_t ctx = (iakerb_ctx_id_t)*context_handle;
 
     /* We don't currently support exporting partially established contexts. */
     if (!ctx->established)
@@ -1072,13 +1072,41 @@
     return maj;
 }
 
-/*
- * Until we implement partial context exports, there are no IAKERB exported
- * context tokens, only tokens for the underlying krb5 context.  So we do not
- * need to implement an iakerb_gss_import_sec_context() yet; it would be
- * unreachable except via a manually constructed token.
- */
+OM_uint32 KRB5_CALLCONV
+iakerb_gss_import_sec_context(OM_uint32 *minor_status,
+                              gss_buffer_t interprocess_token,
+                              gss_ctx_id_t *context_handle)
+{
+    OM_uint32 maj, tmpmin;
+    krb5_error_code code;
+    gss_ctx_id_t gssc;
+    krb5_gss_ctx_id_t kctx;
+    iakerb_ctx_id_t ctx;
+
+    maj = krb5_gss_import_sec_context(minor_status, interprocess_token, &gssc);
+    if (maj != GSS_S_COMPLETE)
+        return maj;
+    kctx = (krb5_gss_ctx_id_t)gssc;
+
+    if (!kctx->established) {
+        /* We don't currently support importing partially established
+         * contexts. */
+        krb5_gss_delete_sec_context(&tmpmin, &gssc, GSS_C_NO_BUFFER);
+        return GSS_S_FAILURE;
+    }
+
+    code = iakerb_alloc_context(&ctx, kctx->initiate);
+    if (code != 0) {
+        krb5_gss_delete_sec_context(&tmpmin, &gssc, GSS_C_NO_BUFFER);
+        *minor_status = code;
+        return GSS_S_FAILURE;
+    }
 
+    ctx->gssc = gssc;
+    ctx->established = 1;
+    *context_handle = (gss_ctx_id_t)ctx;
+    return GSS_S_COMPLETE;
+}
 #endif /* LEAN_CLIENT */
 
 OM_uint32 KRB5_CALLCONV
