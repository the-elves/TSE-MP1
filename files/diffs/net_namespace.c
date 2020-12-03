--- files/before/net_namespace.c	2020-12-03 05:29:41.392002261 +0530
+++ files/after/net_namespace.c	2020-12-03 05:29:42.280144259 +0530
@@ -267,7 +267,7 @@
 	spin_lock_bh(&net->nsid_lock);
 	peer = idr_find(&net->netns_ids, id);
 	if (peer)
-		get_net(peer);
+		peer = maybe_get_net(peer);
 	spin_unlock_bh(&net->nsid_lock);
 	rcu_read_unlock();
 
