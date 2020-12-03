--- files/before/nfs4proc.c	2020-12-03 05:26:52.693030212 +0530
+++ files/after/nfs4proc.c	2020-12-03 05:26:53.625179247 +0530
@@ -3937,8 +3937,13 @@
 		goto out_free;
 	}
 	nfs4_write_cached_acl(inode, pages, res.acl_data_offset, res.acl_len);
-	if (buf)
+	if (buf) {
+		if (res.acl_len > buflen) {
+			ret = -ERANGE;
+			goto out_free;
+		}
 		_copy_from_pages(buf, pages, res.acl_data_offset, res.acl_len);
+	}
 out_ok:
 	ret = res.acl_len;
 out_free:
