--- files/before/catc.c	2020-12-03 05:28:15.730306438 +0530
+++ files/after/catc.c	2020-12-03 05:28:16.682458670 +0530
@@ -776,7 +776,7 @@
 	struct net_device *netdev;
 	struct catc *catc;
 	u8 broadcast[ETH_ALEN];
-	int i, pktsz, ret;
+	int pktsz, ret;
 
 	if (usb_set_interface(usbdev,
 			intf->altsetting->desc.bInterfaceNumber, 1)) {
@@ -840,15 +840,24 @@
                 catc->irq_buf, 2, catc_irq_done, catc, 1);
 
 	if (!catc->is_f5u011) {
+		u32 *buf;
+		int i;
+
 		dev_dbg(dev, "Checking memory size\n");
 
-		i = 0x12345678;
-		catc_write_mem(catc, 0x7a80, &i, 4);
-		i = 0x87654321;	
-		catc_write_mem(catc, 0xfa80, &i, 4);
-		catc_read_mem(catc, 0x7a80, &i, 4);
+		buf = kmalloc(4, GFP_KERNEL);
+		if (!buf) {
+			ret = -ENOMEM;
+			goto fail_free;
+		}
+
+		*buf = 0x12345678;
+		catc_write_mem(catc, 0x7a80, buf, 4);
+		*buf = 0x87654321;
+		catc_write_mem(catc, 0xfa80, buf, 4);
+		catc_read_mem(catc, 0x7a80, buf, 4);
 	  
-		switch (i) {
+		switch (*buf) {
 		case 0x12345678:
 			catc_set_reg(catc, TxBufCount, 8);
 			catc_set_reg(catc, RxBufCount, 32);
@@ -863,6 +872,8 @@
 			dev_dbg(dev, "32k Memory\n");
 			break;
 		}
+
+		kfree(buf);
 	  
 		dev_dbg(dev, "Getting MAC from SEEROM.\n");
 	  
