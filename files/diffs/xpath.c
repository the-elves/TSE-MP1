--- files/before/xpath.c	2020-12-03 05:33:32.957025506 +0530
+++ files/after/xpath.c	2020-12-03 05:33:34.029196926 +0530
@@ -17,6 +17,7 @@
 #define IN_LIBXML
 #include "libxml.h"
 
+#include <limits.h>
 #include <string.h>
 
 #ifdef HAVE_SYS_TYPES_H
@@ -3105,7 +3106,8 @@
 		snprintf(buffer, buffersize, "NaN");
 	} else if (number == 0 && xmlXPathGetSign(number) != 0) {
 	    snprintf(buffer, buffersize, "0");
-	} else if (number == ((int) number)) {
+	} else if ((number > INT_MIN) && (number < INT_MAX) &&
+                   (number == (int) number)) {
 	    char work[30];
 	    char *ptr, *cur;
 	    int value = (int) number;
@@ -4021,13 +4023,7 @@
 		set1->nodeTab = temp;
 		set1->nodeMax *= 2;
 	    }
-	    if (n2->type == XML_NAMESPACE_DECL) {
-		xmlNsPtr ns = (xmlNsPtr) n2;
-
-		set1->nodeTab[set1->nodeNr++] =
-		    xmlXPathNodeSetDupNs((xmlNodePtr) ns->next, ns);
-	    } else
-		set1->nodeTab[set1->nodeNr++] = n2;
+	    set1->nodeTab[set1->nodeNr++] = n2;
 skip_node:
 	    {}
 	}
@@ -4196,33 +4192,6 @@
 }
 
 /**
- * xmlXPathNodeSetClear:
- * @set:  the node set to clear
- *
- * Clears the list from all temporary XPath objects (e.g. namespace nodes
- * are feed), but does *not* free the list itself. Sets the length of the
- * list to 0.
- */
-static void
-xmlXPathNodeSetClear(xmlNodeSetPtr set, int hasNsNodes)
-{
-    if ((set == NULL) || (set->nodeNr <= 0))
-	return;
-    else if (hasNsNodes) {
-	int i;
-	xmlNodePtr node;
-
-	for (i = 0; i < set->nodeNr; i++) {
-	    node = set->nodeTab[i];
-	    if ((node != NULL) &&
-		(node->type == XML_NAMESPACE_DECL))
-		xmlXPathNodeSetFreeNs((xmlNsPtr) node);
-	}
-    }
-    set->nodeNr = 0;
-}
-
-/**
  * xmlXPathNodeSetClearFromPos:
  * @set: the node set to be cleared
  * @pos: the start position to clear from
@@ -4234,7 +4203,7 @@
 static void
 xmlXPathNodeSetClearFromPos(xmlNodeSetPtr set, int pos, int hasNsNodes)
 {
-    if ((set == NULL) || (set->nodeNr <= 0) || (pos >= set->nodeNr))
+    if ((set == NULL) || (pos >= set->nodeNr))
 	return;
     else if ((hasNsNodes)) {
 	int i;
@@ -4251,6 +4220,46 @@
 }
 
 /**
+ * xmlXPathNodeSetClear:
+ * @set:  the node set to clear
+ *
+ * Clears the list from all temporary XPath objects (e.g. namespace nodes
+ * are feed), but does *not* free the list itself. Sets the length of the
+ * list to 0.
+ */
+static void
+xmlXPathNodeSetClear(xmlNodeSetPtr set, int hasNsNodes)
+{
+    xmlXPathNodeSetClearFromPos(set, 0, hasNsNodes);
+}
+
+/**
+ * xmlXPathNodeSetKeepLast:
+ * @set: the node set to be cleared
+ *
+ * Move the last node to the first position and clear temporary XPath objects
+ * (e.g. namespace nodes) from all other nodes. Sets the length of the list
+ * to 1.
+ */
+static void
+xmlXPathNodeSetKeepLast(xmlNodeSetPtr set)
+{
+    int i;
+    xmlNodePtr node;
+
+    if ((set == NULL) || (set->nodeNr <= 1))
+	return;
+    for (i = 0; i < set->nodeNr - 1; i++) {
+        node = set->nodeTab[i];
+        if ((node != NULL) &&
+            (node->type == XML_NAMESPACE_DECL))
+            xmlXPathNodeSetFreeNs((xmlNsPtr) node);
+    }
+    set->nodeTab[0] = set->nodeTab[set->nodeNr-1];
+    set->nodeNr = 1;
+}
+
+/**
  * xmlXPathFreeValueTree:
  * @obj:  the xmlNodeSetPtr to free
  *
@@ -6289,7 +6298,15 @@
  */
 void
 xmlXPathFreeParserContext(xmlXPathParserContextPtr ctxt) {
+    int i;
+
     if (ctxt->valueTab != NULL) {
+        for (i = 0; i < ctxt->valueNr; i++) {
+            if (ctxt->context)
+                xmlXPathReleaseObject(ctxt->context, ctxt->valueTab[i]);
+            else
+                xmlXPathFreeObject(ctxt->valueTab[i]);
+        }
         xmlFree(ctxt->valueTab);
     }
     if (ctxt->comp != NULL) {
@@ -6702,7 +6719,13 @@
 	    valuePush(ctxt, val);
 	    return(xmlXPathCompareValues(ctxt, inf, strict));
 	default:
-	    TODO
+            xmlGenericError(xmlGenericErrorContext,
+                    "xmlXPathCompareNodeSetValue: Can't compare node set "
+                    "and object of type %d\n",
+                    val->type);
+            xmlXPathReleaseObject(ctxt->context, arg);
+            xmlXPathReleaseObject(ctxt->context, val);
+            XP_ERROR0(XPATH_INVALID_TYPE);
     }
     return(0);
 }
@@ -8114,12 +8137,12 @@
 		return(NULL);
 	    return(cur->parent);
 	case XML_ATTRIBUTE_NODE: {
-	    xmlAttrPtr att = (xmlAttrPtr) ctxt->context->node;
+	    xmlAttrPtr att = (xmlAttrPtr) cur;
 
 	    return(att->parent);
 	}
 	case XML_NAMESPACE_DECL: {
-	    xmlNsPtr ns = (xmlNsPtr) ctxt->context->node;
+	    xmlNsPtr ns = (xmlNsPtr) cur;
 
 	    if ((ns->next != NULL) &&
 	        (ns->next->type != XML_NAMESPACE_DECL))
@@ -8236,10 +8259,16 @@
 
     if (cur == NULL) {
         cur = ctxt->context->node;
-        if (cur->type == XML_NAMESPACE_DECL)
-            return(NULL);
-        if (cur->type == XML_ATTRIBUTE_NODE)
+        if (cur->type == XML_ATTRIBUTE_NODE) {
             cur = cur->parent;
+        } else if (cur->type == XML_NAMESPACE_DECL) {
+            xmlNsPtr ns = (xmlNsPtr) cur;
+
+            if ((ns->next == NULL) ||
+                (ns->next->type == XML_NAMESPACE_DECL))
+                return (NULL);
+            cur = (xmlNodePtr) ns->next;
+        }
     }
     if (cur == NULL) return(NULL) ; /* ERROR */
     if (cur->next != NULL) return(cur->next) ;
@@ -8300,10 +8329,16 @@
     if ((ctxt == NULL) || (ctxt->context == NULL)) return(NULL);
     if (cur == NULL) {
         cur = ctxt->context->node;
-        if (cur->type == XML_NAMESPACE_DECL)
-            return(NULL);
-        if (cur->type == XML_ATTRIBUTE_NODE)
-            return(cur->parent);
+        if (cur->type == XML_ATTRIBUTE_NODE) {
+            cur = cur->parent;
+        } else if (cur->type == XML_NAMESPACE_DECL) {
+            xmlNsPtr ns = (xmlNsPtr) cur;
+
+            if ((ns->next == NULL) ||
+                (ns->next->type == XML_NAMESPACE_DECL))
+                return (NULL);
+            cur = (xmlNodePtr) ns->next;
+        }
     }
     if ((cur == NULL) || (cur->type == XML_NAMESPACE_DECL))
 	return (NULL);
@@ -8348,8 +8383,16 @@
         cur = ctxt->context->node;
         if (cur == NULL)
             return (NULL);
-        if (cur->type == XML_NAMESPACE_DECL)
-            return (NULL);
+        if (cur->type == XML_ATTRIBUTE_NODE) {
+            cur = cur->parent;
+        } else if (cur->type == XML_NAMESPACE_DECL) {
+            xmlNsPtr ns = (xmlNsPtr) cur;
+
+            if ((ns->next == NULL) ||
+                (ns->next->type == XML_NAMESPACE_DECL))
+                return (NULL);
+            cur = (xmlNodePtr) ns->next;
+        }
         ctxt->ancestor = cur->parent;
     }
     if (cur->type == XML_NAMESPACE_DECL)
@@ -9634,18 +9677,6 @@
     xmlXPathReleaseObject(ctxt->context, cur);
 }
 
-/*
- * To assure working code on multiple platforms, we want to only depend
- * upon the characteristic truncation of converting a floating point value
- * to an integer.  Unfortunately, because of the different storage sizes
- * of our internal floating point value (double) and integer (int), we
- * can't directly convert (see bug 301162).  This macro is a messy
- * 'workaround'
- */
-#define XTRUNC(f, v)            \
-    f = fmod((v), INT_MAX);     \
-    f = (v) - (f) + (double)((int)(f));
-
 /**
  * xmlXPathFloorFunction:
  * @ctxt:  the XPath Parser context
@@ -9658,19 +9689,11 @@
  */
 void
 xmlXPathFloorFunction(xmlXPathParserContextPtr ctxt, int nargs) {
-    double f;
-
     CHECK_ARITY(1);
     CAST_TO_NUMBER;
     CHECK_TYPE(XPATH_NUMBER);
 
-    XTRUNC(f, ctxt->value->floatval);
-    if (f != ctxt->value->floatval) {
-	if (ctxt->value->floatval > 0)
-	    ctxt->value->floatval = f;
-	else
-	    ctxt->value->floatval = f - 1;
-    }
+    ctxt->value->floatval = floor(ctxt->value->floatval);
 }
 
 /**
@@ -9685,28 +9708,11 @@
  */
 void
 xmlXPathCeilingFunction(xmlXPathParserContextPtr ctxt, int nargs) {
-    double f;
-
     CHECK_ARITY(1);
     CAST_TO_NUMBER;
     CHECK_TYPE(XPATH_NUMBER);
 
-#if 0
     ctxt->value->floatval = ceil(ctxt->value->floatval);
-#else
-    XTRUNC(f, ctxt->value->floatval);
-    if (f != ctxt->value->floatval) {
-	if (ctxt->value->floatval > 0)
-	    ctxt->value->floatval = f + 1;
-	else {
-	    if (ctxt->value->floatval < 0 && f == 0)
-	        ctxt->value->floatval = xmlXPathNZERO;
-	    else
-	        ctxt->value->floatval = f;
-	}
-
-    }
-#endif
 }
 
 /**
@@ -9718,7 +9724,7 @@
  *    number round(number)
  * The round function returns the number that is closest to the
  * argument and that is an integer. If there are two such numbers,
- * then the one that is even is returned.
+ * then the one that is closest to positive infinity is returned.
  */
 void
 xmlXPathRoundFunction(xmlXPathParserContextPtr ctxt, int nargs) {
@@ -9728,25 +9734,21 @@
     CAST_TO_NUMBER;
     CHECK_TYPE(XPATH_NUMBER);
 
-    if ((xmlXPathIsNaN(ctxt->value->floatval)) ||
-	(xmlXPathIsInf(ctxt->value->floatval) == 1) ||
-	(xmlXPathIsInf(ctxt->value->floatval) == -1) ||
-	(ctxt->value->floatval == 0.0))
+    f = ctxt->value->floatval;
+
+    /* Test for zero to keep negative zero unchanged. */
+    if ((xmlXPathIsNaN(f)) || (f == 0.0))
 	return;
 
-    XTRUNC(f, ctxt->value->floatval);
-    if (ctxt->value->floatval < 0) {
-	if (ctxt->value->floatval < f - 0.5)
-	    ctxt->value->floatval = f - 1;
-	else
-	    ctxt->value->floatval = f;
-	if (ctxt->value->floatval == 0)
-	    ctxt->value->floatval = xmlXPathNZERO;
-    } else {
-	if (ctxt->value->floatval < f + 0.5)
-	    ctxt->value->floatval = f;
-	else
-	    ctxt->value->floatval = f + 1;
+    if ((f >= -0.5) && (f < 0.0)) {
+        /* Negative zero. */
+        ctxt->value->floatval = xmlXPathNZERO;
+    }
+    else {
+        double rounded = floor(f);
+        if (f - rounded >= 0.5)
+            rounded += 1.0;
+        ctxt->value->floatval = rounded;
     }
 }
 
@@ -10056,20 +10058,6 @@
 
 #define MAX_FRAC 20
 
-/*
- * These are used as divisors for the fractional part of a number.
- * Since the table includes 1.0 (representing '0' fractional digits),
- * it must be dimensioned at MAX_FRAC+1 (bug 133921)
- */
-static double my_pow10[MAX_FRAC+1] = {
-    1.0, 10.0, 100.0, 1000.0, 10000.0,
-    100000.0, 1000000.0, 10000000.0, 100000000.0, 1000000000.0,
-    10000000000.0, 100000000000.0, 1000000000000.0, 10000000000000.0,
-    100000000000000.0,
-    1000000000000000.0, 10000000000000000.0, 100000000000000000.0,
-    1000000000000000000.0, 10000000000000000000.0, 100000000000000000000.0
-};
-
 /**
  * xmlXPathStringEvalNumber:
  * @str:  A string to scan
@@ -10132,20 +10120,25 @@
 #endif
 
     if (*cur == '.') {
-	int v, frac = 0;
+	int v, frac = 0, max;
 	double fraction = 0;
 
         cur++;
 	if (((*cur < '0') || (*cur > '9')) && (!ok)) {
 	    return(xmlXPathNAN);
 	}
-	while (((*cur >= '0') && (*cur <= '9')) && (frac < MAX_FRAC)) {
+        while (*cur == '0') {
+	    frac = frac + 1;
+	    cur++;
+        }
+        max = frac + MAX_FRAC;
+	while (((*cur >= '0') && (*cur <= '9')) && (frac < max)) {
 	    v = (*cur - '0');
 	    fraction = fraction * 10 + v;
 	    frac = frac + 1;
 	    cur++;
 	}
-	fraction /= my_pow10[frac];
+	fraction /= pow(10.0, frac);
 	ret = ret + fraction;
 	while ((*cur >= '0') && (*cur <= '9'))
 	    cur++;
@@ -10159,7 +10152,8 @@
         cur++;
       }
       while ((*cur >= '0') && (*cur <= '9')) {
-	exponent = exponent * 10 + (*cur - '0');
+        if (exponent < 1000000)
+	  exponent = exponent * 10 + (*cur - '0');
 	cur++;
       }
     }
@@ -10221,20 +10215,25 @@
     }
 #endif
     if (CUR == '.') {
-	int v, frac = 0;
+	int v, frac = 0, max;
 	double fraction = 0;
 
         NEXT;
         if (((CUR < '0') || (CUR > '9')) && (!ok)) {
             XP_ERROR(XPATH_NUMBER_ERROR);
         }
-        while ((CUR >= '0') && (CUR <= '9') && (frac < MAX_FRAC)) {
+        while (CUR == '0') {
+            frac = frac + 1;
+            NEXT;
+        }
+        max = frac + MAX_FRAC;
+        while ((CUR >= '0') && (CUR <= '9') && (frac < max)) {
 	    v = (CUR - '0');
 	    fraction = fraction * 10 + v;
 	    frac = frac + 1;
             NEXT;
         }
-        fraction /= my_pow10[frac];
+        fraction /= pow(10.0, frac);
         ret = ret + fraction;
         while ((CUR >= '0') && (CUR <= '9'))
             NEXT;
@@ -10248,7 +10247,8 @@
 	    NEXT;
 	}
         while ((CUR >= '0') && (CUR <= '9')) {
-            exponent = exponent * 10 + (CUR - '0');
+            if (exponent < 1000000)
+                exponent = exponent * 10 + (CUR - '0');
             NEXT;
         }
         if (is_exponent_negative)
@@ -10379,6 +10379,7 @@
     NEXT;
     name = xmlXPathParseQName(ctxt, &prefix);
     if (name == NULL) {
+        xmlFree(prefix);
 	XP_ERROR(XPATH_VARIABLE_REF_ERROR);
     }
     ctxt->comp->last = -1;
@@ -10452,6 +10453,8 @@
 #endif
 
     if (CUR != '(') {
+	xmlFree(name);
+	xmlFree(prefix);
 	XP_ERROR(XPATH_EXPR_ERROR);
     }
     NEXT;
@@ -10480,6 +10483,8 @@
 	    nbargs++;
 	    if (CUR == ')') break;
 	    if (CUR != ',') {
+		xmlFree(name);
+		xmlFree(prefix);
 		XP_ERROR(XPATH_EXPR_ERROR);
 	    }
 	    NEXT;
@@ -12054,6 +12059,8 @@
 	(exprOp->value4 != NULL) &&
 	(((xmlXPathObjectPtr) exprOp->value4)->type == XPATH_NUMBER))
     {
+        double floatval = ((xmlXPathObjectPtr) exprOp->value4)->floatval;
+
 	/*
 	* We have a "[n]" predicate here.
 	* TODO: Unfortunately this simplistic test here is not
@@ -12064,13 +12071,12 @@
 	* like it "[position() < 5]", is also not detected.
 	* Maybe we could rewrite the AST to ease the optimization.
 	*/
-	*maxPos = (int) ((xmlXPathObjectPtr) exprOp->value4)->floatval;
 
-	if (((xmlXPathObjectPtr) exprOp->value4)->floatval ==
-	    (float) *maxPos)
-	{
-	    return(1);
-	}
+        if ((floatval > INT_MIN) && (floatval < INT_MAX)) {
+	    *maxPos = (int) floatval;
+            if (floatval == (double) *maxPos)
+                return(1);
+        }
     }
     return(0);
 }
@@ -12772,11 +12778,15 @@
                 xmlXPathCompOpEvalFirst(ctxt, &comp->steps[op->ch2],
                                         first);
 	    CHECK_ERROR0;
-            CHECK_TYPE0(XPATH_NODESET);
-            arg2 = valuePop(ctxt);
 
-            CHECK_TYPE0(XPATH_NODESET);
+            arg2 = valuePop(ctxt);
             arg1 = valuePop(ctxt);
+            if ((arg1 == NULL) || (arg1->type != XPATH_NODESET) ||
+                (arg2 == NULL) || (arg2->type != XPATH_NODESET)) {
+	        xmlXPathReleaseObject(ctxt->context, arg1);
+	        xmlXPathReleaseObject(ctxt->context, arg2);
+                XP_ERROR0(XPATH_INVALID_TYPE);
+            }
 
             arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
                                                     arg2->nodesetval);
@@ -12907,11 +12917,15 @@
                 && (ctxt->value->nodesetval != NULL)
                 && (ctxt->value->nodesetval->nodeNr >= 1)) { /* TODO: NOP ? */
             }
-            CHECK_TYPE0(XPATH_NODESET);
-            arg2 = valuePop(ctxt);
 
-            CHECK_TYPE0(XPATH_NODESET);
+            arg2 = valuePop(ctxt);
             arg1 = valuePop(ctxt);
+            if ((arg1 == NULL) || (arg1->type != XPATH_NODESET) ||
+                (arg2 == NULL) || (arg2->type != XPATH_NODESET)) {
+	        xmlXPathReleaseObject(ctxt->context, arg1);
+	        xmlXPathReleaseObject(ctxt->context, arg2);
+                XP_ERROR0(XPATH_INVALID_TYPE);
+            }
 
             arg1->nodesetval = xmlXPathNodeSetMerge(arg1->nodesetval,
                                                     arg2->nodesetval);
@@ -13022,13 +13036,7 @@
 		(ctxt->value->nodesetval != NULL) &&
 		(ctxt->value->nodesetval->nodeTab != NULL) &&
 		(ctxt->value->nodesetval->nodeNr > 1)) {
-		ctxt->value->nodesetval->nodeTab[0] =
-		    ctxt->value->nodesetval->nodeTab[ctxt->
-		    value->
-		    nodesetval->
-		    nodeNr -
-		    1];
-		ctxt->value->nodesetval->nodeNr = 1;
+                xmlXPathNodeSetKeepLast(ctxt->value->nodesetval);
 		*first = *(ctxt->value->nodesetval->nodeTab);
 	    }
 	    return (total);
@@ -13445,11 +13453,15 @@
 	    ctxt->context->contextSize = cs;
             total += xmlXPathCompOpEval(ctxt, &comp->steps[op->ch2]);
 	    CHECK_ERROR0;
-            CHECK_TYPE0(XPATH_NODESET);
-            arg2 = valuePop(ctxt);
 
-            CHECK_TYPE0(XPATH_NODESET);
+            arg2 = valuePop(ctxt);
             arg1 = valuePop(ctxt);
+            if ((arg1 == NULL) || (arg1->type != XPATH_NODESET) ||
+                (arg2 == NULL) || (arg2->type != XPATH_NODESET)) {
+	        xmlXPathReleaseObject(ctxt->context, arg1);
+	        xmlXPathReleaseObject(ctxt->context, arg2);
+                XP_ERROR0(XPATH_INVALID_TYPE);
+            }
 
 	    if ((arg1->nodesetval == NULL) ||
 		((arg2->nodesetval != NULL) &&
@@ -13677,7 +13689,8 @@
                             (ctxt->value->type == XPATH_NODESET) &&
                             (ctxt->value->nodesetval != NULL) &&
                             (ctxt->value->nodesetval->nodeNr > 1))
-                            ctxt->value->nodesetval->nodeNr = 1;
+                            xmlXPathNodeSetClearFromPos(ctxt->value->nodesetval,
+                                                        1, 1);
                         return (total);
                     }
                 }
@@ -13711,15 +13724,8 @@
                             (ctxt->value->type == XPATH_NODESET) &&
                             (ctxt->value->nodesetval != NULL) &&
                             (ctxt->value->nodesetval->nodeTab != NULL) &&
-                            (ctxt->value->nodesetval->nodeNr > 1)) {
-                            ctxt->value->nodesetval->nodeTab[0] =
-                                ctxt->value->nodesetval->nodeTab[ctxt->
-                                                                 value->
-                                                                 nodesetval->
-                                                                 nodeNr -
-                                                                 1];
-                            ctxt->value->nodesetval->nodeNr = 1;
-                        }
+                            (ctxt->value->nodesetval->nodeNr > 1))
+                            xmlXPathNodeSetKeepLast(ctxt->value->nodesetval);
                         return (total);
                     }
                 }
@@ -14908,10 +14914,11 @@
 static int
 xmlXPathCompiledEvalInternal(xmlXPathCompExprPtr comp,
 			     xmlXPathContextPtr ctxt,
-			     xmlXPathObjectPtr *resObj,
+			     xmlXPathObjectPtr *resObjPtr,
 			     int toBool)
 {
     xmlXPathParserContextPtr pctxt;
+    xmlXPathObjectPtr resObj;
 #ifndef LIBXML_THREAD_ENABLED
     static int reentance = 0;
 #endif
@@ -14939,43 +14946,26 @@
     pctxt = xmlXPathCompParserContext(comp, ctxt);
     res = xmlXPathRunEval(pctxt, toBool);
 
-    if (resObj) {
-	if (pctxt->value == NULL) {
-	    xmlGenericError(xmlGenericErrorContext,
-		"xmlXPathCompiledEval: evaluation failed\n");
-	    *resObj = NULL;
-	} else {
-	    *resObj = valuePop(pctxt);
-	}
+    if (pctxt->error != XPATH_EXPRESSION_OK) {
+        resObj = NULL;
+    } else {
+        resObj = valuePop(pctxt);
+        if (resObj == NULL) {
+            if (!toBool)
+                xmlGenericError(xmlGenericErrorContext,
+                    "xmlXPathCompiledEval: No result on the stack.\n");
+        } else if (pctxt->valueNr > 0) {
+            xmlGenericError(xmlGenericErrorContext,
+                "xmlXPathCompiledEval: %d object(s) left on the stack.\n",
+                pctxt->valueNr);
+        }
     }
 
-    /*
-    * Pop all remaining objects from the stack.
-    */
-    if (pctxt->valueNr > 0) {
-	xmlXPathObjectPtr tmp;
-	int stack = 0;
-
-	do {
-	    tmp = valuePop(pctxt);
-	    if (tmp != NULL) {
-		stack++;
-		xmlXPathReleaseObject(ctxt, tmp);
-	    }
-	} while (tmp != NULL);
-	if ((stack != 0) &&
-	    ((toBool) || ((resObj) && (*resObj))))
-	{
-	    xmlGenericError(xmlGenericErrorContext,
-		"xmlXPathCompiledEval: %d objects left on the stack.\n",
-		stack);
-	}
-    }
+    if (resObjPtr)
+        *resObjPtr = resObj;
+    else
+        xmlXPathReleaseObject(ctxt, resObj);
 
-    if ((pctxt->error != XPATH_EXPRESSION_OK) && (resObj) && (*resObj)) {
-	xmlXPathFreeObject(*resObj);
-	*resObj = NULL;
-    }
     pctxt->comp = NULL;
     xmlXPathFreeParserContext(pctxt);
 #ifndef LIBXML_THREAD_ENABLED
@@ -15043,22 +15033,21 @@
         if (ctxt->comp != NULL)
 	    xmlXPathFreeCompExpr(ctxt->comp);
         ctxt->comp = comp;
-	if (ctxt->cur != NULL)
-	    while (*ctxt->cur != 0) ctxt->cur++;
     } else
 #endif
     {
 	xmlXPathCompileExpr(ctxt, 1);
-	if ((ctxt->error == XPATH_EXPRESSION_OK) &&
-	    (ctxt->comp != NULL) &&
-	    (ctxt->comp->nbStep > 1) &&
-	    (ctxt->comp->last >= 0))
-	{
+        CHECK_ERROR;
+
+        /* Check for trailing characters. */
+        if (*ctxt->cur != 0)
+            XP_ERROR(XPATH_EXPR_ERROR);
+
+	if ((ctxt->comp->nbStep > 1) && (ctxt->comp->last >= 0))
 	    xmlXPathOptimizeExpression(ctxt->comp,
 		&ctxt->comp->steps[ctxt->comp->last]);
-	}
     }
-    CHECK_ERROR;
+
     xmlXPathRunEval(ctxt, 0);
 }
 
@@ -15075,8 +15064,7 @@
 xmlXPathObjectPtr
 xmlXPathEval(const xmlChar *str, xmlXPathContextPtr ctx) {
     xmlXPathParserContextPtr ctxt;
-    xmlXPathObjectPtr res, tmp, init = NULL;
-    int stack = 0;
+    xmlXPathObjectPtr res;
 
     CHECK_CTXT(ctx)
 
@@ -15087,37 +15075,18 @@
         return NULL;
     xmlXPathEvalExpr(ctxt);
 
-    if (ctxt->value == NULL) {
-	xmlGenericError(xmlGenericErrorContext,
-		"xmlXPathEval: evaluation failed\n");
-	res = NULL;
-    } else if ((*ctxt->cur != 0) && (ctxt->comp != NULL)
-#ifdef XPATH_STREAMING
-            && (ctxt->comp->stream == NULL)
-#endif
-	      ) {
-	xmlXPatherror(ctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
+    if (ctxt->error != XPATH_EXPRESSION_OK) {
 	res = NULL;
     } else {
 	res = valuePop(ctxt);
-    }
-
-    do {
-        tmp = valuePop(ctxt);
-	if (tmp != NULL) {
-	    if (tmp != init)
-		stack++;
-	    xmlXPathReleaseObject(ctx, tmp);
+        if (res == NULL) {
+            xmlGenericError(xmlGenericErrorContext,
+                "xmlXPathCompiledEval: No result on the stack.\n");
+        } else if (ctxt->valueNr > 0) {
+            xmlGenericError(xmlGenericErrorContext,
+                "xmlXPathCompiledEval: %d object(s) left on the stack.\n",
+                ctxt->valueNr);
         }
-    } while (tmp != NULL);
-    if ((stack != 0) && (res != NULL)) {
-	xmlGenericError(xmlGenericErrorContext,
-		"xmlXPathEval: %d object left on the stack\n",
-	        stack);
-    }
-    if (ctxt->error != XPATH_EXPRESSION_OK) {
-	xmlXPathFreeObject(res);
-	res = NULL;
     }
 
     xmlXPathFreeParserContext(ctxt);
@@ -15172,46 +15141,11 @@
  * @str:  the XPath expression
  * @ctxt:  the XPath context
  *
- * Evaluate the XPath expression in the given context.
- *
- * Returns the xmlXPathObjectPtr resulting from the evaluation or NULL.
- *         the caller has to free the object.
+ * Alias for xmlXPathEval.
  */
 xmlXPathObjectPtr
 xmlXPathEvalExpression(const xmlChar *str, xmlXPathContextPtr ctxt) {
-    xmlXPathParserContextPtr pctxt;
-    xmlXPathObjectPtr res, tmp;
-    int stack = 0;
-
-    CHECK_CTXT(ctxt)
-
-    xmlXPathInit();
-
-    pctxt = xmlXPathNewParserContext(str, ctxt);
-    if (pctxt == NULL)
-        return NULL;
-    xmlXPathEvalExpr(pctxt);
-
-    if ((*pctxt->cur != 0) || (pctxt->error != XPATH_EXPRESSION_OK)) {
-	xmlXPatherror(pctxt, __FILE__, __LINE__, XPATH_EXPR_ERROR);
-	res = NULL;
-    } else {
-	res = valuePop(pctxt);
-    }
-    do {
-        tmp = valuePop(pctxt);
-	if (tmp != NULL) {
-	    xmlXPathReleaseObject(ctxt, tmp);
-	    stack++;
-	}
-    } while (tmp != NULL);
-    if ((stack != 0) && (res != NULL)) {
-	xmlGenericError(xmlGenericErrorContext,
-		"xmlXPathEvalExpression: %d object left on the stack\n",
-	        stack);
-    }
-    xmlXPathFreeParserContext(pctxt);
-    return(res);
+    return(xmlXPathEval(str, ctxt));
 }
 
 /************************************************************************
