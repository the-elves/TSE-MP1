--- files/before/wddx.c	2020-12-03 05:28:58.349120452 +0530
+++ files/after/wddx.c	2020-12-03 05:28:59.513306584 +0530
@@ -811,6 +811,11 @@
 				php_wddx_process_data(user_data, atts[i+1], strlen(atts[i+1]));
 				break;
 			}
+		} else {
+			ent.type = ST_BOOLEAN;
+			SET_STACK_VARNAME;
+			ZVAL_FALSE(&ent.data);
+			wddx_stack_push((wddx_stack *)stack, &ent, sizeof(st_entry));
 		}
 	} else if (!strcmp(name, EL_NULL)) {
 		ent.type = ST_NULL;
