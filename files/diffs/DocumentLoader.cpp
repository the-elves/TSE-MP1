--- files/before/DocumentLoader.cpp	2020-12-03 05:32:41.080731382 +0530
+++ files/after/DocumentLoader.cpp	2020-12-03 05:32:42.104895127 +0530
@@ -773,14 +773,15 @@
   redirect_chain_.push_back(url);
 }
 
-void DocumentLoader::DetachFromFrame() {
-  DCHECK(frame_);
-
-  // It never makes sense to have a document loader that is detached from its
-  // frame have any loads active, so go ahead and kill all the loads.
+void DocumentLoader::StopLoading() {
   fetcher_->StopFetching();
   if (frame_ && !SentDidFinishLoad())
     LoadFailed(ResourceError::CancelledError(Url()));
+}
+
+void DocumentLoader::DetachFromFrame() {
+  DCHECK(frame_);
+  StopLoading();
   fetcher_->ClearContext();
 
   // If that load cancellation triggered another detach, leave.
