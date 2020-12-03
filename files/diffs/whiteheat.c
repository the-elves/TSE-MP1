--- files/before/whiteheat.c	2020-12-03 05:27:15.604693391 +0530
+++ files/after/whiteheat.c	2020-12-03 05:27:16.500836669 +0530
@@ -514,6 +514,10 @@
 		dev_dbg(&urb->dev->dev, "%s - command_info is NULL, exiting.\n", __func__);
 		return;
 	}
+	if (!urb->actual_length) {
+		dev_dbg(&urb->dev->dev, "%s - empty response, exiting.\n", __func__);
+		return;
+	}
 	if (status) {
 		dev_dbg(&urb->dev->dev, "%s - nonzero urb status: %d\n", __func__, status);
 		if (status != -ENOENT)
@@ -534,7 +538,8 @@
 		/* These are unsolicited reports from the firmware, hence no
 		   waiting command to wakeup */
 		dev_dbg(&urb->dev->dev, "%s - event received\n", __func__);
-	} else if (data[0] == WHITEHEAT_GET_DTR_RTS) {
+	} else if ((data[0] == WHITEHEAT_GET_DTR_RTS) &&
+		(urb->actual_length - 1 <= sizeof(command_info->result_buffer))) {
 		memcpy(command_info->result_buffer, &data[1],
 						urb->actual_length - 1);
 		command_info->command_finished = WHITEHEAT_CMD_COMPLETE;
