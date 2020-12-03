--- files/before/pegasus.c	2020-12-03 05:28:19.614927521 +0530
+++ files/after/pegasus.c	2020-12-03 05:28:20.619088069 +0530
@@ -126,40 +126,61 @@
 
 static int get_registers(pegasus_t *pegasus, __u16 indx, __u16 size, void *data)
 {
+	u8 *buf;
 	int ret;
 
+	buf = kmalloc(size, GFP_NOIO);
+	if (!buf)
+		return -ENOMEM;
+
 	ret = usb_control_msg(pegasus->usb, usb_rcvctrlpipe(pegasus->usb, 0),
 			      PEGASUS_REQ_GET_REGS, PEGASUS_REQT_READ, 0,
-			      indx, data, size, 1000);
+			      indx, buf, size, 1000);
 	if (ret < 0)
 		netif_dbg(pegasus, drv, pegasus->net,
 			  "%s returned %d\n", __func__, ret);
+	else if (ret <= size)
+		memcpy(data, buf, ret);
+	kfree(buf);
 	return ret;
 }
 
-static int set_registers(pegasus_t *pegasus, __u16 indx, __u16 size, void *data)
+static int set_registers(pegasus_t *pegasus, __u16 indx, __u16 size,
+			 const void *data)
 {
+	u8 *buf;
 	int ret;
 
+	buf = kmemdup(data, size, GFP_NOIO);
+	if (!buf)
+		return -ENOMEM;
+
 	ret = usb_control_msg(pegasus->usb, usb_sndctrlpipe(pegasus->usb, 0),
 			      PEGASUS_REQ_SET_REGS, PEGASUS_REQT_WRITE, 0,
-			      indx, data, size, 100);
+			      indx, buf, size, 100);
 	if (ret < 0)
 		netif_dbg(pegasus, drv, pegasus->net,
 			  "%s returned %d\n", __func__, ret);
+	kfree(buf);
 	return ret;
 }
 
 static int set_register(pegasus_t *pegasus, __u16 indx, __u8 data)
 {
+	u8 *buf;
 	int ret;
 
+	buf = kmemdup(&data, 1, GFP_NOIO);
+	if (!buf)
+		return -ENOMEM;
+
 	ret = usb_control_msg(pegasus->usb, usb_sndctrlpipe(pegasus->usb, 0),
 			      PEGASUS_REQ_SET_REG, PEGASUS_REQT_WRITE, data,
-			      indx, &data, 1, 1000);
+			      indx, buf, 1, 1000);
 	if (ret < 0)
 		netif_dbg(pegasus, drv, pegasus->net,
 			  "%s returned %d\n", __func__, ret);
+	kfree(buf);
 	return ret;
 }
 