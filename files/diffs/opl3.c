--- files/before/opl3.c	2020-12-03 05:26:39.522924540 +0530
+++ files/after/opl3.c	2020-12-03 05:26:40.563090851 +0530
@@ -820,7 +820,7 @@
 }
 
 static int opl3_load_patch(int dev, int format, const char __user *addr,
-		int offs, int count, int pmgr_flag)
+		int count, int pmgr_flag)
 {
 	struct sbi_instrument ins;
 
@@ -830,11 +830,7 @@
 		return -EINVAL;
 	}
 
-	/*
-	 * What the fuck is going on here?  We leave junk in the beginning
-	 * of ins and then check the field pretty close to that beginning?
-	 */
-	if(copy_from_user(&((char *) &ins)[offs], addr + offs, sizeof(ins) - offs))
+	if (copy_from_user(&ins, addr, sizeof(ins)))
 		return -EFAULT;
 
 	if (ins.channel < 0 || ins.channel >= SBFM_MAXINSTR)