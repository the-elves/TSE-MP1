--- files/before/spnego_mech.c	2020-12-03 05:27:11.868095975 +0530
+++ files/after/spnego_mech.c	2020-12-03 05:27:12.844252045 +0530
@@ -818,7 +818,6 @@
 	OM_uint32 tmpmin;
 	size_t i;
 
-	generic_gss_release_oid(&tmpmin, &sc->internal_mech);
 	gss_delete_sec_context(&tmpmin, &sc->ctx_handle,
 			       GSS_C_NO_BUFFER);
 
