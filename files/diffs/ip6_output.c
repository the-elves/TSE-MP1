--- files/before/ip6_output.c	2020-12-03 05:26:56.633660256 +0530
+++ files/after/ip6_output.c	2020-12-03 05:26:57.597814406 +0530
@@ -1008,6 +1008,7 @@
 
 {
 	struct sk_buff *skb;
+	struct frag_hdr fhdr;
 	int err;
 
 	/* There is support for UDP large send offload by network
@@ -1015,8 +1016,6 @@
 	 * udp datagram
 	 */
 	if ((skb = skb_peek_tail(&sk->sk_write_queue)) == NULL) {
-		struct frag_hdr fhdr;
-
 		skb = sock_alloc_send_skb(sk,
 			hh_len + fragheaderlen + transhdrlen + 20,
 			(flags & MSG_DONTWAIT), &err);
@@ -1036,20 +1035,24 @@
 		skb->transport_header = skb->network_header + fragheaderlen;
 
 		skb->protocol = htons(ETH_P_IPV6);
-		skb->ip_summed = CHECKSUM_PARTIAL;
 		skb->csum = 0;
 
-		/* Specify the length of each IPv6 datagram fragment.
-		 * It has to be a multiple of 8.
-		 */
-		skb_shinfo(skb)->gso_size = (mtu - fragheaderlen -
-					     sizeof(struct frag_hdr)) & ~7;
-		skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
-		ipv6_select_ident(&fhdr, rt);
-		skb_shinfo(skb)->ip6_frag_id = fhdr.identification;
 		__skb_queue_tail(&sk->sk_write_queue, skb);
+	} else if (skb_is_gso(skb)) {
+		goto append;
 	}
 
+	skb->ip_summed = CHECKSUM_PARTIAL;
+	/* Specify the length of each IPv6 datagram fragment.
+	 * It has to be a multiple of 8.
+	 */
+	skb_shinfo(skb)->gso_size = (mtu - fragheaderlen -
+				     sizeof(struct frag_hdr)) & ~7;
+	skb_shinfo(skb)->gso_type = SKB_GSO_UDP;
+	ipv6_select_ident(&fhdr, rt);
+	skb_shinfo(skb)->ip6_frag_id = fhdr.identification;
+
+append:
 	return skb_append_datato_frags(sk, skb, getfrag, from,
 				       (length - transhdrlen));
 }
