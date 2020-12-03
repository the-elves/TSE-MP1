--- files/before/rdpdr.c	2020-12-03 05:30:06.980093355 +0530
+++ files/after/rdpdr.c	2020-12-03 05:30:07.868235353 +0530
@@ -854,6 +854,7 @@
 	uint16 vmin;
 	uint16 component;
 	uint16 pakid;
+	struct stream packet = *s;
 
 	logger(Protocol, Debug, "rdpdr_process()");
 	/* hexdump(s->p, s->end - s->p); */
@@ -873,8 +874,18 @@
 				/* DR_CORE_SERVER_ANNOUNCE_REQ */
 				in_uint8s(s, 2);	/* skip versionMajor */
 				in_uint16_le(s, vmin);	/* VersionMinor */
+
 				in_uint32_le(s, g_client_id);	/* ClientID */
 
+				/* g_client_id is sent back to server,
+				   so lets check that we actually got
+				   valid data from stream to prevent
+				   that we leak back data to server */
+				if (!s_check(s))
+				{
+					rdp_protocol_error("rdpdr_process(), consume of g_client_id from stream did overrun", &packet);
+				}
+
 				/* The RDP client is responsibility to provide a random client id
 				   if server version is < 12 */
 				if (vmin < 0x000c)
