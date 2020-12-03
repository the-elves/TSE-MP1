--- files/before/inode.c	2020-12-03 05:28:35.021390748 +0530
+++ files/after/inode.c	2020-12-03 05:28:35.889529549 +0530
@@ -766,7 +766,7 @@
 {
 	int error;
 	struct dentry *dentry = NULL, *trap;
-	const char *old_name;
+	struct name_snapshot old_name;
 
 	trap = lock_rename(new_dir, old_dir);
 	/* Source or destination directories don't exist? */
@@ -781,19 +781,19 @@
 	if (IS_ERR(dentry) || dentry == trap || d_really_is_positive(dentry))
 		goto exit;
 
-	old_name = fsnotify_oldname_init(old_dentry->d_name.name);
+	take_dentry_name_snapshot(&old_name, old_dentry);
 
 	error = simple_rename(d_inode(old_dir), old_dentry, d_inode(new_dir),
 			      dentry, 0);
 	if (error) {
-		fsnotify_oldname_free(old_name);
+		release_dentry_name_snapshot(&old_name);
 		goto exit;
 	}
 	d_move(old_dentry, dentry);
-	fsnotify_move(d_inode(old_dir), d_inode(new_dir), old_name,
+	fsnotify_move(d_inode(old_dir), d_inode(new_dir), old_name.name,
 		d_is_dir(old_dentry),
 		NULL, old_dentry);
-	fsnotify_oldname_free(old_name);
+	release_dentry_name_snapshot(&old_name);
 	unlock_rename(new_dir, old_dir);
 	dput(dentry);
 	return old_dentry;
