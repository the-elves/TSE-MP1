--- files/before/phar.c	2020-12-03 05:28:52.304153962 +0530
+++ files/after/phar.c	2020-12-03 05:28:53.284310672 +0530
@@ -981,7 +981,6 @@
 		/* if the alias is stored we enforce it (implicit overrides explicit) */
 		if (alias && alias_len && (alias_len != (int)tmp_len || strncmp(alias, buffer, tmp_len)))
 		{
-			buffer[tmp_len] = '\0';
 			php_stream_close(fp);
 
 			if (signature) {
@@ -989,7 +988,7 @@
 			}
 
 			if (error) {
-				spprintf(error, 0, "cannot load phar \"%s\" with implicit alias \"%s\" under different alias \"%s\"", fname, buffer, alias);
+				spprintf(error, 0, "cannot load phar \"%s\" with implicit alias \"%.*s\" under different alias \"%s\"", fname, tmp_len, buffer, alias);
 			}
 
 			efree(savebuf);
