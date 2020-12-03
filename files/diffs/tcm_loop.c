--- files/before/tcm_loop.c	2020-12-03 05:30:16.581628474 +0530
+++ files/after/tcm_loop.c	2020-12-03 05:30:17.497774952 +0530
@@ -1205,7 +1205,7 @@
 	tpgt_str += 5; /* Skip ahead of "tpgt_" */
 	tpgt = (unsigned short int) simple_strtoul(tpgt_str, &end_ptr, 0);
 
-	if (tpgt > TL_TPGS_PER_HBA) {
+	if (tpgt >= TL_TPGS_PER_HBA) {
 		printk(KERN_ERR "Passed tpgt: %hu exceeds TL_TPGS_PER_HBA:"
 				" %u\n", tpgt, TL_TPGS_PER_HBA);
 		return ERR_PTR(-EINVAL);
