--- files/before/MediaStreamComponent.cpp	2020-12-03 05:31:36.122345658 +0530
+++ files/after/MediaStreamComponent.cpp	2020-12-03 05:31:37.150510045 +0530
@@ -34,8 +34,8 @@
 #include "core/platform/mediastream/MediaStreamComponent.h"
 
 #include "platform/audio/AudioBus.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "platform/UUID.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "public/platform/WebAudioSourceProvider.h"
 
 namespace WebCore {
