--- files/before/MediaStreamDescriptor.cpp	2020-12-03 05:31:38.106662917 +0530
+++ files/after/MediaStreamDescriptor.cpp	2020-12-03 05:31:39.074817708 +0530
@@ -34,8 +34,8 @@
 #include "core/platform/mediastream/MediaStreamDescriptor.h"
 
 #include "core/platform/mediastream/MediaStreamComponent.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "platform/UUID.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "wtf/RefCounted.h"
 #include "wtf/Vector.h"
 
