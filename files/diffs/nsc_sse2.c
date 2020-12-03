--- files/before/nsc_sse2.c	2020-12-03 05:29:22.152926264 +0530
+++ files/after/nsc_sse2.c	2020-12-03 05:29:23.049069542 +0530
@@ -385,7 +385,7 @@
 	}
 }
 
-static void nsc_encode_sse2(NSC_CONTEXT* context, const BYTE* data,
+static BOOL nsc_encode_sse2(NSC_CONTEXT* context, const BYTE* data,
                             UINT32 scanline)
 {
 	nsc_encode_argb_to_aycocg_sse2(context, data, scanline);
@@ -394,6 +394,8 @@
 	{
 		nsc_encode_subsampling_sse2(context);
 	}
+
+	return TRUE;
 }
 
 void nsc_init_sse2(NSC_CONTEXT* context)
