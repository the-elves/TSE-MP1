--- files/before/gs_usb.c	2020-12-03 05:28:23.383530056 +0530
+++ files/after/gs_usb.c	2020-12-03 05:28:24.279673334 +0530
@@ -908,10 +908,14 @@
 	struct gs_usb *dev;
 	int rc = -ENOMEM;
 	unsigned int icount, i;
-	struct gs_host_config hconf = {
-		.byte_order = 0x0000beef,
-	};
-	struct gs_device_config dconf;
+	struct gs_host_config *hconf;
+	struct gs_device_config *dconf;
+
+	hconf = kmalloc(sizeof(*hconf), GFP_KERNEL);
+	if (!hconf)
+		return -ENOMEM;
+
+	hconf->byte_order = 0x0000beef;
 
 	/* send host config */
 	rc = usb_control_msg(interface_to_usbdev(intf),
@@ -920,16 +924,22 @@
 			     USB_DIR_OUT|USB_TYPE_VENDOR|USB_RECIP_INTERFACE,
 			     1,
 			     intf->altsetting[0].desc.bInterfaceNumber,
-			     &hconf,
-			     sizeof(hconf),
+			     hconf,
+			     sizeof(*hconf),
 			     1000);
 
+	kfree(hconf);
+
 	if (rc < 0) {
 		dev_err(&intf->dev, "Couldn't send data format (err=%d)\n",
 			rc);
 		return rc;
 	}
 
+	dconf = kmalloc(sizeof(*dconf), GFP_KERNEL);
+	if (!dconf)
+		return -ENOMEM;
+
 	/* read device config */
 	rc = usb_control_msg(interface_to_usbdev(intf),
 			     usb_rcvctrlpipe(interface_to_usbdev(intf), 0),
@@ -937,28 +947,33 @@
 			     USB_DIR_IN|USB_TYPE_VENDOR|USB_RECIP_INTERFACE,
 			     1,
 			     intf->altsetting[0].desc.bInterfaceNumber,
-			     &dconf,
-			     sizeof(dconf),
+			     dconf,
+			     sizeof(*dconf),
 			     1000);
 	if (rc < 0) {
 		dev_err(&intf->dev, "Couldn't get device config: (err=%d)\n",
 			rc);
+		kfree(dconf);
 		return rc;
 	}
 
-	icount = dconf.icount + 1;
+	icount = dconf->icount + 1;
 	dev_info(&intf->dev, "Configuring for %d interfaces\n", icount);
 
 	if (icount > GS_MAX_INTF) {
 		dev_err(&intf->dev,
 			"Driver cannot handle more that %d CAN interfaces\n",
 			GS_MAX_INTF);
+		kfree(dconf);
 		return -EINVAL;
 	}
 
 	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
-	if (!dev)
+	if (!dev) {
+		kfree(dconf);
 		return -ENOMEM;
+	}
+
 	init_usb_anchor(&dev->rx_submitted);
 
 	atomic_set(&dev->active_channels, 0);
@@ -967,7 +982,7 @@
 	dev->udev = interface_to_usbdev(intf);
 
 	for (i = 0; i < icount; i++) {
-		dev->canch[i] = gs_make_candev(i, intf, &dconf);
+		dev->canch[i] = gs_make_candev(i, intf, dconf);
 		if (IS_ERR_OR_NULL(dev->canch[i])) {
 			/* save error code to return later */
 			rc = PTR_ERR(dev->canch[i]);
@@ -978,12 +993,15 @@
 				gs_destroy_candev(dev->canch[i]);
 
 			usb_kill_anchored_urbs(&dev->rx_submitted);
+			kfree(dconf);
 			kfree(dev);
 			return rc;
 		}
 		dev->canch[i]->parent = dev;
 	}
 
+	kfree(dconf);
+
 	return 0;
 }
 
