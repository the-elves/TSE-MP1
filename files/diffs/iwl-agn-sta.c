--- files/before/iwl-agn-sta.c	2020-12-03 05:30:14.345270920 +0530
+++ files/after/iwl-agn-sta.c	2020-12-03 05:30:15.645478800 +0530
@@ -35,9 +35,12 @@
 #include "iwl-trans.h"
 
 /* priv->shrd->sta_lock must be held */
-static void iwl_sta_ucode_activate(struct iwl_priv *priv, u8 sta_id)
+static int iwl_sta_ucode_activate(struct iwl_priv *priv, u8 sta_id)
 {
-
+	if (sta_id >= IWLAGN_STATION_COUNT) {
+		IWL_ERR(priv, "invalid sta_id %u", sta_id);
+		return -EINVAL;
+	}
 	if (!(priv->stations[sta_id].used & IWL_STA_DRIVER_ACTIVE))
 		IWL_ERR(priv, "ACTIVATE a non DRIVER active station id %u "
 			"addr %pM\n",
@@ -53,6 +56,7 @@
 		IWL_DEBUG_ASSOC(priv, "Added STA id %u addr %pM to uCode\n",
 				sta_id, priv->stations[sta_id].sta.sta.addr);
 	}
+	return 0;
 }
 
 static int iwl_process_add_sta_resp(struct iwl_priv *priv,
@@ -77,8 +81,7 @@
 	switch (pkt->u.add_sta.status) {
 	case ADD_STA_SUCCESS_MSK:
 		IWL_DEBUG_INFO(priv, "REPLY_ADD_STA PASSED\n");
-		iwl_sta_ucode_activate(priv, sta_id);
-		ret = 0;
+		ret = iwl_sta_ucode_activate(priv, sta_id);
 		break;
 	case ADD_STA_NO_ROOM_IN_TABLE:
 		IWL_ERR(priv, "Adding station %d failed, no room in table.\n",
