--- files/before/d1_lib.c	2020-12-03 05:27:34.427702865 +0530
+++ files/after/d1_lib.c	2020-12-03 05:27:35.383855737 +0530
@@ -202,9 +202,12 @@
 
 	while ( (item = pqueue_pop(s->d1->buffered_app_data.q)) != NULL)
 		{
-		frag = (hm_fragment *)item->data;
-		OPENSSL_free(frag->fragment);
-		OPENSSL_free(frag);
+		rdata = (DTLS1_RECORD_DATA *) item->data;
+		if (rdata->rbuf.buf)
+			{
+			OPENSSL_free(rdata->rbuf.buf);
+			}
+		OPENSSL_free(item->data);
 		pitem_free(item);
 		}
 	}
