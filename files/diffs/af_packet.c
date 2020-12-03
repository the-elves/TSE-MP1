--- files/before/af_packet.c	2020-12-03 05:28:11.849685994 +0530
+++ files/after/af_packet.c	2020-12-03 05:28:12.817840785 +0530
@@ -2191,6 +2191,7 @@
 	struct timespec ts;
 	__u32 ts_status;
 	bool is_drop_n_account = false;
+	bool do_vnet = false;
 
 	/* struct tpacket{2,3}_hdr is aligned to a multiple of TPACKET_ALIGNMENT.
 	 * We may add members to them until current aligned size without forcing
@@ -2241,8 +2242,10 @@
 		netoff = TPACKET_ALIGN(po->tp_hdrlen +
 				       (maclen < 16 ? 16 : maclen)) +
 				       po->tp_reserve;
-		if (po->has_vnet_hdr)
+		if (po->has_vnet_hdr) {
 			netoff += sizeof(struct virtio_net_hdr);
+			do_vnet = true;
+		}
 		macoff = netoff - maclen;
 	}
 	if (po->tp_version <= TPACKET_V2) {
@@ -2259,8 +2262,10 @@
 					skb_set_owner_r(copy_skb, sk);
 			}
 			snaplen = po->rx_ring.frame_size - macoff;
-			if ((int)snaplen < 0)
+			if ((int)snaplen < 0) {
 				snaplen = 0;
+				do_vnet = false;
+			}
 		}
 	} else if (unlikely(macoff + snaplen >
 			    GET_PBDQC_FROM_RB(&po->rx_ring)->max_frame_len)) {
@@ -2273,6 +2278,7 @@
 		if (unlikely((int)snaplen < 0)) {
 			snaplen = 0;
 			macoff = GET_PBDQC_FROM_RB(&po->rx_ring)->max_frame_len;
+			do_vnet = false;
 		}
 	}
 	spin_lock(&sk->sk_receive_queue.lock);
@@ -2298,7 +2304,7 @@
 	}
 	spin_unlock(&sk->sk_receive_queue.lock);
 
-	if (po->has_vnet_hdr) {
+	if (do_vnet) {
 		if (virtio_net_hdr_from_skb(skb, h.raw + macoff -
 					    sizeof(struct virtio_net_hdr),
 					    vio_le(), true)) {
