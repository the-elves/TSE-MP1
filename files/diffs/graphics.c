--- files/before/graphics.c	2020-12-03 05:29:23.913207703 +0530
+++ files/after/graphics.c	2020-12-03 05:29:24.797349062 +0530
@@ -141,9 +141,17 @@
 {
 	UINT32 SrcSize = length;
 	rdpGdi* gdi = context->gdi;
+	UINT32 size = DstWidth * DstHeight;
 	bitmap->compressed = FALSE;
 	bitmap->format = gdi->dstFormat;
-	bitmap->length = DstWidth * DstHeight * GetBytesPerPixel(bitmap->format);
+
+	if ((GetBytesPerPixel(bitmap->format) == 0) ||
+	    (DstWidth == 0) || (DstHeight == 0) || (DstWidth > UINT32_MAX / DstHeight) ||
+	    (size > (UINT32_MAX / GetBytesPerPixel(bitmap->format))))
+		return FALSE;
+
+	size *= GetBytesPerPixel(bitmap->format);
+	bitmap->length = size;
 	bitmap->data = (BYTE*) _aligned_malloc(bitmap->length, 16);
 
 	if (!bitmap->data)
