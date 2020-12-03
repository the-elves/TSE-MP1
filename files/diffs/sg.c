--- files/before/sg.c	2020-12-03 05:28:05.832723989 +0530
+++ files/after/sg.c	2020-12-03 05:28:06.776874942 +0530
@@ -787,8 +787,14 @@
 		return k;	/* probably out of space --> ENOMEM */
 	}
 	if (atomic_read(&sdp->detaching)) {
-		if (srp->bio)
+		if (srp->bio) {
+			if (srp->rq->cmd != srp->rq->__cmd)
+				kfree(srp->rq->cmd);
+
 			blk_end_request_all(srp->rq, -EIO);
+			srp->rq = NULL;
+		}
+
 		sg_finish_rem_req(srp);
 		return -ENODEV;
 	}
