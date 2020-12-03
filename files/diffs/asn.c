--- files/before/asn.c	2020-12-03 05:29:52.681807310 +0530
+++ files/after/asn.c	2020-12-03 05:29:53.557947389 +0530
@@ -22,7 +22,7 @@
 
 /* Parse an ASN.1 BER header */
 RD_BOOL
-ber_parse_header(STREAM s, int tagval, int *length)
+ber_parse_header(STREAM s, int tagval, uint32 *length)
 {
 	int tag, len;
 
