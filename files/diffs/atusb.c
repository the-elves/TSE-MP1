--- files/before/atusb.c	2020-12-03 05:28:46.803274468 +0530
+++ files/after/atusb.c	2020-12-03 05:28:47.691416466 +0530
@@ -117,13 +117,26 @@
 {
 	struct usb_device *usb_dev = atusb->usb_dev;
 	int ret;
+	uint8_t *buffer;
 	uint8_t value;
 
+	buffer = kmalloc(1, GFP_KERNEL);
+	if (!buffer)
+		return -ENOMEM;
+
 	dev_dbg(&usb_dev->dev, "atusb: reg = 0x%x\n", reg);
 	ret = atusb_control_msg(atusb, usb_rcvctrlpipe(usb_dev, 0),
 				ATUSB_REG_READ, ATUSB_REQ_FROM_DEV,
-				0, reg, &value, 1, 1000);
-	return ret >= 0 ? value : ret;
+				0, reg, buffer, 1, 1000);
+
+	if (ret >= 0) {
+		value = buffer[0];
+		kfree(buffer);
+		return value;
+	} else {
+		kfree(buffer);
+		return ret;
+	}
 }
 
 static int atusb_write_subreg(struct atusb *atusb, uint8_t reg, uint8_t mask,
@@ -608,9 +621,13 @@
 static int atusb_get_and_show_revision(struct atusb *atusb)
 {
 	struct usb_device *usb_dev = atusb->usb_dev;
-	unsigned char buffer[3];
+	unsigned char *buffer;
 	int ret;
 
+	buffer = kmalloc(3, GFP_KERNEL);
+	if (!buffer)
+		return -ENOMEM;
+
 	/* Get a couple of the ATMega Firmware values */
 	ret = atusb_control_msg(atusb, usb_rcvctrlpipe(usb_dev, 0),
 				ATUSB_ID, ATUSB_REQ_FROM_DEV, 0, 0,
@@ -631,15 +648,20 @@
 		dev_info(&usb_dev->dev, "Please update to version 0.2 or newer");
 	}
 
+	kfree(buffer);
 	return ret;
 }
 
 static int atusb_get_and_show_build(struct atusb *atusb)
 {
 	struct usb_device *usb_dev = atusb->usb_dev;
-	char build[ATUSB_BUILD_SIZE + 1];
+	char *build;
 	int ret;
 
+	build = kmalloc(ATUSB_BUILD_SIZE + 1, GFP_KERNEL);
+	if (!build)
+		return -ENOMEM;
+
 	ret = atusb_control_msg(atusb, usb_rcvctrlpipe(usb_dev, 0),
 				ATUSB_BUILD, ATUSB_REQ_FROM_DEV, 0, 0,
 				build, ATUSB_BUILD_SIZE, 1000);
@@ -648,6 +670,7 @@
 		dev_info(&usb_dev->dev, "Firmware: build %s\n", build);
 	}
 
+	kfree(build);
 	return ret;
 }
 
