--- files/before/rdpsnd.c	2020-12-03 05:30:08.776380550 +0530
+++ files/after/rdpsnd.c	2020-12-03 05:30:09.680525107 +0530
@@ -269,6 +269,12 @@
 	uint16 tick;
 	uint16 packsize;
 	STREAM out;
+	struct stream packet = *in;
+
+	if (!s_check_rem(in, 4))
+	{
+		rdp_protocol_error("rdpsnd_process_training(), consume of training data from stream would overrun", &packet);
+	}
 
 	in_uint16_le(in, tick);
 	in_uint16_le(in, packsize);
@@ -445,6 +451,11 @@
 	static char *rest = NULL;
 	char *buf;
 
+	if (!s_check(s))
+	{
+		rdp_protocol_error("rdpsnddbg_process(), stream is in unstable state", s);
+	}
+
 	pkglen = s->end - s->p;
 	/* str_handle_lines requires null terminated strings */
 	buf = (char *) xmalloc(pkglen + 1);
