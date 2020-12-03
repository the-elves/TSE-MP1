--- files/before/WebMediaStreamTrack.cpp	2020-12-03 05:31:32.637788538 +0530
+++ files/after/WebMediaStreamTrack.cpp	2020-12-03 05:31:34.338060382 +0530
@@ -27,7 +27,7 @@
 #include "public/platform/WebMediaStreamTrack.h"
 
 #include "core/platform/mediastream/MediaStreamComponent.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "public/platform/WebAudioSourceProvider.h"
 #include "public/platform/WebMediaStream.h"
 #include "public/platform/WebMediaStreamSource.h"
