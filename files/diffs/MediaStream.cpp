--- files/before/MediaStream.cpp	2020-12-03 05:31:40.090980175 +0530
+++ files/after/MediaStream.cpp	2020-12-03 05:31:41.267168227 +0530
@@ -30,9 +30,9 @@
 #include "core/events/Event.h"
 #include "core/dom/ExceptionCode.h"
 #include "core/platform/mediastream/MediaStreamCenter.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "modules/mediastream/MediaStreamRegistry.h"
 #include "modules/mediastream/MediaStreamTrackEvent.h"
+#include "platform/mediastream/MediaStreamSource.h"
 
 namespace WebCore {
 
