--- files/before/virtio_console.c	2020-12-03 05:28:21.539235185 +0530
+++ files/after/virtio_console.c	2020-12-03 05:28:22.463382941 +0530
@@ -1136,6 +1136,8 @@
 {
 	struct port *port;
 	struct scatterlist sg[1];
+	void *data;
+	int ret;
 
 	if (unlikely(early_put_chars))
 		return early_put_chars(vtermno, buf, count);
@@ -1144,8 +1146,14 @@
 	if (!port)
 		return -EPIPE;
 
-	sg_init_one(sg, buf, count);
-	return __send_to_port(port, sg, 1, count, (void *)buf, false);
+	data = kmemdup(buf, count, GFP_ATOMIC);
+	if (!data)
+		return -ENOMEM;
+
+	sg_init_one(sg, data, count);
+	ret = __send_to_port(port, sg, 1, count, data, false);
+	kfree(data);
+	return ret;
 }
 
 /*
