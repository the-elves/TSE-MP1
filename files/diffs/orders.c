--- files/before/orders.c	2020-12-03 05:30:03.363515130 +0530
+++ files/after/orders.c	2020-12-03 05:30:04.239655208 +0530
@@ -1259,11 +1259,17 @@
 	uint16 flags;
 	uint8 type;
 	uint8 *next_order;
+	struct stream packet = *s;
 
 	in_uint16_le(s, length);
 	in_uint16_le(s, flags);	/* used by bmpcache2 */
 	in_uint8(s, type);
 
+	if (!s_check_rem(s, length + 7))
+	{
+		rdp_protocol_error("process_secondary_order(), next order pointer would overrun stream", &packet);
+	}
+
 	next_order = s->p + (sint16) length + 7;
 
 	switch (type)
