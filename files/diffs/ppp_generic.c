--- files/before/ppp_generic.c	2020-12-03 05:27:58.919618700 +0530
+++ files/after/ppp_generic.c	2020-12-03 05:27:59.887773490 +0530
@@ -2307,7 +2307,7 @@
 
 	pch->ppp = NULL;
 	pch->chan = chan;
-	pch->chan_net = net;
+	pch->chan_net = get_net(net);
 	chan->ppp = pch;
 	init_ppp_file(&pch->file, CHANNEL);
 	pch->file.hdrlen = chan->hdrlen;
@@ -2404,6 +2404,8 @@
 	spin_lock_bh(&pn->all_channels_lock);
 	list_del(&pch->list);
 	spin_unlock_bh(&pn->all_channels_lock);
+	put_net(pch->chan_net);
+	pch->chan_net = NULL;
 
 	pch->file.dead = 1;
 	wake_up_interruptible(&pch->file.rwait);
