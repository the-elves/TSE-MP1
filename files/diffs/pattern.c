--- files/before/pattern.c	2020-12-03 05:33:13.913980850 +0530
+++ files/after/pattern.c	2020-12-03 05:33:14.894137561 +0530
@@ -969,6 +969,7 @@
 		ERROR5(NULL, NULL, NULL,
 		    "xmlCompileAttributeTest : no namespace bound to prefix %s\n",
 		    prefix);
+	        XML_PAT_FREE_STRING(ctxt, prefix);
 		ctxt->error = 1;
 		goto error;
 	    }
