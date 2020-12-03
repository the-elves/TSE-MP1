--- files/before/WebMediaStream.cpp	2020-12-03 05:31:29.017209671 +0530
+++ files/after/WebMediaStream.cpp	2020-12-03 05:31:30.925514775 +0530
@@ -28,8 +28,8 @@
 
 #include "core/platform/mediastream/MediaStreamComponent.h"
 #include "core/platform/mediastream/MediaStreamDescriptor.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "platform/UUID.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "public/platform/WebMediaStreamSource.h"
 #include "public/platform/WebMediaStreamTrack.h"
 #include "public/platform/WebString.h"
