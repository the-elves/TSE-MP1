--- files/before/iscsi_target_parameters.c	2020-12-03 05:27:00.482275579 +0530
+++ files/after/iscsi_target_parameters.c	2020-12-03 05:27:01.454431009 +0530
@@ -758,9 +758,9 @@
 	}
 	INIT_LIST_HEAD(&extra_response->er_list);
 
-	strncpy(extra_response->key, key, strlen(key) + 1);
-	strncpy(extra_response->value, NOTUNDERSTOOD,
-			strlen(NOTUNDERSTOOD) + 1);
+	strlcpy(extra_response->key, key, sizeof(extra_response->key));
+	strlcpy(extra_response->value, NOTUNDERSTOOD,
+		sizeof(extra_response->value));
 
 	list_add_tail(&extra_response->er_list,
 			&param_list->extra_response_list);
@@ -1629,8 +1629,6 @@
 
 		if (phase & PHASE_SECURITY) {
 			if (iscsi_check_for_auth_key(key) > 0) {
-				char *tmpptr = key + strlen(key);
-				*tmpptr = '=';
 				kfree(tmpbuf);
 				return 1;
 			}
