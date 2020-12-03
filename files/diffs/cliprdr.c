--- files/before/cliprdr.c	2020-12-03 05:29:56.274381697 +0530
+++ files/after/cliprdr.c	2020-12-03 05:29:57.154522416 +0530
@@ -118,6 +118,7 @@
 	uint16 type, status;
 	uint32 length, format;
 	uint8 *data;
+	struct stream packet = *s;
 
 	in_uint16_le(s, type);
 	in_uint16_le(s, status);
@@ -127,6 +128,11 @@
 	logger(Clipboard, Debug, "cliprdr_process(), type=%d, status=%d, length=%d", type, status,
 	       length);
 
+	if (!s_check_rem(s, length))
+	{
+		rdp_protocol_error("cliprdr_process(), consume of packet from stream would overrun", &packet);
+	}
+
 	if (status == CLIPRDR_ERROR)
 	{
 		switch (type)
