--- files/before/mcs.c	2020-12-03 05:30:01.607234326 +0530
+++ files/after/mcs.c	2020-12-03 05:30:02.495376325 +0530
@@ -45,9 +45,16 @@
 static RD_BOOL
 mcs_parse_domain_params(STREAM s)
 {
-	int length;
+	uint32 length;
+	struct stream packet = *s;
 
 	ber_parse_header(s, MCS_TAG_DOMAIN_PARAMS, &length);
+
+	if (!s_check_rem(s, length))
+	{
+		rdp_protocol_error("mcs_parse_domain_params(), consume domain params from stream would overrun", &packet);
+	}
+
 	in_uint8s(s, length);
 
 	return s_check(s);
@@ -89,8 +96,9 @@
 {
 	UNUSED(mcs_data);
 	uint8 result;
-	int length;
+	uint32 length;
 	STREAM s;
+	struct stream packet;
 	RD_BOOL is_fastpath;
 	uint8 fastpath_hdr;
 
@@ -99,6 +107,8 @@
 
 	if (s == NULL)
 		return False;
+	
+	packet = *s;
 
 	ber_parse_header(s, MCS_CONNECT_RESPONSE, &length);
 
@@ -112,6 +122,12 @@
 
 	ber_parse_header(s, BER_TAG_INTEGER, &length);
 	in_uint8s(s, length);	/* connect id */
+
+	if (!s_check_rem(s, length))
+	{
+		rdp_protocol_error("mcs_recv_connect_response(), consume connect id from stream would overrun", &packet);
+	}
+
 	mcs_parse_domain_params(s);
 
 	ber_parse_header(s, BER_TAG_OCTET_STRING, &length);
