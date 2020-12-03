--- files/before/ccm.c	2020-12-03 05:28:25.235826206 +0530
+++ files/after/ccm.c	2020-12-03 05:28:26.215982916 +0530
@@ -45,6 +45,7 @@
 
 struct crypto_ccm_req_priv_ctx {
 	u8 odata[16];
+	u8 idata[16];
 	u8 auth_tag[16];
 	u32 flags;
 	struct scatterlist src[3];
@@ -183,8 +184,8 @@
 	AHASH_REQUEST_ON_STACK(ahreq, ctx->mac);
 	unsigned int assoclen = req->assoclen;
 	struct scatterlist sg[3];
-	u8 odata[16];
-	u8 idata[16];
+	u8 *odata = pctx->odata;
+	u8 *idata = pctx->idata;
 	int ilen, err;
 
 	/* format control data for input */
