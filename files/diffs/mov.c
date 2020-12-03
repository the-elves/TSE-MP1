--- files/before/mov.c	2020-12-03 05:28:01.111969219 +0530
+++ files/after/mov.c	2020-12-03 05:28:02.784236584 +0530
@@ -401,6 +401,7 @@
     if (entries >= UINT_MAX / sizeof(*sc->drefs))
         return AVERROR_INVALIDDATA;
     av_free(sc->drefs);
+    sc->drefs_count = 0;
     sc->drefs = av_mallocz(entries * sizeof(*sc->drefs));
     if (!sc->drefs)
         return AVERROR(ENOMEM);
