--- files/before/br_multicast.c	2020-12-03 05:26:45.147823866 +0530
+++ files/after/br_multicast.c	2020-12-03 05:26:46.087974179 +0530
@@ -719,7 +719,8 @@
 		goto err;
 
 	if (!port) {
-		hlist_add_head(&mp->mglist, &br->mglist);
+		if (hlist_unhashed(&mp->mglist))
+			hlist_add_head(&mp->mglist, &br->mglist);
 		mod_timer(&mp->timer, now + br->multicast_membership_interval);
 		goto out;
 	}
