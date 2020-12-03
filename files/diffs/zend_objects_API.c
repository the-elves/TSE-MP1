--- files/before/zend_objects_API.c	2020-12-03 05:27:41.896897061 +0530
+++ files/after/zend_objects_API.c	2020-12-03 05:27:42.825045455 +0530
@@ -215,7 +215,7 @@
 					} zend_end_try();
 				}
 			}
-			
+
 			/* re-read the object from the object store as the store might have been reallocated in the dtor */
 			obj = &EG(objects_store).object_buckets[handle].bucket.obj;
 
@@ -306,8 +306,8 @@
 {
 	zend_object_handle handle = Z_OBJ_HANDLE_P(zobject);
 	zend_object_store_bucket *obj_bucket = &EG(objects_store).object_buckets[handle];
-	
-	obj_bucket->bucket.obj.handlers = Z_OBJ_HT_P(zobject);;
+
+	obj_bucket->bucket.obj.handlers = Z_OBJ_HT_P(zobject);
 	obj_bucket->destructor_called = 1;
 }
 
