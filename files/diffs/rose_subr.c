--- files/before/rose_subr.c	2020-12-03 05:26:31.849697725 +0530
+++ files/after/rose_subr.c	2020-12-03 05:26:32.765844200 +0530
@@ -290,10 +290,15 @@
 				facilities->source_ndigis = 0;
 				facilities->dest_ndigis   = 0;
 				for (pt = p + 2, lg = 0 ; lg < l ; pt += AX25_ADDR_LEN, lg += AX25_ADDR_LEN) {
-					if (pt[6] & AX25_HBIT)
+					if (pt[6] & AX25_HBIT) {
+						if (facilities->dest_ndigis >= ROSE_MAX_DIGIS)
+							return -1;
 						memcpy(&facilities->dest_digis[facilities->dest_ndigis++], pt, AX25_ADDR_LEN);
-					else
+					} else {
+						if (facilities->source_ndigis >= ROSE_MAX_DIGIS)
+							return -1;
 						memcpy(&facilities->source_digis[facilities->source_ndigis++], pt, AX25_ADDR_LEN);
+					}
 				}
 			}
 			p   += l + 2;
@@ -333,6 +338,11 @@
 
 		case 0xC0:
 			l = p[1];
+
+			/* Prevent overflows*/
+			if (l < 10 || l > 20)
+				return -1;
+
 			if (*p == FAC_CCITT_DEST_NSAP) {
 				memcpy(&facilities->source_addr, p + 7, ROSE_ADDR_LEN);
 				memcpy(callsign, p + 12,   l - 10);
@@ -373,12 +383,16 @@
 			switch (*p) {
 			case FAC_NATIONAL:		/* National */
 				len = rose_parse_national(p + 1, facilities, facilities_len - 1);
+				if (len < 0)
+					return 0;
 				facilities_len -= len + 1;
 				p += len + 1;
 				break;
 
 			case FAC_CCITT:		/* CCITT */
 				len = rose_parse_ccitt(p + 1, facilities, facilities_len - 1);
+				if (len < 0)
+					return 0;
 				facilities_len -= len + 1;
 				p += len + 1;
 				break;
