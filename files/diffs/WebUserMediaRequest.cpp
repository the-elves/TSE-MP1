--- files/before/WebUserMediaRequest.cpp	2020-12-03 05:31:46.311974805 +0530
+++ files/after/WebUserMediaRequest.cpp	2020-12-03 05:31:47.308134074 +0530
@@ -42,9 +42,9 @@
 #include "WebSecurityOrigin.h"
 #include "core/dom/Document.h"
 #include "core/platform/mediastream/MediaStreamDescriptor.h"
-#include "core/platform/mediastream/MediaStreamSource.h"
 #include "modules/mediastream/UserMediaRequest.h"
 #include "platform/mediastream/MediaConstraints.h"
+#include "platform/mediastream/MediaStreamSource.h"
 #include "weborigin/SecurityOrigin.h"
 
 using namespace WebCore;
