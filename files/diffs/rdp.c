--- files/before/rdp.c	2020-12-03 05:30:05.151801044 +0530
+++ files/after/rdp.c	2020-12-03 05:30:06.059946240 +0530
@@ -285,6 +285,19 @@
 	size_t ibl, obl;
 	char *pin, *pout;
 
+	struct stream packet = *s;
+
+	if ((in_len < 0) || ((uint32)in_len >= (RD_UINT32_MAX / 2)))
+	{
+		logger(Protocol, Error, "rdp_in_unistr(), length of unicode data is out of bounds.");
+		abort();
+	}
+
+	if (!s_check_rem(s, in_len))
+	{
+		rdp_protocol_error("rdp_in_unistr(), consume of unicode data from stream would overrun", &packet);
+	}
+
 	// if not already open
 	if (!icv_utf16_to_local)
 	{
@@ -1211,6 +1224,7 @@
 {
 	uint8 type;
 	uint16 len_src_descriptor, len_combined_caps;
+	struct stream packet = *s;
 
 	/* at this point we need to ensure that we have ui created */
 	rd_create_ui();
@@ -1218,6 +1232,11 @@
 	in_uint32_le(s, g_rdp_shareid);
 	in_uint16_le(s, len_src_descriptor);
 	in_uint16_le(s, len_combined_caps);
+
+	if (!s_check_rem(s, len_src_descriptor))
+	{
+		rdp_protocol_error("rdp_demand_active(), consume of source descriptor from stream would overrun", &packet);
+	}
 	in_uint8s(s, len_src_descriptor);
 
 	logger(Protocol, Debug, "process_demand_active(), shareid=0x%x", g_rdp_shareid);
@@ -1390,78 +1409,113 @@
 	}
 }
 
-/* Process bitmap updates */
-void
-process_bitmap_updates(STREAM s)
+/* Process TS_BITMAP_DATA */
+static void
+process_bitmap_data(STREAM s)
 {
-	uint16 num_updates;
 	uint16 left, top, right, bottom, width, height;
-	uint16 cx, cy, bpp, Bpp, compress, bufsize, size;
+	uint16 cx, cy, bpp, Bpp, flags, bufsize, size;
 	uint8 *data, *bmpdata;
-	int i;
-
+	
 	logger(Protocol, Debug, "%s()", __func__);
 
-	in_uint16_le(s, num_updates);
+	struct stream packet = *s;
 
-	for (i = 0; i < num_updates; i++)
-	{
-		in_uint16_le(s, left);
-		in_uint16_le(s, top);
-		in_uint16_le(s, right);
-		in_uint16_le(s, bottom);
-		in_uint16_le(s, width);
-		in_uint16_le(s, height);
-		in_uint16_le(s, bpp);
-		Bpp = (bpp + 7) / 8;
-		in_uint16_le(s, compress);
-		in_uint16_le(s, bufsize);
-
-		cx = right - left + 1;
-		cy = bottom - top + 1;
-
-		logger(Graphics, Debug,
-		       "process_bitmap_updates(), [%d,%d,%d,%d], [%d,%d], bpp=%d, compression=%d",
-		       left, top, right, bottom, width, height, Bpp, compress);
+	in_uint16_le(s, left); /* destLeft */
+	in_uint16_le(s, top); /* destTop */
+	in_uint16_le(s, right); /* destRight */
+	in_uint16_le(s, bottom); /* destBottom */
+	in_uint16_le(s, width); /* width */
+	in_uint16_le(s, height); /* height */
+	in_uint16_le(s, bpp); /*bitsPerPixel */
+	Bpp = (bpp + 7) / 8;
+	in_uint16_le(s, flags); /* flags */
+	in_uint16_le(s, bufsize); /* bitmapLength */
 
-		if (!compress)
-		{
-			int y;
-			bmpdata = (uint8 *) xmalloc(width * height * Bpp);
-			for (y = 0; y < height; y++)
-			{
-				in_uint8a(s, &bmpdata[(height - y - 1) * (width * Bpp)],
-					  width * Bpp);
-			}
-			ui_paint_bitmap(left, top, cx, cy, width, height, bmpdata);
-			xfree(bmpdata);
-			continue;
-		}
+	cx = right - left + 1;
+	cy = bottom - top + 1;
 
+	/* FIXME: There are a assumtion that we do not consider in
+		this code. The value of bpp is not passed to
+		ui_paint_bitmap() which relies on g_server_bpp for drawing
+		the bitmap data.
 
-		if (compress & 0x400)
-		{
-			size = bufsize;
-		}
-		else
-		{
-			in_uint8s(s, 2);	/* pad */
-			in_uint16_le(s, size);
-			in_uint8s(s, 4);	/* line_size, final_size */
-		}
-		in_uint8p(s, data, size);
+		Does this means that we can sanity check bpp with g_server_bpp ?
+	*/
+
+	if (Bpp == 0 || width == 0 || height == 0)
+	{
+        logger(Protocol, Warning, "%s(), [%d,%d,%d,%d], [%d,%d], bpp=%d, flags=%x", __func__,
+				left, top, right, bottom, width, height, bpp, flags);
+		rdp_protocol_error("TS_BITMAP_DATA, unsafe size of bitmap data received from server", &packet);
+	}
+
+	if ((RD_UINT32_MAX / Bpp) <= (width * height))
+	{
+		logger(Protocol, Warning, "%s(), [%d,%d,%d,%d], [%d,%d], bpp=%d, flags=%x", __func__,
+				left, top, right, bottom, width, height, bpp, flags);
+		rdp_protocol_error("TS_BITMAP_DATA, unsafe size of bitmap data received from server", &packet);
+	}
+ 
+	if (flags == 0)
+	{
+		/* read uncompressed bitmap data */
+		int y;
 		bmpdata = (uint8 *) xmalloc(width * height * Bpp);
-		if (bitmap_decompress(bmpdata, width, height, data, size, Bpp))
-		{
-			ui_paint_bitmap(left, top, cx, cy, width, height, bmpdata);
-		}
-		else
+		for (y = 0; y < height; y++)
 		{
-			logger(Graphics, Warning,
-			       "process_bitmap_updates(), failed to decompress bitmap");
+			in_uint8a(s, &bmpdata[(height - y - 1) * (width * Bpp)], width * Bpp);
 		}
-
+		
+		ui_paint_bitmap(left, top, cx, cy, width, height, bmpdata);
 		xfree(bmpdata);
+		return;
+	}
+
+	if (flags & NO_BITMAP_COMPRESSION_HDR)
+	{
+		size = bufsize;
+	}
+	else
+	{
+		/* Read TS_CD_HEADER */
+		in_uint8s(s, 2);        /* skip cbCompFirstRowSize (must be 0x0000) */
+		in_uint16_le(s, size);  /* cbCompMainBodySize */
+		in_uint8s(s, 2);        /* skip cbScanWidth */
+		in_uint8s(s, 2);        /* skip cbUncompressedSize */
+	}
+
+	/* read compressed bitmap data */
+	if (!s_check_rem(s, size))
+	{
+		rdp_protocol_error("process_bitmap_data(), consume of bitmap data from stream would overrun", &packet);
+	}
+	in_uint8p(s, data, size);
+	bmpdata = (uint8 *) xmalloc(width * height * Bpp);
+	if (bitmap_decompress(bmpdata, width, height, data, size, Bpp))
+	{
+		ui_paint_bitmap(left, top, cx, cy, width, height, bmpdata);
+	}
+	else
+	{
+		logger(Protocol, Warning, "%s(), failed to decompress bitmap", __func__);
+	}
+
+	xfree(bmpdata);
+}
+
+/* Process TS_UPDATE_BITMAP_DATA */
+void
+process_bitmap_updates(STREAM s)
+{
+	int i;
+	uint16 num_updates;
+	
+	in_uint16_le(s, num_updates);   /* rectangles */
+
+	for (i = 0; i < num_updates; i++)
+	{
+		process_bitmap_data(s);
 	}
 }
 
@@ -2013,3 +2067,21 @@
 	logger(Protocol, Debug, "%s()", __func__);
 	sec_disconnect();
 }
+
+/* Abort rdesktop upon protocol error
+
+   A protocol error is defined as:
+
+    - A value is outside specified range for example;
+      bpp for a bitmap is not allowed to be greater than the
+      value 32 but is represented by a byte in protocol.
+
+*/
+void
+rdp_protocol_error(const char *message, STREAM s)
+{
+	logger(Protocol, Error, "%s(), %s", __func__, message);
+	if (s)
+		hexdump(s->p, s_length(s));
+	exit(0);
+}
