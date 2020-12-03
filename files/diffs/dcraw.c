--- files/before/dcraw.c	2020-12-03 05:28:40.778311181 +0530
+++ files/after/dcraw.c	2020-12-03 05:28:41.850482601 +0530
@@ -12870,6 +12870,10 @@
         load_raw = &CLASS sony_arw_load_raw;
         data_offset = get4() + base;
         ifd++;
+#ifdef LIBRAW_LIBRARY_BUILD
+	if (ifd >= sizeof tiff_ifd / sizeof tiff_ifd[0])
+	  throw LIBRAW_EXCEPTION_IO_CORRUPT;
+#endif    
         break;
       }
 #ifdef LIBRAW_LIBRARY_BUILD
@@ -13177,7 +13181,7 @@
       break;
     case 50454: /* Sinar tag */
     case 50455:
-      if (len > 2560000 || !(cbuf = (char *)malloc(len)))
+      if (len < 1 || len > 2560000 || !(cbuf = (char *)malloc(len)))
         break;
 #ifndef LIBRAW_LIBRARY_BUILD
       fread(cbuf, 1, len, ifp);
@@ -14795,7 +14799,11 @@
     }
     order = get2();
     hlen = get4();
-    if (get4() == 0x48454150) /* "HEAP" */
+    if (get4() == 0x48454150
+#ifdef LIBRAW_LIBRARY_BUILD
+	&& (save+hlen) >= 0 && (save+hlen)<=ifp->size()
+#endif
+	) /* "HEAP" */
     {
 #ifdef LIBRAW_LIBRARY_BUILD
       imgdata.lens.makernotes.CameraMount = LIBRAW_MOUNT_FixedLens;