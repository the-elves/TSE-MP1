--- files/before/ber_dec.cpp	2020-12-03 05:29:00.497463933 +0530
+++ files/after/ber_dec.cpp	2020-12-03 05:29:01.353600814 +0530
@@ -9,6 +9,7 @@
 #include <botan/ber_dec.h>
 #include <botan/bigint.h>
 #include <botan/loadstor.h>
+#include <botan/internal/safeint.h>
 
 namespace Botan {
 
@@ -126,7 +127,9 @@
       size_t item_size = decode_length(&source, length_size);
       source.discard_next(item_size);
 
-      length += item_size + length_size + tag_size;
+      length = BOTAN_CHECKED_ADD(length, item_size);
+      length = BOTAN_CHECKED_ADD(length, tag_size);
+      length = BOTAN_CHECKED_ADD(length, length_size);
 
       if(type_tag == EOC && class_tag == UNIVERSAL)
          break;
