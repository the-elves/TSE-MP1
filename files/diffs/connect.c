--- files/before/connect.c	2020-12-03 05:26:58.521962161 +0530
+++ files/after/connect.c	2020-12-03 05:26:59.462112473 +0530
@@ -3279,8 +3279,8 @@
 	pos = full_path + unc_len;
 
 	if (pplen) {
-		*pos++ = CIFS_DIR_SEP(cifs_sb);
-		strncpy(pos, vol->prepath, pplen);
+		*pos = CIFS_DIR_SEP(cifs_sb);
+		strncpy(pos + 1, vol->prepath, pplen);
 		pos += pplen;
 	}
 
