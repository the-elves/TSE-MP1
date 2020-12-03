--- files/before/hns_dsaf_gmac.c	2020-12-03 05:29:31.722456267 +0530
+++ files/after/hns_dsaf_gmac.c	2020-12-03 05:29:32.630601466 +0530
@@ -666,7 +666,7 @@
 
 static int hns_gmac_get_sset_count(int stringset)
 {
-	if (stringset == ETH_SS_STATS)
+	if (stringset == ETH_SS_STATS || stringset == ETH_SS_PRIV_FLAGS)
 		return ARRAY_SIZE(g_gmac_stats_string);
 
 	return 0;
