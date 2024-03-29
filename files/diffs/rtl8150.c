--- files/before/rtl8150.c	2020-12-03 05:28:17.614607704 +0530
+++ files/after/rtl8150.c	2020-12-03 05:28:18.714783604 +0530
@@ -155,16 +155,36 @@
 */
 static int get_registers(rtl8150_t * dev, u16 indx, u16 size, void *data)
 {
-	return usb_control_msg(dev->udev, usb_rcvctrlpipe(dev->udev, 0),
-			       RTL8150_REQ_GET_REGS, RTL8150_REQT_READ,
-			       indx, 0, data, size, 500);
+	void *buf;
+	int ret;
+
+	buf = kmalloc(size, GFP_NOIO);
+	if (!buf)
+		return -ENOMEM;
+
+	ret = usb_control_msg(dev->udev, usb_rcvctrlpipe(dev->udev, 0),
+			      RTL8150_REQ_GET_REGS, RTL8150_REQT_READ,
+			      indx, 0, buf, size, 500);
+	if (ret > 0 && ret <= size)
+		memcpy(data, buf, ret);
+	kfree(buf);
+	return ret;
 }
 
-static int set_registers(rtl8150_t * dev, u16 indx, u16 size, void *data)
+static int set_registers(rtl8150_t * dev, u16 indx, u16 size, const void *data)
 {
-	return usb_control_msg(dev->udev, usb_sndctrlpipe(dev->udev, 0),
-			       RTL8150_REQ_SET_REGS, RTL8150_REQT_WRITE,
-			       indx, 0, data, size, 500);
+	void *buf;
+	int ret;
+
+	buf = kmemdup(data, size, GFP_NOIO);
+	if (!buf)
+		return -ENOMEM;
+
+	ret = usb_control_msg(dev->udev, usb_sndctrlpipe(dev->udev, 0),
+			      RTL8150_REQ_SET_REGS, RTL8150_REQT_WRITE,
+			      indx, 0, buf, size, 500);
+	kfree(buf);
+	return ret;
 }
 
 static void async_set_reg_cb(struct urb *urb)
