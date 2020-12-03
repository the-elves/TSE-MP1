--- files/before/pmcraid.c	2020-12-03 05:26:35.746320727 +0530
+++ files/after/pmcraid.c	2020-12-03 05:26:36.746480635 +0530
@@ -3871,6 +3871,9 @@
 			pmcraid_err("couldn't build passthrough ioadls\n");
 			goto out_free_buffer;
 		}
+	} else if (request_size < 0) {
+		rc = -EINVAL;
+		goto out_free_buffer;
 	}
 
 	/* If data is being written into the device, copy the data from user
