--- files/before/tree.c	2020-12-03 05:33:20.611051598 +0530
+++ files/after/tree.c	2020-12-03 05:33:21.775237731 +0530
@@ -1401,6 +1401,8 @@
 			else if ((ent != NULL) && (ent->children == NULL)) {
 			    xmlNodePtr temp;
 
+                            /* Set to non-NULL value to avoid recursion. */
+			    ent->children = (xmlNodePtr) -1;
 			    ent->children = xmlStringGetNodeList(doc,
 				    (const xmlChar*)node->content);
 			    ent->owner = 1;
@@ -1593,6 +1595,7 @@
 			else if ((ent != NULL) && (ent->children == NULL)) {
 			    xmlNodePtr temp;
 
+                            /* Set to non-NULL value to avoid recursion. */
 			    ent->children = (xmlNodePtr) -1;
 			    ent->children = xmlStringGetNodeList(doc,
 				    (const xmlChar*)node->content);
@@ -1600,6 +1603,7 @@
 			    temp = ent->children;
 			    while (temp) {
 				temp->parent = (xmlNodePtr)ent;
+				ent->last = temp;
 				temp = temp->next;
 			    }
 			}
