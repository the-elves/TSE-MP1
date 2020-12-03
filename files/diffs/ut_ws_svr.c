--- files/before/ut_ws_svr.c	2020-12-03 05:29:10.571074527 +0530
+++ files/after/ut_ws_svr.c	2020-12-03 05:29:11.467217807 +0530
@@ -129,7 +129,7 @@
     if (upgrade == NULL || strcasecmp(upgrade, "websocket") != 0)
         goto error;
     const char *connection = http_request_get_header(info->request, "Connection");
-    if (connection == NULL)
+    if (connection == NULL || strlen(connection) > UT_WS_SVR_MAX_HEADER_SIZE)
         goto error;
     else {
         bool found_upgrade = false;
