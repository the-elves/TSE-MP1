--- files/before/smbencrypt.c	2020-12-03 05:28:54.800553093 +0530
+++ files/after/smbencrypt.c	2020-12-03 05:28:55.684694451 +0530
@@ -23,7 +23,7 @@
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
-#include <crypto/skcipher.h>
+#include <linux/crypto.h>
 #include <linux/module.h>
 #include <linux/slab.h>
 #include <linux/fs.h>
@@ -69,46 +69,22 @@
 static int
 smbhash(unsigned char *out, const unsigned char *in, unsigned char *key)
 {
-	int rc;
 	unsigned char key2[8];
-	struct crypto_skcipher *tfm_des;
-	struct scatterlist sgin, sgout;
-	struct skcipher_request *req;
+	struct crypto_cipher *tfm_des;
 
 	str_to_key(key, key2);
 
-	tfm_des = crypto_alloc_skcipher("ecb(des)", 0, CRYPTO_ALG_ASYNC);
+	tfm_des = crypto_alloc_cipher("des", 0, 0);
 	if (IS_ERR(tfm_des)) {
-		rc = PTR_ERR(tfm_des);
 		cifs_dbg(VFS, "could not allocate des crypto API\n");
-		goto smbhash_err;
+		return PTR_ERR(tfm_des);
 	}
 
-	req = skcipher_request_alloc(tfm_des, GFP_KERNEL);
-	if (!req) {
-		rc = -ENOMEM;
-		cifs_dbg(VFS, "could not allocate des crypto API\n");
-		goto smbhash_free_skcipher;
-	}
-
-	crypto_skcipher_setkey(tfm_des, key2, 8);
-
-	sg_init_one(&sgin, in, 8);
-	sg_init_one(&sgout, out, 8);
-
-	skcipher_request_set_callback(req, 0, NULL, NULL);
-	skcipher_request_set_crypt(req, &sgin, &sgout, 8, NULL);
-
-	rc = crypto_skcipher_encrypt(req);
-	if (rc)
-		cifs_dbg(VFS, "could not encrypt crypt key rc: %d\n", rc);
-
-	skcipher_request_free(req);
+	crypto_cipher_setkey(tfm_des, key2, 8);
+	crypto_cipher_encrypt_one(tfm_des, out, in);
+	crypto_free_cipher(tfm_des);
 
-smbhash_free_skcipher:
-	crypto_free_skcipher(tfm_des);
-smbhash_err:
-	return rc;
+	return 0;
 }
 
 static int
