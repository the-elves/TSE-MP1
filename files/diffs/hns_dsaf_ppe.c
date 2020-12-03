--- files/before/hns_dsaf_ppe.c	2020-12-03 05:29:33.562750499 +0530
+++ files/after/hns_dsaf_ppe.c	2020-12-03 05:29:34.414886740 +0530
@@ -422,7 +422,7 @@
 
 int hns_ppe_get_sset_count(int stringset)
 {
-	if (stringset == ETH_SS_STATS)
+	if (stringset == ETH_SS_STATS || stringset == ETH_SS_PRIV_FLAGS)
 		return ETH_PPE_STATIC_NUM;
 	return 0;
 }
