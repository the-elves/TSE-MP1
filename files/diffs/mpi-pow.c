--- files/before/mpi-pow.c	2020-12-03 05:27:40.052602191 +0530
+++ files/after/mpi-pow.c	2020-12-03 05:27:40.968748667 +0530
@@ -64,8 +64,13 @@
 	if (!esize) {
 		/* Exponent is zero, result is 1 mod MOD, i.e., 1 or 0
 		 * depending on if MOD equals 1.  */
-		rp[0] = 1;
 		res->nlimbs = (msize == 1 && mod->d[0] == 1) ? 0 : 1;
+		if (res->nlimbs) {
+			if (mpi_resize(res, 1) < 0)
+				goto enomem;
+			rp = res->d;
+			rp[0] = 1;
+		}
 		res->sign = 0;
 		goto leave;
 	}
