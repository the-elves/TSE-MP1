--- files/before/secure.c	2020-12-03 05:30:12.520979246 +0530
+++ files/after/secure.c	2020-12-03 05:30:13.417122524 +0530
@@ -296,6 +296,9 @@
 void
 sec_decrypt(uint8 * data, int length)
 {
+	if (length <= 0)
+		return;
+
 	if (g_sec_decrypt_use_count == 4096)
 	{
 		sec_update(g_sec_decrypt_key, g_sec_decrypt_update_key);
@@ -848,9 +851,11 @@
 	uint16 sec_flags;
 	uint16 channel;
 	STREAM s;
+	struct stream packet;
 
 	while ((s = mcs_recv(&channel, is_fastpath, &fastpath_hdr)) != NULL)
 	{
+		packet = *s;
 		if (*is_fastpath == True)
 		{
 			/* If fastpath packet is encrypted, read data
@@ -859,6 +864,10 @@
 			fastpath_flags = (fastpath_hdr & 0xC0) >> 6;
 			if (fastpath_flags & FASTPATH_OUTPUT_ENCRYPTED)
 			{
+				if (!s_check_rem(s, 8)) {
+					rdp_protocol_error("sec_recv(), consume fastpath signature from stream would overrun", &packet);
+				}
+
 				in_uint8s(s, 8);	/* signature */
 				sec_decrypt(s->p, s->end - s->p);
 			}
@@ -875,6 +884,10 @@
 			{
 				if (sec_flags & SEC_ENCRYPT)
 				{
+					if (!s_check_rem(s, 8)) {
+						rdp_protocol_error("sec_recv(), consume encrypt signature from stream would overrun", &packet);
+					}
+
 					in_uint8s(s, 8);	/* signature */
 					sec_decrypt(s->p, s->end - s->p);
 				}
@@ -889,6 +902,10 @@
 				{
 					uint8 swapbyte;
 
+					if (!s_check_rem(s, 8)) {
+						rdp_protocol_error("sec_recv(), consume redirect signature from stream would overrun", &packet);
+					}
+
 					in_uint8s(s, 8);	/* signature */
 					sec_decrypt(s->p, s->end - s->p);
 
