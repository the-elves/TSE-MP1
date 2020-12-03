--- files/before/vop_vringh.c	2020-12-03 05:27:48.561962694 +0530
+++ files/after/vop_vringh.c	2020-12-03 05:27:49.546120042 +0530
@@ -945,6 +945,11 @@
 			ret = -EFAULT;
 			goto free_ret;
 		}
+		/* Ensure desc has not changed between the two reads */
+		if (memcmp(&dd, dd_config, sizeof(dd))) {
+			ret = -EINVAL;
+			goto free_ret;
+		}
 		mutex_lock(&vdev->vdev_mutex);
 		mutex_lock(&vi->vop_mutex);
 		ret = vop_virtio_add_device(vdev, dd_config);
