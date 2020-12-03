--- files/before/FrameLoader.cpp	2020-12-03 05:32:43.113056315 +0530
+++ files/after/FrameLoader.cpp	2020-12-03 05:32:44.133219422 +0530
@@ -988,7 +988,7 @@
   if (in_stop_all_loaders_)
     return;
 
-  in_stop_all_loaders_ = true;
+  AutoReset<bool> in_stop_all_loaders(&in_stop_all_loaders_, true);
 
   for (Frame* child = frame_->Tree().FirstChild(); child;
        child = child->Tree().NextSibling()) {
@@ -998,21 +998,11 @@
 
   frame_->GetDocument()->CancelParsing();
   if (document_loader_)
-    document_loader_->Fetcher()->StopFetching();
+    document_loader_->StopLoading();
   if (!protect_provisional_loader_)
     DetachDocumentLoader(provisional_document_loader_);
-
   frame_->GetNavigationScheduler().Cancel();
-
-  // It's possible that the above actions won't have stopped loading if load
-  // completion had been blocked on parsing or if we were in the middle of
-  // committing an empty document. In that case, emulate a failed navigation.
-  if (document_loader_ && !document_loader_->SentDidFinishLoad()) {
-    document_loader_->LoadFailed(
-        ResourceError::CancelledError(document_loader_->Url()));
-  }
-
-  in_stop_all_loaders_ = false;
+  DidFinishNavigation();
 
   TakeObjectSnapshot();
 }
