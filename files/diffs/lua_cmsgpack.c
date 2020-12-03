--- files/before/lua_cmsgpack.c	2020-12-03 05:29:14.603719277 +0530
+++ files/after/lua_cmsgpack.c	2020-12-03 05:29:15.895925879 +0530
@@ -515,6 +515,9 @@
     if (nargs == 0)
         return luaL_argerror(L, 0, "MessagePack pack needs input.");
 
+    if (!lua_checkstack(L, nargs))
+        return luaL_argerror(L, 0, "Too many arguments for MessagePack pack.");
+
     buf = mp_buf_new(L);
     for(i = 1; i <= nargs; i++) {
         /* Copy argument i to top of stack for _encode processing;
