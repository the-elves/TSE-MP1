--- files/before/sequencer.c	2020-12-03 05:26:43.359537952 +0530
+++ files/after/sequencer.c	2020-12-03 05:26:44.243679310 +0530
@@ -241,7 +241,7 @@
 				return -ENXIO;
 
 			fmt = (*(short *) &event_rec[0]) & 0xffff;
-			err = synth_devs[dev]->load_patch(dev, fmt, buf, p + 4, c, 0);
+			err = synth_devs[dev]->load_patch(dev, fmt, buf + p, c, 0);
 			if (err < 0)
 				return err;
 
