--- files/before/drm_crtc.c	2020-12-03 05:26:29.993400937 +0530
+++ files/after/drm_crtc.c	2020-12-03 05:26:30.953554448 +0530
@@ -1873,6 +1873,10 @@
 	}
 
 	if (num_clips && clips_ptr) {
+		if (num_clips < 0 || num_clips > DRM_MODE_FB_DIRTY_MAX_CLIPS) {
+			ret = -EINVAL;
+			goto out_err1;
+		}
 		clips = kzalloc(num_clips * sizeof(*clips), GFP_KERNEL);
 		if (!clips) {
 			ret = -ENOMEM;
