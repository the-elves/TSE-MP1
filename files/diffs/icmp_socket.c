--- files/before/icmp_socket.c	2020-12-03 05:27:04.282883229 +0530
+++ files/after/icmp_socket.c	2020-12-03 05:27:05.179026507 +0530
@@ -136,10 +136,9 @@
 
 	spin_unlock_bh(&socket_client->lock);
 
-	error = copy_to_user(buf, &socket_packet->icmp_packet,
-			     socket_packet->icmp_len);
+	packet_len = min(count, socket_packet->icmp_len);
+	error = copy_to_user(buf, &socket_packet->icmp_packet, packet_len);
 
-	packet_len = socket_packet->icmp_len;
 	kfree(socket_packet);
 
 	if (error)
