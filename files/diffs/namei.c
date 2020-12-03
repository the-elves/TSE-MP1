--- files/before/namei.c	2020-12-03 05:28:36.929695853 +0530
+++ files/after/namei.c	2020-12-03 05:28:37.905851924 +0530
@@ -4362,11 +4362,11 @@
 {
 	int error;
 	bool is_dir = d_is_dir(old_dentry);
-	const unsigned char *old_name;
 	struct inode *source = old_dentry->d_inode;
 	struct inode *target = new_dentry->d_inode;
 	bool new_is_dir = false;
 	unsigned max_links = new_dir->i_sb->s_max_links;
+	struct name_snapshot old_name;
 
 	if (source == target)
 		return 0;
@@ -4413,7 +4413,7 @@
 	if (error)
 		return error;
 
-	old_name = fsnotify_oldname_init(old_dentry->d_name.name);
+	take_dentry_name_snapshot(&old_name, old_dentry);
 	dget(new_dentry);
 	if (!is_dir || (flags & RENAME_EXCHANGE))
 		lock_two_nondirectories(source, target);
@@ -4468,14 +4468,14 @@
 		inode_unlock(target);
 	dput(new_dentry);
 	if (!error) {
-		fsnotify_move(old_dir, new_dir, old_name, is_dir,
+		fsnotify_move(old_dir, new_dir, old_name.name, is_dir,
 			      !(flags & RENAME_EXCHANGE) ? target : NULL, old_dentry);
 		if (flags & RENAME_EXCHANGE) {
 			fsnotify_move(new_dir, old_dir, old_dentry->d_name.name,
 				      new_is_dir, NULL, new_dentry);
 		}
 	}
-	fsnotify_oldname_free(old_name);
+	release_dentry_name_snapshot(&old_name);
 
 	return error;
 }
