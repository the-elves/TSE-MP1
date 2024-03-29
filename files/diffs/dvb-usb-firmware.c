--- files/before/dvb-usb-firmware.c	2020-12-03 05:28:31.176776061 +0530
+++ files/after/dvb-usb-firmware.c	2020-12-03 05:28:32.056916783 +0530
@@ -36,16 +36,18 @@
 int usb_cypress_load_firmware(struct usb_device *udev, const struct firmware *fw, int type)
 {
 	struct hexline *hx;
-	u8 reset;
-	int ret,pos=0;
+	u8 *buf;
+	int ret, pos = 0;
+	u16 cpu_cs_register = cypress[type].cpu_cs_register;
 
-	hx = kmalloc(sizeof(*hx), GFP_KERNEL);
-	if (!hx)
+	buf = kmalloc(sizeof(*hx), GFP_KERNEL);
+	if (!buf)
 		return -ENOMEM;
+	hx = (struct hexline *)buf;
 
 	/* stop the CPU */
-	reset = 1;
-	if ((ret = usb_cypress_writemem(udev,cypress[type].cpu_cs_register,&reset,1)) != 1)
+	buf[0] = 1;
+	if (usb_cypress_writemem(udev, cpu_cs_register, buf, 1) != 1)
 		err("could not stop the USB controller CPU.");
 
 	while ((ret = dvb_usb_get_hexline(fw, hx, &pos)) > 0) {
@@ -61,21 +63,21 @@
 	}
 	if (ret < 0) {
 		err("firmware download failed at %d with %d",pos,ret);
-		kfree(hx);
+		kfree(buf);
 		return ret;
 	}
 
 	if (ret == 0) {
 		/* restart the CPU */
-		reset = 0;
-		if (ret || usb_cypress_writemem(udev,cypress[type].cpu_cs_register,&reset,1) != 1) {
+		buf[0] = 0;
+		if (usb_cypress_writemem(udev, cpu_cs_register, buf, 1) != 1) {
 			err("could not restart the USB controller CPU.");
 			ret = -EINVAL;
 		}
 	} else
 		ret = -EIO;
 
-	kfree(hx);
+	kfree(buf);
 
 	return ret;
 }
