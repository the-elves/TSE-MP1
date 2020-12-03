--- files/before/encode_msg.c	2020-12-03 05:28:03.836404808 +0530
+++ files/after/encode_msg.c	2020-12-03 05:28:04.804559599 +0530
@@ -158,6 +158,7 @@
 
    if(len < MAX_ENCODED_MSG + MAX_MESSAGE_LEN)
       return -1;
+
    if(parse_headers(msg,HDR_EOH_F,0)<0){
       myerror="in parse_headers";
       goto error;
@@ -266,6 +267,11 @@
    /*j+=k;*/
    /*pkg_free(payload2);*/
    /*now we copy the actual message after the headers-meta-section*/
+
+	if(len < j + msg->len + 1) {
+   	   LM_ERR("not enough space to encode sip message\n");
+   	   return -1;
+	}
    memcpy(&payload[j],msg->buf,msg->len);
    LM_DBG("msglen = %d,msg starts at %d\n",msg->len,j);
    j=htons(j);
