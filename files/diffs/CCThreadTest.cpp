--- files/before/CCThreadTest.cpp	2020-12-03 05:30:24.390877038 +0530
+++ files/after/CCThreadTest.cpp	2020-12-03 05:30:25.331027352 +0530
@@ -32,6 +32,7 @@
 #include "WebThread.h"
 #include "cc/CCCompletionEvent.h"
 #include "cc/CCMainThreadTask.h"
+#include "cc/CCScopedMainThreadProxy.h"
 #include "cc/CCThreadTask.h"
 
 #include <gtest/gtest.h>
@@ -71,9 +72,14 @@
 
 class PingPongTestUsingTasks {
 public:
+    PingPongTestUsingTasks()
+        : m_mainThreadProxy(CCScopedMainThreadProxy::create())
+    {
+    }
+
     void ping()
     {
-        CCMainThread::postTask(createMainThreadTask(this, &PingPongTestUsingTasks::pong));
+        m_mainThreadProxy->postTask(createMainThreadTask(this, &PingPongTestUsingTasks::pong));
         hit = true;
     }
 
@@ -84,6 +90,9 @@
     }
 
     bool hit;
+
+private:
+    RefPtr<CCScopedMainThreadProxy> m_mainThreadProxy;
 };
 
 #if OS(WINDOWS) || OS(MAC_OS_X)
