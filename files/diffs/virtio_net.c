--- files/before/virtio_net.c	2020-12-03 05:27:24.818166466 +0530
+++ files/after/virtio_net.c	2020-12-03 05:27:25.738313581 +0530
@@ -1756,9 +1756,9 @@
 	/* Do we support "hardware" checksums? */
 	if (virtio_has_feature(vdev, VIRTIO_NET_F_CSUM)) {
 		/* This opens up the world of extra features. */
-		dev->hw_features |= NETIF_F_HW_CSUM|NETIF_F_SG|NETIF_F_FRAGLIST;
+		dev->hw_features |= NETIF_F_HW_CSUM | NETIF_F_SG;
 		if (csum)
-			dev->features |= NETIF_F_HW_CSUM|NETIF_F_SG|NETIF_F_FRAGLIST;
+			dev->features |= NETIF_F_HW_CSUM | NETIF_F_SG;
 
 		if (virtio_has_feature(vdev, VIRTIO_NET_F_GSO)) {
 			dev->hw_features |= NETIF_F_TSO | NETIF_F_UFO
