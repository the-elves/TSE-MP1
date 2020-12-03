--- files/before/midi_synth.c	2020-12-03 05:26:41.451232849 +0530
+++ files/after/midi_synth.c	2020-12-03 05:26:42.387382522 +0530
@@ -476,7 +476,7 @@
 
 int
 midi_synth_load_patch(int dev, int format, const char __user *addr,
-		      int offs, int count, int pmgr_flag)
+		      int count, int pmgr_flag)
 {
 	int             orig_dev = synth_devs[dev]->midi_dev;
 
@@ -491,33 +491,29 @@
 	if (!prefix_cmd(orig_dev, 0xf0))
 		return 0;
 
+	/* Invalid patch format */
 	if (format != SYSEX_PATCH)
-	{
-/*		  printk("MIDI Error: Invalid patch format (key) 0x%x\n", format);*/
 		  return -EINVAL;
-	}
+
+	/* Patch header too short */
 	if (count < hdr_size)
-	{
-/*		printk("MIDI Error: Patch header too short\n");*/
 		return -EINVAL;
-	}
+
 	count -= hdr_size;
 
 	/*
-	 * Copy the header from user space but ignore the first bytes which have
-	 * been transferred already.
+	 * Copy the header from user space
 	 */
 
-	if(copy_from_user(&((char *) &sysex)[offs], &(addr)[offs], hdr_size - offs))
+	if (copy_from_user(&sysex, addr, hdr_size))
 		return -EFAULT;
- 
- 	if (count < sysex.len)
-	{
-/*		printk(KERN_WARNING "MIDI Warning: Sysex record too short (%d<%d)\n", count, (int) sysex.len);*/
+
+	/* Sysex record too short */
+	if ((unsigned)count < (unsigned)sysex.len)
 		sysex.len = count;
-	}
-  	left = sysex.len;
-  	src_offs = 0;
+
+	left = sysex.len;
+	src_offs = 0;
 
 	for (i = 0; i < left && !signal_pending(current); i++)
 	{
