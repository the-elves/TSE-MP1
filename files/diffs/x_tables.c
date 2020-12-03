--- files/before/x_tables.c	2020-12-03 05:27:57.027316152 +0530
+++ files/after/x_tables.c	2020-12-03 05:27:57.963465829 +0530
@@ -659,6 +659,9 @@
 	struct xt_table_info *info = NULL;
 	size_t sz = sizeof(*info) + size;
 
+	if (sz < sizeof(*info))
+		return NULL;
+
 	/* Pedantry: prevent them from hitting BUG() in vmalloc.c --RR */
 	if ((SMP_ALIGN(size) >> PAGE_SHIFT) + 2 > totalram_pages)
 		return NULL;
