--- files/before/hns_dsaf_rcb.c	2020-12-03 05:29:35.943131080 +0530
+++ files/after/hns_dsaf_rcb.c	2020-12-03 05:29:36.847275637 +0530
@@ -876,7 +876,7 @@
  */
 int hns_rcb_get_ring_sset_count(int stringset)
 {
-	if (stringset == ETH_SS_STATS)
+	if (stringset == ETH_SS_STATS || stringset == ETH_SS_PRIV_FLAGS)
 		return HNS_RING_STATIC_REG_NUM;
 
 	return 0;
