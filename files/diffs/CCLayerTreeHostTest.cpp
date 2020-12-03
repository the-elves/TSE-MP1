--- files/before/CCLayerTreeHostTest.cpp	2020-12-03 05:30:22.454567455 +0530
+++ files/after/CCLayerTreeHostTest.cpp	2020-12-03 05:30:23.382715850 +0530
@@ -24,13 +24,12 @@
 
 #include "config.h"
 
-#if USE(THREADED_COMPOSITING)
-
 #include "cc/CCLayerTreeHost.h"
 
 #include "cc/CCLayerImpl.h"
 #include "cc/CCLayerTreeHostImpl.h"
 #include "cc/CCMainThreadTask.h"
+#include "cc/CCScopedMainThreadProxy.h"
 #include "cc/CCThreadTask.h"
 #include "GraphicsContext3DPrivate.h"
 #include <gtest/gtest.h>
@@ -170,9 +169,7 @@
     {
     }
 
-#if !USE(THREADED_COMPOSITING)
     virtual void scheduleComposite() { }
-#endif
 
 private:
     explicit MockLayerTreeHostClient(TestHooks* testHooks) : m_testHooks(testHooks) { }
@@ -207,20 +204,31 @@
         callOnMainThread(CCLayerTreeHostTest::dispatchSetNeedsRedraw, this);
     }
 
+    void timeout()
+    {
+        m_timedOut = true;
+        endTest();
+    }
+
+    void clearTimeout()
+    {
+        m_timeoutTask = 0;
+    }
+
+
 protected:
     CCLayerTreeHostTest()
         : m_beginning(false)
         , m_endWhenBeginReturns(false)
-        , m_running(false)
         , m_timedOut(false)
     {
         m_webThread = adoptPtr(webKitPlatformSupport()->createThread("CCLayerTreeHostTest"));
         WebCompositor::setThread(m_webThread.get());
-#if USE(THREADED_COMPOSITING)
-        m_settings.enableCompositorThread = true;
-#else
-        m_settings.enableCompositorThread = false;
+#ifndef NDEBUG
+        CCProxy::setMainThread(currentThread());
 #endif
+        ASSERT(CCProxy::isMainThread());
+        m_mainThreadProxy = CCScopedMainThreadProxy::create();
     }
 
     void doBeginTest();
@@ -234,6 +242,7 @@
     {
         ASSERT(isMainThread());
         webkit_support::QuitMessageLoop();
+        webkit_support::RunAllPendingMessages();
         CCLayerTreeHostTest* test = static_cast<CCLayerTreeHostTest*>(self);
         ASSERT(test);
         test->m_layerTreeHost.clear();
@@ -257,32 +266,55 @@
           test->m_layerTreeHost->setNeedsRedraw();
     }
 
-    virtual void runTest()
+    class TimeoutTask : public webkit_support::TaskAdaptor {
+    public:
+        explicit TimeoutTask(CCLayerTreeHostTest* test)
+            : m_test(test)
+        {
+        }
+
+        void clearTest()
+        {
+            m_test = 0;
+        }
+
+        virtual ~TimeoutTask()
+        {
+            if (m_test)
+                m_test->clearTimeout();
+        }
+
+        virtual void Run()
+        {
+            if (m_test)
+                m_test->timeout();
+        }
+
+    private:
+        CCLayerTreeHostTest* m_test;
+    };
+
+    virtual void runTest(bool threaded)
     {
+        m_settings.enableCompositorThread = threaded;
         webkit_support::PostDelayedTask(CCLayerTreeHostTest::onBeginTest, static_cast<void*>(this), 0);
-        webkit_support::PostDelayedTask(CCLayerTreeHostTest::testTimeout, static_cast<void*>(this), 5000);
+        m_timeoutTask = new TimeoutTask(this);
+        webkit_support::PostDelayedTask(m_timeoutTask, 5000); // webkit_support takes ownership of the task
         webkit_support::RunMessageLoop();
-        m_running = false;
-        bool timedOut = m_timedOut; // Save whether we're timed out in case RunAllPendingMessages has the timeout.
         webkit_support::RunAllPendingMessages();
+
+        if (m_timeoutTask)
+            m_timeoutTask->clearTest();
+
         ASSERT(!m_layerTreeHost.get());
         m_client.clear();
-        if (timedOut) {
+        if (m_timedOut) {
             FAIL() << "Test timed out";
             return;
         }
         afterTest();
     }
 
-    static void testTimeout(void* self)
-    {
-        CCLayerTreeHostTest* test = static_cast<CCLayerTreeHostTest*>(self);
-        if (!test->m_running)
-            return;
-        test->m_timedOut = true;
-        test->endTest();
-    }
-
     CCSettings m_settings;
     OwnPtr<MockLayerTreeHostClient> m_client;
     RefPtr<CCLayerTreeHost> m_layerTreeHost;
@@ -290,17 +322,16 @@
 private:
     bool m_beginning;
     bool m_endWhenBeginReturns;
-    bool m_running;
     bool m_timedOut;
 
     OwnPtr<WebThread> m_webThread;
+    RefPtr<CCScopedMainThreadProxy> m_mainThreadProxy;
+    TimeoutTask* m_timeoutTask;
 };
 
 void CCLayerTreeHostTest::doBeginTest()
 {
     ASSERT(isMainThread());
-    ASSERT(!m_running);
-    m_running = true;
     m_client = MockLayerTreeHostClient::create(this);
 
     RefPtr<LayerChromium> rootLayer = LayerChromium::create(0);
@@ -318,7 +349,7 @@
 {
     // If we are called from the CCThread, re-call endTest on the main thread.
     if (!isMainThread())
-        CCMainThread::postTask(createMainThreadTask(this, &CCLayerTreeHostTest::endTest));
+        m_mainThreadProxy->postTask(createMainThreadTask(this, &CCLayerTreeHostTest::endTest));
     else {
         // For the case where we endTest during beginTest(), set a flag to indicate that
         // the test should end the second beginTest regains control.
@@ -331,10 +362,9 @@
 
 class CCLayerTreeHostTestThreadOnly : public CCLayerTreeHostTest {
 public:
-    virtual void runTest()
+    void runTest()
     {
-        if (m_settings.enableCompositorThread)
-            CCLayerTreeHostTest::runTest();
+        CCLayerTreeHostTest::runTest(true);
     }
 };
 
@@ -352,10 +382,18 @@
     {
     }
 };
-TEST_F(CCLayerTreeHostTestShortlived1, run)
-{
-    runTest();
-}
+
+#define SINGLE_AND_MULTI_THREAD_TEST_F(TEST_FIXTURE_NAME) \
+    TEST_F(TEST_FIXTURE_NAME, runSingleThread)            \
+    {                                                     \
+        runTest(false);                                   \
+    }                                                     \
+    TEST_F(TEST_FIXTURE_NAME, runMultiThread)             \
+    {                                                     \
+        runTest(true);                                    \
+    }
+
+SINGLE_AND_MULTI_THREAD_TEST_F(CCLayerTreeHostTestShortlived1)
 
 // Shortlived layerTreeHosts shouldn't die with a commit in flight.
 class CCLayerTreeHostTestShortlived2 : public CCLayerTreeHostTest {
@@ -372,10 +410,8 @@
     {
     }
 };
-TEST_F(CCLayerTreeHostTestShortlived2, run)
-{
-    runTest();
-}
+
+SINGLE_AND_MULTI_THREAD_TEST_F(CCLayerTreeHostTestShortlived2)
 
 // Shortlived layerTreeHosts shouldn't die with a redraw in flight.
 class CCLayerTreeHostTestShortlived3 : public CCLayerTreeHostTest {
@@ -392,10 +428,8 @@
     {
     }
 };
-TEST_F(CCLayerTreeHostTestShortlived3, run)
-{
-    runTest();
-}
+
+SINGLE_AND_MULTI_THREAD_TEST_F(CCLayerTreeHostTestShortlived3)
 
 // Constantly redrawing layerTreeHosts shouldn't die when they commit
 class CCLayerTreeHostTestCommitingWithContinuousRedraw : public CCLayerTreeHostTest {
@@ -435,14 +469,12 @@
     int m_numCompleteCommits;
     int m_numDraws;
 };
-TEST_F(CCLayerTreeHostTestCommitingWithContinuousRedraw, run)
-{
-    runTest();
-}
+
+SINGLE_AND_MULTI_THREAD_TEST_F(CCLayerTreeHostTestCommitingWithContinuousRedraw)
 
 // Two setNeedsCommits in a row should lead to at least 1 commit and at least 1
 // draw with frame 0.
-class CCLayerTreeHostTestSetNeedsCommit1 : public CCLayerTreeHostTest {
+class CCLayerTreeHostTestSetNeedsCommit1 : public CCLayerTreeHostTestThreadOnly {
 public:
     CCLayerTreeHostTestSetNeedsCommit1()
         : m_numCommits(0)
@@ -478,14 +510,15 @@
     int m_numCommits;
     int m_numDraws;
 };
-TEST_F(CCLayerTreeHostTestSetNeedsCommit1, run)
+
+TEST_F(CCLayerTreeHostTestSetNeedsCommit1, runMultiThread)
 {
     runTest();
 }
 
 // A setNeedsCommit should lead to 1 commit. Issuing a second commit after that
 // first committed frame draws should lead to another commit.
-class CCLayerTreeHostTestSetNeedsCommit2 : public CCLayerTreeHostTest {
+class CCLayerTreeHostTestSetNeedsCommit2 : public CCLayerTreeHostTestThreadOnly {
 public:
     CCLayerTreeHostTestSetNeedsCommit2()
         : m_numCommits(0)
@@ -521,14 +554,15 @@
     int m_numCommits;
     int m_numDraws;
 };
-TEST_F(CCLayerTreeHostTestSetNeedsCommit2, run)
+
+TEST_F(CCLayerTreeHostTestSetNeedsCommit2, runMultiThread)
 {
     runTest();
 }
 
 // 1 setNeedsRedraw after the first commit has completed should lead to 1
 // additional draw.
-class CCLayerTreeHostTestSetNeedsRedraw : public CCLayerTreeHostTest {
+class CCLayerTreeHostTestSetNeedsRedraw : public CCLayerTreeHostTestThreadOnly {
 public:
     CCLayerTreeHostTestSetNeedsRedraw()
         : m_numCommits(0)
@@ -553,6 +587,7 @@
 
     virtual void commitCompleteOnCCThread(CCLayerTreeHostImpl*)
     {
+        EXPECT_EQ(0, m_numDraws);
         m_numCommits++;
     }
 
@@ -566,9 +601,9 @@
     int m_numCommits;
     int m_numDraws;
 };
-TEST_F(CCLayerTreeHostTestSetNeedsRedraw, run)
+
+TEST_F(CCLayerTreeHostTestSetNeedsRedraw, runMultiThread)
 {
-    CCSettings setings;
     runTest();
 }
 
@@ -637,7 +672,8 @@
     IntSize m_scrollAmount;
     int m_scrolls;
 };
-TEST_F(CCLayerTreeHostTestScrollSimple, run)
+
+TEST_F(CCLayerTreeHostTestScrollSimple, runMultiThread)
 {
     runTest();
 }
@@ -710,11 +746,11 @@
     IntSize m_scrollAmount;
     int m_scrolls;
 };
-TEST_F(CCLayerTreeHostTestScrollMultipleRedraw, run)
+
+TEST_F(CCLayerTreeHostTestScrollMultipleRedraw, runMultiThread)
 {
     runTest();
 }
 
 } // namespace
 
-#endif
