--- files/before/seamless.c	2020-12-03 05:30:10.720691412 +0530
+++ files/after/seamless.c	2020-12-03 05:30:11.628836608 +0530
@@ -168,6 +168,12 @@
 
 			icon_buf[len] = strtol(byte, NULL, 16);
 			len++;
+
+			if ((size_t)len >= sizeof(icon_buf))
+			{
+				logger(Protocol, Warning, "seamless_process_line(), icon data would overrun icon_buf");
+				break;
+			}
 		}
 
 		ui_seamless_seticon(id, tok5, width, height, chunk, icon_buf, len);
@@ -370,6 +376,12 @@
 {
 	unsigned int pkglen;
 	char *buf;
+	struct stream packet = *s;
+
+	if (!s_check(s))
+	{
+		rdp_protocol_error("seamless_process(), stream is in unstable state", &packet);
+	}
 
 	pkglen = s->end - s->p;
 	/* str_handle_lines requires null terminated strings */
