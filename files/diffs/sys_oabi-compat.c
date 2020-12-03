--- files/before/sys_oabi-compat.c	2020-12-03 05:26:37.642623913 +0530
+++ files/after/sys_oabi-compat.c	2020-12-03 05:26:38.554769749 +0530
@@ -311,7 +311,7 @@
 	long err;
 	int i;
 
-	if (nsops < 1)
+	if (nsops < 1 || nsops > SEMOPM)
 		return -EINVAL;
 	sops = kmalloc(sizeof(*sops) * nsops, GFP_KERNEL);
 	if (!sops)
