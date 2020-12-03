--- files/before/dvb_usb_core.c	2020-12-03 05:28:27.124128113 +0530
+++ files/after/dvb_usb_core.c	2020-12-03 05:28:28.064278426 +0530
@@ -1012,8 +1012,8 @@
 void dvb_usbv2_disconnect(struct usb_interface *intf)
 {
 	struct dvb_usb_device *d = usb_get_intfdata(intf);
-	const char *name = d->name;
-	struct device dev = d->udev->dev;
+	const char *devname = kstrdup(dev_name(&d->udev->dev), GFP_KERNEL);
+	const char *drvname = d->name;
 
 	dev_dbg(&d->udev->dev, "%s: bInterfaceNumber=%d\n", __func__,
 			intf->cur_altsetting->desc.bInterfaceNumber);
@@ -1023,8 +1023,9 @@
 
 	dvb_usbv2_exit(d);
 
-	dev_info(&dev, "%s: '%s' successfully deinitialized and disconnected\n",
-			KBUILD_MODNAME, name);
+	pr_info("%s: '%s:%s' successfully deinitialized and disconnected\n",
+		KBUILD_MODNAME, drvname, devname);
+	kfree(devname);
 }
 EXPORT_SYMBOL(dvb_usbv2_disconnect);
 
