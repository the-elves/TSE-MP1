--- files/before/upnpreplyparse.c	2020-12-03 05:29:29.302069289 +0530
+++ files/after/upnpreplyparse.c	2020-12-03 05:29:30.174208729 +0530
@@ -1,7 +1,8 @@
 /* $Id: upnpreplyparse.c,v 1.19 2015/07/15 10:29:11 nanard Exp $ */
-/* MiniUPnP project
+/* vim: tabstop=4 shiftwidth=4 noexpandtab
+ * MiniUPnP project
  * http://miniupnp.free.fr/ or http://miniupnp.tuxfamily.org/
- * (c) 2006-2015 Thomas Bernard
+ * (c) 2006-2017 Thomas Bernard
  * This software is subject to the conditions detailed
  * in the LICENCE file provided within the distribution */
 
@@ -104,9 +105,7 @@
                struct NameValueParserData * data)
 {
 	struct xmlparser parser;
-	data->l_head = NULL;
-	data->portListing = NULL;
-	data->portListingLength = 0;
+	memset(data, 0, sizeof(struct NameValueParserData));
 	/* init xmlparser object */
 	parser.xmlstart = buffer;
 	parser.xmlsize = bufsize;
