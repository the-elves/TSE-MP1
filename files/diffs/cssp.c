--- files/before/cssp.c	2020-12-03 05:29:58.058666972 +0530
+++ files/after/cssp.c	2020-12-03 05:29:58.974813449 +0530
@@ -595,6 +595,7 @@
 	STREAM s;
 	int length;
 	int tagval;
+	struct stream packet;
 
 	s = tcp_recv(NULL, 4);
 
@@ -622,6 +623,7 @@
 
 	// receive the remainings of message
 	s = tcp_recv(s, length);
+	packet = *s;
 
 	// parse the response and into nego token
 	if (!ber_in_header(s, &tagval, &length) ||
@@ -632,6 +634,12 @@
 	if (!ber_in_header(s, &tagval, &length) ||
 	    tagval != (BER_TAG_CTXT_SPECIFIC | BER_TAG_CONSTRUCTED | 0))
 		return False;
+
+	if (!s_check_rem(s, length))
+	{
+		 rdp_protocol_error("cssp_read_tsrequest(), consume of version from stream would overrun",
+				    &packet);
+	}
 	in_uint8s(s, length);
 
 	// negoToken [1]
@@ -653,7 +661,14 @@
 		if (!ber_in_header(s, &tagval, &length) || tagval != BER_TAG_OCTET_STRING)
 			return False;
 
-		token->end = token->p = token->data;
+		if (!s_check_rem(s, length))
+		{
+			rdp_protocol_error("cssp_read_tsrequest(), consume of token from stream would overrun",
+					   &packet);
+		}
+
+		s_realloc(token, length);
+		s_reset(token);
 		out_uint8p(token, s->p, length);
 		s_mark_end(token);
 	}
