--- files/before/bitmap.c	2020-12-03 05:29:54.458091306 +0530
+++ files/after/bitmap.c	2020-12-03 05:29:55.354234583 +0530
@@ -794,7 +794,7 @@
 					replen = revcode;
 					collen = 0;
 				}
-				while (collen > 0)
+				while (indexw < width && collen > 0)
 				{
 					color = CVAL(in);
 					*out = color;
@@ -802,7 +802,7 @@
 					indexw++;
 					collen--;
 				}
-				while (replen > 0)
+				while (indexw < width && replen > 0)
 				{
 					*out = color;
 					out += 4;
@@ -824,7 +824,7 @@
 					replen = revcode;
 					collen = 0;
 				}
-				while (collen > 0)
+				while (indexw < width && collen > 0)
 				{
 					x = CVAL(in);
 					if (x & 1)
@@ -844,7 +844,7 @@
 					indexw++;
 					collen--;
 				}
-				while (replen > 0)
+				while (indexw < width && replen > 0)
 				{
 					x = last_line[indexw * 4] + color;
 					*out = x;
