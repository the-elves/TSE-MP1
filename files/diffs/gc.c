--- files/before/gc.c	2020-12-03 05:27:32.591409274 +0530
+++ files/after/gc.c	2020-12-03 05:27:33.483551912 +0530
@@ -148,12 +148,12 @@
 		if (test_bit(KEY_FLAG_INSTANTIATED, &key->flags))
 			atomic_dec(&key->user->nikeys);
 
-		key_user_put(key->user);
-
 		/* now throw away the key memory */
 		if (key->type->destroy)
 			key->type->destroy(key);
 
+		key_user_put(key->user);
+
 		kfree(key->description);
 
 #ifdef KEY_DEBUGGING
