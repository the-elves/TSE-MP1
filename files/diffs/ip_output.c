--- files/before/ip_output.c	2020-12-03 05:26:54.525323164 +0530
+++ files/after/ip_output.c	2020-12-03 05:26:55.525483073 +0530
@@ -772,15 +772,20 @@
 		/* initialize protocol header pointer */
 		skb->transport_header = skb->network_header + fragheaderlen;
 
-		skb->ip_summed = CHECKSUM_PARTIAL;
 		skb->csum = 0;
 
-		/* specify the length of each IP datagram fragment */
-		skb_shinfo(skb)->gso_size = maxfraglen - fragheaderlen;
-		skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
+
 		__skb_queue_tail(queue, skb);
+	} else if (skb_is_gso(skb)) {
+		goto append;
 	}
 
+	skb->ip_summed = CHECKSUM_PARTIAL;
+	/* specify the length of each IP datagram fragment */
+	skb_shinfo(skb)->gso_size = maxfraglen - fragheaderlen;
+	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
+
+append:
 	return skb_append_datato_frags(sk, skb, getfrag, from,
 				       (length - transhdrlen));
 }
