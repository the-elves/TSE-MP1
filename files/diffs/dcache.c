--- files/before/dcache.c	2020-12-03 05:28:33.117086285 +0530
+++ files/after/dcache.c	2020-12-03 05:28:34.133248751 +0530
@@ -277,6 +277,33 @@
 	return dentry->d_name.name != dentry->d_iname;
 }
 
+void take_dentry_name_snapshot(struct name_snapshot *name, struct dentry *dentry)
+{
+	spin_lock(&dentry->d_lock);
+	if (unlikely(dname_external(dentry))) {
+		struct external_name *p = external_name(dentry);
+		atomic_inc(&p->u.count);
+		spin_unlock(&dentry->d_lock);
+		name->name = p->name;
+	} else {
+		memcpy(name->inline_name, dentry->d_iname, DNAME_INLINE_LEN);
+		spin_unlock(&dentry->d_lock);
+		name->name = name->inline_name;
+	}
+}
+EXPORT_SYMBOL(take_dentry_name_snapshot);
+
+void release_dentry_name_snapshot(struct name_snapshot *name)
+{
+	if (unlikely(name->name != name->inline_name)) {
+		struct external_name *p;
+		p = container_of(name->name, struct external_name, name[0]);
+		if (unlikely(atomic_dec_and_test(&p->u.count)))
+			kfree_rcu(p, u.head);
+	}
+}
+EXPORT_SYMBOL(release_dentry_name_snapshot);
+
 static inline void __d_set_inode_and_type(struct dentry *dentry,
 					  struct inode *inode,
 					  unsigned type_flags)
