--- files/before/AssertMatchingEnums.cpp	2020-12-03 05:31:44.263647315 +0530
+++ files/after/AssertMatchingEnums.cpp	2020-12-03 05:31:45.267807862 +0530
@@ -78,7 +78,6 @@
 #include "core/platform/Cursor.h"
 #include "core/platform/graphics/MediaSourcePrivate.h"
 #include "core/platform/graphics/filters/FilterOperation.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "core/platform/mediastream/RTCDataChannelHandlerClient.h"
 #include "core/platform/mediastream/RTCPeerConnectionHandlerClient.h"
 #include "core/rendering/CompositingReasons.h"
@@ -100,6 +99,7 @@
 #include "platform/fonts/FontDescription.h"
 #include "platform/fonts/FontSmoothingMode.h"
 #include "platform/graphics/media/MediaPlayer.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "platform/network/ResourceLoadPriority.h"
 #include "platform/network/ResourceResponse.h"
 #include "platform/text/TextChecking.h"
