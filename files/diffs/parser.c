--- files/before/parser.c	2020-12-03 05:33:09.829327785 +0530
+++ files/after/parser.c	2020-12-03 05:33:10.965509441 +0530
@@ -43,6 +43,7 @@
 #include <limits.h>
 #include <string.h>
 #include <stdarg.h>
+#include <stddef.h>
 #include <libxml/xmlmemory.h>
 #include <libxml/threads.h>
 #include <libxml/globals.h>
@@ -1093,7 +1094,12 @@
 struct _xmlDefAttrs {
     int nbAttrs;	/* number of defaulted attributes on that element */
     int maxAttrs;       /* the size of the array */
-    const xmlChar *values[5]; /* array of localname/prefix/values/external */
+#if __STDC_VERSION__ >= 199901L
+    /* Using a C99 flexible array member avoids UBSan errors. */
+    const xmlChar *values[]; /* array of localname/prefix/values/external */
+#else
+    const xmlChar *values[5];
+#endif
 };
 
 /**
@@ -2121,7 +2127,6 @@
 	ctxt->input->line++; ctxt->input->col = 1;			\
     } else ctxt->input->col++;						\
     ctxt->input->cur += l;				\
-    if (*ctxt->input->cur == '%') xmlParserHandlePEReference(ctxt);	\
   } while (0)
 
 #define CUR_CHAR(l) xmlCurrentChar(ctxt, &l)
@@ -3412,13 +3417,6 @@
 	    len += l;
 	    NEXTL(l);
 	    c = CUR_CHAR(l);
-	    if (c == 0) {
-		count = 0;
-		GROW;
-                if (ctxt->instate == XML_PARSER_EOF)
-                    return(NULL);
-		c = CUR_CHAR(l);
-	    }
 	}
     }
     if ((len > XML_MAX_NAME_LENGTH) &&
@@ -3426,15 +3424,18 @@
         xmlFatalErr(ctxt, XML_ERR_NAME_TOO_LONG, "Name");
         return(NULL);
     }
-    if (ctxt->input->cur > ctxt->input->base && (*ctxt->input->cur == '\n') && (ctxt->input->cur[-1] == '\r')) {
-        if (ctxt->input->base > ctxt->input->cur - (len + 1)) {
-            return(NULL);
-        }
-        return(xmlDictLookup(ctxt->dict, ctxt->input->cur - (len + 1), len));
-    }
-    if (ctxt->input->base > ctxt->input->cur - len) {
-        return(NULL);
+    if (ctxt->input->cur - ctxt->input->base < len) {
+        /*
+         * There were a couple of bugs where PERefs lead to to a change
+         * of the buffer. Check the buffer size to avoid passing an invalid
+         * pointer to xmlDictLookup.
+         */
+        xmlFatalErr(ctxt, XML_ERR_INTERNAL_ERROR,
+                    "unexpected change of input buffer");
+        return (NULL);
     }
+    if ((*ctxt->input->cur == '\n') && (ctxt->input->cur[-1] == '\r'))
+        return(xmlDictLookup(ctxt->dict, ctxt->input->cur - (len + 1), len));
     return(xmlDictLookup(ctxt->dict, ctxt->input->cur - len, len));
 }
 
@@ -5718,7 +5719,7 @@
 	    }
 	}
 	if (ctxt->instate == XML_PARSER_EOF)
-	    return;
+	    goto done;
 	SKIP_BLANKS;
 	if (RAW != '>') {
 	    xmlFatalErrMsgStr(ctxt, XML_ERR_ENTITY_NOT_FINISHED,
@@ -5749,17 +5750,17 @@
 		    cur = xmlSAX2GetEntity(ctxt, name);
 		}
 	    }
-            if (cur != NULL) {
-	        if (cur->orig != NULL)
-		    xmlFree(orig);
-		else
-		    cur->orig = orig;
-	    } else
-		xmlFree(orig);
+            if ((cur != NULL) && (cur->orig == NULL)) {
+		cur->orig = orig;
+                orig = NULL;
+	    }
 	}
+
+done:
 	if (value != NULL) xmlFree(value);
 	if (URI != NULL) xmlFree(URI);
 	if (literal != NULL) xmlFree(literal);
+        if (orig != NULL) xmlFree(orig);
     }
 }
 
@@ -6112,7 +6113,6 @@
 	SKIP_BLANKS;
 	GROW;
 	while ((RAW != '>') && (ctxt->instate != XML_PARSER_EOF)) {
-	    const xmlChar *check = CUR_PTR;
 	    int type;
 	    int def;
 	    xmlChar *defaultValue = NULL;
@@ -6172,15 +6172,6 @@
 		}
 		SKIP_BLANKS;
 	    }
-	    if (check == CUR_PTR) {
-		xmlFatalErr(ctxt, XML_ERR_INTERNAL_ERROR,
-		            "in xmlParseAttributeListDecl\n");
-		if (defaultValue != NULL)
-		    xmlFree(defaultValue);
-	        if (tree != NULL)
-		    xmlFreeEnumeration(tree);
-		break;
-	    }
 	    if ((ctxt->sax != NULL) && (!ctxt->disableSAX) &&
 		(ctxt->sax->attributeDecl != NULL))
 		ctxt->sax->attributeDecl(ctxt->userData, elemName, attrName,
@@ -6285,7 +6276,7 @@
 	    if (elem == NULL) {
 		xmlFatalErrMsg(ctxt, XML_ERR_NAME_REQUIRED,
 			"xmlParseElementMixedContentDecl : Name expected\n");
-		xmlFreeDocElementContent(ctxt->myDoc, cur);
+		xmlFreeDocElementContent(ctxt->myDoc, ret);
 		return(NULL);
 	    }
 	    SKIP_BLANKS;
@@ -8353,6 +8344,7 @@
 	entity = ctxt->sax->getParameterEntity(ctxt->userData, name);
     if (ctxt->instate == XML_PARSER_EOF) {
 	xmlFree(name);
+	*str = ptr;
 	return(NULL);
     }
     if (entity == NULL) {
@@ -9410,8 +9402,7 @@
     const xmlChar **atts = ctxt->atts;
     int maxatts = ctxt->maxatts;
     int nratts, nbatts, nbdef;
-    int i, j, nbNs, attval, oldline, oldcol, inputNr;
-    const xmlChar *base;
+    int i, j, nbNs, attval;
     unsigned long cur;
     int nsNr = ctxt->nsNr;
 
@@ -9425,13 +9416,8 @@
      *       The Shrinking is only possible once the full set of attribute
      *       callbacks have been done.
      */
-reparse:
     SHRINK;
-    base = ctxt->input->base;
     cur = ctxt->input->cur - ctxt->input->base;
-    inputNr = ctxt->inputNr;
-    oldline = ctxt->input->line;
-    oldcol = ctxt->input->col;
     nbatts = 0;
     nratts = 0;
     nbdef = 0;
@@ -9455,8 +9441,6 @@
      */
     SKIP_BLANKS;
     GROW;
-    if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr))
-        goto base_changed;
 
     while (((RAW != '>') &&
 	   ((RAW != '/') || (NXT(1) != '>')) &&
@@ -9467,203 +9451,174 @@
 
 	attname = xmlParseAttribute2(ctxt, prefix, localname,
 	                             &aprefix, &attvalue, &len, &alloc);
-	if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr)) {
-	    if ((attvalue != NULL) && (alloc != 0))
-	        xmlFree(attvalue);
-	    attvalue = NULL;
-	    goto base_changed;
-	}
-        if ((attname != NULL) && (attvalue != NULL)) {
-	    if (len < 0) len = xmlStrlen(attvalue);
-            if ((attname == ctxt->str_xmlns) && (aprefix == NULL)) {
-	        const xmlChar *URL = xmlDictLookup(ctxt->dict, attvalue, len);
-		xmlURIPtr uri;
-
-                if (URL == NULL) {
-		    xmlErrMemory(ctxt, "dictionary allocation failure");
-		    if ((attvalue != NULL) && (alloc != 0))
-			xmlFree(attvalue);
-		    return(NULL);
-		}
-                if (*URL != 0) {
-		    uri = xmlParseURI((const char *) URL);
-		    if (uri == NULL) {
-			xmlNsErr(ctxt, XML_WAR_NS_URI,
-			         "xmlns: '%s' is not a valid URI\n",
-					   URL, NULL, NULL);
-		    } else {
-			if (uri->scheme == NULL) {
-			    xmlNsWarn(ctxt, XML_WAR_NS_URI_RELATIVE,
-				      "xmlns: URI %s is not absolute\n",
-				      URL, NULL, NULL);
-			}
-			xmlFreeURI(uri);
-		    }
-		    if (URL == ctxt->str_xml_ns) {
-			if (attname != ctxt->str_xml) {
-			    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			 "xml namespace URI cannot be the default namespace\n",
-				     NULL, NULL, NULL);
-			}
-			goto skip_default_ns;
-		    }
-		    if ((len == 29) &&
-			(xmlStrEqual(URL,
-				 BAD_CAST "http://www.w3.org/2000/xmlns/"))) {
-			xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			     "reuse of the xmlns namespace name is forbidden\n",
-				 NULL, NULL, NULL);
-			goto skip_default_ns;
-		    }
-		}
-		/*
-		 * check that it's not a defined namespace
-		 */
-		for (j = 1;j <= nbNs;j++)
-		    if (ctxt->nsTab[ctxt->nsNr - 2 * j] == NULL)
-			break;
-		if (j <= nbNs)
-		    xmlErrAttributeDup(ctxt, NULL, attname);
-		else
-		    if (nsPush(ctxt, NULL, URL) > 0) nbNs++;
-skip_default_ns:
-		if ((attvalue != NULL) && (alloc != 0)) {
-		    xmlFree(attvalue);
-		    attvalue = NULL;
-		}
-		if ((RAW == '>') || (((RAW == '/') && (NXT(1) == '>'))))
-		    break;
-		if (!IS_BLANK_CH(RAW)) {
-		    xmlFatalErrMsg(ctxt, XML_ERR_SPACE_REQUIRED,
-				   "attributes construct error\n");
-		    break;
-		}
-		SKIP_BLANKS;
-		if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr))
-		    goto base_changed;
-		continue;
-	    }
-            if (aprefix == ctxt->str_xmlns) {
-	        const xmlChar *URL = xmlDictLookup(ctxt->dict, attvalue, len);
-		xmlURIPtr uri;
-
-                if (attname == ctxt->str_xml) {
-		    if (URL != ctxt->str_xml_ns) {
-		        xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			         "xml namespace prefix mapped to wrong URI\n",
-			         NULL, NULL, NULL);
-		    }
-		    /*
-		     * Do not keep a namespace definition node
-		     */
-		    goto skip_ns;
-		}
+        if ((attname == NULL) || (attvalue == NULL))
+            goto next_attr;
+	if (len < 0) len = xmlStrlen(attvalue);
+
+        if ((attname == ctxt->str_xmlns) && (aprefix == NULL)) {
+            const xmlChar *URL = xmlDictLookup(ctxt->dict, attvalue, len);
+            xmlURIPtr uri;
+
+            if (URL == NULL) {
+                xmlErrMemory(ctxt, "dictionary allocation failure");
+                if ((attvalue != NULL) && (alloc != 0))
+                    xmlFree(attvalue);
+                return(NULL);
+            }
+            if (*URL != 0) {
+                uri = xmlParseURI((const char *) URL);
+                if (uri == NULL) {
+                    xmlNsErr(ctxt, XML_WAR_NS_URI,
+                             "xmlns: '%s' is not a valid URI\n",
+                                       URL, NULL, NULL);
+                } else {
+                    if (uri->scheme == NULL) {
+                        xmlNsWarn(ctxt, XML_WAR_NS_URI_RELATIVE,
+                                  "xmlns: URI %s is not absolute\n",
+                                  URL, NULL, NULL);
+                    }
+                    xmlFreeURI(uri);
+                }
                 if (URL == ctxt->str_xml_ns) {
-		    if (attname != ctxt->str_xml) {
-		        xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			         "xml namespace URI mapped to wrong prefix\n",
-			         NULL, NULL, NULL);
-		    }
-		    goto skip_ns;
-		}
-                if (attname == ctxt->str_xmlns) {
-		    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			     "redefinition of the xmlns prefix is forbidden\n",
-			     NULL, NULL, NULL);
-		    goto skip_ns;
-		}
-		if ((len == 29) &&
-		    (xmlStrEqual(URL,
-		                 BAD_CAST "http://www.w3.org/2000/xmlns/"))) {
-		    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-			     "reuse of the xmlns namespace name is forbidden\n",
-			     NULL, NULL, NULL);
-		    goto skip_ns;
-		}
-		if ((URL == NULL) || (URL[0] == 0)) {
-		    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
-		             "xmlns:%s: Empty XML namespace is not allowed\n",
-			          attname, NULL, NULL);
-		    goto skip_ns;
-		} else {
-		    uri = xmlParseURI((const char *) URL);
-		    if (uri == NULL) {
-			xmlNsErr(ctxt, XML_WAR_NS_URI,
-			     "xmlns:%s: '%s' is not a valid URI\n",
-					   attname, URL, NULL);
-		    } else {
-			if ((ctxt->pedantic) && (uri->scheme == NULL)) {
-			    xmlNsWarn(ctxt, XML_WAR_NS_URI_RELATIVE,
-				      "xmlns:%s: URI %s is not absolute\n",
-				      attname, URL, NULL);
-			}
-			xmlFreeURI(uri);
-		    }
-		}
+                    if (attname != ctxt->str_xml) {
+                        xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                     "xml namespace URI cannot be the default namespace\n",
+                                 NULL, NULL, NULL);
+                    }
+                    goto next_attr;
+                }
+                if ((len == 29) &&
+                    (xmlStrEqual(URL,
+                             BAD_CAST "http://www.w3.org/2000/xmlns/"))) {
+                    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                         "reuse of the xmlns namespace name is forbidden\n",
+                             NULL, NULL, NULL);
+                    goto next_attr;
+                }
+            }
+            /*
+             * check that it's not a defined namespace
+             */
+            for (j = 1;j <= nbNs;j++)
+                if (ctxt->nsTab[ctxt->nsNr - 2 * j] == NULL)
+                    break;
+            if (j <= nbNs)
+                xmlErrAttributeDup(ctxt, NULL, attname);
+            else
+                if (nsPush(ctxt, NULL, URL) > 0) nbNs++;
+
+        } else if (aprefix == ctxt->str_xmlns) {
+            const xmlChar *URL = xmlDictLookup(ctxt->dict, attvalue, len);
+            xmlURIPtr uri;
+
+            if (attname == ctxt->str_xml) {
+                if (URL != ctxt->str_xml_ns) {
+                    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                             "xml namespace prefix mapped to wrong URI\n",
+                             NULL, NULL, NULL);
+                }
+                /*
+                 * Do not keep a namespace definition node
+                 */
+                goto next_attr;
+            }
+            if (URL == ctxt->str_xml_ns) {
+                if (attname != ctxt->str_xml) {
+                    xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                             "xml namespace URI mapped to wrong prefix\n",
+                             NULL, NULL, NULL);
+                }
+                goto next_attr;
+            }
+            if (attname == ctxt->str_xmlns) {
+                xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                         "redefinition of the xmlns prefix is forbidden\n",
+                         NULL, NULL, NULL);
+                goto next_attr;
+            }
+            if ((len == 29) &&
+                (xmlStrEqual(URL,
+                             BAD_CAST "http://www.w3.org/2000/xmlns/"))) {
+                xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                         "reuse of the xmlns namespace name is forbidden\n",
+                         NULL, NULL, NULL);
+                goto next_attr;
+            }
+            if ((URL == NULL) || (URL[0] == 0)) {
+                xmlNsErr(ctxt, XML_NS_ERR_XML_NAMESPACE,
+                         "xmlns:%s: Empty XML namespace is not allowed\n",
+                              attname, NULL, NULL);
+                goto next_attr;
+            } else {
+                uri = xmlParseURI((const char *) URL);
+                if (uri == NULL) {
+                    xmlNsErr(ctxt, XML_WAR_NS_URI,
+                         "xmlns:%s: '%s' is not a valid URI\n",
+                                       attname, URL, NULL);
+                } else {
+                    if ((ctxt->pedantic) && (uri->scheme == NULL)) {
+                        xmlNsWarn(ctxt, XML_WAR_NS_URI_RELATIVE,
+                                  "xmlns:%s: URI %s is not absolute\n",
+                                  attname, URL, NULL);
+                    }
+                    xmlFreeURI(uri);
+                }
+            }
 
-		/*
-		 * check that it's not a defined namespace
-		 */
-		for (j = 1;j <= nbNs;j++)
-		    if (ctxt->nsTab[ctxt->nsNr - 2 * j] == attname)
-			break;
-		if (j <= nbNs)
-		    xmlErrAttributeDup(ctxt, aprefix, attname);
-		else
-		    if (nsPush(ctxt, attname, URL) > 0) nbNs++;
-skip_ns:
-		if ((attvalue != NULL) && (alloc != 0)) {
-		    xmlFree(attvalue);
-		    attvalue = NULL;
-		}
-		if ((RAW == '>') || (((RAW == '/') && (NXT(1) == '>'))))
-		    break;
-		if (!IS_BLANK_CH(RAW)) {
-		    xmlFatalErrMsg(ctxt, XML_ERR_SPACE_REQUIRED,
-				   "attributes construct error\n");
-		    break;
-		}
-		SKIP_BLANKS;
-		if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr))
-		    goto base_changed;
-		continue;
-	    }
+            /*
+             * check that it's not a defined namespace
+             */
+            for (j = 1;j <= nbNs;j++)
+                if (ctxt->nsTab[ctxt->nsNr - 2 * j] == attname)
+                    break;
+            if (j <= nbNs)
+                xmlErrAttributeDup(ctxt, aprefix, attname);
+            else
+                if (nsPush(ctxt, attname, URL) > 0) nbNs++;
 
-	    /*
-	     * Add the pair to atts
-	     */
-	    if ((atts == NULL) || (nbatts + 5 > maxatts)) {
-	        if (xmlCtxtGrowAttrs(ctxt, nbatts + 5) < 0) {
-		    if (attvalue[len] == 0)
-			xmlFree(attvalue);
-		    goto failed;
-		}
-	        maxatts = ctxt->maxatts;
-		atts = ctxt->atts;
-	    }
-	    ctxt->attallocs[nratts++] = alloc;
-	    atts[nbatts++] = attname;
-	    atts[nbatts++] = aprefix;
-	    atts[nbatts++] = NULL; /* the URI will be fetched later */
-	    atts[nbatts++] = attvalue;
-	    attvalue += len;
-	    atts[nbatts++] = attvalue;
-	    /*
-	     * tag if some deallocation is needed
-	     */
-	    if (alloc != 0) attval = 1;
-	} else {
-	    if ((attvalue != NULL) && (attvalue[len] == 0))
-		xmlFree(attvalue);
-	}
+        } else {
+            /*
+             * Add the pair to atts
+             */
+            if ((atts == NULL) || (nbatts + 5 > maxatts)) {
+                if (xmlCtxtGrowAttrs(ctxt, nbatts + 5) < 0) {
+                    goto next_attr;
+                }
+                maxatts = ctxt->maxatts;
+                atts = ctxt->atts;
+            }
+            ctxt->attallocs[nratts++] = alloc;
+            atts[nbatts++] = attname;
+            atts[nbatts++] = aprefix;
+            /*
+             * The namespace URI field is used temporarily to point at the
+             * base of the current input buffer for non-alloced attributes.
+             * When the input buffer is reallocated, all the pointers become
+             * invalid, but they can be reconstructed later.
+             */
+            if (alloc)
+                atts[nbatts++] = NULL;
+            else
+                atts[nbatts++] = ctxt->input->base;
+            atts[nbatts++] = attvalue;
+            attvalue += len;
+            atts[nbatts++] = attvalue;
+            /*
+             * tag if some deallocation is needed
+             */
+            if (alloc != 0) attval = 1;
+            attvalue = NULL; /* moved into atts */
+        }
 
-failed:
+next_attr:
+        if ((attvalue != NULL) && (alloc != 0)) {
+            xmlFree(attvalue);
+            attvalue = NULL;
+        }
 
 	GROW
         if (ctxt->instate == XML_PARSER_EOF)
             break;
-	if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr))
-	    goto base_changed;
 	if ((RAW == '>') || (((RAW == '/') && (NXT(1) == '>'))))
 	    break;
 	if (!IS_BLANK_CH(RAW)) {
@@ -9679,8 +9634,20 @@
 	    break;
 	}
         GROW;
-	if ((ctxt->input->base != base) || (inputNr != ctxt->inputNr))
-	    goto base_changed;
+    }
+
+    /* Reconstruct attribute value pointers. */
+    for (i = 0, j = 0; j < nratts; i += 5, j++) {
+        if (atts[i+2] != NULL) {
+            /*
+             * Arithmetic on dangling pointers is technically undefined
+             * behavior, but well...
+             */
+            ptrdiff_t offset = ctxt->input->base - atts[i+2];
+            atts[i+2]  = NULL;    /* Reset repurposed namespace URI */
+            atts[i+3] += offset;  /* value */
+            atts[i+4] += offset;  /* valuend */
+        }
     }
 
     /*
@@ -9837,34 +9804,6 @@
     }
 
     return(localname);
-
-base_changed:
-    /*
-     * the attribute strings are valid iif the base didn't changed
-     */
-    if (attval != 0) {
-	for (i = 3,j = 0; j < nratts;i += 5,j++)
-	    if ((ctxt->attallocs[j] != 0) && (atts[i] != NULL))
-	        xmlFree((xmlChar *) atts[i]);
-    }
-
-    /*
-     * We can't switch from one entity to another in the middle
-     * of a start tag
-     */
-    if (inputNr != ctxt->inputNr) {
-        xmlFatalErrMsg(ctxt, XML_ERR_ENTITY_BOUNDARY,
-		    "Start tag doesn't start and stop in the same entity\n");
-	return(NULL);
-    }
-
-    ctxt->input->cur = ctxt->input->base + cur;
-    ctxt->input->line = oldline;
-    ctxt->input->col = oldcol;
-    if (ctxt->wellFormed == 1) {
-	goto reparse;
-    }
-    return(NULL);
 }
 
 /**
@@ -15383,10 +15322,6 @@
 	ctxt->options |= XML_PARSE_NONET;
         options -= XML_PARSE_NONET;
     }
-    if (options & XML_PARSE_NOXXE) {
-	ctxt->options |= XML_PARSE_NOXXE;
-        options -= XML_PARSE_NOXXE;
-    }
     if (options & XML_PARSE_COMPACT) {
 	ctxt->options |= XML_PARSE_COMPACT;
         options -= XML_PARSE_COMPACT;
