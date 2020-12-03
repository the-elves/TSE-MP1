--- files/before/CCThreadProxy.cpp	2020-12-03 05:30:20.538261072 +0530
+++ files/after/CCThreadProxy.cpp	2020-12-03 05:30:21.522418420 +0530
@@ -32,6 +32,7 @@
 #include "cc/CCLayerTreeHost.h"
 #include "cc/CCMainThreadTask.h"
 #include "cc/CCScheduler.h"
+#include "cc/CCScopedMainThreadProxy.h"
 #include "cc/CCScrollController.h"
 #include "cc/CCThreadTask.h"
 #include <wtf/CurrentTime.h>
@@ -61,7 +62,7 @@
 
     virtual void scheduleBeginFrameAndCommit()
     {
-        CCMainThread::postTask(m_proxy->createBeginFrameAndCommitTaskOnCCThread());
+        m_proxy->postBeginFrameAndCommitOnCCThread();
     }
 
     virtual void scheduleDrawAndPresent()
@@ -114,6 +115,7 @@
     , m_started(false)
     , m_lastExecutedBeginFrameAndCommitSequenceNumber(-1)
     , m_numBeginFrameAndCommitsIssuedOnCCThread(0)
+    , m_mainThreadProxy(CCScopedMainThreadProxy::create())
 {
     TRACE_EVENT("CCThreadProxy::CCThreadProxy", this, 0);
     ASSERT(isMainThread());
@@ -299,6 +301,8 @@
     s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::layerTreeHostClosedOnCCThread, AllowCrossThreadAccess(&completion)));
     completion.wait();
 
+    m_mainThreadProxy->shutdown(); // Stop running tasks posted to us.
+
     ASSERT(!m_layerTreeHostImpl); // verify that the impl deleted.
     m_layerTreeHost = 0;
     m_started = false;
@@ -317,6 +321,11 @@
     completion->signal();
 }
 
+void CCThreadProxy::postBeginFrameAndCommitOnCCThread()
+{
+    m_mainThreadProxy->postTask(createBeginFrameAndCommitTaskOnCCThread());
+}
+
 void CCThreadProxy::obtainBeginFrameAndCommitTaskFromCCThread(CCCompletionEvent* completion, CCMainThread::Task** taskPtr)
 {
     OwnPtr<CCMainThread::Task> task = createBeginFrameAndCommitTaskOnCCThread();
