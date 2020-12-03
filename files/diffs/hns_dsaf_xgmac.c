--- files/before/hns_dsaf_xgmac.c	2020-12-03 05:29:37.755420834 +0530
+++ files/after/hns_dsaf_xgmac.c	2020-12-03 05:29:38.631560916 +0530
@@ -781,7 +781,7 @@
  */
 static int hns_xgmac_get_sset_count(int stringset)
 {
-	if (stringset == ETH_SS_STATS)
+	if (stringset == ETH_SS_STATS || stringset == ETH_SS_PRIV_FLAGS)
 		return ARRAY_SIZE(g_xgmac_stats_string);
 
 	return 0;
