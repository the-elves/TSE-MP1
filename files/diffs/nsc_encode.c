--- files/before/nsc_encode.c	2020-12-03 05:29:20.384643548 +0530
+++ files/after/nsc_encode.c	2020-12-03 05:29:21.296789384 +0530
@@ -51,6 +51,7 @@
 		for (i = 0; i < 5; i++)
 		{
 			BYTE* tmp = (BYTE*) realloc(context->priv->PlaneBuffers[i], length);
+
 			if (!tmp)
 				goto fail;
 
@@ -87,7 +88,7 @@
 	return FALSE;
 }
 
-static void nsc_encode_argb_to_aycocg(NSC_CONTEXT* context, const BYTE* data,
+static BOOL nsc_encode_argb_to_aycocg(NSC_CONTEXT* context, const BYTE* data,
                                       UINT32 scanline)
 {
 	UINT16 x;
@@ -104,10 +105,20 @@
 	INT16 b_val;
 	BYTE a_val;
 	UINT32 tempWidth;
+
+	if (!context || data || (scanline == 0))
+		return FALSE;
+
 	tempWidth = ROUND_UP_TO(context->width, 8);
 	rw = (context->ChromaSubsamplingLevel ? tempWidth : context->width);
 	ccl = context->ColorLossLevel;
 
+	if (context->priv->PlaneBuffersLength < rw * scanline)
+		return FALSE;
+
+	if (rw < scanline * 2)
+		return FALSE;
+
 	for (y = 0; y < context->height; y++)
 	{
 		src = data + (context->height - 1 - y) * scanline;
@@ -242,31 +253,37 @@
 		CopyMemory(coplane, coplane - rw, rw);
 		CopyMemory(cgplane, cgplane - rw, rw);
 	}
+
+	return TRUE;
 }
 
-static void nsc_encode_subsampling(NSC_CONTEXT* context)
+static BOOL nsc_encode_subsampling(NSC_CONTEXT* context)
 {
 	UINT16 x;
 	UINT16 y;
-	BYTE* co_dst;
-	BYTE* cg_dst;
-	INT8* co_src0;
-	INT8* co_src1;
-	INT8* cg_src0;
-	INT8* cg_src1;
 	UINT32 tempWidth;
 	UINT32 tempHeight;
+
+	if (!context)
+		return FALSE;
+
 	tempWidth = ROUND_UP_TO(context->width, 8);
 	tempHeight = ROUND_UP_TO(context->height, 2);
 
+	if (tempHeight == 0)
+		return FALSE;
+
+	if (tempWidth > context->priv->PlaneBuffersLength / tempHeight)
+		return FALSE;
+
 	for (y = 0; y < tempHeight >> 1; y++)
 	{
-		co_dst = context->priv->PlaneBuffers[1] + y * (tempWidth >> 1);
-		cg_dst = context->priv->PlaneBuffers[2] + y * (tempWidth >> 1);
-		co_src0 = (INT8*) context->priv->PlaneBuffers[1] + (y << 1) * tempWidth;
-		co_src1 = co_src0 + tempWidth;
-		cg_src0 = (INT8*) context->priv->PlaneBuffers[2] + (y << 1) * tempWidth;
-		cg_src1 = cg_src0 + tempWidth;
+		BYTE* co_dst = context->priv->PlaneBuffers[1] + y * (tempWidth >> 1);
+		BYTE* cg_dst = context->priv->PlaneBuffers[2] + y * (tempWidth >> 1);
+		const INT8* co_src0 = (INT8*) context->priv->PlaneBuffers[1] + (y << 1) * tempWidth;
+		const INT8* co_src1 = co_src0 + tempWidth;
+		const INT8* cg_src0 = (INT8*) context->priv->PlaneBuffers[2] + (y << 1) * tempWidth;
+		const INT8* cg_src1 = cg_src0 + tempWidth;
 
 		for (x = 0; x < tempWidth >> 1; x++)
 		{
@@ -280,19 +297,28 @@
 			cg_src1 += 2;
 		}
 	}
+
+	return TRUE;
 }
 
-void nsc_encode(NSC_CONTEXT* context, const BYTE* bmpdata, UINT32 rowstride)
+BOOL nsc_encode(NSC_CONTEXT* context, const BYTE* bmpdata, UINT32 rowstride)
 {
-	nsc_encode_argb_to_aycocg(context, bmpdata, rowstride);
+	if (!context || !bmpdata || (rowstride == 0))
+		return FALSE;
+
+	if (!nsc_encode_argb_to_aycocg(context, bmpdata, rowstride))
+		return FALSE;
 
 	if (context->ChromaSubsamplingLevel)
 	{
-		nsc_encode_subsampling(context);
+		if (!nsc_encode_subsampling(context))
+			return FALSE;
 	}
+
+	return TRUE;
 }
 
-static UINT32 nsc_rle_encode(BYTE* in, BYTE* out, UINT32 originalSize)
+static UINT32 nsc_rle_encode(const BYTE* in, BYTE* out, UINT32 originalSize)
 {
 	UINT32 left;
 	UINT32 runlength = 1;
