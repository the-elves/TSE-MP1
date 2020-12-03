--- files/before/lz4_decompress.c	2020-12-03 05:27:08.243516468 +0530
+++ files/after/lz4_decompress.c	2020-12-03 05:27:09.147661024 +0530
@@ -72,6 +72,8 @@
 			len = *ip++;
 			for (; len == 255; length += 255)
 				len = *ip++;
+			if (unlikely(length > (size_t)(length + len)))
+				goto _output_error;
 			length += len;
 		}
 
