--- files/before/regparse.c	2020-12-03 05:28:13.769993017 +0530
+++ files/after/regparse.c	2020-12-03 05:28:14.782154844 +0530
@@ -3020,7 +3020,7 @@
         PUNFETCH;
         prev = p;
         num = scan_unsigned_octal_number(&p, end, 3, enc);
-        if (num < 0) return ONIGERR_TOO_BIG_NUMBER;
+        if (num < 0 || num >= 256) return ONIGERR_TOO_BIG_NUMBER;
         if (p == prev) {  /* can't read nothing. */
           num = 0; /* but, it's not error */
         }
@@ -3392,7 +3392,7 @@
       if (IS_SYNTAX_OP(syn, ONIG_SYN_OP_ESC_OCTAL3)) {
         prev = p;
         num = scan_unsigned_octal_number(&p, end, (c == '0' ? 2:3), enc);
-        if (num < 0) return ONIGERR_TOO_BIG_NUMBER;
+        if (num < 0 || num >= 256) return ONIGERR_TOO_BIG_NUMBER;
         if (p == prev) {  /* can't read nothing. */
           num = 0; /* but, it's not error */
         }
