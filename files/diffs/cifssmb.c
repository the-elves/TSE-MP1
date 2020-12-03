--- files/before/cifssmb.c	2020-12-03 05:26:33.754002189 +0530
+++ files/after/cifssmb.c	2020-12-03 05:26:34.754162099 +0530
@@ -4079,7 +4079,8 @@
 	T2_FNEXT_RSP_PARMS *parms;
 	char *response_data;
 	int rc = 0;
-	int bytes_returned, name_len;
+	int bytes_returned;
+	unsigned int name_len;
 	__u16 params, byte_count;
 
 	cFYI(1, "In FindNext");
