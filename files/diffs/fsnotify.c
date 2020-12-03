--- files/before/fsnotify.c	2020-12-03 05:28:38.765989445 +0530
+++ files/after/fsnotify.c	2020-12-03 05:28:39.690137200 +0530
@@ -161,16 +161,20 @@
 	if (unlikely(!fsnotify_inode_watches_children(p_inode)))
 		__fsnotify_update_child_dentry_flags(p_inode);
 	else if (p_inode->i_fsnotify_mask & mask) {
+		struct name_snapshot name;
+
 		/* we are notifying a parent so come up with the new mask which
 		 * specifies these are events which came from a child. */
 		mask |= FS_EVENT_ON_CHILD;
 
+		take_dentry_name_snapshot(&name, dentry);
 		if (path)
 			ret = fsnotify(p_inode, mask, path, FSNOTIFY_EVENT_PATH,
-				       dentry->d_name.name, 0);
+				       name.name, 0);
 		else
 			ret = fsnotify(p_inode, mask, dentry->d_inode, FSNOTIFY_EVENT_INODE,
-				       dentry->d_name.name, 0);
+				       name.name, 0);
+		release_dentry_name_snapshot(&name);
 	}
 
 	dput(parent);
