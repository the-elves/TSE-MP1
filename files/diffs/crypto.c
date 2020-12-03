--- files/before/crypto.c	2020-12-03 05:28:56.552833258 +0530
+++ files/after/crypto.c	2020-12-03 05:28:57.432973977 +0530
@@ -526,6 +526,93 @@
 	}
 }
 
+static int ceph_aes_crypt(const struct ceph_crypto_key *key, bool encrypt,
+			  void *buf, int buf_len, int in_len, int *pout_len)
+{
+	struct crypto_skcipher *tfm = ceph_crypto_alloc_cipher();
+	SKCIPHER_REQUEST_ON_STACK(req, tfm);
+	struct sg_table sgt;
+	struct scatterlist prealloc_sg;
+	char iv[AES_BLOCK_SIZE];
+	int pad_byte = AES_BLOCK_SIZE - (in_len & (AES_BLOCK_SIZE - 1));
+	int crypt_len = encrypt ? in_len + pad_byte : in_len;
+	int ret;
+
+	if (IS_ERR(tfm))
+		return PTR_ERR(tfm);
+
+	WARN_ON(crypt_len > buf_len);
+	if (encrypt)
+		memset(buf + in_len, pad_byte, pad_byte);
+	ret = setup_sgtable(&sgt, &prealloc_sg, buf, crypt_len);
+	if (ret)
+		goto out_tfm;
+
+	crypto_skcipher_setkey((void *)tfm, key->key, key->len);
+	memcpy(iv, aes_iv, AES_BLOCK_SIZE);
+
+	skcipher_request_set_tfm(req, tfm);
+	skcipher_request_set_callback(req, 0, NULL, NULL);
+	skcipher_request_set_crypt(req, sgt.sgl, sgt.sgl, crypt_len, iv);
+
+	/*
+	print_hex_dump(KERN_ERR, "key: ", DUMP_PREFIX_NONE, 16, 1,
+		       key->key, key->len, 1);
+	print_hex_dump(KERN_ERR, " in: ", DUMP_PREFIX_NONE, 16, 1,
+		       buf, crypt_len, 1);
+	*/
+	if (encrypt)
+		ret = crypto_skcipher_encrypt(req);
+	else
+		ret = crypto_skcipher_decrypt(req);
+	skcipher_request_zero(req);
+	if (ret) {
+		pr_err("%s %scrypt failed: %d\n", __func__,
+		       encrypt ? "en" : "de", ret);
+		goto out_sgt;
+	}
+	/*
+	print_hex_dump(KERN_ERR, "out: ", DUMP_PREFIX_NONE, 16, 1,
+		       buf, crypt_len, 1);
+	*/
+
+	if (encrypt) {
+		*pout_len = crypt_len;
+	} else {
+		pad_byte = *(char *)(buf + in_len - 1);
+		if (pad_byte > 0 && pad_byte <= AES_BLOCK_SIZE &&
+		    in_len >= pad_byte) {
+			*pout_len = in_len - pad_byte;
+		} else {
+			pr_err("%s got bad padding %d on in_len %d\n",
+			       __func__, pad_byte, in_len);
+			ret = -EPERM;
+			goto out_sgt;
+		}
+	}
+
+out_sgt:
+	teardown_sgtable(&sgt);
+out_tfm:
+	crypto_free_skcipher(tfm);
+	return ret;
+}
+
+int ceph_crypt(const struct ceph_crypto_key *key, bool encrypt,
+	       void *buf, int buf_len, int in_len, int *pout_len)
+{
+	switch (key->type) {
+	case CEPH_CRYPTO_NONE:
+		*pout_len = in_len;
+		return 0;
+	case CEPH_CRYPTO_AES:
+		return ceph_aes_crypt(key, encrypt, buf, buf_len, in_len,
+				      pout_len);
+	default:
+		return -ENOTSUPP;
+	}
+}
+
 static int ceph_key_preparse(struct key_preparsed_payload *prep)
 {
 	struct ceph_crypto_key *ckey;
