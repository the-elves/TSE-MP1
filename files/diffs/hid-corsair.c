--- files/before/hid-corsair.c	2020-12-03 05:28:48.587559745 +0530
+++ files/after/hid-corsair.c	2020-12-03 05:28:49.491704301 +0530
@@ -148,7 +148,11 @@
 	struct usb_interface *usbif = to_usb_interface(dev->parent);
 	struct usb_device *usbdev = interface_to_usbdev(usbif);
 	int brightness;
-	char data[8];
+	char *data;
+
+	data = kmalloc(8, GFP_KERNEL);
+	if (!data)
+		return -ENOMEM;
 
 	ret = usb_control_msg(usbdev, usb_rcvctrlpipe(usbdev, 0),
 			      K90_REQUEST_STATUS,
@@ -158,16 +162,22 @@
 	if (ret < 0) {
 		dev_warn(dev, "Failed to get K90 initial state (error %d).\n",
 			 ret);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
 	brightness = data[4];
 	if (brightness < 0 || brightness > 3) {
 		dev_warn(dev,
 			 "Read invalid backlight brightness: %02hhx.\n",
 			 data[4]);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
-	return brightness;
+	ret = brightness;
+out:
+	kfree(data);
+
+	return ret;
 }
 
 static enum led_brightness k90_record_led_get(struct led_classdev *led_cdev)
@@ -253,7 +263,11 @@
 	struct usb_interface *usbif = to_usb_interface(dev->parent);
 	struct usb_device *usbdev = interface_to_usbdev(usbif);
 	const char *macro_mode;
-	char data[8];
+	char *data;
+
+	data = kmalloc(2, GFP_KERNEL);
+	if (!data)
+		return -ENOMEM;
 
 	ret = usb_control_msg(usbdev, usb_rcvctrlpipe(usbdev, 0),
 			      K90_REQUEST_GET_MODE,
@@ -263,7 +277,8 @@
 	if (ret < 0) {
 		dev_warn(dev, "Failed to get K90 initial mode (error %d).\n",
 			 ret);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
 
 	switch (data[0]) {
@@ -277,10 +292,15 @@
 	default:
 		dev_warn(dev, "K90 in unknown mode: %02hhx.\n",
 			 data[0]);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
 
-	return snprintf(buf, PAGE_SIZE, "%s\n", macro_mode);
+	ret = snprintf(buf, PAGE_SIZE, "%s\n", macro_mode);
+out:
+	kfree(data);
+
+	return ret;
 }
 
 static ssize_t k90_store_macro_mode(struct device *dev,
@@ -320,7 +340,11 @@
 	struct usb_interface *usbif = to_usb_interface(dev->parent);
 	struct usb_device *usbdev = interface_to_usbdev(usbif);
 	int current_profile;
-	char data[8];
+	char *data;
+
+	data = kmalloc(8, GFP_KERNEL);
+	if (!data)
+		return -ENOMEM;
 
 	ret = usb_control_msg(usbdev, usb_rcvctrlpipe(usbdev, 0),
 			      K90_REQUEST_STATUS,
@@ -330,16 +354,22 @@
 	if (ret < 0) {
 		dev_warn(dev, "Failed to get K90 initial state (error %d).\n",
 			 ret);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
 	current_profile = data[7];
 	if (current_profile < 1 || current_profile > 3) {
 		dev_warn(dev, "Read invalid current profile: %02hhx.\n",
 			 data[7]);
-		return -EIO;
+		ret = -EIO;
+		goto out;
 	}
 
-	return snprintf(buf, PAGE_SIZE, "%d\n", current_profile);
+	ret = snprintf(buf, PAGE_SIZE, "%d\n", current_profile);
+out:
+	kfree(data);
+
+	return ret;
 }
 
 static ssize_t k90_store_current_profile(struct device *dev,
