--- files/before/SimplifyMarkupCommand.cpp	2020-12-03 05:31:53.209077532 +0530
+++ files/after/SimplifyMarkupCommand.cpp	2020-12-03 05:31:54.193234883 +0530
@@ -77,7 +77,7 @@
             }
 
             unsigned context;
-            if (currentNode->renderStyle()->visualInvalidationDiff(*startingStyle, context) == StyleDifferenceEqual && !context)
+            if (currentNode->renderStyle()->visualInvalidationDiff(*startingStyle, context).hasNoChange() && !context)
                 topNodeWithStartingStyle = currentNode;
 
         }
