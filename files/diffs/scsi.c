--- files/before/scsi.c	2020-12-03 05:27:26.654460058 +0530
+++ files/after/scsi.c	2020-12-03 05:27:27.574607173 +0530
@@ -1249,7 +1249,7 @@
 		 * lun[4-7] need to be zero according to virtio-scsi spec.
 		 */
 		evt->event.lun[0] = 0x01;
-		evt->event.lun[1] = tpg->tport_tpgt & 0xFF;
+		evt->event.lun[1] = tpg->tport_tpgt;
 		if (lun->unpacked_lun >= 256)
 			evt->event.lun[2] = lun->unpacked_lun >> 8 | 0x40 ;
 		evt->event.lun[3] = lun->unpacked_lun & 0xFF;
@@ -2120,12 +2120,12 @@
 			struct vhost_scsi_tport, tport_wwn);
 
 	struct vhost_scsi_tpg *tpg;
-	unsigned long tpgt;
+	u16 tpgt;
 	int ret;
 
 	if (strstr(name, "tpgt_") != name)
 		return ERR_PTR(-EINVAL);
-	if (kstrtoul(name + 5, 10, &tpgt) || tpgt > UINT_MAX)
+	if (kstrtou16(name + 5, 10, &tpgt) || tpgt >= VHOST_SCSI_MAX_TARGET)
 		return ERR_PTR(-EINVAL);
 
 	tpg = kzalloc(sizeof(struct vhost_scsi_tpg), GFP_KERNEL);
