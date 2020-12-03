--- files/before/gssapi_krb5.c	2020-12-03 05:27:28.670782432 +0530
+++ files/after/gssapi_krb5.c	2020-12-03 05:27:29.582928269 +0530
@@ -970,7 +970,7 @@
     NULL,
 #else
     iakerb_gss_export_sec_context,
-    NULL,
+    iakerb_gss_import_sec_context,
 #endif
     krb5_gss_inquire_cred_by_mech,
     krb5_gss_inquire_names_for_mech,
